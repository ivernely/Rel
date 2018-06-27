#include "measclasses.h"
#include "eq_control.h"

/*-----------------------------------------------------------------------------------------------*/
Test::Test()
{
    prober = new Prober();
    file = NULL;

    bUseMatrix = true;
    bUseProber = false;
    bNeedCal = false;
    b2point = false;

    TestType = 0;
    Number = 0;
    comment = "";

    connect(this, SIGNAL(finished_test()), this, SLOT(ClearConnections()));
    connect(this, SIGNAL(finished_test()), this, SLOT(deleteLater()));
    connect(prober, SIGNAL(cmd_interrupt()), this, SLOT(ClearConnections()));
}
/*-----------------------------------------------------------------------------------------------*/
Test::~Test()
{
    ClearConnections();

    if(file != NULL){
        fclose(file);
        delete file;
        file = NULL;
    }

    if(prober!=NULL)
        delete prober;
}
/*-----------------------------------------------------------------------------------------------*/
bool Test::SetNumSMU(SMU &smu, const QString &s)
{
    QStringList values;

    if(s.length()<2)
        return false;

    values = s.split("SMU", QString::SkipEmptyParts);

    foreach (const QString &s, values){
        bool ok;
        int i = s.toInt(&ok);
        if(ok)
            smu.Num = i;
    }

    if(smu.Num==0 && s.compare("GND", Qt::CaseInsensitive)!=0)
        smu.Num = GNDU;

    return b1500_FindSlot(smu);
}
/*-----------------------------------------------------------------------------------------------*/
bool Test::b1500_FindSlot(SMU &smu)
{
    ViStatus    errStatus = VI_SUCCESS;
    ViUInt32    received;

    char        *ptr;

    char    Modules[BUF_MODULES] = {0};
    int     cnt_smu,
            cnt_slot;

    if(smu.Num <= 0)
        return false;

    if(smu.Num == GNDU){
        smu.Slot = SLOT_GNDU;
        return true;
    }

    //инициализация приборов
    errStatus = Init_B1500(vi_b1500);
    if(errStatus != VI_SUCCESS)
        return false;

    try{
        errStatus = viPrintf(vi_b1500, ViString("UNT? 0\n"));
        errStatus = viRead(vi_b1500, (ViBuf)Modules, BUF_MODULES-1, &received);
    }
    catch(...){
        return false;
    }
    agb1500_dcl(vi_b1500);

    cnt_smu = 0;
    cnt_slot = 0;

    ptr = strtok(Modules,";,\n");
    cnt_slot++;

    if(ptr!=NULL){
        if(strstr(ptr, "15") && !strstr(ptr, "1525")){
            cnt_smu++;
            if(cnt_smu == smu.Num)
                smu.Slot = cnt_slot;
        }
        ptr = strtok(NULL, ";,\n");
    }

    if(smu.Slot > 0)
        return true;

    while(ptr!=NULL){
        ptr = strtok(NULL, ";,\n");
        cnt_slot++;
        if(ptr!=NULL){
            if(strstr(ptr, "15") && !strstr(ptr, "1525")){
                cnt_smu++;
                if(cnt_smu == smu.Num){
                    smu.Slot = cnt_slot;
                    break;
                }
            }
            ptr = strtok(NULL, ";,\n");
        }
    }//end while

    return true;
}
/*-----------------------------------------------------------------------------------------------*/
void Test::ClearConnections()
{    
    if(vi_b1500>VI_NULL){
        agb1500_setSwitch(vi_b1500, agb1500_CH_ALL, 0);
        agb1500_dcl(vi_b1500);
        vi_b1500 = VI_NULL;
    }

    if(bUseMatrix && vi_2200a>VI_NULL){
        OpenCommMatrix(vi_2200a);
        vi_2200a = VI_NULL;
    }

    if(bUseProber)
        prober->ClearProb();

    if(file != NULL)
        fclose(file);
}
/*-----------------------------------------------------------------------------------------------*/
void Test::Run()
{
    ushort die, subdie;
    char str[1024] = {0};
    QString curr_date;
    QString FileName;

    ret = VI_SUCCESS;

    //инициализация приборов
    if(Init_B1500(vi_b1500) != VI_SUCCESS)
        return;

    ret = agb1500_dcl(vi_b1500);
    if(ret != VI_SUCCESS){
        emit finished_test();
        return;
    }

    ret = agb1500_timeOut(vi_b1500, AGILENT_DELAY_CMD);
    if(ret != VI_SUCCESS){
        emit finished_test();
        return;
    }

    //инициализация матрицы
    if(bUseMatrix && Init_2200A(vi_2200a)!= VI_SUCCESS){
        ret = QMessageBox::information(0, "Info", QString("Failed Agilent 2200A initialization! Continue?"),
                                           QMessageBox::Ok|QMessageBox::Cancel);
        if(ret == QMessageBox::Cancel){
            emit finished_test();
            return;
        }
    }

    if(bUseMatrix && vi_2200a > VI_NULL)
        OpenCommMatrix(vi_2200a);

    if(bNeedCal){
        emit change_test_status(QString("Calibration"));

        ret = B1500Calibration();
        if(ret != VI_SUCCESS){
            emit finished_test();
            return;
        }
    }

    if(bUseProber){
        //инициализация пробера
        prober->ClearProb();
        if(!Init_Cascade12k(prober->vi))
            return;

        csc12000k_dcl(prober->vi);
        csc12000k_timeOut(prober->vi, AGILENT_DELAY_CMD);

        prober->UnContact();
        prober->LightOff();
        prober->FirstDie();
        prober->TestSequence();

        ret = VI_SUCCESS;
        for(die=0; die<prober->NumTestDie; die++){
            for(subdie=0; subdie<prober->TestSubdie.size(); subdie++){

                if(prober->TestSubdie.at(subdie)==0){
                    prober->NextSubDie();
                    continue;
                }

                prober->Contact();
                RunTest(prober->SubsiteLabel(prober->TestSubdie.at(subdie)),die+1);
                prober->UnContact();

                if(ret != VI_SUCCESS){
                    check_err_B1500(vi_b1500, ret);
                    break;
                }

                prober->NextSubDie();

            }//end step on subdies
            if(ret != VI_SUCCESS)
                break;
            prober->NextDie();
        }//end step on dies


        prober->UnContact();
        prober->LightOff();

        csc12000k_dcl(prober->vi);
        csc12000k_close(prober->vi);

        prober->ClearProb();
    }
    else{
        emit change_test_status(QString("Test"));
        //создание файла для записи данных

        curr_date = QDateTime::currentDateTime().toString("hh-mm-ss dd.MM.yyyy");
        FileName = trUtf8("%1Test_%2.txt").arg(Path_to_save_data).arg(curr_date);

        file = fopen(FileName.toLocal8Bit().data(), "w+a");
        if(file != NULL){
            ret = VI_SUCCESS;

            RunTest();

            sprintf(str, "Status=%d\n", (int)ret);
            fputs(str, file);
            fclose(file);
        }
    }

    emit finished_test();
}
/*-----------------------------------------------------------------------------------------------*/
ViStatus Test::B1500Calibration()
{
    emit change_test_status(QString("Calibration"));

    char str[256];
    ret = agb1500_cmd(vi_b1500, ViString("CA"));
    if(ret == VI_SUCCESS)
        ret = agb1500_cmdString_Q(vi_b1500, ViString("*OPC?"), 255, str);

    return ret;
}
/*-----------------------------------------------------------------------------------------------*/
