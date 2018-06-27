#include "measclasses.h"
#include "eq_control.h"
#include "math.h"

extern bool abort_meas;
/*-----------------------------------------------------------------------------------------------*/
/*NBTI::NBTI()
{
   Clear();
}*/
/*-----------------------------------------------------------------------------------------------*/
/*void NBTI::Clear()
{
    gate = 0;
    drain = 0;

    gate_smu = 0;
    drain_smu = 0;

    Number = 0;
    Index = 0;

    ClassicTest = 1;

    gate_pins.clear();
    drain_pins.clear();
    sb_pins.clear();

    TypeCh = NCHANN;

    W = 0.0;
    L = 0.0;
    T = 0.0;

    Vtlin = 0.0;
    Vtsat = 0.0;
    StressTime = 0.0;

    Vgstress = 0.0;
    Vdstress = 0.0;
    Idmax = 0.0;

    Vgstart = 0.0;
    Vgstop = 0.0;

    Ith = 0.0;
}*/
/*-----------------------------------------------------------------------------------------------*/
/*void StressTesting::NBTI_Main()
{   
    //Classic NBTI testing (like HC)
    if(nbti.at(0).ClassicTest == 1){
        double  Meas_time[NUM_HC_TIMES] = {0.0,
                                           2.0,
                                           5.0,
                                           10.0,
                                           20.0,
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

        double  Stress_time;
        double tlimit;
        int i;

        tlimit = nbti.begin()->StressTime;

        if(NBTI_ParamsSpot_Classic(0.0)==VI_SUCCESS){
            for(i=0; i<NUM_HC_TIMES-1; i++){
                if(Meas_time[i+1] <= tlimit){
                    Stress_time = Meas_time[i+1] - Meas_time[i];

                    if(NBTI_Stress_Classic(Stress_time)!=VI_SUCCESS)
                        break;

                    if(NBTI_ParamsSpot_Classic(Meas_time[i+1])!=VI_SUCCESS)
                        break;
                }
                else
                    break;

                emit change_test_time(Meas_time[i+1]);
            }//end for(i)
        }

        NBTISaveData_Classic(&nbti, file);
    }
    //On-The-Fly testing!!!!!!
    else{
        if(1 == NBTI_ParamsSpot(vi_b1500, vi_2200a, &nbti, true) &&
           1 == NBTI_Parall())
                NBTI_ParamsSpot(vi_b1500, vi_2200a, &nbti, false);
        NBTISaveData_Fly(&nbti, file);
    }

    emit finished_test();
}*/
/*-----------------------------------------------------------------------------------------------*/
//получение характеристик после стрессового воздействия
//точечное измерение
/*int StressTesting::NBTI_ParamsSpot_Classic(double Time)
{
    ViStatus ret;
    int Result = VI_SUCCESS;
    double Idfs, Idfl, Vtlin, Vtsat;
    short cnt = 0;

    QVector<NBTI>::iterator it;

    for(it=nbti.begin(); it<nbti.end(); it++){

        //выключение каналов
        ret = agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        if(ret!=VI_SUCCESS)
            return ERR_SWITCH_OFF;

        //коммутация матрицы
        OpenCommMatrix(vi_2200a);

        if(abort_meas){
            agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
            agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
            return ERR_ABORT;
        }

        if(!ConnectPorts2200AVect(vi_2200a, it->gate_smu, it->gate_pins) ||           //gate
           !ConnectPorts2200AVect(vi_2200a, it->drain_smu, it->drain_pins) ||         //drain
            !ConnectPorts2200AVect(vi_2200a, GNDU, it->sb_pins)){                     //source and body
            return ERR_MATRIX;
        }

        ret = agb1500_setSwitch(vi_b1500, it->gate, ON);
        ret = agb1500_setSwitch(vi_b1500, it->drain, ON);
        if(ret!=VI_SUCCESS){
            Result = ERR_SWITCH_ON;
            break;
        }

        ret = agb1500_setAdcType(vi_b1500, it->gate, agb1500_HRESOLN_ADC);
        ret = agb1500_setAdcType(vi_b1500, it->drain, agb1500_HRESOLN_ADC);
        if(ret!=VI_SUCCESS){
            Result = ERR_ADC;
            break;
        }

        //установка фильтра
        ret = agb1500_setFilter(vi_b1500, agb1500_CH_ALL, agb1500_FLAG_OFF);
        if(ret!=VI_SUCCESS){
            Result = ERR_FILTER;
            break;
        }

        //настройка ADC конвертера
        ret = agb1500_setAdc(vi_b1500, agb1500_HRESOLN_ADC, agb1500_INTEG_AUTO, 25, agb1500_FLAG_OFF);
        if(ret!=VI_SUCCESS){
            Result = ERR_ADC;
            break;
        }

        it->Time.push_back(Time);

        //---------------------------Idfs measurement (Ion)
        ret = agb1500_force(vi_b1500, it->gate, agb1500_VF_MODE, agb1500_AUTO_RANGE, it->Vtsat, it->Idmax, agb1500_POL_AUTO);//gate
        if(ret!=VI_SUCCESS){
            Result = ERR_FORCE;
            break;
        }

        ret = agb1500_force(vi_b1500, it->drain, agb1500_VF_MODE, agb1500_AUTO_RANGE, it->Vtsat, it->Idmax, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS){
            Result = ERR_FORCE;
            break;
        }

        ret = agb1500_spotMeas(vi_b1500, it->drain, agb1500_IM_MODE, agb1500_AUTO_RANGE, &Idfs, 0, 0);
        if(ret!=VI_SUCCESS){
            Result = ERR_SPOTMEAS;
            break;
        }

        //второй раз измерение, так как первый почему-то не проходит
        ret = agb1500_force(vi_b1500, it->drain, agb1500_VF_MODE, agb1500_AUTO_RANGE, it->Vtsat, it->Idmax, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS){
            Result = ERR_FORCE;
            break;
        }

        ret = agb1500_spotMeas(vi_b1500, it->drain, agb1500_IM_MODE, agb1500_AUTO_RANGE, &Idfs, 0, 0);
        if(ret!=VI_SUCCESS){
            Result = ERR_SPOTMEAS;
            break;
        }
        it->Idfs.push_back(Idfs);


        //---------------------------Idfl measurement (Ioff)
        ret = agb1500_force(vi_b1500, it->drain, agb1500_VF_MODE, agb1500_AUTO_RANGE, it->Vtlin, it->Idmax, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS){
            Result = ERR_FORCE;
            break;
        }

        ret = agb1500_spotMeas(vi_b1500, it->drain, agb1500_IM_MODE, agb1500_AUTO_RANGE, &Idfl, 0, 0);
        if(ret!=VI_SUCCESS){
            Result = ERR_SPOTMEAS;
            break;
        }
        it->Idfl.push_back(Idfl);


        //---------------------------Vtlin measurement
        ret = VthBinary(vi_b1500, it->drain, it->gate, it->Vtlin, it->Vgstart, it->Vgstop, it->Idmax, it->Ith, 16, Vtlin);
        if(ret!=VI_SUCCESS){
            Result = ERR_SWPVTH;
            break;
        }
        it->Vlin.push_back(Vtlin);

        //---------------------------Vtsat measurement
        ret = VthBinary(vi_b1500, it->drain, it->gate, it->Vtsat, it->Vgstart, it->Vgstop, it->Idmax, it->Ith, 16, Vtsat);
        if(ret!=VI_SUCCESS){
            Result = ERR_SWPVTH;
            break;
        }
        it->Vsat.push_back(Vtsat);


        emit change_HC_item(cnt, 0, Idfs, Idfl, Vtlin, Vtsat, 0, 0);

        if(abort_meas){
            agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
            agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
            return ERR_ABORT;
        }

        ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
        if(ret!=VI_SUCCESS){
            agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, 0);
            return ERR_ZERO;
        }

        cnt++;
     }//end for(it)

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS)
        Result = ERR_ZERO;

    ret = agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
    if(ret!=VI_SUCCESS)
        Result = ERR_SWITCH_OFF;

    return Result;
}*/
/*-----------------------------------------------------------------------------------------------*/
//стрессовое воздействие
/*int StressTesting::NBTI_Stress_Classic(double Stress_time)
{
    ViStatus ret;
    int i;
    int N = nbti.size();

    double Interval = 1.0;
    int Steps, subSteps;

    if(Stress_time <= HC_INTERVAL){
        Steps = (int)Stress_time;
        subSteps = 1;
    }
    else{
        Steps = (int)HC_INTERVAL;
        subSteps = (int)(Stress_time/Steps + 0.00001);
    }

    QVector<NBTI>::iterator it;

    ViInt32     pts,
                idx[Steps*N+1],
                mode[N],
                ch[N+1];

    ViReal64    rng[N],
                values[Steps*N+1];


    memset(ch, 0, (N+1)*sizeof(ViInt32));

    //выключение каналов
    ret = agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
    if(ret!=VI_SUCCESS)
        return ERR_SWITCH_OFF;

    OpenCommMatrix(vi_2200a);

    if(abort_meas){
        agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_ABORT;
    }

    //коммутация матрицы
    for(it = nbti.begin(), i=0; it<nbti.end(); it++, i++){
        if(!ConnectPorts2200AVect(vi_2200a, it->gate_smu, it->gate_pins) ||          //gate
           !ConnectPorts2200AVect(vi_2200a, it->drain_smu, it->drain_pins) ||         //drain
            !ConnectPorts2200AVect(vi_2200a, GNDU, it->sb_pins))                //source and body
            return ERR_MATRIX;

        ret = agb1500_setSwitch(vi_b1500, it->gate, ON);
        ret = agb1500_setSwitch(vi_b1500, it->drain, ON);
        if(ret!=VI_SUCCESS){
            return ERR_SWITCH_ON;
        }

        ret = agb1500_setAdcType(vi_b1500, it->gate, agb1500_HSPEED_ADC);
        ret = agb1500_setAdcType(vi_b1500, it->drain, agb1500_HSPEED_ADC);
        if(ret!=VI_SUCCESS){
            return ERR_ADC;
        }

        ch[i] = it->gate;
        mode[i] = agb1500_IM_MODE;
        rng[i] = agb1500_FIX_100mA_RANGE; //измерение параметров не требуется
    }//end for(it)

    //установка фильтра
    ret = agb1500_setFilter(vi_b1500, agb1500_CH_ALL, agb1500_FLAG_ON);
    if(ret!=VI_SUCCESS)
        return ERR_FILTER;

    //настройка ADC конвертера
    ret = agb1500_setAdc(vi_b1500, agb1500_HSPEED_ADC, agb1500_INTEG_AUTO, 1, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS)
        return ERR_ADC;

    //тип измерений
    ret = agb1500_setSampleMode(vi_b1500, agb1500_SMP_LINEAR);
    if(ret!=VI_SUCCESS)
        return ERR_SAMPLEMODE;

    it = nbti.begin();
    ret = agb1500_addSampleSyncIv(vi_b1500, it->drain, agb1500_VF_MODE, agb1500_AUTO_RANGE, it->Vdstress, it->Vdstress, it->Idmax);
    if(ret!=VI_SUCCESS)
        return ERR_SAMPLESOURCE;

    for(it = nbti.begin(); it<nbti.end(); it++){
        ret = agb1500_addSampleSyncIv(vi_b1500, it->gate, agb1500_VF_MODE, agb1500_AUTO_RANGE, it->Vgstress, it->Vgstress, it->Idmax);
        if(ret!=VI_SUCCESS)
            return ERR_SAMPLESOURCE;
    }

    ret = agb1500_setSample(vi_b1500, 0.0, 0.0, Interval, Steps);
    if(ret!=VI_SUCCESS)
        return ERR_SETSAMPLE;

    for(int i=0; i<subSteps; i++){
        ret = agb1500_sampleIv(vi_b1500, ch, mode, rng, &pts, &idx[0], &values[0], NULL, NULL);
        if(ret<VI_SUCCESS)
            return ERR_SAMPLEIV;

        if(abort_meas){
            agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
            agb1500_clearSampleSync(vi_b1500);
            agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
            return ERR_ABORT;
        }
    }

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS)
        return ERR_ZERO;

    ret = agb1500_clearSampleSync(vi_b1500);
    if(ret!=VI_SUCCESS)
        return ERR_CLRSAMPLE;

    ret = agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
    if(ret!=VI_SUCCESS)
        return ERR_SWITCH_OFF;

    if(abort_meas)
        return ERR_ABORT;

    return VI_SUCCESS;
}*/
/*-----------------------------------------------------------------------------------------------*/
/*int NBTI::NBTI_ParamsSpot(ViSession &vi_B1500, ViSession &vi_2200A, QVector<NBTI> *t, bool InitOrEndMeas)
{
    int     i;
    int N = t->size();

    QVector<NBTI>::iterator it;

    QVector<ViInt32>    drains,
                        gates;

    QVector<ViReal64>   Vdlin,
                        Vdsat,
                        Vstart,
                        Vstop,
                        Icompl,
                        Itarget;

    ViInt32     mode[N],
                ch[N+1];

    ViReal64    rng[N],
                Idlin[N],
                Idsat[N];

    QVector<double> Vmeas_lin,
                    Vmeas_sat;

    ViStatus ret;

    drains.clear();
    gates.clear();
    Vdlin.clear();
    Vdsat.clear();
    Vstart.clear();
    Vstop.clear();
    Icompl.clear();
    Itarget.clear();

    ret = agb1500_setSwitch(vi_B1500, agb1500_CH_ALL, OFF);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        return ERR_SWITCH_OFF;
    }


    memset(ch, 0, (N+1)*sizeof(ViInt32));
    OpenCommMatrix(vi_2200A);

    if(abort_meas)
        return ERR_ABORT;

    //коммутация матрицы
    for(it = t->begin(); it<t->end(); it++){
        if(!ConnectPorts2200AVect(vi_2200A, it->gate, it->gate_pins) ||          //gate
           !ConnectPorts2200AVect(vi_2200A, it->drain, it->drain_pins) ||         //drain
            !ConnectPorts2200AVect(vi_2200A, GNDU, it->sb_pins))                //source and body
        {
            return ERR_MATRIX;
        }

        ret = agb1500_setSwitch(vi_B1500, it->gate, ON);
        if(ret!=VI_SUCCESS){
            check_err_B1500(vi_B1500, ret);
            return ERR_SWITCH_ON;
        }

        ret = agb1500_setAdcType(vi_B1500, it->gate, agb1500_HRESOLN_ADC);
        if(ret!=VI_SUCCESS){
            check_err_B1500(vi_B1500, ret);
            return ERR_ADC;
        }

        ret = agb1500_setSwitch(vi_B1500, it->drain, ON);
        if(ret!=VI_SUCCESS){
            check_err_B1500(vi_B1500, ret);
            return ERR_SWITCH_ON;
        }

        ret = agb1500_setAdcType(vi_B1500, it->drain, agb1500_HRESOLN_ADC);
        if(ret!=VI_SUCCESS){
            check_err_B1500(vi_B1500, ret);
            return ERR_ADC;
        }

        drains.push_back(it->drain);
        gates.push_back(it->gate);
        Vdlin.push_back(it->Vtlin);
        Vdsat.push_back(it->Vtsat);
        Vstart.push_back(it->Vgstart);
        Vstop.push_back(it->Vgstop);
        Icompl.push_back(it->Idmax);
        Itarget.push_back(it->Ith);
    }//end for(it)

    ret = agb1500_setFilter(vi_B1500, agb1500_CH_ALL, agb1500_FLAG_ON);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        agb1500_setSwitch(vi_B1500, agb1500_CH_ALL, OFF);
        return ERR_FILTER;
    }


    ret = agb1500_setAdc(vi_B1500, agb1500_HRESOLN_ADC, agb1500_INTEG_PLC, 3, agb1500_FLAG_ON);     //60ms
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        agb1500_setSwitch(vi_B1500, agb1500_CH_ALL, OFF);
        return ERR_ADC;
    }


    //--------------------
    //--------------------Idlin
    //--------------------
    for(it = t->begin(), i=0; it<t->end(); it++, i++){
        ret = agb1500_force(vi_B1500, it->gate, agb1500_VF_MODE, agb1500_AUTO_RANGE, it->Vtsat, it->Idmax, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS){
            check_err_B1500(vi_B1500, ret);
            agb1500_setSwitch(vi_B1500, agb1500_CH_ALL, OFF);
            return ERR_FILTER;
        }

        ret = agb1500_force(vi_B1500, it->drain, agb1500_VF_MODE, agb1500_AUTO_RANGE, it->Vtlin, it->Idmax, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS){
            check_err_B1500(vi_B1500, ret);
            agb1500_setSwitch(vi_B1500, agb1500_CH_ALL, OFF);
            return ERR_FILTER;
        }

        ch[i] = it->drain;
        mode[i] = agb1500_IM_MODE;
        rng[i] = agb1500_AUTO_RANGE;
    }

    ret = agb1500_measureM(vi_B1500, ch, mode, rng, &Idlin[0],  NULL,  NULL);
    if(ret<VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        agb1500_setSwitch(vi_B1500, agb1500_CH_ALL, OFF);
        return ERR_SAMPLEIV;
    }

    //--------------------
    //--------------------Idsat
    //--------------------
    for(it = t->begin(), i=0; it<t->end(); it++, i++){
        ret = agb1500_force(vi_B1500, it->drain, agb1500_VF_MODE, agb1500_AUTO_RANGE, it->Vtsat, it->Idmax, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS){
            check_err_B1500(vi_B1500, ret);
            agb1500_setSwitch(vi_B1500, agb1500_CH_ALL, OFF);
            return ERR_FORCE;
        }

        ch[i] = it->drain;
        mode[i] = agb1500_IM_MODE;
        rng[i] = agb1500_AUTO_RANGE;
    }

    ret = agb1500_measureM(vi_B1500, ch, mode, rng, &Idsat[0],  NULL,  NULL);
    if(ret<VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        agb1500_setSwitch(vi_B1500, agb1500_CH_ALL, OFF);
        return ERR_SAMPLEIV;
    }

    //--------------------
    //--------------------Vtlin
    //--------------------
    ret = VthBinaryM(vi_B1500, drains, gates, Vdlin, Vstart, Vstop, Icompl, Itarget, 16,  Vmeas_lin);
    if(ret<VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        agb1500_setSwitch(vi_B1500, agb1500_CH_ALL, OFF);
        return ERR_SWPVTH;
    }

    //--------------------
    //--------------------Vtsat
    //--------------------
    ret = VthBinaryM(vi_B1500, drains, gates, Vdsat, Vstart, Vstop, Icompl, Itarget, 16,  Vmeas_sat);
    if(ret<VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        agb1500_setSwitch(vi_B1500, agb1500_CH_ALL, OFF);
        return ERR_SWPVTH;
    }



    //---------
    //---------запись результатов
    //---------
    if(InitOrEndMeas)
        for(it = t->begin(), i=0; it<t->end(); it++, i++){
            it->Ioff_init = Idlin[i];
            it->Ion_init = Idsat[i];
            it->Vtlin_init = Vmeas_lin[i];
            it->Vtsat_init = Vmeas_sat[i];

        }
    else
        for(it = t->begin(), i=0; it<t->end(); it++, i++){
            it->Ioff_end = Idlin[i];
            it->Ion_end = Idsat[i];
            it->Vtlin_end = Vmeas_lin[i];
            it->Vtsat_end = Vmeas_sat[i];

        }

    ret = agb1500_zeroOutput(vi_B1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        agb1500_setSwitch(vi_B1500, agb1500_CH_ALL, 0);
        return ERR_ZERO;
    }

    ret = agb1500_clearSampleSync(vi_B1500);
    if(ret!=VI_SUCCESS)
    {
        check_err_B1500(vi_B1500, ret);
        agb1500_setSwitch(vi_B1500, agb1500_CH_ALL, OFF);
        return ERR_CLRSAMPLE;
    }

    ret = agb1500_setSwitch(vi_B1500, agb1500_CH_ALL, OFF);
    if(ret!=VI_SUCCESS)
    {
        check_err_B1500(vi_B1500, ret);
        return ERR_SWITCH_OFF;
    }

    return VI_SUCCESS;
}*/
/*-----------------------------------------------------------------------------------------------*/
/*int StressTesting::NBTI_Parall()
{
    ViStatus ret;

    int     i,
            x,
            y,
            gate,
            cnt,
            N = nbti.size(),
            Result = 1;

    QVector<NBTI>::iterator it;
    double  TimeLimit,
            Vgstress,
            Idmax,
            Vtsat;

    double  Stress_time,
            Interval,
            Compl_Id;

    int     nSampl;

    double  Meas_time[NUM_HC_TIMES] = {0.0,
                                       2.0,
                                       5.0,
                                       10.0,
                                       20.0,
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

    ViInt32     pts,
                index[N*NBTI_SAMPLES],
                idx[N*SAMPLES_PER_CHANN],
                mode[N],
                ch[N + 1];

    ViReal64    rng[N],
                values[N*NBTI_SAMPLES],
                val[N*SAMPLES_PER_CHANN];

    memset(ch, 0, (N+1)*sizeof(ViInt32));
    TimeLimit = nbti.begin()->StressTime;
    gate = nbti.begin()->gate;
    Vgstress = nbti.begin()->Vgstress;
    Idmax = nbti.begin()->Idmax;
    Vtsat = nbti.begin()->Vtsat;

    OpenCommMatrix(vi_2200a);

    if(abort_meas)
        return ERR_ABORT;

    //коммутация матрицы
    for(it = nbti.begin(), i=0; it<nbti.end(); it++, i++){
        if(!ConnectPorts2200AVect(vi_2200a, it->gate, it->gate_pins) ||          //gate
           !ConnectPorts2200AVect(vi_2200a, it->drain, it->drain_pins) ||         //drain
            !ConnectPorts2200AVect(vi_2200a, GNDU, it->sb_pins))                //source and body
        {
            return 2;
        }

        ret = agb1500_setSwitch(vi_b1500, it->gate, ON);
        if(ret!=VI_SUCCESS){
            check_err_B1500(vi_b1500, ret);
            return ERR_SWITCH_ON;
        }

        ret = agb1500_setAdcType(vi_b1500, it->gate, agb1500_HSPEED_ADC);
        if(ret!=VI_SUCCESS){
            check_err_B1500(vi_b1500, ret);
            return ERR_ADC;
        }

        ret = agb1500_setSwitch(vi_b1500, it->drain, ON);
        if(ret!=VI_SUCCESS){
            check_err_B1500(vi_b1500, ret);
            return ERR_SWITCH_ON;
        }

        ret = agb1500_setAdcType(vi_b1500, it->drain, agb1500_HSPEED_ADC);
        if(ret!=VI_SUCCESS){
            check_err_B1500(vi_b1500, ret);
            return ERR_ADC;
        }

        ch[i] = it->drain;
        mode[i] = agb1500_IM_MODE;
        rng[i] = agb1500_AUTO_RANGE;//agb1500_FIX_10uA_RANGE;                                     //only fixed range!!!!
    }//end for(it)

    ret = agb1500_setFilter(vi_b1500, agb1500_CH_ALL, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_FILTER;
    }

    //only high speed converter !!!!!!!
    ret = agb1500_setAdc(vi_b1500, agb1500_HSPEED_ADC, agb1500_INTEG_MANUAL, 1, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        return ERR_ADC;
    }

    ret = agb1500_setSampleMode(vi_b1500, agb1500_SMP_LINEAR);//only linear mode!!!
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_SAMPLEMODE;
    }

    //NBTI испытания
    Compl_Id = agb1500_LMA_1uA_RANGE;
    for(i=1; i<NUM_HC_TIMES; i++){
        if(abort_meas){
            Result = ERR_ABORT;
            break;
        }

        if(Meas_time[i]<=TimeLimit)
            Stress_time = Meas_time[i] - Meas_time[i-1] - TIME_ADJUST;
        else
            break;

        if(Stress_time >= 500.0){
            Interval =  Stress_time/SAMPLES_PER_CHANN;
            nSampl = (int)(SAMPLES_PER_CHANN + 1)*0.999;
        }
        else{
            Interval = 0.1;
            nSampl = (int)(Stress_time/Interval + 1)*0.998;
        }

        //------start stress------
        ret = agb1500_setSample(vi_b1500, 0.0, 0.0, Interval, nSampl);
        if(ret!=VI_SUCCESS){
            Result = ERR_SETSAMPLE;
            break;
        }

        ret = agb1500_addSampleSyncIv(vi_b1500, gate, agb1500_VF_MODE, agb1500_LMA_20V_RANGE, Vgstress, Vgstress, Idmax);
        if(ret!=VI_SUCCESS){
            Result = ERR_SAMPLESOURCE;
            break;
        }

        for(it=nbti.begin(); it<nbti.end(); it++){
            ret = agb1500_addSampleSyncIv(vi_b1500, it->drain, agb1500_VF_MODE,
                                          agb1500_LMA_20V_RANGE, it->Vdstress, it->Vdstress, it->Idmax);
            if(ret!=VI_SUCCESS){
                Result = ERR_SAMPLESOURCE;
                break;
            }
        }

        ret = agb1500_sampleIv(vi_b1500, ch, mode, rng, &pts, &idx[0], &val[0], NULL, NULL);
        if(ret!=VI_SUCCESS){
            Result = ERR_SAMPLEIV;
            break;
        }
        //------end stress------


        //
        //
        //проблема в диапазонах измерений!!
        //ошибка в функции от Agilent, Compliance и Range для канала измерений могут быть поменяны местами
        //необходимо проверить все варианты
        //
        //
        //
        //
        //
        //
        //


        //------start measurement------
        ret = agb1500_setSample(vi_b1500, NEGATIVE_INERVAL, TIME_ADJUST, NBTI_INTERVAL, NBTI_SAMPLES);
        if(ret!=VI_SUCCESS){
            Result = ERR_SETSAMPLE;
            break;
        }

        ret = agb1500_addSampleSyncIv(vi_b1500, gate, agb1500_VF_MODE, agb1500_LMA_20V_RANGE, Vgstress, Vtsat, Idmax);
        if(ret!=VI_SUCCESS){
            Result = ERR_SAMPLESOURCE;
            break;
        }

        for(it=nbti.begin(); it<nbti.end(); it++){
            ret = agb1500_addSampleSyncIv(vi_b1500, it->drain, agb1500_VF_MODE,
                                          agb1500_LMA_20V_RANGE, it->Vdstress, it->Vtsat, Compl_Id);
            if(ret!=VI_SUCCESS){
                Result = ERR_SAMPLESOURCE;
                break;
            }
        }

        ret = agb1500_sampleIv(vi_b1500, ch, mode, rng, &pts, &index[0], &values[0], NULL, NULL);
        if(ret<VI_SUCCESS){
            Result = ERR_SAMPLEIV;
            break;
        }
        emit change_test_time(Meas_time[i]);

        //------end measurement------

        y=0;
        cnt = 0;
        //Compl_Id = agb1500_LMA_1uA_RANGE;
        for(x=0; x<pts; x++)
            for(it=nbti.begin(); it<nbti.end(); it++, y++){
                it->Time.push_back(Meas_time[i]);
                it->Ifly.push_back(values[y]);
                if(Compl_Id < values[y])
                    Compl_Id = values[y];
                if(x==pts-1){
                    emit change_NBTI_item(cnt, values[y]);
                    cnt++;
                }
            }
        Compl_Id = SetLimIrange(Compl_Id);
    }//end for(i)   //окончание испытаний

    //QThread::msleep(500);
    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, 0);
        return ERR_ZERO;
    }

    ret = agb1500_clearSampleSync(vi_b1500);
    if(ret!=VI_SUCCESS)
    {
        check_err_B1500(vi_b1500, ret);
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_CLRSAMPLE;
    }

    ret = agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
    if(ret!=VI_SUCCESS)
    {
        check_err_B1500(vi_b1500, ret);
        return ERR_SWITCH_OFF;
    }

    return Result;
}*/
/*-----------------------------------------------------------------------------------------------*/
/*void NBTISaveData_Fly(QVector<NBTI> *t, FILE *file)
{
    QVector<NBTI>::iterator it;
    int     i,
            N = t->size();
    char str[128];
    int steps = t->begin()->Time.size();

    fputs("--- NBTI on-the-fly test ---\n", file);
    fputs("---Initial input data---\n", file);
    //название
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "DUT,%d,,,", i+1);
        else
            sprintf(str, "DUT,%d,,,,\t\t,", i+1);
        fputs(str, file);
    }
    fputs("\n", file);

    //drain
    for(it=t->begin(); it<t->end(); it++){
        fputs("DrainPins,", file);

        if(i==N-1){
            for(i=0; i<it->drain_pins.size(); i++){
                sprintf(str, "%d;", it->drain_pins[i]);
                fputs(str, file);
            }
            fputs(",,,", file);
        }
        else{
            for(i=0; i<it->drain_pins.size(); i++){
                sprintf(str, "%d;", it->drain_pins[i]);
                fputs(str, file);
            }
            fputs(",,,,\t\t,", file);
        }
    }
    fputs("\n", file);

    //gate
    for(it=t->begin(); it<t->end(); it++){
        fputs("GatePins,", file);

        if(i==N-1){
            for(i=0; i<it->gate_pins.size(); i++){
                sprintf(str, "%d;", it->gate_pins[i]);
                fputs(str, file);
            }
            fputs(",,,", file);
        }
        else{
            for(i=0; i<it->gate_pins.size(); i++){
                sprintf(str, "%d;", it->gate_pins[i]);
                fputs(str, file);
            }
            fputs(",,,,\t\t,", file);
        }
    }
    fputs("\n", file);

    //source and body
    for(it=t->begin(); it<t->end(); it++){
        fputs("SourceBodyPins,", file);

        if(i==N-1){
            for(i=0; i<it->sb_pins.size(); i++){
                sprintf(str, "%d;", it->sb_pins[i]);
                fputs(str, file);
            }
            fputs(",,,", file);
        }
        else{
            for(i=0; i<it->sb_pins.size(); i++){
                sprintf(str, "%d;", it->sb_pins[i]);
                fputs(str, file);
            }
            fputs(",,,,\t\t,", file);
        }
    }
    fputs("\n", file);

    //SMU Drain
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "DrainSMU,%d,,,", it->drain_smu);
        else
            sprintf(str, "DrainSMU,%d,,,,\t\t,", it->drain_smu);
        fputs(str, file);
    }
    fputs("\n", file);

    //SMU Gate
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "GateSMU,%d,,,", it->gate_smu);
        else
            sprintf(str, "GateSMU,%d,,,,\t\t,", it->gate_smu);
        fputs(str, file);
    }
    fputs("\n", file);

    //type channel
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            if(it->TypeCh == true)
                sprintf(str, "Channel type,N,,,");
            else
                sprintf(str, "Channel type,P,,,");
        else
            if(it->TypeCh == true)
                sprintf(str, "Channel type,N,,,,\t\t,");
            else
                sprintf(str, "Channel type,P,,,,\t\t,");
        fputs(str, file);
    }
    fputs("\n", file);

    //Width
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Width,%f,um,,", it->W);
        else
            sprintf(str, "Width,%f,um,,,\t\t,", it->W);
        fputs(str, file);
    }
    fputs("\n", file);

    //Length
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Length,%f,um,,", it->L);
        else
            sprintf(str, "Length,%f,um,,,\t\t,", it->L);
        fputs(str, file);
    }
    fputs("\n", file);

    //Tempearture
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Temp,%f,degC,,", it->T);
        else
            sprintf(str, "Temp,%f,degC,,,\t\t,", it->T);
        fputs(str, file);
    }
    fputs("\n", file);

    //Stress time
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "StressTime,%f,s,,", it->StressTime);
        else
            sprintf(str, "StressTime,%f,s,,,\t\t,", it->StressTime);
        fputs(str, file);
    }
    fputs("\n", file);

    //Vdlin
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Vdlin,%f,V,,", it->Vtlin);
        else
            sprintf(str, "Vdlin,%f,V,,,\t\t,", it->Vtlin);
        fputs(str, file);
    }
    fputs("\n", file);

    //Vdsat
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Vdsat,%f,V,,", it->Vtsat);
        else
            sprintf(str, "Vdsat,%f,V,,,\t\t,", it->Vtsat);
        fputs(str, file);
    }
    fputs("\n", file);

    //Vgstart
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Vgstart,%f,V,,", it->Vgstart);
        else
            sprintf(str, "Vgstart,%f,V,,,\t\t,", it->Vgstart);
        fputs(str, file);
    }
    fputs("\n", file);

    //Vgstop
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Vgstop,%f,V,,", it->Vgstop);
        else
            sprintf(str, "Vgstop,%f,V,,,\t\t,", it->Vgstop);
        fputs(str, file);
    }
    fputs("\n", file);

    //Ith
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Ith,%e,A/sq,,", it->Ith);
        else
            sprintf(str, "Ith,%e,A/sq,,,\t\t,", it->Ith);
        fputs(str, file);
    }
    fputs("\n", file);

    //Vgstress
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Vgstress,%f,V,,", it->Vgstress);
        else
            sprintf(str, "Vgstress,%f,V,,,\t\t,", it->Vgstress);
        fputs(str, file);
    }
    fputs("\n", file);

    //Vdstress
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Vdstress,%f,V,,", it->Vdstress);
        else
            sprintf(str, "Vdstress,%f,V,,,\t\t,", it->Vdstress);
        fputs(str, file);
    }
    fputs("\n", file);

    //Idmax
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Idmax,%f,A,,", it->Idmax);
        else
            sprintf(str, "Idmax,%f,A,,,\t\t,", it->Idmax);
        fputs(str, file);
    }
    fputs("\n", file);

    fputs("---Measured stress data---\n", file);

    //Stress values names
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Time(s),Id_fly(A),,,");
        else
            sprintf(str, "Time(s),Id_fly(A),,,,\t\t,");
        fputs(str, file);
    }
    fputs("\n", file);

    //data
    for(int j=0; j<steps; j++){
        for(it=t->begin(), i=0; it<t->end(); it++, i++){
            if(i==N-1)
                sprintf(str, "%f,%e,,,", it->Time[j], it->Ifly[j]);
            else
                sprintf(str, "%f,%e,,,,\t\t,", it->Time[j], it->Ifly[j]);
            fputs(str, file);
        }
        fputs("\n", file);
    }
    fputs("\n", file);


    fputs("---Analyse data---\n", file);
    //Ion_init
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Ion_init,%e,A,,", it->Ion_init);
        else
            sprintf(str, "Ion_init,%e,A,,,\t\t,", it->Ion_init);
        fputs(str, file);
    }
    fputs("\n", file);

    //Ioff_init
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Ioff_init,%e,A,,", it->Ioff_init);
        else
            sprintf(str, "Ioff_init,%e,A,,,\t\t,", it->Ioff_init);
        fputs(str, file);
    }
    fputs("\n", file);

    //Vtlin_init
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Vtlin_init,%f,V,,", it->Vtlin_init);
        else
            sprintf(str, "Vtlin_init,%f,V,,,\t\t,", it->Vtlin_init);
        fputs(str, file);
    }
    fputs("\n", file);

    //Vtsat_init
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Vtsat_init,%f,V,,", it->Vtsat_init);
        else
            sprintf(str, "Vtsat_init,%f,V,,,\t\t,", it->Vtsat_init);
        fputs(str, file);
    }
    fputs("\n", file);

    //Ion_end
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Ion_end,%e,A,,", it->Ion_end);
        else
            sprintf(str, "Ion_end,%e,A,,,\t\t,", it->Ion_end);
        fputs(str, file);
    }
    fputs("\n", file);

    //Ioff_end
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Ioff_end,%e,A,,", it->Ioff_end);
        else
            sprintf(str, "Ioff_end,%e,A,,,\t\t,", it->Ioff_end);
        fputs(str, file);
    }
    fputs("\n", file);

    //Vtlin_end
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Vtlin_end,%f,V,,", it->Vtlin_end);
        else
            sprintf(str, "Vtlin_end,%f,V,,,\t\t,", it->Vtlin_end);
        fputs(str, file);
    }
    fputs("\n", file);

    //Vtsat_end
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Vtsat_end,%f,V,,", it->Vtsat_end);
        else
            sprintf(str, "Vtsat_end,%f,V,,,\t\t,", it->Vtsat_end);
        fputs(str, file);
    }
    fputs("\n", file);
}*/
/*-----------------------------------------------------------------------------------------------*/
//сохранение измеренных данных
/*void NBTISaveData_Classic(QVector<NBTI> *t, FILE *file)
{
    QVector<NBTI>::iterator it;
    int     i,
            N = t->size();
    char str[128];

    fputs("--- NBTI test ---\n", file);
    fputs("---Initial input data---\n", file);
    //название
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "DUT,%d,,,", i+1);
        else
            sprintf(str, "DUT,%d,,,,\t\t,", i+1);
        fputs(str, file);
    }
    fputs("\n", file);

    //drain
    for(it=t->begin(); it<t->end(); it++){
        fputs("DrainPins,", file);

        if(i==N-1){
            for(i=0; i<it->drain_pins.size(); i++){
                sprintf(str, "%d;", it->drain_pins[i]);
                fputs(str, file);
            }
            fputs(",,,", file);
        }
        else{
            for(i=0; i<it->drain_pins.size(); i++){
                sprintf(str, "%d;", it->drain_pins[i]);
                fputs(str, file);
            }
            fputs(",,,,\t\t,", file);
        }
    }
    fputs("\n", file);

    //gate
    for(it=t->begin(); it<t->end(); it++){
        fputs("GatePins,", file);

        if(i==N-1){
            for(i=0; i<it->gate_pins.size(); i++){
                sprintf(str, "%d;", it->gate_pins[i]);
                fputs(str, file);
            }
            fputs(",,,", file);
        }
        else{
            for(i=0; i<it->gate_pins.size(); i++){
                sprintf(str, "%d;", it->gate_pins[i]);
                fputs(str, file);
            }
            fputs(",,,,\t\t,", file);
        }
    }
    fputs("\n", file);

    //source and body
    for(it=t->begin(); it<t->end(); it++){
        fputs("SourceBodyPins,", file);

        if(i==N-1){
            for(i=0; i<it->sb_pins.size(); i++){
                sprintf(str, "%d;", it->sb_pins[i]);
                fputs(str, file);
            }
            fputs(",,,", file);
        }
        else{
            for(i=0; i<it->sb_pins.size(); i++){
                sprintf(str, "%d;", it->sb_pins[i]);
                fputs(str, file);
            }
            fputs(",,,,\t\t,", file);
        }
    }
    fputs("\n", file);

    //SMU Drain
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "DrainSMU,%d,,,", it->drain_smu);
        else
            sprintf(str, "DrainSMU,%d,,,,\t\t,", it->drain_smu);
        fputs(str, file);
    }
    fputs("\n", file);

    //SMU Gate
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "GateSMU,%d,,,", it->gate_smu);
        else
            sprintf(str, "GateSMU,%d,,,,\t\t,", it->gate_smu);
        fputs(str, file);
    }
    fputs("\n", file);

    //type channel
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            if(it->TypeCh == true)
                sprintf(str, "Channel type,N,,,");
            else
                sprintf(str, "Channel type,P,,,");
        else
            if(it->TypeCh == true)
                sprintf(str, "Channel type,N,,,,\t\t,");
            else
                sprintf(str, "Channel type,P,,,,\t\t,");
        fputs(str, file);
    }
    fputs("\n", file);

    //Width
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Width,%f,um,,", it->W);
        else
            sprintf(str, "Width,%f,um,,,\t\t,", it->W);
        fputs(str, file);
    }
    fputs("\n", file);

    //Length
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Length,%f,um,,", it->L);
        else
            sprintf(str, "Length,%f,um,,,\t\t,", it->L);
        fputs(str, file);
    }
    fputs("\n", file);

    //Tempearture
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Temp,%f,degC,,", it->T);
        else
            sprintf(str, "Temp,%f,degC,,,\t\t,", it->T);
        fputs(str, file);
    }
    fputs("\n", file);

    //Stress time
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "StressTime,%f,s,,", it->StressTime);
        else
            sprintf(str, "StressTime,%f,s,,,\t\t,", it->StressTime);
        fputs(str, file);
    }
    fputs("\n", file);

    //Vdlin
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Vdlin,%f,V,,", it->Vtlin);
        else
            sprintf(str, "Vdlin,%f,V,,,\t\t,", it->Vtlin);
        fputs(str, file);
    }
    fputs("\n", file);

    //Vdsat
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Vdsat,%f,V,,", it->Vtsat);
        else
            sprintf(str, "Vdsat,%f,V,,,\t\t,", it->Vtsat);
        fputs(str, file);
    }
    fputs("\n", file);

    //Vgstart
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Vgstart,%f,V,,", it->Vgstart);
        else
            sprintf(str, "Vgstart,%f,V,,,\t\t,", it->Vgstart);
        fputs(str, file);
    }
    fputs("\n", file);

    //Vgstop
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Vgstop,%f,V,,", it->Vgstop);
        else
            sprintf(str, "Vgstop,%f,V,,,\t\t,", it->Vgstop);
        fputs(str, file);
    }
    fputs("\n", file);

    //Ith
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Ith,%e,A/sq,,", it->Ith);
        else
            sprintf(str, "Ith,%e,A/sq,,,\t\t,", it->Ith);
        fputs(str, file);
    }
    fputs("\n", file);

    //Vgstress
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Vgstress,%f,V,,", it->Vgstress);
        else
            sprintf(str, "Vgstress,%f,V,,,\t\t,", it->Vgstress);
        fputs(str, file);
    }
    fputs("\n", file);

    //Vdstress
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Vdstress,%f,V,,", it->Vdstress);
        else
            sprintf(str, "Vdstress,%f,V,,,\t\t,", it->Vdstress);
        fputs(str, file);
    }
    fputs("\n", file);

    //Idmax
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Idmax,%f,A,,", it->Idmax);
        else
            sprintf(str, "Idmax,%f,A,,,\t\t,", it->Idmax);
        fputs(str, file);
    }
    fputs("\n", file);


    fputs("---Measured stress data---\n", file);
    //Stress values names
    for(it=t->begin(), i=0; it<t->end(); it++, i++){
        if(i==N-1)
            sprintf(str, "Time(s),Idfl(A),Idfs(A),Vtlin(V),Vtsat(V)");
        else
            sprintf(str, "Time(s),Idfl(A),Idfs(A),Vtlin(V),Vtsat(V),\t\t,");
        fputs(str, file);
    }
    fputs("\n", file);

    //data
    for(int j=0; j<t->begin()->Time.size(); j++){
        for(it=t->begin(), i=0; it<t->end(); it++, i++){
            if(i==N-1)
                sprintf(str, "%f,%e,%e,%f,%f", it->Time[j], it->Idfl[j], it->Idfs[j], it->Vlin[j], it->Vsat[j]);
            else
                sprintf(str, "%f,%e,%e,%f,%f,\t\t,", it->Time[j], it->Idfl[j], it->Idfs[j], it->Vlin[j], it->Vsat[j]);
            fputs(str, file);
        }
        fputs("\n", file);
    }
    fputs("\n", file);
}*/
/*-----------------------------------------------------------------------------------------------*/

