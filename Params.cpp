#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "eq_control.h"
#include "XML.h"
#include <QThread>
#include <QFont>

/* -----------------------------------------------------------------------------------------------*/
void MainWindow::TDDB_Test()
{
    /*StressTesting *ST = new StressTesting(this);
    TDDB tddb_curr;
    QThread *thread = new QThread;

    ST->moveToThread(thread);

    connect(ST, SIGNAL(interrupt()), ST, SLOT(deleteLater()));
    connect(ST, SIGNAL(interrupt()), ST, SLOT(ClearConnections()));
    connect(ST, SIGNAL(interrupt()), thread, SLOT(quit()));

    connect(ST, SIGNAL(finished_test()), thread, SLOT(quit()));

    connect(thread, SIGNAL(finished()), this, SLOT(CloseConnections()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), this, SLOT(SetReady()));

    connect(thread, SIGNAL(started()), ST, SLOT(TDDB_Main()));

    connect(ST, SIGNAL(change_TDDB_item(double,int,int)), this, SLOT(IsertTDDBData(double,int,int)));
    connect(ST, SIGNAL(change_test_time(int)), this, SLOT(SetTestTime(int)));
    connect(ST, SIGNAL(change_test_status(QString)), this, SLOT(SetCurrentTestStatus(QString)));

    //ui->lMeasStatus->palette().setColor(QPalette::windowText(), QColor(255, 0, 0, 127));//QColor(255, 0, 0, 127)

    if(TDDBParamValues(ui->rbtn_TDDB_Nch_1,
                                   ui->le_TDDB_T,
                                   ui->le_TDDB_Width_1,
                                   ui->le_TDDB_Length_1,
                                   ui->le_TDDB_tox_1,
                                   ui->le_TDDB_Vuse_1,
                                   ui->le_TDDB_Iuse_1,
                                   ui->le_TDDB_Istress_1,
                                   ui->le_TDDB_Vsilc_1,
                                   ui->le_TDDB_Vstress_1,
                                   ui->le_TDDB_TimeLimit,
                                   ui->le_TDDB_High_1,
                                   ui->le_TDDB_Low_1,
                                   ui->le_TDDB_DUT1_Comment,
                                   &tddb_curr, 1))
        ST->tddb.push_back(tddb_curr);

    if(TDDBParamValues(ui->rbtn_TDDB_Nch_2,
                                   ui->le_TDDB_T,
                                   ui->le_TDDB_Width_2,
                                   ui->le_TDDB_Length_2,
                                   ui->le_TDDB_tox_2,
                                   ui->le_TDDB_Vuse_2,
                                   ui->le_TDDB_Iuse_2,
                                   ui->le_TDDB_Istress_2,
                                   ui->le_TDDB_Vsilc_2,
                                   ui->le_TDDB_Vstress_2,
                                   ui->le_TDDB_TimeLimit,
                                   ui->le_TDDB_High_2,
                                   ui->le_TDDB_Low_2,
                                   ui->le_TDDB_DUT2_Comment,
                                   &tddb_curr, 2))
        ST->tddb.push_back(tddb_curr);

    if(TDDBParamValues(ui->rbtn_TDDB_Nch_3,
                                   ui->le_TDDB_T,
                                   ui->le_TDDB_Width_3,
                                   ui->le_TDDB_Length_3,
                                   ui->le_TDDB_tox_3,
                                   ui->le_TDDB_Vuse_3,
                                   ui->le_TDDB_Iuse_3,
                                   ui->le_TDDB_Istress_3,
                                   ui->le_TDDB_Vsilc_3,
                                   ui->le_TDDB_Vstress_3,
                                   ui->le_TDDB_TimeLimit,
                                   ui->le_TDDB_High_3,
                                   ui->le_TDDB_Low_3,
                                   ui->le_TDDB_DUT3_Comment,
                                   &tddb_curr, 3))
        ST->tddb.push_back(tddb_curr);

    if(TDDBParamValues(ui->rbtn_TDDB_Nch_4,
                                   ui->le_TDDB_T,
                                   ui->le_TDDB_Width_4,
                                   ui->le_TDDB_Length_4,
                                   ui->le_TDDB_tox_4,
                                   ui->le_TDDB_Vuse_4,
                                   ui->le_TDDB_Iuse_4,
                                   ui->le_TDDB_Istress_4,
                                   ui->le_TDDB_Vsilc_4,
                                   ui->le_TDDB_Vstress_4,
                                   ui->le_TDDB_TimeLimit,
                                   ui->le_TDDB_High_4,
                                   ui->le_TDDB_Low_4,
                                   ui->le_TDDB_DUT4_Comment,
                                   &tddb_curr, 4))
        ST->tddb.push_back(tddb_curr);

    if(TDDBParamValues(ui->rbtn_TDDB_Nch_5,
                                   ui->le_TDDB_T,
                                   ui->le_TDDB_Width_5,
                                   ui->le_TDDB_Length_5,
                                   ui->le_TDDB_tox_5,
                                   ui->le_TDDB_Vuse_5,
                                   ui->le_TDDB_Iuse_5,
                                   ui->le_TDDB_Istress_5,
                                   ui->le_TDDB_Vsilc_5,
                                   ui->le_TDDB_Vstress_5,
                                   ui->le_TDDB_TimeLimit,
                                   ui->le_TDDB_High_5,
                                   ui->le_TDDB_Low_5,
                                   ui->le_TDDB_DUT5_Comment,
                                   &tddb_curr, 5))
        ST->tddb.push_back(tddb_curr);

    if(TDDBParamValues(ui->rbtn_TDDB_Nch_6,
                                   ui->le_TDDB_T,
                                   ui->le_TDDB_Width_6,
                                   ui->le_TDDB_Length_6,
                                   ui->le_TDDB_tox_6,
                                   ui->le_TDDB_Vuse_6,
                                   ui->le_TDDB_Iuse_6,
                                   ui->le_TDDB_Istress_6,
                                   ui->le_TDDB_Vsilc_6,
                                   ui->le_TDDB_Vstress_6,
                                   ui->le_TDDB_TimeLimit,
                                   ui->le_TDDB_High_6,
                                   ui->le_TDDB_Low_6,
                                   ui->le_TDDB_DUT6_Comment,
                                   &tddb_curr, 6))
        ST->tddb.push_back(tddb_curr);

    if(ST->tddb.size()<1){
        QMessageBox::information(this, "Error", QString("Structures for TDDB testing are missing. Test interrupted."), QMessageBox::Ok);
        return;
    }

    //инициализация приборов
    if(!Init_B1500(vi_B1500)){
        QMessageBox::information(this, "Info", QString("Failed Agilent B1500 initialization!"), QMessageBox::Ok);
        return;
    }
    if(!Init_2200A(vi_2200A)){
        CloseConnection(vi_B1500, vi_2200A);
        QMessageBox::information(this, "Info", QString("Failed Agilent 2200A initialization!"), QMessageBox::Ok);
        return;
    }

    if(vi_2200A > VI_NULL)
        OpenCommMatrix(vi_2200A);

    agb1500_dcl(vi_B1500);
    agb1500_timeOut(vi_B1500, AGILENT_DELAY_CMD);

    ST->vi_b1500 = vi_B1500;
    ST->vi_2200a = vi_2200A;

    ST->bNeedCal = ui->chb_TDDB_NeedCal->isChecked();


    thread->start();
    thread->setPriority(QThread::HighPriority);

    //создание файлов для сохранения
    FileName = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("Text Files (*.txt)"));
    if(FileName.length()<4){
        CloseConnection(vi_B1500, vi_2200A);
        return;
    }

    ST->file = fopen(FileName.toLocal8Bit().data(), "w+a");
    if(ST->file == NULL)
    {
        CloseConnection(vi_B1500, vi_2200A);
        QMessageBox::critical(this, "Error", QString("Cannot create file for saving data!"),
                              QMessageBox::Ok);
    }
    else
    {
        ui->lMeasStatus->setText("TDDB testing");

        fputs("--- TDDB test ---\n", ST->file);

        agb1500_dcl(vi_B1500);
        agb1500_timeOut(vi_B1500, AGILENT_DELAY_CMD);

        ST->vi_b1500 = vi_B1500;
        ST->vi_2200a = vi_2200A;

        thread->start();
        thread->setPriority(QThread::HighPriority);
    }*/
}
/*------------------------------------------------------------------------------------------------*/
void MainWindow::NBTI_Test()
{
    /*NBTI nbti_curr;
    StressTesting *ST = new StressTesting(this);
    QThread *thread = new QThread;

    QString FileName;
    int err;

    ST->moveToThread(thread);

    connect(ST, SIGNAL(interrupt()), ST, SLOT(deleteLater()));
    connect(ST, SIGNAL(interrupt()), ST, SLOT(ClearConnections()));
    connect(ST, SIGNAL(interrupt()), thread, SLOT(quit()));

    connect(ST, SIGNAL(finished_test()), thread, SLOT(quit()));

    connect(thread, SIGNAL(finished()), this, SLOT(CloseConnections()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), this, SLOT(SetReady()));

    connect(thread, SIGNAL(started()), ST, SLOT(NBTI_Main()));

    connect(ST, SIGNAL(change_NBTI_item(short,double)), this, SLOT(InsertNBTIData(short,double)));
    connect(ST, SIGNAL(change_test_time(int)), this, SLOT(SetTestTime(int)));

    connect(ST, SIGNAL(change_HC_item(short,double,double,double,double,double,double,double)),
                    this, SLOT(InsertHCData(short,double,double,double,double,double,double,double)));

    nbti_curr.Clear();
   //проверка правильности введенных данных
    if(NBTIReadAndCheckParamValues(ui->rbtn_NBTI_Nch_1,
                                    ui->le_NBTI_Tstress,
                                    ui->cb_NBTI_StressTime,
                                    ui->le_NBTI_Width_1,
                                    ui->le_NBTI_Length_1,
                                    ui->le_NBTI_Vgstart_1,
                                    ui->le_NBTI_Vgstop_1,
                                    ui->le_NBTI_Ith_1,
                                    ui->le_NBTI_Idmax_1,
                                    ui->le_NBTI_Vdlin_1,
                                    ui->le_NBTI_Vdsat_1,
                                    ui->le_NBTI_Vdstress,
                                    ui->le_NBTI_Vgstress,
                                    &nbti_curr,
                                    1)){
        if(NBTIReadAndCheckConnections(&nbti_curr,
                                       ui->le_NBTI_Drain_1,
                                       ui->le_NBTI_Gate_1,
                                       ui->le_NBTI_SourceBody_1,
                                       SMU2, 1))
            ST->nbti.push_back(nbti_curr);
            nbti_curr.Clear();
    }

    if(NBTIReadAndCheckParamValues(ui->rbtn_NBTI_Nch_2,
                                    ui->le_NBTI_Tstress,
                                    ui->cb_NBTI_StressTime,
                                    ui->le_NBTI_Width_2,
                                    ui->le_NBTI_Length_2,
                                    ui->le_NBTI_Vgstart_2,
                                    ui->le_NBTI_Vgstop_2,
                                    ui->le_NBTI_Ith_2,
                                    ui->le_NBTI_Idmax_2,
                                    ui->le_NBTI_Vdlin_2,
                                    ui->le_NBTI_Vdsat_2,
                                    ui->le_NBTI_Vdstress,
                                    ui->le_NBTI_Vgstress,
                                    &nbti_curr,
                                    2)){
        if(NBTIReadAndCheckConnections(&nbti_curr,
                                       ui->le_NBTI_Drain_2,
                                       ui->le_NBTI_Gate_2,
                                       ui->le_NBTI_SourceBody_2,
                                       SMU3, 2))
            ST->nbti.push_back(nbti_curr);
            nbti_curr.Clear();
    }

    if(NBTIReadAndCheckParamValues(ui->rbtn_NBTI_Nch_3,
                                    ui->le_NBTI_Tstress,
                                    ui->cb_NBTI_StressTime,
                                    ui->le_NBTI_Width_3,
                                    ui->le_NBTI_Length_3,
                                    ui->le_NBTI_Vgstart_3,
                                    ui->le_NBTI_Vgstop_3,
                                    ui->le_NBTI_Ith_3,
                                    ui->le_NBTI_Idmax_3,
                                    ui->le_NBTI_Vdlin_3,
                                    ui->le_NBTI_Vdsat_3,
                                    ui->le_NBTI_Vdstress,
                                    ui->le_NBTI_Vgstress,
                                    &nbti_curr,
                                    3)){
        if(NBTIReadAndCheckConnections(&nbti_curr,
                                       ui->le_NBTI_Drain_3,
                                       ui->le_NBTI_Gate_3,
                                       ui->le_NBTI_SourceBody_3,
                                       SMU4, 3))
            ST->nbti.push_back(nbti_curr);
            nbti_curr.Clear();
    }

    if(NBTIReadAndCheckParamValues(ui->rbtn_NBTI_Nch_4,
                                    ui->le_NBTI_Tstress,
                                    ui->cb_NBTI_StressTime,
                                    ui->le_NBTI_Width_4,
                                    ui->le_NBTI_Length_4,
                                    ui->le_NBTI_Vgstart_4,
                                    ui->le_NBTI_Vgstop_4,
                                    ui->le_NBTI_Ith_4,
                                    ui->le_NBTI_Idmax_4,
                                    ui->le_NBTI_Vdlin_4,
                                    ui->le_NBTI_Vdsat_4,
                                    ui->le_NBTI_Vdstress,
                                    ui->le_NBTI_Vgstress,
                                    &nbti_curr,
                                    4)){
        if(NBTIReadAndCheckConnections(&nbti_curr,
                                       ui->le_NBTI_Drain_4,
                                       ui->le_NBTI_Gate_4,
                                       ui->le_NBTI_SourceBody_4,
                                       SMU5, 4))
            ST->nbti.push_back(nbti_curr);
            nbti_curr.Clear();
    }

    if(NBTIReadAndCheckParamValues(ui->rbtn_NBTI_Nch_5,
                                    ui->le_NBTI_Tstress,
                                    ui->cb_NBTI_StressTime,
                                    ui->le_NBTI_Width_5,
                                    ui->le_NBTI_Length_5,
                                    ui->le_NBTI_Vgstart_5,
                                    ui->le_NBTI_Vgstop_5,
                                    ui->le_NBTI_Ith_5,
                                    ui->le_NBTI_Idmax_5,
                                    ui->le_NBTI_Vdlin_5,
                                    ui->le_NBTI_Vdsat_5,
                                    ui->le_NBTI_Vdstress,
                                    ui->le_NBTI_Vgstress,
                                    &nbti_curr,
                                    5)){
        if(NBTIReadAndCheckConnections(&nbti_curr,
                                       ui->le_NBTI_Drain_5,
                                       ui->le_NBTI_Gate_5,
                                       ui->le_NBTI_SourceBody_5,
                                       SMU6, 5))
            ST->nbti.push_back(nbti_curr);
            nbti_curr.Clear();
    }

    if(ST->nbti.size()<1){
        QMessageBox::critical(this, "Error", QString("Structures for NBTI testing are missing. Test interrupted."), QMessageBox::Ok);
        return;
    }

    err = QMessageBox::information(this, "Info", QString("%1 DUTs ready for NBTI testing. Would you like to continue?").arg(ST->nbti.size()), QMessageBox::Ok|QMessageBox::Cancel);
    if(err == QMessageBox::Cancel)
        return;

    //инициализация приборов
    if(!Init_B1500(vi_B1500)){
        QMessageBox::information(this, "Info", QString("Failed Agilent B1500 initialization!"), QMessageBox::Ok);
        return;
    }
    if(!Init_2200A(vi_2200A)){
        CloseConnection(vi_B1500, vi_2200A);
        QMessageBox::information(this, "Info", QString("Failed Agilent 2200A initialization!"), QMessageBox::Ok);
        return;
    }

    FileName = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("Text Files (*.txt)"));
    if(FileName.length()<4){
        CloseConnection(vi_B1500, vi_2200A);
        return;
    }
    ST->file = fopen(FileName.toLocal8Bit().data(), "w+a");
    if(ST->file == NULL)
    {
        CloseConnection(vi_B1500, vi_2200A);
        QMessageBox::critical(this, "Error", QString("Cannot create file for saving data!"),
                              QMessageBox::Ok);
    }
    else
    {
        ui->lMeasStatus->setText("NBTI testing");

        fputs("--- NBTI test ---\n", ST->file);

        fputs("Date,", ST->file);
        fputs(CurrentDate().toLocal8Bit().data(), ST->file);
        fputs("\n", ST->file);

        agb1500_dcl(vi_B1500);
        agb1500_timeOut(vi_B1500, AGILENT_DELAY_CMD);

        ST->vi_b1500 = vi_B1500;
        ST->vi_2200a = vi_2200A;

        thread->start();
        thread->setPriority(QThread::HighPriority);
    }*/
}
/*------------------------------------------------------------------------------------------------*/
void MainWindow::VRDB_UPR_Test()
{
    /*StressTesting *ST = new StressTesting(this);
    QThread *thread = new QThread;
    int ret;

    ST->moveToThread(thread);

    connect(ST, SIGNAL(interrupt()), ST, SLOT(deleteLater()));
    connect(ST, SIGNAL(interrupt()), ST, SLOT(ClearConnections()));
    connect(ST, SIGNAL(interrupt()), thread, SLOT(quit()));

    connect(ST, SIGNAL(finished_test()), thread, SLOT(quit()));

    connect(thread, SIGNAL(finished()), this, SLOT(CloseConnections()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), this, SLOT(SetReady()));

    connect(thread, SIGNAL(started()), ST, SLOT(UPR_Main()));

    connect(ST, SIGNAL(change_test_time(int)), this, SLOT(SetTestTime(int)));
    connect(ST, SIGNAL(change_test_status(QString)), this, SLOT(SetCurrentTestStatus(QString)));

    ST->vrdb->Clear();

    ST->vrdb->NumSMUHigh = SetNumOfSMU(ui->cb_VRDB_HighSMU);
    ST->vrdb->NumSMULow = SetNumOfSMU(ui->cb_VRDB_LowSMU);

    if(ST->vrdb->NumSMUHigh == ST->vrdb->NumSMULow){
        QMessageBox::information(this, "Info", QString("SMU number for hidh and low sources are equal!"), QMessageBox::Ok);
        return;
    }

    if(ST->vrdb->NumSMUHigh < 1|| ST->vrdb->NumSMULow < 1){
        QMessageBox::information(this, "Info", QString("SMU number for hidh and low sources are fail! Please, check them"), QMessageBox::Ok);
        return;
    }

    ST->vrdb->NumSlotHigh = SetSlotOfSMU(ST->vrdb->NumSMUHigh);
    ST->vrdb->NumSlotLow = SetSlotOfSMU(ST->vrdb->NumSMULow);

    //проверка правильности введенных данных
    if(!VRDBParamValues(ST->vrdb))
        return;

    //инициализация приборов
    if(!Init_B1500(vi_B1500)){
        QMessageBox::information(this, "Info", QString("Failed Agilent B1500 initialization!"), QMessageBox::Ok);
        return;
    }

    if(!Init_2200A(vi_2200A)){
        ret = QMessageBox::information(this, "Info", QString("Failed Agilent 2200A initialization! Continue?"),
                                       QMessageBox::Ok|QMessageBox::Cancel);
        if(ret == QMessageBox::Cancel){
            CloseConnection(vi_B1500, vi_2200A);
            return;
        }
    }

    if(vi_2200A > VI_NULL)
        OpenCommMatrix(vi_2200A);

    agb1500_dcl(vi_B1500);
    agb1500_timeOut(vi_B1500, AGILENT_DELAY_CMD);

    ST->vi_b1500 = vi_B1500;
    ST->vi_2200a = vi_2200A;


    thread->start();
    thread->setPriority(QThread::HighPriority);*/
}
/*------------------------------------------------------------------------------------------------*/
void MainWindow::VRDB_Ramp_Test()
{
    /*StressTesting *ST = new StressTesting(this);
    QThread *thread = new QThread;

    int ret;

    ST->moveToThread(thread);

    connect(ST, SIGNAL(finished_test()), thread, SLOT(quit()));
    connect(ST, SIGNAL(change_test_status(QString)), this, SLOT(SetCurrentTestStatus(QString)));

    connect(thread, SIGNAL(finished()), this, SLOT(SetReady()));
    connect(thread, SIGNAL(finished()), this, SLOT(CloseConnections()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(thread, SIGNAL(started()), ST, SLOT(VRDB_Main()));

    ST->vrdb->Clear();

    ST->vrdb->NumSMUHigh = SetNumOfSMU(ui->cb_VRDB_HighSMU);
    ST->vrdb->NumSMULow = SetNumOfSMU(ui->cb_VRDB_LowSMU);

    if(ST->vrdb->NumSMUHigh == ST->vrdb->NumSMULow){
        QMessageBox::information(this, "Info", QString("SMU number for hidh and low sources are equal!"), QMessageBox::Ok);
        return;
    }

    if(ST->vrdb->NumSMUHigh < 1|| ST->vrdb->NumSMULow < 1){
        QMessageBox::information(this, "Info", QString("SMU number for hidh and low sources are fail! Please, check them"), QMessageBox::Ok);
        return;
    }

    ST->vrdb->NumSlotHigh = SetSlotOfSMU(ST->vrdb->NumSMUHigh);
    ST->vrdb->NumSlotLow = SetSlotOfSMU(ST->vrdb->NumSMULow);

    //проверка правильности введенных данных
    if(!VRDBParamValues(ST->vrdb))
        return;

    //инициализация приборов
    if(!Init_B1500(vi_B1500)){
        QMessageBox::information(this, "Info", QString("Failed Agilent B1500 initialization!"), QMessageBox::Ok);
        return;
    }

    if(!Init_2200A(vi_2200A)){
        ret = QMessageBox::information(this, "Info", QString("Failed Agilent 2200A initialization! Continue?"),
                                       QMessageBox::Ok|QMessageBox::Cancel);
        if(ret == QMessageBox::Cancel){
            CloseConnection(vi_B1500, vi_2200A);
            return;
        }
    }

    if(vi_2200A > VI_NULL)
        OpenCommMatrix(vi_2200A);

    agb1500_dcl(vi_B1500);
    agb1500_timeOut(vi_B1500, AGILENT_DELAY_CMD);

    ST->vi_b1500 = vi_B1500;
    ST->vi_2200a = vi_2200A;

    thread->start();
    thread->setPriority(QThread::HighPriority);*/
}
/*------------------------------------------------------------------------------------------------*/
bool MainWindow::EMReadAndCheckConnections(EM *t)
{
    t->TestType = ui->cb_EM_TestType->currentIndex();
    t->bUseProber = ui->chb_EM_UseProber->isChecked();

    if(t->TestType==ICONST_2P || t->TestType==ISWEEP_2P || t->TestType==RMEAS_2P)
        t->b2point = true;
    else
        t->b2point = false;


    if(ui->chb_EM_Ext->checkState() == Qt::Checked)
        t->bMeasExt = true;
    else
        t->bMeasExt = false;

    if(ui->rbtn_EM_Al->isChecked())
        t->MetalType = ALUMINUM;
    else
        if(ui->rbtn_EM_Cu->isChecked())
            t->MetalType = COPPER;
        else
            t->MetalType = OTHER;

    if(!t->SetNumSMU(t->forceH, ui->cb_EM_Force1->currentText()) ||
        !t->SetNumSMU(t->forceL, ui->cb_EM_Force2->currentText())){
        QMessageBox::critical(this, "Error", QString("Sources are not correct in EM test. Please, check them."),
                              QMessageBox::Ok);
        return false;
    }

    if(!SetPinsVect(ui->le_EM_Force1->text(), t->forceH.pins) ||
            SetPinsVect(ui->le_EM_Force2->text(), t->forceL.pins)){
        QMessageBox::critical(this, "Error", QString("Pin numbers are not correct in EM test. Please, check them."),
                              QMessageBox::Ok);
        return false;
    }

    if(t->bMeasExt && !t->SetNumSMU(t->ExtMon, ui->cb_EM_Ext->currentText()) && \
                            !SetPinsVect(ui->le_EM_Ext->text(), t->ExtMon.pins)){
        QMessageBox::critical(this, "Error", QString("Extrusion monitor settings are not correct. Please, check them."),
                              QMessageBox::Ok);
        return false;
    }

    if(!t->b2point){
        if(!t->SetNumSMU(t->senseH, ui->cb_EM_Sense1->currentText()) ||
            !t->SetNumSMU(t->senseL, ui->cb_EM_Sense2->currentText())){
            QMessageBox::critical(this, "Error", QString("Sources are not correct in EM test. Please, check them."),
                                  QMessageBox::Ok);
            return false;
        }

        if(!SetPinsVect(ui->le_EM_Sense1->text(), t->senseH.pins) ||
                SetPinsVect(ui->le_EM_Sense2->text(), t->senseL.pins)){
            QMessageBox::critical(this, "Error", QString("Pin numbers are not correct in EM test. Please, check them."),
                                  QMessageBox::Ok);
            return false;
        }
    }

    if(!t->CheckSrc()){
        QMessageBox::critical(this, "Error", QString("Sources are not correct in EM test. Please, check them."),
                              QMessageBox::Ok);
        return false;
    }

    return true;
}
/* -----------------------------------------------------------------------------------------------*/
bool MainWindow::EMSetConnectionsAndParams(EM *t)
{
    QString s;
    QVector<ViInt32>::iterator it;

    ui->cb_EM_TestType->setCurrentIndex(t->TestType);

    switch(t->MetalType){
        case ALUMINUM: ui->rbtn_EM_Al->setChecked(true); break;

        case COPPER: ui->rbtn_EM_Cu->setChecked(true); break;

        case OTHER: ui->rbtn_EM_Other->setChecked(true); break;

        default:ui->rbtn_EM_Al->setChecked(true); break;
    };

    ui->cb_EM_Force1->setCurrentIndex(t->forceH.Num-1);
    if(t->forceL.Num == GNDU)
        ui->cb_EM_Force2->setCurrentIndex(ui->cb_EM_Force2->count()-1);
    else
        ui->cb_EM_Force2->setCurrentIndex(t->forceL.Num-1);
    ui->cb_EM_Sense1->setCurrentIndex(t->senseH.Num-1);
    ui->cb_EM_Sense2->setCurrentIndex(t->senseL.Num-1);

    it=t->forceH.pins.begin();
    s = QString::number(*it);
    for(it=t->forceH.pins.begin()+1; it<t->forceH.pins.end(); it++)
        s = s + "," + QString::number(*it);
    ui->le_EM_Force1->setText(s);

    it=t->forceL.pins.begin();
    s = QString::number(*it);
    for(it=t->forceL.pins.begin()+1; it<t->forceL.pins.end(); it++)
        s = s + "," + QString::number(*it);
    ui->le_EM_Force2->setText(s);

    it=t->senseH.pins.begin();
    s = QString::number(*it);
    for(it=t->senseH.pins.begin()+1; it<t->senseH.pins.end(); it++)
        s = s + "," + QString::number(*it);
    ui->le_EM_Sense1->setText(s);

    it=t->senseL.pins.begin();
    s = QString::number(*it);
    for(it=t->senseL.pins.begin()+1; it<t->senseL.pins.end(); it++)
        s = s + "," + QString::number(*it);
    ui->le_EM_Sense2->setText(s);

    if(t->bMeasExt){
        ui->chb_EM_Ext->setCheckState(Qt::Checked);
        ui->cb_EM_Ext->setEnabled(true);
        ui->le_EM_Ext->setEnabled(true);
        ui->cb_EM_Ext->setCurrentIndex(t->ExtMon.Num-1);

        it=t->ExtMon.pins.begin();
        s = QString::number(*it);
        for(it=t->ExtMon.pins.begin()+1; it<t->ExtMon.pins.end(); it++)
            s = s + "," + QString::number(*it);
        ui->le_EM_Ext->setText(s);
    }
    else{
        ui->chb_EM_Ext->setCheckState(Qt::Unchecked);
        ui->cb_EM_Ext->setEnabled(false);
        ui->le_EM_Ext->setEnabled(false);
    }

    ui->le_EM_Width->setText(QString("%1").arg(t->W));
    ui->le_EM_Height->setText(QString("%1").arg(t->H));
    ui->le_EM_Tstress->setText(QString("%1").arg(t->Tstress));
    ui->le_EM_Tref->setText(QString("%1").arg(t->Tref));
    ui->le_EM_Tchuck->setText(QString("%1").arg(t->Tchuck));
    ui->le_EM_TCR->setText(QString("%1").arg(t->TCRref));
    ui->le_EM_StopCond->setText(QString("%1").arg(t->StopCond*100));
    ui->le_EM_TimeLimit->setText(QString("%1").arg(t->TimeLimit));
    ui->le_EM_Istress->setText(QString("%1").arg(t->Istress*1000.0));
    ui->le_EM_Ea->setText(QString("%1").arg(t->Ea));
    ui->le_EM_A->setText(QString("%1").arg(t->A));
    ui->le_EM_n->setText(QString("%1").arg(t->n));
    ui->le_EM_Iuse->setText(QString("%1").arg(t->Iuse*1E+6));
    ui->le_EM_PulseW->setText(QString("%1").arg(t->PulseW));

    return true;
}
/* -----------------------------------------------------------------------------------------------*/
//проверка правильности введенных значений параметров для EMIsoT испытаний
bool MainWindow::EMReadAndCheckParamValues(EM *t)
{
    bool Ok;

    t->W = (ui->le_EM_Width->text()).toDouble(&Ok);
    if(!Ok || t->W>MAX_WIDTH || t->W<MIN_WIDTH)
    {
        QMessageBox::critical(this, "Error", QString("Width value is not correct for EM test. Please, enter value from %1 to %2")
                              .arg(MIN_WIDTH).arg(MAX_WIDTH), QMessageBox::Ok);
        return false;
    }

    t->H = (ui->le_EM_Height->text()).toDouble(&Ok);
    if(!Ok || t->H>MAX_HEIGHT || t->H<MIN_HEIGHT)
    {
        QMessageBox::critical(this, "Error", QString("Height value is not correct for EM test. Please, enter value from %1 to %2")
                              .arg(MIN_HEIGHT).arg(MAX_HEIGHT), QMessageBox::Ok);
        return false;
    }

    t->Tstress = (ui->le_EM_Tstress->text()).toDouble(&Ok);
    if(!Ok || t->Tstress>MAX_TEMP || t->Tstress<MIN_TEMP)
    {
        QMessageBox::critical(this, "Error", QString("Stress temperature value is not correct for EM test. Please, enter value from %1 to %2")
                              .arg(MIN_TEMP).arg(MAX_TEMP), QMessageBox::Ok);
        return false;
    }

    t->Tref = (ui->le_EM_Tref->text()).toDouble(&Ok);
    if(t->TestType == PCONST || t->TestType == SWEAT)
        if(!Ok || t->Tref>MAX_TEMP || t->Tref<MIN_TEMP || t->Tref>t->Tstress)
        {
            QMessageBox::critical(this, "Error", QString("Reference temperature value is not correct for EM test. Please, enter value from %1 to %2")
                                  .arg(MIN_TEMP).arg(MAX_TEMP), QMessageBox::Ok);
            return false;
        }

    t->Tchuck = (ui->le_EM_Tchuck->text()).toDouble(&Ok);
    if(t->TestType == PCONST || t->TestType == SWEAT)
        if(!Ok || t->Tchuck>MAX_TEMP || t->Tchuck<MIN_TEMP || t->Tchuck>t->Tstress)
        {
            QMessageBox::critical(this, "Error", QString("Room temperature value is not correct for EM test. Please, enter value from %1 to %2")
                                  .arg(MIN_TEMP).arg(MAX_TEMP), QMessageBox::Ok);
            return false;
        }

    t->TCRref = (ui->le_EM_TCR->text()).toDouble(&Ok);
    if(t->TestType == PCONST || t->TestType == SWEAT)
        if(!Ok || t->TCRref>MAX_TCR || t->TCRref<MIN_TCR)
        {
            QMessageBox::critical(this, "Error", QString("TCR value is not correct for EM test. Please, enter value from %1 to %2")
                                  .arg(MIN_TCR).arg(MAX_TCR), QMessageBox::Ok);
            return false;
        }

    t->StopCond = ((ui->le_EM_StopCond->text()).toDouble(&Ok))/100.0;
    if(!Ok || t->StopCond>MAX_STOP_COND || t->StopCond<MIN_STOP_COND)
    {
        QMessageBox::critical(this, "Error", QString("Stop condition value is not correct for EM test. Please, enter value from %1 to %2")
                              .arg(MIN_STOP_COND).arg(MAX_STOP_COND), QMessageBox::Ok);
        return false;
    }

    t->TimeLimit = (ui->le_EM_TimeLimit->text()).toDouble(&Ok);
    if(!Ok || t->TimeLimit>MAX_TIME || t->TimeLimit<MIN_TIME)
    {
        QMessageBox::critical(this, "Error", QString("Time limit is not correct for EM test. Please, enter value from %1 to %2")
                              .arg(MIN_TIME).arg(MAX_TIME), QMessageBox::Ok);
        return false;
    }

    t->Istress = (ui->le_EM_Istress->text()).toDouble(&Ok)/1000.0;
    if(!Ok){
        QMessageBox::critical(this, "Error", QString("Stress current is not correct for EM test. Please, enter value from %1 to %2")
                              .arg(MIN_I).arg(MAX_I), QMessageBox::Ok);
        return false;
    }
        else{
            if(t->bHPRegime && (t->Istress>MAX_HI || t->Istress<MIN_I)){
                QMessageBox::critical(this, "Error", QString("Stress current is not correct for HP EM test. Please, enter value from %1 to %2")
                                      .arg(MIN_I).arg(MAX_HI), QMessageBox::Ok);
                return false;
            }

            if(!t->bHPRegime && (t->Istress>MAX_I || t->Istress<MIN_I)){
                QMessageBox::critical(this, "Error", QString("Stress current is not correct for EM test. Please, enter value from %1 to %2")
                                      .arg(MIN_I).arg(MAX_I), QMessageBox::Ok);
                return false;
            }

    }

    t->Ea = (ui->le_EM_Ea->text()).toDouble(&Ok);
    if(!Ok || t->Ea<0.0){
        QMessageBox::critical(this, "Error", QString("Activation energy is not correct for EM test. Please, enter value >0")
                              , QMessageBox::Ok);
        return false;
    }

    t->n = (ui->le_EM_n->text()).toDouble(&Ok);
    if(!Ok || t->n<0.0){
        QMessageBox::critical(this, "Error", QString("Coefficient n is not correct for EM test. Please, enter value >0")
                              , QMessageBox::Ok);
        return false;
    }

    t->A = (ui->le_EM_A->text()).toDouble(&Ok);
    if(!Ok || t->A<0.0){
        QMessageBox::critical(this, "Error", QString("Coefficient A is not correct for EM test. Please, enter value >0")
                              , QMessageBox::Ok);
        return false;
    }

    t->Iuse = (ui->le_EM_Iuse->text()).toDouble(&Ok)/1E+6;
    if(!Ok || t->Iuse>MAX_I || t->Iuse<MIN_I){
        QMessageBox::critical(this, "Error", QString("Use current is not correct for EM test. Please, enter value from %1 to %2")
                              .arg(MIN_I).arg(MAX_I), QMessageBox::Ok);
        return false;
    }

    t->PulseW = (ui->le_EM_PulseW->text()).toDouble(&Ok);
    if(!Ok || t->PulseW>MAX_PULSE_W || t->PulseW<MIN_PULSE_W){
        QMessageBox::critical(this, "Error", QString("Pulse width is not correct for EM test. Please, enter value from %1 to %2 ms")
                              .arg(MIN_PULSE_W).arg(MAX_PULSE_W), QMessageBox::Ok);
        return false;
    }

    t->comment = ui->le_EM_Comment->text();

    if(ui->chb_EM_NeedCal->isChecked())
        t->bNeedCal = true;
    else
        t->bNeedCal = false;

    return true;
}
/*------------------------------------------------------------------------------------------------*/
bool MainWindow::HCReadData(HC *hc, Transistor *t, const QWidget* const w, const int &index)
{
    QList<QLineEdit *> le = w->findChildren<QLineEdit*>();
    bool ok;

    t->clear();
    for(int i=0; i<le.size(); i++){
        if(le[i]->objectName().contains("Drain", Qt::CaseInsensitive)){
            if(!SetPinsVect(le[i]->text().trimmed(), t->drain.pins)){
                QMessageBox::critical(this, "Error", QString("Drain's pins are not correct in HC test for DUT%1.\n"\
                                                             "Please, check them.").arg(index), QMessageBox::Ok);
                return false;
            }
            else
                for(auto i: t->drain.pins)
                    hc->Drain.pins.push_back(i);
        }

        if(le[i]->objectName().contains("Gate", Qt::CaseInsensitive)){
            if(!SetPinsVect(le[i]->text().trimmed(), t->gate.pins)){
                QMessageBox::critical(this, "Error", QString("Gate's pins are not correct in HC test for DUT%1.\n"\
                                                             "Please, check them.").arg(index), QMessageBox::Ok);
                return false;
            }
            else
                for(auto i: t->gate.pins)
                    hc->Gate.pins.push_back(i);
        }

        if(le[i]->objectName().contains("Source", Qt::CaseInsensitive)){
            if(!SetPinsVect(le[i]->text().trimmed(), t->source.pins)){
                QMessageBox::critical(this, "Error", QString("Source's pins are not correct in HC test for DUT%1.\n"\
                                                             "Please, check them.").arg(index), QMessageBox::Ok);
                return false;
            }
            else
                for(auto i: t->source.pins)
                    hc->Source.pins.push_back(i);
        }

        if(le[i]->objectName().contains("Body", Qt::CaseInsensitive)){
            if(!SetPinsVect(le[i]->text().trimmed(), t->body.pins)){
                QMessageBox::critical(this, "Error", QString("Body's pins are not correct in HC test for DUT%1.\n"\
                                                             "Please, check them.").arg(index), QMessageBox::Ok);
                return false;
            }
            for(auto i: t->body.pins)
                hc->Body.pins.push_back(i);
        }

        if(le[i]->objectName().contains("Comment", Qt::CaseInsensitive))
            t->comment = le[i]->text().trimmed();

        if(le[i]->objectName().contains("Length", Qt::CaseInsensitive)){
            t->L = le[i]->text().trimmed().toDouble(&ok);
            if(!ok || t->L>MAX_WIDTH || t->L<MIN_WIDTH){
                QMessageBox::critical(this, "Error", QString("Lenght value is not correct in HC test for DUT%1.\n"\
                                                             "Please, enter value from %2 to %3")
                                      .arg(index).arg(MIN_WIDTH).arg(MAX_WIDTH), QMessageBox::Ok);
                return false;
            }
        }

        if(le[i]->objectName().contains("Width", Qt::CaseInsensitive)){
            t->W = le[i]->text().trimmed().toDouble(&ok);
            if(!ok || t->W>MAX_WIDTH || t->W<MIN_WIDTH){
                QMessageBox::critical(this, "Error", QString("Width value is not correct in HC test for DUT%1.\n"\
                                                             "Please, enter value from %2 to %3")
                                      .arg(index).arg(MIN_WIDTH).arg(MAX_WIDTH), QMessageBox::Ok);
                return false;
            }
        }

        if(le[i]->objectName().contains("Vgstart", Qt::CaseInsensitive)){
            t->Vgstart = le[i]->text().trimmed().toDouble(&ok);
            if(!ok || t->Vgstart>MAX_V || t->Vgstart<0.0){
                QMessageBox::critical(this, "Error", QString("Gate sweep start voltage value is not correct in HC test for DUT%1.\n"\
                                                             "Please, enter value from %2 to %3")
                                      .arg(index).arg(0).arg(MAX_V), QMessageBox::Ok);
                return false;
            }
        }

        if(le[i]->objectName().contains("Vgstop", Qt::CaseInsensitive)){
            t->Vgstop = le[i]->text().trimmed().toDouble(&ok);
            if(!ok || t->Vgstop>MAX_V || t->Vgstop<0.0){
                QMessageBox::critical(this, "Error", QString("Gate sweep stop voltage value is not correct in HC test for DUT%1.\n"\
                                                             "Please, enter value from %2 to %3")
                                      .arg(index).arg(0).arg(MAX_V), QMessageBox::Ok);
                return false;
            }
        }

        if(le[i]->objectName().contains("Vdlin", Qt::CaseInsensitive)){
            t->Vdlin = le[i]->text().trimmed().toDouble(&ok);
            if(!ok || t->Vdlin>MAX_V || t->Vdlin<0.0){
                QMessageBox::critical(this, "Error", QString("Drain voltage value for Vtlin search is not correct in HC test for DUT%1.\n"\
                                                             "Please, enter value from %2 to %3")
                                      .arg(index).arg(0).arg(MAX_V), QMessageBox::Ok);
                return false;
            }
        }

        if(le[i]->objectName().contains("Vdsat", Qt::CaseInsensitive)){
            t->Vdsat = le[i]->text().trimmed().toDouble(&ok);
            if(!ok || t->Vdsat>MAX_V || t->Vdsat<0.0){
                QMessageBox::critical(this, "Error", QString("Drain voltage value for Vtsar search is not correct in HC test for DUT%1.\n"\
                                                             "Please, enter value from %2 to %3")
                                      .arg(index).arg(0).arg(MAX_V), QMessageBox::Ok);
                return false;
            }
        }

        if(le[i]->objectName().contains("Idmax", Qt::CaseInsensitive)){
            t->Idmax = le[i]->text().trimmed().toDouble(&ok);
            if(!ok || t->Idmax>MAX_I || t->Idmax<0.0){
                QMessageBox::critical(this, "Error", QString("Maximum drain current value is not correct in HC test for DUT%1.\n"\
                                                             "Please, enter value from %2 to %3")
                                      .arg(index).arg(0).arg(MAX_I), QMessageBox::Ok);
                return false;
            }
        }

        if(le[i]->objectName().contains("Ith", Qt::CaseInsensitive)){
            t->Ith_sq = le[i]->text().trimmed().toDouble(&ok);
            if(!ok){
                QMessageBox::critical(this, "Error", QString("Threshold current value is not correct in HC test for DUT%1")
                                      .arg(index), QMessageBox::Ok);
                return false;
            }
        }
    }//end for

    if(t->Vgstart >= t->Vgstop){
        QMessageBox::critical(this, "Error", QString("Gate sweep start voltage must be less than stop voltage in HC test for DUT%1.\n"\
                                                     "Please, check these parameters."), QMessageBox::Ok);
        return false;
    }

    double I = t->Ith_sq * t->W / t->L;
    if(I>MAX_I || I<0.0){
        QMessageBox::critical(this, "Error", QString("Threshold current value is not correct in HC test for DUT%1.\n"\
                                                     "Please, enter value from %2 to %3")
                              .arg(index).arg(0).arg(MAX_I*t->L/t->W), QMessageBox::Ok);
        return false;
    }
    else
        t->Ith = I;

    t->Index = index;

    return true;
}
/* -----------------------------------------------------------------------------------------------*/
void MainWindow::HCSetData(const Transistor* const t, const QWidget *w)
{
    QList<QLineEdit*> le = w->findChildren<QLineEdit*>();
    QVector<const ViInt32>::iterator p;
    QString str;

    for(int i=0; i<le.size(); i++){
        if(le[i]->objectName().contains("Drain", Qt::CaseInsensitive)){
            p=t->drain.pins.begin();
            str = QString::number(*(++p));
            for(;p<t->drain.pins.end();p++)
                str = str + QString(",%1").arg(*p);
        }

        if(le[i]->objectName().contains("Gate", Qt::CaseInsensitive)){
            p=t->gate.pins.begin();
            str = QString::number(*(++p));
            for(;p<t->gate.pins.end();p++)
                str = str + QString(",%1").arg(*p);
        }

        if(le[i]->objectName().contains("Source", Qt::CaseInsensitive)){
            p=t->source.pins.begin();
            str = QString::number(*(++p));
            for(;p<t->source.pins.end();p++)
                str = str + QString(",%1").arg(*p);
        }

        if(le[i]->objectName().contains("Body", Qt::CaseInsensitive)){
            p=t->body.pins.begin();
            str = QString::number(*(++p));
            for(;p<t->body.pins.end();p++)
                str = str + QString(",%1").arg(*p);
        }

        if(le[i]->objectName().contains("Comment", Qt::CaseInsensitive))
            str = t->comment;

        if(le[i]->objectName().contains("Length", Qt::CaseInsensitive))
            str = QString("%1").arg(t->L);

        if(le[i]->objectName().contains("Width", Qt::CaseInsensitive))
            str = QString("%1").arg(t->W);

        if(le[i]->objectName().contains("Vgstart", Qt::CaseInsensitive))
            str = QString("%1").arg(t->Vgstart);

        if(le[i]->objectName().contains("Vgstop", Qt::CaseInsensitive))
            str = QString("%1").arg(t->Vgstop);

        if(le[i]->objectName().contains("Vdlin", Qt::CaseInsensitive))
            str = QString("%1").arg(t->Vdlin);

        if(le[i]->objectName().contains("Vdsat", Qt::CaseInsensitive))
            str = QString("%1").arg(t->Vdsat);

        if(le[i]->objectName().contains("Idmax", Qt::CaseInsensitive))
            str = QString("%1").arg(t->Idmax);

        if(le[i]->objectName().contains("Ith", Qt::CaseInsensitive))
            str = QString("%1").arg(t->Ith);

        le[i]->setText(str);
    }//end for
}
/*-----------------------------------------------------------------------------------------------*/
//проверка правильности введенных значений параметров для TDDB испытаний
bool MainWindow::TDDBParamValues(QRadioButton *Chann,
                                 QLineEdit *Temperature,
                                 QLineEdit *W,
                                 QLineEdit *L,
                                 QLineEdit *tox,
                                 QLineEdit *Vuse,
                                 QLineEdit *Iuse,
                                 QLineEdit *Istress,
                                 QLineEdit *Vsilc,
                                 QLineEdit *Vstress,
                                 QLineEdit *TimeLimit,
                                 QLineEdit *HighPin,
                                 QLineEdit *LowPin,
                                 QLineEdit *Comment,
                                 /*TDDB *t,*/
                                 int index){
    /* bool Ok;

     if(Temperature->text().length()<1 ||
             W->text().length()<1 ||
             L->text().length()<1 ||
             Vuse->text().length()<1 ||
             Iuse->text().length()<1 ||
             Istress->text().length()<1 ||
             Vsilc->text().length()<1 ||
             Vstress->text().length()<1 ||
             TimeLimit->text().length()<1 ||
             HighPin->text().length()<1 ||
             LowPin->text().length()<1)
         return false;

     t->Clear();

     //изменить параметр t->Index для измеренмя нескольких структур подряд (на будущее)
     t->Index = index;
     t->NumSMU = index;

     if(t->NumSMU<1){
         QMessageBox::critical(this, "Error", QString("Failed during SMU detecting for TDDB DUT%1. Please, try again")
                                .arg(index), QMessageBox::Ok);
         return false;
     }

     t->NumSlot = SetSlotOfSMU(t->NumSMU);
     if(t->NumSMU == 0){
         QMessageBox::critical(this, "Error", QString("Failed during SMU slot detecting for TDDB DUT%1. Please, try again")
                                .arg(index), QMessageBox::Ok);
         return false;
     }

     t->HighPin = (HighPin->text()).toDouble(&Ok);
     if(!Ok || t->HighPin>NUM_CARD_PINS || t->HighPin<1){
         QMessageBox::critical(this, "Error", QString("High terminal value for TDDB DUT%1 is not correct. Please, enter value from %2 to %3")
                                .arg(index).arg(1).arg(NUM_CARD_PINS), QMessageBox::Ok);
         return false;
     }

     if(!SetPinsVect(LowPin, t->LowPin)){
         QMessageBox::critical(this, "Error", QString("Low terminal values for TDDB DUT%1 are not correct")
                                .arg(index).arg(1).arg(NUM_CARD_PINS), QMessageBox::Ok);
         return false;
     }

     for(int i=0; i<t->LowPin.size(); i++)
         if(t->HighPin == t->LowPin[i]){
             QMessageBox::critical(this, "Error", QString("Pin numbers for high and low terminals is equal for TDDB DUT%1. Please, check them")
                                    .arg(index), QMessageBox::Ok);
             return false;
         }

     if(Chann->isChecked())
        t->TypeCh = NCHANN;
     else
        t->TypeCh = PCHANN;

     t->T = (Temperature->text()).toDouble(&Ok);
     if(!Ok || t->T>MAX_TEMP || t->T<MIN_TEMP){
         QMessageBox::critical(this, "Error", QString("Temperature value for TDDB DUT%1 is not correct. Please, enter value from %2 to %3")
                                .arg(index).arg(MIN_TEMP).arg(MAX_TEMP), QMessageBox::Ok);
         return false;
     }

     t->W = (W->text()).toDouble(&Ok);
     if(!Ok || t->W>MAX_WIDTH || t->W<MIN_WIDTH){
         QMessageBox::critical(this, "Error", QString("Width value for TDDB DUT%1 is not correct. Please, enter value from %2 to %3")
                                .arg(index).arg(MIN_WIDTH).arg(MAX_WIDTH), QMessageBox::Ok);
         return false;
     }

     t->L = (L->text()).toDouble(&Ok);
     if(!Ok || t->L>MAX_WIDTH || t->L<MIN_WIDTH){
         QMessageBox::critical(this, "Error", QString("Lenght value for TDDB DUT%1 is not correct. Please, enter value from %2 to %3")
                                .arg(index).arg(MIN_WIDTH).arg(MAX_WIDTH), QMessageBox::Ok);
         return false;
     }

     t->tox = (tox->text()).toDouble(&Ok);
     if(!Ok || t->tox>MAX_TOX || t->tox<MIN_TOX){
         QMessageBox::critical(this, "Error", QString("Thickness value for TDDB DUT%1 is not correct. Please, enter value from %2 to %3")
                                .arg(index).arg(MIN_TOX).arg(MAX_TOX), QMessageBox::Ok);
         return false;
     }

     t->Vuse = (Vuse->text()).toDouble(&Ok);
     if(!Ok || t->Vuse>MAX_V || t->Vuse<0.0){
         QMessageBox::critical(this, "Error", QString("Use voltage value for TDDB DUT%1 is not correct. Please, enter value from %2 to %3")
                                .arg(index).arg(0).arg(MAX_V), QMessageBox::Ok);
         return false;
     }

     t->Iuse_init = ((Iuse->text()).toDouble(&Ok))/1E+6;
     if(!Ok || t->Iuse_init>MAX_I || t->Iuse_init<0.0){
         QMessageBox::critical(this, "Error", QString("Use current value for TDDB DUT%1 is not correct. Please, enter value from %2A to %3A")
                                .arg(index).arg(0).arg(MAX_I), QMessageBox::Ok);
         return false;
     }

     t->Istress_init = ((Istress->text()).toDouble(&Ok))/1000.0;
     if(!Ok || t->Istress_init>MAX_I || t->Istress_init<0.0){
         QMessageBox::critical(this, "Error", QString("Stress current value for TDDB DUT%1 is not correct. Please, enter value from %2A to %3A")
                                .arg(index).arg(0).arg(MAX_I), QMessageBox::Ok);
         return false;
     }

     t->Vsilc = (Vsilc->text()).toDouble(&Ok);
     if(!Ok || t->Vsilc>MAX_V || t->Vsilc<0.0){
         QMessageBox::critical(this, "Error", QString("SILC voltage value for TDDB DUT%1 is not correct. Please, enter value from %2 to %3")
                                .arg(index).arg(0).arg(MAX_V), QMessageBox::Ok);
         return false;
     }

     t->Vstress = (Vstress->text()).toDouble(&Ok);
     if(!Ok || t->Vstress>MAX_V || t->Vstress<0.0){
         QMessageBox::critical(this, "Error", QString("Stress voltage value for TDDB DUT%1 is not correct. Please, enter value from %2 to %3")
                                .arg(index).arg(0).arg(MAX_V), QMessageBox::Ok);
         return false;
     }

     t->StressTime = (TimeLimit->text()).toDouble(&Ok);
     if(!Ok || t->StressTime>MAX_TIME || t->StressTime<MIN_TIME){
         QMessageBox::critical(this, "Error", QString("Maximum stress time value for TDDB DUT%1 is not correct. Please, enter value from %2 to %3")
                                .arg(index).arg(MIN_TIME).arg(MAX_TIME), QMessageBox::Ok);
         return false;
     }

     t->comment = Comment->text();

     if(t->TypeCh == PCHANN){
      t->Iuse_init = -1*t->Iuse_init;
      t->Istress_init = -1*t->Istress_init;
      t->Vsilc = -1*t->Vsilc;
      t->Vstress = -1*t->Vstress;
      t->Vuse = -1*t->Vuse;
     }*/

     return true;
}
/*-----------------------------------------------------------------------------------------------*/
bool MainWindow::NBTIReadAndCheckConnections(/*NBTI *t,*/
                                             QLineEdit *Drain,
                                             QLineEdit *Gate,
                                             QLineEdit *SourceBody,
                                             unsigned smu,
                                             int index)
{
    /* if(ui->rbtn_NBTI_Nch_1->isChecked())
         t->TypeCh = NCHANN;
     else
         t->TypeCh = PCHANN;

     t->gate_smu = smu;
     t->gate = SetSlotOfSMU(smu);

     t->drain_smu = SMU1;
     t->drain = SLOT_SMU1;

     if(t->gate_smu == t->drain_smu)
         return false;

     if(!SetPinsVect(Drain, t->drain_pins) ||
             !SetPinsVect(Gate, t->gate_pins) ||
             !SetPinsVect(SourceBody, t->sb_pins))
     {
         QMessageBox::critical(this, "Error", QString("Pin numbers are not correct in NBTI test for DUT%1. Please, check them.").arg(index),
                               QMessageBox::Ok);
         return false;
     }*/

     return true;
}
/*-----------------------------------------------------------------------------------------------*/
//проверка правильности введенных значений параметров для NBTI испытаний
bool MainWindow::NBTIReadAndCheckParamValues(QRadioButton *Chann,
                                            QLineEdit *Temperature,
                                            QComboBox *StressTime,
                                            QLineEdit *W,
                                            QLineEdit *L,
                                            QLineEdit *Vstart,
                                            QLineEdit *Vstop,
                                            QLineEdit *Ithreshold,
                                            QLineEdit *IMax,
                                            QLineEdit *Vdlin,
                                            QLineEdit *Vdsat,
                                            QLineEdit *Vdstress,
                                            QLineEdit *Vgstress,
                                            /*NBTI *t,*/
                                            int index){
     /*bool Ok;
     double I, Imax;

     if(W->text().length()<1 ||
             L->text().length()<1 ||
             Vstart->text().length()<1 ||
             Vstop->text().length()<1 ||
             Ithreshold->text().length()<1 ||
             IMax->text().length()<1 ||
             Vdlin->text().length()<1 ||
             Vdsat->text().length()<1)
         return false;

     t->Index = index;

     t->T = (Temperature->text()).toDouble(&Ok);
     if(!Ok || t->T>MAX_TEMP || t->T<MIN_TEMP)
     {
         QMessageBox::critical(this, "Error", QString("Stress temperature value is not correct for NBTI DUT%1. Please, enter value from %2 to %3")
                               .arg(index).arg(MIN_TEMP).arg(MAX_TEMP), QMessageBox::Ok);
         return false;
     }

     t->StressTime = (StressTime->currentText()).toDouble(&Ok);
     if(!Ok || t->StressTime>MAX_TIME || t->StressTime<MIN_TIME)
     {
         QMessageBox::critical(this, "Error", QString("Time limit is not correct for NBTI DUT%1. Please, enter value from %2 to %3")
                               .arg(index).arg(MIN_TIME).arg(MAX_TIME), QMessageBox::Ok);
         return false;
     }

     if(Chann->isChecked())
         t->TypeCh = NCHANN;
     else
         t->TypeCh = PCHANN;

     t->W = (W->text()).toDouble(&Ok);
     if(!Ok || t->W>MAX_WIDTH || t->W<MIN_WIDTH)
     {
         QMessageBox::critical(this, "Error", QString("Width value is not correct for NBTI DUT%1. Please, enter value from %2 to %3")
                               .arg(index).arg(MIN_WIDTH).arg(MAX_WIDTH), QMessageBox::Ok);
         return false;
     }

     t->L = (L->text()).toDouble(&Ok);
     if(!Ok || t->L>MAX_WIDTH || t->L<MIN_WIDTH)
     {
         QMessageBox::critical(this, "Error", QString("Lenght value is not correct for NBTI DUT%1. Please, enter value from %2 to %3")
                               .arg(index).arg(MIN_WIDTH).arg(MAX_WIDTH), QMessageBox::Ok);
         return false;
     }

     Imax = MAX_I / t->W * t->L;

     t->Idmax = (IMax->text()).toDouble(&Ok);
     if(!Ok || t->Idmax>MAX_I || t->Idmax<0.0)
     {
         QMessageBox::critical(this, "Error", QString("Maximum drain current value is not correct for NBTI DUT%1. Please, enter value from %2 to %3")
                               .arg(index).arg(0).arg(MAX_I), QMessageBox::Ok);
         return false;
     }

     t->Ith_sq = (Ithreshold->text()).toDouble(&Ok);
     if(!Ok)
     {
         QMessageBox::critical(this, "Error", QString("Threshold current value is not correct for NBTI DUT%1. Please, enter value from %2 to %3")
                               .arg(index).arg(0).arg(Imax), QMessageBox::Ok);
         return false;
     }
     else
     {
         I = t->Ith_sq * t->W / t->L;
         if(I>Imax || I<0.0)
         {
             QMessageBox::critical(this, "Error", QString("Threshold current value is not correct for NBTI DUT%1. Please, enter value from %2 to %3")
                                   .arg(index).arg(0).arg(Imax), QMessageBox::Ok);
             return false;
         }

     }
     t->Ith = I;


     t->Vdstress = (Vdstress->text()).toDouble(&Ok);
     if(!Ok || t->Vdstress>MAX_V || t->Vdstress<0.0)
     {
         QMessageBox::critical(this, "Error", QString("Drain stress voltage value is not correct for NBTI DUT%1. Please, enter value from %2 to %3")
                               .arg(index).arg(0).arg(MAX_V), QMessageBox::Ok);
         return false;
     }

     t->Vgstress = (Vgstress->text()).toDouble(&Ok);
     if(!Ok || t->Vgstress>MAX_V || t->Vgstress<0.0)
     {
         QMessageBox::critical(this, "Error", QString("Gate stress voltage value is not correct for NBTI DUT%1. Please, enter value from %2 to %3")
                               .arg(index).arg(0).arg(MAX_V), QMessageBox::Ok);
         return false;
     }

     t->Vgstart = (Vstart->text()).toDouble(&Ok);
     if(!Ok || t->Vgstart>MAX_V || t->Vgstart<0.0)
     {
         QMessageBox::critical(this, "Error", QString("Gate sweep start voltage value is not correct for NBTI DUT%1. Please, enter value from %2 to %3")
                               .arg(index).arg(0).arg(MAX_V), QMessageBox::Ok);
         return false;
     }

     t->Vgstop = (Vstop->text()).toDouble(&Ok);
     if(!Ok || t->Vgstop>MAX_V || t->Vgstop<0.0)
     {
         QMessageBox::critical(this, "Error", QString("Gate sweep stop voltage value is not correct for NBTI DUT%1. Please, enter value from %2 to %3")
                               .arg(index).arg(0).arg(MAX_V), QMessageBox::Ok);
         return false;
     }

     if(t->Vgstart >= t->Vgstop)
     {
         QMessageBox::critical(this, "Error", QString("Gate sweep start voltage must be less than stop voltage for NBTI DUT%1. Please, check these parameters.").arg(index)
                               , QMessageBox::Ok);
         return false;
     }

     t->Vtlin = (Vdlin->text()).toDouble(&Ok);
     if(!Ok || t->Vtlin>MAX_V || t->Vtlin<0.0)
     {
         QMessageBox::critical(this, "Error", QString("Drain voltage value for Vtlin is not correct for NBTI DUT%1. Please, enter value from %2 to %3")
                               .arg(index).arg(0).arg(MAX_V), QMessageBox::Ok);
         return false;
     }

     t->Vtsat = (Vdsat->text()).toDouble(&Ok);
     if(!Ok || t->Vtsat>MAX_V || t->Vtsat<0.0)
     {
         QMessageBox::critical(this, "Error", QString("Drain voltage value for Vtsat is not correct for NBTI DUT%1. Please, enter value from %2 to %3")
                               .arg(index).arg(0).arg(MAX_V), QMessageBox::Ok);
         return false;
     }

     if(t->TypeCh == PCHANN){
      t->Vdstress = -1*t->Vdstress;
      t->Vgstress = -1*t->Vgstress;

      t->Vgstart = -1*t->Vgstart;
      t->Vgstop = -1*t->Vgstop;

      t->Vtlin = -1*t->Vtlin;
      t->Vtsat = -1*t->Vtsat;

      t->Idmax = -1*t->Idmax;
      t->Ith = -1*t->Ith;
      t->Ith_sq = -1*t->Ith_sq;
     }

     if(ui->rb_NBTI_Classic->isChecked())
         t->ClassicTest = 1;
     else
         t->ClassicTest = 0;*/

     return true;
}
/*-----------------------------------------------------------------------------------------------*/
bool MainWindow::VRDBParamValues(/*VRDB *t*/)
{
   /* bool Ok;

    if(ui->le_VRDB_Width->text().length()<1 ||
            ui->le_VRDB_Lenght->text().length()<1 ||
            ui->le_VRDB_tox->text().length()<1 ||
            ui->le_VRDB_Vuse->text().length()<1 ||
            ui->le_VRDB_Vstop->text().length()<1 ||
            ui->le_VRDB_Rate->text().length()<1 ||
            ui->le_VRDB_Iuse->text().length()<1)
        return false;

    if(ui->rbtn_VRDB_Ramp->isChecked())
        t->TypeTest = VRDB_TYPE_VRAMP;
    else
        t->TypeTest = VRDB_TYPE_VUPR;

    t->HighPin = (ui->le_VRDB_High->text()).toDouble(&Ok);
    if(!Ok || t->HighPin>NUM_CARD_PINS || t->HighPin<1){
        QMessageBox::critical(this, "Error", QString("High terminal value for VRDB test is not correct.\n"
                                                  "Please, enter value from %1 to %2.")
                               .arg(1).arg(NUM_CARD_PINS), QMessageBox::Ok);
        return false;
    }

    if(!SetPinsVect(ui->le_VRDB_Low, t->LowPin)){
        QMessageBox::critical(this, "Error", QString("Low terminal values for VRDB test are not correct.\n"
                                                  "Please, enter value from %1 to %2.")
                               .arg(1).arg(NUM_CARD_PINS), QMessageBox::Ok);
        return false;
    }

    for(int i=0; i<t->LowPin.size(); i++)
        if(t->HighPin == t->LowPin[i]){
            QMessageBox::critical(this, "Error", QString("Pin numbers for high and low terminals is equal for VRDB test.\n"
                                                      "Please, check them."), QMessageBox::Ok);
            return false;
        }

    if(ui->rbtn_VRDB_Nch->isChecked())
        t->TypeCh = NCHANN;
    else
        t->TypeCh = PCHANN;

    t->T = (ui->le_VRDB_Tstress->text()).toDouble(&Ok);
    if(!Ok || t->T>MAX_TEMP || t->T<MIN_TEMP){
        QMessageBox::critical(this, "Error", QString("Stress temperature value is not correct for VRDB test.\n"
                                                  "Please, enter value from %1 to %2.")
                              .arg(MIN_TEMP).arg(MAX_TEMP), QMessageBox::Ok);
        return false;
    }

    t->W = (ui->le_VRDB_Width->text()).toDouble(&Ok);
    if(!Ok || t->W>MAX_WIDTH || t->W<MIN_WIDTH){
        QMessageBox::critical(this, "Error", QString("Width value is not correct for VRDB test. Please, enter value from %1 to %2")
                               .arg(MIN_WIDTH).arg(MAX_WIDTH), QMessageBox::Ok);
        return false;
    }

    t->L = (ui->le_VRDB_Lenght->text()).toDouble(&Ok);
    if(!Ok || t->L>MAX_WIDTH || t->L<MIN_WIDTH){
        QMessageBox::critical(this, "Error", QString("Lenght value is not correct for VRDB test. Please, enter value from %1 to %2")
                               .arg(MIN_WIDTH).arg(MAX_WIDTH), QMessageBox::Ok);
        return false;
    }

    t->tox = (ui->le_VRDB_tox->text()).toDouble(&Ok);
    if(!Ok || t->tox>MAX_TOX || t->tox<MIN_TOX){
        QMessageBox::critical(this, "Error", QString("Thickness value is not correct for VRDB test. Please, enter value from %1 to %2")
                               .arg(MIN_TOX).arg(MAX_TOX), QMessageBox::Ok);
        return false;
    }

    t->Vuse = (ui->le_VRDB_Vuse->text()).toDouble(&Ok);
    if(!Ok || t->Vuse>MAX_HV || t->Vuse<0.0){
        QMessageBox::critical(this, "Error", QString("Use voltage value for VRDB test is not correct. Please, enter value from %1 to %2")
                                .arg(0).arg(MAX_HV), QMessageBox::Ok);
        return false;
    }

    t->Vstop = (ui->le_VRDB_Vstop->text()).toDouble(&Ok);
    if(!Ok || t->Vstop>MAX_HV || t->Vstop<0.0){
        QMessageBox::critical(this, "Error", QString("Stop voltage value for VRDB test is not correct. Please, enter value from %1 to %2")
                                .arg(0).arg(MAX_HV), QMessageBox::Ok);
        return false;
    }

    t->Rate = (ui->le_VRDB_Rate->text()).toDouble(&Ok);
    if(!Ok || t->Rate<=0.0){
        QMessageBox::critical(this, "Error", QString("Step voltage value for VRDB test is not correct. Please, enter value more than zero")
                                , QMessageBox::Ok);
        return false;
    }

    t->t_hold = 0.0;   //10 ms

    t->Iuse = (ui->le_VRDB_Iuse->text()).toDouble(&Ok)/1E+6;
    if(!Ok || t->Iuse>MAX_I/100.0 || t->Iuse<0.0){
        QMessageBox::critical(this, "Error", QString("Use current value for VRDB test is not correct. Please, enter value from %1 to %2")
                                .arg(0).arg(MAX_I/100.0), QMessageBox::Ok);
        return false;
    }

    t->Slope = (ui->le_VRDB_Slope->text()).toDouble(&Ok);
    if(!Ok || t->Slope<=0){
        QMessageBox::critical(this, "Error", QString("Slope value for VRDB test is not correct. "
                                                     "Please, enter value from more than 0"), QMessageBox::Ok);
        return false;
    }

    t->Imax = (ui->le_VRDB_Imax->text()).toDouble(&Ok);
    if(!Ok || t->Imax>MAX_I){
        QMessageBox::critical(this, "Error", QString("Maximum current value for VRDB test is not correct. "
                                                     "Please, enter value from %1 to %2").arg(0).arg(MAX_I), QMessageBox::Ok);
        return false;
    }

    t->Comment = ui->le_VRDB_Comment->text();

    t->Istop = VRDB_I_ABS_BRKDWN * t->L * t->W * 1E-8;
    if(t->Istop > MAX_I)
        t->Istop = MAX_I;

    if(t->Istop > t->Imax)
        t->Istop = t->Imax;

    t->Vstart = 0.0;
    t->Vstep = t->tox * VRDB_RAMP_STEP;
    t->points = int((t->Vstop - t->Vstart)/t->Vstep + 1.0);   //количество точек для испытаний

    if(t->points>VRDB_NUM_STEPS){
        t->points = VRDB_NUM_STEPS;
        t->Vstep = int((t->Vstop - t->Vstart)/VRDB_NUM_STEPS);
    }

    t->period = t->Vstep / t->tox / t->Rate * 10;    //period calculation in sec
    if(t->period>VRDB_MAX_STEPTIME || t->period<VRDB_MIN_STEPTIME){
        QMessageBox::critical(this, "Error", QString("Time step for VRDB test is %2. It must be less than 100ms.\nPlease, check oxide thickness and Vstep values.")
                                .arg(t->period), QMessageBox::Ok);
        return false;
    }

    t->bNeedCal = ui->chb_VRDB_NeedCal->isChecked();

    t->t_delay = VRDB_MIN_DELAY * 5.0;        //пятикратное увеличение минимально возможной задержки, чтобы измерение точно прошло за время, отведенное на ступеньку
    t->t_sdelay = t->period - t->t_delay;

    if(t->TypeCh == PCHANN){
     t->Vuse = -1*t->Vuse;
     t->Iuse = -1*t->Iuse;
     t->Vstep = -1*t->Vstep;
     t->Rate = -1*t->Rate;
     t->Vstop = -1*t->Vstop;
     t->Istop = -1*t->Istop;
     t->Vstart = -1*t->Vstart;
    }

    t->Vg0 = t->Vuse/10.0;
    t->VlowE = t->Vuse/3.0;*/

    return true;
}
/*-----------------------------------------------------------------------------------------------*/
bool MainWindow::VRDBSetParams(/*VRDB *t*/)
{
    /*QString s;
    int cnt;

    switch(t->TypeTest){
        case VRDB_TYPE_VRAMP: ui->rbtn_VRDB_Ramp->setChecked(true); break;

        case VRDB_TYPE_VUPR: ui->rbtn_VRDB_UPR->setChecked(true); break;

        default: ui->rbtn_VRDB_Ramp->setChecked(true); break;
    };

    if(t->TypeCh == NCHANN)
        ui->rbtn_VRDB_Nch->setChecked(true);
    else
        ui->rbtn_VRDB_Pch->setChecked(true);

    ui->cb_VRDB_HighSMU->setCurrentIndex(t->NumSMUHigh-1);
    ui->cb_VRDB_LowSMU->setCurrentIndex(t->NumSMULow-1);

    ui->le_VRDB_High->setText(QString::number(t->HighPin));

    s = "";
    cnt = 0;
    for(int i=0; i<t->LowPin.size(); i++){
        if(cnt == 0)
            s = s + QString::number(t->LowPin[i]);
        else
            s = s + "," + QString::number(t->LowPin[i]);
        cnt++;
    }
    ui->le_VRDB_Low->setText(s);

    ui->le_VRDB_Width->setText(QString("%1").arg(t->W));
    ui->le_VRDB_Lenght->setText(QString("%1").arg(t->L));
    ui->le_VRDB_tox->setText(QString("%1").arg(t->tox));
    ui->le_VRDB_Tstress->setText(QString("%1").arg(t->T));
    ui->le_VRDB_Vuse->setText(QString("%1").arg(t->Vuse));
    ui->le_VRDB_Vstop->setText(QString("%1").arg(t->Vstop));
    ui->le_VRDB_Rate->setText(QString("%1").arg(t->Rate));
    ui->le_VRDB_Iuse->setText(QString("%1").arg(t->Iuse*1E+6));
    ui->le_VRDB_Slope->setText(QString("%1").arg(t->Slope));
    ui->le_VRDB_Imax->setText(QString("%1").arg(t->Imax));*/

    return true;
}
/*-----------------------------------------------------------------------------------------------*/
bool MainWindow::TDDBReadData(GOI *tddb, Capacitor *t, const QWidget * const w, const int &index)
{
    QList<QLineEdit*> le = w->findChildren<QLineEdit*>();
    QVector<const ViInt32>::iterator p;
    QString str;
    int i;

    for(i=0; i<smu.size(); i++){
        if(i==index)
            t->High = smu.at(i);

        if(smu.at(i).Slot == SLOT_GNDU)
            t->Low = smu.at(i);
    }

    for(i=0; i<le.size(); i++){
        if(le[i]->objectName().contains("High", Qt::CaseInsensitive)){
            p=t->High.pins.begin();
            str = QString::number(*(++p));
            for(;p<t->High.pins.end();p++)
                str = str + QString(",%1").arg(*p);
        }

        if(le[i]->objectName().contains("Low", Qt::CaseInsensitive)){
            p=t->Low.pins.begin();
            str = QString::number(*(++p));
            for(;p<t->Low.pins.end();p++)
                str = str + QString(",%1").arg(*p);
        }

        if(le[i]->objectName().contains("Comment", Qt::CaseInsensitive))
            str = t->comment;

        if(le[i]->objectName().contains("Length", Qt::CaseInsensitive))
            str = QString("%1").arg(t->L);

        if(le[i]->objectName().contains("Width", Qt::CaseInsensitive))
            str = QString("%1").arg(t->W);

        if(le[i]->objectName().contains("Vgstart", Qt::CaseInsensitive))
            str = QString("%1").arg(t->Vgstart);

        if(le[i]->objectName().contains("Vgstop", Qt::CaseInsensitive))
            str = QString("%1").arg(t->Vgstop);

        if(le[i]->objectName().contains("Vdlin", Qt::CaseInsensitive))
            str = QString("%1").arg(t->Vdlin);

        if(le[i]->objectName().contains("Vdsat", Qt::CaseInsensitive))
            str = QString("%1").arg(t->Vdsat);

        if(le[i]->objectName().contains("Idmax", Qt::CaseInsensitive))
            str = QString("%1").arg(t->Idmax);

        if(le[i]->objectName().contains("Ith", Qt::CaseInsensitive))
            str = QString("%1").arg(t->Ith);

        le[i]->setText(str);
    }//end for
}
/*-----------------------------------------------------------------------------------------------*/
bool MainWindow::TDDBSetParams(/*TDDB *t*/)
{
    bool Result = true;
    /*QString s;
    int cnt;
    QVector<unsigned>::iterator it;

    ui->le_TDDB_T->setText(QString("%1").arg(t->T));
    ui->le_TDDB_TimeLimit->setText(QString("%1").arg(t->StressTime));

    switch(t->NumSMU){
        case 1:
                if(t->TypeCh == NCHANN)
                    ui->rbtn_TDDB_Nch_1->setChecked(true);
                else
                    ui->rbtn_TDDB_Pch_1->setChecked(true);

                ui->le_TDDB_Length_1->setText(QString("%1").arg(t->L));
                ui->le_TDDB_Width_1->setText(QString("%1").arg(t->W));
                ui->le_TDDB_Iuse_1->setText(QString("%1").arg(t->Iuse_init));
                ui->le_TDDB_tox_1->setText(QString("%1").arg(t->tox));
                ui->le_TDDB_Vsilc_1->setText(QString("%1").arg(t->Vsilc));
                ui->le_TDDB_Vstress_1->setText(QString("%1").arg(t->Vstress));
                ui->le_TDDB_Vuse_1->setText(QString("%1").arg(t->Vuse));
                ui->le_TDDB_Istress_1->setText(QString("%1").arg(t->Istress_init));
                ui->le_TDDB_High_1->setText(QString::number(t->HighPin));

                s = "";
                cnt = 0;
                for(it=t->LowPin.begin(); it<t->LowPin.end(); it++){
                        if(cnt == 0)
                            s = s + QString::number(*it);
                        else
                            s = s + "," + QString::number(*it);
                        cnt++;
                    }
                ui->le_TDDB_Low_1->setText(s); break;

        case 2:
                if(t->TypeCh == NCHANN)
                    ui->rbtn_TDDB_Nch_2->setChecked(true);
                else
                    ui->rbtn_TDDB_Pch_2->setChecked(true);

                ui->le_TDDB_Length_2->setText(QString("%1").arg(t->L));
                ui->le_TDDB_Width_2->setText(QString("%1").arg(t->W));
                ui->le_TDDB_Iuse_2->setText(QString("%1").arg(t->Iuse_init));
                ui->le_TDDB_tox_2->setText(QString("%1").arg(t->tox));
                ui->le_TDDB_Vsilc_2->setText(QString("%1").arg(t->Vsilc));
                ui->le_TDDB_Vstress_2->setText(QString("%1").arg(t->Vstress));
                ui->le_TDDB_Vuse_2->setText(QString("%1").arg(t->Vuse));
                ui->le_TDDB_Istress_2->setText(QString("%1").arg(t->Istress_init));
                ui->le_TDDB_High_2->setText(QString::number(t->HighPin));

                s = "";
                cnt = 0;
                for(it=t->LowPin.begin(); it<t->LowPin.end(); it++){
                        if(cnt == 0)
                            s = s + QString::number(*it);
                        else
                            s = s + "," + QString::number(*it);
                        cnt++;
                    }
                ui->le_TDDB_Low_2->setText(s); break;

        case 3:
                if(t->TypeCh == NCHANN)
                    ui->rbtn_TDDB_Nch_3->setChecked(true);
                else
                    ui->rbtn_TDDB_Pch_3->setChecked(true);
                ui->le_TDDB_Length_3->setText(QString("%1").arg(t->L));
                ui->le_TDDB_Width_3->setText(QString("%1").arg(t->W));
                ui->le_TDDB_Iuse_3->setText(QString("%1").arg(t->Iuse_init));
                ui->le_TDDB_tox_3->setText(QString("%1").arg(t->tox));
                ui->le_TDDB_Vsilc_3->setText(QString("%1").arg(t->Vsilc));
                ui->le_TDDB_Vstress_3->setText(QString("%1").arg(t->Vstress));
                ui->le_TDDB_Vuse_3->setText(QString("%1").arg(t->Vuse));
                ui->le_TDDB_Istress_3->setText(QString("%1").arg(t->Istress_init));
                ui->le_TDDB_High_3->setText(QString::number(t->HighPin));

                s = "";
                cnt = 0;
                for(it=t->LowPin.begin(); it<t->LowPin.end(); it++){
                        if(cnt == 0)
                            s = s + QString::number(*it);
                        else
                            s = s + "," + QString::number(*it);
                        cnt++;
                    }
                ui->le_TDDB_Low_3->setText(s); break;

        case 4:
                if(t->TypeCh == NCHANN)
                    ui->rbtn_TDDB_Nch_4->setChecked(true);
                else
                    ui->rbtn_TDDB_Pch_4->setChecked(true);

                ui->le_TDDB_Length_4->setText(QString("%1").arg(t->L));
                ui->le_TDDB_Width_4->setText(QString("%1").arg(t->W));
                ui->le_TDDB_Iuse_4->setText(QString("%1").arg(t->Iuse_init));
                ui->le_TDDB_tox_4->setText(QString("%1").arg(t->tox));
                ui->le_TDDB_Vsilc_4->setText(QString("%1").arg(t->Vsilc));
                ui->le_TDDB_Vstress_4->setText(QString("%1").arg(t->Vstress));
                ui->le_TDDB_Vuse_4->setText(QString("%1").arg(t->Vuse));
                ui->le_TDDB_Istress_4->setText(QString("%1").arg(t->Istress_init));
                ui->le_TDDB_High_4->setText(QString::number(t->HighPin));

                s = "";
                cnt = 0;
                for(it=t->LowPin.begin(); it<t->LowPin.end(); it++){
                        if(cnt == 0)
                            s = s + QString::number(*it);
                        else
                            s = s + "," + QString::number(*it);
                        cnt++;
                    }
                ui->le_TDDB_Low_4->setText(s); break;

        case 5:
                if(t->TypeCh == NCHANN)
                    ui->rbtn_TDDB_Nch_5->setChecked(true);
                else
                    ui->rbtn_TDDB_Pch_5->setChecked(true);

                ui->le_TDDB_Length_5->setText(QString("%1").arg(t->L));
                ui->le_TDDB_Width_5->setText(QString("%1").arg(t->W));
                ui->le_TDDB_Iuse_5->setText(QString("%1").arg(t->Iuse_init));
                ui->le_TDDB_tox_5->setText(QString("%1").arg(t->tox));
                ui->le_TDDB_Vsilc_5->setText(QString("%1").arg(t->Vsilc));
                ui->le_TDDB_Vstress_5->setText(QString("%1").arg(t->Vstress));
                ui->le_TDDB_Vuse_5->setText(QString("%1").arg(t->Vuse));
                ui->le_TDDB_Istress_5->setText(QString("%1").arg(t->Istress_init));
                ui->le_TDDB_High_5->setText(QString::number(t->HighPin));

                s = "";
                cnt = 0;
                for(it=t->LowPin.begin(); it<t->LowPin.end(); it++){
                        if(cnt == 0)
                            s = s + QString::number(*it);
                        else
                            s = s + "," + QString::number(*it);
                        cnt++;
                    }
                ui->le_TDDB_Low_5->setText(s); break;

        case 6:
            if(t->TypeCh == NCHANN)
                ui->rbtn_TDDB_Nch_6->setChecked(true);
            else
                ui->rbtn_TDDB_Pch_6->setChecked(true);

            ui->le_TDDB_Length_6->setText(QString("%1").arg(t->L));
            ui->le_TDDB_Width_6->setText(QString("%1").arg(t->W));
            ui->le_TDDB_Iuse_6->setText(QString("%1").arg(t->Iuse_init));
            ui->le_TDDB_tox_6->setText(QString("%1").arg(t->tox));
            ui->le_TDDB_Vsilc_6->setText(QString("%1").arg(t->Vsilc));
            ui->le_TDDB_Vstress_6->setText(QString("%1").arg(t->Vstress));
            ui->le_TDDB_Vuse_6->setText(QString("%1").arg(t->Vuse));
            ui->le_TDDB_Istress_6->setText(QString("%1").arg(t->Istress_init));
            ui->le_TDDB_High_6->setText(QString::number(t->HighPin));

            s = "";
            cnt = 0;
            for(it=t->LowPin.begin(); it<t->LowPin.end(); it++){
                    if(cnt == 0)
                        s = s + QString::number(*it);
                    else
                        s = s + "," + QString::number(*it);
                    cnt++;
                }
            ui->le_TDDB_Low_6->setText(s); break;

        default: Result=false; break;
    };*/

    return Result;
}
/*-----------------------------------------------------------------------------------------------*/
bool MainWindow::NBTISetParams(/*NBTI *t*/)
{
    QString s;
    int cnt;
    bool Result = true;
    /*QVector<unsigned>::iterator it;

    ui->le_NBTI_Tstress->setText(QString("%1").arg(t->T));
    ui->cb_NBTI_StressTime->setCurrentText(QString("%1").arg(t->StressTime));
    ui->le_NBTI_Vgstress->setText(QString("%1").arg(t->Vgstress));
    ui->le_NBTI_Vdstress->setText(QString("%1").arg(t->Vdstress));
    if(t->ClassicTest)
        ui->rb_NBTI_Classic->setChecked(true);
    else
        ui->rb_NBTI_ONF->setChecked(true);

    switch(t->Index){
        case 1:
                if(t->TypeCh == NCHANN)
                    ui->rbtn_NBTI_Nch_1->setChecked(true);
                else
                    ui->rbtn_NBTI_Pch_1->setChecked(true);

                ui->le_NBTI_Length_1->setText(QString("%1").arg(t->L));
                ui->le_NBTI_Width_1->setText(QString("%1").arg(t->W));
                ui->le_NBTI_Vgstart_1->setText(QString("%1").arg(t->Vgstart));
                ui->le_NBTI_Vgstop_1->setText(QString("%1").arg(t->Vgstop));
                ui->le_NBTI_Idmax_1->setText(QString("%1").arg(t->Idmax));
                ui->le_NBTI_Ith_1->setText(QString("%1").arg(t->Ith_sq));
                ui->le_NBTI_Vdlin_1->setText(QString("%1").arg(t->Vtlin));
                ui->le_NBTI_Vdsat_1->setText(QString("%1").arg(t->Vtsat));

                s = "";
                cnt = 0;
                for(it=t->drain_pins.begin(); it<t->drain_pins.end(); it++){
                        if(cnt == 0)
                            s = s + QString::number(*it);
                        else
                            s = s + "," + QString::number(*it);
                        cnt++;
                    }
                ui->le_NBTI_Drain_1->setText(s);

                s = "";
                cnt = 0;
                for(it=t->gate_pins.begin(); it<t->gate_pins.end(); it++){
                        if(cnt == 0)
                            s = s + QString::number(*it);
                        else
                            s = s + "," + QString::number(*it);
                        cnt++;
                    }
                ui->le_NBTI_Gate_1->setText(s);

                s = "";
                cnt = 0;
                for(it=t->sb_pins.begin(); it<t->sb_pins.end(); it++){
                        if(cnt == 0)
                            s = s + QString::number(*it);
                        else
                            s = s + "," + QString::number(*it);
                        cnt++;
                    }
                ui->le_NBTI_SourceBody_1->setText(s);break;
        case 2:
            if(t->TypeCh == NCHANN)
                ui->rbtn_NBTI_Nch_2->setChecked(true);
            else
                ui->rbtn_NBTI_Pch_2->setChecked(true);

            ui->le_NBTI_Length_2->setText(QString("%1").arg(t->L));
            ui->le_NBTI_Width_2->setText(QString("%1").arg(t->W));
            ui->le_NBTI_Vgstart_2->setText(QString("%1").arg(t->Vgstart));
            ui->le_NBTI_Vgstop_2->setText(QString("%1").arg(t->Vgstop));
            ui->le_NBTI_Idmax_2->setText(QString("%1").arg(t->Idmax));
            ui->le_NBTI_Ith_2->setText(QString("%1").arg(t->Ith_sq));
            ui->le_NBTI_Vdlin_2->setText(QString("%1").arg(t->Vtlin));
            ui->le_NBTI_Vdsat_2->setText(QString("%1").arg(t->Vtsat));

            s = "";
            cnt = 0;
            for(it=t->drain_pins.begin(); it<t->drain_pins.end(); it++){
                    if(cnt == 0)
                        s = s + QString::number(*it);
                    else
                        s = s + "," + QString::number(*it);
                    cnt++;
                }
            ui->le_NBTI_Drain_2->setText(s);

            s = "";
            cnt = 0;
            for(it=t->gate_pins.begin(); it<t->gate_pins.end(); it++){
                    if(cnt == 0)
                        s = s + QString::number(*it);
                    else
                        s = s + "," + QString::number(*it);
                    cnt++;
                }
            ui->le_NBTI_Gate_2->setText(s);

            s = "";
            cnt = 0;
            for(it=t->sb_pins.begin(); it<t->sb_pins.end(); it++){
                    if(cnt == 0)
                        s = s + QString::number(*it);
                    else
                        s = s + "," + QString::number(*it);
                    cnt++;
                }
            ui->le_NBTI_SourceBody_2->setText(s);break;

        case 3:
            if(t->TypeCh == NCHANN)
                ui->rbtn_NBTI_Nch_3->setChecked(true);
            else
                ui->rbtn_NBTI_Pch_3->setChecked(true);

            ui->le_NBTI_Length_3->setText(QString("%1").arg(t->L));
            ui->le_NBTI_Width_3->setText(QString("%1").arg(t->W));
            ui->le_NBTI_Vgstart_3->setText(QString("%1").arg(t->Vgstart));
            ui->le_NBTI_Vgstop_3->setText(QString("%1").arg(t->Vgstop));
            ui->le_NBTI_Idmax_3->setText(QString("%1").arg(t->Idmax));
            ui->le_NBTI_Ith_3->setText(QString("%1").arg(t->Ith_sq));
            ui->le_NBTI_Vdlin_3->setText(QString("%1").arg(t->Vtlin));
            ui->le_NBTI_Vdsat_3->setText(QString("%1").arg(t->Vtsat));

            s = "";
            cnt = 0;
            for(it=t->drain_pins.begin(); it<t->drain_pins.end(); it++){
                    if(cnt == 0)
                        s = s + QString::number(*it);
                    else
                        s = s + "," + QString::number(*it);
                    cnt++;
                }
            ui->le_NBTI_Drain_3->setText(s);

            s = "";
            cnt = 0;
            for(it=t->gate_pins.begin(); it<t->gate_pins.end(); it++){
                    if(cnt == 0)
                        s = s + QString::number(*it);
                    else
                        s = s + "," + QString::number(*it);
                    cnt++;
                }
            ui->le_NBTI_Gate_3->setText(s);

            s = "";
            cnt = 0;
            for(it=t->sb_pins.begin(); it<t->sb_pins.end(); it++){
                    if(cnt == 0)
                        s = s + QString::number(*it);
                    else
                        s = s + "," + QString::number(*it);
                    cnt++;
                }
            ui->le_NBTI_SourceBody_3->setText(s);break;

        case 4:
            if(t->TypeCh == NCHANN)
                ui->rbtn_NBTI_Nch_4->setChecked(true);
            else
                ui->rbtn_NBTI_Pch_4->setChecked(true);

            ui->le_NBTI_Length_4->setText(QString("%1").arg(t->L));
            ui->le_NBTI_Width_4->setText(QString("%1").arg(t->W));
            ui->le_NBTI_Vgstart_4->setText(QString("%1").arg(t->Vgstart));
            ui->le_NBTI_Vgstop_4->setText(QString("%1").arg(t->Vgstop));
            ui->le_NBTI_Idmax_4->setText(QString("%1").arg(t->Idmax));
            ui->le_NBTI_Ith_4->setText(QString("%1").arg(t->Ith_sq));
            ui->le_NBTI_Vdlin_4->setText(QString("%1").arg(t->Vtlin));
            ui->le_NBTI_Vdsat_4->setText(QString("%1").arg(t->Vtsat));

            s = "";
            cnt = 0;
            for(it=t->drain_pins.begin(); it<t->drain_pins.end(); it++){
                    if(cnt == 0)
                        s = s + QString::number(*it);
                    else
                        s = s + "," + QString::number(*it);
                    cnt++;
                }
            ui->le_NBTI_Drain_4->setText(s);

            s = "";
            cnt = 0;
            for(it=t->gate_pins.begin(); it<t->gate_pins.end(); it++){
                    if(cnt == 0)
                        s = s + QString::number(*it);
                    else
                        s = s + "," + QString::number(*it);
                    cnt++;
                }
            ui->le_NBTI_Gate_4->setText(s);

            s = "";
            cnt = 0;
            for(it=t->sb_pins.begin(); it<t->sb_pins.end(); it++){
                    if(cnt == 0)
                        s = s + QString::number(*it);
                    else
                        s = s + "," + QString::number(*it);
                    cnt++;
                }
            ui->le_NBTI_SourceBody_4->setText(s);break;


        case 5:
            if(t->TypeCh == NCHANN)
                ui->rbtn_NBTI_Nch_5->setChecked(true);
            else
                ui->rbtn_NBTI_Pch_5->setChecked(true);

            ui->le_NBTI_Length_5->setText(QString("%1").arg(t->L));
            ui->le_NBTI_Width_5->setText(QString("%1").arg(t->W));
            ui->le_NBTI_Vgstart_5->setText(QString("%1").arg(t->Vgstart));
            ui->le_NBTI_Vgstop_5->setText(QString("%1").arg(t->Vgstop));
            ui->le_NBTI_Idmax_5->setText(QString("%1").arg(t->Idmax));
            ui->le_NBTI_Ith_5->setText(QString("%1").arg(t->Ith_sq));
            ui->le_NBTI_Vdlin_5->setText(QString("%1").arg(t->Vtlin));
            ui->le_NBTI_Vdsat_5->setText(QString("%1").arg(t->Vtsat));

            s = "";
            cnt = 0;
            for(it=t->drain_pins.begin(); it<t->drain_pins.end(); it++){
                    if(cnt == 0)
                        s = s + QString::number(*it);
                    else
                        s = s + "," + QString::number(*it);
                    cnt++;
                }
            ui->le_NBTI_Drain_5->setText(s);

            s = "";
            cnt = 0;
            for(it=t->gate_pins.begin(); it<t->gate_pins.end(); it++){
                    if(cnt == 0)
                        s = s + QString::number(*it);
                    else
                        s = s + "," + QString::number(*it);
                    cnt++;
                }
            ui->le_NBTI_Gate_5->setText(s);

            s = "";
            cnt = 0;
            for(it=t->sb_pins.begin(); it<t->sb_pins.end(); it++){
                    if(cnt == 0)
                        s = s + QString::number(*it);
                    else
                        s = s + "," + QString::number(*it);
                    cnt++;
                }
            ui->le_NBTI_SourceBody_5->setText(s);break;

        default: Result = false; break;
    };*/

    return Result;
}
/*-----------------------------------------------------------------------------------------------*/
