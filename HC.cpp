#include "measclasses.h"
#include "eq_control.h"

extern bool abort_meas;
/*-----------------------------------------------------------------------------------------------*/
HC::HC()
{
   Clear();
}
/*-----------------------------------------------------------------------------------------------*/
void HC::Clear()
{
    MOS.clear();

    Drain.clear();
    Source.clear();
    Gate.clear();
    Body.clear();

    TypeCh = NCHANN;     //1=N, 0=P
    T = 0.0;
    TimeLimit = 0.0;
    comment = "";

    Vgstress = 0.0;
    Vdstress = 0.0;
    Vsstress = 0.0;
    Vbstress = 0.0;

    Time.clear();
}
/*-----------------------------------------------------------------------------------------------*/
bool HC::CheckSrc()
{
    if(this->Body.Slot==SLOT_GNDU ||
            this->Source.Slot==SLOT_GNDU ||
            this->Drain.Slot==SLOT_GNDU ||
            this->Gate.Slot==SLOT_GNDU)
        return false;

    return true;
}
/*-----------------------------------------------------------------------------------------------*/
void HC::RunTest(const QString &Macro, const int &Die)
{
    double  Meas_time[NUM_HC_TIMES] = {0.0,
                                       1.0,
                                       2.0,
                                       5.0,
                                       10.0,
                                       30.0,
                                       50.0,
                                       70.0,
                                       100.0,
                                       200.0,
                                       300.0,
                                       500.0,
                                       700.0,
                                       1000.0,
                                       2000.0,
                                       3000.0,
                                       4000.0,
                                       5000.0,
                                       6000.0,
                                       7000.0,
                                       8000.0,
                                       9000.0,
                                       10000.0,
                                       15000.0,
                                       20000.0,
                                       25000.0,
                                       30000.0,
                                       35000.0,
                                       40000.0,
                                       45000.0,
                                       50000.0,
                                       55000.0,
                                       60000.0,
                                       65000.0,
                                       70000.0,
                                       75000.0,
                                       80000.0,
                                       85000.0,
                                       90000.0,
                                       95000.0,
                                       100000.0};


    QVector<Transistor>::iterator it;
    QString FileName;
    QString curr_date;
    QString status;
    int i;
    double Stress_time;
    char str[256];

    curr_date = QDateTime::currentDateTime().toString("hh-mm-ss dd.MM.yyyy");
    status = QString("HC_%1_die%2").arg(Macro).arg(Die);
    FileName = trUtf8("%1%2_%3.txt").arg(Path_to_save_data).arg(status).arg(curr_date);

    file = fopen(FileName.toLocal8Bit().data(), "w+a");

    if(file != NULL){
        emit change_test_status(status);

        for(it=MOS.begin(); it<MOS.end(); it++)
            it->clear_data();

        sprintf(str, "%s\n", curr_date.toLocal8Bit().data());
        fputs(str, file);
        sprintf(str, "Die,%d\n", Die);
        fputs(str, file);
        sprintf(str, "Macro,%s\n", Macro.toLocal8Bit().data());
        fputs(str, file);

        fputs("--- HC test ---\n", file);

        ret = SetSources(agb1500_HSPEED_ADC);
        if(ret!=VI_SUCCESS)
            return;

        ret = agb1500_setAdc(vi_b1500, agb1500_HSPEED_ADC, agb1500_INTEG_PLC, 1, agb1500_FLAG_OFF);
        if(ret!=VI_SUCCESS)
            return;

        if(HC_ParamsSpot(0.0)==VI_SUCCESS){
            for(i=0; i<NUM_HC_TIMES-1; i++){
                if(Meas_time[i+1] <= TimeLimit){
                    Stress_time = Meas_time[i+1] - Meas_time[i];

                    if(HC_Stress(Stress_time)!=VI_SUCCESS)
                        break;

                    if(HC_ParamsSpot(Meas_time[i+1])!=VI_SUCCESS)
                        break;
                }
                else
                    break;

                emit change_test_time(Meas_time[i+1]);
            }//end for(i)
        }

        HCSaveTxtData();

        sprintf(str, "Status=%d\n", (int)ret);
        fputs(str, file);
        fclose(file);
    }

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    ret = agb1500_clearSampleSync(vi_b1500);
    ret = agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, 0);
}
/*-----------------------------------------------------------------------------------------------*/
//установка источников для испытаний
ViStatus HC::SetSources(const ViInt32 &adc_type)
{
    //drain
    Drain.ForceMode = agb1500_VF_MODE;
    Drain.MeasMode = agb1500_IM_MODE;
    Drain.Range = agb1500_LMA_10nA_RANGE; //for meas
    ret = agb1500_setSwitch(vi_b1500, Drain.Slot, ON);
    if(ret != VI_SUCCESS)
        return ret;
    ret = agb1500_setAdcType(vi_b1500, Drain.Slot, adc_type);
    if(ret != VI_SUCCESS)
        return ret;

    //gate
    Gate.ForceMode = agb1500_VF_MODE;
    Gate.MeasMode = agb1500_IM_MODE;
    Gate.Range = agb1500_LMA_10nA_RANGE;
    ret = agb1500_setSwitch(vi_b1500, Gate.Slot, ON);
    if(ret != VI_SUCCESS)
        return ret;
    ret = agb1500_setAdcType(vi_b1500, Gate.Slot, adc_type);
    if(ret != VI_SUCCESS)
        return ret;

    //source
    if(Source.Slot != SLOT_GNDU){
        Source.ForceMode = agb1500_VF_MODE;
        Source.MeasMode = agb1500_IM_MODE;
        Source.Range = agb1500_LMA_10nA_RANGE;
        ret = agb1500_setSwitch(vi_b1500, Source.Slot, ON);
        if(ret != VI_SUCCESS)
            return ret;
        ret = agb1500_setAdcType(vi_b1500, Source.Slot, adc_type);
        if(ret != VI_SUCCESS)
            return ret;
    }

    //body
    if(Body.Slot != SLOT_GNDU && Source.Slot != Body.Slot){
        Body.ForceMode = agb1500_VF_MODE;
        Body.MeasMode = agb1500_IM_MODE;
        Body.Range = agb1500_LMA_10nA_RANGE;
        ret = agb1500_setSwitch(vi_b1500, Body.Slot, ON);
        if(ret != VI_SUCCESS)
            return ret;
        ret = agb1500_setAdcType(vi_b1500, Body.Slot, adc_type);
        if(ret != VI_SUCCESS)
            return ret;
    }
    //---ADC, Filter, Autozero settings---
    ret = agb1500_setFilter(vi_b1500, agb1500_CH_ALL, agb1500_FLAG_ON);
    if(ret != VI_SUCCESS)
        return ret;

    return agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
}
/*-----------------------------------------------------------------------------------------------*/
bool HC::CommMatrix(SMU &Drain, SMU &Gate, SMU &Source, SMU &Body)
{
    if(bUseMatrix){
        OpenCommMatrix(vi_2200a);

        if(Drain.Num!=0 && !ConnectPorts2200AVect(vi_2200a, Drain.Num, Drain.pins))
            return false;

        if(Gate.Num!=0 && !ConnectPorts2200AVect(vi_2200a, Gate.Num, Gate.pins))
            return false;

        if(Source.Num!=0 && !ConnectPorts2200AVect(vi_2200a, Source.Num, Source.pins))
            return false;

        if(Body.Num!=0 && !ConnectPorts2200AVect(vi_2200a, Body.Num, Body.pins))
            return false;
    }

    return true;
}
/*-----------------------------------------------------------------------------------------------*/
//получение характеристик после стрессового воздействия, развертка
/*int HC_ParamsSweep(ViSession &vi_B1500, ViSession &vi_2200A, HC *t, FILE *file)
{
    ViStatus ret;
    int     i,
            N;

    int Result = 1;

    char str[1023];
    ViInt32 points;

    unsigned    drain[NUM_CARD_PINS] = {0},
                gate[NUM_CARD_PINS] = {0},
                source[NUM_CARD_PINS] = {0},
                body[NUM_CARD_PINS] = {0};

    double Vthlin, Vthsat;

    double  *Idfl,
            *Idfs,
            *Idrs,
            *Vg;

    N = (int)((t->Vgstop - t->Vgstart)/t->Vgstep + 0.00001) + 1;

    if(N>1001 || N<1)
        return ERR_SWPPOINTS;

    //---ADC, Filter, Autozero settings---
    ret = agb1500_setFilter(vi_B1500, agb1500_CH_ALL, FILTER_ON);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        return ERR_FILTER;
    }

    ret = agb1500_setAdc(vi_B1500, agb1500_HRESOLN_ADC, agb1500_INTEG_MANUAL, 10, agb1500_FLAG_OFF);    //0.8ms
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        return ERR_ADC;
    }

    Idfl = new double[N];
    Idfs = new double[N];
    Idrs = new double[N];
    Vg = new double[N];

    //pins[HC_OUTPUTS][NUM_CARD_PINS];
    for(i=0; i<NUM_CARD_PINS; i++)
        if(t->pins[0][i]!=0 && t->pins[1][i]!=0 && t->pins[2][i]!=0 && t->pins[3][i]!=0){

            drain[0] = t->pins[0][i];
            gate[0] = t->pins[1][i];
            source[0] = t->pins[2][i];
            body[0] = t->pins[3][i];

            OpenCommMatrix(vi_2200A);
            //коммутация матрицы
            if(!ConnectPorts2200A(vi_2200A, t->src[0], drain) ||
                    !ConnectPorts2200A(vi_2200A, t->src[1], gate) ||
                    !ConnectPorts2200A(vi_2200A, t->src[2], source) ||
                    !ConnectPorts2200A(vi_2200A, t->src[3], body)){
                    QMessageBox::critical(0, "Error", QString("Failure during matrix commutation!"),
                                           QMessageBox::Ok);
                    Result = 2;
                    break;
            }

            //включение каналов
            for(int i=0; i<HC_OUTPUTS; i++)
                if(t->slot[i]!=0){
                    agb1500_setSwitch(vi_B1500, t->slot[i], ON);
                    if(ret!=VI_SUCCESS){
                        check_err_B1500(vi_B1500, ret);
                        return ERR_SWITCH_ON;
                    }

                    ret = agb1500_setAdcType(vi_B1500, t->slot[i], agb1500_HRESOLN_ADC);
                    if(ret!=VI_SUCCESS){
                        check_err_B1500(vi_B1500, ret);
                        return ERR_ADC;
                    }
                }

            ret = agb1500_stopMode(vi_B1500, agb1500_FLAG_ON, agb1500_LAST_START);
            if(ret!=VI_SUCCESS){
                Result = ERR_SWPSTOPMODE;
                break;
            }

            memset(Idfl, 0, N*sizeof(double));
            memset(Idfs, 0, N*sizeof(double));
            memset(Idrs, 0, N*sizeof(double));
            memset(Vg, 0, N*sizeof(double));

            for(int q=0; q<N; q++)
                Vg[q] = t->Vgstart + t->Vgstep * q;

            ret = agb1500_force(vi_B1500, t->slot[2], agb1500_VF_MODE, agb1500_AUTO_RANGE, 0.0, MAX_I, agb1500_POL_AUTO);
            ret = agb1500_force(vi_B1500, t->slot[3], agb1500_VF_MODE, agb1500_AUTO_RANGE, 0.0, MAX_I, agb1500_POL_AUTO);

            //---------------------------1) Idfl measurement
            ret = agb1500_force(vi_B1500, t->slot[0], agb1500_VF_MODE, agb1500_AUTO_RANGE, t->Vtlin, MAX_I, agb1500_POL_AUTO);//drain
            if(ret!=VI_SUCCESS){
                Result = ERR_FORCE;
                break;
            }

            ret = agb1500_setIv(vi_B1500, t->slot[1], agb1500_SWP_VF_SGLLIN, agb1500_AUTO_RANGE, t->Vgstart, t->Vgstop, N, 0, 0, 0, t->Idmax, 0);
            if(ret!=VI_SUCCESS){
                Result = ERR_SWPSETIV;
                break;
            }

            ret = agb1500_sweepIv(vi_B1500, t->slot[1], agb1500_IM_MODE, agb1500_AUTO_RANGE, &points, 0, &Idfl[0], 0, 0);
            if(ret!=VI_SUCCESS){
                Result = ERR_SWPIV;
                break;
            }

            //---------------------------2) Idfs measurement
            ret = agb1500_force(vi_B1500, t->slot[0], agb1500_VF_MODE, agb1500_AUTO_RANGE, t->Vtsat, MAX_I, agb1500_POL_AUTO);
            if(ret!=VI_SUCCESS){
                Result = ERR_FORCE;
                break;
            }

            ret = agb1500_setIv(vi_B1500, t->slot[1], agb1500_SWP_VF_SGLLIN, agb1500_AUTO_RANGE, t->Vgstart, t->Vgstop, N, 0, 0, 0, t->Idmax, 0);
            if(ret!=VI_SUCCESS){
                Result = ERR_SWPSETIV;
                break;
            }

            ret = agb1500_sweepIv(vi_B1500, t->slot[1], agb1500_IM_MODE, agb1500_AUTO_RANGE, &points, 0, &Idfs[0], 0, 0);
            if(ret!=VI_SUCCESS){
                Result = ERR_SWPIV;
                break;
            }

            //---------------------------3) Idrs measurement
            ret = agb1500_force(vi_B1500, t->slot[2], agb1500_VF_MODE, agb1500_AUTO_RANGE, t->Vtsat, MAX_I, agb1500_POL_AUTO);//source
            ret = agb1500_force(vi_B1500, t->slot[0], agb1500_VF_MODE, agb1500_AUTO_RANGE, 0.0, MAX_I, agb1500_POL_AUTO);     //drain
            if(ret!=VI_SUCCESS){
                Result = ERR_FORCE;
                break;
            }

            ret = agb1500_setIv(vi_B1500, t->slot[1], agb1500_SWP_VF_SGLLIN, agb1500_AUTO_RANGE, t->Vgstart, t->Vgstop, N, 0, 0, 0, t->Idmax, 0);
            if(ret!=VI_SUCCESS){
                Result = ERR_SWPSETIV;
                break;
            }

            ret = agb1500_sweepIv(vi_B1500, t->slot[1], agb1500_IM_MODE, agb1500_AUTO_RANGE, &points, 0, &Idrs[0], 0, 0);
            if(ret!=VI_SUCCESS){
                Result = ERR_SWPIV;
                break;
            }

            for(int i=0; i<N; i++){
                sprintf(str, "%e,%e,%e,%e\n", Vg[i], Idfl[i], Idfs[i], Idrs[i]);
                fputs(str, file);
            }

            //---------------------------4) Vtlin measurement
            VthBinary(vi_B1500, t->slot[0], t->slot[1], t->Vtlin, t->Vgstart, t->Vgstop, t->Idmax, t->Ith, 16, Vthlin);
            if(ret!=VI_SUCCESS){
                Result = ERR_SWPVTH;
                break;
            }

            //---------------------------5) Vtsat measurement
            VthBinary(vi_B1500, t->slot[0], t->slot[1], t->Vtlin, t->Vgstart, t->Vgstop, t->Idmax, t->Ith, 16, Vthsat);
            if(ret!=VI_SUCCESS){
                Result = ERR_SWPVTH;
                break;
            }

            ret = agb1500_zeroOutput(vi_B1500, agb1500_CH_ALL);
            if(ret!=VI_SUCCESS)
            {
                check_err_B1500(vi_B1500, ret);
                Result = ERR_ZERO;
            }
        }//end if(i)

    if(ret!=VI_SUCCESS)
        check_err_B1500(vi_B1500, ret);

    ret = agb1500_setSwitch(vi_B1500, agb1500_CH_ALL, OFF);
    check_err_B1500(vi_B1500, ret);
    if(ret!=VI_SUCCESS)
        Result = ERR_SWITCH_OFF;

    delete[] Idfl;
    delete[] Idfs;
    delete[] Idrs;
    delete[] Vg;

    return Result;
}*/
/*-----------------------------------------------------------------------------------------------*/
//получение характеристик после стрессового воздействия
//точечное измерение
ViStatus HC::HC_ParamsSpot(double TestTime)
{
    ViInt32 points;
    double  *Idrain, Vstep;
    double Irfs, Idfs, Idfl, Vtlin, Vtsat, Gm, VtGm;
    int cnt = 0;

    QVector<Transistor>::iterator it;

    Time.push_back(TestTime);

    for(it=MOS.begin(); it<MOS.end(); it++){
        if(abort_meas)
            return ret=ERR_ABORT;

        CommMatrix(it->drain, it->gate, it->source, it->body);
        //-------------------------------------------------------------------------------------
        //-------------------------- 1)Irfs measurement (Ion reverse)--------------------------
        //-------------------------------------------------------------------------------------
        //порядок - подложка, исток, сток, затвор
        ret = agb1500_force(vi_b1500, Body.Slot, Body.ForceMode, agb1500_AUTO_RANGE, 0.0, it->Idmax, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            break;

        ret = agb1500_force(vi_b1500, Drain.Slot, Drain.ForceMode, agb1500_AUTO_RANGE, 0.0, it->Idmax, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            break;

        ret = agb1500_force(vi_b1500, Source.Slot, Source.ForceMode, agb1500_AUTO_RANGE, it->Vdsat, it->Idmax, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            break;

        ret = agb1500_force(vi_b1500, Gate.Slot, Gate.ForceMode, agb1500_AUTO_RANGE, it->Vdsat, it->Idmax, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            break;

        //ret = agb1500_opc_Q(vi_b1500, &opc);
        //if(ret!=VI_SUCCESS || opc!=VI_TRUE)
        //    break;

        ret = agb1500_spotMeas(vi_b1500, Source.Slot, Source.MeasMode, Source.Range, &Irfs, 0, 0);
        if(ret!=VI_SUCCESS)
            break;
        it->Irfs.push_back(Irfs);


        //-------------------------------------------------------------------------------------
        //-------------------------- 2) Idfs measurement (Ion)---------------------------------
        //-------------------------------------------------------------------------------------
        ret = agb1500_force(vi_b1500, Source.Slot, Source.ForceMode, agb1500_AUTO_RANGE, 0.0, it->Idmax, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            break;

        ret = agb1500_force(vi_b1500, Drain.Slot, Drain.ForceMode, agb1500_AUTO_RANGE, it->Vdsat, it->Idmax, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            break;

        ret = agb1500_spotMeas(vi_b1500, Drain.Slot, Drain.MeasMode, Drain.Range, &Idfs, 0, 0);
        if(ret!=VI_SUCCESS)
            break;
        it->Idfs.push_back(Idfs);


        //-------------------------------------------------------------------------------------
        //-------------------------- 3) Idfl measurement --------------------------------------
        //-------------------------------------------------------------------------------------
        ret = agb1500_force(vi_b1500, Source.Slot, Source.ForceMode, agb1500_AUTO_RANGE, 0.0, it->Idmax, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            break;

        ret = agb1500_force(vi_b1500, Drain.Slot, Drain.ForceMode, agb1500_AUTO_RANGE, it->Vdlin, it->Idmax, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            break;

        ret = agb1500_spotMeas(vi_b1500, Drain.Slot, Drain.MeasMode, Drain.Range, &Idfl, 0, 0);
        if(ret!=VI_SUCCESS)
            break;
        it->Idfs.push_back(Idfl);


        //-------------------------------------------------------------------------------------
        //-------------------------- 4) Vtlin measurement -------------------------------------
        //-------------------------------------------------------------------------------------
        ret = VthBinary(vi_b1500, Drain.Slot, Gate.Slot, it->Vdlin, it->Vgstart, it->Vgstop, it->Idmax, it->Ith, 16, Vtlin);
        if(ret!=VI_SUCCESS)
            break;
        it->Vlin.push_back(Vtlin);


        //-------------------------------------------------------------------------------------
        //---------------------------5) Vtsat measurement -------------------------------------
        //-------------------------------------------------------------------------------------
        ret = VthBinary(vi_b1500, Drain.Slot, Gate.Slot, it->Vdsat, it->Vgstart, it->Vgstop, it->Idmax, it->Ith, 16, Vtsat);
        if(ret!=VI_SUCCESS)
            break;
        it->Vsat.push_back(Vtsat);


        //-------------------------------------------------------------------------------------
        //---------------------------6) Gm mesurement -----------------------------------------
        //-------------------------------------------------------------------------------------
#define NUM_STEPS    1001
            Vstep = fabs(it->Vgstop - it->Vgstart)/NUM_STEPS;
            Idrain = new double[NUM_STEPS];

            ret = agb1500_force(vi_b1500, Source.Slot, Source.ForceMode, agb1500_AUTO_RANGE, 0.0, it->Idmax, agb1500_POL_AUTO);
            if(ret!=VI_SUCCESS)
                break;

            ret = agb1500_force(vi_b1500, Drain.Slot, Drain.ForceMode, agb1500_AUTO_RANGE, it->Vdsat, it->Idmax, agb1500_POL_AUTO);
            if(ret!=VI_SUCCESS)
                break;

            ret = agb1500_setIv(vi_b1500, Gate.Slot, agb1500_SWP_VF_SGLLIN, agb1500_AUTO_RANGE, \
                                                        it->Vgstart, it->Vgstop, NUM_STEPS, 0.0, 0.0, 0.0, it->Idmax, 0);
            if(ret!=VI_SUCCESS)
                break;

            ret = agb1500_sweepIv(vi_b1500, Drain.Slot, Drain.MeasMode, Drain.Range, &points, 0, &Idrain[0], 0, 0);
            if(ret!=VI_SUCCESS)
                break;

            Gm = MaxGm(Idrain, it->Vgstart, Vstep, it->Vdsat, NUM_STEPS, HC_SATURATION_REGIME, VtGm);

            it->Gm.push_back(Gm);
            it->Vt_Gmmax.push_back(VtGm);

            delete[] Idrain;
            Idrain = NULL;

            /*
            для проверки работы
            Vtlin = 0;
            it->Vlin.push_back(Vtlin);
            Vtsat = 0;
            it->Vsat.push_back(Vtsat);
            Gm=0;
            VtGm=0;
            it->Gm.push_back(0);
            it->Vt_Gmmax.push_back(0);
            */

            emit change_HC_item(cnt, Irfs, Idfs, Idfl, Vtlin, Vtsat, VtGm, Gm);

            if(abort_meas){
                ret=ERR_ABORT;
                break;
            }

            ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
            if(ret != VI_SUCCESS)
                return ret;

            cnt++;
     }//end for(it)

    return VI_SUCCESS;
}
/*-----------------------------------------------------------------------------------------------*/
//стрессовое воздействие
//параллельное воздействие на все структуры
//в файл с данными ничего не пишем
ViStatus HC::HC_Stress(double Stress_time)
{
    double Interval = HC_INTERVAL;
    int Steps;

    if(Stress_time <= HC_INTERVAL)
        Steps = 1;
    else
        Steps = (int)(Stress_time/Interval + 0.00001);

    ViInt32     pts,
                idx[Steps+1],
                mode[1],
                ch[2] = {0};

    ViReal64    rng[1],
                values[Steps+1];

    ch[0] = Drain.Slot;
    mode[0] = Drain.MeasMode;
    rng[0] = agb1500_FIX_100mA_RANGE;

    if(!CommMatrix(Drain, Gate, Source, Body))
        return ret = agb1500_INSTR_ERROR_DETECTED;

    if(abort_meas)
        return ret = ERR_ABORT;

    //тип измерений
    ret = agb1500_setSampleMode(vi_b1500, agb1500_SMP_LINEAR);
    if(ret!=VI_SUCCESS)
        return ret;

    ret = agb1500_addSampleSyncIv(vi_b1500, Body.Slot, Body.ForceMode, agb1500_AUTO_RANGE, Vbstress, Vbstress, MAX_I);
    if(ret!=VI_SUCCESS)
        return ret;
    ret = agb1500_addSampleSyncIv(vi_b1500, Source.Slot, Source.ForceMode, agb1500_AUTO_RANGE, Vsstress, Vsstress, MAX_I);
    if(ret!=VI_SUCCESS)
        return ret;
    ret = agb1500_addSampleSyncIv(vi_b1500, Gate.Slot, Gate.ForceMode, agb1500_AUTO_RANGE, Vgstress, Vgstress, MAX_I);
    if(ret!=VI_SUCCESS)
        return ret;
    ret = agb1500_addSampleSyncIv(vi_b1500, Drain.Slot, Drain.ForceMode, agb1500_AUTO_RANGE, Vdstress, Vdstress, MAX_I);
    if(ret!=VI_SUCCESS)
        return ret;

    ret = agb1500_setSample(vi_b1500, 0.0, 0.0, Interval, 1);
    if(ret!=VI_SUCCESS)
        return ret;

    for(int i=0; i<Steps; i++){
        ret = agb1500_sampleIv(vi_b1500, ch, mode, rng, &pts, &idx[0], &values[0], NULL, NULL);
        if(ret != VI_SUCCESS)
            break;

        if(abort_meas)
            return ret=ERR_ABORT;
    }

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS)
        return ret;

    ret = agb1500_clearSampleSync(vi_b1500);
    if(ret!=VI_SUCCESS)
        return ret;

    if(abort_meas)
        return ret=ERR_ABORT;

    return VI_SUCCESS;
}
/*-----------------------------------------------------------------------------------------------*/
//сохранение измеренных данных
void HC::HCSaveTxtData()
{
    QString s;

    fputs("---Initial input data---\n", file);

    s = "";
    for(auto it:MOS)
        s = s + QString("DUT,%d").arg(it.Index) + SetDelim(',', 7);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    s = "";
    for(auto it:MOS)
        s = s + it.comment + SetDelim(',', 8);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //drain
    for(auto it:MOS){
        fputs("DrainPins,", file);
        s = "";
        for(auto n: it.drain.pins)
            s = s + QString("%1;").arg(n);
        s = s + SetDelim(',', 7);
        fputs(s.toLocal8Bit().data(), file);
    }
    fputs("\n", file);

    //gate
    for(auto it:MOS){
        fputs("GatePins,", file);
        s = "";
        for(auto n: it.gate.pins)
            s = s + QString("%1;").arg(n);
        s = s + SetDelim(',', 7);
        fputs(s.toLocal8Bit().data(), file);
    }
    fputs("\n", file);

    //source
    for(auto it:MOS){
        fputs("SourcePins,", file);
        s = "";
        for(auto n: it.source.pins)
            s = s + QString("%1;").arg(n);
        s = s + SetDelim(',', 7);
        fputs(s.toLocal8Bit().data(), file);
    }
    fputs("\n", file);

    //body
    for(auto it:MOS){
        fputs("BodyPins,", file);
        s = "";
        for(auto n: it.body.pins)
            s = s + QString("%1;").arg(n);
        s = s + SetDelim(',', 7);
        fputs(s.toLocal8Bit().data(), file);
    }
    fputs("\n", file);

    //SMU Drain
    s = "";
    for(auto it:MOS)
        s = s + QString("DrainSMU,%1").arg(it.drain.Num) + SetDelim(',', 7);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //SMU Gate
    s = "";
    for(auto it:MOS)
        s = s + QString("GateSMU,%1").arg(it.gate.Num) + SetDelim(',', 7);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //SMU Source
    s = "";
    for(auto it:MOS)
        s = s + QString("SourceSMU,%1").arg(it.source.Num) + SetDelim(',', 7);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //SMU Body
    s = "";
    for(auto it:MOS)
        s = s + QString("BodySMU,%1").arg(it.body.Num) + SetDelim(',', 7);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //type channel
    s = "";
    for(auto it:MOS)
        if(TypeCh == NCHANN)
            s = s + QString("Channel type,N") + SetDelim(',', 7);
        else
            s = s + QString("Channel type,P") + SetDelim(',', 7);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Width
    s = "";
    for(auto it:MOS)
        s = s + QString("Width,%1,um").arg(it.W) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Length
    s = "";
    for(auto it:MOS)
        s = s + QString("Length,%1,um").arg(it.L) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Tempearture
    s = "";
    for(auto it:MOS)
        s = s + QString("Temp,%1,degC").arg(T) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Time limit
    s = "";
    for(auto it:MOS)
        s = s + QString("TimeLimit,%1,s").arg(TimeLimit) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Vtlin
    s = "";
    for(auto it:MOS)
        s = s + QString("Vdlin,%1,V").arg(it.Vdlin) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Vtsat
    s = "";
    for(auto it:MOS)
        s = s + QString("Vdsat,%1,V").arg(it.Vdsat) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Vgstart
    s = "";
    for(auto it:MOS)
        s = s + QString("Vgstart,%1,V").arg(it.Vgstart) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Vgstop
    s = "";
    for(auto it:MOS)
        s = s + QString("Vgstop,%1,V").arg(it.Vgstop) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Ith
    s = "";
    for(auto it:MOS)
        s = s + QString("Ith,%1,A").arg(it.Ith) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Vgstress
    s = "";
    for(auto it:MOS)
        s = s + QString("Vgstress,%1,V").arg(Vgstress) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Vdstress
    s = "";
    for(auto it:MOS)
        s = s + QString("Vdstress,%1,V").arg(Vdstress) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Vsstress
    s = "";
    for(auto it:MOS)
        s = s + QString("Vsstress,%1,V").arg(Vsstress) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Vbstress
    s = "";
    for(auto it:MOS)
        s = s + QString("Vbstress,%1,V").arg(Vbstress) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Vbstress
    s = "";
    for(auto it:MOS)
        s = s + QString("Idmax,%1,A").arg(it.Idmax) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    fputs("---Measured stress data---\n", file);

    //Stress values names
    s = "";
    for(auto it:MOS)
        s = s + QString("Time(s),Idfl(A),Idfs(A),Irfs(A),Vtlin(V),Vtsat(V),GmMax(S),Vt_GmMax(V),");
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //data
    int j = 0;
    for(auto tm:Time){
        s = "";
        for(auto it:MOS)
            s = s + QString("%1,%2,%3,%4,%5,%6,%7,%8,").arg(tm).arg(it.Idfl.at(j))\
                                                      .arg(it.Idfs.at(j))\
                                                      .arg(it.Irfs.at(j))\
                                                      .arg(it.Vlin.at(j))\
                                                      .arg(it.Vsat.at(j))\
                                                      .arg(it.Vt_Gmmax.at(j))\
                                                      .arg(it.Gm.at(j));
        fputs(s.toLocal8Bit().data(), file);
        fputs("\n", file);
        j++;
    }
    fputs("\n", file);
}
/*-----------------------------------------------------------------------------------------------*/
