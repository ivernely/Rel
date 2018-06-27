/* Copyright (C) 1995-2005 Agilent Technologies Company */
/* VXIplug&play Instrument Driver for the agb1500  */

#ifndef agb1500_INCLUDE 
#define agb1500_INCLUDE
#include "vpptype.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif
/*****************************************************************************/
/*  template.h                                                               */
/*  Copyright (C) 1995-2005 Agilent Technologies Company                     */
/*---------------------------------------------------------------------------*/
/*  Instrument Driver for the agb1500                                         */
/*****************************************************************************/

/*****************************************************************************/
/*  STANDARD SECTION                                                         */
/*  Constants and function prototypes for standard functions.                */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* DEVELOPER: Remove what you don't need from this standard function         */
/*              section, with the exception that VPP required functions      */
/*              may not be removed.                                          */
/*            Don't add to this section - add to the instrument specific     */
/*              section below.                                               */
/*            Don't change section - if you need to "change" the prototype   */
/*              of a standard function, delete it from this section and      */
/*              add a new function (named differently) in the instrument     */
/*              specific section.                                            */
/*---------------------------------------------------------------------------*/

        /***************************************************/
        /*  Standard constant error conditions returned    */
        /*  by driver functions.                           */
        /*    HP Common Error numbers start at BFFC0D00    */
        /*    The parameter errors extend the number of    */
        /*      errors over the eight defined in VPP 3.4   */
        /***************************************************/

#define agb1500_INSTR_ERROR_NULL_PTR      (_VI_ERROR+0x3FFC0D02L) /* 0xBFFC0D02 */
#define agb1500_INSTR_ERROR_RESET_FAILED  (_VI_ERROR+0x3FFC0D03L) /* 0xBFFC0D03 */
#define agb1500_INSTR_ERROR_UNEXPECTED    (_VI_ERROR+0x3FFC0D04L) /* 0xBFFC0D04 */
#define agb1500_INSTR_ERROR_INV_SESSION   (_VI_ERROR+0x3FFC0D05L) /* 0xBFFC0D05 */
#define agb1500_INSTR_ERROR_LOOKUP        (_VI_ERROR+0x3FFC0D06L) /* 0xBFFC0D06 */
#define agb1500_INSTR_ERROR_DETECTED      (_VI_ERROR+0x3FFC0D07L) /* 0xBFFC0D07 */

#define agb1500_INSTR_ERROR_PARAMETER9    (_VI_ERROR+0x3FFC0D20L) /* 0xBFFC0D20 */
#define agb1500_INSTR_ERROR_PARAMETER10   (_VI_ERROR+0x3FFC0D21L) /* 0xBFFC0D21 */
#define agb1500_INSTR_ERROR_PARAMETER11   (_VI_ERROR+0x3FFC0D22L) /* 0xBFFC0D22 */
#define agb1500_INSTR_ERROR_PARAMETER12   (_VI_ERROR+0x3FFC0D23L) /* 0xBFFC0D23 */
#define agb1500_INSTR_ERROR_PARAMETER13   (_VI_ERROR+0x3FFC0D24L) /* 0xBFFC0D24 */
#define agb1500_INSTR_ERROR_PARAMETER14   (_VI_ERROR+0x3FFC0D25L) /* 0xBFFC0D25 */
#define agb1500_INSTR_ERROR_PARAMETER15   (_VI_ERROR+0x3FFC0D26L) /* 0xBFFC0D26 */
#define agb1500_INSTR_ERROR_PARAMETER16   (_VI_ERROR+0x3FFC0D27L) /* 0xBFFC0D27 */
#define agb1500_INSTR_ERROR_PARAMETER17   (_VI_ERROR+0x3FFC0D28L) /* 0xBFFC0D28 */
#define agb1500_INSTR_ERROR_PARAMETER18   (_VI_ERROR+0x3FFC0D29L) /* 0xBFFC0D29 */

        /***************************************************/
        /*  Constants used by system status functions      */
        /*    These defines are bit numbers which define   */
        /*    the operation and questionable registers.    */
        /*    They are instrument specific.                */
        /***************************************************/

/*---------------------------------------------------------------------------*/
/* DEVELOPER: Modify these bit values to reflect the meanings of the         */
/*            operation and questionable status registers for your           */
/*              instrument.                                                  */
/*---------------------------------------------------------------------------*/

        /***************************************************/
        /*  Constants used by function agb1500_timeOut     */
        /***************************************************/

#define agb1500_TIMEOUT_MAX         2147483647L
#define agb1500_TIMEOUT_MIN         1L

#define agb1500_CMDINT16ARR_Q_MIN   1L
#define agb1500_CMDINT16ARR_Q_MAX   2147483647L

#define agb1500_CMDINT32ARR_Q_MIN   1L
#define agb1500_CMDINT32ARR_Q_MAX   2147483647L

#define agb1500_CMDREAL32ARR_Q_MIN  1L
#define agb1500_CMDREAL32ARR_Q_MAX  2147483647L

#define agb1500_CMDREAL64ARR_Q_MIN  1L
#define agb1500_CMDREAL64ARR_Q_MAX  2147483647L

        /***************************************************/
        /*  Required plug and play functions from VPP-3.1  */
        /***************************************************/

ViStatus _VI_FUNC agb1500_init(ViRsrc InstrDesc, ViBoolean id_query,
    ViBoolean do_reset, ViPSession vi);
ViStatus _VI_FUNC agb1500_close(ViSession vi);
ViStatus _VI_FUNC agb1500_reset(ViSession vi);
ViStatus _VI_FUNC agb1500_self_test(ViSession vi, ViPInt16 test_result,
    ViChar _VI_FAR test_message[]);
ViStatus _VI_FUNC agb1500_error_query(ViSession vi, ViPInt32 error_number,
    ViChar _VI_FAR error_message[]);
ViStatus _VI_FUNC agb1500_error_message(ViSession vi, ViStatus error_number,
    ViChar _VI_FAR error_message[]);
ViStatus _VI_FUNC agb1500_revision_query(ViSession vi,
    ViChar _VI_FAR driver_rev[], ViChar _VI_FAR instr_rev[]);

        /***************************************************/
        /*  Agilent standard utility functions             */
        /***************************************************/

ViStatus _VI_FUNC agb1500_timeOut(ViSession vi, ViInt32 time_out);
ViStatus _VI_FUNC agb1500_timeOut_Q(ViSession vi, ViPInt32 time_out);
ViStatus _VI_FUNC agb1500_errorQueryDetect(ViSession vi, ViBoolean eq_detect);
ViStatus _VI_FUNC agb1500_errorQueryDetect_Q(ViSession vi, ViPBoolean eq_detect);
ViStatus _VI_FUNC agb1500_dcl(ViSession vi);
ViStatus _VI_FUNC agb1500_opc_Q(ViSession vi, ViPBoolean instr_ready);


        /***************************************************/
        /*  Agilent standard status functions              */
        /***************************************************/

ViStatus _VI_FUNC agb1500_readStatusByte_Q(ViSession vi, ViPInt16 status_byte);

        /***************************************************/
        /*  Agilent standard command passthrough functions */
        /***************************************************/

ViStatus _VI_FUNC agb1500_cmd( ViSession vi, ViString cmd_string);
ViStatus _VI_FUNC agb1500_cmdString_Q(ViSession vi, ViString cmd_string, 
                      ViInt32 size,ViChar _VI_FAR result[]);
ViStatus _VI_FUNC agb1500_cmdData_Q(ViSession vi, ViString cmd_string, 
                      ViInt32 size,ViChar _VI_FAR result[]);

ViStatus _VI_FUNC agb1500_cmdInt( ViSession vi, ViString cmd_string,
                      ViInt32 val);
ViStatus _VI_FUNC agb1500_cmdInt16_Q(ViSession vi, ViString cmd_string,
                      ViPInt16 result);
ViStatus _VI_FUNC agb1500_cmdInt32_Q(ViSession vi, ViString cmd_string,
                      ViPInt32 result);
ViStatus _VI_FUNC agb1500_cmdInt16Arr_Q(ViSession vi, ViString cmd_string,
                      ViInt32 size, ViInt16 _VI_FAR result[], ViPInt32 count);
ViStatus _VI_FUNC agb1500_cmdInt32Arr_Q(ViSession vi, ViString cmd_string,
                      ViInt32 size, ViInt32 _VI_FAR result[], ViPInt32 count);
    
ViStatus _VI_FUNC agb1500_cmdReal( ViSession vi, ViString cmd_string,
                      ViReal64 val);

ViStatus _VI_FUNC agb1500_cmdReal64_Q(ViSession vi, ViString cmd_string,
                      ViPReal64 result);

ViStatus _VI_FUNC agb1500_cmdReal64Arr_Q(ViSession vi, ViString cmd_string,
                      ViInt32 size, ViReal64 _VI_FAR result[], ViPInt32 count);


        /***************************************************/
        /*     the following use Real32 (float) types      */
        /***************************************************/


#ifdef INSTR_REAL32
ViStatus _VI_FUNC agb1500_cmdReal32_Q(ViSession vi, ViString p1,ViPReal32 p2);
ViStatus _VI_FUNC agb1500_cmdReal32Arr_Q(ViSession vi, ViString p1, 
                      ViInt32 p2,ViReal32 _VI_FAR p3[],ViPInt32 p4);
#endif

/* End of Agilent standard declarations */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
/*  INSTRUMENT SPECIFIC SECTION                                              */
/*  Constants and function prototypes for instrument specific functions.     */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* DEVELOPER: Add constants and function prototypes here.                    */
/*            As a matter of style, add the constant #define's first,        */
/*              followed by function prototypes.                             */
/*            Remember that function prototypes must be consistent with      */
/*              the driver's function panel prototypes.                      */
/*---------------------------------------------------------------------------*/

        /***************************************************/
        /*  Instrument specific constants                  */
        /***************************************************/

#define agb1500_FILE_OPEN_ERROR             (_VI_ERROR+0x3FFC0D30L) /* 0xBFFC0D30 */
#define agb1500_FILE_READ_ERROR             (_VI_ERROR+0x3FFC0D31L) /* 0xBFFC0D31 */
#define agb1500_FILE_WRITE_ERROR            (_VI_ERROR+0x3FFC0D32L) /* 0xBFFC0D32 */

#define agb1500_MEAS_DATA_INCONSISTENCY     (_VI_ERROR+0x3FFC0D33L) /* 0xBFFC0D33 */
#define agb1500_NO_CMU_UNT				    (_VI_ERROR+0x3FFC0D34L) /* 0xBFFC0D34 */

        /***************************************************/
        /*  Instrument specific functions                  */
        /***************************************************/

ViStatus _VI_FUNC agb1500_autoCal(ViSession vi, ViInt32 state);

ViStatus _VI_FUNC agb1500_setAdc(ViSession vi, ViInt32 adc, 
                                ViInt32 mode, ViInt32 value, 
                                ViInt32 autozero);

ViStatus _VI_FUNC agb1500_stopMode(ViSession vi, ViInt32 stop, 
                                ViInt32 last_mode);

ViStatus _VI_FUNC agb1500_abortMeasure(ViSession vi);

ViStatus _VI_FUNC agb1500_resetTimestamp(ViSession vi);

ViStatus _VI_FUNC agb1500_setSwitch(ViSession vi, ViInt32 channel, 
                                ViInt32 state);

ViStatus _VI_FUNC agb1500_setFilter(ViSession vi, ViInt32 channel, 
                                ViInt32 state);

ViStatus _VI_FUNC agb1500_setSerRes(ViSession vi, ViInt32 channel, 
                                ViInt32 state);

ViStatus _VI_FUNC agb1500_setAdcType(ViSession vi, ViInt32 channel, 
                                ViInt32 adc);

ViStatus _VI_FUNC agb1500_force(ViSession vi, ViInt32 channel,ViInt32 mode, 
                                ViReal64 range, ViReal64 value,ViReal64 comp,
                                ViInt32 polarity);

ViStatus _VI_FUNC agb1500_zeroOutput(ViSession vi, ViInt32 channel) ;

ViStatus _VI_FUNC agb1500_recoverOutput(ViSession vi, ViInt32 channel) ;

ViStatus _VI_FUNC agb1500_setIv(ViSession vi, 
                                ViInt32 channel, ViInt32 mode, ViReal64 range,
                                ViReal64 start, ViReal64 stop, ViInt32 point,
                                ViReal64 hold, ViReal64 delay, ViReal64 s_delay,
                                ViReal64 comp, ViReal64 p_comp ) ;

ViStatus _VI_FUNC agb1500_setPbias(ViSession vi, ViInt32 channel, 
                                ViInt32 mode, ViReal64 range, 
                                ViReal64 base, ViReal64 peak,
                                ViReal64 width, ViReal64 period, 
                                ViReal64 hold, ViReal64 comp );

ViStatus _VI_FUNC agb1500_setPiv(ViSession vi, 
                                ViInt32 channel, ViInt32 mode, ViReal64 range,
                                ViReal64 base, ViReal64 start, ViReal64 stop, 
                                ViInt32 point,
                                ViReal64 hold, ViReal64 width, ViReal64 period,
                                ViReal64 comp );

ViStatus _VI_FUNC agb1500_setSweepSync(ViSession vi, 
                                ViInt32 channel, ViInt32 mode, ViReal64 range,
                                ViReal64 start, ViReal64 stop,
                                ViReal64 comp, ViReal64 p_comp );

ViStatus _VI_FUNC agb1500_setNthSweep(ViSession vi, ViInt32 n,
                                ViInt32 channel, ViInt32 mode, ViReal64 range,
                                ViReal64 start, ViReal64 stop,
                                ViReal64 comp, ViReal64 p_comp );

ViStatus _VI_FUNC agb1500_spotMeas(ViSession vi, ViInt32 channel,
                                ViInt32 mode, ViReal64 range, 
                                ViPReal64 value, ViPInt32 status,
                                ViPReal64 time);

ViStatus _VI_FUNC agb1500_measureM(ViSession vi, ViInt32 channel[], 
                                ViInt32 mode[], ViReal64 range[],
                                ViReal64 value[], ViInt32 status[],
                                ViReal64 time[]);

ViStatus _VI_FUNC agb1500_sweepIv(ViSession vi, 
                                ViInt32 channel, ViInt32 mode, ViReal64 range,
                                ViPInt32 point, ViReal64 source[], 
                                ViReal64 value[], ViInt32 status[],
                                ViReal64 time[]);

ViStatus _VI_FUNC agb1500_sweepMiv(ViSession vi, 
                                ViInt32 channel[], ViInt32 mode[], 
                                ViReal64 range[],
                                ViPInt32 point, ViReal64 source[], 
                                ViReal64 value[], ViInt32 status[],
                                ViReal64 time[]);

ViStatus _VI_FUNC agb1500_measureP(ViSession vi, ViInt32 channel, 
                                ViInt32 mode, ViReal64 range,
                                ViPReal64 value, ViPInt32 status,
                                ViPReal64 time);

ViStatus _VI_FUNC agb1500_sweepPiv(ViSession vi, 
                                ViInt32 channel, ViInt32 mode, ViReal64 range,
                                ViPInt32 point, ViReal64 source[], 
                                ViReal64 value[], ViInt32 status[],
                                ViReal64 time[]);

ViStatus _VI_FUNC agb1500_sweepPbias(ViSession vi, 
                                ViInt32 channel, ViInt32 mode, ViReal64 range,
                                ViPInt32 point, ViReal64 source[], 
                                ViReal64 value[], ViInt32 status[],
                                ViReal64 time[]);

ViStatus _VI_FUNC agb1500_msweepIv(ViSession vi, 
                                ViInt32 channel, ViInt32 mode, ViReal64 range,
                                ViPInt32 point, ViReal64 source[], 
                                ViReal64 value[], ViInt32 status[],
                                ViReal64 time[]);

ViStatus _VI_FUNC agb1500_msweepMiv(ViSession vi, 
                                ViInt32 channel[], ViInt32 mode[], 
                                ViReal64 range[],
                                ViPInt32 point, ViReal64 source[], 
                                ViReal64 value[], ViInt32 status[],
                                ViReal64 time[]);

ViStatus _VI_FUNC agb1500_setBdv(ViSession vi, ViInt32 channel, 
                                ViReal64 range, 
                                ViReal64 start,  ViReal64 stop,  ViReal64 current, 
                                ViReal64 hold,  ViReal64 delay );

ViStatus _VI_FUNC agb1500_measureBdv(ViSession vi, ViInt32 interval,
                                ViPReal64 value, ViPInt32 status );

ViStatus _VI_FUNC agb1500_setIleak(ViSession vi, ViInt32 channel, ViReal64 range, 
                                ViReal64 voltage,  ViReal64 compliance,  
                                ViReal64 start, ViReal64 hold,  ViReal64 delay );

ViStatus _VI_FUNC  agb1500_measureIleak(ViSession vi, ViInt32 channel, 
                                ViInt32 interval, ViPReal64 value, ViPInt32 status );

ViStatus _VI_FUNC agb1500_startMeasure(ViSession vi, ViInt32 meas_type,
                                ViInt32 channel[], ViInt32 mode[], 
                                ViReal64 range[], ViInt32 source, ViInt32 timestamp,
								ViInt32 monitor );

ViStatus _VI_FUNC agb1500_readData(ViSession vi, ViPInt32 eod,
                                ViPInt32 data_type, ViPReal64 value, 
                                ViPInt32 status, ViPInt32 channel );

ViStatus _VI_FUNC agb1500_asuLed(ViSession vi, ViInt32 channel, ViInt32 mode );

ViStatus _VI_FUNC agb1500_asuPath(ViSession vi, ViInt32 channel, ViInt32 path );

ViStatus _VI_FUNC agb1500_asuRange(ViSession vi, ViInt32 channel, ViInt32 mode );

ViStatus _VI_FUNC agb1500_forceCmuDcBias(ViSession vi, ViInt32 channel, ViReal64 value );

ViStatus _VI_FUNC agb1500_setCmuInteg(ViSession vi, ViInt32 mode, ViInt32 value);

ViStatus _VI_FUNC agb1500_forceCmuAcLevel(ViSession vi, ViInt32 channel, ViReal64 value );

ViStatus _VI_FUNC agb1500_setCmuFreq(ViSession vi, ViInt32 channel, ViReal64 frequency );

ViStatus _VI_FUNC agb1500_setCv(ViSession vi, 
                                ViInt32 channel, ViInt32 mode, 
                                ViReal64 start, ViReal64 stop, ViInt32 point,
                                ViReal64 hold, ViReal64 delay, ViReal64 s_delay) ;

ViStatus _VI_FUNC agb1500_spotCmuMeas(ViSession vi, ViInt32 channel,
                                ViInt32 mode, ViReal64 range, 
                                ViReal64 data[], ViInt32 status[],
								ViReal64 monitor[],ViInt32 status_mon[],
                                ViPReal64 time);

ViStatus _VI_FUNC agb1500_sweepCv(ViSession vi, 
                                ViInt32 channel, ViInt32 mode, ViReal64 range,
                                ViPInt32 point, ViReal64 source[], 
                                ViReal64 value[], ViInt32 status[],
                                ViReal64 monitor[], ViInt32 status_mon[],
								ViReal64 time[]);

ViStatus _VI_FUNC agb1500_scuuLed(ViSession vi, ViInt32 channel, ViInt32 mode );

ViStatus _VI_FUNC agb1500_scuuPath(ViSession vi, ViInt32 channel, ViInt32 path );

ViStatus _VI_FUNC agb1500_setSample(ViSession vi, 
                               ViReal64 bias_hold, ViReal64 base_hold, ViReal64 interval, ViInt32 point);

ViStatus _VI_FUNC agb1500_addSampleSyncIv(ViSession vi, 
                               ViInt32 channel, ViInt32 mode, ViReal64 range,
                               ViReal64 base, ViReal64 bias, ViReal64 comp ); 

ViStatus _VI_FUNC agb1500_clearSampleSync(ViSession vi) ;

ViStatus _VI_FUNC agb1500_sampleIv(ViSession vi, 
                               ViInt32 channel[], ViInt32 mode[], 
                               ViReal64 range[], 
                               ViPInt32 point, ViInt32 index[],
                               ViReal64 value[], ViInt32 status[], ViReal64 time[]);

ViStatus _VI_FUNC agb1500_setSampleMode(ViSession vi, ViInt32 mode );

ViStatus _VI_FUNC agb1500_setCmuAdjustMode(ViSession vi, ViInt32 channel, ViInt32 mode) ;

ViStatus _VI_FUNC agb1500_execCmuAdjust(ViSession vi, ViInt32 channel, ViPInt16 result);

ViStatus _VI_FUNC agb1500_setOpenCorrMode(ViSession vi, ViInt32 channel, ViInt32 mode) ;

ViStatus _VI_FUNC agb1500_execOpenCorr(ViSession vi, ViInt32 channel, ViReal64 freq, ViInt32 mode,
									   ViReal64 primary, ViReal64 secondary, ViPInt16 result);

ViStatus _VI_FUNC agb1500_setShortCorrMode(ViSession vi, ViInt32 channel, ViInt32 mode) ;

ViStatus _VI_FUNC agb1500_execShortCorr(ViSession vi, ViInt32 channel, ViReal64 freq, ViInt32 mode,
									   ViReal64 primary, ViReal64 secondary, ViPInt16 result);

ViStatus _VI_FUNC agb1500_setLoadCorrMode(ViSession vi, ViInt32 channel, ViInt32 mode) ;

ViStatus _VI_FUNC agb1500_execLoadCorr(ViSession vi, ViInt32 channel, ViReal64 freq, ViInt32 mode,
									   ViReal64 primary, ViReal64 secondary, ViPInt16 result);

ViStatus _VI_FUNC agb1500_clearCorrData(ViSession vi, ViInt32 channel, ViInt32 mode) ;

/******   Channel Definition  *****/
#define agb1500_CH_ALL      0
#define agb1500_CH_1		1
#define agb1500_CH_2		2
#define agb1500_CH_3		3
#define agb1500_CH_4		4
#define agb1500_CH_5		5
#define agb1500_CH_6		6
#define agb1500_CH_7		7
#define agb1500_CH_8		8
#define agb1500_CH_9		9
#define agb1500_CH_10		10
#define agb1500_CH_CMU		-1

/******   Force Mode Definition  *****/
#define agb1500_IF_MODE      1
#define agb1500_VF_MODE      2


/******   Polarity Definition  *****/
#define agb1500_POL_AUTO     0
#define agb1500_POL_MANUAL   1


/******   Common Min/Max Definition  *****/
#define agb1500_RANGE_MIN    -200.0
#define agb1500_RANGE_MAX     200.0
#define agb1500_SMUV_MIN     -200.0
#define agb1500_SMUV_MAX      200.0
#define agb1500_SMUI_MIN       -1.0
#define agb1500_SMUI_MAX        1.0


/******      Spot Measurement Mode   *****/
#define agb1500_IM_MODE        1
#define agb1500_VM_MODE        2

/****** I/V Range Definition *****/
#define agb1500_AUTO_RANGE          0.0
#define agb1500_LMA_5V_RANGE        5.0
#define agb1500_LMA_05V_RANGE       0.5
#define agb1500_LMA_2V_RANGE        2.0    
#define agb1500_LMA_20V_RANGE      20.0    
#define agb1500_LMA_40V_RANGE      40.0    
#define agb1500_LMA_100V_RANGE    100.0    
#define agb1500_LMA_200V_RANGE    200.0    
#define agb1500_FIX_5V_RANGE       -5.0
#define agb1500_FIX_05V_RANGE      -0.5
#define agb1500_FIX_2V_RANGE       -2.0    
#define agb1500_FIX_20V_RANGE     -20.0    
#define agb1500_FIX_40V_RANGE     -40.0    
#define agb1500_FIX_100V_RANGE   -100.0    
#define agb1500_FIX_200V_RANGE   -200.0    
#define agb1500_LMA_100pA_RANGE     1.0e-10
#define agb1500_LMA_10pA_RANGE      1.0e-11
#define agb1500_LMA_1pA_RANGE       1.0e-12
#define agb1500_LMA_1nA_RANGE       1.0e-9 
#define agb1500_LMA_10nA_RANGE     10.0e-9 
#define agb1500_LMA_100nA_RANGE   100.0e-9 
#define agb1500_LMA_1uA_RANGE       1.0e-6 
#define agb1500_LMA_10uA_RANGE     10.0e-6 
#define agb1500_LMA_100uA_RANGE   100.0e-6 
#define agb1500_LMA_1mA_RANGE       1.0e-3 
#define agb1500_LMA_10mA_RANGE     10.0e-3 
#define agb1500_LMA_100mA_RANGE   100.0e-3 
#define agb1500_LMA_1A_RANGE        1.0    
#define agb1500_FIX_100pA_RANGE	   -1.0e-10
#define agb1500_FIX_10pA_RANGE	   -1.0e-11
#define agb1500_FIX_1pA_RANGE	   -1.0e-12
#define agb1500_FIX_1nA_RANGE      -1.0e-9 
#define agb1500_FIX_10nA_RANGE    -10.0e-9 
#define agb1500_FIX_100nA_RANGE  -100.0e-9 
#define agb1500_FIX_1uA_RANGE      -1.0e-6 
#define agb1500_FIX_10uA_RANGE    -10.0e-6 
#define agb1500_FIX_100uA_RANGE  -100.0e-6 
#define agb1500_FIX_1mA_RANGE      -1.0e-3 
#define agb1500_FIX_10mA_RANGE    -10.0e-3 
#define agb1500_FIX_100mA_RANGE  -100.0e-3 
#define agb1500_FIX_1A_RANGE       -1.0    

/****** Status Bit Definition *****/
#define    agb1500_STAT_OVF_BIT        1        /* DAC OverFlow */
#define    agb1500_STAT_OSC_BIT        2        /* Oscilation */
#define    agb1500_STAT_OCC_BIT        3        /* Other Channel Compliance */
#define    agb1500_STAT_TCC_BIT        4        /* This Channel Compliance */
#define    agb1500_STAT_SNF_BIT        5        /* Search target not found */
#define    agb1500_STAT_SAB_BIT        6        /* Measurement was aborted */
/****** Status Bir Definition for CMU *****/
#define    agb1500_STAT_NLUB_BIT       2        /* Null Loop Unbalance */
#define    agb1500_STAT_ASAT_BIT       3        /* IV Amp Saturation */

/****** Status Definition *****/
#define    agb1500_FLAG_OFF            0        /* AUTO CAL IBRATION OFF */
#define    agb1500_FLAG_ON             1        /* AUTO CAL IBRATION ON */

/****** ADC Type *****/
#define agb1500_HSPEED_ADC           0        /* HIGH-SPEED ADC */
#define agb1500_HRESOLN_ADC          1        /* HIGH-RESOLUTION ADC */

/****** Integration Mode *****/
#define agb1500_INTEG_AUTO             0        /* AUTO MODE */
#define agb1500_INTEG_MANUAL           1        /* MANUAL MODE */
#define agb1500_INTEG_PLC              2        /* PLC MODE */
#define agb1500_SETADC_VALUE_MIN       1        /* MIN AVERAGE/INTEG TIME */
#define agb1500_SETADC_VALUE_MAX    1023        /* MAX AVERAGE/INTEG TIME */

/****** Sweep Mode *****/
#define    agb1500_SWP_VF_SGLLIN       1        /* SINGLE LINEAR VOLTAGE */
#define    agb1500_SWP_VF_SGLLOG       2        /* SINGLE LOG VOLTAGE */
#define    agb1500_SWP_VF_DBLLIN       3        /* DOUBLE LINEAR VOLTAGE */
#define    agb1500_SWP_VF_DBLLOG       4        /* DOUBLE LOG VOLTAGE */
#define    agb1500_SWP_IF_SGLLIN      -1        /* SINGLE LINEAR CURRENT */
#define    agb1500_SWP_IF_SGLLOG      -2        /* SINGLE LOG CURRENT */
#define    agb1500_SWP_IF_DBLLIN      -3        /* DOUBLE LINEAR CURRENT */
#define    agb1500_SWP_IF_DBLLOG      -4        /* DOUBLE LOG CURRENT */

/****** Sweep Point *****/
#define    agb1500_SWP_POINT_MIN       1        /* Sweep Point Min. */
#define    agb1500_SWP_POINT_MAX    1001        /* Sweep Point Max. */

/****** Sweep Hold *****/
#define    agb1500_SWP_HOLD_MIN        0.00     /* Sweep Hold Min. */
#define    agb1500_SWP_HOLD_MAX      655.35     /* Sweep Hold Max. */

/****** Sweep Delay *****/
#define    agb1500_SWP_DELAY_MIN       0.00     /* Sweep Delay Min. */
#define    agb1500_SWP_DELAY_MAX      65.535    /* Sweep Delay Max of SMU. */
#define    agb1500_SWP_DELAY_CMU_MAX  655.35    /* Sweep Delay Max of CMU. */

/****** Sweep Step Delay *****/
#define    agb1500_SWP_SDELAY_MIN      0.00     /* Sweep Step Delay Min. */
#define    agb1500_SWP_SDELAY_MAX      1.00     /* Sweep Step Delay Max. */

/****** Sweep Power compliance *****/
#define    agb1500_SMU_POWER_MIN       1.0E-3    /* Sweep Power compliance Min. */
#define    agb1500_SMU_POWER_MAX      20.00     /* Sweep Power compliance Max. */

/****** Pulse Width *****/
#define    agb1500_SMUP_WIDTH_MIN      0.5E-3   /* Pulse Width Min. */
#define    agb1500_SMUP_WIDTH_MAX      2.0   /* Pulse Width Min. */

/****** Pulse Period *****/
#define    agb1500_SMUP_PERIOD_MIN     5.0E-3   /* Pulse Period Min. */
#define    agb1500_SMUP_PERIOD_MAX     5.0     /* Pulse Period Min. */

/****** Pulse Hold *****/
#define    agb1500_SMUP_HOLD_MIN       0.00     /* Pulse Hold Min. */
#define    agb1500_SMUP_HOLD_MAX     655.35     /* Pulse Hold Min. */

/****** Quasi-Pulsed Spot Interval *****/
#define    agb1500_SHORT_INTERVAL      0        /* Short Interval Measurement */
#define    agb1500_LONG_INTERVAL       1        /* Long Interval Measurement */

/****** Quasi-Pulsed Spot Hold *****/
#define    agb1500_QPM_HOLD_MIN        0.00     /* Quasi-Pulsed Spot Hold Min. */
#define    agb1500_QPM_HOLD_MAX      655.35     /* Quasi-Pulsed Spot Hold Max. */

/****** Quasi-Pulsed Spot Delay *****/
#define    agb1500_QPM_DELAY_MIN       0.00     /* Quasi-Pulsed Spot Delay Min. */
#define    agb1500_QPM_DELAY_MAX      6.5535    /* Quasi-Pulsed Spot Delay Max. */

/****** Multi-Channel Sweep Source Number *****/
#define    agb1500_NTH_SWEEP_MIN       2
#define    agb1500_NTH_SWEEP_MAX       10

/****** Last Mode *****/
#define    agb1500_LAST_START          1         /* RETURNS TO START VALUE */
#define    agb1500_LAST_STOP           2         /* KEEPS STOP VALUE */

/****** Measurment Type *****/
#define    agb1500_MM_MSPOT            1         /* MULTI SPOT */
#define    agb1500_MM_SWEEP            2         /* STAIRCASE SWEEP */
#define    agb1500_MM_PSPOT            3         /* PULSE SPOT */
#define    agb1500_MM_PSWEEP           4         /* PULSE SWEEP */
#define    agb1500_MM_SWEEPP           5         /* SWEEP WITH PULSED BIAS */
#define    agb1500_MM_ASEARCH          6         /* Reserved : ANALOG SEARCH */
#define    agb1500_MM_DPSPOT           7         /* Reserved : 2CH PULSE SPOT */
#define    agb1500_MM_PSWEEPP          8         /* Reserved : PULSED SWEEP WITH PULSED BIAS */
#define    agb1500_MM_QPSPOT           9         /* QUASI-PULSED SPOT */
#define    agb1500_MM_IVSAMPLE        10         /* I/V SAMPLING */
#define    agb1500_MM_LSEARCH         14         /* LINEAR SEARCH */
#define    agb1500_MM_BSEARCH         15         /* BINARY SEARCH */
#define    agb1500_MM_MSWEEP          16         /* MULTI-CHANNEL SWEEP */
#define    agb1500_MM_CSPOT           17         /* C SPOT */
#define    agb1500_MM_CVSWEEP         18          /* C-V STAIRCASE SWEEP */

/****** Dummy Data *****/
#define    agb1500_DUMMY_VALUE         1.0E+99
#define    agb1500_DUMMY_STATUS        0
#define    agb1500_DUMMY_TIMESTAMP     0.0
#define    agb1500_DUMMY_INDEX		   0

/****** Read Data Type *****/
#define    agb1500_DT_DUMMY           -1        /* DUMMY RETURN DATA */
#define    agb1500_DT_IM               1        /* CURRENT MEASUREMENT DATA */
#define    agb1500_DT_VM               2        /* VOLTAGE MEASUREMENT DATA */
#define    agb1500_DT_IS               3        /* CURRENT OUTPUT DATA */
#define    agb1500_DT_VS               4        /* VOLTAGE OUTPUT DATA */
#define    agb1500_DT_TM               5        /* TIME STAMP DATA */ 
#define    agb1500_DT_ZM			   6        /* IMPEDANCE MEASUREMENT DATA (Resistance and Reactance) */
#define    agb1500_DT_YM			   7	    /* ADMITTANCE MEASUREMENT DATA (Conductance and Susceptance) */
#define    agb1500_DT_CM			   8        /* CAPACITANCE MEASUREMENT DATA */
#define    agb1500_DT_DM               9		/* DISSIPATION MEASUREMENT DATA */
#define    agb1500_DT_QM               10       /* QUALITY FACTOR */ 
#define    agb1500_DT_LM               11       /* INDUCTANCE MEASUREMENT DATA */
#define    agb1500_DT_RM               12       /* PHASE MEASUREMENT DATA (Radian) */
#define    agb1500_DT_PM               13       /* PHASE MEASUREMENT DATA (Degree) */
#define    agb1500_DT_FS               14       /* FREQUENCY DATA */
#define    agb1500_DT_X                15		/* SAMPLING INDEX */
#define    agb1500_DT_INV              16       /* INVALID DATA */
#define    agb1500_CH_NOCH             -1        /* NO CHANNEL RELATED DATA */

/****** ASU Path Type *****/
#define    agb1500_ASU_DC              1        /* SMU Output Path */
#define    agb1500_ASU_AUX             2        /* AUX Output Path */

/****** CMU DC Bias Type *****/
#define    agb1500_CMUV_MIN            -100     /* CMU DC Bias Min */
#define    agb1500_CMUV_MAX            100      /* CMU DC Bias Max */

/****** CMU AC Level Type *****/
#define    agb1500_CMUAC_MIN           0.0      /* CMU AC Level Min ïœçXÇÃâ¬î\ê´Ç†ÇË*/
#define    agb1500_CMUAC_MAX           0.25     /* CMU AC Level Max */

/****** CMU Frequency Type *****/
#define    agb1500_CMUF_MIN            1000.0    /* CMU Frequency Min */
#define    agb1500_CMUF_MAX            5000000.0 /* CMU Frequency Max */

/****** CMU Impedance Measurment Mode *****/
#define    agb1500_CMUM_R_X            1         /* R-X Function Mode */
#define    agb1500_CMUM_G_B            2         /* G-B Function Mode */
#define    agb1500_CMUM_Z_TRAD         10        /* Z-Theta(radian) Function Mode */
#define    agb1500_CMUM_Z_TDEG         11        /* Z-Theta(degree) Function Mode */
#define    agb1500_CMUM_Y_TRAD         20        /* Y-Theta(radian) Function Mode */
#define    agb1500_CMUM_Y_TDEG         21        /* Y-Theta(degree) Function Mode */
#define    agb1500_CMUM_CP_G           100       /* Cp-G Function Mode */
#define    agb1500_CMUM_CP_D           101       /* Cp-D Function Mode */
#define    agb1500_CMUM_CP_Q           102       /* Cp-Q Function Mode */
#define    agb1500_CMUM_CP_RP          103       /* Cp-Rp Function Mode */
#define    agb1500_CMUM_CS_RS          200       /* Cs-Rs Function Mode */
#define    agb1500_CMUM_CS_D           201       /* Cs-D Function Mode */
#define    agb1500_CMUM_CS_Q		   202		 /* Cs-Q Function Mode */
#define    agb1500_CMUM_LP_G           300       /* Lp-G Function Mode */
#define    agb1500_CMUM_LP_D           301       /* Lp-D Function Mode */
#define    agb1500_CMUM_LP_Q           302       /* Lp-Q Function Mode */
#define    agb1500_CMUM_LP_RP          303       /* Lp-Rp Function Mode */
#define    agb1500_CMUM_LS_RS          400       /* Ls-Rs Function Mode */
#define    agb1500_CMUM_LS_D           401       /* Ls-D Function Mode */
#define    agb1500_CMUM_LS_Q           402       /* Ls-Q Function Mode */     

/****** SCUU Path Type *****/
#define    agb1500_SCUU_SMUH		   1         /* Relay Path SMU (SMUH) */
#define    agb1500_SCUU_SMUL		   2         /* Relay Path SMU (SMUL) */
#define    agb1500_SCUU_SMUHL          3         /* Relay Path SMU (SMUH and SMUL) */
#define    agb1500_SCUU_CMU            4         /* Relay Path CMU (CMU Bias Path) */

/****** Sampling Measurement Type *****/
#define    agb1500_SMP_HOLD_MIN		   -0.09     /* Sampling hold time Min */
#define	   agb1500_SMP_HOLD_MAX	       655.35    /* Sampling hold time Max */
#define    agb1500_SMP_INTVL_MIN       0.0001    /* Sampling interval Min  */
#define    agb1500_SMP_INTVL_MAX       65.535    /* Sampling interval Max  */
#define    agb1500_SMP_POINT_MIN	   1         /* Sampling point Min     */
#define    agb1500_SMP_POINT_MAX       100001    /* Sampling point Max     */
#define    agb1500_SMP_LINEAR          0         /* Linear Sampling Mode   */
#define    agb1500_SMP_LOG10           1         /* Log10 Sampling Mode    */
#define    agb1500_SMP_LOG25           2         /* Log25 Sampling Mode    */
#define    agb1500_SMP_LOG50           3         /* Log50 Sampling Mode    */
#define    agb1500_SMP_LOG100          4         /* Log100 Sampling Mode   */
#define    agb1500_SMP_LOG250          5         /* Log50 Sampling Mode    */
#define    agb1500_SMP_LOG500          6         /* Log100 Sampling Mode   */
 	 
/****** CMU Null-Gain Phase Adjust Mode Type *****/
#define	   agb1500_CMUADJ_MANUAL		1		 /* Manual Mode */
#define    agb1500_CMUADJ_AUTO			0		 /* Auto Mode */

/****** CMU Correction Data Clear Mode Type *****/
#define	   agb1500_CMUCORR_DEFAULT	    0        /* Set Default Frequency Parameter */ 
#define    agb1500_CMUCORR_CLEAR        1        /* Clear Frequency Parameter */

/* Used for "C" externs in C++ */
#if defined(__cplusplus) || defined(__cplusplus__)
}    /* end of "C" externs for C++ */
#endif 

#endif /* agb1500_INCLUDE */

