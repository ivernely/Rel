#include <QTime>
#include "measclasses.h"
#include "eq_control.h"
#include "visa.h"

extern bool abort_meas;
/*-----------------------------------------------------------------------------------------------*/
GOI::GOI()
{
   Clear();
}
/*-----------------------------------------------------------------------------------------------*/
void GOI::Clear()
{
    CAP.clear();

    Number = 0;
    Adc_type = agb1500_HSPEED_ADC;

    T = 0.0;
    TimeLimit = 0.0;

    tstress.clear();
}
/*-----------------------------------------------------------------------------------------------*/
bool GOI::CheckSrc()
{
    for(auto it : CAP)
        if(it.High.Slot == SLOT_GNDU)
            return false;

    return true;
}
/*-----------------------------------------------------------------------------------------------*/
ViStatus GOI::SetSources(const ViInt32 &adc_type)
{
    for(auto it : CAP){
        it.High.ForceMode = agb1500_VF_MODE;
        it.High.MeasMode = agb1500_IM_MODE;
        it.High.Range = agb1500_LMA_10nA_RANGE; //for meas
        ret = agb1500_setSwitch(vi_b1500, it.High.Slot, ON);
        if(ret != VI_SUCCESS)
            return ret;
        ret = agb1500_setAdcType(vi_b1500, it.High.Slot, adc_type);
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
/*bool TDDB::CommMatrix()
{
    if(bUseMatrix){

        OpenCommMatrix(vi_2200a);

        for(auto it: CAP){
            if(it.High.Num!=0 && !ConnectPorts2200AVect(vi_2200a, it.High.Num, it.High.pins))
                return false;

            if(it.Low.Num!=0 && !ConnectPorts2200AVect(vi_2200a, it.Low.Num, it.Low.pins))
                return false;
        }
    }

    return true;
}
/*-----------------------------------------------------------------------------------------------*/
void GOI::RunTest(const QString &Macro, const int &Die)
{
    QString FileName;
    QString curr_date;
    QString status;
    char str[256];

    curr_date = QDateTime::currentDateTime().toString("hh-mm-ss dd.MM.yyyy");
    status = QString("TDDB_%1_die%2").arg(Macro).arg(Die);
    FileName = trUtf8("%1%2_%3.txt").arg(Path_to_save_data).arg(status).arg(curr_date);

    file = fopen(FileName.toLocal8Bit().data(), "w+a");
    if(file != NULL){

        emit change_test_status(status);

        for(auto it : CAP)
            it.clear_data();

        sprintf(str, "%s\n", curr_date.toLocal8Bit().data());
        fputs(str, file);
        sprintf(str, "Die,%d\n", Die);
        fputs(str, file);
        sprintf(str, "Macro,%s\n", Macro.toLocal8Bit().data());
        fputs(str, file);

        fputs("--- TDDB test ---\n", file);

        Adc_type = agb1500_HSPEED_ADC;
        ret = SetSources(Adc_type);
        if(ret!=VI_SUCCESS)
            return;

        ret = agb1500_setAdc(vi_b1500, Adc_type, agb1500_INTEG_PLC, 1, agb1500_FLAG_OFF);
        if(ret!=VI_SUCCESS)
            return;

        /*if(!CommMatrix()){
            ret = agb1500_INSTR_ERROR_DETECTED;
            return;
         }

        if(TDDB_Ileak_Meas(true) == VI_SUCCESS)
            if(TDDB_Istress_Meas() == VI_SUCCESS)
                if(TDDB_Main_Parall() == VI_SUCCESS)
                    TDDB_Ileak_Meas(false);


        TDDBSaveData();*/

        fclose(file);
    }

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    ret = agb1500_clearSampleSync(vi_b1500);
    ret = agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, 0);
}
/*-----------------------------------------------------------------------------------------------*/
/*void StressTesting::TDDB_Main()
{
    QVector<TDDB>::iterator it;
    QString FileName;
    QString curr_date;
    ushort die, subdie;
    QString status;
    char str[256];

    prober.LightOff();

    if(bNeedCal){
        emit change_test_status(QString("Calibration"));

        ret = agb1500_cmd(vi_b1500, ViString("CA"));
        if(ret != VI_SUCCESS){
            emit finished_test();
            return;
        }
        else{
            ret = agb1500_cmdString_Q(vi_b1500,ViString("*OPC?"), 255, str);
            if(ret != VI_SUCCESS){
                emit finished_test();
                return;
            }
        }
    }

    emit change_test_status(QString("TDDB testing"));

    if(VI_SUCCESS == TDDB_Ileak_Meas(true))
        if(VI_SUCCESS == TDDB_Istress_Meas())
            if(VI_SUCCESS == TDDB_Main_Parall())
                TDDB_Ileak_Meas(false);

    //создание файлов для сохранения
    if(1){
        prober.TestSequence();
        prober.UnContact();
        prober.FirstDie();

        for(die=0; die<prober.NumTestDie; die++){
            for(subdie=0; subdie<prober.TestSubdie.size(); subdie++){

                if(prober.TestSubdie.at(subdie)==0){
                    prober.NextSubDie();
                    continue;
                }


                }//end if(file!=NULL)

                if(abort_meas)
                    break;

                prober.NextSubDie();
            }//end step on subdies

            if(abort_meas)
                break;
            prober.NextDie();
        }//end step on dies
    }
    else{
        curr_date = QDateTime::currentDateTime().toString("hh-mm-ss dd.MM.yyyy");
        FileName = trUtf8("%1TDDB_%2.txt").arg(Path_to_save_data).arg(curr_date);
        file = fopen(FileName.toLocal8Bit().data(), "w+a");

        if(file != NULL){
            fputs("--- TDDB test ---\n", file);
            TDDBSaveHeader(&tddb, file);

            if(VI_SUCCESS == TDDB_Ileak_Meas(true))
                    if(VI_SUCCESS == TDDB_Istress_Meas())
                        if(VI_SUCCESS == TDDB_Main_Parall())
                            TDDB_Ileak_Meas(false);

            DetectTDDBFailureType(&tddb);
            TDDBSaveData(&tddb, file);
        }//end if(file != NULL)
    }

    prober.LightOff();
    emit finished_test();
}*/
/*-----------------------------------------------------------------------------------------------*/
//измерение тока утечки для каждой структуры
//проводится в начале и в конце испытаний
ViStatus GOI::TDDB_Ileak_Meas(const bool &bInitOrEnd)
{
    QVector<Capacitor>::iterator it;

    int N = CAP.size();
    int i;

    ViInt32 ch[N+1],
            mode[N];

    ViReal64    range[N],
                values[N];

    memset(ch, 0, (N+1)*sizeof(ViInt32));
    memset(mode, 0, N*sizeof(ViInt32));
    memset(range, 0, N*sizeof(ViReal64));
    memset(values, 0, N*sizeof(ViReal64));

    if(abort_meas)
        return ret=ERR_ABORT;

    ret = agb1500_setAdc(vi_b1500, Adc_type, agb1500_INTEG_PLC, 10, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS)
        return ret;

    for(it=CAP.begin(), i=0; it<CAP.end(); it++, i++){
        ret = agb1500_force(vi_b1500, it->High.Slot, it->High.ForceMode, agb1500_AUTO_RANGE, \
                                                        it->Vuse, it->High.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return ret;

        ch[i] = it->High.Slot;
        mode[i] = it->High.MeasMode;
        range[i] = it->High.Range;
    }

    ret = agb1500_measureM(vi_b1500, ch, mode, range, &values[0],  NULL,  NULL);
    if(ret!=VI_SUCCESS)
        return ret;

    if(bInitOrEnd)
        for(it=CAP.begin(), i=0; it<CAP.end(); it++, i++)
            it->Iinit = values[i];
    else
        for(it=CAP.begin(), i=0; it<CAP.end(); it++, i++)
            it->Iend = values[i];

    return VI_SUCCESS;
}
/*-----------------------------------------------------------------------------------------------*/
//измерение тока при стрессовых условиях для каждой структуры
//проводится в начале испытаний для выставления фиксированного диапазона при испытаниях
ViStatus GOI::TDDB_Istress_Meas()
{
    QVector<Capacitor>::iterator it;

    int N = CAP.size();
    int i;

    ViInt32 ch[N+1],
            mode[N];

    ViReal64    range[N],
                Istress[N],
                Isilc[N];


    memset(ch, 0, (N+1)*sizeof(ViInt32));
    memset(mode, 0, N*sizeof(ViInt32));
    memset(range, 0, N*sizeof(ViReal64));
    memset(Istress, 0, N*sizeof(ViReal64));
    memset(Isilc, 0, N*sizeof(ViReal64));

    if(abort_meas)
        return ERR_ABORT;

    ret = agb1500_setAdc(vi_b1500, Adc_type, agb1500_INTEG_AUTO, 25, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS)
        return ret;

    for(it=CAP.begin(), i=0; it<CAP.end(); it++, i++){
        ret = agb1500_force(vi_b1500, it->High.Slot, it->High.ForceMode, agb1500_AUTO_RANGE, \
                                                    it->Vstress, it->High.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return ret;

        ch[i] = it->High.Slot;
        mode[i] = it->High.MeasMode;
        range[i] = it->High.Range;
    }

    ret = agb1500_measureM(vi_b1500, ch, mode, range, &Istress[0],  NULL,  NULL);
    if(ret!=VI_SUCCESS)
        return ret;


    for(auto cp: CAP){
        ret = agb1500_force(vi_b1500, cp.High.Slot, cp.High.ForceMode, agb1500_AUTO_RANGE, \
                                                        cp.Vsilc, cp.High.Compl, agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return ret;
    }

    ret = agb1500_measureM(vi_b1500, ch, mode, range, &Isilc[0],  NULL,  NULL);
    if(ret!=VI_SUCCESS)
        return ERR_MEAS;


    tstress.push_back(0.0);
    for(it=CAP.begin(), i=0; it<CAP.end(); it++, i++){
        it->Istress.push_back(Istress[i]);
        it->Isilc.push_back(Isilc[i]);

        //расчет временной задержки заряда/разряда емкости
        it->twait = (it->Vsilc/Isilc[i])*(3.9*8.85E-12*it->W*it->L*1E-12/it->tox);
        if(it->twait < 0.01)
            it->twait = 0.01;

        it->Qbd.push_back(0.0);
        it->Inoise.push_back(0.0);
    }

    return VI_SUCCESS;
}
/*-----------------------------------------------------------------------------------------------*/
ViStatus GOI::TDDB_Main_Parall()
{
    QTime tm1, tm2;
    double value;

    int     i,
            j;

    const int N_SILC = agb1500_SMP_POINT_MIN;

    QVector<Capacitor>::iterator it;

    double Interval,
            charge_time,
            StressTime;
    double sum, sum2;
    double StepTime;
    int SubSteps;

    int N = CAP.size();

    ViInt32     ch[N+1],
                mode[N],
                status[N],
                silc_points, stress_points,
                idx_silc[N_SILC*N];

    ViReal64    range_stress[N],
                range_silc[N],
                Isilc[N_SILC*N];

    memset(ch, 0, (N+1)*sizeof(ViInt32));
    memset(status, 0, N*sizeof(ViInt32));
    memset(mode, 0, N*sizeof(ViInt32));
    memset(range_stress, 0, N*sizeof(ViReal64));
    memset(range_silc, 0, N*sizeof(ViReal64));

    StepTime = TimeLimit/TDDB_NUM_STEPS;
    Interval = TDDB_MIN_STEPTIME;
    SubSteps = (int)round(StepTime/Interval);

    if(SubSteps < 1)
        SubSteps = 1;

    if(SubSteps > (int)(100001/N)){
        SubSteps = (int)(100001/N);
        Interval = StepTime/SubSteps;
    }

    ViInt32     idx_stress[SubSteps*N+1];
    ViReal64    Istress[SubSteps*N+1],
                ts[SubSteps*N+1];

    for(it=CAP.begin(), i=0; it<CAP.end(); it++, i++){
        ch[i] = it->High.Slot;
        mode[i] = it->High.MeasMode;

        range_stress[i] = SetFixIrange(it->Istress_init);
        range_silc[i] = SetFixIrange(it->Isilc_init);

        if(it->twait > charge_time)
            charge_time = it->twait;
    }

    ret = agb1500_setAdc(vi_b1500, Adc_type, agb1500_INTEG_AUTO, 7, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS)
        return ret;

    //тип подачи напряжения
    ret = agb1500_setSampleMode(vi_b1500, agb1500_SMP_LINEAR);
    if(ret!=VI_SUCCESS)
        return ret;

    //остановка испытаний
    ret = agb1500_stopMode(vi_b1500, OFF, agb1500_LAST_START);
    if(ret!=VI_SUCCESS)
        return ret;

    StressTime = 0.0;
    //-------испытания----------
    while(StressTime<TimeLimit){

        //корректировка диапазонов измерений тока Isilc
        for(it=CAP.begin(), i=0; it<CAP.end(); it++, i++){
            //!!!!!!!!!!!!!!!!
            //обязателен фиксированный диапазон!!!!!!!!!!!!!!!!!!!!!!!!!
            //!!!!!!!!!!!!!!!!
            range_silc[i] = SetFixIrange(it->Isilc[it->Isilc.size()-1]);
            range_stress[i] = SetFixIrange(it->Istress[it->Istress.size()-1]);

            ret = agb1500_clearSampleSync(vi_b1500);
            if(ret!=VI_SUCCESS)
                break;
            ret = agb1500_addSampleSyncIv(vi_b1500, it->High.Slot, it->High.ForceMode, \
                                          SetLimVrange(it->Vsilc), it->Vsilc, it->Vsilc, it->High.Compl);
            if(ret!=VI_SUCCESS)
                break;
        }

        if(ret!=VI_SUCCESS)
            break;

        //----------------Isilc MEAS-----------------------
        //временные интервалы и кол-во шагов
        ret = agb1500_setSample(vi_b1500, charge_time, 0.0, TDDB_MIN_STEPTIME, N_SILC);
        if(ret!=VI_SUCCESS)
            break;

        ret = agb1500_sampleIv(vi_b1500, ch, mode, range_silc, &silc_points, &idx_silc[0], &Isilc[0], &status[0], NULL);
        if(ret!=VI_SUCCESS)
            break;


        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         // сделать обработку отключения каналов при достижении compliance или осцилляции
         // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //


        //если переполнение ADC-конвертера
        for(it=CAP.begin(), i=0; it<CAP.end(); it++, i++)
            if(status[i]&1){
                ret = agb1500_clearSampleSync(vi_b1500);
                agb1500_force(vi_b1500, it->High.Slot, it->High.ForceMode, agb1500_AUTO_RANGE, it->Vsilc, it->High.Compl, agb1500_POL_AUTO);
                agb1500_spotMeas(vi_b1500, it->High.Slot, it->High.MeasMode, agb1500_AUTO_RANGE, &value, NULL, NULL);
                Isilc[i] = value;
            }

        //----------------STRESS-----------------------
        //установка каналов для стресса
        for(it=CAP.begin(), i=0; it<CAP.end(); it++, i++){
            ret = agb1500_clearSampleSync(vi_b1500);
            if(ret!=VI_SUCCESS)
                break;

            ret = agb1500_addSampleSyncIv(vi_b1500, it->High.Slot, it->High.ForceMode, \
                                          SetLimVrange(it->Vstress), it->Vsilc, it->Vstress, it->High.Compl);
            if(ret!=VI_SUCCESS)
                break;
        }

        if(ret!=VI_SUCCESS)
            break;

        //временные интервалы и кол-во шагов
        ret = agb1500_setSample(vi_b1500, 0.0, 0.0, Interval, SubSteps);
        if(ret!=VI_SUCCESS)
            break;

        ret = agb1500_resetTimestamp(vi_b1500);
        if(ret!=VI_SUCCESS)
            break;

        tm1 = QTime::currentTime();
        ret = agb1500_sampleIv(vi_b1500, ch, mode, range_stress, &stress_points, &idx_stress[0],\
                                                                                    &Istress[0], &status[0], &ts[0]);
        tm2 = QTime::currentTime();

        if(ret!=VI_SUCCESS)
            break;

        //если переполнение ADC-конвертера
        for(it=CAP.begin(), i=0; it<CAP.end(); it++, i++)
            if(status[i]&1){
                agb1500_force(vi_b1500, it->High.Slot, it->High.ForceMode, agb1500_AUTO_RANGE, \
                                                                            it->Vstress, it->High.Compl, agb1500_POL_AUTO);
                agb1500_spotMeas(vi_b1500, it->High.Slot, it->High.MeasMode, agb1500_AUTO_RANGE, &value, NULL, NULL);
                Istress[i] = value;
            }

        //ts = tm1.msecsTo(tm2)/1000.0;
        //StressTime = StressTime + ts;
        StressTime = StressTime + ts[(stress_points-1)*N];

        emit change_test_time(StressTime);

        tstress.push_back(StressTime);

        for(i=0; i<stress_points*N; i=i+N){
            for(it=CAP.begin(), j=0; it<CAP.end(); it++, j++){
                it->Isilc.push_back(Isilc[j]);
                it->Istress.push_back(Istress[i+j]);
            }//end for(it)
        }//end for(i)

    }//end while
    //--------окончание испытаний-------

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS)
        return ret;

    ret = agb1500_clearSampleSync(vi_b1500);
    if(ret!=VI_SUCCESS)
        return ret;

    //расчет параметров Inoise и Qbd
    for(auto cp : CAP)
        for(i=1; i<cp.Istress.size();i++){
            if(i>=STEPS){
                sum = SummDiff(cp.Istress, i-STEPS, i);
                sum2 = Summ2Diff(cp.Istress, i-STEPS, i);
                cp.Inoise.push_back(sqrt(fabs((sum2-sum*sum/5)/4)));
            }
            else
               cp.Inoise.push_back(0.0);

            cp.Qbd.push_back(fabs(cp.Istress[i])*(tstress.at(i) - tstress.at(i-1))/cp.W/cp.L*1E+8 + cp.Qbd[i-1]);
        }

    return VI_SUCCESS;
}
/*----------------------------------------------------------------------------------------------*/
/*int StressTesting::TDDB_Main_Parall()
{
    ViStatus ret;
    QString src;
    char read_buf[16] = {0};
    ViUInt32    received;
    ViUInt16    st_byte;
    bool        remeas_silc, remeas_stress;

    int     Vsilc_range,
            Vstress_range,
            Istress_range,
            Isilc_range;

    unsigned    i, j;
    int         cnt1,
                row,
                Result = VI_SUCCESS;
    const int   N_SILC = agb1500_SMP_POINT_MIN;

    QVector<TDDB>::iterator it;
    QVector<double> Isilc, Istress;
    double  Interval,
            charge_time,
            StressTime;

    double sum, sum2;
    double StopTime, StepTime;
    int SubSteps, NumChann;

    ViInt32     chann[SLOT_NUM_MAX+1],
                mode[SLOT_NUM_MAX];


    memset(chann, 0, (SLOT_NUM_MAX+1)*sizeof(ViInt32));
    memset(mode, 0, SLOT_NUM_MAX*sizeof(ViInt32));

    NumChann = tddb.size();
    it=tddb.begin();
    charge_time = it->twait;
    StopTime = it->StressTime;
    StepTime = StopTime/TDDB_NUM_STEPS;
    Interval = TDDB_MIN_STEPTIME;
    SubSteps = (int)round(StepTime/Interval);

    if(SubSteps < agb1500_SMP_POINT_MIN)
        SubSteps = agb1500_SMP_POINT_MIN;

    if(SubSteps > (int)(100001/NumChann)){
        SubSteps = (int)(100001/NumChann);
        Interval = StepTime/SubSteps;
    }

    //выключаем все каналы
    agb1500_dcl(vi_b1500);

    if(abort_meas)
        return ERR_ABORT;

    //раскоммутируем матрицу
    OpenCommMatrix(vi_2200a);
    for(it=tddb.begin(), i=0; it<tddb.end(); it++, i++){
        //коммутация матрицы
        if(!ConnectPorts2200AVect(vi_2200a, GNDU, it->LowPin) ||
                !ConnectPort2200A(vi_2200a, it->NumSMU, it->HighPin)){
            return 2;
        }

        //включение каналов
        ret = agb1500_setSwitch(vi_b1500, it->NumSlot, ON);
        if(ret!=VI_SUCCESS){
            check_err_B1500(vi_b1500, ret);
            return ERR_SWITCH_ON;
        }

        //настройка ADC конвертера
        ret = agb1500_setAdcType(vi_b1500, it->NumSlot, agb1500_HSPEED_ADC);
        if(ret!=VI_SUCCESS){
            check_err_B1500(vi_b1500, ret);
            return ERR_ADC;
        }

        it->Qbd.push_back(0.0);
        it->Inoise.push_back(0.0);

        chann[i] = it->NumSlot;
        mode[i] = agb1500_IM_MODE;

        if(it->twait > charge_time)
            charge_time =it->twait;
    }

    //---ADC, Filter, Autozero settings---
    ret = agb1500_setFilter(vi_b1500, agb1500_CH_ALL, FILTER_OFF);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_FILTER;
    }

    ret = agb1500_setAdc(vi_b1500, agb1500_HSPEED_ADC, agb1500_INTEG_AUTO, 16, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_ADC;
    }

    //тип подачи напряжения
    ret = agb1500_setSampleMode(vi_b1500, agb1500_SMP_LINEAR);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_SAMPLEMODE;
    }

    //остановка испытаний
    ret = agb1500_stopMode(vi_b1500, OFF, agb1500_LAST_START);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_STOPMODE;
    }

    ret = GPIB_cmd(vi_b1500, "FMT 21,0");
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_DATAFORMATE;
    }

    src = QString("MM 10");
    for(it=tddb.begin(); it<tddb.end(); it++)
        src = src + QString(",%1").arg(it->NumSlot);


    GPIB_cmd(vi_b1500, "TSC 0");
    StressTime = 0.0;
    Vsilc_range = SetLimVrangeGPIB(it->Vsilc);
    Vstress_range = SetLimVrangeGPIB(it->Vstress);
    //-------испытания----------
    while(StopTime>StressTime){
        //----------------STRESS-----------------------
        //временные интервалы и кол-во шагов
        ret = viPrintf(vi_b1500, ViString("MT 0,%f,%d,0\n"), Interval, SubSteps);

        //установка каналов для Istress
        for(it=tddb.begin(),i=0; it<tddb.end(); it++,i++){
            ret = viPrintf(vi_b1500, ViString("MV %d,%d,0,%f,%f\n"), it->NumSlot, Vstress_range,it->Vstress, 0.02);
            ret = viPrintf(vi_b1500, ViString("CMM %d,1\n"), it->NumSlot);
            Istress_range = SetFixIrangeGPIB(it->Istress[it->Istress.size()-1]);
            ret = viPrintf(vi_b1500, ViString("RI %d,%d\n"), it->NumSlot, Istress_range);
        }

        if(abort_meas){
            Result = ERR_ABORT;
            break;
        }

        ret = GPIB_cmd(vi_b1500, src.toLatin1());
        ret = viPrintf(vi_b1500, ViString("BC\n"));
        ret = viPrintf(vi_b1500, ViString("XE\n"));
        if(ret != VI_SUCCESS)
            return ERR_SAMPLEIV;

        Istress.clear();
        remeas_stress = false;
        for(i=0; i<(unsigned)SubSteps; i++){
            for(it=tddb.begin(); it<tddb.end(); it++){
                memset(read_buf, 0, 16*sizeof(char));
                viRead(vi_b1500, (ViBuf)read_buf, 3, &received);
                st_byte = atoi(read_buf);
                //if(st_byte%2)
                    remeas_stress = true;
                viRead(vi_b1500, (ViBuf)read_buf, 16, &received);
                strtok(read_buf, "I");
                Istress.push_back(atof(strtok(NULL, ",\r\n")));
             }
             StressTime = StressTime + Interval;
        }

        emit change_test_time(StressTime);

        if(remeas_stress){
            //установка каналов для Istress
            for(it=tddb.begin(),i=0; it<tddb.end(); it++,i++){
                ret = viPrintf(vi_b1500, ViString("MV %d,%d,0,%f,%f\n"), it->NumSlot,
                                                                Vstress_range,it->Vstress, 0.02);
                ret = viPrintf(vi_b1500, ViString("CMM %d,1\n"), it->NumSlot);
                ret = viPrintf(vi_b1500, ViString("RI %d,0\n"), it->NumSlot);
            }

            if(abort_meas){
                Result = ERR_ABORT;
                break;
            }

            ret = GPIB_cmd(vi_b1500, src.toLatin1());
            ret = viPrintf(vi_b1500, ViString("BC\n"));
            ret = viPrintf(vi_b1500, ViString("XE\n"));

            if(ret != VI_SUCCESS)
                return ERR_SAMPLEIV;

            Istress.clear();
            for(i=0; i<(unsigned)SubSteps; i++){
                for(it=tddb.begin(); it<tddb.end(); it++){
                    memset(read_buf, 0, 16*sizeof(char));
                    viRead(vi_b1500, (ViBuf)read_buf, 3, &received);
                    viRead(vi_b1500, (ViBuf)read_buf, 16, &received);
                    strtok(read_buf, "I");
                    Istress.push_back(atof(strtok(NULL, ",\r\n")));
                 }
                 StressTime = StressTime + Interval;
            }
        }//if(remeas)

        //----------------Isilc MEAS-----------------------
        //временные интервалы и кол-во шагов
        ret = viPrintf(vi_b1500, ViString("MT 0,%f,%d,%f\n"), TDDB_MIN_INTERVAL, N_SILC, charge_time);

        //установка каналов для Isilc
        for(it=tddb.begin(),i=0; it<tddb.end(); it++,i++){
            ret = viPrintf(vi_b1500, ViString("MV %d,%d,0,%f,%f\n"), it->NumSlot,
                                                                Vsilc_range,it->Vsilc, 0.02);
            ret = viPrintf(vi_b1500, ViString("CMM %d,1\n"), it->NumSlot);
            Isilc_range = SetFixIrangeGPIB(it->Isilc[it->Isilc.size()-1]);
            ret = viPrintf(vi_b1500, ViString("RI %d,%d\n"), it->NumSlot, Isilc_range);
        }

        if(abort_meas){
            Result = ERR_ABORT;
            break;
        }

        ret = GPIB_cmd(vi_b1500, src.toLatin1());
        ret = viPrintf(vi_b1500, ViString("BC\n"));
        ret = viPrintf(vi_b1500, ViString("XE\n"));
        if(ret != VI_SUCCESS)
            return ERR_SAMPLEIV;

        Isilc.clear();
        remeas_silc = false;
        for(i=0; i<N_SILC; i++)
            for(it=tddb.begin(); it<tddb.end(); it++){
                memset(read_buf, 0, 16*sizeof(char));
                viRead(vi_b1500, (ViBuf)read_buf, 3, &received);
                st_byte = atoi(read_buf);
                if(st_byte%2)
                    remeas_silc = true;
                viRead(vi_b1500, (ViBuf)read_buf, 16, &received);
                strtok(read_buf , "I");
                Isilc.push_back(atof(strtok(NULL,",\r\n")));
            }


        if(remeas_silc){
            //повторное измерение Isilc при переполнении ADC
            for(it=tddb.begin(),i=0; it<tddb.end(); it++,i++){
                ret = viPrintf(vi_b1500, ViString("MV %d,%d,0,%f,%f\n"), it->NumSlot,
                                                            Vsilc_range,it->Vsilc, 0.02);
                ret = viPrintf(vi_b1500, ViString("CMM %d,1\n"), it->NumSlot);
                ret = viPrintf(vi_b1500, ViString("RI %d,0\n"), it->NumSlot);
            }

            if(abort_meas){
                Result = ERR_ABORT;
                break;
            }

            ret = GPIB_cmd(vi_b1500, src.toLatin1());
            ret = viPrintf(vi_b1500, ViString("BC\n"));
            ret = viPrintf(vi_b1500, ViString("XE\n"));
            if(ret != VI_SUCCESS)
                return ERR_SAMPLEIV;

            Isilc.clear();
            for(i=0; i<N_SILC; i++)
                for(it=tddb.begin(); it<tddb.end(); it++){
                    viRead(vi_b1500, (ViBuf)read_buf, 3, &received);
                    viRead(vi_b1500, (ViBuf)read_buf, 16, &received);
                    strtok(read_buf , "I");
                    Isilc.push_back(atof(strtok(NULL,",\r\n")));
                }
        }//end if(remeas)

        emit change_test_time(StressTime);

        //копирование измеренных данных
        cnt1 = 0;
        row =0;
        for(i=0; i<(unsigned)SubSteps; i++)
            for(it=tddb.begin(); it<tddb.end(); it++){
                it->Istress.push_back(Istress[cnt1]);
                if(remeas_stress)
                    it->Istress.push_back(Istress[cnt1]);
                if(i==uint(SubSteps-1)){
                    emit change_TDDB_item(Istress[cnt1]*1E+6,row,0);
                    row++;
                }
                cnt1++;
            }

        for(i=0; i<(unsigned)SubSteps; i++){
            cnt1=0;
            row = 0;
            for(j=0; j<N_SILC; j++)
                for(it=tddb.begin(); it<tddb.end(); it++){
                    it->Isilc.push_back(Isilc[cnt1]);
                    if(remeas_stress)
                        it->Isilc.push_back(Isilc[cnt1]);
                    if(i==uint(SubSteps-1)){
                        emit change_TDDB_item(Isilc[cnt1]*1E+9,row,1);
                        row++;
                    }
                    cnt1++;
                }
       }
    }//end while
    //--------окончание испытаний-------


    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, 0);
        return ERR_ZERO;
    }

    ret = agb1500_clearSampleSync(vi_b1500);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_CLRSAMPLE;
    }

    ret = agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        return ERR_SWITCH_OFF;
    }

    //расчет параметров Inoise и Qbd
    for(it=tddb.begin(); it<tddb.end(); it++){
        StressTime = 0.0;
        for(i=1; i<(unsigned)it->Istress.size();i++){
            if(i>=STEPS){
                sum = SummDiff(it->Istress, i-STEPS, i);
                sum2 = Summ2Diff(it->Istress, i-STEPS, i);
                it->Inoise.push_back(sqrt(fabs((sum2-sum*sum/5)/4)));
            }
            else
               it->Inoise.push_back(0.0);

            StressTime = StressTime + Interval;

            it->tstress.push_back(StressTime);
            it->Qbd.push_back(fabs(it->Istress[i])*Interval/it->W/it->L*1E+8 + it->Qbd[i-1]);

        }//end for(i)
    }//end for(it)

    return Result;
}*/
/*----------------------------------------------------------------------------------------------*/
void GOI::DetectTDDBFailureType()
{
    QVector<double>::iterator i;
    int cnt;

    for(auto it:CAP){

        //detect initial failure
        it.FailureType = 0;
        if(fabs(it.Iinit) > INIT_BREAKDOWN*fabs(it.Iuse_init))
            it.FailureType = it.FailureType|TDDB_INIT_FAILURE;
        else
            if(fabs(it.Iinit) > fabs(it.Iuse_init))
                 it.FailureType = it.FailureType|TDDB_YIELD_FAILURE;

        //detect soft failure
        for(i=it.Isilc.begin()+1, cnt=0; i<it.Isilc.end(); i++, cnt++){
            if(fabs(*i)>MIN_CURRENT_FOR_ANALYSE && fabs(*i)>SOFT_BREAKDOWN*fabs(*(i-1))){
                it.FailureType = it.FailureType|TDDB_SOFT_FAILURE;
                it.tbd_soft = tstress.at(cnt);
                it.Ibd_soft = it.Istress.at(cnt);
                it.Isilc_soft = *(i-1);
                it.Inoise_soft = it.Inoise.at(cnt);
                it.qbd_soft = it.Qbd.at(cnt);
                break;
            }
        }

        //detect noise failure
        for(i=it.Inoise.begin()+1, cnt=0; i<it.Inoise.end(); i++, cnt++){
            if(fabs(*i)>MIN_CURRENT_FOR_ANALYSE && fabs(*i)>NOISE_BREAKDOWN*fabs(*(i-1))){
                it.FailureType = it.FailureType + TDDB_NOISE_FAILURE;
                it.tbd_noise = tstress.at(cnt);
                it.Ibd_noise = it.Istress.at(cnt);
                it.Isilc_noise = it.Isilc.at(cnt);
                it.Inoise_noise = *(i-1);
                it.qbd_noise = it.Qbd.at(cnt);
                break;
            }
         }

        //detect hard failure
        for(i=it.Istress.begin()+1, cnt=0; i<it.Istress.end(); i++, cnt++){
            if(fabs(*i)>MIN_CURRENT_FOR_ANALYSE && fabs(*i)>HARD_BREAKDOWN*fabs(*(i-1))){
                it.FailureType = it.FailureType|TDDB_HARD_FAILURE;
                it.tbd_hard = tstress.at(cnt);
                it.Ibd_hard = *(i-1);
                it.Isilc_hard = it.Isilc.at(cnt-1);
                it.Inoise_hard = it.Inoise.at(cnt-1);
                it.qbd_hard = it.Qbd.at(cnt-1);
                break;
            }
        }

        //detect catastrophic failure
        if(fabs(it.Iend)>MIN_CURRENT_FOR_ANALYSE && fabs(it.Iend)>INIT_BREAKDOWN*fabs(it.Iuse_init))
            it.FailureType = it.FailureType|TDDB_INIT_FAILURE;
    }
}
/*-----------------------------------------------------------------------------------------------*/
//запись данных в файл
void GOI::TDDBSaveData()
{
    QString s;
    auto N = tstress.size();

    TDDBSaveHeaderTxt();
    DetectTDDBFailureType();

    fputs("---Measured stress data---\n", file);

    //Stress values names
    s = "";
    for(auto it:CAP)
        s = s + QString("Time(s),Istress(A),Isilc(A),Inoise(A),Qbd(C/cm2),,");
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);


    //data
    for(int j=0; j<N; j++){
        s = "";
        for(auto it:CAP)
            s = s + QString("%1,%2,%3,%4,%5,,").arg(tstress.at(j)).arg(it.Istress.at(j)).arg(it.Isilc.at(j)).\
                                                                                arg(it.Inoise.at(j)).arg(it.Qbd.at(j));
        fputs(s.toLocal8Bit().data(), file);
        fputs("\n", file);
    }

    fputs("---Analyse data---\n", file);
    //Iinit
    s = "";
    for(auto it: CAP)
        s = s + QString("Iuse_init,%1,A,,,,").arg(it.Iinit);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Iend
    s = "";
    for(auto it: CAP)
        s = s + QString("Iuse_final,%1,A,,,,").arg(it.Iend);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Failure type
    s = "";
    for(auto it: CAP)
        s = s + QString("FailType,%1,,,,,").arg(it.FailureType);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //---------------soft data-----------------------------
    s = "";
    for(auto it: CAP)
        s = s + QString("tbd_soft,%1,s,,,,").arg(it.tbd_soft);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    s = "";
    for(auto it: CAP)
        s = s + QString("Ibd_soft,%1,A,,,,").arg(it.Ibd_soft);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    s = "";
    for(auto it: CAP)
        s = s + QString("Isilc_soft,%1,A,,,,").arg(it.Isilc_soft);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    s = "";
    for(auto it: CAP)
        s = s + QString("Inoise_soft,%1,A,,,,").arg(it.Inoise_soft);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    s = "";
    for(auto it: CAP)
        s = s + QString("Qbd_soft,%1,C/cm2,,,,").arg(it.qbd_soft);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n\n", file);

    //---------------noise data-----------------------------
    s = "";
    for(auto it: CAP)
        s = s + QString("tbd_noise,%1,s,,,,").arg(it.tbd_noise);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    s = "";
    for(auto it: CAP)
        s = s + QString("Ibd_noise,%1,A,,,,").arg(it.Ibd_noise);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    s = "";
    for(auto it: CAP)
        s = s + QString("Isilc_noise,%1,A,,,,").arg(it.Isilc_noise);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    s = "";
    for(auto it: CAP)
        s = s + QString("Inoise_noise,%1,A,,,,").arg(it.Inoise_noise);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    s = "";
    for(auto it: CAP)
        s = s + QString("Qbd_noise,%1,C/cm2,,,,").arg(it.qbd_noise);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n\n", file);

    //---------------hard data-----------------------------
    s = "";
    for(auto it: CAP)
        s = s + QString("tbd_hard,%1,s,,,,").arg(it.tbd_hard);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    s = "";
    for(auto it: CAP)
        s = s + QString("Ibd_hard,%1,A,,,,").arg(it.Ibd_hard);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    s = "";
    for(auto it: CAP)
        s = s + QString("Isilc_hard,%1,A,,,,").arg(it.Isilc_hard);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    s = "";
    for(auto it: CAP)
        s = s + QString("Inoise_hard,%1,A,,,,").arg(it.Inoise_hard);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    s = "";
    for(auto it: CAP)
        s = s + QString("Qbd_hard,%1,C/cm2,,,,").arg(it.qbd_hard);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);
}
/*-----------------------------------------------------------------------------------------------*/
void GOI::TDDBSaveHeaderTxt()
{
    QString s;

    fputs("---Initial input data---\n", file);
    //название
    s = "";
    for(auto it: CAP)
        s = s + QString("DUT,%d").arg(it.Index) + SetDelim(',', 7);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //комментарий
    s = "";
    for(auto it: CAP)
        s = s + it.comment + SetDelim(',', 8);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //high pin
    for(auto it: CAP){
        fputs("High pins,", file);
        s = "";
        for(auto n : it.High.pins)
            s = s + QString("%1;").arg(n);
        s = s + SetDelim(',', 7);
        fputs(s.toLocal8Bit().data(), file);
    }
    fputs("\n", file);

    //Low pin
    for(auto it: CAP){
        fputs("Low pins,", file);
        s = "";
        for(auto n : it.Low.pins)
            s = s + QString("%1;").arg(n);
        s = s + SetDelim(',', 7);
        fputs(s.toLocal8Bit().data(), file);
    }
    fputs("\n", file);

    //SMU
    s = "";
    for(auto it: CAP)
        s = s + QString("SMU,%1").arg(it.High.Num) + SetDelim(',', 7);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //type channel
    s = "";
    for(auto it: CAP)
        if(it.TypeCh == NCHANN)
            s = s + QString("Channel type,N") + SetDelim(',', 7);
        else
            s = s + QString("Channel type,P") + SetDelim(',', 7);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Width
    s = "";
    for(auto it: CAP)
        s = s + QString("Width,%1,um").arg(it.W) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Length
    s = "";
    for(auto it: CAP)
        s = s + QString("Length,%1,um").arg(it.L) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Oxide thickness
    s = "";
    for(auto it: CAP)
        s = s + QString("Length,%1,um").arg(it.tox) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Tempearture
    s = "";
    for(auto it: CAP)
        s = s + QString("Temp,%1,degC").arg(T) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Time limit
    s = "";
    for(auto it: CAP)
        s = s + QString("TimeLimit,%1,s").arg(TimeLimit) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Vuse
    s = "";
    for(auto it: CAP)
        s = s + QString("Vuse,%1,V").arg(it.Vuse) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Vsilc
    s = "";
    for(auto it: CAP)
        s = s + QString("Vsilc,%1,V").arg(it.Vsilc) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Vstress
    s = "";
    for(auto it: CAP)
        s = s + QString("Vstress,%1,V").arg(it.Vstress) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Iuse initial
    s = "";
    for(auto it: CAP)
        s = s + QString("Iuse_init,%1,A").arg(it.Iuse_init) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);

    //Istress initial
    s = "";
    for(auto it: CAP)
        s = s + QString("Istress_init,%1,A").arg(it.Istress_init) + SetDelim(',', 6);
    fputs(s.toLocal8Bit().data(), file);
    fputs("\n", file);
}
/*-----------------------------------------------------------------------------------------------*/
