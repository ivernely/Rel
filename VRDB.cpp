#include "measclasses.h"
#include "eq_control.h"
#include "QTime"

extern bool abort_meas;
/*-----------------------------------------------------------------------------------------------*/
VRDB::VRDB()
{
   Clear();
}
/*-----------------------------------------------------------------------------------------------*/
void VRDB::Clear()
{
    /*HighPin = 0;
    LowPin.clear();
    NumSMUHigh = 0;
    NumSlotHigh = 0;
    NumSMULow = 0;
    NumSlotLow = 0;

    bNeedCal = false;
    ret = VI_SUCCESS;

    TypeCh = 1;
    TypeTest = VRDB_TYPE_VRAMP;
    Number = 0;

    W = 0.0;
    L = 0.0;
    T = 0.0;
    tox = 0.0;
    Vuse = 0.0;
    Vstep = 0.0;
    Vstart = 0.0;
    Rate = 0.0;
    Vstop = 0.0;
    Istop = 0.0;
    Imax = 0.0;
    Iuse = 0.0;
    Slope = 0.0;

    Vg0 = 0.0;
    VlowE = 0.0;
    Ig0 = 0.0;
    IlowE = 0.0;
    Iuse_before = 0.0;
    Iuse_after = 0.0;

    t_hold = 0.0;
    t_delay = 0.0;
    t_sdelay = 0.0;
    period = 0.0;

    points = 0;

    Ibd = 0.0;
    Qbd = 0.0;
    qbd = 0.0;
    Vbd = 0.0;
    tbd = 0.0;
    Isilcbd = 0.0;

    Ibd_slope = 0.0;
    Qbd_slope = 0.0;
    qbd_slope = 0.0;
    Vbd_slope = 0.0;
    tbd_slope = 0.0;
    Isilcbd_slope = 0.0;

    Vstress.clear();
    Istress.clear();
    tstress.clear();
    qstress.clear();
    Isilc.clear();

    FailureType = 0;*/
}
/*-----------------------------------------------------------------------------------------------*/
bool VRDB::CheckSrc()
{
    return true;
}
/*-----------------------------------------------------------------------------------------------*/
void VRDB::RunTest(const QString &Macro, const int &Die)
{
    return;
}
/*-----------------------------------------------------------------------------------------------*/
ViStatus VRDB::SetSources(const ViInt32 &adc_type)
{
    return VI_SUCCESS;
}
/*-----------------------------------------------------------------------------------------------*/
/*void StressTesting::VRDB_Main()
{
    QString FileName;
    QString curr_date;
    ushort die, subdie;
    QString status;
    char str[256];

    if(vrdb->bNeedCal){
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

    prober.LightOff();
    emit change_test_status(QString("VRAMP testing"));

    //создание файлов для сохранения
    if(prober.bUseProber){
        prober.TestSequence();
        prober.UnContact();
        prober.FirstDie();


        for(die=0; die<prober.NumTestDie; die++){
            for(subdie=0; subdie<prober.TestSubdie.size(); subdie++){

                if(prober.TestSubdie.at(subdie)==0){
                    prober.NextSubDie();
                    continue;
                }

                status = QString("VRamp die%1 sub%2").arg(die+1).arg(subdie);
                emit change_test_status(status);

                curr_date = QDateTime::currentDateTime().toString("hh-mm-ss dd.MM.yyyy");

                FileName = trUtf8("%1Vramp_Die%2_%3_%4.txt").arg(Path_to_save_data)
                                                    .arg(die+1).arg(prober.SubsiteLabel(prober.TestSubdie.at(subdie))).arg(curr_date);

                file = fopen(FileName.toLocal8Bit().data(), "w+a");
                fputs("--- VRDB RAMP test ---\n", file);
                sprintf(str, "Die,%d\n", die+1);
                fputs(str, file);
                VRDBSaveHeader(vrdb, file);

                if(file != NULL){
                    prober.Contact();

                    vrdb->Ibd = 0.0;
                    vrdb->Qbd = 0.0;
                    vrdb->qbd = 0.0;
                    vrdb->Vbd = 0.0;
                    vrdb->tbd = 0.0;
                    vrdb->Isilcbd = 0.0;

                    vrdb->Ibd_slope = 0.0;
                    vrdb->Qbd_slope = 0.0;
                    vrdb->qbd_slope = 0.0;
                    vrdb->Vbd_slope = 0.0;
                    vrdb->tbd_slope = 0.0;
                    vrdb->Isilcbd_slope = 0.0;

                    vrdb->Vstress.clear();
                    vrdb->Istress.clear();
                    vrdb->tstress.clear();
                    vrdb->qstress.clear();
                    vrdb->Isilc.clear();

                    vrdb->FailureType = 0;

                    vrdb->ret = VRDB_Ileak(vrdb, 1);
                    if(vrdb->ret == VI_SUCCESS){
                        vrdb->ret = VRDB_Ileak(vrdb, 2);
                        if(vrdb->ret == VI_SUCCESS){
                            vrdb->ret = VRDB_Ileak(vrdb, 3);
                            if(vrdb->ret == VI_SUCCESS){
                                vrdb->ret = VRDB_Ramp(vrdb);
                                if(vrdb->ret == VI_SUCCESS){
                                    vrdb->ret = VRDB_Ileak(vrdb, 4);
                                    if(vrdb->ret != VI_SUCCESS){
                                        sprintf(str, "Stop at Iuse meas after test\n");
                                        fputs(str, file);
                                        sprintf(str, "Status=%d\n", vrdb->ret);
                                        fputs(str, file);
                                    }
                                    else{
                                        DetectVRDBFailureType(vrdb);
                                        VRDBSaveData(vrdb, file, vrdb->TypeTest);
                                    }
                                }
                                else{
                                    sprintf(str, "Stop at Vramp test\n");
                                    fputs(str, file);
                                    sprintf(str, "Status=%d\n", vrdb->ret);
                                    fputs(str, file);
                                }
                            }
                            else{
                                sprintf(str, "Stop at Iuse meas before test\n");
                                fputs(str, file);
                                sprintf(str, "Status=%d\n", vrdb->ret);
                                fputs(str, file);
                            }
                        }
                        else{
                            sprintf(str, "Stop at IlowE\n");
                            fputs(str, file);
                            sprintf(str, "Status=%d\n", vrdb->ret);
                            fputs(str, file);
                        }
                    }
                    else{
                        sprintf(str, "Stop at Ig0\n");
                        fputs(str, file);
                        sprintf(str, "Status=%d\n", vrdb->ret);
                        fputs(str, file);
                    }

                    prober.UnContact();
                    fclose(file);
                }

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
        FileName = trUtf8("%1Vramp_%2.txt").arg(Path_to_save_data).arg(curr_date);
        file = fopen(FileName.toLocal8Bit().data(), "w+a");

        if(file != NULL){
            fputs("--- VRDB RAMP test ---\n", file);
            VRDBSaveHeader(vrdb, file);

            if(VI_SUCCESS != VRDB_Ileak(vrdb, 1))
                emit finished_test();
            else
                if(VI_SUCCESS != VRDB_Ileak(vrdb, 2))
                    emit finished_test();
                else
                    if(VI_SUCCESS != VRDB_Ileak(vrdb, 3))
                        emit finished_test();
                    else
                        if(VI_SUCCESS != VRDB_Ramp(vrdb))
                            emit finished_test();
                        else
                            if(VI_SUCCESS != VRDB_Ileak(vrdb, 4))
                                emit finished_test();
                            else{
                                DetectVRDBFailureType(vrdb);
                                VRDBSaveData(vrdb, file, vrdb->TypeTest);
                            }
        }//end if(file != NULL)
    }

    prober.LightOff();
    emit finished_test();
}*/
/*-----------------------------------------------------------------------------------------------*/
/*void StressTesting::UPR_Main()
{
    QString FileName;
    QString curr_date;
    ushort die, subdie;
    QString status;
    char str[256];

    if(vrdb->bNeedCal){
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

    prober.LightOff();
    emit change_test_status(QString("UPR testing"));

    //создание файлов для сохранения
    if(prober.bUseProber){
        prober.TestSequence();
        prober.UnContact();
        prober.FirstDie();


        for(die=0; die<prober.NumTestDie; die++){
            for(subdie=0; subdie<prober.TestSubdie.size(); subdie++){

                if(prober.TestSubdie.at(subdie)==0){
                    prober.NextSubDie();
                    continue;
                }

                status = QString("UPR die%1 sub%2").arg(die+1).arg(subdie);
                emit change_test_status(status);

                curr_date = QDateTime::currentDateTime().toString("hh-mm-ss dd.MM.yyyy");

                FileName = trUtf8("%1UPR_Die%2_%3_%4.txt").arg(Path_to_save_data)
                                                    .arg(die+1).arg(prober.SubsiteLabel(prober.TestSubdie.at(subdie))).arg(curr_date);

                file = fopen(FileName.toLocal8Bit().data(), "w+a");
                fputs("--- VRDB UPR test ---\n", file);
                sprintf(str, "Die,%d\n", die+1);
                fputs(str, file);
                VRDBSaveHeader(vrdb, file);

                if(file != NULL){
                    prober.Contact();

                    vrdb->Ibd = 0.0;
                    vrdb->Qbd = 0.0;
                    vrdb->qbd = 0.0;
                    vrdb->Vbd = 0.0;
                    vrdb->tbd = 0.0;
                    vrdb->Isilcbd = 0.0;

                    vrdb->Ibd_slope = 0.0;
                    vrdb->Qbd_slope = 0.0;
                    vrdb->qbd_slope = 0.0;
                    vrdb->Vbd_slope = 0.0;
                    vrdb->tbd_slope = 0.0;
                    vrdb->Isilcbd_slope = 0.0;

                    vrdb->Vstress.clear();
                    vrdb->Istress.clear();
                    vrdb->tstress.clear();
                    vrdb->qstress.clear();
                    vrdb->Isilc.clear();

                    vrdb->FailureType = 0;


                    vrdb->ret = VRDB_Ileak(vrdb, 1);
                    if(vrdb->ret == VI_SUCCESS){
                        vrdb->ret = VRDB_Ileak(vrdb, 2);
                        if(vrdb->ret == VI_SUCCESS){
                            vrdb->ret = VRDB_Ileak(vrdb, 3);
                            if(vrdb->ret == VI_SUCCESS){
                                vrdb->ret = VRDB_UPR(vrdb);
                                if(vrdb->ret == VI_SUCCESS){
                                    vrdb->ret = VRDB_Ileak(vrdb, 4);
                                    if(vrdb->ret != VI_SUCCESS){
                                        sprintf(str, "Stop at Iuse meas after test\n");
                                        fputs(str, file);
                                        sprintf(str, "Status=%d\n", vrdb->ret);
                                        fputs(str, file);
                                    }
                                    else{
                                        DetectVRDBFailureType(vrdb);
                                        VRDBSaveData(vrdb, file, vrdb->TypeTest);
                                    }
                                }
                                else{
                                    sprintf(str, "Stop at UPR test\n");
                                    fputs(str, file);
                                    sprintf(str, "Status=%d\n", vrdb->ret);
                                    fputs(str, file);
                                }
                            }
                            else{
                                sprintf(str, "Stop at Iuse meas before test\n");
                                fputs(str, file);
                                sprintf(str, "Status=%d\n", vrdb->ret);
                                fputs(str, file);
                            }
                        }
                        else{
                            sprintf(str, "Stop at IlowE\n");
                            fputs(str, file);
                            sprintf(str, "Status=%d\n", vrdb->ret);
                            fputs(str, file);
                        }
                    }
                    else{
                        sprintf(str, "Stop at Ig0\n");
                        fputs(str, file);
                        sprintf(str, "Status=%d\n", vrdb->ret);
                        fputs(str, file);
                    }

                    prober.UnContact();
                    fclose(file);
                }

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
        FileName = trUtf8("%1UPR_%2.txt").arg(Path_to_save_data).arg(curr_date);
        file = fopen(FileName.toLocal8Bit().data(), "w+a");

        if(file != NULL){
            fputs("--- VRDB UPR test ---\n", file);
            VRDBSaveHeader(vrdb, file);

            if(VI_SUCCESS != VRDB_Ileak(vrdb, 1))
                emit finished_test();
            else
                if(VI_SUCCESS != VRDB_Ileak(vrdb, 2))
                    emit finished_test();
                else
                    if(VI_SUCCESS != VRDB_Ileak(vrdb, 3))
                        emit finished_test();
                    else
                        if(VI_SUCCESS != VRDB_UPR(vrdb))
                            emit finished_test();
                        else
                            if(VI_SUCCESS != VRDB_Ileak(vrdb, 4))
                                emit finished_test();
                            else{
                                DetectVRDBFailureType(vrdb);
                                VRDBSaveData(vrdb, file, vrdb->TypeTest);
                            }
        }//end if(file != NULL)
    }

    prober.LightOff();
    emit finished_test();
}*/
/*-----------------------------------------------------------------------------------------------*/
//измерение тока утечки для определения целостности
/*int StressTesting::VRDB_Ileak(VRDB *t, int test_type)
{
    ViStatus ret;

    const int NUMMEAS = 1;

    ViInt32 chann[2],
            mode[1],
            points,
            idx[NUMMEAS];

    ViReal64    range[1],
                values[NUMMEAS];


    memset(chann, 0, 2*sizeof(ViInt32));
    memset(mode, 0, sizeof(ViInt32));
    memset(range, 0, sizeof(ViReal64));
    memset(values, 0, sizeof(ViReal64));

    agb1500_dcl(vi_b1500);

    //коммутация матрицы, если матрица подключена
    if(abort_meas)
        return ERR_ABORT;

    if(vi_2200a > VI_NULL){
        OpenCommMatrix(vi_2200a);
        if(!ConnectPorts2200AVect(vi_2200a, t->NumSMULow, t->LowPin) ||
            !ConnectPort2200A(vi_2200a, t->NumSMUHigh, t->HighPin)){
            return ERR_MATRIX;
        }
    }//end if(vi_2200A > VI_NULL)

    //включение канала
    ret = agb1500_setSwitch(vi_b1500, t->NumSlotHigh, ON);
    if(ret!=VI_SUCCESS){
        return ERR_SWITCH_ON;
    }

    //настройка ADC конвертера
    ret = agb1500_setAdcType(vi_b1500, t->NumSlotHigh, agb1500_HRESOLN_ADC);
    if(ret!=VI_SUCCESS){
        return ERR_ADC;
    }

    ret = agb1500_setSwitch(vi_b1500, t->NumSlotLow, ON);
    if(ret!=VI_SUCCESS){
        return ERR_SWITCH_ON;
    }

    ret = agb1500_setAdcType(vi_b1500, t->NumSlotLow, agb1500_HRESOLN_ADC);
    if(ret!=VI_SUCCESS){
        return ERR_ADC;
    }

    //---ADC, Filter, Autozero settings---
    ret = agb1500_setFilter(vi_b1500, agb1500_CH_ALL, agb1500_FLAG_ON);
    if(ret!=VI_SUCCESS){
        return ERR_FILTER;
    }

    ret = agb1500_setAdc(vi_b1500, agb1500_HRESOLN_ADC, agb1500_INTEG_PLC, 10, agb1500_FLAG_ON);
    if(ret!=VI_SUCCESS){
        return ERR_ADC;
    }

    //тип подачи напряжения
    ret = agb1500_setSampleMode(vi_b1500, agb1500_SMP_LINEAR);
    if(ret!=VI_SUCCESS){
        return ERR_SAMPLEMODE;
    }

    //временные интервалы и кол-во шагов
    ret = agb1500_setSample(vi_b1500, 0.0, 0.5, 0.2, NUMMEAS);           //delay - 500ms, integration time - 200ms
    if(ret!=VI_SUCCESS){
        return ERR_SETSAMPLE;
    }

    ret = agb1500_force(vi_b1500, t->NumSlotLow, agb1500_VF_MODE, agb1500_AUTO_RANGE, 0.0, t->Istop, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS){
        return ERR_FORCE;
    }

    switch(test_type){
        case 1:
                ret = agb1500_addSampleSyncIv(vi_b1500, t->NumSlotHigh, agb1500_VF_MODE, agb1500_AUTO_RANGE, 0, t->Vg0, t->Istop);
                if(ret!=VI_SUCCESS){
                    agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
                    return ERR_SAMPLESOURCE;
                }

                chann[0] = t->NumSlotHigh;
                mode[0] = agb1500_IM_MODE;
                range[0] = agb1500_AUTO_RANGE;


            ret = agb1500_sampleIv(vi_b1500, chann, mode, range, &points, &idx[0], &values[0], NULL, NULL);
            if(ret!=VI_SUCCESS){
                agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
                return ERR_SAMPLEIV;
            }

            t->Ig0 = values[0];
            break;

        case 2:
                ret = agb1500_addSampleSyncIv(vi_b1500, t->NumSlotHigh, agb1500_VF_MODE, agb1500_AUTO_RANGE, 0, t->VlowE, t->Istop);
                if(ret!=VI_SUCCESS){
                    agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
                    return ERR_SAMPLESOURCE;
                }

                chann[0] = t->NumSlotHigh;
                mode[0] = agb1500_IM_MODE;
                range[0] = agb1500_AUTO_RANGE;


            ret = agb1500_sampleIv(vi_b1500, chann, mode, range, &points, &idx[0], &values[0], NULL, NULL);
            if(ret!=VI_SUCCESS){
                agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
                return ERR_SAMPLEIV;
            }


            t->IlowE = values[0];
            break;

        case 3:
                ret = agb1500_addSampleSyncIv(vi_b1500, t->NumSlotHigh, agb1500_VF_MODE, agb1500_AUTO_RANGE, 0, t->Vuse, t->Istop);
                if(ret!=VI_SUCCESS){
                    agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
                    return ERR_SAMPLESOURCE;
                }

                chann[0] = t->NumSlotHigh;
                mode[0] = agb1500_IM_MODE;
                range[0] = agb1500_AUTO_RANGE;

            ret = agb1500_sampleIv(vi_b1500, chann, mode, range, &points, &idx[0], &values[0], NULL, NULL);
            if(ret!=VI_SUCCESS){
                agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
                return ERR_SAMPLEIV;
            }


            t->Iuse_before = values[0];
            break;

         case 4:
                ret = agb1500_addSampleSyncIv(vi_b1500, t->NumSlotHigh, agb1500_VF_MODE, agb1500_AUTO_RANGE, 0, t->Vuse, t->Istop);
                if(ret!=VI_SUCCESS){
                    agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
                    return ERR_SAMPLESOURCE;
                }

                chann[0] = t->NumSlotHigh;
                mode[0] = agb1500_IM_MODE;
                range[0] = agb1500_AUTO_RANGE;

            ret = agb1500_sampleIv(vi_b1500, chann, mode, range, &points, &idx[0], &values[0], NULL, NULL);
            if(ret!=VI_SUCCESS){
                agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
                return ERR_SAMPLEIV;
            }

            t->Iuse_after = values[0];
            break;

        default:break;
    }//end switch

    ret = agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS){
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, 0);
        return ERR_ZERO;
    }

    ret = agb1500_clearSampleSync(vi_b1500);
    if(ret!=VI_SUCCESS){
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_CLRSAMPLE;
    }

    ret = agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
    if(ret!=VI_SUCCESS){
        return ERR_SWITCH_OFF;
    }

    return VI_SUCCESS;
}*/
/*-----------------------------------------------------------------------------------------------*/
/*int StressTesting::VRDB_Ramp(VRDB *t)
{
    ViStatus ret;

    int     i;

    ViInt32 points;

    ViReal64    Vstress[VRDB_NUM_STEPS+1],
                Istress[VRDB_NUM_STEPS+1],
                tstress[VRDB_NUM_STEPS+1];

    agb1500_dcl(vi_b1500);

    if(abort_meas)
        return ERR_ABORT;

    //коммутация матрицы, если матрица подключена
    if(vi_2200a > VI_NULL){
        OpenCommMatrix(vi_2200a);
            if(!ConnectPorts2200AVect(vi_2200a, t->NumSMULow, t->LowPin) ||
                !ConnectPort2200A(vi_2200a, t->NumSMUHigh, t->HighPin)){
                    return ERR_MATRIX;
            }
    }//end if(vi_2200A > VI_NULL)

    //включение канала
    ret = agb1500_setSwitch(vi_b1500, t->NumSlotHigh, ON);
    if(ret!=VI_SUCCESS){
        return ERR_SWITCH_ON;
    }

    //настройка ADC конвертера
    ret = agb1500_setAdcType(vi_b1500, t->NumSlotHigh, agb1500_HSPEED_ADC);
    if(ret!=VI_SUCCESS){
        return ERR_ADC;
    }

    ret = agb1500_setSwitch(vi_b1500, t->NumSlotLow, ON);
    if(ret!=VI_SUCCESS){
        return ERR_SWITCH_ON;
    }

    ret = agb1500_setAdcType(vi_b1500, t->NumSlotLow, agb1500_HSPEED_ADC);
    if(ret!=VI_SUCCESS){
        return ERR_ADC;
    }

    //---ADC, Filter, Autozero settings---
    ret = agb1500_setFilter(vi_b1500, agb1500_CH_ALL, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS){
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_FILTER;
    }

    ret = agb1500_setAdc(vi_b1500, agb1500_HSPEED_ADC, agb1500_INTEG_PLC, 1, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS){
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_ADC;
    }

    ret = agb1500_force(vi_b1500, t->NumSlotLow, agb1500_VF_MODE, agb1500_FIX_100mA_RANGE, 0.0, t->Istop, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS){
        return ERR_FORCE;
    }

    ret = agb1500_setIv(vi_b1500, t->NumSlotHigh, agb1500_SWP_VF_SGLLIN, agb1500_LMA_1nA_RANGE, t->Vstart, t->Vstop,
                            t->points, t->t_hold, t->t_delay, t->t_sdelay, t->Istop, OFF);

    if(ret!=VI_SUCCESS){
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_SWPM_SRC;
    }

    ret = agb1500_stopMode(vi_b1500, ON, agb1500_LAST_START);
    if(ret!=VI_SUCCESS){
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_STOPMODE;
    }

    ret = agb1500_resetTimestamp(vi_b1500);
    if(ret!=VI_SUCCESS){
        return ERR_TIMESTAMP;
    }


    //!!!!!!!!!!
    //-------------испытания
    //!!!!!!!!!!
    if(!abort_meas)
        ret = agb1500_sweepIv(vi_b1500, t->NumSlotHigh, agb1500_IM_MODE, agb1500_LMA_1nA_RANGE, &points,
                                                                 &Vstress[0], &Istress[0], NULL, &tstress[0]);

    agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);

    //запись данных
    for(i=0; i<points; i++){
        if(fabs(Vstress[i])<MAX_HV){
            t->Istress.push_back(Istress[i]);
            t->Vstress.push_back(Vstress[i]);
            t->tstress.push_back(tstress[i]);
        }
    }//end for(i)

    t->qstress.push_back(fabs(t->Istress[0])*(t->tstress[0]));

    //вычисление параметров
    for(i=1; i<t->Istress.size(); i++)
        t->qstress.push_back(fabs(t->Istress[i])*(t->tstress[i] - t->tstress[i-1]) + t->qstress[i-1]);

    if((ret&4 || ret&8) && fabs(Istress[points-1])<fabs(t->Istop)){
        t->FailureType = t->FailureType + VRDB_HARD_BRKDWN;
        t->Vbd = t->Vstress[points-1];
        t->tbd = t->tstress[points-1];
        t->Ibd = t->Istress[points-1];
        t->Qbd = t->qstress[points-1]/t->W/t->L*1E+8;
        t->qbd = t->qstress[points-1];
    }

    return VI_SUCCESS;
}*/
/*-----------------------------------------------------------------------------------------------*/
/*int StressTesting::VRDB_UPR(VRDB *t)
{
    ViStatus ret;
    int status;

    int     n,
            High,
            Result = VI_SUCCESS;

    double  Vn,
            q,
            period,
            Ih_range,
            Il_range,
            Maxcompl,
            Vlow,
            meas_time,
            stress_time;

    double t1;

    ViReal64    Ih,
                Il;

    agb1500_dcl(vi_b1500);

    if(abort_meas)
        return ERR_ABORT;

    //коммутация матрицы, если матрица подключена
    if(vi_2200a > VI_NULL){
        OpenCommMatrix(vi_2200a);
            if(!ConnectPorts2200AVect(vi_2200a, t->NumSMULow, t->LowPin) ||
                !ConnectPort2200A(vi_2200a, t->NumSMUHigh, t->HighPin)){
                QMessageBox::critical(0, "Error", QString("Failure during VRAMP matrix commutation!"),
                                   QMessageBox::Ok);
                return 2;
            }
    }//end if(vi_2200A > VI_NULL)

    //включение канала
    ret = agb1500_setSwitch(vi_b1500, t->NumSlotHigh, ON);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        return ERR_SWITCH_ON;
    }

    //настройка ADC конвертера
    ret = agb1500_setAdcType(vi_b1500, t->NumSlotHigh, agb1500_HSPEED_ADC);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        return ERR_ADC;
    }

    ret = agb1500_setSwitch(vi_b1500, t->NumSlotLow, ON);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        return ERR_SWITCH_ON;
    }

    ret = agb1500_setAdcType(vi_b1500, t->NumSlotLow, agb1500_HSPEED_ADC);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        return ERR_ADC;
    }


    //---ADC, Filter, Autozero settings---
    ret = agb1500_setFilter(vi_b1500, agb1500_CH_ALL, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_FILTER;
    }

    ret = agb1500_setAdc(vi_b1500, agb1500_HSPEED_ADC, agb1500_INTEG_MANUAL, 3, agb1500_FLAG_OFF);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);
        return ERR_ADC;
    }

    ret = agb1500_force(vi_b1500, t->NumSlotLow, agb1500_VF_MODE, agb1500_FIX_100mA_RANGE, 0.0, t->Istop, agb1500_POL_AUTO);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_b1500, ret);
        return ERR_FORCE;
    }

    Maxcompl = t->Istop;
    Ih_range = SetLimIrange(t->Ig0);
    Il_range = SetLimIrange(t->Ig0);
    Vlow = t->Vstart;
    High = t->NumSlotHigh;
    meas_time = 0.0;
    period = t->period;
    q = 0.0;

    //-------испытания----------
    for(n=0; n<t->points+1; n++){

        if(abort_meas){
            Result = ERR_ABORT;
            break;
        }

        stress_time = 0.0;
        t1=0.0;
        Vn = t->Vstart + t->Vstep*n;
                                                        //Ih_range
        ret = PulseSpotMeas(vi_b1500, High, Vn, Vlow, agb1500_AUTO_RANGE, Maxcompl, t->t_hold, period, Ih, status);
        if(ret!=VI_SUCCESS){
            check_err_B1500(vi_b1500, ret);
            Result = ERR_MEASP;
            break;
        }


        //если ЦАП переполняется, то переключаем в другой режим измерений
        if(status != VI_SUCCESS){
            //ret = HighSpeedIMeas(vi_b1500, High, Vn, Ih_range, Maxcompl, Ih, t1);
            agb1500_resetTimestamp(vi_b1500);
            agb1500_force(vi_b1500, High, agb1500_VF_MODE, agb1500_AUTO_RANGE, Vn, Maxcompl, agb1500_POL_AUTO);
            ret = agb1500_spotMeas(vi_b1500, High, agb1500_IM_MODE, agb1500_AUTO_RANGE, &Ih, NULL, &t1);
            if(ret!=VI_SUCCESS){
                check_err_B1500(vi_b1500, ret);
                Result = ERR_MEAS;
                break;
            }
            stress_time = t1+fabs(Vn-Vlow)/32000.0;
        }//end if(status==1)

        //HighSpeedIMeas(vi_b1500, High, t->Vuse, Il_range, Maxcompl, Il, t1);
        agb1500_force(vi_b1500, High, agb1500_VF_MODE, agb1500_AUTO_RANGE, t->Vuse, Maxcompl, agb1500_POL_AUTO);
        agb1500_spotMeas(vi_b1500, High, agb1500_IM_MODE, Il_range, &Il, NULL, NULL);
        Ih_range = SetLimIrange(Ih);
        Il_range = SetLimIrange(Il);

        meas_time = meas_time + period + stress_time;

        emit change_test_time(meas_time);
        q = q + fabs(Ih)*(period+stress_time);

        t->Vstress.push_back(Vn);
        t->Istress.push_back(Ih);
        t->qstress.push_back(q);
        t->tstress.push_back(meas_time);
        t->Isilc.push_back(Il);

        if(Ih >= Maxcompl*0.99)
            break;
    }//end for(n)
    //--------окончание испытаний-------

    agb1500_zeroOutput(vi_b1500, agb1500_CH_ALL);
    agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, OFF);

    return Result;
}*/
/*-----------------------------------------------------------------------------------------------*/
//сохранение заголовка в файл
/*void VRDB::VRDBSaveHeader(VRDB *t, FILE *file)
{
    char str[256];
    int i;
    QString dt = CurrentDate();

    fputs("---Initial input data---\n", file);

    //дата
    sprintf(str, "Date,%s", dt.toLocal8Bit().data());
    fputs(str, file);
    fputs("\n", file);

    //комментарий
    sprintf(str, "%s", t->Comment.toLocal8Bit().data());
    fputs(str, file);
    fputs("\n", file);

    //high pin
    sprintf(str, "High pin,%d\n", t->HighPin);
    fputs(str, file);

    //low pins
    fputs("Low pins,", file);
    for(i=0; i<t->LowPin.size(); i++){
        sprintf(str, "%d;", t->LowPin[i]);
        fputs(str, file);
    }
    fputs("\n", file);

    //SMU High
    sprintf(str, "High SMU,SMU%d\n", t->NumSMUHigh);
    fputs(str, file);

    //SMU Low
    sprintf(str, "Low SMU,SMU%d\n", t->NumSMULow);
    fputs(str, file);

    //type channel
    if(t->TypeCh == true)
        sprintf(str, "Channel type,N\n");
    else
        sprintf(str, "Channel type,P\n");
    fputs(str, file);

    //Width
    sprintf(str, "Width,%f,um\n", t->W);
    fputs(str, file);

    //Length
    sprintf(str, "Length,%f,um\n", t->L);
    fputs(str, file);

    //Oxide thickness
    sprintf(str, "tox,%f,nm\n", t->tox);
    fputs(str, file);

    //Tempearture
    sprintf(str, "Temp,%f,degC\n", t->T);
    fputs(str, file);

    //Vuse
    sprintf(str, "Vuse,%f,V\n", t->Vuse);
    fputs(str, file);

    //Vstep
    sprintf(str, "Vstep,%f,V\n", t->Vstep);
    fputs(str, file);

    //Vstart
    sprintf(str, "Vstart,%f,V\n", t->Vstart);
    fputs(str, file);

    //Vstop
    sprintf(str, "Vstop,%f,V\n", t->Vstop);
    fputs(str, file);

    //Rate
    sprintf(str, "Rate,%f,MV/(cm*sec)\n", t->Rate);
    fputs(str, file);

    //period
    sprintf(str, "Period,%f,sec\n", t->period);
    fputs(str, file);

    //Slope
    sprintf(str, "Slope,%f\n", t->Slope);
    fputs(str, file);

    //hold
    sprintf(str, "t_hold,%f,sec\n", t->t_hold);
    fputs(str, file);

    //points
    sprintf(str, "points,%d\n", t->points);
    fputs(str, file);

    //Iuse
    sprintf(str, "Iuse,%f,uA\n", t->Iuse*1E+6);
    fputs(str, file);

    //Istop
    sprintf(str, "Istop,%f,mA\n", t->Istop*1000.0);
    fputs(str, file);

    //Vg0
    sprintf(str, "Vg0,%f,V\n", t->Vg0);
    fputs(str, file);

    //VlowE
    sprintf(str, "VlowE,%f,V\n\n", t->VlowE);
    fputs(str, file);
}*/
/*-----------------------------------------------------------------------------------------------*/
//анализ данных и определение типа пробоя
/*void VRDB::DetectVRDBFailureType(VRDB *t)
{
    int steps;
    double Slope;
    double slope_prev, slope_new;

    Slope = t->Slope;

    if(fabs(t->Ig0) > fabs(t->Iuse)*10)
        t->FailureType = t->FailureType + VRDB_IG0_BRKDWN;

    if(fabs(t->IlowE) > fabs(t->Iuse)*10)
        t->FailureType = t->FailureType + VRDB_LOW_E_BRKDWN;

    if(fabs(t->Iuse) > fabs(t->Iuse)*10)
        t->FailureType = t->FailureType + VRDB_IUSE_BRKDWN;


    //detect slope breakdown    
    steps = t->Vstress.size();
    for(int i=2; i<steps; i++){
        if(t->Istress[i-2]!=0 && t->Istress[i-1]!=0 && t->Istress[i]!=0){
            slope_prev = fabs(log(fabs(t->Istress[i-1]))-log(fabs(t->Istress[i-2])))/
                                            fabs(t->Vstress[i-1] - t->Vstress[i-2]);

            slope_new = fabs(log(fabs(t->Istress[i]))-log(fabs(t->Istress[i-1])))/
                                            fabs(t->Vstress[i] - t->Vstress[i-1]);
        }

        if(fabs(t->Istress[i])>1E-10 && slope_new>=Slope*slope_prev)
        {
            t->FailureType = t->FailureType + VRDB_SLOPE_BRKDWN;
            t->Vbd_slope = t->Vstress[i-1];
            t->tbd_slope = t->tstress[i-1];
            t->Ibd_slope = t->Istress[i-1];
            t->Qbd_slope = t->qstress[i-1]/t->W/t->L*1E+8;
            t->qbd_slope = t->qstress[i-1];
            break;
         }
    }

    //detect hard breakdown
    if(!(t->FailureType&VRDB_HARD_BRKDWN)){
        for(int i=1; i<steps; i++){
            if(fabs(t->Istress[i]) >= fabs(t->Istop))
            {
                t->FailureType = t->FailureType + VRDB_HARD_BRKDWN;
                t->Vbd = t->Vstress[i-1];
                t->tbd = t->tstress[i-1];
                t->Ibd = t->Istress[i-1];
                t->Qbd = t->qstress[i-1]/t->W/t->L*1E+8;
                t->qbd = t->qstress[i-1];
                break;
            }
        }
    }
}*/
/*-----------------------------------------------------------------------------------------------*/
//запись данных в файл
/*void VRDB::VRDBSaveData(VRDB *t, FILE *file, int TestType)
{
    char str[1024];
    int i;
    int steps = t->Istress.size();

    fputs("---Measured stress data---\n", file);
    //Stress values names  
    switch(TestType){
        case VRDB_TYPE_VRAMP:    fputs("Time(s),Vstress(V),Istress(A),q(C)\n", file);
                                for(i=0; i<steps; i++){
                                    sprintf(str, "%f,%f,%e,%e\n", t->tstress[i], t->Vstress[i], t->Istress[i],
                                                                            t->qstress[i]);
                                    fputs(str, file);
                                }
                                fputs("\n", file);
                                break;

        case VRDB_TYPE_VUPR:     fputs("Time(s),Vstress(V),Istress(A),Isilc(A),q(C)\n", file);
                                for(i=0; i<steps; i++){
                                    sprintf(str, "%f,%f,%e,%e,%e\n", t->tstress[i], t->Vstress[i], t->Istress[i], t->Isilc[i],
                                                                            t->qstress[i]);
                                    fputs(str, file);
                                }
                                fputs("\n", file);
                                break;

        default:    fputs("Failed test type\n", file);
    };



    fputs("---Analyse data---\n", file);
    //Ig0
    sprintf(str, "Ig0,%e,A\n", t->Ig0);
    fputs(str, file);

    //IlowE
    sprintf(str, "IlowE,%e,A\n", t->IlowE);
    fputs(str, file);

    //Iuse_before
    sprintf(str, "Iuse_before,%e,A\n", t->Iuse_before);
    fputs(str, file);

    //Iuse_after
    sprintf(str, "Iuse_after,%e,A\n", t->Iuse_after);
    fputs(str, file);

    //Failure type
    sprintf(str, "FailType,%d\n\n", t->FailureType);
    fputs(str, file);

    //---------------noise data-----------------------------
    sprintf(str, "Vbd_slope,%f,V\n", t->Vbd_slope);
    fputs(str, file);

    sprintf(str, "tbd_slope,%f,s\n", t->tbd_slope);
    fputs(str, file);

    sprintf(str, "Ibd_slope,%e,A\n", t->Ibd_slope);
    fputs(str, file);

    sprintf(str, "qbd_slope,%e,C\n", t->qbd_slope);
    fputs(str, file);

    sprintf(str, "Qbd_slope,%e,C/cm2\n\n", t->Qbd_slope);
    fputs(str, file);

    //---------------hard data-----------------------------
    sprintf(str, "Vbd_hard,%f,V\n", t->Vbd);
    fputs(str, file);

    sprintf(str, "tbd_hard,%f,s\n", t->tbd);
    fputs(str, file);

    sprintf(str, "Ibd_hard,%e,A\n", t->Ibd);
    fputs(str, file);

    sprintf(str, "qbd_hard,%e,C\n", t->qbd);
    fputs(str, file);

    sprintf(str, "Qbd_hard,%e,C/cm2\n", t->Qbd);
    fputs(str, file);
}*/
/*-----------------------------------------------------------------------------------------------*/
