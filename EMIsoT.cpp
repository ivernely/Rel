#include "measclasses.h"
#include "eq_control.h"

extern bool abort_meas;
/*=====================================================================================================*/
EM::EM()
{
    Clear();
}
/*=====================================================================================================*/
void EM::Clear()
{  
    forceH.clear();
    forceL.clear();
    senseH.clear();
    senseL.clear();
    ExtMon.clear();

    bHPRegime = false;
    bMeasExt = false;

    MetalType = ALUMINUM;
    TestType = PCONST;
    Number = 0;

    W = 0.0;
    H = 0.0;

    Tstress = 0.0;
    Tref = 0.0;
    TCRref = 0.0;
    StopCond = 0.0;
    Tchuck = 0.0;
    TimeLimit = 0.0;
    Istress = 0.0;
    PulseW = 0.0;

    Rchuck = 0.0;
    TCRchuck = 0.0;

    Itest = 0.0;
    Iuse = 0.0;

    t_stl = 0.0;

    Ea = 0.0;
    n = 0.0;
    A = 0.0;

    Rfail = 0.0;
    TTF = 0.0;
    Iext_fail = 0.0;

    Rstart_stress = 0.0;
    Iext_init = 0.0;
    Iext_start_stress = 0.0;

    P.clear();
    T.clear();
    R.clear();
}
/*=====================================================================================================*/
bool EM::CheckSrc()
{
    if(forceH.Num<1 || forceL.Num<1 || forceH==forceL)
        return false;

    if(bMeasExt && (ExtMon.Num<1 ||
                    forceH==ExtMon ||
                    forceL==ExtMon))
        return false;

    if(!b2point){
        if(senseH.Num<1 ||  senseL.Num<1)
            return false;

        if(senseH==senseL ||
                forceH==senseH ||
                forceH==senseL ||
                forceL==senseH ||
                forceL==senseL)
            return false;

        if(bMeasExt && (ExtMon.Num<1 ||
                        senseH==ExtMon ||
                        senseL==ExtMon))
            return false;
    }

    if(forceH.Model==HPSMU && forceL.Model==GNDSMU)
        bHPRegime = true;

    return true;
}
/*=====================================================================================================*/
void EM::RunTest(const QString &Macro, const int &Die)
{
    QString curr_date;
    QString status;
    QString FileName;
    char str[256];

    curr_date = QDateTime::currentDateTime().toString("hh-mm-ss dd.MM.yyyy");
    status = QString("EM_%1_die%2").arg(Macro).arg(Die);
    FileName = trUtf8("%1%2_%3.txt").arg(Path_to_save_data).arg(status).arg(curr_date);

    file = fopen(FileName.toLocal8Bit().data(), "w+a");

    if(file != NULL){
        emit change_test_status(status);

        sprintf(str, "%s\n", curr_date.toLocal8Bit().data());
        fputs(str, file);
        sprintf(str, "Die,%d\n", Die);
        fputs(str, file);
        sprintf(str, "Macro,%s\n", Macro.toLocal8Bit().data());
        fputs(str, file);

        switch (TestType){
            case PCONST:    EM_Isothermal_test();
                            break;

            case ICONST:    EM_Iconst_Stress();
                            break;

            case SWEAT:     EM_SWEAT();//in progress
                            break;

            case IPULSE:    EM_Ipulse_Stress();
                            break;

            case ICONST_2P: EM_Iconst_2p_Stress();
                            break;

            //case KELVIN:    R_kelvin(Die, Iinit());
            //                break;

            case ISWEEP_2P: Isweep_2p();
                            break;

            //case RMEAS_2P:  R_2points(Die, Iinit());
            //                break;

            default:        break;
        };
    }

    sprintf(str, "Status=%d\n", (int)ret);
    fputs(str, file);
    fclose(file);

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    ret = agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, 0);
}
/*=====================================================================================================*/
double EM::Fcorr(const double &T)
{
    double F;

    F = 1.0;

    if(MetalType==COPPER && T>177)
        F = 1.0167-8.39751E-5*T-3.74768E-8*T*T;

    if(F>1.0)
        F = 1.0;

    return F;
}
/*=====================================================================================================*/
double EM::Fcorr_inv(const double &T)
{
    double F;

    F = 1.0;

    if(MetalType==COPPER && T>177)
        F = 1.01535-7.21714E-5*T-6.53459E-8*T*T;

    if(F>1.0)
        F = 1.0;

    return F;
}
/*=====================================================================================================*/
void EM::SetVForceCompl(const double &Iforce)
{
    if(bHPRegime){
        forceH.Compl = SetHVcompliance(Iforce);
        forceL.Compl = MAX_HI;
    }
    else{
        forceH.Compl = SetVcompliance(Iforce);
        forceL.Compl = MAX_I;
    }
}
/*=====================================================================================================*/
ViStatus EM::SetSources(const ViInt32 &adc_type)
{
    //forceH
    forceH.ForceMode = agb1500_IF_MODE;
    forceH.MeasMode = agb1500_VM_MODE;
    forceH.Range = agb1500_AUTO_RANGE;
    ret = agb1500_setSwitch(vi_b1500, forceH.Slot, ON);
    if(ret != VI_SUCCESS)
        return ret;
    ret = agb1500_setAdcType(vi_b1500, forceH.Slot, adc_type);
    if(ret != VI_SUCCESS)
        return ret;

    //forceL
    forceL.ForceMode = agb1500_VF_MODE;
    forceL.MeasMode = agb1500_IM_MODE;
    forceL.Range = agb1500_AUTO_RANGE;
    if(forceL.Model != GNDSMU){
        ret = agb1500_setSwitch(vi_b1500, forceL.Slot, ON);
        if(ret != VI_SUCCESS)
            return ret;
        ret = agb1500_setAdcType(vi_b1500, forceL.Slot, adc_type);
        if(ret != VI_SUCCESS)
            return ret;
    }

    //senseH
    senseH.ForceMode = agb1500_IF_MODE;
    senseH.MeasMode = agb1500_VM_MODE;
    senseH.Range = agb1500_AUTO_RANGE;
    senseH.Compl = MAX_V;
    ret = agb1500_setSwitch(vi_b1500, senseH.Slot, ON);
    if(ret != VI_SUCCESS)
        return ret;
    ret = agb1500_setAdcType(vi_b1500, senseH.Slot, adc_type);
    if(ret != VI_SUCCESS)
        return ret;

    //senseL
    senseL.ForceMode = agb1500_IF_MODE;
    senseL.MeasMode = agb1500_VM_MODE;
    senseL.Range = agb1500_AUTO_RANGE;
    senseL.Compl = MAX_V;
    ret = agb1500_setSwitch(vi_b1500, senseL.Slot, ON);
    if(ret != VI_SUCCESS)
        return ret;
    ret = agb1500_setAdcType(vi_b1500, senseL.Slot, adc_type);
    if(ret != VI_SUCCESS)
        return ret;

    //ExtMon
    if(bMeasExt){
        ExtMon.ForceMode = agb1500_VF_MODE;
        ExtMon.MeasMode = agb1500_IM_MODE;
        ExtMon.Range = agb1500_AUTO_RANGE;
        ExtMon.Compl = 1E-3;
        ret = agb1500_setSwitch(vi_b1500, ExtMon.Slot, ON);
        if(ret != VI_SUCCESS)
            return ret;
        ret = agb1500_setAdcType(vi_b1500, ExtMon.Slot, adc_type);
        if(ret != VI_SUCCESS)
            return ret;
    }

    //---ADC, Filter, Autozero settings---
    ret = agb1500_setFilter(vi_b1500, agb1500_CH_ALL, agb1500_FLAG_OFF);
    if(ret != VI_SUCCESS)
        return ret;

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);

    return ret;
}
/*=====================================================================================================*/
bool EM::CommMatrix(SMU *f1, SMU *f2, SMU *s1, SMU *s2, SMU *extmon)
{
    if(bUseMatrix){
        OpenCommMatrix(vi_2200a);

        if(f1->Num!=0 && !ConnectPorts2200AVect(vi_2200a, f1->Num, f1->pins))
            return false;

        if(f2->Num!=0 && !ConnectPorts2200AVect(vi_2200a, f2->Num, f2->pins))
            return false;

        if(s1->Num!=0 && !b2point && !ConnectPorts2200AVect(vi_2200a, s1->Num, s1->pins))
            return false;

        if(s2->Num!=0 && !b2point && !ConnectPorts2200AVect(vi_2200a, s2->Num, s2->pins))
            return false;

        if(bMeasExt && extmon->Num!=0 && !ConnectPorts2200AVect(vi_2200a, extmon->Num, extmon->pins))
            return false;
    }

    return true;
}
/*=====================================================================================================*/
double EM::New_I(const double &t_ft, const double &T, const double &Rth, const double &T0, const double &I)
{
    double Ti, t_i;
    double Inew;
    uint cnt;
    double D;

    Inew = I;
    D = MAX_HI - I;

    //Ri = R/(1 + TCRchuck*(T/Fcorr_inv(T) - Tchuck)); //Ri = Rchuck
    Ti = (Rth*I*I*Rchuck*(1-Tchuck*TCRchuck)+T0)/(1-Rth*I*I*Rchuck*TCRchuck/Fcorr_inv(T));
    t_i = A*pow((W*H*1E-8)/I,n)*exp(Ea/(8.617E-5*(Ti+273.16)));

    cnt = 0;
    while(fabs(t_ft-t_i)>BE){

        cnt++;

        if(t_ft > t_i)
            Inew = Inew - D/pow(2,cnt);
        else
            Inew = Inew + D/pow(2,cnt);

        Ti = (Rth*Inew*Inew*Rchuck*(1-Tchuck*TCRchuck)+T0)/(1-Rth*Inew*Inew*Rchuck*TCRchuck/Fcorr_inv(Ti));
        t_i = A*pow((W*H*1E-8)/Inew,n)*exp(Ea/(8.617E-5*(Ti+273.16)));

        if(cnt>100){
            Inew = 7777;
            break;
        }
    }

    return Inew;
}
/*=====================================================================================================*/
double EM::Iinit()
{
    return INIT_CURR * W * H * 1E-6;
}
/*=====================================================================================================*/
void EM::Meas_Resitance(const double &Iforce)
{
    double Rd, Rr, Id, Ir;
    char str[256] = {0};

    ViInt32     mode[3],
                ch[4] = {0};
    ViReal64    rng[3],
                values[3] = {0};

    ch[0] = senseH.Slot;
    ch[1] = senseL.Slot;
    if(bMeasExt)
        ch[2] = ExtMon.Slot;

    SetVForceCompl(Iforce);

    mode[0] = senseH.MeasMode;
    mode[1] = senseL.MeasMode;
    mode[2] = ExtMon.MeasMode;

    rng[0] = senseH.Range;
    rng[1] = senseL.Range;
    rng[2] = ExtMon.Range;

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS)
        return;

    if(fabs(Iforce)>ICOMPL_CARD){
        ret = ERR_COMPL_CARD;
        return;
    }

    CommMatrix(&forceL, &forceH, &senseH, &senseL, &ExtMon);

    //----Start of resistance measurement at chuck temperature----//
    if(forceL.Model != GNDSMU){
        ret = agb1500_force(vi_b1500, forceL.Slot, forceL.ForceMode, forceL.Range, 0.0, forceL.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    ret = agb1500_force(vi_b1500, senseH.Slot, senseH.ForceMode, senseH.Range, 0.0, senseH.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_force(vi_b1500, senseL.Slot, senseL.ForceMode, senseL.Range, 0.0, senseL.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    if(bMeasExt){
        ret = agb1500_force(vi_b1500, ExtMon.Slot, ExtMon.ForceMode, ExtMon.Range, 0.0, ExtMon.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    ret = agb1500_force(vi_b1500, forceH.Slot, forceH.ForceMode, forceH.Range, Iforce, forceH.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0],  NULL,  NULL);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS)
        return;

    Rr = (values[0] - values[1])/Iforce;
    Ir = values[2];

    fputs("\t\t==========Reverse mode==========\n", file);
    sprintf(str, "%g,%e,%e,%e,%e,%e,\n", t_stl+TTF, Iforce, values[0], values[1], Ir, Rr);
    fputs(str, file);

    CommMatrix(&forceH, &forceL, &senseH, &senseL, &ExtMon);

    if(forceL.Model != GNDSMU){
        ret = agb1500_force(vi_b1500, forceL.Slot, forceL.ForceMode, forceL.Range, 0.0, forceL.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    ret = agb1500_force(vi_b1500, senseH.Slot, senseH.ForceMode, senseH.Range, 0.0, senseH.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_force(vi_b1500, senseL.Slot, senseL.ForceMode, senseL.Range, 0.0, senseL.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    if(bMeasExt){
        ret = agb1500_force(vi_b1500, ExtMon.Slot, ExtMon.ForceMode, ExtMon.Range, 0.0, ExtMon.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    ret = agb1500_force(vi_b1500, forceH.Slot, forceH.ForceMode, forceH.Range, Iforce, forceH.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0],  NULL,  NULL);
    if(ret!=VI_SUCCESS)
        return;

    Rd = (values[0] - values[1])/Iforce;
    Id = values[2];

    Rchuck = (Rr + Rd)/2.0;
    Iext_init = (Ir + Id)/2.0;

    fputs("\t\t==========Direct mode==========\n", file);
    sprintf(str, "%g,%e,%e,%e,%e,%e,\n", t_stl+TTF, Iforce, values[0], values[1], Id, Rd);
    fputs(str, file);

    emit change_EMRvalue(QString("%1").arg(Rchuck));
    emit change_EMIvalue(QString("%1").arg(Iext_init));
}
/*=====================================================================================================*/
void EM::Meas_2p_Resitance(const double &Iforce)
{
    double Rd, Rr, Id, Ir;
    char str[256] = {0};

    ViInt32     mode[3],
                ch[4] = {0};
    ViReal64    rng[3],
                values[3] = {0};

    SetVForceCompl(Iforce);

    ch[0] = forceH.Slot;
    ch[1] = forceL.Slot;
    if(bMeasExt)
        ch[2] = ExtMon.Slot;

    mode[0] = forceH.MeasMode;
    mode[1] = forceL.MeasMode;
    mode[2] = ExtMon.MeasMode;

    rng[0] = forceH.Range;
    rng[1] = forceL.Range;
    rng[2] = ExtMon.Range;

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS)
        return;

    if(fabs(Iforce)>ICOMPL_CARD){
        ret = ERR_COMPL_CARD;
        return;
    }

    CommMatrix(&forceL, &forceH, &senseH, &senseL, &ExtMon);

    //----Start of resistance measurement at chuck temperature----//
    if(forceL.Model != GNDSMU){
        ret = agb1500_force(vi_b1500, forceL.Slot, forceL.ForceMode, forceL.Range, 0.0, forceL.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    if(bMeasExt){
        ret = agb1500_force(vi_b1500, ExtMon.Slot, ExtMon.ForceMode, ExtMon.Range, 0.0, ExtMon.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    ret = agb1500_force(vi_b1500, forceH.Slot, forceH.ForceMode, forceH.Range, Iforce, forceH.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0],  NULL,  NULL);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS)
        return;

    Rr = fabs(values[0]/values[1]);
    Ir = values[2];

    fputs("\t\t==========Reverse mode==========\n", file);
    sprintf(str, "%g,%e,%e,%e,%e,\n", t_stl+TTF, fabs(values[1]), values[0], Ir, Rr);
    fputs(str, file);

    CommMatrix(&forceH, &forceL, &senseH, &senseL, &ExtMon);

    if(forceL.Model != GNDSMU){
        ret = agb1500_force(vi_b1500, forceL.Slot, forceL.ForceMode, forceL.Range, 0.0, forceL.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    if(bMeasExt){
        ret = agb1500_force(vi_b1500, ExtMon.Slot, ExtMon.ForceMode, ExtMon.Range, 0.0, ExtMon.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    ret = agb1500_force(vi_b1500, forceH.Slot, forceH.ForceMode, forceH.Range, Iforce, forceH.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0],  NULL,  NULL);
    if(ret!=VI_SUCCESS)
        return;

    Rd = fabs(values[0]/values[1]);
    Id = values[2];

    Rchuck = (Rr + Rd)/2.0;
    Iext_init = (Ir + Id)/2.0;

    fputs("\t\t==========Direct mode==========\n", file);
    sprintf(str, "%g,%e,%e,%e,%e,\n", t_stl+TTF, fabs(values[1]), values[0], Id, Rd);
    fputs(str, file);

    emit change_EMRvalue(QString("%1").arg(Rchuck));
    emit change_EMIvalue(QString("%1").arg(Iext_init));
}
/*=====================================================================================================*/
void EM::Meas_Extrusion_Leakage(const double &Vforce)
{
    char str[512] = {0};

    double  If1_to_ext,
            If2_to_ext;

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, 0);
    if(ret!=VI_SUCCESS)
        return;

    //Sense1 to extrusion monitor
    ret = agb1500_setSwitch(vi_b1500, senseH.Slot, ON);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_setSwitch(vi_b1500, ExtMon.Slot, ON);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_force(vi_b1500, senseH.Slot, agb1500_VF_MODE, agb1500_AUTO_RANGE, Vforce, MAX_I, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_force(vi_b1500, ExtMon.Slot, agb1500_VF_MODE, agb1500_AUTO_RANGE, 0.0, MAX_I, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_spotMeas(vi_b1500, ExtMon.Slot,  agb1500_IM_MODE, agb1500_AUTO_RANGE, &If1_to_ext, NULL, NULL);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_setSwitch(vi_b1500, senseH.Slot, OFF);
    if(ret!=VI_SUCCESS){
        ret = ERR_SWITCH_OFF;
        return;
    }

    //Sense2 to extrusion monitor
    ret = agb1500_setSwitch(vi_b1500, senseL.Slot, ON);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_force(vi_b1500, senseL.Slot, agb1500_VF_MODE, agb1500_AUTO_RANGE, Vforce, MAX_I, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_force(vi_b1500, ExtMon.Slot, agb1500_VF_MODE, agb1500_AUTO_RANGE, 0.0, MAX_I, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_spotMeas(vi_b1500, ExtMon.Slot,  agb1500_IM_MODE, agb1500_AUTO_RANGE, &If2_to_ext, NULL, NULL);
    if(ret!=VI_SUCCESS)
        return;
    //----End of leakage measurement----//

    sprintf(str, "\n\nLeakage Sense1 to ExtMon,%e\n", If1_to_ext);
    fputs(str, file);
    sprintf(str, "Leakage Sense2 to ExtMon,%e\n", If2_to_ext);
    fputs(str, file);
}
/*=====================================================================================================*/
void EM::EM_Isothermal_test()
{
    char str[1024] = {0};

    int     i,
            start,
            end;

    double  I_init,
            Icurr;

    double  Rmeas[RAMP_STEPS],
            Imeas[RAMP_STEPS],
            Tmeas[RAMP_STEPS];

    double  Ti_1,
            Pi_1,
            Rth,
            T0,
            Tstaircase,
            dP,
            Pi,
            Ti,
            Ri,
            Iforce,
            Tcal,
            R_1,
            Pth,
            R0,
            Ptest,
            Tcal_prev;

    ViInt32     mode[3],
                ch[4];
    ViReal64    rng[3],
                values[3] = {0},
                time[3];

    I_init = Iinit();
    TCRchuck = TCRref/(1 + TCRref*(Tchuck - Tref));

    if(fabs(I_init)>ICOMPL_CARD){
        ret = ERR_COMPL_CARD;
        return;
    }

    SetVForceCompl(I_init);

    if(abort_meas){
        ret = ERR_ABORT;
        return;
    }

    ret = SetSources(agb1500_HSPEED_ADC);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_setAdc(vi_b1500, agb1500_HSPEED_ADC, agb1500_INTEG_PLC, 1, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS)
        return;

    fputs("--- EM P=const test ---\n", file);
    sprintf(str, "%s\n", comment.toLocal8Bit().data());
    fputs(str, file);
    sprintf(str, "Date,%s\n", QDateTime::currentDateTime().toString("hh-mm-ss dd.MM.yyyy").toLocal8Bit().data());
    fputs(str, file);
    sprintf(str, "W,%f,um\nH,%f,um\nTstress,%f,deg C,\nTCR,%f,1/degC\n", W, H, Tstress, TCRref);
    fputs(str, file);
    sprintf(str, "Tref,%f,degC\nStopCondition,%f,%c,\nTroom,%f,degC,\n", Tref, StopCond*100.0, '%', Tchuck);
    fputs(str, file);
    sprintf(str, "MaxStressTime,%f,s\n", TimeLimit);
    fputs(str, file);

    if(MetalType == ALUMINUM)
        fputs("Material,Al\n", file);
    else
        if(MetalType == COPPER)
            fputs("Material,Cu\n", file);
        else
            fputs("Material,NoName\n", file);

    sprintf(str, "Iinit,%e,A\nTCRchuck,%e,1/degC\n", I_init, TCRchuck);
    fputs(str, file);

    fputs("\t\t==========Measurement of chuck resistance==========\n", file);
    fputs("Time,Iforce1,Vsense1,Vsense2,Iext,Rmeas,Theat\n", file);
    fputs("s,A,V,V,A,Ohm,degC\n", file);

    ch[0] = senseH.Slot;
    ch[1] = senseL.Slot;
    if(bMeasExt)
        ch[2] = ExtMon.Slot;
    ch[3] = 0;

    mode[0] = senseH.MeasMode;
    mode[1] = senseL.MeasMode;
    mode[2] = ExtMon.MeasMode;

    rng[0] = senseH.Range;
    rng[1] = senseL.Range;
    rng[2] = ExtMon.Range;

    if(abort_meas){
        ret = ERR_ABORT;
        return;
    }

    Meas_Resitance(I_init);

    if(Rchuck > Rchuck*(1 + StopCond) || Iext_init>=MIN_I_LEAK){
        ret = ERR_STOP_COND;
        return;
    }

    emit change_EMRvalue(QString("%1").arg(Rchuck));
    emit change_EMIvalue(QString("%1").arg(Iext_init/1E-9));
    //----End of resistance measurement at chuck temperature----//


    //----------------------------------------------------------------------------------------------------------
    //                          Нагрев на 50 градусов выше температуры столика (initialization)
    //----------------------------------------------------------------------------------------------------------

    fputs("\t\t==========Initialization phase==========\n", file);

    Icurr = I_init;
    ret = agb1500_resetTimestamp(vi_b1500);
    if(ret!=VI_SUCCESS){
        ret = ERR_TIMESTAMP;
        return;
    }

    for(i=0; i<RAMP_STEPS; i++)
    {
        Icurr = Icurr*F_CURRENT;

        if(fabs(Icurr)>ICOMPL_CARD){
            ret = ERR_COMPL_CARD;
            return;
        }

        if(bHPRegime){
            forceH.Compl = SetHVcompliance(Icurr);

            if(fabs(Icurr)>MAX_HI || fabs(Icurr)<MIN_IFORCE){
                ret = ERR_MAXCURRENT;
                return;
            }
        }
        else{
            forceH.Compl = SetVcompliance(Icurr);

            if(fabs(Icurr)>MAX_I || fabs(Icurr)<MIN_IFORCE){
                ret = ERR_MAXCURRENT;
                return;
            }
        }

        ret = agb1500_force(vi_b1500, forceH.Slot, forceH.ForceMode, forceH.Range, Icurr, forceH.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;

        QThread::msleep(EM_TIME_STEP);

        ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0],  NULL,  &time[0]);
        if(ret!=VI_SUCCESS)
            return;

        Rmeas[i] = (values[0]-values[1])/Icurr;
        Imeas[i] = Icurr;

        Tmeas[i] = Tchuck + (Rmeas[i] - Rchuck)/(Rchuck*TCRchuck);
        Tmeas[i] = Tmeas[i]*Fcorr(Tmeas[i]);

        sprintf(str, "%e,%e,%e,%e,%e,%e,%f\n", time[0], Icurr, values[0], values[1], values[2], Rmeas[i], Tmeas[i]);
        fputs(str, file);

        emit change_EMRvalue(QString("%1").arg(Rmeas[i]));
        emit change_EMIvalue(QString("%1").arg(values[2]/1E-9));
        emit change_test_time(time[0]);

        if(fabs(Tmeas[i]-Tchuck)>=50.0 || Tmeas[i]>Tstress)
            break;

        if(abort_meas){
            ret = ERR_ABORT;
            return;
        }

    }//end for(i)
    t_stl = time[0];

    if(i<STEPS-1){
        ret = ERR_APPROX;
        return;
    }

    i = i-STEPS+1;
    for(int j=0; j<STEPS; j++){
        R.push_back(Rmeas[i+j]);
        P.push_back(Rmeas[i+j] * Imeas[i+j] * Imeas[i+j]);
        T.push_back(Tmeas[i+j]);
    }


    Ti_1 = T.at(STEPS-1);
    Pi_1 = P.at(STEPS-1);

    if(!Approx(P, T, 0, STEPS, Rth, T0)){
        ret = ERR_APPROX;
        return;
    }

    //----------------------------------------------------------------------------------------------------------
    //                          Нагрев до требуемой температуры испытаний (staircase)
    //----------------------------------------------------------------------------------------------------------

    fputs("\t\t==========Temperature staircase==========\n", file);
    Tstaircase = Tstress - F_POWER*T_STEP;

    if(abort_meas)
        return;

    ret = agb1500_resetTimestamp(vi_b1500);
    if(ret!=VI_SUCCESS)
        return;

    do{
        if(abort_meas)
            return;

        if(Ti_1 >= Tstaircase){
            dP = (Tstress + BE/2.0 - Ti_1)/Rth;
            Pi = Pi_1 + dP/F_POWER;
            Ti = T0 + Rth*Pi;
        }//end convergence phase
        else{
            Ti = Ti_1 + T_STEP;
            Pi = (Ti - T0)/Rth;
        }//end staircase phase

        Ri = Rchuck*(1 + TCRchuck*(Ti/Fcorr_inv(Ti) - Tchuck));
        Iforce = sqrt(Pi/Ri);

        if(Iforce>ICOMPL_CARD){
            ret = ERR_COMPL_CARD;
            return;
        }

        SetVForceCompl(Iforce);

        ret = agb1500_force(vi_b1500, forceH.Slot, forceH.ForceMode, forceH.Range, Iforce, forceH.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;

        QThread::msleep(EM_TIME_STEP);
        ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0], NULL, &time[0]);
        if(ret!=VI_SUCCESS)
            return;

        Rmeas[0] = (values[0] - values[1])/Iforce;
        R.push_back(Rmeas[0]);
        P.push_back(Rmeas[0] * Iforce * Iforce);
        Tcal = Tchuck + (Rmeas[0] - Rchuck)/(Rchuck * TCRchuck);
        Tcal = Tcal * Fcorr(Tcal);
        T.push_back(Tcal);

        sprintf(str, "%e,%e,%e,%e,%e,%e,%f\n", t_stl+time[0], Iforce, values[0], values[1], values[2], Rmeas[0], Tcal);
        fputs(str, file);

        emit change_EMRvalue(QString("%1").arg(Rmeas[0]));
        emit change_EMIvalue(QString("%1").arg(values[2]/1E-9));
        emit change_test_time(t_stl+time[0]);

        if(R.size() > 2)
            R_1 = R.at(R.size()-2);
            if(fabs(R_1-Rmeas[0]) >= R_1*StopCond || fabs(values[2])>MIN_I_LEAK){
                ret = ERR_STOP_COND;
                return;
            }

        start = 0;
        end = T.size();
        for(i=0; i<end; i++)
           if(Tcal-T[i]<50){
               start = i;
               break;
           }

        if(!Approx(P, T, start, end, Rth, T0)){
            ret = ERR_APPROX;
            return;
        }

        if(!Approx(P, R, start, end, Pth, R0)){
            ret = ERR_APPROX;
            return;
        }

        Ti_1 = T[end-1];
        Pi_1 = P[end-1];
    }
    while((Tcal+BE/2.0) < Tstress);

    Itest = Iforce;
    t_stl = t_stl + time[0];
    //-------------End of temperature staircase phase----------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------------------
    //                          Изотермические испытания (isothermal)
    //----------------------------------------------------------------------------------------------------------
    Ptest = Rmeas[0] * Itest * Itest;

    fputs("\t\t==========Isothermal phase==========\n", file);
    ret = agb1500_resetTimestamp(vi_b1500);
    if(ret!=VI_SUCCESS)
        return;

    i = 0;
    Tcal_prev = Tstress;
    do{
        if(abort_meas)
            return;

        Ri = Rmeas[0] + Pth*fabs(Ptest - Pi_1);
        Iforce = sqrt(Ptest/Ri);
        Tcal = T0 + Rth*Pi_1;

        if(fabs(Iforce)>ICOMPL_CARD){
            ret = ERR_COMPL_CARD;
            break;
        }

        if(fabs(Tcal-Tcal_prev)>10.0){
            ret = ERR_TEMPER;
            break;
        }

        if(bHPRegime)
            forceH.Compl = SetHVcompliance(Iforce);

        else
            forceH.Compl = SetVcompliance(Iforce);

        ret = agb1500_force(vi_b1500, forceH.Slot, forceH.ForceMode, forceH.Range, Iforce, forceH.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            break;

        QThread::msleep(EM_TIME_STEP);

        ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0],  NULL,  &time[0]);
        if(ret!=VI_SUCCESS){
            ret = ERR_READ_STATUS;
            break;
        }

        Rmeas[0] = (values[0] - values[1])/Iforce;
        Pi_1 = (values[0] - values[1]) * Iforce;

        Rfail = Rmeas[0];
        TTF = time[0];
        Iext_fail = values[2];

        sprintf(str, "%e,%e,%e,%e,%e,%e,%f\n", t_stl + time[0], Iforce, values[0], values[1], values[2], Rmeas[0], Tcal);
        fputs(str, file);

        emit change_EMRvalue(QString("%1").arg(Rmeas[0]));
        emit change_EMIvalue(QString("%1").arg(values[2]/1E-9));
        emit change_test_time(t_stl+time[0]);

        if(i == 0){
            Rstart_stress = Rmeas[0];
            Iext_start_stress = values[2];
        }

        Tcal_prev = Tcal;

        i++;

    }while(fabs(Rstart_stress-Rmeas[0])<Rstart_stress*StopCond && time[0]<TimeLimit && fabs(values[2])<MIN_I_LEAK);
    //-------------End of isothermal phase-------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------------------
    //                          Финальные измерения (final)
    //----------------------------------------------------------------------------------------------------------

    fputs("\t\t==========Final phase==========\n", file);
    Meas_Resitance(I_init);
    agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, 0);

    Rmeas[0] = (values[0] - values[1])/I_init;
    sprintf(str, "%e,%e,%e,%e,%e,%e,\n", time[0] + t_stl + TTF, I_init, values[0], values[1], values[2], Rmeas[0]);
    fputs(str, file);

    fputs("\n---Data analysis---\n", file);
    if(bMeasExt)
        Meas_Extrusion_Leakage(VLEAK);

    sprintf(str, "\nTTF,%e\nTstl,%e\n", TTF, t_stl);
    fputs(str, file);

    sprintf(str, "Rinit,%e\nRend,%e\n", Rchuck, Rmeas[0]);
    fputs(str, file);

    sprintf(str, "Rstart_stress,%e\nRfinal_stress,%e\n", Rstart_stress, Rfail);
    fputs(str, file);

    sprintf(str, "Iext_init,%e\nIext_end,%e\n", Iext_init, values[2]);
    fputs(str, file);

    sprintf(str, "\nIext_start_stress,%e\nIext_final_stress,%e\n", Iext_start_stress, Iext_fail);
    fputs(str, file);
}
/*=====================================================================================================*/
void EM::EM_Iconst_Stress()
{
    char str[1024] = {0};
    bool intrpt;

    double  Rmeas,
            I_init;

    ViInt32     mode[3],
                ch[4],
                status[3];

    ViReal64    rng[3],
                values[3] = {0},
                time[3];

    ch[0] = senseH.Slot;
    ch[1] = senseL.Slot;
    if(bMeasExt)
        ch[2] = ExtMon.Slot;
    ch[3] = 0;

    ret = SetSources(agb1500_HSPEED_ADC);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_setAdc(vi_b1500, agb1500_HSPEED_ADC, agb1500_INTEG_PLC, 1, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS)
        return;

    mode[0] = senseH.MeasMode;
    mode[1] = senseL.MeasMode;
    mode[2] = ExtMon.MeasMode;

    rng[0] = senseH.Range;
    rng[1] = senseL.Range;
    rng[2] = ExtMon.Range;

    memset(status, 0, 3*sizeof(ViInt32));

    fputs("--- EM I=const test ---\n", file);
    sprintf(str, "Date,%s\n", QDateTime::currentDateTime().toString("hh-mm-ss dd.MM.yyyy").toLocal8Bit().data());
    fputs(str, file);
    sprintf(str, "%s\n", comment.toLocal8Bit().data());
    fputs(str, file);
    sprintf(str, "W,%f,um\nH,%f,um\nTstress,%f,deg C,\n", W, H, Tstress);
    fputs(str, file);
    sprintf(str, "Istress,%f,mA\nStopCondition,%f,%c,\n", Istress*1000.0, StopCond*100.0, '%');
    fputs(str, file);

    sprintf(str, "MaxStressTime,%f,s\n", TimeLimit);
    fputs(str, file);

    if(MetalType == ALUMINUM)
        fputs("Material,Al\n", file);
    else
        if(MetalType == COPPER)
            fputs("Material,Cu\n", file);
        else
            fputs("Material,NoName\n", file);

    if(Istress>ICOMPL_CARD){
        ret = ERR_COMPL_CARD;
        return;
    }

    if(abort_meas){
        ret = ERR_ABORT;
        return;
    }

    fputs("Time,Iforce,Vsense1,Vsense2,Iext,Rmeas\n", file);
    fputs("s,A,V,V,A,Ohm\n", file);

    //--------------------------Start of initial stress----------------------------------------
    I_init = Iinit();

    Meas_Resitance(I_init);
    //--------------------------End of initial stress------------------------------------------


    //--------------------------Start of I=const stress----------------------------------------
    fputs("\t\t========== I=const stress ==========\n", file);
    SetVForceCompl(Istress);

    ret = agb1500_force(vi_b1500, forceH.Slot, agb1500_IF_MODE, agb1500_AUTO_RANGE, Istress, forceH.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_resetTimestamp(vi_b1500);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0],  NULL,  &time[0]);
    if(ret!=VI_SUCCESS)
        return;

    Rmeas = (values[0] - values[1])/Istress;

    sprintf(str, "%e,%e,%e,%e,%e,%e\n", time[0], Istress, values[0], values[1], values[2], Rmeas);
    fputs(str, file);

    Rstart_stress = Rmeas;

    emit change_EMRvalue(QString("%1").arg(Rmeas));
    emit change_EMIvalue(QString("%1").arg(values[2]/1E-9));
    emit change_test_time(time[0]);

    SetVForceCompl(fabs(values[0]) * (1 + StopCond));

    ret = agb1500_force(vi_b1500, forceH.Slot, agb1500_IF_MODE, agb1500_AUTO_RANGE, Istress, forceH.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS){
        ret = ERR_FORCE;
        return;
    }

    do{
        if(abort_meas){
            ret = ERR_ABORT;
            return;
        }

        ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0],  &status[0],  &time[0]);
        if(ret!=VI_SUCCESS){
            intrpt = true;
            for(int i=0; i<3; i++)
                if(status[i]&8 || status[i]&4){
                    intrpt = false;
                    break;
                }

            if(intrpt)
                return;
            else
                break;
        }


        Rmeas = (values[0] - values[1])/Istress;

        sprintf(str, "%e,%e,%e,%e,%e,%e\n", time[0], Istress, values[0], values[1], values[2], Rmeas);
        fputs(str, file);

        emit change_EMRvalue(QString("%1").arg(Rmeas));
        emit change_EMIvalue(QString("%1").arg(values[2]/1E-9));
        emit change_test_time(time[0]);

        if(fabs(Rstart_stress-Rmeas)<Rstart_stress*StopCond){
            TTF = time[0];
            Iext_fail = values[2];
        }

        QThread::msleep(INTERVAL);

        emit change_test_time(time[0]);
        Rfail = Rmeas;
    }
    while(fabs(Rstart_stress-Rmeas)<Rstart_stress*StopCond && time[0]<TimeLimit && fabs(values[2])<MIN_I_LEAK);

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS)
        return;
    //--------------------------End of I=const stress-------------------------------------

    if(abort_meas){
        ret = ERR_ABORT;
        return;
    }

    //--------------------------Start of final stress-------------------------------------
    fputs("\t\t==========Measurement of final resistance==========\n", file);
    Meas_Resitance(I_init);
    //--------------------------End of final stress---------------------------------------


    fputs("\n---Data analysis---\n", file);
    if(bMeasExt)
        Meas_Extrusion_Leakage(VLEAK);

    sprintf(str, "\nTTF,%e\nRfail,%e\nIext_fail,%e\n", TTF, Rfail, Iext_fail);
    fputs(str, file);
    sprintf(str, "Rinit,%e\nRend,%e\n", Rchuck, Rmeas);
    fputs(str, file);
}
/*=====================================================================================================*/
void EM::EM_Ipulse_Stress()
{
    char str[1024] = {0};

    double  PulseW_test,
            Period,
            Rmeas,
            v,
            curr_test_time,
            TTFmeas = -7777.0,
            Iext = -7777.0;

    ViInt32     mode[3],
                ch[4],
                status[3];

    ViReal64    rng[3],
                values[3] = {0},
                time[3];

    memset(status, 0, 3*sizeof(ViInt32));

    fputs("--- EM I=pulse test ---\n", file);
    sprintf(str, "Date,%s\n", QDateTime::currentDateTime().toString("hh-mm-ss dd.MM.yyyy").toLocal8Bit().data());
    fputs(str, file);
    sprintf(str, "%s\n", comment.toLocal8Bit().data());
    fputs(str, file);
    sprintf(str, "W,%f,um\nH,%f,um\nTstress,%f,deg C,\n", W, H, Tstress);
    fputs(str, file);
    sprintf(str, "Istress,%f,mA\nStopCondition,%f,%c,\n", Istress*1000.0, StopCond*100.0, '%');
    fputs(str, file);
    sprintf(str, "Iuse,%f,uA\nPulseWidth,%f,ms,\n", Iuse*1E+6, PulseW);
    fputs(str, file);
    sprintf(str, "MaxStressTime,%f,s\n", TimeLimit);
    fputs(str, file);

    if(MetalType == ALUMINUM)
        fputs("Material,Al\n", file);
    else
        if(MetalType == COPPER)
            fputs("Material,Cu\n", file);
        else
            fputs("Material,NoName\n", file);

    ch[0] = senseH.Slot;
    ch[1] = senseL.Slot;
    if(bMeasExt)
        ch[2] = ExtMon.Slot;
    ch[3] = 0;

    ret = SetSources(agb1500_HSPEED_ADC);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_setAdc(vi_b1500, agb1500_HSPEED_ADC, agb1500_INTEG_PLC, 1, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS)
        return;

    mode[0] = senseH.MeasMode;
    mode[1] = senseL.MeasMode;
    mode[2] = ExtMon.MeasMode;

    rng[0] = senseH.Range;
    rng[1] = senseL.Range;
    rng[2] = ExtMon.Range;

    PulseW_test = PulseW*1.0E-3;
    if(PulseW > 100.0)
        Period = PulseW_test + 0.01;
    else
        Period = PulseW_test + 0.002;

    if(Period<0.005)
        Period = 0.005;

    if(Istress>ICOMPL_CARD){
        ret = ERR_COMPL_CARD;
        return;
    }

    if(abort_meas){
        ret = ERR_ABORT;
        return;
    }

    fputs("Time,Iforce1,Vsense1,Vsense2,Iext,Ruse\n", file);
    fputs("s,A,V,V,A,Ohm\n", file);

    //--------------------------Start of initial R meas----------------------------------------
    Meas_Resitance(Iuse);
    //--------------------------End of initial R meas------------------------------------------

    //--------------------------Start of Ipulse stress----------------------------------------
    fputs("\t\t========== Ipulse stress ==========\n", file);
    SetVForceCompl(Rchuck * (1 + StopCond) * Istress);

    emit change_EMRvalue(QString("%1").arg(Rchuck));
    emit change_EMIvalue(QString("%1").arg(values[2]/1E-9));

    curr_test_time = 0.0;

    ret = agb1500_resetTimestamp(vi_b1500);
    if(ret!=VI_SUCCESS)
        return;

    do{
        if(abort_meas){
            ret = ERR_ABORT;
            return;
        }

        ret = agb1500_setPbias(vi_b1500, forceH.Slot,  forceH.ForceMode, forceH.Range, Iuse, Istress,
                                                                            PulseW_test, Period, 0.0, forceH.Compl);
        if(ret!=VI_SUCCESS)
            return;

        ret = agb1500_measureP(vi_b1500, forceH.Slot, forceH.MeasMode, agb1500_FIX_100V_RANGE, &v, NULL, &time[0]);
        if(ret!=VI_SUCCESS)
            return;

        ret = agb1500_force(vi_b1500, forceH.Slot, forceH.ForceMode, forceH.Range, Iuse, forceH.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;

        ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0],  &status[0],  NULL);
        if(ret!=VI_SUCCESS)
            return;

        Rmeas = (values[0] - values[1])/Iuse;

        sprintf(str, "%e,%e,%e,%e,%e,%e\n", curr_test_time, Iuse, values[0], values[1], values[2], Rmeas);
        fputs(str, file);

        emit change_EMRvalue(QString("%1").arg(Rmeas));
        emit change_EMIvalue(QString("%1").arg(values[2]/1E-9));
        emit change_test_time(curr_test_time);

        if(fabs(Rchuck-Rmeas)<Rchuck*StopCond){
            TTFmeas = time[0];
            Iext = values[2];
        }

        curr_test_time = curr_test_time + PulseW_test;
        emit change_test_time(curr_test_time);
        TTF = TTFmeas;
        Rfail = Rmeas;
        Iext_fail = Iext;
    }
    while(fabs(Rchuck-Rmeas)<Rchuck*StopCond && curr_test_time<TimeLimit && fabs(Iext)<MIN_I_LEAK);


    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS)
        return;
    //--------------------------End of Ipulse stress-------------------------------------

    //--------------------------Start of final stress-------------------------------------

    if(abort_meas){
        ret = ERR_ABORT;
        return;
    }

    Meas_Resitance(Iuse);
    //--------------------------End of final stress---------------------------------------

    fputs("\n---Data analysis---\n", file);
    if(bMeasExt)
        Meas_Extrusion_Leakage(VLEAK);

    sprintf(str, "\nTTF,%e\nRfail,%e\nIext_fail,%e\n", TTF, Rfail, Iext_fail);
    fputs(str, file);
}
/*=====================================================================================================*/
void EM::EM_Iconst_2p_Stress()
{
    char str[1024] = {0};
    bool intrpt;

    double  I_init,
            Rmeas,
            Iext = -7777.0;

    ViInt32     mode[3],
                ch[4] = {0},
                status[3];

    ViReal64    rng[3],
                values[3] = {0},
                time[3];

    memset(status, 0, 3*sizeof(ViInt32));

    fputs("--- EM I=const 2point test ---\n", file);
    sprintf(str, "Date,%s\n", QDateTime::currentDateTime().toString("hh-mm-ss dd.MM.yyyy").toLocal8Bit().data());
    fputs(str, file);
    sprintf(str, "%s\n", comment.toLocal8Bit().data());
    fputs(str, file);
    sprintf(str, "W,%f,um\nH,%f,um\nTstress,%f,deg C,\n", W, H, Tstress);
    fputs(str, file);
    sprintf(str, "Istress,%f,mA\nStopCondition,%f,%c,\n", Istress*1000.0, StopCond*100.0, '%');
    fputs(str, file);

    sprintf(str, "MaxStressTime,%f,s\n", TimeLimit);
    fputs(str, file);

    if(MetalType == ALUMINUM)
        fputs("Material,Al\n", file);
    else
        if(MetalType == COPPER)
            fputs("Material,Cu\n", file);
        else
            fputs("Material,NoName\n", file);

    fputs("Time,Iforce,Vsense,Iext,Rmeas\n", file);
    fputs("s,A,V,A,Ohm\n", file);

    ret = SetSources(agb1500_HSPEED_ADC);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_setAdc(vi_b1500, agb1500_HSPEED_ADC, agb1500_INTEG_MANUAL, 5, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS)
        return;

    ch[0] = forceH.Slot;
    ch[1] = forceL.Slot;
    if(bMeasExt)
        ch[2] = ExtMon.Slot;

    if(abort_meas){
        emit finished_test();
        return;
    }

    if(fabs(Istress)>ICOMPL_CARD){
        ret = ERR_COMPL_CARD;
        emit finished_test();
        return;
    }

    //--------------------------Start of initial stress----------------------------------------
    I_init = Iinit();

    fputs("\t\t==========Measurement of initial resistance==========\n", file);
    Meas_2p_Resitance(I_init);
    //--------------------------End of initial stress------------------------------------------

    mode[0] = forceH.MeasMode;
    mode[1] = forceL.MeasMode;
    mode[2] = ExtMon.MeasMode;

    rng[0] = forceH.Range;
    rng[1] = forceL.Range;
    rng[2] = ExtMon.Range;

    if(fabs(Istress)>ICOMPL_CARD){
        ret = ERR_COMPL_CARD;
        emit finished_test();
        return;
    }

    fputs("\t\t========== I=const stress ==========\n", file);
    SetVForceCompl(Istress);
    //-----------------------------------------------------------------------------------------
    //--------------------------Start of I=const stress----------------------------------------
    //-----------------------------------------------------------------------------------------
    ret = agb1500_force(vi_b1500, forceH.Slot, forceH.ForceMode, forceH.Range, Istress, forceH.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS){
        emit finished_test();
        return;
    }

    ret = agb1500_resetTimestamp(vi_b1500);
    if(ret!=VI_SUCCESS){
        emit finished_test();
        return;
    }

    ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0],  NULL,  &time[0]);
    if(ret!=VI_SUCCESS){
        emit finished_test();
        return;
    }

    Rmeas = values[0]/fabs(values[1]);
    sprintf(str, "%e,%e,%e,%e,%e\n", time[0], fabs(values[1]), values[0], values[2], Rmeas);
    fputs(str, file);

    Rstart_stress = Rmeas;
    SetVForceCompl(fabs(values[0]) * (1+StopCond));

    emit change_EMRvalue(QString("%1").arg(Rmeas));
    emit change_EMIvalue(QString("%1").arg(values[2]/1E-9));
    emit change_test_time(time[0]);

    ret = agb1500_force(vi_b1500, forceH.Slot, forceH.ForceMode, forceH.Range, Istress, forceH.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS){
        emit finished_test();
        return;
    }

    do{
        if(abort_meas){
            emit finished_test();
            return;
        }

        ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0],  &status[0],  &time[0]);
        if(ret!=VI_SUCCESS){
            intrpt = true;
            for(int i=0; i<3; i++)
                if(status[i]&8 || status[i]&4){
                    intrpt = false;
                    break;
                }

            if(intrpt){
                emit finished_test();
                return;
            }
            else
                break;
        }

        Rmeas = values[0]/fabs(values[1]);
        sprintf(str, "%e,%e,%e,%e,%e\n", time[0], fabs(values[1]), values[0], values[2], Rmeas);
        fputs(str, file);

        emit change_EMRvalue(QString("%1").arg(Rmeas));
        emit change_EMIvalue(QString("%1").arg(values[2]/1E-9));
        emit change_test_time(time[0]);

        if(fabs(Rstart_stress-Rmeas)<Rstart_stress*StopCond){
            TTF = time[0];
            Iext_fail = values[2];
            Rfail = Rmeas;
        }

        QThread::msleep(INTERVAL);

        emit change_test_time(time[0]);
    }
    while(fabs(Rstart_stress-Rmeas)<Rstart_stress*StopCond && time[0]<TimeLimit && fabs(Iext)<MIN_I_LEAK);

    ret = agb1500_zeroOutput(vi_b1500, forceH.Slot);
    if(ret!=VI_SUCCESS){
        emit finished_test();
        return;
    }
    //--------------------------End of I=const stress-------------------------------------

    fputs("\t\t==========Measurement of final resistance==========\n", file);
    Meas_2p_Resitance(I_init);


    fputs("\n---Data analysis---\n", file);
    sprintf(str, "\nTTF,%e\nRfail,%e\nIext_fail,%e\n", TTF, Rfail, Iext_fail);
    fputs(str, file);
}
/*=====================================================================================================*/
//!!!испытания по методике SWEAT (постоянная длительность испытаний)
//в процессе отладки
void EM::EM_SWEAT()
{
    /*char str[1023] = {0};
    int i,
        start,
        end,
        cnt;

    bool    hp;


    double  Rth,
            Rmeas;

    double  Iforce;

    double  Vcompl;

    double  t_fe, t_ft, Pmeas, T0, Tmeas;
    bool bRamp_finished, bStopTest;

    ViInt32     mode[3],
                ch[4],
                status[3];

    ViReal64    rng[3],
                values[3] = {0},
                time[3];

    ch[0] = slot[2];
    ch[1] = slot[3];
    ch[2] = slot[4];
    ch[3] = 0;

    mode[0] = agb1500_VM_MODE;
    mode[1] = agb1500_VM_MODE;
    mode[2] = agb1500_IM_MODE;

    rng[0] = agb1500_AUTO_RANGE;
    rng[1] = agb1500_AUTO_RANGE;
    rng[2] = agb1500_AUTO_RANGE;

    memset(status, 0, 3*sizeof(ViInt32));

    if(!Approx(P, T, 0, STEPS, Rth, T0)){
        ret = ERR_APPROX;
        emit finished_test();
        return;
    }
    //-------------Start of current settling phase----------------------------

    fputs("\t\t==========Current settling==========\n", file);

    if(abort_meas){
        ret = ERR_ABORT;
        emit finished_test();
        return;
    }

    for(i=0; i<EM_OUTPUTS; i++)
        if(slot[i]!=0 && slot[i]!=SLOT_GNDU){
            ret = agb1500_setSwitch(vi_b1500, slot[i], ON);
            if(ret!=VI_SUCCESS){
                ret = ERR_SWITCH_ON;
                emit finished_test();
                return;
            }

            ret = agb1500_setAdcType(vi_b1500, slot[i], agb1500_HSPEED_ADC);
            if(ret!=VI_SUCCESS){
                ret = ERR_ADCTYPE;
                emit finished_test();
                return;
            }
    }

    if(slot[1] == SLOT_GNDU && slot[0] == SLOT_SMU1)
        hp = true;
    else
        hp = false;

    //---ADC, Filter, Autozero settings---
    ret = agb1500_setFilter(vi_b1500, agb1500_CH_ALL, FILTER_OFF);
    if(ret!=VI_SUCCESS){
        ret = ERR_FILTER;
        emit finished_test();
        return;
    }

    ret = agb1500_setAdc(vi_b1500, agb1500_HSPEED_ADC, agb1500_INTEG_AUTO, 10, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS){
        ret = ERR_ADC;
        emit finished_test();
        return;
    }

    if(slot[1] != SLOT_GNDU){
         ret = agb1500_force(vi_b1500, slot[1], agb1500_VF_MODE, agb1500_AUTO_RANGE, 0.0, MAX_I, agb1500_POL_AUTO);
         if(ret!=VI_SUCCESS){
             ret = ERR_FORCE;
             emit finished_test();
             return;
         }
    }

    ret = agb1500_force(vi_b1500, slot[2], agb1500_IF_MODE, agb1500_AUTO_RANGE, 0.0, MAX_V, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS){
        ret = ERR_FORCE;
        emit finished_test();
        return;
    }

    ret = agb1500_force(vi_b1500, slot[3], agb1500_IF_MODE, agb1500_AUTO_RANGE, 0.0, MAX_V, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS){
        ret = ERR_FORCE;
        emit finished_test();
        return;
    }

    if(bMeasExt){
        ret = agb1500_force(vi_b1500, slot[4], agb1500_VF_MODE, agb1500_AUTO_RANGE, 0.0, MAX_I, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS){
            ret = ERR_FORCE;
            emit finished_test();
            return;
        }
    }

    ret = agb1500_resetTimestamp(vi_b1500);
    if(ret!=VI_SUCCESS){
        ret = ERR_TIMESTAMP;
        emit finished_test();
        return;
    }

    t_ft = TimeLimit*100.0;
    Iforce = sqrt(P[STEPS-1]/R[STEPS-1]);
    bRamp_finished = false;
    bStopTest = false;
    Itest = Iforce;
    cnt = 0;

    do{
        if(abort_meas){
            ret = ERR_ABORT;
            emit finished_test();
            return;
        }

        if(Iforce>ICOMPL_CARD){
            ret = ERR_COMPL_CARD;
            bStopTest = true;
            emit finished_test();
            return;
        }

        if(hp){
            Vcompl = SetHVcompliance(Iforce);

            if(fabs(Iforce)>MAX_HI || fabs(Iforce)<MIN_IFORCE){
                ret = ERR_MAXCURRENT;
                bStopTest = true;
                emit finished_test();
                return;
            }
        }//end if(hp)
        else{
            Vcompl = SetVcompliance(Iforce);

            if(fabs(Iforce)>MAX_I || fabs(Iforce)<MIN_IFORCE){
                ret = ERR_MAXCURRENT;
                bStopTest = true;
                emit finished_test();
                return;
            }
        }

        ret = agb1500_force(vi_b1500, slot[0], agb1500_IF_MODE, agb1500_AUTO_RANGE, Iforce, Vcompl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS){
            ret = ERR_FORCE;
            bStopTest = true;
            emit finished_test();
            return;
        }

        QThread::msleep(EM_TIME_STEP);
        ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0], NULL, &time[0]);
        if(ret!=VI_SUCCESS){
            ret = ERR_FORCE;
            bStopTest = true;
        }

        agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL); //удалить (нужна для отладки)!!!!!!!!!!!!!!!

        Rmeas = (values[0] - values[1])/Iforce;
        Pmeas = Rmeas * Iforce * Iforce;
        R.push_back(Rmeas);
        P.push_back(Pmeas);
        Vcompl = fabs(values[0]);

        if(bRamp_finished)
            Tmeas = Rth*Rmeas*Iforce*Iforce + T0;
        else{
            Tmeas = Tchuck + (Rmeas - Rchuck)/(Rchuck*TCRchuck);
            Tmeas = Tmeas * Fcorr(Tmeas);
        }
        T.push_back(Tmeas);

        t_fe = A*pow(Iforce/(W*H*1E-8),(-1)*n)*
                                    exp(Ea/(8.617E-5*(Tmeas+273.16)));
        sprintf(str, "%e,%e,%e,%e,%e,%e,%f\n", t_stl + time[0], Iforce, values[0],
                                            values[1], values[2], Rmeas, Tmeas);
        fputs(str, file);

        emit change_EMRvalue(QString("%1").arg(Rmeas));
        emit change_EMIvalue(QString("%1").arg(values[2]/1E-9));
        emit change_test_time(t_stl + time[0]);

        Rfail = Rmeas;
        TTF = time[0];
        Iext_fail = values[2];

        if(fabs(t_fe-TimeLimit)>BE)
            Iforce = New_I(t_ft, Tmeas, Rth, T0, Iforce);
        else{
            if(t_ft == TimeLimit){
                t_ft = (t_ft + TimeLimit)/2.0;
                if(fabs(t_ft-TimeLimit)>BE){
                    t_ft = TimeLimit;
                     t_stl = t_stl + time[0];
                }

                start = 0;
                end = T.size();
                for(i=0; i<end; i++)
                    if(Tmeas-T[i]<50){
                        start = i;
                        break;
                    }

                if(!Approx(P, T, start, end, Rth, T0)){
                    ret = ERR_APPROX;
                    bStopTest = true;
                }
                Iforce = New_I(t_ft, Tmeas, Rth, T0, Iforce);

                bRamp_finished = true;
            }//end if(t_ft == em->TimeLimit)
        }

        if(Rmeas > Rmeas*(1 + StopCond)){
            ret = VI_SUCCESS;
            bStopTest = true;
        }

        if(cnt == 0){
           Rstart_stress = Rmeas;
           Iext_start_stress = values[2];
        }

        emit change_test_time(time[0]);

        cnt++;
    }
    while(!bStopTest);//fabs(t_fe-em->TimeLimit)>BE
    //-------------End of current settling phase-------------------------------

    agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, 0);*/
}
/*=====================================================================================================*/
//!!!измерение сопротивления 2-х выводной структуры
void EM::R_2points(const int &N, const double &Iforce)
{
    double Rd, Rr, Id, Ir;
    char str[256] = {0};

    ViInt32     mode[3],
                ch[4];
    ViReal64    rng[3],
                values[3] = {0};

    ch[0] = forceH.Slot;
    ch[1] = forceL.Slot;
    if(bMeasExt)
        ch[2] = ExtMon.Slot;
    ch[3] = 0;

    ret = SetSources(agb1500_HRESOLN_ADC);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_setAdc(vi_b1500, agb1500_HRESOLN_ADC, agb1500_INTEG_PLC, 1, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS)
        return;

    mode[0] = forceH.MeasMode;
    mode[1] = forceL.MeasMode;
    mode[2] = ExtMon.MeasMode;

    rng[0] = forceH.Range;
    rng[1] = forceL.Range;
    rng[2] = ExtMon.Range;

    if(fabs(Iforce)>ICOMPL_CARD){
        ret = ERR_COMPL_CARD;
        return;
    }

    CommMatrix(&forceL, &forceH, &senseH, &senseL, &ExtMon);

    SetVForceCompl(Iforce);

    //----Start of resistance measurement at chuck temperature----//
    if(forceL.Model != GNDSMU){
        ret = agb1500_force(vi_b1500, forceL.Slot, forceL.ForceMode, forceL.Range, 0.0, forceL.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    if(bMeasExt){
        ret = agb1500_force(vi_b1500, ExtMon.Slot, ExtMon.ForceMode, ExtMon.Range, 0.0, ExtMon.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    ret = agb1500_force(vi_b1500, forceH.Slot, forceH.ForceMode, forceH.Range, Iforce, forceH.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0],  NULL,  NULL);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS)
        return;

    Rr = fabs(values[0]/values[1]);
    Ir = values[2];

    CommMatrix(&forceH, &forceL, &senseH, &senseL, &ExtMon);

    if(forceL.Model != GNDSMU){
        ret = agb1500_force(vi_b1500, forceL.Slot, forceL.ForceMode, forceL.Range, 0.0, forceL.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    if(bMeasExt){
        ret = agb1500_force(vi_b1500, ExtMon.Slot, ExtMon.ForceMode, ExtMon.Range, 0.0, ExtMon.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    ret = agb1500_force(vi_b1500, forceH.Slot, forceH.ForceMode, forceH.Range, Iforce, forceH.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0],  NULL,  NULL);
    if(ret!=VI_SUCCESS)
        return;

    Rd = fabs(values[0]/values[1]);
    Id = values[2];

    Rchuck = (Rr + Rd)/2.0;
    Iext_init = (Ir + Id)/2.0;

    sprintf(str, "%d,%e,%e,%e,%e\n", N, fabs(values[1]), values[0], Iext_init, Rchuck);
    fputs(str, file);

    emit change_EMRvalue(QString("%1").arg(Rchuck));
    emit change_EMIvalue(QString("%1").arg(Iext_init));
}
/*=====================================================================================================*/
//!!!измерение сопротивления 4-х выводной структуры
void EM::R_kelvin(const int &N, const double &Iforce)
{
    double Rd, Rr, Id, Ir;
    char str[256] = {0};

    ViInt32     mode[3],
                ch[4] = {0};
    ViReal64    rng[3],
                values[3] = {0};

    ch[0] = senseH.Slot;
    ch[1] = senseL.Slot;
    if(bMeasExt)
        ch[2] = ExtMon.Slot;

    ret = SetSources(agb1500_HRESOLN_ADC);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_setAdc(vi_b1500, agb1500_HRESOLN_ADC, agb1500_INTEG_PLC, 1, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS)
        return;

    SetVForceCompl(Iforce);

    mode[0] = senseH.MeasMode;
    mode[1] = senseL.MeasMode;
    mode[2] = ExtMon.MeasMode;

    rng[0] = senseH.Range;
    rng[1] = senseL.Range;
    rng[2] = ExtMon.Range;

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS)
        return;

    if(fabs(Iforce)>ICOMPL_CARD){
        ret = ERR_COMPL_CARD;
        return;
    }

    CommMatrix(&forceL, &forceH, &senseH, &senseL, &ExtMon);

    //----Start of resistance measurement at chuck temperature----//
    if(forceL.Model != GNDSMU){
        ret = agb1500_force(vi_b1500, forceL.Slot, forceL.ForceMode, forceL.Range, 0.0, forceL.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    ret = agb1500_force(vi_b1500, senseH.Slot, senseH.ForceMode, senseH.Range, 0.0, senseH.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_force(vi_b1500, senseL.Slot, senseL.ForceMode, senseL.Range, 0.0, senseL.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    if(bMeasExt){
        ret = agb1500_force(vi_b1500, ExtMon.Slot, ExtMon.ForceMode, ExtMon.Range, 0.0, ExtMon.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    ret = agb1500_force(vi_b1500, forceH.Slot, forceH.ForceMode, forceH.Range, Iforce, forceH.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0],  NULL,  NULL);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS)
        return;

    Rr = (values[0] - values[1])/Iforce;
    Ir = values[2];

    CommMatrix(&forceH, &forceL, &senseH, &senseL, &ExtMon);

    if(forceL.Model != GNDSMU){
        ret = agb1500_force(vi_b1500, forceL.Slot, forceL.ForceMode, forceL.Range, 0.0, forceL.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    ret = agb1500_force(vi_b1500, senseH.Slot, senseH.ForceMode, senseH.Range, 0.0, senseH.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_force(vi_b1500, senseL.Slot, senseL.ForceMode, senseL.Range, 0.0, senseL.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    if(bMeasExt){
        ret = agb1500_force(vi_b1500, ExtMon.Slot, ExtMon.ForceMode, ExtMon.Range, 0.0, ExtMon.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    ret = agb1500_force(vi_b1500, forceH.Slot, forceH.ForceMode, forceH.Range, Iforce, forceH.Compl, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_measureM(vi_b1500, ch, mode, rng, &values[0],  NULL,  NULL);
    if(ret!=VI_SUCCESS)
        return;

    Rd = (values[0] - values[1])/Iforce;
    Id = values[2];

    Rchuck = (Rr + Rd)/2.0;
    Iext_init = (Ir + Id)/2.0;

    sprintf(str, "%d,%e,%e,%e,%e,%e\n", N, Iforce, values[0], values[1], Iext_init, Rchuck);
    fputs(str, file);

    emit change_EMRvalue(QString("%1").arg(Rchuck));
    emit change_EMIvalue(QString("%1").arg(Iext_init));
}
/*=====================================================================================================*/
//!!!линейное увеличение тока до момента отказа для 2-х выводной структуры
void EM::Isweep_2p()
{
    char str[1024] = {0};
    int i;
    bool fail_detected;
    QString curr_date;

    double  I_init;

    double Rprev, Rcurr, Ifail;

    ViInt32 points;

    ViReal64    Vstress[EM_SWEEP_POINTS+1],
                Istrss[EM_SWEEP_POINTS+1],
                tstress[EM_SWEEP_POINTS+1];

    curr_date =  QDateTime::currentDateTime().toString("hh-mm-ss dd.MM.yyyy");

    fputs("--- Isweep test ---\n", file);
    sprintf(str, "Date,%s\n", curr_date.toLocal8Bit().data());
    fputs(str, file);
    sprintf(str, "%s\n", comment.toLocal8Bit().data());
    fputs(str, file);
    sprintf(str, "W,%f,um\nH,%f,um\nTstress,%f,deg C,\n", W, H, Tstress);
    fputs(str, file);
    sprintf(str, "Istop,%f,mA\n", Istress*1000.0);
    fputs(str, file);

    if(MetalType == ALUMINUM)
        fputs("Material,Al\n", file);
    else
        if(MetalType == COPPER)
            fputs("Material,Cu\n", file);
        else
            fputs("Material,NoName\n", file);

    if(bHPRegime)
        fputs("High power\n", file);

    fputs("Time,Iforce,Vsense,Iext,Rmeas\n", file);
    fputs("s,A,V,A,Ohm\n", file);

    I_init =  Iinit();

    if(abort_meas){
        ret = ERR_ABORT;
        return;
    }

    ret = SetSources(agb1500_HRESOLN_ADC);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_setAdc(vi_b1500, agb1500_HRESOLN_ADC, agb1500_INTEG_PLC, 1, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS)
        return;

    Meas_2p_Resitance(I_init);

    fputs("\t\t==========I linear sweep test==========\n", file);
    SetVForceCompl(Istress);

    //--------------------------Start of Isweep----------------------------------------
    ret = agb1500_setIv(vi_b1500, forceH.Slot, agb1500_SWP_IF_SGLLIN, forceH.Range, 0, Istress,
                            EM_SWEEP_POINTS, 0, 0, 0, forceH.Compl, OFF);
    if(ret!=VI_SUCCESS)
        return;

    ret = agb1500_stopMode(vi_b1500, ON, agb1500_LAST_START);
    if(ret!=VI_SUCCESS)
        return;

    if(forceL.Model != GNDSMU){
        ret = agb1500_force(vi_b1500, forceL.Slot, forceL.ForceMode, forceL.Range, 0.0, forceL.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    if(bMeasExt){
        ret = agb1500_force(vi_b1500, ExtMon.Slot, ExtMon.ForceMode, ExtMon.Range, 0.0, ExtMon.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return;
    }

    ret = agb1500_resetTimestamp(vi_b1500);
    if(ret!=VI_SUCCESS)
        return;

    if(!abort_meas)
        ret = agb1500_sweepIv(vi_b1500, forceH.Slot, forceH.MeasMode, forceH.Range, &points,
                                                             &Istrss[0], &Vstress[0], NULL, &tstress[0]);

    ret = agb1500_zeroOutput(vi_b1500, forceH.Slot);
    if(ret!=VI_SUCCESS)
        return;
    //--------------------------End of Isweep-------------------------------------

    //data analyze
    fail_detected = false;

    Ifail = 0.0;
    for(i=0;i<points;i++){

        Rcurr = Vstress[i]/Istrss[i];

        if(i>0 && !fail_detected){
            Rprev = Vstress[i-1]/Istrss[i-1];

            if(fabs(Rcurr-Rprev)/Rcurr > StopCond){
                TTF = tstress[i-1];
                Rfail = Rprev;
                Ifail = Istrss[i-1];
                fail_detected = true;
            }
        }

        sprintf(str, "%e,%e,%e,0,%e\n", tstress[i], Istrss[i], Vstress[i], Rcurr);
        fputs(str, file);
    }

    //--------------------------Start of final stress---------------------------------------
    Meas_2p_Resitance(I_init);
    //--------------------------End of final stress---------------------------------------

    fputs("\t\t==========Analyze data==========\n", file);
    sprintf(str, "TTF,%e\n", TTF);
    fputs(str, file);
    sprintf(str, "Rfail,%e\n", Rfail);
    fputs(str, file);
    sprintf(str, "Ifail,%e\n", Ifail);
    fputs(str, file);
}
/*=====================================================================================================*/
