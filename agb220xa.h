/* Copyright (C) 2004 Agilent Technologies */
/* VXIplug&play Instrument Driver for the agb220xa*/

#ifndef agb220xa_INCLUDE 
#define agb220xa_INCLUDE
#include "vpptype.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif
/*****************************************************************************/
/*  template.h                                                               */
/*  Copyright (C) 2004 Agilent Technologies                                  */
/*---------------------------------------------------------------------------*/
/*  Instrument Driver for the agb220xa                                         */
/*****************************************************************************/

/*****************************************************************************/
/*  STANDARD SECTION                                                         */
/*  Constants and function prototypes for Agilent standard functions.        */
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

        /********************************************************/
        /*  Standard constant error conditions returned         */
        /*  by driver functions.                                */
        /*    Agilent Common Error numbers start at BFFC0D00    */
        /*    The parameter errors extend the number of         */
        /*      errors over the eight defined in VPP 3.4        */
        /********************************************************/

#define agb220xa_INSTR_ERROR_NULL_PTR      (_VI_ERROR+0x3FFC0D02L) /* 0xBFFC0D02 */
#define agb220xa_INSTR_ERROR_RESET_FAILED  (_VI_ERROR+0x3FFC0D03L) /* 0xBFFC0D03 */
#define agb220xa_INSTR_ERROR_UNEXPECTED    (_VI_ERROR+0x3FFC0D04L) /* 0xBFFC0D04 */
#define agb220xa_INSTR_ERROR_INV_SESSION   (_VI_ERROR+0x3FFC0D05L) /* 0xBFFC0D05 */
#define agb220xa_INSTR_ERROR_LOOKUP        (_VI_ERROR+0x3FFC0D06L) /* 0xBFFC0D06 */
#define agb220xa_INSTR_ERROR_DETECTED      (_VI_ERROR+0x3FFC0D07L) /* 0xBFFC0D07 */

#define agb220xa_INSTR_ERROR_PARAMETER9    (_VI_ERROR+0x3FFC0D20L) /* 0xBFFC0D20 */
#define agb220xa_INSTR_ERROR_PARAMETER10   (_VI_ERROR+0x3FFC0D21L) /* 0xBFFC0D21 */
#define agb220xa_INSTR_ERROR_PARAMETER11   (_VI_ERROR+0x3FFC0D22L) /* 0xBFFC0D22 */
#define agb220xa_INSTR_ERROR_PARAMETER12   (_VI_ERROR+0x3FFC0D23L) /* 0xBFFC0D23 */
#define agb220xa_INSTR_ERROR_PARAMETER13   (_VI_ERROR+0x3FFC0D24L) /* 0xBFFC0D24 */
#define agb220xa_INSTR_ERROR_PARAMETER14   (_VI_ERROR+0x3FFC0D25L) /* 0xBFFC0D25 */
#define agb220xa_INSTR_ERROR_PARAMETER15   (_VI_ERROR+0x3FFC0D26L) /* 0xBFFC0D26 */
#define agb220xa_INSTR_ERROR_PARAMETER16   (_VI_ERROR+0x3FFC0D27L) /* 0xBFFC0D27 */
#define agb220xa_INSTR_ERROR_PARAMETER17   (_VI_ERROR+0x3FFC0D28L) /* 0xBFFC0D28 */
#define agb220xa_INSTR_ERROR_PARAMETER18   (_VI_ERROR+0x3FFC0D29L) /* 0xBFFC0D29 */

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
#define agb220xa_QUES_VOLT          1
#define agb220xa_QUES_CURR          2
#define agb220xa_QUES_TIME          4
#define agb220xa_QUES_POW           8
#define agb220xa_QUES_TEMP         16
#define agb220xa_QUES_FREQ         32
#define agb220xa_QUES_PHAS         64
#define agb220xa_QUES_MOD         128
#define agb220xa_QUES_CAL         256
#define agb220xa_QUES_AVAIL10     512
#define agb220xa_QUES_AVAIL11    1024
#define agb220xa_QUES_AVAIL12    2048
#define agb220xa_QUES_AVAIL13    4096
#define agb220xa_QUES_INST       8192
#define agb220xa_QUES_CMD_WARN  16384

#define agb220xa_OPER_CAL           1
#define agb220xa_OPER_SETT          2
#define agb220xa_OPER_RANG          4
#define agb220xa_OPER_SWE           8
#define agb220xa_OPER_MEAS         16
#define agb220xa_OPER_TRIG         32
#define agb220xa_OPER_ARM          64
#define agb220xa_OPER_CORR        128
#define agb220xa_OPER_AVAIL9      256
#define agb220xa_OPER_AVAIL10     512
#define agb220xa_OPER_AVAIL11    1024
#define agb220xa_OPER_AVAIL12    2048
#define agb220xa_OPER_AVAIL13    4096
#define agb220xa_OPER_INST       8192
#define agb220xa_OPER_PROG      16384

        /***************************************************/
        /*  Constants used by function agb220xa_timeOut      */
        /***************************************************/

#define agb220xa_TIMEOUT_MAX         2147483647L
#define agb220xa_TIMEOUT_MIN         1L

#define agb220xa_CMDINT16ARR_Q_MIN   1L
#define agb220xa_CMDINT16ARR_Q_MAX   2147483647L

#define agb220xa_CMDINT32ARR_Q_MIN   1L
#define agb220xa_CMDINT32ARR_Q_MAX   2147483647L

#define agb220xa_CMDREAL32ARR_Q_MIN  1L
#define agb220xa_CMDREAL32ARR_Q_MAX  2147483647L

#define agb220xa_CMDREAL64ARR_Q_MIN  1L
#define agb220xa_CMDREAL64ARR_Q_MAX  2147483647L

        /***************************************************/
        /*  Required plug and play functions from VPP-3.1  */
        /***************************************************/

ViStatus _VI_FUNC agb220xa_init(ViRsrc InstrDesc, ViBoolean id_query,
    ViBoolean do_reset, ViPSession vi);
ViStatus _VI_FUNC agb220xa_close(ViSession vi);
ViStatus _VI_FUNC agb220xa_reset(ViSession vi);
ViStatus _VI_FUNC agb220xa_self_test(ViSession vi, ViPInt16 test_result,
    ViChar _VI_FAR test_message[]);
ViStatus _VI_FUNC agb220xa_error_query(ViSession vi, ViPInt32 error_number,
    ViChar _VI_FAR error_message[]);
ViStatus _VI_FUNC agb220xa_error_message(ViSession vi, ViStatus error_number,
    ViChar _VI_FAR error_message[]);
ViStatus _VI_FUNC agb220xa_revision_query(ViSession vi,
    ViChar _VI_FAR driver_rev[], ViChar _VI_FAR instr_rev[]);

        /***************************************************/
        /*  Agilent standard utility functions             */
        /***************************************************/

ViStatus _VI_FUNC agb220xa_timeOut(ViSession vi, ViInt32 time_out);
ViStatus _VI_FUNC agb220xa_timeOut_Q(ViSession vi, ViPInt32 time_out);
ViStatus _VI_FUNC agb220xa_errorQueryDetect(ViSession vi, ViBoolean eq_detect);
ViStatus _VI_FUNC agb220xa_errorQueryDetect_Q(ViSession vi, ViPBoolean eq_detect);
ViStatus _VI_FUNC agb220xa_dcl(ViSession vi);
ViStatus _VI_FUNC agb220xa_opc_Q(ViSession vi, ViPBoolean instr_ready);

        /***************************************************/
        /*  Agilent standard status functions              */
        /***************************************************/

ViStatus _VI_FUNC agb220xa_readStatusByte_Q(ViSession vi, ViPInt16 status_byte);
ViStatus _VI_FUNC agb220xa_operEvent_Q(ViSession vi, ViPInt32 sregister);
ViStatus _VI_FUNC agb220xa_operCond_Q(ViSession vi, ViPInt32 sregister );
ViStatus _VI_FUNC agb220xa_quesEvent_Q(ViSession vi, ViPInt32 sregister );
ViStatus _VI_FUNC agb220xa_quesCond_Q(ViSession vi, ViPInt32 sregister );

        /***************************************************/
        /*  Agilent standard command passthrough functions */
        /***************************************************/

ViStatus _VI_FUNC agb220xa_cmd( ViSession vi, ViString cmd_string);
ViStatus _VI_FUNC agb220xa_cmdString_Q(ViSession vi, ViString cmd_string, 
	ViInt32 size,ViChar _VI_FAR result[]);
ViStatus _VI_FUNC agb220xa_cmdData_Q(ViSession vi, ViString cmd_string, 
	ViInt32 size,ViChar _VI_FAR result[]);
	
ViStatus _VI_FUNC agb220xa_cmdInt( ViSession vi, ViString cmd_string,
    ViInt32 val);
ViStatus _VI_FUNC agb220xa_cmdInt16_Q(ViSession vi, ViString cmd_string,
    ViPInt16 result);
ViStatus _VI_FUNC agb220xa_cmdInt32_Q(ViSession vi, ViString cmd_string,
    ViPInt32 result);
ViStatus _VI_FUNC agb220xa_cmdInt16Arr_Q(ViSession vi, ViString cmd_string,
    ViInt32 size, ViInt16 _VI_FAR result[], ViPInt32 count);
ViStatus _VI_FUNC agb220xa_cmdInt32Arr_Q(ViSession vi, ViString cmd_string,
    ViInt32 size, ViInt32 _VI_FAR result[], ViPInt32 count);
    
ViStatus _VI_FUNC agb220xa_cmdReal( ViSession vi, ViString cmd_string,
    ViReal64 val);
ViStatus _VI_FUNC agb220xa_cmdReal64_Q(ViSession vi, ViString cmd_string,
    ViPReal64 result);
ViStatus _VI_FUNC agb220xa_cmdReal64Arr_Q(ViSession vi, ViString cmd_string,
    ViInt32 size, ViReal64 _VI_FAR result[], ViPInt32 count);



        /***************************************************/
	  /*     the following use Real32 (float) types      */
        /***************************************************/


#ifdef INSTR_REAL32
ViStatus _VI_FUNC agb220xa_cmdReal32_Q(ViSession vi, ViString p1,ViPReal32 p2);
ViStatus _VI_FUNC agb220xa_cmdReal32Arr_Q(ViSession vi, ViString p1, 
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

#define agb220xa_FILE_OPEN_ERROR     		(_VI_ERROR+0x3FFC0D30L) /* 0xBFFC0D30 */
#define agb220xa_FILE_READ_ERROR    		(_VI_ERROR+0x3FFC0D31L) /* 0xBFFC0D31 */
#define agb220xa_FILE_WRITE_ERROR    		(_VI_ERROR+0x3FFC0D32L) /* 0xBFFC0D32 */
#define agb220xa_FILE_REV_ERROR    			(_VI_ERROR+0x3FFC0D33L) /* 0xBFFC0D33 */
#define agb220xa_FILE_FORMAT_ERROR    		(_VI_ERROR+0x3FFC0D34L) /* 0xBFFC0D34 */


#define MAX_CHANNEL_LIST_ELEM			100 /* Maximum no. of channels */

        /***************************************************/
        /*  Instrument specific functions                  */
        /***************************************************/


/******   Used by Function agb220xa_biasChanCard   *****/
#define  agb220xa_BIAS_ENAB  0
#define  agb220xa_BIAS_DISA  1
#define  agb220xa_BIAS_CARD0  0
#define  agb220xa_BIAS_CARD1  1
#define  agb220xa_BIAS_CARD2  2
#define  agb220xa_BIAS_CARD3  3
#define  agb220xa_BIAS_CARD4  4
#define  agb220xa_BIAS_CARDA  5

/******   Used by Function agb220xa_groundChanCard   *****/
#define  agb220xa_AGND_ENAB  0
#define  agb220xa_AGND_DISA  1
#define  agb220xa_AGND_CARD0  0
#define  agb220xa_AGND_CARD1  1
#define  agb220xa_AGND_CARD2  2
#define  agb220xa_AGND_CARD3  3
#define  agb220xa_AGND_CARD4  4
#define  agb220xa_AGND_CARDA  5

/******   Used by Function agb220xa_biasChanList   *****/
/*  agb220xa_BIAS_ENAB  */
/*  agb220xa_BIAS_DISA  */


/******   Used by Function agb220xa_biasChanList_Q   *****/
/*  agb220xa_BIAS_ENAB  */
/*  agb220xa_BIAS_DISA  */


/******   Used by Function agb220xa_biasPort   *****/
#define  agb220xa_PORT_CARD0  0
#define  agb220xa_PORT_CARD1  1
#define  agb220xa_PORT_CARD2  2
#define  agb220xa_PORT_CARD3  3
#define  agb220xa_PORT_CARD4  4
#define  agb220xa_PORT_CARDA  5
#define  agb220xa_PORT_MAX  14
#define  agb220xa_PORT_MIN  1


/******   Used by Function agb220xa_biasState   *****/
/*  agb220xa_BIAS_CARD0  */
/*  agb220xa_BIAS_CARD1  */
/*  agb220xa_BIAS_CARD2  */
/*  agb220xa_BIAS_CARD3  */
/*  agb220xa_BIAS_CARD4  */
/*  agb220xa_BIAS_CARDA  */
#define  agb220xa_BIAS_OFF  0
#define  agb220xa_BIAS_ON  1

/******   Used by Function agb220xa_groundState   *****/
/*  agb220xa_AGND_CARD0  */
/*  agb220xa_AGND_CARD1  */
/*  agb220xa_AGND_CARD2  */
/*  agb220xa_AGND_CARD3  */
/*  agb220xa_AGND_CARD4  */
/*  agb220xa_AGND_CARDA  */
#define  agb220xa_AGND_OFF  0
#define  agb220xa_AGND_ON  1

/******   Used by Function agb220xa_closeCard_Q   *****/
#define  agb220xa_CLOS_CARD0  0
#define  agb220xa_CLOS_CARD1  1
#define  agb220xa_CLOS_CARD2  2
#define  agb220xa_CLOS_CARD3  3
#define  agb220xa_CLOS_CARD4  4


/******   Used by Function agb220xa_connRuleSeq   *****/
#define  agb220xa_CONN_CARD0  0
#define  agb220xa_CONN_CARD1  1
#define  agb220xa_CONN_CARD2  2
#define  agb220xa_CONN_CARD3  3
#define  agb220xa_CONN_CARD4  4
#define  agb220xa_CONN_CARDA  5
#define  agb220xa_CONN_FREE  0
#define  agb220xa_CONN_SROU  1
#define  agb220xa_CONN_NSEQ  0
#define  agb220xa_CONN_BBM  1
#define  agb220xa_CONN_MBBR  2


/******   Used by Function agb220xa_couplePort   *****/
#define  agb220xa_COUP_CARD0  0
#define  agb220xa_COUP_CARD1  1
#define  agb220xa_COUP_CARD2  2
#define  agb220xa_COUP_CARD3  3
#define  agb220xa_COUP_CARD4  4
#define  agb220xa_COUP_CARDA  5
#define  agb220xa_COUP_PORT10  0
#define  agb220xa_COUP_PORT11  1
#define  agb220xa_COUP_PORT30  0
#define  agb220xa_COUP_PORT31  1
#define  agb220xa_COUP_PORT50  0
#define  agb220xa_COUP_PORT51  1
#define  agb220xa_COUP_PORT70  0
#define  agb220xa_COUP_PORT71  1
#define  agb220xa_COUP_PORT90  0
#define  agb220xa_COUP_PORT91  1
#define  agb220xa_COUP_PORT110  0
#define  agb220xa_COUP_PORT111  1
#define  agb220xa_COUP_PORT130  0
#define  agb220xa_COUP_PORT131  1


/******   Used by Function agb220xa_coupleState   *****/
/*  agb220xa_COUP_CARD0  */
/*  agb220xa_COUP_CARD1  */
/*  agb220xa_COUP_CARD2  */
/*  agb220xa_COUP_CARD3  */
/*  agb220xa_COUP_CARD4  */
/*  agb220xa_COUP_CARDA  */
#define  agb220xa_COUP_OFF  0
#define  agb220xa_COUP_ON  1


/******   Used by Function agb220xa_func   *****/
#define  agb220xa_FUNC_ACON  0
#define  agb220xa_FUNC_NCON  1


/******   Used by Function agb220xa_openCard   *****/
#define  agb220xa_OPEN_CARD0  0
#define  agb220xa_OPEN_CARD1  1
#define  agb220xa_OPEN_CARD2  2
#define  agb220xa_OPEN_CARD3  3
#define  agb220xa_OPEN_CARD4  4
#define  agb220xa_OPEN_CARDA  5


/******   Used by Function agb220xa_testClear   *****/
#define  agb220xa_CLEAR_FPALL  0
#define  agb220xa_CLEAR_CARD1  1
#define  agb220xa_CLEAR_CARD2  2
#define  agb220xa_CLEAR_CARD3  3
#define  agb220xa_CLEAR_CARD4  4
#define  agb220xa_CLEAR_CARDA  5
#define  agb220xa_CLEAR_FPAN   6
#define  agb220xa_CLEAR_CONT  7
#define  agb220xa_CLEAR_PEN   8
#define  agb220xa_CLEAR_LED   9
#define  agb220xa_CLEAR_BEEP  10


/******   Used by Function agb220xa_testExec_Q   *****/
#define  agb220xa_EXEC_CARD1  1
#define  agb220xa_EXEC_CARD2  2
#define  agb220xa_EXEC_CARD3  3
#define  agb220xa_EXEC_CARD4  4
#define  agb220xa_EXEC_CARDA  5
#define  agb220xa_EXEC_FPAN   6
#define  agb220xa_EXEC_CONT  7
#define  agb220xa_EXEC_PEN   8
#define  agb220xa_EXEC_LED   9
#define  agb220xa_EXEC_BEEP  10

/******   Used by Function agb220xa_compenC   *****/
#define  agb220xa_FREQ_MAX			1e6
#define  agb220xa_FREQ_MIN			1e3
#define  agb220xa_CCOM_B2210A		0
#define  agb220xa_CCOM_B2211A		1
#define  agb220xa_CCOM_TRIAX_1_5	0
#define  agb220xa_CCOM_TRIAX_3	1
#define  agb220xa_CCOM_TRIAX_4	2
#define  agb220xa_CCOM_KELVIN_1_5	3
#define  agb220xa_CCOM_KELVIN_3	4

/******   Used by Function agb220xa_unusedPort   *****/
#define agb220xa_UNUSEDPORT_MAX  8

ViStatus _VI_FUNC agb220xa_biasChanCard  (ViSession vi, ViInt16 disable_enable, ViInt16 bias_cardno);

ViStatus _VI_FUNC agb220xa_biasChanList  (ViSession vi, ViInt16 biaschan_disen, ViInt32 _VI_FAR biaschan_list[]);

ViStatus _VI_FUNC agb220xa_biasChanList_Q  (ViSession vi, ViInt16 bias_disen, ViInt32 _VI_FAR biaschan_list[], ViInt32 _VI_FAR bias_status[]);

ViStatus _VI_FUNC agb220xa_biasPort  (ViSession vi, ViInt16 biasport_cardno, ViInt16 bias_port);

ViStatus _VI_FUNC agb220xa_biasState  (ViSession vi, ViInt16 biasstate_cardno, ViInt16 state);

ViStatus _VI_FUNC agb220xa_closeCard_Q  (ViSession vi, ViInt16 close_card, ViInt32 _VI_FAR closechan_list[]);

ViStatus _VI_FUNC agb220xa_closeList  (ViSession vi, ViInt32 _VI_FAR closechan_list[]);

ViStatus _VI_FUNC agb220xa_closeList_Q  (ViSession vi, ViInt32 _VI_FAR closechan_list[], ViInt32 _VI_FAR close_status[]);

ViStatus _VI_FUNC agb220xa_connRuleSeq  (ViSession vi, ViInt16 cardno_ruleseq, ViInt16 rule, ViInt16 sequence);

ViStatus _VI_FUNC agb220xa_couplePort  (ViSession vi, ViInt16 coupleport_cardno, ViInt16 port1, ViInt16 port3, ViInt16 port5, ViInt16 port7, ViInt16 port9, ViInt16 port11, ViInt16 port13);

ViStatus _VI_FUNC agb220xa_coupleState  (ViSession vi, ViInt16 couplestate_cardno, ViInt16 couple_state);

ViStatus _VI_FUNC agb220xa_esr_Q  (ViSession vi, ViChar _VI_FAR errstr[]);

ViStatus _VI_FUNC agb220xa_func  (ViSession vi, ViInt16 channel_config);

ViStatus _VI_FUNC agb220xa_openCard  (ViSession vi, ViInt16 open_cardno);

ViStatus _VI_FUNC agb220xa_openList  (ViSession vi, ViInt32 _VI_FAR openchan_list[]);

ViStatus _VI_FUNC agb220xa_openList_Q  (ViSession vi, ViInt32 _VI_FAR openchan_list[], ViInt32 _VI_FAR open_status[]);

ViStatus _VI_FUNC agb220xa_testClear  (ViSession vi, ViInt16 framecard_clear);

ViStatus _VI_FUNC agb220xa_testExec_Q  (ViSession vi, ViInt16 framecard_exec, ViPInt16 exec_result);

ViStatus _VI_FUNC agb220xa_selectCompenFile  (ViSession vi, ViString file_name);

ViStatus _VI_FUNC agb220xa_compenC(ViSession vi, ViReal64 frequency, ViReal64 raw_c, ViReal64 raw_g, ViPReal64 compen_c, ViPReal64 compen_g);

ViStatus _VI_FUNC agb220xa_groundPort  (ViSession vi, ViInt16 groundport_cardno, ViInt16 ground_port);

ViStatus _VI_FUNC agb220xa_groundChanCard  (ViSession vi, ViInt16 disable_enable, ViInt16 ground_cardno);

ViStatus _VI_FUNC agb220xa_groundChanList  (ViSession vi, ViInt16 groundchan_disen, ViInt32 _VI_FAR groundchan_list[]);

ViStatus _VI_FUNC agb220xa_groundChanList_Q  (ViSession vi, ViInt16 gndchan_disen, ViInt32 _VI_FAR gndchan_list[], ViInt32 _VI_FAR gndchan_status[]);

ViStatus _VI_FUNC agb220xa_groundState  (ViSession vi, ViInt16 groundstate_cardno, ViInt16 state);

ViStatus _VI_FUNC agb220xa_unusedPort  (ViSession vi, ViInt16 usedport_cardno, ViInt32 _VI_FAR unusedport_list[]);

ViStatus _VI_FUNC agb220xa_detectCouplePort  (ViSession vi);

/* Used for "C" externs in C++ */
#if defined(__cplusplus) || defined(__cplusplus__)
}    /* end of "C" externs for C++ */
#endif 

#endif /* agb220xa_INCLUDE */

