#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "eq_control.h"
#include "XML.h"

#include <QThread>

#define TIMER_STEP  500

bool abort_meas;
ViSession Test::vi_b1500 = VI_NULL;
ViSession Test::vi_2200a = VI_NULL;
/* ----------------------------------------------------------------------------------------------- */
MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int N;

    ColorCnt = 0;
    abort_meas = false;

    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->plotWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
    connect(ui->plotWidget->xAxis, SIGNAL(rangeChanged(QCPRange,QCPRange)), this, SLOT(xAxisRangeChanged(QCPRange,QCPRange)));
    connect(ui->plotWidget->yAxis, SIGNAL(rangeChanged(QCPRange,QCPRange)), this, SLOT(yAxisRangeChanged(QCPRange,QCPRange)));
    connect(ui->plotWidget, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel(QWheelEvent*)));

    ui->tab_TestTypes->setCurrentIndex(0);
    ui->tab_TDDB_Devices->setCurrentIndex(0);
    ui->tab_HC_Devices->setCurrentIndex(0);
    ui->tab_NBTI_Devices->setCurrentIndex(0);

    ui->cb_HC_StressTime->setCurrentIndex(10);
    ui->cb_NBTI_StressTime->setCurrentIndex(12);

    ui->rbtn_EM_Al->setChecked(true);
    ui->cb_EM_TestType->setCurrentIndex(0);
    ui->l_EM_Istress->setVisible(false);
    ui->le_EM_Istress->setVisible(false);
    ui->l_EM_Ea->setVisible(false);
    ui->le_EM_Ea->setVisible(false);
    ui->l_EM_n->setVisible(false);
    ui->le_EM_n->setVisible(false);
    ui->l_EM_A->setVisible(false);
    ui->le_EM_A->setVisible(false);
    ui->chb_EM_Ext->setChecked(false);
    ui->l_EM_Iuse->setVisible(false);
    ui->le_EM_Iuse->setVisible(false);
    ui->l_EM_PulseW->setVisible(false);
    ui->le_EM_PulseW->setVisible(false);

    ui->rbtn_VRDB_Ramp->setChecked(true);

    if(b1500_FindSMU(smu)){
        N = smu.size();

        for(auto it:smu){
            ui->cb_EM_Force1->addItem(it.Name);
            ui->cb_VRDB_LowSMU->addItem(it.Name);
            if(it.Num != GNDU){
                ui->cb_EM_Force2->addItem(it.Name);
                ui->cb_EM_Sense1->addItem(it.Name);
                ui->cb_EM_Sense2->addItem(it.Name);
                ui->cb_VRDB_HighSMU->addItem(it.Name);
            }
        }

        if(N>4){
            ui->cb_EM_Force1->setCurrentIndex(0);
            ui->cb_EM_Force2->setCurrentIndex(1);
            ui->cb_EM_Sense1->setCurrentIndex(2);
            ui->cb_EM_Sense2->setCurrentIndex(3);
            ui->cb_EM_Ext->setCurrentIndex(4);
        }

        if(N>2){
            ui->cb_VRDB_HighSMU->setCurrentIndex(0);
            ui->cb_VRDB_LowSMU->setCurrentIndex(1);
        }
    }


    ui->leMinX->setValidator(new QDoubleValidator(1E-30, 1E+30, 5, this));
    ui->leMaxX->setValidator(new QDoubleValidator(1E-30, 1E+30, 5, this));
    ui->leMinY->setValidator(new QDoubleValidator(-28, 6.0, 5, this));
    ui->leMaxY->setValidator(new QDoubleValidator(-28, 6.0, 5, this));

    SetPlotArea(ui->plotWidget);
    SetPlotAreaWeibull(ui->plotWidget);
}
/* -----------------------------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
    emit close_connection();
    delete ui;
}
/*------------------------------------------------------------------------------------------------*/
void MainWindow::SetTestTime(int TestTime)
{
    int Hours, Mins, Secs;
    int TimeSec;
    char str[32];

    TimeSec = TestTime;

    Hours = int(TimeSec/3600.0);
    Mins = int((TimeSec - Hours*3600)/60.0);
    Secs = int(TimeSec - Hours*3600 - Mins*60);

    sprintf(str, "%02d:%02d:%02d", Hours, Mins, Secs);

    ui->le_TestTime->setText(str);
}
/*------------------------------------------------------------------------------------------------*/
void MainWindow::SetReady()
{
    ui->lMeasStatus->setText("Ready");
    SetTestTime(0);
    abort_meas = false;
}
/*------------------------------------------------------------------------------------------------*/
bool MainWindow::GetCommonHCData(HC *t)
{
    bool Ok;

    if(smu.size()<4){
        QMessageBox::critical(this, "Error", QString("Sources setting error in Agilent B1500."),
                                              QMessageBox::Ok);
        return false;
    }

    t->Body = smu.at(0);
    t->Source = smu.at(1);
    t->Drain = smu.at(2);
    t->Gate = smu.at(3);

    t->TestType = HCI;
    t->bUseProber = ui->chb_HC_UseProber->isChecked();

    if(ui->rbtn_HC_Nch->isChecked())
        t->TypeCh = NCHANN;
    else
        t->TypeCh = PCHANN;

    t->T = (ui->le_HC_Tstress->text()).toDouble(&Ok);
    if(!Ok || t->T>MAX_TEMP || t->T<MIN_TEMP){
        QMessageBox::critical(this, "Error", QString("Stress temperature value is not correct for HC. Please, enter value from %1 to %2")
                              .arg(MIN_TEMP).arg(MAX_TEMP), QMessageBox::Ok);
        return false;
    }

    t->TimeLimit = (ui->cb_HC_StressTime->currentText()).toDouble(&Ok);
    if(!Ok || t->TimeLimit>MAX_TIME || t->TimeLimit<MIN_TIME){
        QMessageBox::critical(this, "Error", QString("Time limit is not correct for HC. Please, enter value from %1 to %2")
                             .arg(MIN_TIME).arg(MAX_TIME), QMessageBox::Ok);
        return false;
    }

    t->Vdstress = (ui->le_HC_Vdstress->text()).toDouble(&Ok);
    if(!Ok || t->Vdstress>MAX_V || t->Vdstress<0.0){
        QMessageBox::critical(this, "Error", QString("Drain stress voltage value is not correct for HC. Please, enter value from %1 to %2")
                              .arg(0).arg(MAX_V), QMessageBox::Ok);
        return false;
    }

    t->Vgstress = (ui->le_HC_Vgstress->text()).toDouble(&Ok);
    if(!Ok || t->Vgstress>MAX_V || t->Vgstress<0.0){
        QMessageBox::critical(this, "Error", QString("Gate stress voltage value is not correct for HC. Please, enter value from %1 to %2")
                              .arg(0).arg(MAX_V), QMessageBox::Ok);
        return false;
    }

    t->Vsstress = (ui->le_HC_Vsstress->text()).toDouble(&Ok);
    if(!Ok || t->Vsstress>MAX_V || t->Vsstress<0.0){
        QMessageBox::critical(this, "Error", QString("Source stress voltage value is not correct for HC. Please, enter value from %1 to %2")
                              .arg(0).arg(MAX_V), QMessageBox::Ok);
        return false;
    }

    t->Vbstress = (ui->le_HC_Vbstress->text()).toDouble(&Ok);
    if(!Ok || t->Vbstress>MAX_V || t->Vbstress<0.0){
        QMessageBox::critical(this, "Error", QString("Body stress voltage value is not correct for HC. Please, enter value from %1 to %2")
                              .arg(0).arg(MAX_V), QMessageBox::Ok);
        return false;
    }

    if(t->TypeCh == PCHANN){
        t->Vdstress = -1*t->Vdstress;
        t->Vgstress = -1*t->Vgstress;
        t->Vsstress = -1*t->Vsstress;
        t->Vbstress = -1*t->Vbstress;
    }

    if(!t->CheckSrc()){
        QMessageBox::critical(this, "Error", QString("Sources setting error in Agilent B1500."),
                                              QMessageBox::Ok);
        return false;
    }

    return true;
}
/*------------------------------------------------------------------------------------------------*/
bool MainWindow::GetCommonTDDBData(GOI *t)
{
    bool Ok;

    if(smu.size()<2){
        QMessageBox::critical(this, "Error", QString("Sources setting error in Agilent B1500."),
                                              QMessageBox::Ok);
        return false;
    }

    t->TestType = TDDB;
    t->bUseProber = ui->chb_TDDB_UseProber->isChecked();


    t->T = (ui->le_TDDB_T->text()).toDouble(&Ok);
    if(!Ok || t->T>MAX_TEMP || t->T<MIN_TEMP){
        QMessageBox::critical(this, "Error", QString("Stress temperature value is not correct for TDDB. Please, enter value from %1 to %2")
                              .arg(MIN_TEMP).arg(MAX_TEMP), QMessageBox::Ok);
        return false;
    }

    t->TimeLimit = (ui->le_TDDB_TimeLimit->text()).toDouble(&Ok);
    if(!Ok || t->TimeLimit>MAX_TIME || t->TimeLimit<MIN_TIME){
        QMessageBox::critical(this, "Error", QString("Time limit is not correct for TDDB. Please, enter value from %1 to %2")
                             .arg(MIN_TIME).arg(MAX_TIME), QMessageBox::Ok);
        return false;
    }

    if(!t->CheckSrc()){
        QMessageBox::critical(this, "Error", QString("Sources setting error in Agilent B1500."),
                                              QMessageBox::Ok);
        return false;
    }

    return true;
}
/*------------------------------------------------------------------------------------------------*/
void MainWindow::ClearParams(const QWidget *w)
{
    QList<QLineEdit *> le = w->findChildren<QLineEdit*>();
    for(int i=0; i<le.size(); i++)
        le[i]->clear();
}
/*------------------------------------------------------------------------------------------------*/
void MainWindow::IsertTDDBData(const double &value, const int &row, const int &column)
{
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(QString("%1").arg(value));
    ui->table_TDDB_data->setItem(row, column, item);
}
/*------------------------------------------------------------------------------------------------*/
void MainWindow::InsertHCData(const short &n,
                              const double &Irfs,
                              const double &Idfs,
                              const double &Idfl,
                              const double &Vtlin,
                              const double &Vtsat,
                              const double &Vt_Gm,
                              const double &Gm)
{   
    const uint N = 7;
    QTableWidgetItem **item = new QTableWidgetItem*[N];

    item[0]->setText(QString("%1").arg(Irfs*1E+6));
    ui->table_HC_data->setItem(n, 0, item[0]);

    item[1]->setText(QString("%1").arg(Idfs*1E+6));
    ui->table_HC_data->setItem(n, 1, item[1]);

    item[2]->setText(QString("%1").arg(Idfl*1E+6));
    ui->table_HC_data->setItem(n, 2, item[2]);

    item[3]->setText(QString("%1").arg(Vtlin));
    ui->table_HC_data->setItem(n, 3, item[3]);

    item[4]->setText(QString("%1").arg(Vtsat));
    ui->table_HC_data->setItem(n, 4, item[4]);

    item[5]->setText(QString("%1").arg(Vt_Gm));
    ui->table_HC_data->setItem(n, 5, item[5]);

    item[6]->setText(QString("%1").arg(Gm*1E+6));
    ui->table_HC_data->setItem(n, 6, item[6]);
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::InsertNBTIData(const short &n, double &Ifly)
{
    QTableWidgetItem    *item = new QTableWidgetItem();
    item->setText(QString("%1").arg(Ifly*1E+6));
    ui->table_NBTI_data->setItem(n, 0, item);
}
/*-----------------------------------------------------------------------------------------------*/
//испытания транзисторов на стойкость к горячим носителям
void MainWindow::on_btnMeasHC_clicked()
{
    abort_meas = false;

    HC *hc = new HC();
    Transistor mos;

    if(!GetCommonHCData(hc)){
        delete hc;
        return;
    }

    QList<QWidget*> w = ui->tab_HC_Devices->findChildren<QWidget*>();

    //проверка правильности введенных данных
    for(int i=0; i<w.size(); i++){
        if(HCReadData(hc, &mos, w[i], i+1))
            hc->MOS.push_back(mos);
    }

    if(hc->MOS.size()>0){
        for(auto it:hc->MOS){
            it.TypeCh = hc->TypeCh;
            it.drain = hc->Drain;
            it.gate = hc->Gate;
            it.source = hc->Source;
            it.body = hc->Body;
            if(it.TypeCh==PCHANN){
                it.Vgstart *= -1;
                it.Vgstop *= -1;
                it.Vdlin *= -1;
                it.Vdsat *= -1;
                it.Idmax *= -1;
                it.Ith *= -1;
                it.Ith_sq *= -1;
            }
        }
    }
    else
        return;

    QThread *thread = new QThread;
    hc->moveToThread(thread);

    connect(hc, SIGNAL(finished_test()), thread, SLOT(quit()));
    connect(thread, SIGNAL(started()), hc, SLOT(Run()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(hc, SIGNAL(finished_test()), hc, SLOT(HCSaveData()));
    connect(thread, SIGNAL(finished()), this, SLOT(SetReady()));
    connect(hc, SIGNAL(change_test_status(QString)), this, SLOT(SetCurrentTestStatus(QString)));
    connect(hc, SIGNAL(change_HC_item(short,double,double,double,double,double,double,double)),
                    this, SLOT(InsertHCData(short,double,double,double,double,double,double,double)));
    connect(hc, SIGNAL(change_test_time(int)), this, SLOT(SetTestTime(int)));


    thread->start();
    thread->setPriority(QThread::HighPriority);
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_btnMeasEM_clicked()
{
    abort_meas = false;

    EM *em = new EM();
    QThread *thread = new QThread;

    em->moveToThread(thread);

    connect(em, SIGNAL(finished_test()), thread, SLOT(quit()));

    connect(thread, SIGNAL(started()), em, SLOT(Run()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), this, SLOT(SetReady()));
    connect(em, SIGNAL(change_test_status(QString)), this, SLOT(SetCurrentTestStatus(QString)));

    connect(em, SIGNAL(change_EMRvalue(QString)), ui->le_EM_Rmeas, SLOT(setText(QString)));
    connect(em, SIGNAL(change_EMIvalue(QString)), ui->le_EM_Imeas, SLOT(setText(QString)));
    connect(em, SIGNAL(change_test_time(int)), this, SLOT(SetTestTime(int)));

    //проверка правильности коммутации
    if(!EMReadAndCheckConnections(em))
            return;

    //проверка правильности введенных данных
    if(!EMReadAndCheckParamValues(em))
        return;

    thread->start();
    thread->setPriority(QThread::HighPriority);
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_btnMeasNBTI_clicked()
{
    abort_meas = false;
    NBTI_Test();
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_btnMeasVRDB_clicked()
{
    abort_meas = false;

    if(ui->rbtn_VRDB_Ramp->isChecked())
        VRDB_Ramp_Test();

    else
        if(ui->rbtn_VRDB_UPR->isChecked())
            VRDB_UPR_Test();
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_btnMeasTDDB_clicked()
{
    GOI *tddb = new GOI();
    Capacitor cap;

    if(!GetCommonTDDBData(tddb)){
        delete tddb;
        return;
    }

    QList<QWidget*> w = ui->tab_TDDB_Devices->findChildren<QWidget*>();
    for(int i=0; i<w.size(); i++){
            if(TDDBReadData(tddb, &cap, w[i], i+1))
                tddb->CAP.push_back(cap);
        }


    /*for(int i=0; i<ui->tab_TDDB_Devices->count(); i++){
        ui->tab_TDDB_Devices->setCurrentIndex(i);
        if(HCReadData(hc, &mos, ui->tab_HC_Devices->currentWidget(), i+1))
            hc->MOS.push_back(mos);
    }*/

    /*HC *hc = new HC();
    Transistor mos;

    //проверка правильности введенных данных
    if(!GetCommonHCData(hc)){
        delete hc;
        return;
    }

    for(int i=0; i<ui->tab_HC_Devices->count(); i++){
        ui->tab_HC_Devices->setCurrentIndex(i);
        if(HCReadData(hc, &mos, ui->tab_HC_Devices->currentWidget(), i+1))
            hc->MOS.push_back(mos);
    }

    if(hc->MOS.size()>0){
        for(auto it:hc->MOS){
            it.TypeCh = hc->TypeCh;
            it.drain = hc->Drain;
            it.gate = hc->Gate;
            it.source = hc->Source;
            it.body = hc->Body;
            if(it.TypeCh==PCHANN){
                it.Vgstart *= -1;
                it.Vgstop *= -1;
                it.Vdlin *= -1;
                it.Vdsat *= -1;
                it.Idmax *= -1;
                it.Ith *= -1;
                it.Ith_sq *= -1;
            }
        }
    }
    else
        return;

    QThread *thread = new QThread;
    hc->moveToThread(thread);

    connect(hc, SIGNAL(finished_test()), thread, SLOT(quit()));
    connect(thread, SIGNAL(started()), hc, SLOT(Run()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(hc, SIGNAL(finished_test()), hc, SLOT(HCSaveData()));
    connect(thread, SIGNAL(finished()), this, SLOT(SetReady()));
    connect(hc, SIGNAL(change_test_status(QString)), this, SLOT(SetCurrentTestStatus(QString)));
    connect(hc, SIGNAL(change_HC_item(short,double,double,double,double,double,double,double)),
                    this, SLOT(InsertHCData(short,double,double,double,double,double,double,double)));
    connect(hc, SIGNAL(change_test_time(int)), this, SLOT(SetTestTime(int)));


    thread->start();
    thread->setPriority(QThread::HighPriority);*/
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_btnAbortMeasure_clicked()
{
    int ret;
    ret = QMessageBox::information(this, "Abort measurement", QString("This measurement will be interrupted, are you sure?"),
                                   QMessageBox::Yes | QMessageBox::No);
    if(ret == QMessageBox::Yes)
        abort_meas = true;
}
/*-----------------------------------------------------------------------------------------------*/
//сохранение исходных EM данных в XML-файл
void MainWindow::on_btnSaveEMXML_clicked()
{
    QString FileName;
    EM *EMTest;

    //создание файла для записи данных
    FileName = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("XML Files (*.xml)"));
    if(FileName.length()<4)
        return;

    EMTest = new EM();

    EMTest->Clear();
    //проверка правильности коммутации и правильности введенных данных
    if(!EMReadAndCheckConnections(EMTest) ||  !EMReadAndCheckParamValues(EMTest)){
        delete EMTest;
        return;
    }

    SaveEMXml(FileName, EMTest);

    delete EMTest;
}
/*-----------------------------------------------------------------------------------------------*/
//чтение исходных EM данных из XML-файла
void MainWindow::on_btnReadEMXML_clicked()
{
    QString FileName;
    EM *EMTest;

    //создание файла для записи данных
    FileName = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("XML Files (*.xml)"));
    if(FileName.length()<4)
        return;

    EMTest = new EM();

    ReadEMXml(FileName, EMTest);
    EMSetConnectionsAndParams(EMTest);

    delete EMTest;
}
/*-----------------------------------------------------------------------------------------------*/
//сохранение исходных VRDB данных в XML-файл
void MainWindow::on_btnSaveVRDBXML_clicked()
{
    /*QString FileName;
    VRDB *t;

    //создание файла для записи данных
    FileName = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("XML Files (*.xml)"));
    if(FileName.length()<4)
        return;

    t = new VRDB();
    t->Clear();

    t->NumSMUHigh = SetNumOfSMU(ui->cb_VRDB_HighSMU);
    t->NumSMULow = SetNumOfSMU(ui->cb_VRDB_LowSMU);

    if(t->NumSMUHigh == t->NumSMULow){
        QMessageBox::information(this, "Info", QString("SMU number for hidh and low sources are equal!"), QMessageBox::Ok);
        return;
    }

    if(t->NumSMUHigh < 1|| t->NumSMULow < 1){
        QMessageBox::information(this, "Info", QString("SMU number for hidh and low sources are fail! Please, check them"), QMessageBox::Ok);
        return;
    }

    t->NumSlotHigh = SetSlotOfSMU(t->NumSMUHigh);
    t->NumSlotLow = SetSlotOfSMU(t->NumSMULow);

    //проверка правильности введенных данных
    if(!VRDBParamValues(t)){
        QMessageBox::critical(this, "Error", QString("Initial test data are wrong! Please, check them."), QMessageBox::Ok);
        return;
    }

    SaveVRDBXml(FileName, t, 1);

    delete t;*/
}
/*-----------------------------------------------------------------------------------------------*/
//чтение исходных VRDB данных из XML-файла
void MainWindow::on_btnReadVRDBXML_clicked()
{
    /*QString FileName;
    VRDB *t;

    //создание файла для записи данных
    FileName = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("XML Files (*.xml)"));
    if(FileName.length()<4)
        return;

    t = new VRDB();
    t->Clear();

    ReadVRDBXml(FileName, t);
    VRDBSetParams(t);

    delete t;*/
}
/*-----------------------------------------------------------------------------------------------*/
//сохранение исходных HC данных в XML-файл
void MainWindow::on_btnSaveHCXML_clicked()
{
    //создание файла для записи данных
    QString FileName = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("XML Files (*.xml)"));
    if(FileName.length()<4)
        return;

    HC *hc = new HC();
    Transistor mos;

    if(!GetCommonHCData(hc)){
        delete hc;
        return;
    }

    //проверка правильности введенных данных
    for(int i=0; i<ui->tab_HC_Devices->count(); i++){
        ui->tab_HC_Devices->setCurrentIndex(i);
        if(HCReadData(hc, &mos, ui->tab_HC_Devices->currentWidget(), i+1))
            hc->MOS.push_back(mos);
    }

    if(hc->MOS.size()>0){
        for(auto it:hc->MOS){
            it.TypeCh = hc->TypeCh;
            it.drain = hc->Drain;
            it.gate = hc->Gate;
            it.source = hc->Source;
            it.body = hc->Body;
            if(it.TypeCh==PCHANN){
                it.Vgstart *= -1;
                it.Vgstop *= -1;
                it.Vdlin *= -1;
                it.Vdsat *= -1;
                it.Idmax *= -1;
                it.Ith *= -1;
                it.Ith_sq *= -1;
            }
        }

        SaveHCXml(FileName, hc);
    }
    delete hc;
}
/*-----------------------------------------------------------------------------------------------*/
//чтение исходных HC данных из XML-файла
void MainWindow::on_btnReadHCXML_clicked()
{
    QString FileName;

    //создание файла для записи данных
    FileName = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("XML Files (*.xml)"));
    if(FileName.length()<4)
        return;

    HC *hc = new HC();
    ReadHCXml(FileName, hc);

    ui->le_HC_Tstress->setText(QString("%1").arg(hc->T));
    ui->cb_HC_StressTime->setCurrentText(QString("%1").arg(hc->TimeLimit));
    ui->le_HC_Vgstress->setText(QString("%1").arg(hc->Vgstress));
    ui->le_HC_Vdstress->setText(QString("%1").arg(hc->Vdstress));
    ui->le_HC_Vsstress->setText(QString("%1").arg(hc->Vsstress));
    ui->le_HC_Vbstress->setText(QString("%1").arg(hc->Vbstress));

    for(int i=0; i<ui->tab_HC_Devices->count(); i++){
        ui->tab_HC_Devices->setCurrentIndex(i);
        for(auto it: hc->MOS)
            if(it.Index == i+1){
                HCSetData(&it, ui->tab_HC_Devices->currentWidget());
                break;
            }
    }
}
/*-----------------------------------------------------------------------------------------------*/
//сохранение исходных TDDB данных в XML-файл
void MainWindow::on_btnSaveTDDBXML_clicked()
{
   /* QString FileName;
    TDDB tddb;
    QVector<TDDB> dev;

    //создание файла для записи данных
    FileName = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("XML Files (*.xml)"));
    if(FileName.length()<4)
        return;

    tddb.Clear();


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
                                   &tddb, 1))
        dev.push_back(tddb);

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
                                   &tddb, 2))
        dev.push_back(tddb);

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
                                   &tddb, 3))
        dev.push_back(tddb);

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
                                   &tddb, 4))
        dev.push_back(tddb);

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
                                   &tddb, 5))
        dev.push_back(tddb);

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
                                   &tddb, 6))
        dev.push_back(tddb);

    SaveTDDBXml(FileName, &dev, 1);*/
}
/*-----------------------------------------------------------------------------------------------*/
//чтение исходных TDDB данных из XML-файла
void MainWindow::on_btnReadTDDBXML_clicked()
{
    /*QString FileName;
    QVector<TDDB> dev;
    QVector<TDDB>::iterator it;

    //создание файла для записи данных
    FileName = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("XML Files (*.xml)"));
    if(FileName.length()<4)
        return;

    ReadTDDBXml(FileName, &dev);

    for(it=dev.begin(); it<dev.end(); it++)
        TDDBSetParams(it);*/
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_btnClearEM_clicked()
{
    ui->l_EM_Istress->setVisible(false);
    ui->le_EM_Istress->setVisible(false);

    ui->cb_EM_Force1->setCurrentIndex(0);
    ui->cb_EM_Force2->setCurrentIndex(6);
    ui->cb_EM_Sense1->setCurrentIndex(1);
    ui->cb_EM_Sense2->setCurrentIndex(2);
    ui->cb_EM_Ext->setCurrentIndex(3);

    ui->l_EM_Tref->setVisible(true);
    ui->le_EM_Tref->setVisible(true);
    ui->l_EM_TCR->setVisible(true);
    ui->le_EM_TCR->setVisible(true);
    ui->l_EM_Tchuck->setVisible(true);
    ui->le_EM_Tchuck->setVisible(true);
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_btnClearVRDB_clicked()
{
    ui->cb_VRDB_HighSMU->setCurrentIndex(1);
    ui->cb_VRDB_LowSMU->setCurrentIndex(2);
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_btnExit_clicked()
{
    MainWindow::close();
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_btnSaveNBTIXML_clicked()
{
    /*QString FileName;
    NBTI nbti;
    QVector<NBTI> dev;

    //создание файла для записи данных
    FileName = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("XML Files (*.xml)"));
    if(FileName.length()<4)
        return;

    nbti.Clear();

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
                                    &nbti,
                                    1)){
        if(NBTIReadAndCheckConnections(&nbti,
                                       ui->le_NBTI_Drain_1,
                                       ui->le_NBTI_Gate_1,
                                       ui->le_NBTI_SourceBody_1,
                                       SMU2, 1))
            dev.push_back(nbti);
        nbti.Clear();
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
                                    &nbti,
                                    2)){
        if(NBTIReadAndCheckConnections(&nbti,
                                       ui->le_NBTI_Drain_2,
                                       ui->le_NBTI_Gate_2,
                                       ui->le_NBTI_SourceBody_2,
                                       SMU3, 2))
            dev.push_back(nbti);
        nbti.Clear();
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
                                    &nbti,
                                    3)){
        if(NBTIReadAndCheckConnections(&nbti,
                                       ui->le_NBTI_Drain_3,
                                       ui->le_NBTI_Gate_3,
                                       ui->le_NBTI_SourceBody_3,
                                       SMU4, 3))
            dev.push_back(nbti);
        nbti.Clear();
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
                                    &nbti,
                                    4)){
        if(NBTIReadAndCheckConnections(&nbti,
                                       ui->le_NBTI_Drain_4,
                                       ui->le_NBTI_Gate_4,
                                       ui->le_NBTI_SourceBody_4,
                                       SMU5, 4))
            dev.push_back(nbti);
        nbti.Clear();
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
                                    &nbti,
                                    5)){
        if(NBTIReadAndCheckConnections(&nbti,
                                       ui->le_NBTI_Drain_5,
                                       ui->le_NBTI_Gate_5,
                                       ui->le_NBTI_SourceBody_5,
                                       SMU6, 5))
            dev.push_back(nbti);
        nbti.Clear();
    }

    SaveNBTIXml(FileName, &dev, 1);*/
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_btnReadNBTIXML_clicked()
{
    /*QString FileName;
    QVector<NBTI> dev;
    QVector<NBTI>::iterator it;

    //создание файла для записи данных
    FileName = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("XML Files (*.xml)"));
    if(FileName.length()<4)
        return;

    ReadNBTIXml(FileName, &dev);

    for(it=dev.begin(); it<dev.end(); it++)
        NBTISetParams(it);*/
}
/*-----------------------------------------------------------------------------------------------*/
//отключение источника и поля для ввода пинов extrusion monitor
void MainWindow::on_chb_EM_Ext_stateChanged(int arg1)
{
    ui->cb_EM_Ext->setEnabled(arg1);
    ui->le_EM_Ext->setEnabled(arg1);
}
/*-----------------------------------------------------------------------------------------------*/
//отображение статуса измерений на экране
void MainWindow::SetCurrentTestStatus(const QString &status)
{
    ui->lMeasStatus->setText(status);
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::addRandomGraph()
{
    Distrib *w = new Distrib();
    QString FileName;

    FileName = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Text Files (*.txt)"));
    if(FileName.length()<4)
        return;

    if(ReadTextFile(FileName, w)){
        DistribPlot(ui->plotWidget, w);
        Plots.push_back(*w);
    }//end if(read file)
    else{
        QMessageBox::critical(0, "Error", QString("Can't read data file."), QMessageBox::Ok);
        return;
    }
}
/*-----------------------------------------------------------------------------------------------*/
//очистка области построения и всех графиков
void MainWindow::removeAllGraphs()
{
    ui->plotWidget->clearGraphs();
    ui->plotWidget->replot();
    Plots.clear();
    ColorCnt = 0;
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_pbAddPlot_clicked()
{
    addRandomGraph();
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_pbPlotClear_clicked()
{
    removeAllGraphs();
}
/*-----------------------------------------------------------------------------------------------*/
//вертикальная сетка
void MainWindow::on_chbVertGrid_clicked()
{
    ui->plotWidget->xAxis->grid()->setVisible(ui->chbVertGrid->isChecked());
    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
//доп. вертикальная сетка
void MainWindow::on_chbVertGridAdd_clicked()
{
    ui->plotWidget->xAxis->grid()->setSubGridVisible(ui->chbVertGridAdd->isChecked());
    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
//горизонтальная сетка
void MainWindow::on_chbHorGrid_clicked()
{
    ui->plotWidget->yAxis->grid()->setVisible(ui->chbHorGrid->isChecked());
    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
//доп. горизонтальная сетка
void MainWindow::on_chbHorGridAdd_clicked()
{
    ui->plotWidget->yAxis->grid()->setSubGridVisible(ui->chbHorGridAdd->isChecked());
    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
//вкл/выкл легенды
void MainWindow::on_chbLegend_clicked()
{
    ui->plotWidget->legend->setVisible(!ui->plotWidget->legend->visible());
    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
//изменение типа шрифта для названия осей
void MainWindow::on_fcbAxisNameFont_currentFontChanged(const QFont &f)
{
    QFont font = ui->plotWidget->xAxis->labelFont();

    font.setFamily(f.family());
    ui->plotWidget->xAxis->setLabelFont(font);
    ui->plotWidget->yAxis->setLabelFont(font);

    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
//изменение размера шрифта для названия осей
void MainWindow::on_sbFontSizeAxisName_valueChanged(int arg1)
{
    QFont font = ui->plotWidget->xAxis->labelFont();

    font.setPointSize(arg1);
    ui->plotWidget->xAxis->setLabelFont(font);
    ui->plotWidget->yAxis->setLabelFont(font);

    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
//изменение типа шрифта для подписей на осях
void MainWindow::on_fcbAxisLabelFont_currentFontChanged(const QFont &f)
{
    QFont font = ui->plotWidget->xAxis->tickLabelFont();

    font.setFamily(f.family());
    ui->plotWidget->xAxis->setTickLabelFont(font);
    ui->plotWidget->yAxis->setTickLabelFont(font);

    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
//изменение размера шрифта для подписей на осях
void MainWindow::on_sbFontSizeAxisLabel_valueChanged(int arg1)
{
    QFont font = ui->plotWidget->xAxis->tickLabelFont();

    font.setPointSize(arg1);
    ui->plotWidget->xAxis->setTickLabelFont(font);
    ui->plotWidget->yAxis->setTickLabelFont(font);

    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
//изменение размера шрифта для легенды
void MainWindow::on_fcbLegendFont_currentFontChanged(const QFont &f)
{
    QFont font = ui->plotWidget->legend->font();

    font.setFamily(f.family());
    ui->plotWidget->legend->setFont(font);

    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
//изменение размера шрифта для легенды
void MainWindow::on_sbFontSizeLegend_valueChanged(int arg1)
{
    QFont font = ui->plotWidget->legend->font();

    font.setPointSize(arg1);
    ui->plotWidget->legend->setFont(font);

    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_pbSetRangeY_clicked()
{
    double min, max;

    if(ui->rbWeibull->isChecked()){
        min = log(log(1/(1-ui->leMinY->text().toDouble()/100.0)));
        max = log(log(1/(1-ui->leMaxY->text().toDouble()/100.0)));
    }

    if(ui->rbLogN->isChecked()){
        min = inv_normalDF(ui->leMinY->text().toDouble()/100.0);
        max = inv_normalDF(ui->leMaxY->text().toDouble()/100.0);
    }

    ui->plotWidget->yAxis->setRange(min, max);
    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_pbSetRangeX_clicked()
{
    ui->plotWidget->xAxis->setRange(ui->leMinX->text().toDouble(), ui->leMaxX->text().toDouble());
    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_chbRegessionLine_clicked()
{
    int i,
        points,
        N = ui->plotWidget->graphCount();

    if(ui->chbRegessionLine->isChecked()){
        for(i=0; i<N; i++){
            points = ui->plotWidget->graph(i)->dataCount();
            if(points == 2){
                ui->plotWidget->graph(i)->setVisible(true);
                ui->plotWidget->graph(i)->addToLegend();
            }
        }
    }
    else{
        for(i=0; i<N; i++){
            points = ui->plotWidget->graph(i)->dataCount();
            if(points == 2){
                ui->plotWidget->graph(i)->setVisible(false);
                ui->plotWidget->graph(i)->removeFromLegend();
            }
        }
    }
    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_pbFitPlotArea_clicked()
{
    int N;
    int size;
    double minX, maxX, minY, maxY;

    N = ui->plotWidget->graphCount();

    if(N<1)
        return;


    size = ui->plotWidget->graph(0)->data()->size()-1;
    minX = ui->plotWidget->graph(0)->data()->at(0)->key;
    minY = ui->plotWidget->graph(0)->data()->at(0)->value;
    maxX = ui->plotWidget->graph(0)->data()->at(size)->key;
    maxY = ui->plotWidget->graph(0)->data()->at(size)->value;

    for(int i=1; i<N; i++){
        size = ui->plotWidget->graph(i)->data()->size()-1;

        if(minX > ui->plotWidget->graph(i)->data()->at(0)->key)
            minX = ui->plotWidget->graph(i)->data()->at(0)->key;

        if(minY > ui->plotWidget->graph(i)->data()->at(0)->value)
            minY = ui->plotWidget->graph(i)->data()->at(0)->value;

        if(maxX < ui->plotWidget->graph(i)->data()->at(size)->key)
            maxX = ui->plotWidget->graph(i)->data()->at(size)->key;

        if(maxY < ui->plotWidget->graph(i)->data()->at(size)->value)
            maxY = ui->plotWidget->graph(i)->data()->at(size)->value;
    }


    minY<0 ? minY=minY*SCALE_EXT : minY=minY/SCALE_EXT;
    maxY<0 ? maxY=maxY/SCALE_EXT : maxY=maxY*SCALE_EXT;

    minX<0 ? minX=minX*SCALE_EXT : minX=minX/SCALE_EXT;
    maxX<0 ? maxX=maxX/SCALE_EXT : maxX=maxX*SCALE_EXT;

    ui->plotWidget->yAxis->setRange(minY, maxY);
    ui->plotWidget->xAxis->setRange(minX, maxX);
    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
//толщина линии регрессии
void MainWindow::on_sbRegressLineW_valueChanged(int arg1)
{
    QPen graphPen;
    int n;

    for(int i=0; i<Plots.size(); i++){
        n = Plots.at(i).RegrNumber;
        graphPen = ui->plotWidget->graph(n)->pen();
        graphPen.setWidth(arg1);
        ui->plotWidget->graph(n)->setPen(graphPen);
    }

    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
//толщина линии графика
void MainWindow::on_sbPlotLineW_valueChanged(int arg1)
{
    QPen graphPen;
    int n;
    
    for(int i=0; i<Plots.size(); i++){
        n = Plots.at(i).Number;
        graphPen = ui->plotWidget->graph(n)->pen();
        graphPen.setWidth(arg1);
        ui->plotWidget->graph(n)->setPen(graphPen);
    }

    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
//изменение размера точек на графике
void MainWindow::on_sbPointSize_valueChanged(double arg1)
{
    QCPScatterStyle style;

    for(int i=0; i<Plots.size(); i++){
        style = ui->plotWidget->graph(Plots.at(i).Number)->scatterStyle();
        style.setSize(arg1);
        ui->plotWidget->graph(Plots.at(i).Number)->setScatterStyle(style);
    }

    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
//заливка точек на графике
void MainWindow::on_chbFillPoints_clicked()
{
    double size;

    if(ui->chbFillPoints->isChecked())
        for(int i=0; i<Plots.size(); i++){
            size = ui->plotWidget->graph(Plots.at(i).Number)->scatterStyle().size();
            ui->plotWidget->graph(Plots.at(i).Number)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,
                                                              Plots.at(i).color,
                                                              Plots.at(i).color,
                                                              size));
        }
    else
        for(int i=0; i<Plots.size(); i++){
            size = ui->plotWidget->graph(Plots.at(i).Number)->scatterStyle().size();
            ui->plotWidget->graph(Plots.at(i).Number)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,size));
        }
    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_rbWeibull_clicked()
{
    QVector<Distrib>::iterator it;

    SetPlotAreaWeibull(ui->plotWidget);

    for(it=Plots.begin(); it<Plots.end(); it++){
        FindWeibullParams(it);
        DistribPlot(ui->plotWidget, it);
    }
    MainWindow::on_pbFitPlotArea_clicked();
    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_rbLogN_clicked()
{
    QVector<Distrib>::iterator it;

    SetPlotAreaLogN(ui->plotWidget);

    for(it=Plots.begin(); it<Plots.end(); it++){
        FindLogNParams(it);
        DistribPlot(ui->plotWidget, it);
    }
    MainWindow::on_pbFitPlotArea_clicked();
    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_pbSavePlotImage_clicked()
{
    QString exp="";
    QString filename = QFileDialog::getSaveFileName(this, "Save as...", "Distribution", "PNG (*.png);;"
                                                     "BMP (*.bmp);;PDF ( *.pdf);;"
                                                     "JPEG (*.jpg);;SVG (*.svg)");
    exp=filename.mid(filename.lastIndexOf(".")+1);
    exp=exp.toLower();

    if(exp == "png")
        ui->plotWidget->savePng(filename);
    if(exp == "bmp")
        ui->plotWidget->saveBmp(filename);
    if(exp == "jpg")
        ui->plotWidget->saveJpg(filename);
    if(exp == "pdf")
        ui->plotWidget->savePdf(filename);
    if(exp == "svg")
        saveAsSvg(ui->plotWidget, filename);
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);

  if (ui->plotWidget->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  }
  else  // general context menu on graphs requested
  {
    menu->addAction("Add graph", this, SLOT(addRandomGraph()));
    if (ui->plotWidget->selectedGraphs().size() > 0)
      menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    if (ui->plotWidget->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
  }

  menu->popup(ui->plotWidget->mapToGlobal(pos));
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::moveLegend()
{
    // make sure this slot is really called by a context menu action, so it carries the data we need
  if (QAction* contextAction = qobject_cast<QAction*>(sender()))
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->plotWidget->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->plotWidget->replot();
    }
  }
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::removeSelectedGraph()
{
    QVector<Distrib>::iterator it;
    int i, n;
    bool bRegr = false;

    if (ui->plotWidget->selectedGraphs().size() > 0)
    {
        for(i=0; i<ui->plotWidget->graphCount(); i++)
            if(ui->plotWidget->graph(i)->selected()){
                n = i;
                if(ui->plotWidget->graph(i)->dataCount()==2)
                    bRegr = true;
                break;
            }

        if(!bRegr){
            for(i=0; i<Plots.size(); i++){
                if(Plots.at(i).Number == n)
                    Plots.remove(i);
            }

            for(it=Plots.begin(); it<Plots.end(); it++)
                if(it->Number > n){
                    it->Number = it->Number - 2;
                    it->RegrNumber = it->RegrNumber - 2;
                }

            //ui->widget->removeGraph(ui->widget->selectedGraphs().first());
            ui->plotWidget->removeGraph(ui->plotWidget->graph(n));  //удаление точек
            ui->plotWidget->removeGraph(ui->plotWidget->graph(n));  //удаление линии регрессии
            ui->plotWidget->replot();
        }

        if(ui->plotWidget->graphCount() == 0)
            ColorCnt = 0;
    }
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_cb_EM_TestType_currentIndexChanged(int index)
{
    switch(index){
    case PCONST:    ui->l_EM_Tref->setVisible(true);
                    ui->le_EM_Tref->setVisible(true);
                    ui->l_EM_TCR->setVisible(true);
                    ui->le_EM_TCR->setVisible(true);
                    ui->l_EM_Tchuck->setVisible(true);
                    ui->le_EM_Tchuck->setVisible(true);
                    ui->l_EM_Istress->setVisible(false);
                    ui->le_EM_Istress->setVisible(false);
                    ui->l_EM_Ea->setVisible(false);
                    ui->le_EM_Ea->setVisible(false);
                    ui->l_EM_n->setVisible(false);
                    ui->le_EM_n->setVisible(false);
                    ui->l_EM_A->setVisible(false);
                    ui->le_EM_A->setVisible(false);
                    ui->l_EM_Iuse->setVisible(false);
                    ui->le_EM_Iuse->setVisible(false);
                    ui->l_EM_PulseW->setVisible(false);
                    ui->le_EM_PulseW->setVisible(false);
                    break;

    case ICONST:    ui->l_EM_Tref->setVisible(false);
                    ui->le_EM_Tref->setVisible(false);
                    ui->l_EM_TCR->setVisible(false);
                    ui->le_EM_TCR->setVisible(false);
                    ui->l_EM_Tchuck->setVisible(false);
                    ui->le_EM_Tchuck->setVisible(false);
                    ui->l_EM_Ea->setVisible(false);
                    ui->le_EM_Ea->setVisible(false);
                    ui->l_EM_n->setVisible(false);
                    ui->le_EM_n->setVisible(false);
                    ui->l_EM_A->setVisible(false);
                    ui->le_EM_A->setVisible(false);
                    ui->l_EM_Istress->setVisible(true);
                    ui->le_EM_Istress->setVisible(true);
                    ui->l_EM_Iuse->setVisible(false);
                    ui->le_EM_Iuse->setVisible(false);
                    ui->l_EM_PulseW->setVisible(false);
                    ui->le_EM_PulseW->setVisible(false);
                    break;

    case SWEAT:     ui->l_EM_Tref->setVisible(true);
                    ui->le_EM_Tref->setVisible(true);
                    ui->l_EM_TCR->setVisible(true);
                    ui->le_EM_TCR->setVisible(true);
                    ui->l_EM_Tchuck->setVisible(true);
                    ui->le_EM_Tchuck->setVisible(true);
                    ui->l_EM_Ea->setVisible(true);
                    ui->le_EM_Ea->setVisible(true);
                    ui->l_EM_n->setVisible(true);
                    ui->le_EM_n->setVisible(true);
                    ui->l_EM_A->setVisible(true);
                    ui->le_EM_A->setVisible(true);
                    ui->l_EM_Istress->setVisible(false);
                    ui->le_EM_Istress->setVisible(false);
                    ui->l_EM_Iuse->setVisible(false);
                    ui->le_EM_Iuse->setVisible(false);
                    ui->l_EM_PulseW->setVisible(false);
                    ui->le_EM_PulseW->setVisible(false);
                    break;

    case IPULSE:    ui->l_EM_Tref->setVisible(false);
                    ui->le_EM_Tref->setVisible(false);
                    ui->l_EM_TCR->setVisible(false);
                    ui->le_EM_TCR->setVisible(false);
                    ui->l_EM_Tchuck->setVisible(false);
                    ui->le_EM_Tchuck->setVisible(false);
                    ui->l_EM_Ea->setVisible(false);
                    ui->le_EM_Ea->setVisible(false);
                    ui->l_EM_n->setVisible(false);
                    ui->le_EM_n->setVisible(false);
                    ui->l_EM_A->setVisible(false);
                    ui->le_EM_A->setVisible(false);
                    ui->l_EM_Istress->setVisible(true);
                    ui->le_EM_Istress->setVisible(true);
                    ui->l_EM_Iuse->setVisible(true);
                    ui->le_EM_Iuse->setVisible(true);
                    ui->l_EM_PulseW->setVisible(true);
                    ui->le_EM_PulseW->setVisible(true);
                    break;

    case ICONST_2P: ui->l_EM_Tref->setVisible(false);
                    ui->le_EM_Tref->setVisible(false);
                    ui->l_EM_TCR->setVisible(false);
                    ui->le_EM_TCR->setVisible(false);
                    ui->l_EM_Tchuck->setVisible(false);
                    ui->le_EM_Tchuck->setVisible(false);
                    ui->l_EM_Ea->setVisible(false);
                    ui->le_EM_Ea->setVisible(false);
                    ui->l_EM_n->setVisible(false);
                    ui->le_EM_n->setVisible(false);
                    ui->l_EM_A->setVisible(false);
                    ui->le_EM_A->setVisible(false);
                    ui->l_EM_Istress->setVisible(true);
                    ui->le_EM_Istress->setVisible(true);
                    ui->l_EM_Iuse->setVisible(false);
                    ui->le_EM_Iuse->setVisible(false);
                    ui->l_EM_PulseW->setVisible(false);
                    ui->le_EM_PulseW->setVisible(false);
                    break;

    case KELVIN:    ui->l_EM_Tref->setVisible(false);
                    ui->le_EM_Tref->setVisible(false);
                    ui->l_EM_TCR->setVisible(false);
                    ui->le_EM_TCR->setVisible(false);
                    ui->l_EM_Tchuck->setVisible(false);
                    ui->le_EM_Tchuck->setVisible(false);
                    ui->l_EM_Ea->setVisible(false);
                    ui->le_EM_Ea->setVisible(false);
                    ui->l_EM_n->setVisible(false);
                    ui->le_EM_n->setVisible(false);
                    ui->l_EM_A->setVisible(false);
                    ui->le_EM_A->setVisible(false);
                    ui->l_EM_Istress->setVisible(false);
                    ui->le_EM_Istress->setVisible(false);
                    ui->l_EM_Iuse->setVisible(true);
                    ui->le_EM_Iuse->setVisible(true);
                    ui->l_EM_PulseW->setVisible(false);
                    ui->le_EM_PulseW->setVisible(false);
                    break;

    case ISWEEP_2P: ui->l_EM_Tref->setVisible(false);
                    ui->le_EM_Tref->setVisible(false);
                    ui->l_EM_TCR->setVisible(false);
                    ui->le_EM_TCR->setVisible(false);
                    ui->l_EM_Tchuck->setVisible(false);
                    ui->le_EM_Tchuck->setVisible(false);
                    ui->l_EM_Ea->setVisible(false);
                    ui->le_EM_Ea->setVisible(false);
                    ui->l_EM_n->setVisible(false);
                    ui->le_EM_n->setVisible(false);
                    ui->l_EM_A->setVisible(false);
                    ui->le_EM_A->setVisible(false);
                    ui->l_EM_Istress->setVisible(true);
                    ui->le_EM_Istress->setVisible(true);
                    ui->l_EM_Iuse->setVisible(false);
                    ui->le_EM_Iuse->setVisible(false);
                    ui->l_EM_PulseW->setVisible(false);
                    ui->le_EM_PulseW->setVisible(false);
                    break;

    case RMEAS_2P:  ui->l_EM_Tref->setVisible(false);
                    ui->le_EM_Tref->setVisible(false);
                    ui->l_EM_TCR->setVisible(false);
                    ui->le_EM_TCR->setVisible(false);
                    ui->l_EM_Tchuck->setVisible(false);
                    ui->le_EM_Tchuck->setVisible(false);
                    ui->l_EM_Ea->setVisible(false);
                    ui->le_EM_Ea->setVisible(false);
                    ui->l_EM_n->setVisible(false);
                    ui->le_EM_n->setVisible(false);
                    ui->l_EM_A->setVisible(false);
                    ui->le_EM_A->setVisible(false);
                    ui->l_EM_Istress->setVisible(false);
                    ui->le_EM_Istress->setVisible(false);
                    ui->l_EM_Iuse->setVisible(true);
                    ui->le_EM_Iuse->setVisible(true);
                    ui->l_EM_PulseW->setVisible(false);
                    ui->le_EM_PulseW->setVisible(false);
                    break;

    default:    ui->l_EM_Tref->setVisible(true);
                ui->le_EM_Tref->setVisible(true);
                ui->l_EM_TCR->setVisible(true);
                ui->le_EM_TCR->setVisible(true);
                ui->l_EM_Tchuck->setVisible(true);
                ui->le_EM_Tchuck->setVisible(true);
                ui->l_EM_Istress->setVisible(false);
                ui->le_EM_Istress->setVisible(false);
                ui->l_EM_Ea->setVisible(false);
                ui->le_EM_Ea->setVisible(false);
                ui->l_EM_n->setVisible(false);
                ui->le_EM_n->setVisible(false);
                ui->l_EM_A->setVisible(false);
                ui->le_EM_A->setVisible(false);
                ui->l_EM_Iuse->setVisible(false);
                ui->le_EM_Iuse->setVisible(false);
                ui->l_EM_PulseW->setVisible(false);
                ui->le_EM_PulseW->setVisible(false);
                break;
    };
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_pbViewPlotData_clicked()
{
    ViewData *dialog = new ViewData(this);

    dialog->setValuesToTable(Plots);
    if(dialog->exec())
        return;
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_btnClearHC_clicked()
{
    ClearParams(ui->tab_HC_Devices->currentWidget());
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_btnClearTDDB_clicked()
{
    ClearParams(ui->tab_TDDB_Devices->currentWidget());
}
/*-----------------------------------------------------------------------------------------------*/

