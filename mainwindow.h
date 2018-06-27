#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QRadioButton>
#include <QFileDialog>
#include <QTimer>
#include <QSvgGenerator>
#include "measclasses.h"
#include "viewdata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QVector<Distrib>    Plots;
    QSvgGenerator       gen;
    QVector<SMU>        smu;                //полный список источников в системе
    bool ReadTextFile(QString FileName, Distrib *w);
    void ProberMan();

public slots:
    void ClearParams(const QWidget* const w);

    void SetCurrentTestStatus(const QString &status);

    void on_btnMeasHC_clicked();

    bool EMReadAndCheckConnections(EM *t);

    bool EMReadAndCheckParamValues(EM *t);

    bool EMSetConnectionsAndParams(EM *t);

    void on_btnSaveEMXML_clicked();

    void on_btnReadEMXML_clicked();

    void on_btnMeasEM_clicked();

    void on_btnClearEM_clicked();

    void on_chb_EM_Ext_stateChanged(int arg1);

    bool HCReadData(HC *hc, Transistor *t, const QWidget* const w, const int &index);

    void HCSetData(const Transistor* const t, const QWidget* w);

    bool GetCommonHCData(HC *t);

    void on_btnSaveHCXML_clicked();

    void on_btnReadHCXML_clicked();

    bool GetCommonTDDBData(GOI *t);

    bool TDDBReadData(GOI *tddb, Capacitor *t, const QWidget* const w, const int &index);









    void TDDB_Test();

    bool TDDBSetParams(/*TDDB *t*/);

    void NBTI_Test();

    bool NBTISetParams(/*NBTI *t*/);

    void VRDB_Ramp_Test();

    void VRDB_UPR_Test();

    bool VRDBSetParams(/*VRDB *t*/);

    bool TDDBParamValues(QRadioButton *Chann,
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
                                    int index);

    bool NBTIReadAndCheckConnections(/*NBTI *t,*/
                                     QLineEdit *Drain,
                                     QLineEdit *Gate,
                                     QLineEdit *SourceBody,
                                     unsigned smu,
                                     int index);

    bool NBTIReadAndCheckParamValues(QRadioButton *Chann,
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
                                     int index);

    bool VRDBParamValues(/*VRDB *t*/);



    void on_btnMeasNBTI_clicked();

    void on_btnAbortMeasure_clicked();

    void on_btnMeasVRDB_clicked();

    void on_btnSaveVRDBXML_clicked();

    void on_btnReadVRDBXML_clicked();

    void on_btnSaveTDDBXML_clicked();

    void on_btnReadTDDBXML_clicked();

    void on_btnClearVRDB_clicked();

    void on_btnExit_clicked();

    void on_btnSaveNBTIXML_clicked();

    void on_btnReadNBTIXML_clicked();



private:
    Ui::MainWindow *ui;
    int ColorCnt;

signals:
    void close_connection();

public slots:
    void SetTestTime(int TestTime);
    void SetReady(void);

    void IsertTDDBData(const double &value, const int &row, const int &column);
    void InsertHCData(const short &n,
                      const double &Irfs,
                      const double &Idfs,
                      const double &Idfl,
                      const double &Vtlin,
                      const double &Vtsat,
                      const double &Vt_Gm,
                      const double &Gm);
    void InsertNBTIData(const short &n, double &Ifly);

    //функции для работы с графикой
    void SetPlotAreaWeibull(QCustomPlot *customplot);
    void SetPlotAreaLogN(QCustomPlot *customplot);
    void DistribPlot(QCustomPlot *customplot, Distrib *w);
    void addRandomGraph();
    void removeAllGraphs();
    void contextMenuRequest(QPoint pos);
    void moveLegend();
    void removeSelectedGraph();
    void xAxisRangeChanged(const QCPRange &newRange, const QCPRange &oldRange);
    void yAxisRangeChanged(const QCPRange &newRange, const QCPRange &oldRange);
    void mouseWheel(QWheelEvent*);

private slots:
    void on_pbAddPlot_clicked();
    void on_pbPlotClear_clicked();
    void on_chbVertGrid_clicked();
    void on_chbVertGridAdd_clicked();
    void on_chbHorGrid_clicked();
    void on_chbHorGridAdd_clicked();
    void on_chbLegend_clicked();
    void on_fcbAxisNameFont_currentFontChanged(const QFont &f);
    void on_sbFontSizeAxisName_valueChanged(int arg1);
    void on_fcbAxisLabelFont_currentFontChanged(const QFont &f);
    void on_sbFontSizeAxisLabel_valueChanged(int arg1);
    void on_fcbLegendFont_currentFontChanged(const QFont &f);
    void on_sbFontSizeLegend_valueChanged(int arg1);
    void on_pbSetRangeY_clicked();
    void on_pbSetRangeX_clicked();
    void on_chbRegessionLine_clicked();
    void on_pbFitPlotArea_clicked();
    void on_sbRegressLineW_valueChanged(int arg1);
    void on_sbPlotLineW_valueChanged(int arg1);
    void on_sbPointSize_valueChanged(double arg1);
    void on_chbFillPoints_clicked();
    void on_rbWeibull_clicked();
    void on_rbLogN_clicked();
    void on_pbSavePlotImage_clicked();
    void on_cb_EM_TestType_currentIndexChanged(int index);
    void on_pbViewPlotData_clicked();
    void on_sbSubTicksCnt_valueChanged(int arg1);
    void on_leXAxeLabel_returnPressed();
    void on_btnClearHC_clicked();
    void on_btnClearTDDB_clicked();
    void on_btnMeasTDDB_clicked();
};
#endif // MAINWINDOW_H
