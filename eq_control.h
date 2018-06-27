#ifndef EQ_CONTROL_H
#define EQ_CONTROL_H

#include <QMessageBox>
#include <QComboBox>
#include <QLineEdit>
#include <QVector>
#include <QThread>
#include <QString>
#include "agb1500.h"
#include "agb220xa.h"
#include "prober.h"

//Agilent B1500
ViStatus Init_B1500(ViSession &vi);
void check_err_B1500 (const ViSession &vi, ViStatus &ret);

//Agilent 2200A
ViStatus Init_2200A(ViSession &vi);
void check_err_2200A (const ViSession &vi, ViStatus &ret);
bool ConnectPorts2200AVect(ViSession &vi, ViInt32 &In, QVector<ViInt32> &Out);


bool ConnectPorts2200A(ViSession &vi, unsigned In, ViInt32 Out[]);
bool ConnectPort2200A(ViSession &vi, unsigned In, unsigned Out);
bool OpenPorts2200AVect(ViSession &vi, unsigned In,  QVector<unsigned> Out);
bool OpenPorts2200A(ViSession &vi, unsigned In, unsigned Out[]);
bool OpenPort2200A(ViSession &vi, unsigned In, unsigned Out);
void OpenCommMatrix(ViSession &vi);
unsigned InOut2CommProbeCard(unsigned in, unsigned out);
bool CheckConfiguration(ViSession &vi);


//Cascade prober
bool Init_Cascade12k (ViSession &vi);
void check_err_12K (const ViSession &vi, ViStatus &ret);

//common
void CloseConnection(ViSession &b1500, ViSession &mtx);
void CloseCascadeConnection(ViSession &vi_12k);


bool SetPins(QLineEdit *le, ViInt32 pins[]);
bool SetPinsVect(const QString &txt, QVector<ViInt32> &pins);
bool SetChipsVect(QLineEdit *le, QVector<unsigned> &chips);
bool SetPinsXML(QString str, QVector<ViInt32> &pins);
bool SetChipsXML(QString str, ViInt32 chips[]);


double MaxAbs(double in[], const int &size);
double Summ(QVector<double> &in, const int &cnt1, const int &cnt2);
double Summ2(QVector<double> &in, const int &cnt1, const int &cnt2);
double SummDiff(QVector<double> &in, const int &cnt1, const int &cnt2);
double Summ2Diff(QVector<double> &in, const int &cnt1, const int &cnt2);
double Average(QVector<double> &in, const int &cnt1, const int &cnt2);
bool Approx(QVector<double> &x, QVector<double> &y, const int &cnt1, const int &cnt2, double &R, double &T);

double SetVcompliance(const double &Iforce);
double SetHVcompliance(const double &Iforce);
double SetIcompliance(const double &Vforce);
double SetHIcompliance(const double &Vforce);

ViStatus VthBinary(const ViSession &vi, const ViInt32 &drain, const ViInt32 &gate, const double &Vdrain, const double &Vstart, const double &Vstop,
                   const double &Icompl, const double &Itarget, const int &Steps,  double &Vth);

ViStatus VthBinaryM(const ViSession &vi, QVector<ViInt32> &drain, QVector<ViInt32> &gate, QVector<double> &Vdrain, QVector<double> &Vstart, QVector<double> &Vstop,
                    QVector<double> &Icompl, QVector<double> &Itarget, const int &Steps,  QVector<double> &Vth);

ViStatus HighSpeedIMeas(const ViSession &vi, int channel, double Vforce, double Irange, double Icompl, double &value, double &time);
ViStatus PulseSpotMeas(const ViSession &vi, int channel, double Vpulse, double Vbase, double Irange,
                                        double Icompl, double hold, double width, double &value, int &status);

double MaxGm(double *Idrain, double Vstart, double Vstep, double Vdsat, int N, bool bRegime, double &Vt_Gmmax); //bRegime=true - linear; bRegime=false - saturation

ViStatus C_SweepMeas(const ViSession &vi, const ViInt32 &Freq, const double &OscLevel, const double &Vstart,
                     const double &Vstop, const ViInt32 &Points);


double SetFixIrange(const double &Iforce);
double SetLimIrange(const double &Iforce);
double SetLimVrange(const double &Vforce);
int SetLimVrangeGPIB(const double &Vforce);
int SetFixIrangeGPIB(const double &Iforce);
QString CurrentDate();

ViStatus GPIB_cmd(const ViSession &vi, const char* s,...);

#endif // EQ_CONTROL_H
