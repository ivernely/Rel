#ifndef UTILS_H
#define UTILS_H

#include "qcustomplot.h"

#define ENTRY extern
#define LOCAL static

#define GPIB_CHANNEL                0
#define B1500_GPIB_ADDRESS          17  //3-"red" system, 17-"blue" system
#define A2200_GPIB_ADDRESS          22  //4-"red" system, 22-"blue" system
#define SUMMIT12K_GPIB_ADDRESS      28

#define INPUT_BUF_SIZE                  256
#define NUM_CHAN_IN_MTX_CARD            12
#define NUM_MTX_CARDS                   3
#define NUM_MTX_INPUTS					14

#define NUM_CARD_PINS                   36
#define MAX_CHIP_NUM                    64

#define BUF_MODULES 128

#define AGILENT_DELAY_CMD               90000 //за меньшее время калибровка может не пройти

const QString Path_to_save_data= "C:\\Users\\dt1\\Desktop\\Измерения\\";

//out's numbers in Agilent 2200A
#define SMU1	1
#define SMU2	2
#define SMU3	3
#define SMU4	4
#define SMU5	5
#define SMU6	6
#define CMUH    13
#define CMUL    14
#define GNDU	8

//slot's numbers in Agilent B1500
#define SLOT_CMU	-1//10
#define SLOT_GNDU	11

//voltage/current values
#define MAX_V	100      						//max voltage
#define MIN_V	-100                            //min voltage

#define MIN_VSTEP   0.001	      				//mim step voltage

#define MAX_HV	200      						//max high power voltage
#define MIN_HV	-200                            //min high power voltage

#define MAX_I	0.1								//max current (A)
#define MIN_I	-0.1							//min current (A)

#define MAX_HI	1								//max high-power current
#define MIN_HI	-1  							//min high-power current

#define MAX_PWR     2
#define MAX_HPWR    10

#define MIN_PERIOD  0.002   //ms

#define ON      1
#define OFF     0


//EM parameters
enum METAL_TYPE{
    ALUMINUM = 0,
    COPPER,
    OTHER
};

//EM test types
enum TEST_TYPE{
    PCONST = 0,
    ICONST,
    SWEAT,
    IPULSE,
    ICONST_2P,
    KELVIN,
    ISWEEP_2P,
    RMEAS_2P,
    HCI,
    NBTI,
    TDDB,
    VRDB,
    UPR
};

#define MIN_WIDTH       1E-3    //=1nm
#define MAX_WIDTH       1E+5    //=10cm

#define MIN_HEIGHT      1E-3    //=1nm
#define MAX_HEIGHT      1E+3    //=1mm

#define MIN_TEMP        -65
#define MAX_TEMP        2000

#define MIN_STOP_COND   1E-3
#define MAX_STOP_COND   100

#define MAX_R           1E+6

#define MIN_TCR         1E-6
#define MAX_TCR         1

#define MIN_PULSE_W     0.5
#define MAX_PULSE_W     2000

#define MIN_TIME        1.0                //1 second
#define MAX_TIME        100000.0           //27.7 hours in seconds
#define EM_TIME_STEP    100                 //msec

#define MIN_RES         0.1
#define MIN_IFORCE      1E-9

#define MIN_I_LEAK      1E-9            //minimum leakage current for extrusion monitor

#define EM_SWEEP_POINTS     1001            //points for Isweep test

//Measurement parameters
#define VLEAK           3.0             //voltage for leakage measuring
#define STEPS           5               //number of steps for approximation
#define RAMP_STEPS      100             //number of steps for temperature ramp

#define INTERVAL        200             //in ms for measuring resistance
#define INIT_CURR       1000            //[uA/um2]         //in JEDEC 10uA/um2
#define END_CURR        1000            //[uA/um2]         //in JEDEC 10uA/um2

#define F_CURRENT       1.2             //coefficient for raise current
#define F_POWER         2               //coefficient for power calculation

#define T_STEP          10               //step of ramping temperature
#define BE              1               //error band

#define ICOMPL_CARD     0.25            //current compliance for probecard (A)

#define NCHANN          1
#define PCHANN          0

//HC
#define HC_INTERVAL             10.0
#define HC_LINEAR_REGIME        1
#define HC_SATURATION_REGIME    0

//NBTI
#define NUM_HC_TIMES        41
#define NBTI_SAMPLES        7
#define NBTI_INTERVAL       2.0E-4
#define SAMPLES_PER_CHANN   5000        //number of measurements per one channel
#define TIME_ADJUST         2.0        //time delay before meas NBTI effect
#define NEGATIVE_INERVAL    -1.5E-4  //-2.1E-4 //-100 us for NBTI measurement

//TDDB
#define MIN_TOX                 0.1            //1 angstrem
#define MAX_TOX                 1E+6           //1mm

#define TDDB_MIN_STEPTIME       0.1     //=100ms
#define TDDB_MIN_INTERVAL       0.002   //2ms

#define INIT_BREAKDOWN          10
#define HARD_BREAKDOWN          10
#define SOFT_BREAKDOWN          5
#define NOISE_BREAKDOWN         500
#define MIN_CURRENT_FOR_ANALYSE 1E-11

#define TDDB_NUM_STEPS          1001

#define TDDB_INIT_FAILURE       1
#define TDDB_YIELD_FAILURE      2
#define TDDB_SOFT_FAILURE       4
#define TDDB_NOISE_FAILURE      8
#define TDDB_HARD_FAILURE       16
#define TDDB_CATASTROPH_FAILURE 32

//VRDB
//#define VRDB_J_INIT             10      //uA/cm2
#define VRDB_RAMP_STEP          0.01    //[V/nm]                  =0.1 MV/cm
#define VRDB_RAMP_RATE          0.1     //[V/(nm*sec)]            =1 MV/(cm*sec)
#define VRDB_MAX_FIELD_THIN     30      //MV/cm
#define VRDB_MAX_FIELD_THICK    15      //MV/cm

#define VRDB_MIN_STEPTIME       0.01    //s =10ms
#define VRDB_MAX_STEPTIME       0.1     //s =100ms

#define VRDB_TOX_BINLIMIT       20      //nm
#define VRDB_NUM_STEPS          1001
#define VRDB_MIN_DELAY          0.0001  //s =0.1ms

#define VRDB_I_ABS_BRKDWN       25      //A/cm2
//#define VRDB_I_SLOPE_BRKDWN     3       //slope value

#define VRDB_IG0_BRKDWN         1
#define VRDB_LOW_E_BRKDWN       2
#define VRDB_IUSE_BRKDWN        4
#define VRDB_SLOPE_BRKDWN       8
#define VRDB_HARD_BRKDWN        16

//VRDB test
#define VRBD_V_G0               1
#define VRBD_V_LOW_E            2
#define VRBD_V_USE_BEFORE       3
#define VRBD_V_USE_AFTER        4

//VRDB test types
#define VRDB_TYPE_VRAMP   1
#define VRDB_TYPE_VUPR    2
#define VRDB_TYPE_IRAMP   3
#define VRDB_TYPE_IUPR    4


//Errors
#define ERR_B1500       99
#define ERR_FILTER      100
#define ERR_ADC         101
#define ERR_ADCTYPE     102
#define ERR_SWITCH_ON   103
#define ERR_FORCE       104
#define ERR_MEAS        105
#define ERR_ZERO        106
#define ERR_APPROX      107
#define ERR_TIMESTAMP   108
#define ERR_MAXCURRENT  109
#define ERR_TEMPER      110   
#define ERR_SWITCH_OFF  111
#define ERR_TEMP_RAMP   112
#define ERR_COMPL_CARD  113
#define ERR_COMPL_PWR   114
#define ERR_READ_STATUS 115
#define ERR_STOP_COND   116
#define ERR_INIT_COEFF  117

#define ERR_SETSAMPLE       200
#define ERR_SAMPLEMODE      201
#define ERR_SAMPLEIV        203
#define ERR_SPOTMEAS        204
#define ERR_SAMPLESOURCE    205
#define ERR_CLRSAMPLE       206
#define ERR_STOPMODE        207
#define ERR_DATAFORMATE     208

#define ERR_SWPPOINTS   300
#define ERR_SWPSTOPMODE 301
#define ERR_SWPSETIV    302
#define ERR_SWPIV       303
#define ERR_SWPVTH      304
#define ERR_SWPM_SRC    305
#define ERR_SWPMIV      306

#define ERR_SETPBIAS    400
#define ERR_MEASP       401
#define ERR_CMUFREQ     402
#define ERR_CMUACLEVEL  403
#define ERR_CMUDCBIAS   404
#define ERR_SPOTCMU     405
#define ERR_SETCV       406
#define ERR_CVSET       406


#define ERR_ABORT       777
#define ERR_MATRIX      888
#define ERR_FILE        999







//работа
//с
//графиком

#define SCALE_EXT 1.1

double Max(QVector<double> &vect);
double Min(QVector<double> &vect);


ENTRY double studentDF(double &n, double &x);		//распределение Стьюдента
ENTRY double inv_studentDF(double &n, double &p);

class BetaDF {				//бета-распределение
   public:
      BetaDF(double u, double w);
      double value(double x);    // Функция распределения Beta(x|a,b)
      double inv(const double &p);      // Обратная функция: Beta(x|a,b)=p
   private:
      double a,b, logBeta;
      double fraction(double a, double b, double x);
};

ENTRY double logGamma(double x);	// Вычисляет натуральный логарифм полной гамма-функции Gamma(x)

double chi2DF(double n, double x);		//распределение хи-квадрат
double inv_chi2DF(double n, double x);

double normalDF(double x);                     //нормальное распределение
double inv_normalDF(const double &level);

class GammaDF {				//Гамма-распределение
    public:
       GammaDF(double shape, double scale=1);
       double value(double x);    // Функция распределения Gamma(x|shape,scale)
       double inv(const double &p);      // Обратная функция: Gamma(x|shape,scale)=p
    private:
       double a, shape, scale, lga;
       double fraction(const double &x);
       double series(const double &x);
 };

double FDF(double n1, double n2, double x);  // F-распределение (Фишера)
double inv_FDF(double n1, double n2, double p);

void saveAsSvg(QCustomPlot* widget, const QString &filename);

#endif // UTILS_H
