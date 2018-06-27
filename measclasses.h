#ifndef MEASCLASSES_H
#define MEASCLASSES_H

#include <QObject>
#include <QString>
#include <QApplication>
#include <QMessageBox>
#include <QComboBox>
#include <QTableWidgetItem>
#include "utils.h"
#include "agb1500.h"
#include "visa.h"
#include "Prober.h"

const ViInt32 HPSMU = 1510;
const ViInt32 MPSMU = 1511;
const ViInt32 HRSMU = 1517;
const ViInt32 MCSMU = 1514;
const ViInt32 GNDSMU = 7777;

class SMU;
class Test;
class Distrib;

ViStatus b1500_FindSMU(QVector<SMU> &smu);   //определить все источники SMU
QString SetDelim(const char &symbol, const int &num);   //печатает строку длиной n из c-символов
/*=========================================================================================*/
/*=========================================================================================*/
class SMU{
public:
    SMU();

    void clear();

    ViInt32 Model;  //модель источника
    QString Name;   //название платы

    ViInt32 Num;    //номер источника
    ViInt32 Slot;   //номер слота

    ViInt32 ForceMode;   //тип источника
    ViInt32 MeasMode;   //тип измерителя
    ViReal64 Range;  //диапазон
    ViReal64 Compl;    //ограничение

    QVector<ViInt32> pins;  //пины для подключения

    bool operator== (const SMU &compareTo)
    {
        return ((Num == compareTo.Num)
                && (Slot == compareTo.Slot));
    }
};
/*=========================================================================================*/
class Transistor{
public:
    Transistor();
    void clear();
    void clear_data();

    //источники
    SMU drain,
        gate,
        source,
        body;

    QString comment;
    bool TypeCh;     //тип проводимости канала - 1=N, 0=P

    int Index;       //индекс испытуемого устройства (от 1 до 7)

    double Idmax;   //максимальный ток стока

    double  W,      //ширина канала
            L,      //длина канала
            Vdlin,  //напряжение на стоке для определения порогового напряжения в линейном режиме
            Vdsat;  //напряжение на стоке для определения порогового напряжения в режиме насыщения

    double  Vgstart,    //начальное значение напряжения на затворе для поиска порогового напряжения
            Vgstop,     //конечное значение напряжения на затворе для поиска порогового напряжения
            Ith_sq,     //уровень тока для определения порогового напряжения в А/sq
            Ith;        //уровень тока для определения порогового напряжения в А

    //измеренные данные
    QVector<double> Idfl,
                    Idfs,
                    Irfs,
                    Vlin,
                    Vsat,
                    Gm,
                    Vt_Gmmax;
};
/*=========================================================================================*/
class Capacitor{
public:
    Capacitor();
    void clear();
    void clear_data();

    //источники
    SMU     High,
            Low;

    QString comment;

    int     Index;       //индекс испытуемого устройства
    bool    TypeCh;  //тип проводимости канала - 1=N, 0=P
    int     FailureType; //тип отказа

    double  W,              //ширина
            L,              //длина
            tox,            //толщина диэлектрика
            Vsilc,          //напряжение для измерения stress induced leakage current - Isilc
            Vuse,           //рабочее напряжение
            Vstress,        //напряжения испытаний
            twait;          //время заряда емкости


    double  Iuse_init,      //характерный ток утечки при Vuse (задается)
            Istress_init,   //характерный ток утечки при Vstress (задается)
            Isilc_init;     //начальное значение Isilc

    double  Iinit,
            Iend,
            tbd_hard,
            tbd_soft,
            tbd_noise,
            Inoise_hard,
            Inoise_soft,
            Inoise_noise,
            qbd_hard,
            qbd_soft,
            qbd_noise,
            Ibd_hard,
            Ibd_soft,
            Ibd_noise,
            Isilc_hard,
            Isilc_soft,
            Isilc_noise;

    QVector<double> Istress,
                    Isilc,
                    Qbd,
                    Inoise;
};
/*=========================================================================================*/
class Test: public QObject
{
    Q_OBJECT
public:
    Test ();
    virtual ~Test();

    Prober   *prober;     //управление пробером в ходе испытаний
    static ViSession    vi_b1500;
    static ViSession   vi_2200a;

    bool    bUseMatrix; //использовать коммутационную матрицу или нет
    bool    bUseProber; //использовать зондовую станцию или нет
    bool    bNeedCal;   //необходимость калибровки перед испытаниями
    bool    b2point;    //2 point resistance measurement (все измерения сохраняются в один файл)

    int         TestType,       //1 - P=const, 2 - I=const, 3 - SWEAT, ...
                Number;         //порядковый номер испытания в списке испытаний (когда загружен файл с заданием на разные испытания)

    QString     comment;    //комментарий

    ViStatus    ret;        //возвращаемый статус измерения
    bool        SetNumSMU(SMU &smu, const QString &s);      //определить номер SMU из названия

protected:
    FILE        *file;  //файл для сохранения данных
    bool        virtual CheckSrc() = 0; //проверка правильности указанных источников
    void        virtual RunTest(const QString &Macro="Test", const int &Die=0) = 0;      //запуск определенного вида испытаний
    ViStatus    virtual SetSources(const ViInt32 &adc_type) = 0;    //установка источников

private:
    ViStatus    B1500Calibration();    //калибровка Agilent B1500
    bool        b1500_FindSlot(SMU &smu);  //определить номер слота по номеру SMU

public slots:
    void    ClearConnections();    //закрываем все подключения к приборам и закрываем файл с данными
    void    Run();  //запуск измерений

signals:
    void finished_test();               //окончание испытаний
    void change_test_status(QString);   //изменение текущего статуса испытаний
    void change_test_time(int);         //изменение длительности испытаний для отображения на экране
};
/*=========================================================================================*/
//electromigration
class EM: public Test
{
    Q_OBJECT
public:
    EM();
    SMU forceH,
        forceL,
        senseH,
        senseL,
        ExtMon;

    bool    bHPRegime;      //high power режим с подачей тока до 1А с использованием HPSMU
    bool    bMeasExt;       //измерять или нет утечки на extrusion monitor
    int     MetalType;      //материал алюминий, медь или другой материал

    //условия для измерений
    double  W,
            H;

    double  Tstress,
            TCRref,
            Tref,
            StopCond,
            Tchuck,
            TimeLimit,
            Istress,
            Ea,
            n,
            A,
            Iuse,
            PulseW;

    //значения для испытаний
    double  TCRchuck,       //TCR пересчитанный к температуре рабочего стола
            Rchuck;         //сопротивление проводника при температуре рабочего стола

    double  Itest,          //постоянный ток для испытаний
            t_stl;          //время выхода на режим испытаний

    double  Rfail,          //сопротивление при выходе проводника из строя
            TTF,            //наработка до отказа
            Iext_fail;      //ток утечки на проводники окружения при отказе

    double Rstart_stress; //сопротивление проводника в начале испытаний
    double Iext_init; //ток утечки до испытаний
    double Iext_start_stress; //ток утечки в начале испытаний

    QVector<double> T;  //вычисляемая температура
    QVector<double> P;  //вычисляемая мощность
    QVector<double> R;  //измеряемое сопротивление

    void    Clear();
    bool    CheckSrc(); //проверка правильности источников
    void    RunTest(const QString &Macro, const int &Die);  //запуск испытаний
    double  Fcorr(const double &T);         //корректировочный коэффициент
    double  Fcorr_inv(const double &T);     //инверсный корректировочный коэффициент
    void    SetVForceCompl(const double &Iforce);   //установка ограничения для выводов источника (force)
    ViStatus SetSources(const ViInt32 &adc_type);           //установка источников для испытаний
    bool CommMatrix(SMU *f1, SMU *f2, SMU *s1, SMU *s2, SMU *extmon); //коммутация матрицы

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!требует проверки
    double New_I(const double &t_ft, const double &T, const double &Rth, const double &T0, const double &I); //функция для SWEAT теста

    double Iinit(); //расчет тока для измерений

public slots:
    void Meas_Resitance(const double &Iforce);  //измерение сопротивления методом Кельвина в двух направлениях
    void Meas_2p_Resitance(const double &Iforce);   //измерение сопротивления с двумя выводами в двух направлениях
    void Meas_Extrusion_Leakage(const double &Vforce);  //измерение тока утечки на соседние проводники

    void EM_Isothermal_test();  //изотермические испытания
    void EM_Iconst_Stress();  //испытания при постоянном токе без котроля температуры для 4-х выводной структуры (нагрев внешним источником)
    void EM_SWEAT();        //электромиграционные испытания по SWEAT методике

    //                                                             _ (стресс)
    //                                                            | |
    void EM_Ipulse_Stress();     //импульсные испытания током  ___| |___ (измерение сопротивления при малом токе)
    void EM_Iconst_2p_Stress(); //испытания при постоянном токе без котроля температуры 2-х выводной структуры(нагрев внешним источником)
    void Isweep_2p();           //линейное увеличение тока до момента отказа

    void R_kelvin(const int &N, const double &Iforce);    //измерение сопротивления 4-х выводной структуры, Iforce-задаваемый ток, N-порядковый номер
    void R_2points(const int &N, const double &Iforce);   //измерение сопротивления 2-х выводной структуры, Iforce-задаваемый ток, N-порядковый номер

signals:
    void change_EMRvalue(QString);
    void change_EMIvalue(QString);
};
/*=========================================================================================*/
/*class StressTesting: public QObject
{
    Q_OBJECT
public:
    StressTesting (QWidget *parent=0);
    ~StressTesting();

    //EM              *em;
    //VRDB            *vrdb;
    Prober          prober;
    //QVector<HC>     hc;
    QVector<TDDB>   tddb;
    QVector<NBTI>   nbti;
    ViSession       vi_b1500,
                    vi_2200a;
    ViStatus        ret;
    bool    bNeedCal;       //необходимость калибровки перед испытаниями
    FILE *file;

public slots:
    void ClearConnections();

    void TDDB_Main();
    void NBTI_Main();
    void VRDB_Main();
    void UPR_Main();

    int TDDB_Ileak_Meas(bool InitOrEndMeas);
    int TDDB_Istress_Meas();
    int TDDB_Main_Parall();

    int NBTI_Parall();

    int NBTI_ParamsSpot_Classic(double Time);  //измерение необходимых параметров
    int NBTI_Stress_Classic(double Stress_time);  //стрессовое воздействие

    int VRDB_Ileak(VRDB *t, int test_type);
    int VRDB_Ramp(VRDB *t);
    int VRDB_UPR(VRDB *t);

signals:
    void change_EMRvalue(QString);
    void change_EMIvalue(QString);

    void change_TDDB_item(double, int, int);

    void change_NBTI_item(short, double);

    void finished_test();
    void interrupt();
    void change_test_status(QString);
    void change_test_time(int);
};*/
/*=========================================================================================*/
//испытания на горячие носители
//hot carriers
class HC: public Test
{
    Q_OBJECT
public:
    HC();
    void    Clear();                   //очистка введенных параметров
    bool    CheckSrc();             //проверка правильности источников
    void    RunTest(const QString &Macro, const int &Die);      //запуск испытаний
    ViStatus SetSources(const ViInt32 &adc_type);           //установка источников для испытаний

    QVector<Transistor> MOS;

    SMU     Drain,
            Source,
            Gate,
            Body;

    bool TypeCh;     //тип проводимости канала - 1=N, 0=P

    //условия для измерений
    int     TimeLimit;     //длительность испытаний
    double  T;      //температура нагрева при испытаниях

    double  Vgstress,   //напряжение при испытаниях на затворе
            Vdstress,   //напряжение при испытаниях на стоке
            Vsstress,   //напряжение при испытаниях на истоке
            Vbstress;   //напряжение при испытаниях на подложке

    QVector<double> Time;

private:
    bool CommMatrix(SMU &Drain, SMU &Gate, SMU &Source, SMU &Body);
    ViStatus HC_ParamsSpot(double TestTime);  //измерение необходимых параметров
    ViStatus HC_Stress(double Stress_time);  //стрессовое воздействие
    void HCSaveTxtData();       //сохранение данных в текстовом формате

signals:
    void change_HC_item(short,double,double,double,double,double,double,double);
};
/*=========================================================================================*/
//испытания на сдвиг порогового напряжения
//negative bias temperature instability
/*class NBTI: public Test
{
    Q_OBJECT
public:
    NBTI();
    void Clear();                   //очистка введенных параметров
    bool    CheckSrc();             //проверка правильности источников
    void    RunTest(const int &N=0);      //запуск определенного вида испытаний

    unsigned    gate_smu;               //gate SMU
    unsigned    drain_smu;              //drain SMU

    unsigned    gate;               //gate slot
    unsigned    drain;              //drain slot

    //source and body on GNDU

    int ClassicTest; //тип теста true-классический, false - on-the-fly

    int Number;
    int Index;

    //номера слотов
    QVector<unsigned>   gate_pins,
                        drain_pins,
                        sb_pins;            //source and body pins


    //условия для измерений
    bool TypeCh;                                    //тип проводимости канала - 1=N, 0=P
    double  W,
            L,
            T,
            Vtlin,
            Vtsat,
            StressTime;

    double  Vgstress,
            Vdstress,
            Idmax;

    double  Vgstart,
            Vgstop,
            Ith_sq,     //уровень тока для определения порогового напряжения в А/sq
            Ith;        //уровень тока для определения порогового напряжения в А

    //данные
    double  Ion_init,
            Ioff_init,
            Vtsat_init,
            Vtlin_init;

    double  Ion_end,
            Ioff_end,
            Vtsat_end,
            Vtlin_end;

    QVector<double> Time,
                    Ifly,

                    Idfs,
                    Idfl,
                    Vlin,
                    Vsat;
public slots:
    int NBTI_ParamsSpot(ViSession &vi_B1500, ViSession &vi_2200A, QVector<NBTI> *t, bool InitOrEndMeas);
    void NBTISaveData_Fly(QVector<NBTI> *t, FILE *file);
    void NBTISaveData_Classic(QVector<NBTI> *t, FILE *file);
};*/
/*=========================================================================================*/
//испытания подзатворного диэлектрика
//возможно независимое испытание 6 структур одновременно
//для всех испытуемых структур должны совпадать продолжительность испытаний и температура
//time dependent dielectric breakdown
class GOI: public Test
{
    Q_OBJECT
public:
    GOI();
    void        Clear();  //очистка введенных параметров
    bool        CheckSrc();
    void        RunTest(const QString &Macro, const int &Die);
    ViStatus    SetSources(const ViInt32 &adc_type);

    QVector<Capacitor> CAP;

    ViInt32     Adc_type;

    int         Number;         //порядковый номер испытания
    double      T,              //температура
                TimeLimit;      //длительность испытаний

    QVector<double> tstress;

private:
    bool CommMatrix();
    ViStatus TDDB_Ileak_Meas(const bool &bInitOrEnd);
    ViStatus TDDB_Istress_Meas();
    ViStatus TDDB_Main_Parall();
    void DetectTDDBFailureType();
    void TDDBSaveHeaderTxt();
    void TDDBSaveData();

signals:
    void change_TDDB_item(double,int,int);
};
/*=========================================================================================*/
//испытания подзатворного диэлектрика
//voltage ramp dielectric breakdown
class VRDB: public Test
{
    Q_OBJECT
public:
    VRDB();
    void Clear();                   //очистка введенных параметров
    bool        CheckSrc();
    void        RunTest(const QString &Macro, const int &Die);
    ViStatus    SetSources(const ViInt32 &adc_type);

    QVector<Capacitor> CAP;

    ViInt32     Adc_type;

    int         Number;         //порядковый номер испытания
    double      T,              //температура
                TimeLimit;      //длительность испытаний

    QVector<double> tstress;


          /*  Vuse,           //рабочее напряжение
            Vstep,          //шаг увеличения напряжения
            Vstart,         //начальное напряжение
            Rate,           //скорость увеличения напряжения [MV/(cm*sec)]
            Iuse,           //типичное значение тока утечки при Vuse
            Vstop,          //напряжение остановки
            Istop,          //значение тока для остановки испытаний
            Imax,           //максимально допустимый ток
            Slope;          //критерий определения пробоя*/

   /* double  Vg0,            //напряжение на затворе для первоначальной проверки целостности
            VlowE,          //напряжение на затворе для проверки при низкой напряженности поля
            period,         //период измерений
            t_hold,         //время удержания напряжения до начала испытаний [sec]
            t_delay,        //время задержки до начала измерений на каждом шаге [sec]
            t_sdelay;       //tstep = t_delay + t_sdelay*/

    int     points;         //количество шагов

    double  Ig0,            //ток при Vg0
            IlowE,          //ток при VlowE
            Iuse_before,    //ток при Vuse до испытаний
            Iuse_after;     //ток при Vuse после испытаний

    double  Ibd,
            Qbd,            //in C/cm2
            qbd,            //in C
            Vbd,
            tbd,
            Isilcbd;

    double  Ibd_slope,
            Qbd_slope,            //in C/cm2
            qbd_slope,            //in C
            Vbd_slope,
            tbd_slope,
            Isilcbd_slope;

    QVector<double> Vstress,
                    Istress,
                    Isilc,
                    qstress;

//public slots:
    //void DetectVRDBFailureType(VRDB *t);
    //void VRDBSaveHeader(VRDB *t, FILE *file);
    //void VRDBSaveData(VRDB *t, FILE *file, int TestType);
};
/*=========================================================================================*/
//thermal bias stress
class TBS
{
public:
    TBS();
    void Clear();                   //очистка введенных параметров

    unsigned   HighPin;
    QVector<unsigned> LowPin;

    unsigned    NumSMUHigh,
                NumSlotHigh,
                NumSMULow,
                NumSlotLow;

    bool    TypeCh;         //тип проводимости канала - 1=N, 0=P

    double  W,              //ширина
            L,              //длина
            Tstart,         //начальная температура
            Tend,           //конечная температура
            tox,            //толщина диэлектрика
            Eox,            //напряженность электрического поля в диэлектрике
            tstress,        //длительность удержания напряжения
            Vuse;           //рабочее напряжение

    double  Vg,
            Cox;

    double  Vt_pos,
            Vt_neg;
};

int TBS_CapacitanceMeas(ViSession &vi_B1500, ViSession &vi_2200A, TBS *t);
/*=========================================================================================*/

double StFnc(int i, int N);
bool FindWeibullParams(Distrib *w);
bool FindLogNParams(Distrib *w);
void SetPlotArea(QCustomPlot *widget);

#endif // MEASCLASSES_H

