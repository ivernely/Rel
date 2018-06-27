/*****************************************************************************/
/*  agb220xa.cpp                                                               */
/*  Copyright (C) 2004 Agilent Technologies                                  */
/*---------------------------------------------------------------------------*/
/* Driver for agb220xa, $instrument desc$                                    */
/* Driver Version: A.02.00                                                   */
/*---------------------------------------------------------------------------*/
/* This driver is compatible with the following VXIplug&play standards:      */
/*    WIN32 System Framework revision 3.0                                    */
/*    VTL revision 3.0 & VISA revision 1.0                                   */
/*****************************************************************************/

#define agb220xa_REV_CODE    "A.02.00"                 	  /* Driver Revision */

/*****************************************************************************/
/* Exactly one of the following must be defined for the program to           */
/*   compile correctly.                                                      */
/*                                                                           */
/* #define __hpux          (for compilation for HP-UX)                       */
/* #define WIN16           (for compilation for 16-bit Windows)              */
/* #define WIN32           (for compilation for 32-bit Windows)              */
/*                                                                           */
/* In addition, if WIN32 is defined, _CVI_ may also be defined to indicate   */
/*   that the program is being compiled under LabWindows CVI 4.0 or          */
/*   greater.                                                                */
/*****************************************************************************/

/*****************************************************************************/
/* The following defines are intended to provide OS compatibility among      */
/*   Windows 16-bit, Windows 32-bit, and HP-UX C compilers.                  */
/*****************************************************************************/

#ifdef __hpux
#define _fcvt    fcvt
#endif

#ifdef WIN16
#else
#define _fmemcpy memcpy
#define _huge
#endif

/*****************************************************************************/
/* The following define specifies the use of viWrite and viRead rather       */
/*   than viPrintf and viScanf.                                              */
/* NO_FORMAT_IO is preferred in the WIN16 environment.  In many WIN16        */
/*   implementations of VISA viPrintf and viScanf do not handle blocks       */
/*   of over 32K bytes well.  Formatted IO is preferred for WIN32 and        */
/*   HP-UX.                                                                  */
/*****************************************************************************/

/* #define NO_FORMAT_IO 													 */

/*****************************************************************************/

#include <stdlib.h>          /* prototype for malloc()  */
#include <string.h>          /* prototype for strcpy()  */
#include <stdio.h>           /* prototype for sprintf() */
#include <math.h>            /* prototype for pow()     */

#ifdef __hpux
#else
#include <windows.h>
#endif

#ifdef WIN32
#include <winbase.h>
#else
#endif

#include "visa.h"
#include "agb220xa.h"

#define agb220xa_ERR_MSG_LENGTH 256   	/* size of error message buffer      */
#define ZERO_DELAY          0 	        /* Number of milliseconds to delay.  */
                                        /* See doDelay function.             */

/* #define DEBUG_OUTPUT */
#ifdef DEBUG_OUTPUT
FILE *fp;
#endif /* DEBUG_OUTPUT */

/*****************************************************************************/
/*  The string below is used to do the instrument identification in the      */
/*    init routine.  The string matches the first part of the instrument's   */
/*    response to it's *IDN? command.                                        */
/*****************************************************************************/
/*  DEVELOPER:                                                               */
/*  This string should match the instrument's model, but should not attempt  */
/*    to match firmware revision as a rule.  For instance, suppose the       */
/*    instrument responds to *IDN? with                                      */
/*          HEWLETT-PACKARD, 83475B, rev A.01.14                             */
/*    The appropriate IDN_STRING would be                                    */
/*          HEWLETT-PACKARD, 83475                                           */
/*    or                                                                     */
/*          HEWLETT-PACKARD, 83475B                                          */
/*    depending on how similar letter revisions of the instrument are.       */
/*****************************************************************************/

#define agb2200a_IDN_STRING  "AGILENT,B2200A"
#define agb2201a_IDN_STRING  "AGILENT,B2201A"

/*****************************************************************************/
/*  Driver Message Strings.                                                  */
/*****************************************************************************/
/*  DEVELOPER:                                                               */
/*  Add instrument or instrument driver specific message to the bottom of    */
/*  list.                                                                    */
/*****************************************************************************/

#define agb220xa_MSG_VI_OPEN_ERR            "vi was zero.  Was the agb220xa_init() successful?"
#define agb220xa_MSG_CONDITION              "condition"
#define agb220xa_MSG_EVENT                  "event"	
#define agb220xa_MSG_EVENT_HDLR_INSTALLED   "event handler is already installed for event happening"
#define agb220xa_MSG_EVENT_HDLR_INST2       "Only 1 handler can be installed at a time."	
#define agb220xa_MSG_INVALID_HAPPENING      "is not a valid happening."
#define agb220xa_MSG_NOT_QUERIABLE          "is not queriable."	
#define agb220xa_MSG_IN_FUNCTION            "in function" 		
#define agb220xa_MSG_INVALID_STATUS         "Parameter 2 is invalid in function agb220xa_error_message()."
#define agb220xa_MSG_INVALID_STATUS_VALUE   "is not a valid viStatus value."
#define agb220xa_MSG_INVALID_VI             "Parameter 1 is invalid in function agb220xa_error_message()."  \
                                          "  Using an inactive ViSession may cause this error."	\
                                          "  Was the instrument driver closed prematurely?"
#define agb220xa_MSG_NO_ERRORS              "No Errors"
#define agb220xa_MSG_SELF_TEST_FAILED       "Self test failed." 	
#define agb220xa_MSG_SELF_TEST_PASSED       "Self test passed."

#define agb220xa_MSG_BOOLEAN                "Expected 0 or 1; Got %d"
#define agb220xa_MSG_REAL                   "Expected %lg to %lg; Got %lg"
#define agb220xa_MSG_INT                    "Expected %hd to %hd; Got %hd"
#define agb220xa_MSG_LONG                   "Expected %ld to %ld; Got %ld"
#define agb220xa_MSG_LOOKUP                 "Error converting string response to integer"
#define agb220xa_MSG_NO_MATCH               "Could not match string %s"

#define VI_ERROR_PARAMETER1_MSG           "Parameter 1 is invalid"
#define VI_ERROR_PARAMETER2_MSG           "Parameter 2 is invalid"
#define VI_ERROR_PARAMETER3_MSG           "Parameter 3 is invalid"
#define VI_ERROR_PARAMETER4_MSG           "Parameter 4 is invalid"
#define VI_ERROR_PARAMETER5_MSG           "Parameter 5 is invalid"
#define VI_ERROR_PARAMETER6_MSG           "Parameter 6 is invalid"
#define VI_ERROR_PARAMETER7_MSG           "Parameter 7 is invalid"
#define VI_ERROR_PARAMETER8_MSG           "Parameter 8 is invalid"
#define VI_ERROR_PARAMETER9_MSG           "Parameter 9 is invalid"
#define VI_ERROR_PARAMETER10_MSG          "Parameter 10 is invalid"
#define VI_ERROR_PARAMETER11_MSG          "Parameter 11 is invalid"
#define VI_ERROR_PARAMETER12_MSG          "Parameter 12 is invalid"
#define VI_ERROR_PARAMETER13_MSG          "Parameter 13 is invalid"
#define VI_ERROR_PARAMETER14_MSG          "Parameter 14 is invalid"
#define VI_ERROR_PARAMETER15_MSG          "Parameter 15 is invalid"
#define VI_ERROR_PARAMETER16_MSG          "Parameter 16 is invalid"
#define VI_ERROR_PARAMETER17_MSG          "Parameter 17 is invalid"
#define VI_ERROR_PARAMETER18_MSG          "Parameter 18 is invalid"
#define VI_ERROR_FAIL_ID_QUERY_MSG        "Instrument IDN does not match."

#define INSTR_ERROR_INV_SESSION_MSG       "ViSession (parameter 1) was not created by this driver"
#define INSTR_ERROR_NULL_PTR_MSG          "NULL pointer detected"
#define INSTR_ERROR_RESET_FAILED_MSG      "Reset failed"
#define INSTR_ERROR_UNEXPECTED_MSG        "An unexpected error occurred"
#define INSTR_ERROR_DETECTED_MSG          "Instrument Error Detected, call agb220xa_error_query()."
#define INSTR_ERROR_LOOKUP_MSG            "String not found in table"

#define FILE_OPEN_ERROR_MSG               "File I/O error - could not open specified file"
#define FILE_READ_ERROR_MSG               "File I/O error - could not read from file"
#define FILE_WRITE_ERROR_MSG              "File I/O error - could not write to file"
#define FILE_REV_ERROR_MSG                "File I/O error - file revision is wrong"
#define FILE_FORMAT_ERROR_MSG             "File I/O error - file format is wrong"

/*****************************************************************************/
/*  Error table structure.                                                   */
/*    The first element of the table is the error number, the second is the  */
/*    error message.  Error numbers in the "VISA Generic" section of the     */
/*    table are defined in the VISA header files.  Error numbers in the      */
/*    "Instrument Specific" section are defined in the driver header file.   */
/*    All of the error messages are defined above.                           */
/*****************************************************************************/
/*  DEVELOPER:                                                               */
/*  Add instrument or instrument driver specific numbers to the driver       */
/*    header file.                                                           */
/*  Add instrument or instrument driver specific messages to the bottom of   */
/*    messages listed above.                                                 */
/*  Add instrument specific table entries to the "Instrument Specific"       */
/*    section of the table below.                                            */
/*****************************************************************************/

struct instrErrStruct
{
	ViStatus errStatus;
	ViString errMessage;
};

const static struct instrErrStruct instrErrMsgTable[] =
{
        /**************************/
        /*  VISA Generic          */
        /**************************/

    { VI_ERROR_PARAMETER1,                VI_ERROR_PARAMETER1_MSG 		},
    { VI_ERROR_PARAMETER2,                VI_ERROR_PARAMETER2_MSG 		},
    { VI_ERROR_PARAMETER3,                VI_ERROR_PARAMETER3_MSG 		},
    { VI_ERROR_PARAMETER4,                VI_ERROR_PARAMETER4_MSG 		},
    { VI_ERROR_PARAMETER5,                VI_ERROR_PARAMETER5_MSG 		},
    { VI_ERROR_PARAMETER6,                VI_ERROR_PARAMETER6_MSG 		},
    { VI_ERROR_PARAMETER7,                VI_ERROR_PARAMETER7_MSG 		},
    { VI_ERROR_PARAMETER8,                VI_ERROR_PARAMETER8_MSG 		},
    { agb220xa_INSTR_ERROR_PARAMETER9,    VI_ERROR_PARAMETER9_MSG 		},
    { agb220xa_INSTR_ERROR_PARAMETER10,     VI_ERROR_PARAMETER10_MSG 		},
    { agb220xa_INSTR_ERROR_PARAMETER11,     VI_ERROR_PARAMETER11_MSG 		},
    { agb220xa_INSTR_ERROR_PARAMETER12,     VI_ERROR_PARAMETER12_MSG 		},
    { agb220xa_INSTR_ERROR_PARAMETER13,     VI_ERROR_PARAMETER13_MSG 		},
    { agb220xa_INSTR_ERROR_PARAMETER14,     VI_ERROR_PARAMETER14_MSG 		},
    { agb220xa_INSTR_ERROR_PARAMETER15,     VI_ERROR_PARAMETER15_MSG 		},
    { agb220xa_INSTR_ERROR_PARAMETER16,     VI_ERROR_PARAMETER16_MSG 		},
    { agb220xa_INSTR_ERROR_PARAMETER17,     VI_ERROR_PARAMETER17_MSG 		},
    { agb220xa_INSTR_ERROR_PARAMETER18,     VI_ERROR_PARAMETER18_MSG 		},
    { VI_ERROR_FAIL_ID_QUERY,             VI_ERROR_FAIL_ID_QUERY_MSG 	},
    { agb220xa_INSTR_ERROR_INV_SESSION,     INSTR_ERROR_INV_SESSION_MSG 	},
    { agb220xa_INSTR_ERROR_NULL_PTR,        INSTR_ERROR_NULL_PTR_MSG 		},
    { agb220xa_INSTR_ERROR_RESET_FAILED,    INSTR_ERROR_RESET_FAILED_MSG 	},
    { agb220xa_INSTR_ERROR_UNEXPECTED,      INSTR_ERROR_UNEXPECTED_MSG 		},
    { agb220xa_INSTR_ERROR_DETECTED,        INSTR_ERROR_DETECTED_MSG 		},
    { agb220xa_INSTR_ERROR_LOOKUP,          INSTR_ERROR_LOOKUP_MSG 			},

        /**************************/
        /*  Instrument Specific   */
        /**************************/

	{ agb220xa_FILE_OPEN_ERROR,		FILE_OPEN_ERROR_MSG 			},
	{ agb220xa_FILE_READ_ERROR,		FILE_READ_ERROR_MSG 			},
	{ agb220xa_FILE_WRITE_ERROR,    	FILE_WRITE_ERROR_MSG 			},
	{ agb220xa_FILE_REV_ERROR,              FILE_REV_ERROR_MSG              },
	{ agb220xa_FILE_FORMAT_ERROR,           FILE_FORMAT_ERROR_MSG           }

};

/**************************/
/* Default C Compen Table */
/**************************/

class CComp{
public:
    ViReal64 r,i;

    CComp(void){r = 0; i = 0;}
    CComp(ViReal64 a, ViReal64 b){r = a; i = b;}
    CComp(ViReal64 a){r = a; i = 0;}
    CComp operator * (CComp f){
        CComp temp;
        temp.r = r * f.r - i * f.i;
        temp.i = i * f.r + r * f.i;
        return(temp);
    }
    void operator = (CComp f)
    {
        r = f.r;
        i = f.i;
    }
    CComp operator * (ViReal64 f)
    {
        CComp temp;
        temp.r = r * f;
        temp.i = i * f;
        return(temp);
    }
    CComp operator / (ViReal64 f)
    {
        CComp temp;
        temp.r = r / f;
        temp.i = i / f;
        return(temp);
    }
    CComp operator + (CComp f)
    {
        CComp temp;
        temp.r = r + f.r;
        temp.i = i + f.i;
        return(temp);
    }
    CComp operator - (CComp f)
    {
        CComp temp;
        temp.r = r - f.r;
        temp.i = i - f.i;
        return(temp);
    }
    friend ViReal64 abs2(CComp f)
    {
        return(f.r * f.r + f.i * f.i);
    }
    friend CComp conju(CComp f)
    {
        return(CComp(f.r , -f.i));
    }
    CComp operator / (CComp f)
    {
        return(*this * conju(f) / abs2(f));
    }
};

class C2x2{
public:
    CComp a,b,c,d;
    C2x2 operator * (C2x2 f)
    {
        C2x2 temp;
        temp.a = a * f.a + b * f.c;
        temp.b = a * f.b + b * f.d;
        temp.c = c * f.a + d * f.c;
        temp.d = c * f.b + d * f.d;
        return(temp);
    }
    void operator = (C2x2 f)
    {
        a = f.a;
        b = f.b;
        c = f.c;
        d = f.d;
    }
    C2x2 operator + (C2x2 f)
    {
        C2x2 temp;
        temp.a = a + f.a;
        temp.b = b + f.b;
        temp.c = c + f.c;
        temp.d = d + f.d;
        return(temp);
    }
    C2x2 operator - (C2x2 f)
    {
        C2x2 temp;
        temp.a = a - f.a;
        temp.b = b - f.b;
        temp.c = c - f.c;
        temp.d = d - f.d;
        return(temp);
    }
    C2x2 operator * (CComp f)
    {
        C2x2 temp;
        temp.a = a * f;
        temp.b = b * f;
        temp.c = c * f;
        temp.d = d * f;
        return(temp);
    }
    C2x2 operator * (ViReal64 f)
    {
        C2x2 temp;
        temp.a = a * f;
        temp.b = b * f;
        temp.c = c * f;
        temp.d = d * f;
        return(temp);
    }
#ifdef DEBUG_OUTPUT
    void print(char *title, FILE *fp)
    {
        fprintf(fp, "%s(0, 0) = %le+(%le)j\n", title, a.r, a.i);
        fprintf(fp, "%s(0, 1) = %le+(%le)j\n", title, b.r, b.i);
        fprintf(fp, "%s(1, 0) = %le+(%le)j\n", title, c.r, c.i);
        fprintf(fp, "%s(1, 1) = %le+(%le)j\n", title, d.r, d.i);
    }
#endif /* DEBUG_OUTPUT */
};

/*****************************************************************************/
/*  VISA Globals data structure.                                             */
/*  It is desireable to store global variables in VISA rather than the       */
/*    driver.  This avoids having any state variables in the driver.  This   */
/*    structure defines the driver's global data.  Memory is allocated to    */
/*    this structure in the init function, which passes the pointer to VISA, */
/*    and every function which follows retrieves a pointer to it.            */
/*****************************************************************************/
/*  DEVELOPER:                                                               */
/*  Add additional globals under the "Instrument Specific" section.          */
/*  In driver functions,  The globals can then be refered to via the         */
/*  thisPtr pointer to the globals which is retrieved by the GET_GLOBALS     */
/*  macro.  GET_GLOBALS must be executed in each driver function before      */
/*  any references to thisPtr are made.                                      */
/*****************************************************************************/

typedef struct _Zpath {
    ViReal64    R;              // R[ohm]
    ViReal64    L;              // L[H]
    ViReal64    C;              // C[F]
} Zpath;

typedef enum _CardType {
    CARD_INVALID = -1,
    CARD_B2210A  = 0,
    CARD_B2211A  = 1
} CardType;

struct agb220xa_globals
{
    ViSession    defRMSession;

    ViStatus     errNumber;

    ViChar       errFuncName[40];
    ViChar       errMessage[160];

    ViBoolean    errQueryDetect;

    /*****************************************************/
    /* Setting this to VI_TRUE,                          */
    /* will prevent I/O from occuring in an SRQ		 */
    /* This is needed, because VTL 3.0 doesn't provide	 */
    /* atomic write/read operations.  Don't want	 */
    /* to do I/O if the driver is in the middle of	 */
    /* a read from the instrument.			 */
    /*****************************************************/
    ViBoolean    blockSrqIO;
    ViInt32      countSrqIO;

    ViInt32      myDelay;

    ViUInt16     interfaceType;
        
    /**************************/
    /*  Instrument Specific   */
    /**************************/
    Zpath       mother_b;       // input cable & mother board 
    Zpath       matrix_h;       // CMH path of matrix module
    Zpath       matrix_l;       // CML path of matrix module
    Zpath       cable1;         // Triax cable
    Zpath       cable2_h;       // CMH of Probe card I/F or user triax cable
    Zpath       cable2_l;       // CML of Probe card I/F or user triax cable
    Zpath       cable3_h;       // CMH of Probe card or user coax cable
    Zpath       cable3_l;       // CML of Probe card or user coax cable

    ViBoolean   pcif;
    CardType    card_type;

};
                                     /*           R         L         C */
static const Zpath def_mother_board   = {   0.0E+00, 5.25E-08, 2.94E-11 };
static const Zpath def_matrix_h_2210A = {  2.43E+00, 6.31E-07, 1.93E-10 };
static const Zpath def_matrix_l_2210A = {  2.49E+00, 5.87E-07, 1.92E-10 };
static const Zpath def_matrix_h_2211A = {  2.38E+00, 7.29E-07, 2.02E-10 };
static const Zpath def_matrix_l_2211A = {  2.42E+00, 6.95E-07, 2.01E-10 };
static const Zpath def_cable1         = {  6.30E-01, 1.25E-06, 1.60E-10 };
static const Zpath def_cable_2        = { 2.988E-01, 1.25E-06, 1.60E-10 };
static const Zpath def_cable_3        = {   0.0E+00, 9.00E-08, 1.50E-13 };

C2x2 FMatrix(ViReal64 R, ViReal64 L, ViReal64 C, ViReal64 w)
{
	C2x2 temp; 
	CComp t1(R, w * L), t2(0, w * C);

	temp.a = CComp(1 , 0) + t1 * t2 / 2;
	temp.b = t1 * t1 * t2 / (-4) - t1;
	temp.c = t2 * (-1);
	temp.d = temp.a;
	return(temp);
}

/*****************************************************************************/
/*****************************************************************************/
/********************                                     ********************/
/**********                                                         **********/
/****                   SUPPORTING MACROS AND FUNCTIONS                   ****/
/**                                                                         **/
/**-------------------------------------------------------------------------**/
/**     These macros and functions are not exposed to the user, but         **/
/**     are used by other functions in the driver.                          **/
/****                                                                     ****/
/**********                                                         **********/
/********************                                     ********************/
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
/*  DEBUG MACRO                                                              */
/*****************************************************************************/

							  /* don't check the debug pointer all the time! */
#ifdef DEBUG
#define agb220xa_DEBUG_CHK_THIS( vi, thisPtr) 									\
	/* check for NULL user data */											\
	if( 0 == thisPtr)														\
	{																		\
 		agb220xa_LOG_STATUS(vi, NULL, agb220xa_INSTR_ERROR_INV_SESSION );			\
	}																		\
	{																		\
		ViSession defRM;													\
																			\
		/* This should never fail */										\
		errStatus = viGetAttribute( vi, VI_ATTR_RM_SESSION, &defRM);		\
		if( VI_SUCCESS > errStatus )										\
		{																	\
 			agb220xa_LOG_STATUS(	vi, NULL, agb220xa_INSTR_ERROR_UNEXPECTED );	\
		}																	\
		if( defRM != thisPtr->defRMSession )								\
		{																	\
 			agb220xa_LOG_STATUS(vi, NULL, agb220xa_INSTR_ERROR_INV_SESSION ); 		\
		}																	\
	}
#else
#define agb220xa_DEBUG_CHK_THIS( vi, thisPtr) 	
#endif

/*****************************************************************************/
/*  CONTEXT DEPENDENT ERROR MESSAGE                                          */
/*****************************************************************************/
#define WANT_CDE_INIT
#ifdef WANT_CDE_INIT
#define agb220xa_CDE_INIT( funcname )	  										\
	strcpy(thisPtr->errFuncName, funcname );								\
	thisPtr->errNumber = VI_SUCCESS;										\
	thisPtr->errMessage[0] = 0;

#define agb220xa_CDE_MESSAGE( message ) 	 									\
	strcpy(thisPtr->errMessage, message )
#else
#define agb220xa_CDE_INIT( funcname ) 											\
	thisPtr->errNumber = VI_SUCCESS;										\

#define agb220xa_CDE_MESSAGE( message ) 	 									\
	strcpy(thisPtr->errMessage, message )
#endif

/*****************************************************************************/
/*  SWAP MACROS                                                              */
/*****************************************************************************/
/*  Reorders bytes where instrument byte ordering of integers and reals      */
/*    differ from the computing framework.                                   */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    dest                                                                   */
/*      The number to be reordered.  The reordered result is also stored in  */
/*      this variable on completion.                                         */
/*****************************************************************************/
/*  DEVELOPER:                                                               */
/*  You can remove the SWAP macros if you do not have block IO (arbitrary    */
/*  block) if you do then you must also remove the routines that use them    */
/*  (_cmd_arr... functions)                                                  */
/*****************************************************************************/

#define SWAP_FLOAT64(dest)                                         \
 { unsigned char    src[8];                                        \
      *((double *)src) = *((double *)dest);                        \
      ((unsigned char *)(dest))[0] =  ((unsigned char*)(src))[7];  \
      ((unsigned char *)(dest))[1] =  ((unsigned char*)(src))[6];  \
      ((unsigned char *)(dest))[2] =  ((unsigned char*)(src))[5];  \
      ((unsigned char *)(dest))[3] =  ((unsigned char*)(src))[4];  \
      ((unsigned char *)(dest))[4] =  ((unsigned char*)(src))[3];  \
      ((unsigned char *)(dest))[5] =  ((unsigned char*)(src))[2];  \
      ((unsigned char *)(dest))[6] =  ((unsigned char*)(src))[1];  \
      ((unsigned char *)(dest))[7] =  ((unsigned char*)(src))[0];  \
 }

#define SWAP_FLOAT32(dest)                                         \
 { unsigned char    src[4];                                        \
      *((float *)src) = *((float *)dest);                          \
      ((unsigned char *)(dest))[0] =  ((unsigned char*)(src))[3];  \
      ((unsigned char *)(dest))[1] =  ((unsigned char*)(src))[2];  \
      ((unsigned char *)(dest))[2] =  ((unsigned char*)(src))[1];  \
      ((unsigned char *)(dest))[3] =  ((unsigned char*)(src))[0];  \
 }

#define SWAP_32(dest)                                              \
 { unsigned char    src[4];                                        \
      *((long *)src) = *((long *)dest);                            \
      ((unsigned char *)(dest))[0] =  ((unsigned char*)(src))[3];  \
      ((unsigned char *)(dest))[1] =  ((unsigned char*)(src))[2];  \
      ((unsigned char *)(dest))[2] =  ((unsigned char*)(src))[1];  \
      ((unsigned char *)(dest))[3] =  ((unsigned char*)(src))[0];  \
 }

#define SWAP_16(dest)                                              \
 { unsigned char    src[2];                                        \
      *((int *)src) = *((int *)dest);                              \
      ((unsigned char *)(dest))[0] =  ((unsigned char*)(src))[1];  \
      ((unsigned char *)(dest))[1] =  ((unsigned char*)(src))[0];  \
 }


/*****************************************************************************/
/*  MACRO GET_GLOBALS                                                        */
/*****************************************************************************/
/*  Returns a pointer to the VISA globals storage.  This storage was         */
/*    allocated in init, but it is turned over to VISA to avoid holding      */
/*    state variables in the driver.                                         */
/*  If DEBUG is defined, thisPtr is checked for validity.                    */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession            InstrDesc                                         */
/*      The instrument's VISA address.                                       */
/*    struct agb220xa_globals  *thisPtr                                      */
/*      A pointer to the VISA globals storage                                */
/*****************************************************************************/

#define GET_GLOBALS(vi,thisPtr)                                                \
{                                                                              \
    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);      \
    if (errStatus < VI_SUCCESS)                                                \
        return statusUpdate(vi, NULL, errStatus);                              \
}


/*****************************************************************************/
/*  MACRO CDE_INIT                                                           */
/*****************************************************************************/
/*  Defines context dependent errors                                         */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    funcname                                                               */
/*      The name of the function to be stored with the context dependent     */
/*      error information.                                                   */
/*****************************************************************************/

#define CDE_INIT(funcname)                          \
    strcpy(thisPtr->errFuncName, funcname);         \
    thisPtr->errNumber = VI_SUCCESS;                \
    thisPtr->errMessage[0] = 0;


/*****************************************************************************/
/*  DELAY Functions                                                          */
/*  setDelay (located prior to init function) sets the delay time for the    */
/*    instrument.                                                            */
/*  doDelay uses delay time to slow down the driver after a read request.    */
/*---------------------------------------------------------------------------*/
/*  In some SCPI instruments, the instrument cannot change its state to      */
/*    'processing a read request' fast enough to keep another request        */
/*    from getting through to the instrument.  By setting a short delay      */
/*    (less than 100 microseconds should be sufficient) after each read,     */
/*    the driver will wait a sufficient amount of time before attempting     */
/*    to send the next string to the instrument.                             */
/*  DO NOT document these function in the fp file or the help file.  They    */
/*    are intended to address problems in specific instruments, problems     */
/*    that may only appear on faster CPU's.  They are designed so that the   */
/*    problem can be addressed quickly in the field once it is identified.   */
/*  If the problem is identified by driver developers, a 100 ms delay        */
/*    should be set in the agb220xa_init function.  The template version of  */
/*    init currently sets the delay to 0.                                    */
/*****************************************************************************/

/*****************************************************************************/
void doDelay(
        ViInt32 delayTime)
/*****************************************************************************/
/*  Delays execution for a specified number of microseconds.                 */
/*  Platform   Code    Tick Resolution  Tick Increment  Call Overhead        */
/*   WIN       16-bit  1 ms             ~55 ms          0-55 ms              */
/*   WIN32     16-bit  1 ms             ~55 ms          0-55 ms              */
/*   WIN32     32-bit  CPU dependent    CPU dependent   0-20 us              */
/*   HP-UX     32-bit  1 us             CPU dependent   0-20 us              */
/*  PARAMETERS                                                               */
/*    long           delayTime (in)                                          */
/*      The number of micro-seconds to delay before proceeding.              */
/*****************************************************************************/
{
    long   ticksToCount;

    /*************************************************************/
    /*  WIN32 Version                                            */
    /*  WIN32 increments depend on the hardware.  If high-       */
    /*    frequency counting isn't supported, revert to common   */
    /*    standard functions (same as WIN16).                    */
    /*  The call overhead to execute doDelay if high-frequency   */
    /*    is supported is about 25-30 micro-seconds.             */
    /*  The call overhead to execute doDelay if high-frequency   */
    /*    is -NOT- supported is ~10 milli-seconds on             */
    /*    Windows 95 & NT.                                       */
    /*************************************************************/
#ifdef WIN32

#ifdef _CVI_
#else
    __int64  ticksPerSecond;
    __int64  startTicks;
    __int64  nowTicks;
#endif
    DWORD    msTicks;

    if (delayTime == 0)
        return;

#ifdef _CVI_
#else
    if (QueryPerformanceFrequency((LARGE_INTEGER *)&ticksPerSecond))
    {
        ticksToCount = (long)((ticksPerSecond/1E6)*delayTime) + 1;
        QueryPerformanceCounter((LARGE_INTEGER *)&nowTicks);
        startTicks = nowTicks;
        while (nowTicks - startTicks < (DWORD)ticksToCount)
            QueryPerformanceCounter((LARGE_INTEGER *)&nowTicks);
    }
    else
#endif
    {
        ticksToCount = (long)((1E3/1E6)*delayTime) + 1;
        msTicks = GetTickCount();
        while (GetTickCount() - msTicks < (DWORD)ticksToCount) ;
    }

#endif

    /*************************************************************/
    /*  WIN16 Version                                            */
    /*  WIN16 counts in 1 milli-second increments.               */
    /*  The call overhead to execute doDelay if high-frequency   */
    /*    is -NOT- supported is ~55 milli-seconds on Windows     */
    /*    3.1 and ~10 milli-seconds on Windows 95 & NT.          */
    /*************************************************************/
#ifdef WIN16

    DWORD    msTicks;

    if (delayTime == 0)
        return;

    ticksToCount = (long)((1E3/1E6)*delayTime) + 1;
    msTicks = GetTickCount();
    while (GetTickCount() - msTicks < (DWORD)ticksToCount);
    
#endif

    /*************************************************************/
    /*  HP-UX Version                                            */
    /*  HP-UX counts in 1 micro-second increments.               */
    /*  The call overhead to execute doDelay on HP-UX is about   */
    /*    15-20 micro-seconds.                                   */
    /*************************************************************/
#ifdef __hpux

    struct timeval t0, t1;

    if (delayTime == 0)
        return;
    /* printf("doDelay delaying %ld\n", ticks); */ 
    gettimeofday(&t0, NULL);
    t0.tv_usec += delayTime;
    t0.tv_sec += t0.tv_usec / 1000000;
    t0.tv_usec = t0.tv_usec % 1000000;

    gettimeofday(&t1, NULL);
    while (t1.tv_sec < t0.tv_sec)
        gettimeofday(&t1, NULL);

    if (t1.tv_sec > t0.tv_sec) return; 		 /* get out quick if past delay time */
    
    while (t1.tv_usec < t0.tv_usec)
        gettimeofday(&t1, NULL);

#endif

    return;
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus delayScanf(
                    ViSession  vi,
                    ViInt32    delayTime,
                    ViString   readFmt,
                    ... ) 
/*****************************************************************************/
/*  Same as viVscanf, but with the delay added                               */
/*                                                                           */
/*  PARAMETERS (same as viVscanf, except added delayTime)                    */
/*    ViSession   InstrDesc (in)                                             */
/*    ViInt32     delayTime (in)                                             */
/*    ViString    readFmt (in)                                               */
/*    ...                                                                    */
/*****************************************************************************/

{
    ViStatus  errStatus;
    va_list   va;

    va_start(va, readFmt);
    errStatus = viVScanf(vi, readFmt, va);
    va_end(va);

    doDelay(delayTime);

    return errStatus;
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus delayRead(
                   ViSession  vi,
                   ViInt32    delayTime,
                   ViPBuf     buf,
                   ViUInt32   cnt,
                   ViPUInt32  retCnt) 
/*****************************************************************************/
/*  Same as viRead, but with the delay added                                 */
/*                                                                           */
/*  PARAMETERS (same as viRead, except added delayTime)                      */
/*    ViSession   InstrDesc (in)                                             */
/*    ViInt32     delayTime (in)                                             */
/*    ...                                                                    */
/*****************************************************************************/

{
    ViStatus  errStatus;

    errStatus = viRead(vi, buf, cnt, retCnt);

    doDelay(delayTime);

    return errStatus;
}        /* ----- end of function ----- */

#define agb220xa_LOG_STATUS( vi, thisPtr, status )		 	\
  return statusUpdate( vi, thisPtr, status )  

/*****************************************************************************/
ViStatus statusUpdate(
                      ViSession  vi,
                      struct     agb220xa_globals *thisPtr,
                      ViStatus   s ) 
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession            InstrDesc (in)                                    */
/*    struct agb220xa_globals *thisPtr (in)                                    */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViStatus             s (in)                                            */
/*      A driver error return code.                                          */
/*****************************************************************************/

{   
    ViUInt32  rc;         
    ViStatus  errStatus;
    ViChar    lc[20];
    ViInt16   eventQ;

    if (thisPtr)
        thisPtr->errNumber = s;

    /*************************************************************/
    /*  If error query detect is set on and the instrument       */
    /*    has not timed out, then ...                            */
    /*  Read the status byte into eventQ and check the error     */
    /*    bytes.                                                 */
    /*  Potential status byte errors are 0x04 (Query Error),     */
    /*    0x08 (Device Dependent Error), 0x10 (Execution Error), */
    /*    and 0x20 (Command Error).                              */
    /*************************************************************/

    if (thisPtr && thisPtr->errQueryDetect && s != VI_ERROR_TMO)
    {   
        errStatus = viWrite(vi,(ViBuf)"*ESR?",5,&rc);
        if (errStatus < VI_SUCCESS)
            return VI_ERROR_SYSTEM_ERROR;
            
        if ((errStatus = delayRead(vi, thisPtr->myDelay,
                                   (ViPBuf)lc, 20, &rc)) < VI_SUCCESS)
            return VI_ERROR_SYSTEM_ERROR;
            
        lc[rc]='\0';
        eventQ=atoi(lc);
        
        if( (0x04  | 0x08  | 0x10  | 0x20) & eventQ )
            return agb220xa_INSTR_ERROR_DETECTED;
    }

    return s;
}        /* ----- end of function ----- */


/*****************************************************************************/
/*  MACRO agb220xa_CHK_BOOLEAN                                               */
/*****************************************************************************/
/*  Ref agb220xa_chk_boolean and statusUpdate for info.                      */
/*****************************************************************************/

#define agb220xa_CHK_BOOLEAN(my_val, err)                 \
if (agb220xa_chk_boolean(thisPtr, my_val))                \
    return statusUpdate(vi, thisPtr, err);


/*****************************************************************************/
static ViBoolean agb220xa_chk_boolean(
                                      struct    	agb220xa_globals *thisPtr,
                                      ViBoolean  	my_val)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    struct agb220xa_globals *thisPtr (in)                                  */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViBoolean my_val (in)                                                  */
/*      The value to be checked.                                             */
/*****************************************************************************/

{
    ViChar message[agb220xa_ERR_MSG_LENGTH];

    if ( (my_val != VI_TRUE) && (my_val != VI_FALSE) )
        {
            sprintf(message, agb220xa_MSG_BOOLEAN, my_val);	
            strcpy(thisPtr->errMessage, message);				
            return VI_TRUE;
        }

    return VI_FALSE;
}        /* ----- end of function ----- */


/*****************************************************************************/
/*  MACRO agb220xa_CHK_REAL_RANGE                                              */
/*****************************************************************************/
/*  Ref agb220xa_chk_real_range and statusUpdate for info.                 	 */
/*****************************************************************************/

#define agb220xa_CHK_REAL_RANGE(my_val, min, max, err)        \
if (agb220xa_chk_real_range(thisPtr, my_val, min, max))   	\
    return statusUpdate(vi, thisPtr, err);


/*****************************************************************************/
static ViBoolean agb220xa_chk_real_range( 
                                         struct    agb220xa_globals *thisPtr,
                                         ViReal64  my_val,
                                         ViReal64  min,
                                         ViReal64  max)
/*****************************************************************************/
/*  Tests to see if a ViReal64 is in range.                                  */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    struct agb220xa_globals *thisPtr (in)                                  */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViReal64             my_val (in)                                       */
/*      The value to be checked.                                             */
/*    ViReal64             min (in)                                          */
/*      The bottom of the range.                                             */
/*    ViReal64             max (in)                                          */
/*      The top of the range.                                                */
/*****************************************************************************/

{
    ViChar message[agb220xa_ERR_MSG_LENGTH];

    if ( (my_val < min) || (my_val > max) )  			
        {								
            sprintf(message, agb220xa_MSG_REAL, min, max, my_val);	
            strcpy(thisPtr->errMessage, message);
            return VI_TRUE;
        }

    return VI_FALSE;
}        /* ----- end of function ----- */ 
 
  
/*****************************************************************************/
/*  MACRO agb220xa_CHK_INT_RANGE                                               */
/*****************************************************************************/
/*  Ref agb220xa_chk_int_range and statusUpdate for info.                    */
/*****************************************************************************/

#define agb220xa_CHK_INT_RANGE(my_val, min, max, err)       \
if(agb220xa_chk_int_range(thisPtr, my_val, min, max) )      \
    return statusUpdate(vi, thisPtr, err);


/*****************************************************************************/
static ViBoolean agb220xa_chk_int_range( 
                                        struct   agb220xa_globals *thisPtr,
                                        ViInt16  my_val,
                                        ViInt16  min,
                                        ViInt16  max)
/*****************************************************************************/
/*  Tests to see if a ViInt16 is in range.                                   */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    struct agb220xa_globals *thisPtr (in)                                  */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViInt16              my_val (in)                                       */
/*      The value to be checked.                                             */
/*    ViInt16              min (in)                                          */
/*      The bottom of the range.                                             */
/*    ViInt16              max (in)                                          */
/*      The top of the range.                                                */
/*****************************************************************************/

{
    ViChar message[agb220xa_ERR_MSG_LENGTH];

    if ( (my_val < min) || (my_val > max) )  			
        {								
            sprintf(message, agb220xa_MSG_INT, min, max, my_val);	
            strcpy(thisPtr->errMessage, message);
            return VI_TRUE;
        }

    return VI_FALSE;
}        /* ----- end of function ----- */ 
   
   
/*****************************************************************************/
/*  MACRO agb220xa_CHK_LONG_RANGE                                            */
/*****************************************************************************/
/*  Ref agb220xa_chk_long_range and statusUpdate for info.    	             */
/*****************************************************************************/

#define agb220xa_CHK_LONG_RANGE(my_val, min, max, err)      \
if (agb220xa_chk_long_range(thisPtr, my_val, min, max))     \
    return statusUpdate(vi, thisPtr, err);


/*****************************************************************************/
static ViBoolean agb220xa_chk_long_range( 
                                         struct  agb220xa_globals *thisPtr,
                                         ViInt32 my_val,
                                         ViInt32 min,
                                         ViInt32 max)
/*****************************************************************************/
/*  Tests to see if a ViInt32 is in range.                                   */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    struct agb220xa_globals *thisPtr (in)                                  */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViInt32              my_val (in)                                       */
/*      The value to be checked.                                             */
/*    ViInt32              min (in)                                          */
/*      The bottom of the range.                                             */
/*    ViInt32              max (in)                                          */
/*      The top of the range.                                                */
/*****************************************************************************/

{
    ViChar message[agb220xa_ERR_MSG_LENGTH];

    if ( (my_val < min) || (my_val > max) )  			
        {								
            sprintf(message, agb220xa_MSG_LONG, min, max, my_val);	
            strcpy(thisPtr->errMessage, message);
            return VI_TRUE;
        }

    return VI_FALSE;
}        /* ----- end of function ----- */ 
   
   
/*****************************************************************************/
/*  MACRO agb220xa_CHK_ENUM                                                    */
/*****************************************************************************/
/*  Ref agb220xa_chk_enum and statusUpdate for info.                       	 */
/*****************************************************************************/

#define agb220xa_CHK_ENUM( my_val, limit, err )             \
if( agb220xa_chk_enum( thisPtr, my_val, limit) )            \
    return statusUpdate( vi, thisPtr, err);


/*****************************************************************************/
static ViBoolean agb220xa_chk_enum (
                                    struct agb220xa_globals *thisPtr,
                                    ViInt16 my_val,
                                    ViInt16 limit)
/*****************************************************************************/
/* Chk_enum checks whether my_val lies between zero and limit                */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    struct agb220xa_globals *thisPtr (in)                                  */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViInt16              my_val (in)                                       */
/*      The value to be checked.                                             */
/*    ViInt16              limit (in)                                        */
/*      The top of the range.                                                */
/*****************************************************************************/

{
    ViChar message[agb220xa_ERR_MSG_LENGTH];

    if ( ( my_val < 0 ) || (my_val > limit) )  			
        {								
            sprintf(message, agb220xa_MSG_INT, 0, limit, my_val);	
            strcpy(thisPtr->errMessage, message);
            return VI_TRUE;
        }

    return VI_FALSE;
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_setDelay (
                                     ViSession  vi,
                                     ViInt32    delayTime)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession  vi (in)                                                     */
/*      Instrument Handle returned from agb220xa_init()                      */
/*    ViInt32    delayTime (in)                                              */
/*      The delayTime in microseconds.  Must be between 0 and 1,000.  In     */
/*      practice, should never be set higher than 100.                       */
/*****************************************************************************/

{
    ViStatus   errStatus;

    struct agb220xa_globals *thisPtr;

    GET_GLOBALS(vi,thisPtr)

        agb220xa_CHK_LONG_RANGE(delayTime, 0L, 1000L, VI_ERROR_PARAMETER2);

    thisPtr->myDelay = delayTime;

    return statusUpdate(vi, thisPtr, VI_SUCCESS);
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC initError(
                            ViPSession vi,
                            ViStatus   errStatus)
/*****************************************************************************/
/*  This function is only called from agb220xa_init.                         */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViPSession      vi (in)                                                */
/*      Pointer to Instrument Handle returned by viOpen.                     */
/*    ViStatus       errStatus (in)                                          */
/*      A driver error return code.                                          */
/*****************************************************************************/

{
    viClose(*vi);
    *vi=VI_NULL;
    return errStatus;
}        /* ----- end of function ----- */


/*****************************************************************************/
/*****************************************************************************/
/********************                                     ********************/
/**********                                                         **********/
/****                          DRIVER FUNCTIONS                           ****/
/**                                                                         **/
/**-------------------------------------------------------------------------**/
/**     The functions which are exposed to the driver user start here.      **/
/**     They are in the following order:                                    **/
/**        VPP required functions.                                          **/
/**        Agilent required utility functions                               **/
/**        Agilent passthrough functions                                    **/
/**        Instrument specific functions                                    **/
/****                                                                     ****/
/**********                                                         **********/
/********************                                     ********************/
/*****************************************************************************/
/*****************************************************************************/




/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_init(
                                ViRsrc 		InstrDesc,
                                ViBoolean 	id_query,
                                ViBoolean 	do_reset,
                                ViPSession 	vi)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession      InstrDesc (in)                                          */
/*      The instrument's VISA address.                                       */
/*      GPIB Example: "GPIB0::20::INSTR"                                     */
/*    ViBoolean      id_query (in)                                           */
/*      If VI_TRUE, validate instrument identity                             */
/*    ViBoolean      do_reset (in)                                           */
/*      If VI_TRUE, perform an instrument reset                              */
/*    ViPSession     vi (out)                                                */
/*      Pointer to Instrument Handle.  Returns VI_NULL if the init fails     */
/*****************************************************************************/

{
    struct     agb220xa_globals *thisPtr;
    ViStatus   errStatus;
    ViSession  defRM;
    ViChar     idn_buf[256];

    *vi = VI_NULL;

    /*************************************************************/
    /* Find the Default Resource Manager.  Potential errors are  */
    /*   VI_ERROR_SYSTEM_ERROR, VI_ERROR_ALLOC                   */
    /*************************************************************/

    errStatus = viOpenDefaultRM(&defRM);
    if (VI_SUCCESS > errStatus)
	return errStatus;

    /*************************************************************/
    /* Open a session to the instrument.  Potential errors are   */
    /*   VI_ERROR_NSUP_OPER, VI_ERROR_INV_RSRC_NAME,             */
    /*   VI_ERROR_INV_ACC_MODE, VI_ERROR_RSRC_NFOUND,            */
    /*   VI_ERROR_ALLOC                                          */
    /*************************************************************/

    errStatus = viOpen(defRM, InstrDesc, VI_NULL, VI_NULL, vi);
    if (VI_SUCCESS > errStatus)
        {
            viClose(defRM);
            *vi=VI_NULL;
            return errStatus;
        }

    /*************************************************************/
    /* Allocate global memory for the session.  Inititalize the  */
    /*   memory.  Note that viSetAttribute should never fail,    */
    /*   as all of it's input conditions are satisfied.          */
    /*************************************************************/

    thisPtr = (struct agb220xa_globals *)malloc(sizeof( struct agb220xa_globals) );
    if (0 == thisPtr)
        {
            viClose(*vi);
            viClose(defRM);
            *vi=VI_NULL;
            return VI_ERROR_ALLOC ;
        }

    errStatus = viSetAttribute(*vi, VI_ATTR_USER_DATA, (ViAttrState)thisPtr); 
    if( VI_SUCCESS > errStatus)
        {
            viClose(*vi);
            viClose(defRM);
            *vi=VI_NULL;
            return errStatus;
        }

    /*************************************************************/
    /* Set initial values of variables in global memory.         */
    /*************************************************************/

    thisPtr->defRMSession = defRM;
    thisPtr->errNumber = VI_SUCCESS;
    thisPtr->errFuncName[0] = 0;
    thisPtr->errMessage[0] = 0;
    thisPtr->errQueryDetect = VI_FALSE;
    thisPtr->blockSrqIO = VI_FALSE;
    thisPtr->countSrqIO = 0;
    thisPtr->myDelay = 0;


    thisPtr->mother_b = def_mother_board;
    thisPtr->matrix_h = def_matrix_h_2210A;
    thisPtr->matrix_l = def_matrix_l_2210A;
    thisPtr->cable1   = def_cable1;
    thisPtr->cable2_h = def_cable_2;
    thisPtr->cable2_l = def_cable_2;
    thisPtr->cable3_h = def_cable_3;
    thisPtr->cable3_l = def_cable_3;

    thisPtr->pcif     = TRUE;
    thisPtr->card_type = CARD_B2210A;

    errStatus = viGetAttribute(*vi, VI_ATTR_INTF_TYPE, &thisPtr->interfaceType);
    if( VI_SUCCESS > errStatus)
        return initError(vi, errStatus);


    /*************************************************************/
    /* Reset the instrument if so instructed.                    */
    /*   GPIB reset includes a 488.2 device clear.               */
    /*************************************************************/

    if( VI_TRUE == do_reset ) 
        {
            if (thisPtr->interfaceType == VI_INTF_GPIB)
                if (viClear(*vi) <  VI_SUCCESS)
                    return initError(vi, agb220xa_INSTR_ERROR_RESET_FAILED);
            
            if (agb220xa_reset(*vi) <  VI_SUCCESS)
                return initError(vi, agb220xa_INSTR_ERROR_RESET_FAILED);
        }

    /*************************************************************/
    /* Validate the instrument's identity.                       */
    /*************************************************************/

    if( VI_TRUE == id_query ) 
        {
            if (thisPtr->interfaceType == VI_INTF_GPIB)
                if (viClear(*vi) <  VI_SUCCESS)
                    return initError(vi, agb220xa_INSTR_ERROR_RESET_FAILED);

            if (   (viPrintf(*vi, "*IDN?\n") < VI_SUCCESS )   
                   || (delayScanf(*vi, 100, "%t", idn_buf) < VI_SUCCESS )   
                   || (strncmp(idn_buf, agb2200a_IDN_STRING, strlen(agb2200a_IDN_STRING)))
                   && (strncmp(idn_buf, agb2201a_IDN_STRING, strlen(agb2201a_IDN_STRING)))	) 
                return initError(vi, VI_ERROR_FAIL_ID_QUERY);
            if (!strncmp(idn_buf, agb2200a_IDN_STRING, strlen(agb2200a_IDN_STRING))){
                thisPtr->card_type = CARD_B2210A;
            } else {
                thisPtr->card_type = CARD_B2211A;
                thisPtr->matrix_h  = def_matrix_h_2211A;
                thisPtr->matrix_l  = def_matrix_l_2211A;
            }
        }
    
    return statusUpdate(*vi, thisPtr, errStatus);
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_close( 
                                 ViSession vi)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession      vi (in)                                                 */
/*      Instrument Handle returned from agb220xa_init()                      */
/*****************************************************************************/

{
    struct agb220xa_globals *thisPtr;
    ViStatus errStatus;
    ViSession defRM;

    GET_GLOBALS(vi,thisPtr);

    /*************************************************************/
    /* Retrieve Resource Management session, free global memory  */
    /*   for the session (if it exists), and close the vi and    */
    /*   RM sessions.                                            */
    /*************************************************************/

    defRM = thisPtr->defRMSession;

    if( thisPtr )		
        free(thisPtr);

    return viClose(defRM);
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_reset( 
                                 ViSession vi)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession      vi (in)                                                 */
/*      Instrument Handle returned from agb220xa_init()                      */
/*****************************************************************************/

{
    struct agb220xa_globals *thisPtr;
    ViStatus errStatus;

    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_reset" );

    if ((errStatus = viPrintf(vi, "*RST\n")) < VI_SUCCESS)
        return statusUpdate(vi, thisPtr, errStatus);

    return statusUpdate(vi, thisPtr, errStatus);
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_self_test(
                                     ViSession 		vi,
                                     ViPInt16  		test_result,
                                     ViChar _VI_FAR 	test_message[])
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession      vi (in)                                                 */
/*      Instrument Handle returned from agb220xa_init()                      */
/*    ViPInt32       test_result (out)                                       */
/*      Self-test result                                                     */
/*    ViChar _VI_FAR test_message[] (out)                                    */
/*      Self-test status message string, limited to 256 characters.          */
/*****************************************************************************/
{
    struct agb220xa_globals *thisPtr;
    ViStatus errStatus;

    *test_result = -1; 
    test_message[0] = 0; 

    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_self_test" );

    thisPtr->blockSrqIO = VI_TRUE;

    if ((errStatus = viPrintf(vi, "*TST?\n")) < VI_SUCCESS)
        return statusUpdate(vi, thisPtr, errStatus);

    if ((errStatus = delayScanf(vi, thisPtr->myDelay,
                                "%hd%*t", test_result)) < VI_SUCCESS)
        {
            *test_result = -1; 
            return statusUpdate(vi, thisPtr, errStatus);
        }

    switch( *test_result ) {
    case 0:
        sprintf(test_message, agb220xa_MSG_SELF_TEST_PASSED);
        break;

        /*************************************************************/
        /* DEVELOPER:  Add instrument specific self-test error       */
        /*   codes and messages here.                                */
        /*************************************************************/

    default:
        sprintf(test_message, agb220xa_MSG_SELF_TEST_FAILED);
        break;
    }

    return statusUpdate(vi, thisPtr, errStatus);
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_error_query(
                                       ViSession  		vi,
                                       ViPInt32   		inst_error_number,
                                       ViChar _VI_FAR 	inst_error_message[])
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession      vi (in)                                                 */
/*      Instrument Handle returned from agb220xa_init()                      */
/*    ViPInt32       inst_error_number (out)                                 */
/*      An instrument error code                                             */
/*    ViChar _VI_FAR inst_error_message[] (out)                              */
/*      Instrument error message string, limited to 256 characters.          */
/*****************************************************************************/

{
    struct agb220xa_globals *thisPtr;
    ViStatus errStatus;

    *inst_error_number = -1; 
    inst_error_message[0] = 0; 

    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_error_query" );

    thisPtr->blockSrqIO = VI_TRUE;

    if ((errStatus = viPrintf( vi, "SYST:ERR?\n")) < VI_SUCCESS)
        return statusUpdate(vi, thisPtr, errStatus);

    if ((errStatus = delayScanf(vi, thisPtr->myDelay, "%ld,%t",
                                inst_error_number, inst_error_message)) < VI_SUCCESS)
        {
            *inst_error_number = -1; 
            inst_error_message[0] = 0; 
            return statusUpdate(vi, thisPtr, errStatus);
        }

    /* get rid of extra LF at the end of the error_message */
    inst_error_message[strlen(inst_error_message)-1] = 0;

    return statusUpdate(vi, thisPtr, errStatus);
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_error_message(
                                         ViSession  		vi,
                                         ViStatus   		driver_error_number,
                                         ViChar _VI_FAR 	driver_error_message[])
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession      vi (in)                                                 */
/*      Instrument Handle returned from agb220xa_init()                      */
/*      May be VI_NULL for this function                                     */
/*    ViStatus       driver_error_number (in)                                */
/*      The error return value from an instrument driver function            */
/*    ViChar _VI_FAR driver_error_message[] (out)                            */
/*      Error message string.  This is limited to 256 characters.            */
/*****************************************************************************/

{
    struct    agb220xa_globals *thisPtr;
    ViStatus  errStatus;  
    ViInt32   idx;
    
    driver_error_message[0] = 0;
 
    thisPtr = NULL;
    if (vi != VI_NULL)
        GET_GLOBALS(vi,thisPtr);

    if (driver_error_number == VI_SUCCESS)
        {
            sprintf(driver_error_message, agb220xa_MSG_NO_ERRORS);
            return statusUpdate(vi, thisPtr, VI_SUCCESS);
        }

    /*************************************************************/
    /* Search the error message table to see if the error is in  */
    /*   that table.  If it is, copy the corresponding error     */
    /*   message to the output error message.  If context        */
    /*   dependent error messages are being used, append the     */
    /*   context dependent information to the message.           */
    /* If the error is found in the table, exit the routine      */
    /*   successfully.                                           */
    /*************************************************************/

    for (idx=0; idx < (sizeof instrErrMsgTable / 
                       sizeof(struct instrErrStruct)); idx++) {
        if (instrErrMsgTable[idx].errStatus == driver_error_number)
            {
                if ((thisPtr) && (thisPtr->errNumber == driver_error_number))
                    {
                        sprintf(driver_error_message, "%s " agb220xa_MSG_IN_FUNCTION " %s() %s",
                                instrErrMsgTable[idx].errMessage,
                                thisPtr->errFuncName,
                                thisPtr->errMessage);
                    }
                else
                    strcpy(driver_error_message, instrErrMsgTable[idx].errMessage);
                
                return statusUpdate(vi, thisPtr, VI_SUCCESS);
            }
    }
    
    /*************************************************************/
    /* Check to see if the error is a VTL/VISA error number      */
    /*   using viStatusDesc, which will also return the error    */
    /*   message.  If context dependent error messages are       */
    /*   used, append the context dependent information to the   */
    /*   message.                                                */
    /* If the error is found in the table, exit the routine      */
    /*   successfully.                                           */
    /*************************************************************/

    errStatus = viStatusDesc(vi, driver_error_number, driver_error_message);
    if (errStatus == VI_SUCCESS)
        {
            if( (thisPtr) && (thisPtr->errNumber == driver_error_number))
                {
                    strcat(driver_error_message, " ");
                    strcat(driver_error_message, agb220xa_MSG_IN_FUNCTION);
                    strcat(driver_error_message, " ");
                    strcat(driver_error_message, thisPtr->errFuncName);
                    strcat(driver_error_message, "() ");
                    strcat(driver_error_message, thisPtr->errMessage);
                }
            return statusUpdate(vi, thisPtr, VI_SUCCESS);
        }

    /*************************************************************/
    /*   At this point vi is either INVALID or VI_NULL           */
    /*************************************************************/

    if( VI_NULL == vi )
        {
            strcpy(driver_error_message, agb220xa_MSG_VI_OPEN_ERR);
            return statusUpdate(vi, thisPtr, VI_SUCCESS);
        } 

    sprintf(driver_error_message, agb220xa_MSG_INVALID_STATUS "  %ld"
            agb220xa_MSG_INVALID_STATUS_VALUE, (long)driver_error_number);
	
    return statusUpdate(vi, thisPtr, VI_ERROR_PARAMETER2);
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_revision_query(
                                          ViSession      vi,
                                          ViChar _VI_FAR driver_rev[],
                                          ViChar _VI_FAR instr_rev[])
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession      vi (in)                                                 */
/*      Instrument Handle returned from agb220xa_init()                      */
/*    ViChar _VI_FAR driver_rev[] (out)                                      */
/*      Instrument driver revision.  This is limited to 256 characters.      */
/*    ViChar _VI_FAR instr_rev[] (out)                                       */
/*      Instrument firmware revision.  This is limited to 256 characters.    */
/*****************************************************************************/

{
    struct    agb220xa_globals *thisPtr;
    ViStatus  errStatus;
    ViChar    temp_str1[256];         		   /* temp hold for instr rev string */
    ViChar    *last_comma;             		   /* last comma in *IDN string 	 */
	
    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_revision_query" );

    driver_rev[0] = 0; 
    instr_rev[0] = 0; 
    
    sprintf( driver_rev, "%s", agb220xa_REV_CODE);
    thisPtr->blockSrqIO = VI_TRUE;
    
    if ((errStatus = viPrintf( vi, "*IDN?\n")) < VI_SUCCESS)
        {
            driver_rev[0] = 0;  
            return statusUpdate(vi, thisPtr, errStatus); 
        }

    /*************************************************************/
    /* In the standard c template, the format string is          */
    /*   "%t%*t".  This removes white space from the end of the  */
    /*   returned string, but doesn't work if there are          */
    /*   embedded spaces in the returned string.  I used this    */
    /*   format string and resorted to stripping off spaces and  */
    /*   spaces and trainling non-printing characters below.     */
    /*   This is changed in the template for the 86120.          */
    /*************************************************************/

    if ((errStatus = delayScanf(vi, thisPtr->myDelay, "%t", temp_str1)) < VI_SUCCESS)
        {
            driver_rev[0] = 0; 
            instr_rev[0]  = 0; 
            return statusUpdate(vi, thisPtr, errStatus); 
        }
    
    /*************************************************************/
    /* error and exit if last comma not found                    */
    /*************************************************************/

    last_comma = strrchr(temp_str1,',');

    if (!last_comma) 
        {
            instr_rev[0] = 0;
            strcpy(thisPtr->errMessage, "no last comma found in IDN string");
            return statusUpdate(vi, thisPtr, agb220xa_INSTR_ERROR_UNEXPECTED); 
        }

    /*************************************************************/
    /* The next statement assumes that the firmware revision of  */
    /*   the instrument follows the last comma/space pair.       */
    /*************************************************************/

    strcpy(instr_rev, last_comma+1);

    return statusUpdate(vi, thisPtr, VI_SUCCESS); 
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_timeOut (
                                    ViSession  vi,
                                    ViInt32    time_out)
/*****************************************************************************/
/*  Sets the current setting of the driver timeout value for the instrument  */
/*  in milliseconds.                                                         */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  vi (in)                                                     */
/*      Instrument Handle returned from agb220xa_init()                      */
/*    ViInt32    time_out (in)                                               */
/*      The period of time which VISA will allow an instrument command to    */
/*      wait before stopping it's execution.                                 */
/*****************************************************************************/

{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    GET_GLOBALS(vi,thisPtr)
        agb220xa_CDE_INIT( "agb220xa_timeOut" );

    agb220xa_CHK_LONG_RANGE(time_out, agb220xa_TIMEOUT_MIN,
                            agb220xa_TIMEOUT_MAX, VI_ERROR_PARAMETER2);

    if ((errStatus = viSetAttribute(vi, VI_ATTR_TMO_VALUE, time_out)) < VI_SUCCESS)
        return statusUpdate(vi, thisPtr, errStatus);

    return statusUpdate(vi, thisPtr, errStatus);
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_timeOut_Q (
                                      ViSession  vi,
                                      ViPInt32   q_time_out)
/*****************************************************************************/
/*  Returns the current setting of the timeout value of the instrument in    */
/*  milliseconds.                                                            */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  vi (in)                                                     */
/*      Instrument Handle returned from agb220xa_init()                      */
/*    ViPInt32   q_time_out (out)                                            */
/*      The period of time which VISA will allow an instrument command to    */
/*      wait before stopping it's execution.                                 */
/*****************************************************************************/

{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_timeOut_Q" );

    errStatus = viGetAttribute(vi, VI_ATTR_TMO_VALUE, q_time_out);
    if (VI_SUCCESS > errStatus)
        return statusUpdate(vi, thisPtr, errStatus);

    return statusUpdate(vi, thisPtr, errStatus);
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_errorQueryDetect( 
                                            ViSession  vi,
                                            ViBoolean  eq_detect)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession  vi (in)                                                     */
/*      Instrument Handle returned from agb220xa_init()                      */
/*    ViBoolean  eq_detect  (in)                                             */
/*      Enables (VI_TRUE) or disables (VI_FALSE) automatic instrument error  */
/*      querying                                                             */
/*****************************************************************************/

{
    ViStatus errStatus = 0;

    struct agb220xa_globals *thisPtr;

    GET_GLOBALS(vi,thisPtr)
        agb220xa_CDE_INIT( "agb220xa_errorQueryDetect" );

    agb220xa_CHK_BOOLEAN(eq_detect, VI_ERROR_PARAMETER2);

    thisPtr->errQueryDetect = eq_detect;

    return statusUpdate( vi, thisPtr, VI_SUCCESS);
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_errorQueryDetect_Q( 
                                              ViSession   vi,
                                              ViPBoolean  q_eq_detect)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession   vi (in)                                                    */
/*      Instrument Handle returned from agb220xa_init()                      */
/*    ViPBoolean  q_eq_detect (out)                                          */
/*      Equals VI_TRUE if automatic instrument error querying is being       */
/*      being performed.                                                     */
/*****************************************************************************/

{
    ViStatus errStatus = 0;

    struct agb220xa_globals *thisPtr;

    GET_GLOBALS(vi,thisPtr)
        agb220xa_CDE_INIT( "agb220xa_errorQueryDetect_Q" );

    *q_eq_detect = thisPtr->errQueryDetect;

    return statusUpdate( vi, thisPtr, VI_SUCCESS);
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_dcl( 
                               ViSession  vi)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession vi (in)                                                      */
/*      Instrument Handle returned from agb220xa_init()                      */
/*****************************************************************************/

{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    GET_GLOBALS(vi,thisPtr)
        agb220xa_CDE_INIT( "agb220xa_dcl" );

    if (thisPtr->interfaceType == VI_INTF_GPIB)
        if ((errStatus = viClear(vi)) < VI_SUCCESS)
            return statusUpdate( vi, thisPtr, errStatus);	

    return statusUpdate( vi, thisPtr, errStatus);
}        /* ----- end of function ----- */

 
/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_opc_Q(
                                 ViSession   vi,
                                 ViPBoolean  instr_ready)
/*****************************************************************************/
/*  Sends an *OPC? command to the instrument and returns VI_FALSE if there   */
/*  are operations which are still pending.                                  */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession   vi (in)                                                    */
/*      Instrument handle returned from agb220xa_init()                      */
/*    ViPBoolean  instr_ready (out)                                          */
/*      Returns VI_FALSE if operations are still pending.                    */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct agb220xa_globals *thisPtr;
   
    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_opc_Q" );

    if ((errStatus = viPrintf(vi,"*OPC?\n")) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);

    if ((errStatus = delayScanf(vi, thisPtr->myDelay,
                                "%hd%*t", instr_ready)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);

    return statusUpdate( vi, thisPtr, errStatus);
}        /* ----- end of function ----- */ 


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_readStatusByte_Q(
                                            ViSession  vi,
                                            ViPInt16   status_byte)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession vi (in)                                                      */
/*      Instrument Handle returned from agb220xa_init()                      */
/*    ViPInt16  statusByte (out)                                             */
/*      Returns the contents of the status byte                              */
/*****************************************************************************/

{
    ViStatus  errStatus = 0;
    struct    agb220xa_globals *thisPtr;
    ViUInt16  stb;

    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_readStatusByte_Q" );

    if ((errStatus = viReadSTB(vi,&stb)) < VI_SUCCESS)
        return statusUpdate(vi, thisPtr, errStatus );

    *status_byte = (ViInt16)stb;

    return statusUpdate(vi, thisPtr, errStatus);
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_operEvent_Q(
                                       ViSession  vi,
                                       ViPInt32   oe_sregister)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession vi (in)                                                      */
/*      Instrument Handle returned from agb220xa_init()                      */
/*    ViPInt32  oe_sregister (out)                                           */
/*      Returns the contents of the operation event register                 */
/*****************************************************************************/

{
    ViStatus  errStatus;
    struct    agb220xa_globals *thisPtr;
    ViUInt32  count;
    ViChar    buf[20];

    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_operEvent_Q" );

    if ((errStatus = viWrite(vi,(ViBuf)"STAT:OPER:EVEN?",15,&count)) < VI_SUCCESS)
        return statusUpdate(vi, thisPtr, errStatus);

    if ((errStatus = delayRead(vi, thisPtr->myDelay,
                               (ViPBuf)buf, 20, &count)) < VI_SUCCESS)
        return statusUpdate(vi, thisPtr, errStatus);

    buf[count]='\0';
    *oe_sregister = atoi( buf);

    return statusUpdate(vi, thisPtr, errStatus);
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_operCond_Q(
                                      ViSession vi,
                                      ViPInt32  oc_sregister)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession vi (in)                                                      */
/*      Instrument Handle returned from agb220xa_init()                      */
/*    ViPInt32  oc_sregister (out)                                           */
/*      Returns the contents of the operation condition register             */
/*****************************************************************************/

{
    ViStatus   errStatus;
    struct     agb220xa_globals *thisPtr;
    ViUInt32   count;
    ViChar     buf[20];

    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_operCond_Q" );

    if ((errStatus = viWrite(vi,(ViBuf)"STAT:OPER:COND?",15,&count)) < VI_SUCCESS)
        return statusUpdate(vi, thisPtr, errStatus);

    if ((errStatus = delayRead(vi, thisPtr->myDelay,
                               (ViPBuf)buf, 20, &count)) < VI_SUCCESS)
        return statusUpdate(vi, thisPtr, errStatus);

    buf[count]='\0';
    *oc_sregister = atoi( buf);

    return statusUpdate(vi, thisPtr, errStatus);
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_quesEvent_Q(
                                       ViSession vi,
                                       ViPInt32  qe_sregister)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession vi (in)                                                      */
/*      Instrument Handle returned from agb220xa_init()                      */
/*    ViPInt32  qe_sregister (out)                                           */
/*      Returns the contents of the questionable event register              */
/*****************************************************************************/

{
    ViStatus  errStatus;
    struct    agb220xa_globals *thisPtr;
    ViUInt32  count;
    ViChar    buf[20];

    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_quesEvent_Q" );

    if ((errStatus = viWrite(vi,(ViBuf)"STAT:QUES:EVEN?",15,&count)) < VI_SUCCESS)
        return statusUpdate(vi, thisPtr, errStatus );

    if ((errStatus = delayRead(vi, thisPtr->myDelay,
                               (ViPBuf)buf, 20, &count)) < VI_SUCCESS)
        return statusUpdate(vi, thisPtr, errStatus );

    buf[count]='\0';
    *qe_sregister = atoi( buf);

    return statusUpdate(vi, thisPtr, errStatus );
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_quesCond_Q(
                                      ViSession vi,
                                      ViPInt32  qc_sregister)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession vi (in)                                                      */
/*      Instrument Handle returned from agb220xa_init()                      */
/*    ViPInt32  qc_sregister (out)                                           */
/*      Returns the contents of the questionable condition register          */
/*****************************************************************************/
 
{
    ViStatus  errStatus;
    struct    agb220xa_globals *thisPtr;
    ViUInt32  count;
    ViChar    buf[20];

    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_quesCond_Q" );
 
    if ((errStatus = viWrite(vi,(ViBuf)"STAT:QUES:COND?",15,&count)) < VI_SUCCESS)
        return statusUpdate(vi, thisPtr, errStatus);

    if ((errStatus = delayRead(vi, thisPtr->myDelay,
                               (ViPBuf)buf, 20, &count)) < VI_SUCCESS)
        return statusUpdate(vi, thisPtr, errStatus);

    buf[count]='\0';
    *qc_sregister = atoi( buf);

    return statusUpdate(vi, thisPtr, errStatus);
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_cmd( 
                               ViSession vi,
                               ViString  cmd_string)
/*****************************************************************************/
/*  Send a scpi command, it does not look for a response                     */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession vi (in)                                                      */
/*      Instrument handle returned from agb220xa_init()                      */
/*    ViString        cmd_string (in)                                        */
/*      The SCPI command string to be sent to the instrument                 */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct   agb220xa_globals *thisPtr;

    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_cmd" );

#ifdef NO_FORMAT_IO
    {
        ViUInt32 rtn_size;
        errStatus = viWrite(vi, (ViBuf)cmd_string, strlen(cmd_string), &rtn_size );
    }
#else
    errStatus = viPrintf(vi,"%s\n", cmd_string);
#endif
   
    return statusUpdate( vi, thisPtr, errStatus );
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_cmdString_Q(
                                       ViSession       vi,
                                       ViString        sq_cmd_string,
                                       ViInt32         sq_size,
                                       ViChar _VI_FAR  sq_result[] )
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession       vi (in)                                                */
/*      Instrument handle returned from agb220xa_init()                      */
/*    ViString        sq_cmd_string (in)                                     */
/*      The SCPI command string to be sent to the instrument                 */
/*    ViInt32         sq_size (in)                                           */
/*      The size of the char array (result) passed to the function to hold   */
/*      the string returned by the instrument                                */
/*    ViChar _VI_FAR  sq_result[] (out)                                      */
/*      The string returned by the instrument                                */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct   agb220xa_globals *thisPtr;

    /* Command strings must have at least one non-null character */
    if(sq_size < 2)     return statusUpdate(vi,VI_NULL,VI_ERROR_PARAMETER2);

#ifdef WIN16
    /* Strings in 16-bit windows are limited to 32K characters. */	
    if(sq_size > 32767) return statusUpdate(vi,VI_NULL,VI_ERROR_PARAMETER2);
#endif
	
    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_cmdString_Q" );

#ifdef NO_FORMAT_IO
    {
        ViInt32   mySize;
        ViUInt32  rtn_size;
        if ((errStatus = viWrite(vi, (ViBuf)sq_cmd_string,
                                 strlen(sq_cmd_string), &rtn_size )) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);
        mySize = sq_size-1;

        if ((errStatus = delayRead(vi, thisPtr->myDelay,
                                   (ViPBuf)sq_result, mySize, &rtn_size)) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);
        sq_result[rtn_size] = '\0';
        if (thisPtr->interfaceType == VI_INTF_GPIB)
            if (errStatus == VI_SUCCESS_MAX_CNT)
                viClear(vi);
    }
#else
    {
        int sz;
        if ((errStatus = viPrintf(vi, "%s\n",sq_cmd_string )) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);
        sz = (int)sq_size;

        if ((errStatus = delayScanf(vi, thisPtr->myDelay, "%#c", &sz,sq_result)) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);
        sq_result[sz]='\0';
    }
#endif
      
    return statusUpdate( vi, thisPtr, errStatus );
}        /* ----- end of function ----- */ 


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_cmdData_Q(
                                     ViSession  		vi,
                                     ViString   		dq_cmd_string,
                                     ViInt32    		dq_size,
                                     ViChar _VI_FAR 	dq_result[])
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession       vi (in)                                                */
/*      Instrument handle returned from agb220xa_init()                      */
/*    ViString        dq_cmd_string (in)                                     */
/*      The SCPI command string to be sent to the instrument                 */
/*    ViInt32         dq_size (in)                                           */
/*      The size of the char array (result) passed to the function to hold   */
/*      the string returned by the instrument                                */
/*    ViChar _VI_FAR  dq_result[] (out)                                      */
/*      The string returned by the instrument                                */
/*****************************************************************************/

{  
    ViStatus  errStatus;
    ViInt32   mySize;
    struct    agb220xa_globals *thisPtr;

    /* Command strings must have at least one non-null character */
    if(dq_size < 2)     return statusUpdate(vi,VI_NULL,VI_ERROR_PARAMETER2);

#ifdef WIN16
    /* Strings in 16-bit windows are limited to 32K characters. */	
    if(dq_size > 32767) return statusUpdate(vi,VI_NULL,VI_ERROR_PARAMETER2);
#endif
	
    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_cmdData_Q" );

    /* If (message-based I/O) then write to instrument. */
#ifdef NO_FORMAT_IO
    {
        ViUInt32 rtn_size;
        if ((errStatus = viWrite(vi, (ViBuf)dq_cmd_string,
                                 strlen(dq_cmd_string), &rtn_size )) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);
        mySize = dq_size-1;

        if ((errStatus = delayRead(vi, thisPtr->myDelay,
                                   (ViPBuf)dq_result, mySize, &rtn_size)) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);
        if (thisPtr->interfaceType == VI_INTF_GPIB)
            if (errStatus == VI_SUCCESS_MAX_CNT)
                viClear(vi);
    }
#else
    {
        int sz;
        if ((errStatus = viPrintf(vi, "%s\n",dq_cmd_string )) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);
        mySize = dq_size-1;
        sz = (int)dq_size;

        if ((errStatus = delayScanf(vi, thisPtr->myDelay, "%#t", &sz, dq_result)) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);
    }
#endif
      
    return statusUpdate( vi, thisPtr, errStatus );
}        /* ----- end of function ----- */
 

/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_cmdInt(
                                  ViSession  vi,
                                  ViString   i_cmd_string,
                                  ViInt32    i_val )
/*****************************************************************************/
/*  Sends an instrument command which takes a single integer parameter.      */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  vi (in)                                                     */
/*      Instrument handle returned from agb220xa_init()                      */
/*    ViString   i_cmd_string (in)                                           */
/*      The instrument command string to be sent to the instrument.          */
/*    ViInt32    i_val (in)                                                  */
/*      The integer sent to the instrument at the end of the instrument      */
/*      command.  Can be ViInt16 or ViInt32.                                 */
/*****************************************************************************/

{  
    ViStatus errStatus;
    struct agb220xa_globals *thisPtr;
   
    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_cmdInt" );

#ifdef NO_FORMAT_IO
    {
        ViUInt32 rtn_size;
        ViChar   lc[300];

        sprintf(lc,"%s %ld",i_cmd_string,i_val);
        if ((errStatus = viWrite(vi, (ViBuf)lc,
                                 strlen(lc), &rtn_size )) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);
    }
#else
    if ((errStatus = viPrintf(vi, "%s %ld\n",i_cmd_string,i_val )) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
#endif

    return statusUpdate( vi, thisPtr, errStatus );
}        /* ----- end of function ----- */ 


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_cmdInt16_Q(
                                      ViSession  vi,
                                      ViString   iq_cmd_string,
                                      ViPInt16   iq_result)
/*****************************************************************************/
/*  Sends scpi command and waits for a response that must be representable   */
/*    as an int32.  A non numeric instrument response returns zero in        */
/*    result.                                                                */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  vi (in)                                                     */
/*      Instrument handle returned from agb220xa_init()                      */
/*    ViString   iq_cmd_string (in)                                          */
/*      The SCPI command string to be sent to the instrument.                */
/*    ViPInt16   iq_result (out)                                             */
/*      The integer returned from the instrument.                            */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct   agb220xa_globals *thisPtr;
   
    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_cmdInt16_Q" );

#ifdef NO_FORMAT_IO
    {
        ViChar lc[100];
        ViUInt32 rtn_size;

        if ((errStatus = viWrite(vi, (ViBuf)iq_cmd_string,
                                 strlen(iq_cmd_string), &rtn_size )) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);

        if ((errStatus = delayRead(vi, thisPtr->myDelay,
                                   (ViPBuf)lc, 100, &rtn_size)) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);
        lc[rtn_size]='\0';
        *iq_result = atoi(lc);
    }
#else
    if ((errStatus = viPrintf(vi, "%s\n",iq_cmd_string)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);

    if ((errStatus = delayScanf(vi, thisPtr->myDelay, "%hd%*t",iq_result)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
#endif

    return statusUpdate( vi, thisPtr, errStatus );
}        /* ----- end of function ----- */ 


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_cmdInt32_Q(
                                      ViSession vi,
                                      ViString  lq_cmd_string,
                                      ViPInt32  lq_result)
/*****************************************************************************/
/*  Sends scpi command and waits for a response that must be representable   */
/*    as an int32.  A non numeric instrument response returns zero in        */
/*    result.                                                                */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  vi (in)                                                     */
/*      Instrument handle returned from agb220xa_init()                      */
/*    ViString   lq_cmd_string (in)                                          */
/*      The SCPI command string to be sent to the instrument.                */
/*    ViPInt32   lq_result (out)                                             */
/*      The integer returned from the instrument.                            */
/*****************************************************************************/

{  
    ViStatus  errStatus;
    struct    agb220xa_globals *thisPtr;
   
    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_cmdInt32_Q" );

#ifdef NO_FORMAT_IO
    {
        ViUInt32 rtn_size;
        ViChar   lc[100];

        if ((errStatus = viWrite(vi, (ViBuf)lq_cmd_string,
                                 strlen(lq_cmd_string), &rtn_size )) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);

        if ((errStatus = delayRead(vi, thisPtr->myDelay,
                                   (ViPBuf)lc, 100, &rtn_size)) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);
        *lq_result = atol(lc);
    }
#else
    if ((errStatus = viPrintf(vi, "%s\n",lq_cmd_string)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);

    if ((errStatus = delayScanf(vi, thisPtr->myDelay, "%ld%*t", lq_result)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
#endif	
        
    return statusUpdate( vi, thisPtr, errStatus );
}        /* ----- end of function ----- */ 

	
/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_cmdInt16Arr_Q(
                                         ViSession 		vi,
                                         ViString  		iaq_cmd_string,
                                         ViInt32   		iaq_size,
                                         ViInt16 _VI_FAR iaq_result[],
                                         ViPInt32  		iaq_count)
/*****************************************************************************/
/*  This entry point assumes that the return result is a arbitrary block.    */
/*  Do not use this entry point for ascii values.  For that just get the     */
/*  data from _cmd_string_Q routine.                                         */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  vi (in)                                                     */
/*      Instrument handle returned from agb220xa_init()                      */
/*    ViString   iaq_cmd_string (in)                                         */
/*      The instrument command string to be sent to the instrument.          */
/*    ViInt32    iaq_size (in)                                               */
/*      The number of elements allocated to the results array.               */
/*    ViInt16    _VI_FAR iaq_result[] (out)                                  */
/*      Array of 16-bit integers - result from instrument.                   */
/*    ViPInt32   iaq_count (out)                                             */
/*      The actual number of elements returned by the instrument             */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    ViUInt32  cnt;
    struct    agb220xa_globals * thisPtr;
    ViChar    lc[100];
    ViInt32   i;
    ViInt32   ArrSize;
    ViInt32   nbytes;
    ViChar    _huge *Array;
	
    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_cmdInt16Arr_Q" );

    if (iaq_size < agb220xa_CMDINT16ARR_Q_MIN || iaq_size > agb220xa_CMDINT16ARR_Q_MAX)
        return statusUpdate(vi,thisPtr,VI_ERROR_PARAMETER3);

    if ((errStatus = viWrite(vi, (ViBuf)iaq_cmd_string,
                             strlen(iaq_cmd_string), &cnt)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
			
    if ((errStatus = delayRead(vi, thisPtr->myDelay,
                               (ViPBuf)lc, 1, &cnt)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
	
    if (lc[0] != '#')
        return statusUpdate(vi,thisPtr,VI_ERROR_INV_EXPR);
		
    if ((errStatus = delayRead(vi, thisPtr->myDelay,
                               (ViPBuf)lc, 1, &cnt)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
	
    nbytes = lc[0]-'0';
	    
    if (nbytes)
        {
            if ((errStatus = delayRead(vi, thisPtr->myDelay,
                                       (ViPBuf)lc, nbytes, &cnt)) < VI_SUCCESS)
                return statusUpdate(vi,thisPtr,errStatus);
            lc[cnt] = '\0';
            ArrSize = atol(lc);
        }
    else
        ArrSize = iaq_size*sizeof(ViInt16);
		    
#ifdef WIN16
    Array = (ViChar _huge *)halloc(ArrSize, 1 ); 
#else		                             
    Array = (ViChar *)malloc(ArrSize ); 
#endif
	
    if (!Array )
        return statusUpdate(vi,thisPtr,VI_ERROR_ALLOC);
	
    if ((errStatus = delayRead(vi, thisPtr->myDelay,
                               (ViPBuf)Array, ArrSize, &cnt)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
    if (thisPtr->interfaceType == VI_INTF_GPIB)
        if (errStatus == VI_SUCCESS_MAX_CNT)
            viClear(vi);
    ArrSize = cnt/sizeof(ViInt16);
    ArrSize = (ArrSize > iaq_size ) ? iaq_size : ArrSize;
	
    /*copy only as many as the user allows*/
    _fmemcpy(iaq_result,Array,(size_t)ArrSize*sizeof(ViInt16) ); 

#ifdef WIN16
    _hfree(Array);	
#else
    free(Array); 
#endif	

    *iaq_count=ArrSize;

    for (i=0; i<*iaq_count; i++)
        SWAP_16(&iaq_result[i]);

    return statusUpdate(vi,thisPtr,VI_SUCCESS);    	    	
}        /* ----- end of function ----- */
                                                                         

/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_cmdInt32Arr_Q(
                                         ViSession 		vi,
                                         ViString 		laq_cmd_string,
                                         ViInt32 		laq_size,
                                         ViInt32 _VI_FAR laq_result[],
                                         ViPInt32 		laq_count)
/*****************************************************************************/
/*  This entry point assumes that the return result is a arbitrary block.    */
/*  Do not use this entry point for ascii values.  For that just get the     */
/*  data from _cmd_string_Q routine.                                         */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  vi (in)                                                     */
/*      Instrument handle returned from agb220xa_init()                      */
/*    ViString   laq_cmd_string (in)                                         */
/*      The instrument command string to be sent to the instrument.          */
/*    ViInt32    laq_size (in)                                               */
/*      The number of elements allocated to the results array.               */
/*    ViInt32    _VI_FAR laq_result[] (out)                                  */
/*      Array of 32-bit integers - result from instrument.                   */
/*    ViPInt32   laq_count (out)                                             */
/*      The actual number of elements returned by the instrument             */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    ViUInt32  cnt;
    struct    agb220xa_globals * thisPtr;
    ViChar    lc[100];
    ViInt32   i;
    ViInt32   ArrSize;
    ViInt32   nbytes;
    ViChar    _huge *Array;
	
    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_cmdInt32Arr_Q" );

    if (laq_size < agb220xa_CMDINT32ARR_Q_MIN || laq_size > agb220xa_CMDINT32ARR_Q_MAX)
        return statusUpdate(vi,thisPtr,VI_ERROR_PARAMETER3);

    if ((errStatus = viWrite(vi, (ViBuf)laq_cmd_string,
                             strlen(laq_cmd_string), &cnt)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
			
    if ((errStatus = delayRead(vi, thisPtr->myDelay,
                               (ViPBuf)lc, 1, &cnt)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
	
    if (lc[0] != '#')
        return statusUpdate(vi,thisPtr,VI_ERROR_INV_EXPR);
		
    if ((errStatus = delayRead(vi, thisPtr->myDelay,
                               (ViPBuf)lc, 1, &cnt)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
	
    nbytes = lc[0]-'0';

    if (nbytes)
        {
            if ((errStatus = delayRead(vi, thisPtr->myDelay,
                                       (ViPBuf)lc, nbytes, &cnt)) < VI_SUCCESS)
                return statusUpdate(vi,thisPtr,errStatus);
            lc[cnt] = '\0';
            ArrSize = atol(lc);
        }
    else
        ArrSize = laq_size*sizeof(ViInt32);
		    
#ifdef WIN16
    Array = (ViChar _huge *)halloc(ArrSize, 1 ); 
#else		                             
    Array = (ViChar *)malloc(ArrSize ); 
#endif
	
    if (!Array)
        return statusUpdate(vi,thisPtr,VI_ERROR_ALLOC);

    if ((errStatus = delayRead(vi, thisPtr->myDelay,
                               (ViPBuf)Array, ArrSize, &cnt)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
    if (thisPtr->interfaceType == VI_INTF_GPIB)
        if (errStatus == VI_SUCCESS_MAX_CNT)
            viClear(vi);
    ArrSize = cnt/sizeof(ViInt32);
    ArrSize = (ArrSize > laq_size ) ? laq_size : ArrSize;

    /*copy only as many as the user allows*/

    _fmemcpy(laq_result,Array,(size_t)ArrSize*sizeof(ViInt32) );
#ifdef WIN16
    _hfree(Array);
#else
    free(Array);
#endif

    *laq_count=ArrSize;

    for (i=0; i<*laq_count; i++)
        SWAP_32 (&laq_result[i]);

    return statusUpdate(vi,thisPtr,VI_SUCCESS);    	    	
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_cmdReal(
                                   ViSession  vi,
                                   ViString   r_cmd_string,
                                   ViReal64   r_val )
/*****************************************************************************/
/*  Sends an instrument command which takes a single real parameter.         */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  vi (in)                                                     */
/*      Instrument handle returned from agb220xa_init()                      */
/*    ViString   r_cmd_string (in)                                           */
/*      The instrument command string to be sent to the instrument.          */
/*    ViReal64   r_val (in)                                                  */
/*      The integer sent to the instrument at the end of the instrument      */
/*      command.  Can be ViReal32 or ViReal64.                               */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct   agb220xa_globals *thisPtr;
   
    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_cmdReal" );

#ifdef NO_FORMAT_IO
    {
        ViUInt32  rtn_size;
        ViChar    lc[300];

        sprintf(lc,"%s %g",r_cmd_string,r_val);
        if ((errStatus = viWrite(vi, (ViBuf)lc,
                                 strlen(lc), &rtn_size )) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);
    }
#else
    if ((errStatus = viPrintf(vi, "%s %g\n",r_cmd_string,r_val )) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
#endif

    return statusUpdate( vi, thisPtr, errStatus );
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_cmdReal32_Q(
                                       ViSession  vi,
                                       ViString   rq_cmd_string,
                                       ViPReal32  rq_result)
/*****************************************************************************/
/*  Sends scpi command and waits for a response that must be representable   */
/*    as an real64.  A non numeric instrument response returns zero in       */
/*    result, in case of underflow undefined.                                */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  vi (in)                                                     */
/*      Instrument handle returned from agb220xa_init()                      */
/*    ViString   rq_cmd_string (in)                                          */
/*      The SCPI command string to be sent to the instrument.                */
/*    ViPReal32  rq_result (out)                                             */
/*      The real returned from the instrument.                               */
/*****************************************************************************/

{   
#ifdef NO_FORMAT_IO
    ViUInt32  rtn_size;
#endif

    ViStatus  errStatus;
    struct    agb220xa_globals *thisPtr;
   
    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_cmdReal32_Q" );

#ifdef NO_FORMAT_IO
    {
        ViChar lc[100];
        if ((errStatus = viWrite(vi, (ViBuf)rq_cmd_string,
                                 strlen(rq_cmd_string), &rtn_size )) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);

        if ((errStatus = delayRead(vi, thisPtr->myDelay,
                                   (ViPBuf)lc, 100, &rtn_size)) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);
        *rq_result = (ViReal32)atof(lc);
    }
#else
    if ((errStatus = viPrintf(vi, "%s\n",rq_cmd_string)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);

    if ((errStatus = delayScanf(vi, thisPtr->myDelay,
                                "%f%*t", rq_result)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
#endif	
        
    return statusUpdate( vi, thisPtr, errStatus );
}        /* ----- end of function ----- */ 

/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_cmdReal64_Q(
                                       ViSession  vi,
                                       ViString   dq_cmd_string,
                                       ViPReal64  dq_result)
/*****************************************************************************/
/*  Sends scpi command and waits for a response that must be representable   */
/*    as an real64.  A non numeric instrument response returns zero in       */
/*    result, in case of underflow undefined.                                */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  vi (in)                                                     */
/*      Instrument handle returned from agb220xa_init()                      */
/*    ViString   dq_cmd_string (in)                                          */
/*      The SCPI command string to be sent to the instrument.                */
/*    ViPReal64  dq_result (out)                                             */
/*      The real returned from the instrument.                               */
/*****************************************************************************/

{   
    ViStatus  errStatus;
    struct    agb220xa_globals *thisPtr;
   
    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_cmdReal64_Q" );

#ifdef NO_FORMAT_IO
    {
        ViUInt32  rtn_size;
        ViChar   lc[100];
        if ((errStatus = viWrite(vi, (ViBuf)dq_cmd_string,
                                 strlen(dq_cmd_string), &rtn_size )) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);

        if ((errStatus = delayRead(vi, thisPtr->myDelay,
                                   (ViPBuf)lc, 100, &rtn_size)) < VI_SUCCESS)
            return statusUpdate(vi,thisPtr,errStatus);
        *dq_result = atof(lc);
    }
#else
    if ((errStatus = viPrintf(vi, "%s\n",dq_cmd_string)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);

    if ((errStatus = delayScanf(vi, thisPtr->myDelay,
                                "%lg%*t", dq_result)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
#endif	
        
    return statusUpdate( vi, thisPtr, errStatus );
}        /* ----- end of function ----- */ 


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_cmdReal32Arr_Q(
                                          ViSession  			vi,
                                          ViString			raq_cmd_string,
                                          ViInt32    			raq_size,
                                          ViReal32 _VI_FAR	raq_result[],
                                          ViPInt32   			raq_count)
/*****************************************************************************/
/*  This entry point assumes that the return result is a arbitrary block.    */
/*  Do not use this entry point for ascii values.  For that just get the     */
/*  data from _cmd_string_Q routine.                                         */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  vi (in)                                                     */
/*      Instrument handle returned from agb220xa_init()                      */
/*    ViString   raq_cmd_string (in)                                         */
/*      The instrument command string to be sent to the instrument.          */
/*    ViInt32    raq_size (in)                                               */
/*      The number of elements allocated to the results array.               */
/*    ViReal32   _VI_FAR raq_result[] (out)                                  */
/*      Array of 32-bit reals - result from instrument.                      */
/*    ViPInt32   raq_count (out)                                             */
/*      The actual number of elements returned by the instrument             */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    ViUInt32  cnt;
    struct    agb220xa_globals * thisPtr;
    ViChar    lc[100];
    ViInt32   i;
    ViInt32   ArrSize;
    ViInt32   nbytes;
    ViChar    _huge *Array;
	
    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_cmdReal32Arr_Q" );

    if (raq_size < agb220xa_CMDREAL32ARR_Q_MIN || raq_size > agb220xa_CMDREAL32ARR_Q_MAX)
        return statusUpdate(vi,thisPtr,VI_ERROR_PARAMETER3);

    if ((errStatus = viWrite(vi, (ViBuf)raq_cmd_string,
                             strlen(raq_cmd_string),&cnt)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
			
    if ((errStatus = delayRead(vi, thisPtr->myDelay,
                               (ViPBuf)lc, 1, &cnt)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
	
    if (lc[0] != '#')
        return statusUpdate(vi,thisPtr,VI_ERROR_INV_EXPR);
		
    if ((errStatus = delayRead(vi, thisPtr->myDelay,
                               (ViPBuf)lc, 1, &cnt)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);

    nbytes = lc[0]-'0';

    if (nbytes)
        {
            if ((errStatus = delayRead(vi, thisPtr->myDelay,
                                       (ViPBuf)lc, nbytes, &cnt)) < VI_SUCCESS)
                return statusUpdate(vi,thisPtr,errStatus);
            lc[cnt] = '\0';
            ArrSize = atol(lc); 
        }                  
    else
        ArrSize = raq_size*sizeof(ViReal32);

#ifdef WIN16
    Array = (ViChar _huge *)halloc(ArrSize, 1 );
#else		                             
    Array = (ViChar *)malloc(ArrSize); 
#endif
	
    if (!Array)
        return statusUpdate(vi,thisPtr,VI_ERROR_ALLOC);
	
    if ((errStatus = delayRead(vi, thisPtr->myDelay,
                               (ViPBuf)Array, ArrSize, &cnt)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
    if (thisPtr->interfaceType == VI_INTF_GPIB)
        if (errStatus == VI_SUCCESS_MAX_CNT)
            viClear(vi);
    ArrSize = cnt/sizeof(ViReal32);
    ArrSize = (ArrSize > raq_size ) ? raq_size : ArrSize;

    _fmemcpy(raq_result,Array,(size_t)ArrSize*sizeof(ViReal32) );

#ifdef WIN16
    _hfree(Array);
#else
    free(Array);
#endif

    *raq_count=ArrSize;

    for (i=0; i<*raq_count; i++)
        SWAP_FLOAT32(&raq_result[i]);

    return statusUpdate(vi,thisPtr,VI_SUCCESS);	    	
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC agb220xa_cmdReal64Arr_Q(
                                          ViSession  			vi,
                                          ViString   			daq_cmd_string,
                                          ViInt32    			daq_size,
                                          ViReal64 _VI_FAR	daq_result[],
                                          ViPInt32   			daq_count)
/*****************************************************************************/
/*  This entry point assumes that the return result is a arbitrary block.    */
/*  Do not use this entry point for ascii values.  For that just get the     */
/*  data from _cmd_string_Q routine.                                         */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  vi (in)                                                     */
/*      Instrument handle returned from agb220xa_init()                      */
/*    ViString   daq_cmd_string (in)                                         */
/*      The instrument command string to be sent to the instrument.          */
/*    ViInt32    daq_size (in)                                               */
/*      The number of elements allocated to the results array.               */
/*    ViReal64   _VI_FAR daq_result[] (out)                                  */
/*      Array of 64-bit reals - result from instrument.                      */
/*    ViPInt32   daq_count (out)                                             */
/*      The actual number of elements returned by the instrument             */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    ViUInt32  cnt;
    struct    agb220xa_globals * thisPtr;
    ViChar    lc[100];
    ViInt32   i;
    ViInt32   ArrSize;
    ViInt32   nbytes;
    ViChar    _huge *Array;
	
    GET_GLOBALS(vi,thisPtr);
    agb220xa_CDE_INIT( "agb220xa_cmdReal64Arr_Q" );

    if (daq_size < agb220xa_CMDREAL64ARR_Q_MIN || daq_size > agb220xa_CMDREAL64ARR_Q_MAX)
        return statusUpdate(vi,thisPtr,VI_ERROR_PARAMETER3);

    if ((errStatus = viWrite(vi, (ViBuf)daq_cmd_string,
                             strlen(daq_cmd_string),&cnt))< VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
			
    if ((errStatus = delayRead(vi, thisPtr->myDelay,
                               (ViPBuf)lc, 1, &cnt)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);

    if (lc[0] != '#')
        return statusUpdate(vi,thisPtr,VI_ERROR_INV_EXPR);
		
    if ((errStatus = delayRead(vi, thisPtr->myDelay,
                               (ViPBuf)lc, 1, &cnt)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);

    nbytes = lc[0]-'0';

    if (nbytes)
        {
            if ((errStatus = delayRead(vi, thisPtr->myDelay,
                                       (ViPBuf)lc, nbytes, &cnt)) < VI_SUCCESS)
                return statusUpdate(vi,thisPtr,errStatus);
            lc[cnt]='\0';
            ArrSize = atol(lc);
        }
    else /* indefinate */
        ArrSize = daq_size*sizeof(ViReal64);

#ifdef WIN16
    Array = (ViChar _huge *)halloc(ArrSize, 1 );
#else
    Array = (ViChar *)malloc(ArrSize ); 
#endif
	
    if (!Array)
        return statusUpdate(vi,thisPtr,VI_ERROR_ALLOC);
	
    if ((errStatus = delayRead(vi, thisPtr->myDelay,
                               (ViPBuf)Array, ArrSize, &cnt)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
    if (thisPtr->interfaceType == VI_INTF_GPIB)
        if (errStatus == VI_SUCCESS_MAX_CNT)
            viClear(vi);
    ArrSize = cnt/sizeof(ViReal64);
    ArrSize = (ArrSize > daq_size ) ? daq_size : ArrSize;

    /*copy only as many as the user allows*/
    _fmemcpy(daq_result,Array,(size_t)ArrSize*sizeof(ViReal64) );

#ifdef WIN16
    _hfree(Array);
#else
    free(Array);
#endif

    *daq_count=ArrSize;

    for (i=0; i<*daq_count; i++)
        SWAP_FLOAT64(&daq_result[i]);

    return statusUpdate(vi,thisPtr,VI_SUCCESS);
}        /* ----- end of function ----- */
                                                                         

                                                                        


/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_biasChanCard
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function  will enable  or disable bias on all the output
 *            ports of the specified card.
 *           
 *            The  parameter  'disable_enable'  specifies  select disable or
 *            enable.
 *           
 *            The  parameter  'bias_cardno'  specifies  the choice of one or
 *            all cards.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:BIAS:CHAN:ENAB|DIS:CARD 0|1|2|3|4|ALL
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 disable_enable
 * IN        
 *            Select 0 for enable card,1 for disable card
 * 
 * PARAM 3 : ViInt16 bias_cardno
 * IN        
 *            Select 0 for auto,1 for card1,2 for card2,3 for card3,
 *            4 for card4,5 for card all  - card number
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_biasChanCard_disable_enable_a[] = {
    "ENAB","DIS",0};
static const char * const agb220xa_biasChanCard_bias_cardno_a[] = {"0",
                                                                   "1","2","3","4","ALL",0};
ViStatus _VI_FUNC agb220xa_biasChanCard(ViSession vi,
                                        ViInt16 disable_enable,
                                        ViInt16 bias_cardno)
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_biasChanCard" );

    agb220xa_CHK_ENUM(disable_enable,1,VI_ERROR_PARAMETER2);
    agb220xa_CHK_ENUM(bias_cardno,5,VI_ERROR_PARAMETER3);
    errStatus = viPrintf(vi,":ROUT:BIAS:CHAN:%s:CARD %s\n",agb220xa_biasChanCard_disable_enable_a[disable_enable],agb220xa_biasChanCard_bias_cardno_a[bias_cardno]);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
        }

    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_biasChanList
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function will  enable or disable  bias on all the output
 *            ports specified by the biaschan_list.
 *           
 *            The parameter 'biaschan_disen' will select disable or enable.
 *           
 *            The parameter 'biaschan_list' is an array of integers  with
 *           each integer   representing  one  channel. The  last  number  of
 *            the 'biaschan_list'  should be "0" (Numeric zero)  to  identify
 *            the end of the list. The maximum number of channels that can be
 *            specified by the list is 100.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:BIAS:CHAN:ENAB|DIS (@biaschan_list)
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 biaschan_disen
 * IN        
 *            Select 0 for enable,1 for disable
 * 
 * PARAM 3 : ViInt32 _VI_FAR biaschan_list[]
 * IN        
 *           
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_biasChanList_biaschan_disen_a[] = {
        "ENAB","DIS",0};
ViStatus _VI_FUNC agb220xa_biasChanList(ViSession vi,
                                        ViInt16 biaschan_disen,
                                        ViInt32 _VI_FAR biaschan_list[])
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_biasChanList" );

    agb220xa_CHK_ENUM(biaschan_disen,1,VI_ERROR_PARAMETER2);
    
    {
        ViChar string1[MAX_CHANNEL_LIST_ELEM * 6 + 20 ], tmp[10] ;
        ViInt16 elm;
    
        for (elm = 0 ; biaschan_list[elm] != 0 ; elm++ );

        agb220xa_CHK_INT_RANGE(elm, 1, MAX_CHANNEL_LIST_ELEM, VI_ERROR_PARAMETER3);
     
        if( biaschan_disen == 0 )
            strcpy(string1,":ROUT:BIAS:CHAN:ENAB (@\0");
        if( biaschan_disen == 1 )
            strcpy(string1,":ROUT:BIAS:CHAN:DIS (@\0");
    
        
        while( *biaschan_list != 0 )
        							  
            {
                sprintf(tmp ,"%ld",*biaschan_list);
                strcat(string1,tmp );
    
                biaschan_list++;
    			   
                if  (*biaschan_list == 0)
                    strcat(string1,")");
                else
                    {
                        strcat(string1,",");
                    }  	
            }
    
        errStatus = viPrintf(vi," %s\n",string1);
        if (errStatus < VI_SUCCESS)
    
            {
                agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
            }
    	  
    }
    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_biasChanList_Q
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function  will  query the instrument for the  bias status
 *            for the channels given in the list.
 *           
 *            The  parameter  'bias_disen'  specifies  the  disable or enable
 *            status of the query.
 *           
 *            The parameter 'biaschan_list' is an array of integers with each
 *            integer  representing  one  channel. The  last  number  of  the
 *            'biaschan_list' should be "0"  ( Numeric Zero ) to identify the
 *            end of the list
 *           
 *            The  'bias_status' parameter is an array of integers containing
 *            the return values of the query.The 'bias_status' array returned
 *            will correspond one to one with  'biaschan_list parameter'. The
 *            maximum  number  of channels that can be  specified by the list
 *            is 100.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:BIAS:CHAN:ENAB|DIS (@biaschan_list)
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 bias_disen
 * IN        
 *            Select 0 for enable,1 for disable
 * 
 * PARAM 3 : ViInt32 _VI_FAR biaschan_list[]
 * IN        
 *           
 * 
 * PARAM 4 : ViInt32 _VI_FAR bias_status[]
 * OUT       
 *           
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_biasChanList_Q_bias_disen_a[] = {
    "ENAB","DIS",0};
ViStatus _VI_FUNC agb220xa_biasChanList_Q(ViSession vi,
                                          ViInt16 bias_disen,
                                          ViInt32 _VI_FAR biaschan_list[],
                                          ViInt32 _VI_FAR bias_status[])
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_biasChanList_Q" );

    agb220xa_CHK_ENUM(bias_disen,1,VI_ERROR_PARAMETER2);
    
    {
        ViChar string1[MAX_CHANNEL_LIST_ELEM * 6 + 20 ], tmp[10] ;
        ViChar string2[MAX_CHANNEL_LIST_ELEM * 2 + 20 ] ;
        ViInt16 elm,i;
    
        for (elm = 0 ; biaschan_list[elm] != 0 ; elm++ );
        agb220xa_CHK_INT_RANGE(elm, 1, MAX_CHANNEL_LIST_ELEM, VI_ERROR_PARAMETER3);
    
        if( bias_disen == 0 )
            strcpy(string1,":ROUT:BIAS:CHAN:ENAB? (@\0");
        if( bias_disen == 1 )
            strcpy(string1,":ROUT:BIAS:CHAN:DIS? (@\0");
    
        
        while( *biaschan_list != 0 )
        
            {
                sprintf(tmp ,"%ld",*biaschan_list);
                strcat(string1,tmp );
    
                biaschan_list++;
    			   
                if  (*biaschan_list == 0)
                    strcat(string1,")");
                else
                    {
                        strcat(string1,",");
                    }  	
            }
    
        errStatus = viPrintf(vi," %s\n",string1);
        if (errStatus < VI_SUCCESS)
    
            {
                agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
            }
    		
        errStatus = viScanf(vi,"%s",string2);
    
        if (errStatus < VI_SUCCESS)
     
            {
                agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
            }
    
    		 
        for(i=0;string2[i] != '\0' && i < 100;i++)
    		   
            {
                if (string2 [i] == '1' )
                    *bias_status++ = 1;
                else if (string2 [i] == '0' ) 
                    *bias_status++ = 0;
                else if( string2[i] == ',')
                    ;
                else
                    *bias_status++ = -1;
            }
    		
    }
    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_biasPort
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function  will  select which input port is the bias port
 *            on the specified card. For each card you can specify  the same
 *            or different Bias Port.
 *           
 *            The parameter 'biasport_cardno' specifies one or  all possible
 *            cards.
 *           
 *            The  parameter  'bias_port' specifies  the  input port  as the
 *            bias port.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:BIAS:PORT 0|1|2|3|4|ALL, 1|2|3|4|5|6|7|8|9|10|11|12|13|14
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 biasport_cardno
 * IN        
 *            Select 0 for auto,1 for card1,2 for card2,3 for card3,
 *            4 for card4,5 for card all  - card number
 * 
 * PARAM 3 : ViInt16 bias_port
 * IN        
 *            Selects bias port
 *           
 * 
 *      MAX = agb220xa_PORT_MAX   14
 *      MIN = agb220xa_PORT_MIN   1
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_biasPort_biasport_cardno_a[] = {"0",
                                                                   "1","2","3","4","ALL",0};
ViStatus _VI_FUNC agb220xa_biasPort(ViSession vi,
                                    ViInt16 biasport_cardno,
                                    ViInt16 bias_port)
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_biasPort" );

    agb220xa_CHK_ENUM(biasport_cardno,5,VI_ERROR_PARAMETER2);
    agb220xa_CHK_INT_RANGE(bias_port
                           ,agb220xa_PORT_MIN
                           ,agb220xa_PORT_MAX
                           ,VI_ERROR_PARAMETER3);

    errStatus = viPrintf(vi,":ROUT:BIAS:PORT %s,%d\n",agb220xa_biasPort_biasport_cardno_a[biasport_cardno],bias_port);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
        }
    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_biasState
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function controls the  bias mode for the specified card.
 *            When  Bias  Mode is ON, the  input  Bias Port is connected to
 *            all  bias enabled  output ports that are not connected to any
 *            other  input  ports. Bias  disabled  output ports  are  never
 *            connected to  the input Bias Port when Bias Mode is ON.
 *           
 *            The  parameter   'biasstate_cardno'  specifies   one  or  all
 *            possible cards.
 *           
 *            The parameter 'state' specifies bias mode is ON/OFF.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:BIAS:STAT 0|1|2|3|4|ALL, ON|OFF
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 biasstate_cardno
 * IN        
 *            Select 0 for auto,1 for card1,2 for card2,3 for card3,
 *            4 for card4,5 for card all  - card number
 * 
 * PARAM 3 : ViInt16 state
 * IN        
 *            Select 0 turn OFF bias state
 *            Select 1 turn ON bias state
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_biasState_biasstate_cardno_a[] = {"0",
        "1","2","3","4","ALL",0};
static const char * const agb220xa_biasState_state_a[] = {"OFF","ON",0};
ViStatus _VI_FUNC agb220xa_biasState(ViSession vi,
                                     ViInt16 biasstate_cardno,
                                     ViInt16 state)
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_biasState" );

    agb220xa_CHK_ENUM(biasstate_cardno,5,VI_ERROR_PARAMETER2);
    agb220xa_CHK_ENUM(state,1,VI_ERROR_PARAMETER3);
    errStatus = viPrintf(vi,":ROUT:BIAS:STAT %s,%s\n",agb220xa_biasState_biasstate_cardno_a[biasstate_cardno],agb220xa_biasState_state_a[state]);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
        }

    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_closeCard_Q
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function will query the card for the  channels closed of
 *            the specified card.
 *           
 *            The parameter 'close_card' specifies one of the possible cards.
 *           
 *            The  parameter 'closechan_list' contains  the  channel numbers
 *            returned by the instrument. This will be  an array of integers
 *            terminated by 'zero' to identify the end of the list. Array of
 *            enough  length should be passed to the function.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:CLOS:CARD? 0|1|2|3|4
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 close_card
 * IN        
 *            Select 0 for auto,1 for card1,2 for card2,3 for card3,
 *            4 for card4 - card number
 * 
 * PARAM 3 : ViInt32 _VI_FAR closechan_list[]
 * OUT       
 *           
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_closeCard_Q_close_card_a[] = {"0","1",
                                                                 "2","3","4",0};
ViStatus _VI_FUNC agb220xa_closeCard_Q(ViSession vi,
                                       ViInt16 close_card,
                                       ViInt32 _VI_FAR closechan_list[])
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_closeCard_Q" );

    agb220xa_CHK_ENUM(close_card,4,VI_ERROR_PARAMETER2);
    
    errStatus = viPrintf(vi,":ROUT:CLOS:CARD? %s\n",agb220xa_closeCard_Q_close_card_a[close_card]);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
        }

    
    {
        ViChar string1[20],string2[1220] ;
        ViInt16 i=0,k,m ;
        ViInt32 j;
    
        errStatus = viScanf(vi,"%s",string2);
    
        if (errStatus < VI_SUCCESS)
     
            {
                agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
            }
    
    
        for(m=0,k=0;k<20;k++)
            string1[k] = '\0';
        {
            for(i=0;string2[i] != '\0' && i < 600;i++)
    		   
                {
    
                    if (string2 [i] == '@' )
                        ;			     				
                    else if( string2[i] == ',')
                        {
                            j=atol( string1);
                            *closechan_list++ = j;
                            for(m=0,k=0;k<20;k++)
                                string1[k] = '\0';
                        }
    
                    else  
                        string1[m++] = string2[i];  
                }
      
            j=atol( string1);
            *closechan_list++ = j;
        }
     
     	*closechan_list = 0;
    }
    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_closeList
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function  will  connect  the  input ports  to the	output
 *            ports specified by the channel list.
 *           
 *            The  parameter  'closechan_list'  is an array of integers with
 *            each integer representing  one channel. The last number of the
 *            'closechan_list'  should  be   "0" (Numeric Zero) to  identify
 *            the end of the list.The maximum number of channels that can be
 *            specified by the list is 100.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:CLOS (@closechan_list)
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt32 _VI_FAR closechan_list[]
 * IN        
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC agb220xa_closeList(ViSession vi,
                                     ViInt32 _VI_FAR closechan_list[])
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_closeList" );

    
    {
    
        ViChar string1[MAX_CHANNEL_LIST_ELEM * 6 + 20 ], tmp[10] ;
        ViInt16 elm;
    
        for (elm = 0 ; closechan_list[elm] != 0 ; elm++ );
        agb220xa_CHK_INT_RANGE(elm, 1, MAX_CHANNEL_LIST_ELEM, VI_ERROR_PARAMETER2);
    
        strcpy(string1,":ROUT:CLOS (@\0");
        while( *closechan_list != 0 )
            {
                sprintf(tmp ,"%ld",*closechan_list);
                strcat(string1,tmp );
    
                closechan_list++;
    			   
                if  (*closechan_list == 0)
                    strcat(string1,")");
                else
                    {
                        strcat(string1,",");
                    }  	
            }
    
        errStatus = viPrintf(vi," %s\n",string1);
        
    	if (errStatus < VI_SUCCESS)
    
            {
           	agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
            }
    	  
    }
    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_closeList_Q
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This function will query the instrument for the channels closed
 *            given in the 'closechan_list'.
 *           
 *            The  parameter  'closechan_list' is  an  array of integers with
 *            each integer  representing  one  channel.The last number of the
 *            'closechan_list' should be  "0" (Numeric Zero) to  identify the
 *            end of the list. The  maximum  number of channels  that  can be
 *            specified by the list is 100.
 *           
 *            The 'close_status' parameter is an array of integers containing
 *            the  return  values  of  the  query. The  'close_status'  array
 *            returned will correspond one  to  one with the 'closechan_list'
 *            parameter.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:BIAS:CHAN:ENAB|DIS? (@closechan_list)
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt32 _VI_FAR closechan_list[]
 * IN        
 *           
 * 
 * PARAM 3 : ViInt32 _VI_FAR close_status[]
 * OUT       
 *           
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC agb220xa_closeList_Q(ViSession vi,
                                       ViInt32 _VI_FAR closechan_list[],
                                       ViInt32 _VI_FAR close_status[])
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_closeList_Q" );

    
    {
    
        ViChar string1[MAX_CHANNEL_LIST_ELEM * 6 + 20 ], tmp[10] ;
        ViChar string2[MAX_CHANNEL_LIST_ELEM * 2 + 20 ] ;
        ViInt16 elm,i;
    
        for (elm = 0 ; closechan_list[elm] != 0 ; elm++ );
        agb220xa_CHK_INT_RANGE(elm, 1, MAX_CHANNEL_LIST_ELEM, VI_ERROR_PARAMETER2);
    
        strcpy(string1,":ROUT:CLOS? (@\0");
         
        while( *closechan_list != 0 )
        
            {
                sprintf(tmp ,"%ld",*closechan_list);
                strcat(string1,tmp );
    
                closechan_list++;
    			   
                if  (*closechan_list == 0)
                    strcat(string1,")");
                else
                    {
                        strcat(string1,",");
                    }  	
            }
    
        errStatus = viPrintf(vi," %s\n",string1);
        if (errStatus < VI_SUCCESS)
    
            {
                agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
            }
    		
        errStatus = viScanf(vi,"%s",string2);
    
        if (errStatus < VI_SUCCESS)
     
            {
                agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
            }
    
    		 
        for(i=0;string2[i] != '\0' && i < 100;i++)
    		   
            {
                if (string2 [i] == '1' )
                    *close_status++ = 1;
                else if (string2 [i] == '0' ) 
                    *close_status++ = 0;
                else if( string2[i] == ',')
                    ;
                else
                    *close_status++ = -1;
            }
    		
    }
    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_connRuleSeq
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function  will  help  you  set  connection rule  and the
 *            connection sequence mode. The  function  sets connection  rule
 *            (Free or Single Route) for the specified card.
 *           
 *            The parameter  'cardno_ruleseq' specifies selecting one of the
 *            possible  cards.
 *           
 *            The  parameter  'rule' specifies Normal config  mode or
 *            Auto config mode.
 *           
 *            The  parameter  'sequence'  specifies  No-sequence mode
 *            Break-Before-Make  mode  or  Make-Before-Break  mode.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:CONN:RULE 0|1|2|3|4|ALL,FREE|SROU
 *            :ROUT:CONN:SEQ 0|1|2|3|4|ALL,NSEQ|BBM|MBBR
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 cardno_ruleseq
 * IN        
 *            Select 0 for auto,1 for card1,2 for card2,3 for card3,
 *            4 for card4,5 for cardall  - card number
 * 
 * PARAM 3 : ViInt16 rule
 * IN        
 *            Select 0 for free route,1 for single route
 * 
 * PARAM 4 : ViInt16 sequence
 * IN        
 *            Select 0 for no sequence,1 for break before make,
 *            2 for make before break
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_connRuleSeq_cardno_ruleseq_a[] = {"0",
                                                                     "1","2","3","4","ALL",0};
static const char * const agb220xa_connRuleSeq_rule_a[] = {"FREE","SROU",0};
static const char * const agb220xa_connRuleSeq_sequence_a[] = {"NSEQ",
                                                               "BBM","MBBR",0};
ViStatus _VI_FUNC agb220xa_connRuleSeq(ViSession vi,
                                       ViInt16 cardno_ruleseq,
                                       ViInt16 rule,
                                       ViInt16 sequence)
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_connRuleSeq" );

    agb220xa_CHK_ENUM(cardno_ruleseq,5,VI_ERROR_PARAMETER2);
    agb220xa_CHK_ENUM(rule,1,VI_ERROR_PARAMETER3);
    agb220xa_CHK_ENUM(sequence,2,VI_ERROR_PARAMETER4);
    errStatus = viPrintf(vi,":ROUT:CONN:RULE %s,%s\n",agb220xa_connRuleSeq_cardno_ruleseq_a[cardno_ruleseq],agb220xa_connRuleSeq_rule_a[rule]);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
        }

    errStatus = viPrintf(vi,":ROUT:CONN:SEQ %s,%s\n",agb220xa_connRuleSeq_cardno_ruleseq_a[cardno_ruleseq],agb220xa_connRuleSeq_sequence_a[sequence]);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
        }

    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_couplePort
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This function is used to setup  couple ports which are used for
 *            making kelvin  connections on the specified card. The specified
 *            input port number will be  coupled with the next input port and
 *            two  output  ports. For  each card  you  may  setup the same or
 *            different  couple ports. This  command overwrites the  previous
 *            couple port setting for the card.
 *           
 *           
 *            The parameter 'coupleport_cardno' specifies one or all possible
 *            cards.
 *           
 *            For the  parameters 'port1', 'port3', 'port5', 'port7', 'port9'
 *            equal to "1" specifies  the  input  port number  which will  be
 *            coupled  with the  next input port  and  two  output ports. "0"
 *            specifies that the input port is not a couple port.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:COUP:PORT 0|1|2|3|4|ALL, 1|3|5|7|9|11|13
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 coupleport_cardno
 * IN        
 *            Select 0 for auto,1 for card1,2 for card2,3 for card3,
 *            4 for card4,5 for card all  - card number
 * 
 * PARAM 3 : ViInt16 port1
 * IN        
 *            select 0 Couple Port1 off
 *            select 1 Couple Port1 on
 *           
 * 
 * PARAM 4 : ViInt16 port3
 * IN        
 *            select 0 Couple Port3 off
 *            select 1 Couple Port3 on
 *           
 * 
 * PARAM 5 : ViInt16 port5
 * IN        
 *            select 0 Couple Port5 off
 *            select 1 Couple Port5 on
 *           
 * 
 * PARAM 6 : ViInt16 port7
 * IN        
 *            select 0 Couple Port7 off
 *            select 1 Couple Port7 on
 *           
 * 
 * PARAM 7 : ViInt16 port9
 * IN        
 *            select 0 Couple Port9 off
 *            select 1 Couple Port9 on
 *           
 * PARAM 8 : ViInt16 port11
 * IN        
 *            select 0 Couple Port11 off
 *            select 1 Couple Port11 on
 *
 * PARAM 9 : ViInt16 port13
 * IN        
 *            select 0 Couple Port13 off
 *            select 1 Couple Port13 on
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_couplePort_coupleport_cardno_a[] = {
    "0","1","2","3","4","ALL",0};
static const char * const agb220xa_couplePort_port1_a[] = {"0","1",0};
static const char * const agb220xa_couplePort_port3_a[] = {"0","1",0};
static const char * const agb220xa_couplePort_port5_a[] = {"0","1",0};
static const char * const agb220xa_couplePort_port7_a[] = {"0","1",0};
static const char * const agb220xa_couplePort_port9_a[] = {"0","1",0};
static const char * const agb220xa_couplePort_port11_a[] = {"0","1",0};
static const char * const agb220xa_couplePort_port13_a[] = {"0","1",0};
ViStatus _VI_FUNC agb220xa_couplePort(ViSession vi,
                                      ViInt16 coupleport_cardno,
                                      ViInt16 port1,
                                      ViInt16 port3,
                                      ViInt16 port5,
                                      ViInt16 port7,
                                      ViInt16 port9,
                                      ViInt16 port11,
                                      ViInt16 port13)
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_couplePort" );

    agb220xa_CHK_ENUM(coupleport_cardno,5,VI_ERROR_PARAMETER2);
    agb220xa_CHK_ENUM(port1,1,VI_ERROR_PARAMETER3);
    agb220xa_CHK_ENUM(port3,1,VI_ERROR_PARAMETER4);
    agb220xa_CHK_ENUM(port5,1,VI_ERROR_PARAMETER5);
    agb220xa_CHK_ENUM(port7,1,VI_ERROR_PARAMETER6);
    agb220xa_CHK_ENUM(port9,1,VI_ERROR_PARAMETER7);
    agb220xa_CHK_ENUM(port11,1,VI_ERROR_PARAMETER8);
    agb220xa_CHK_ENUM(port13,1,agb220xa_INSTR_ERROR_PARAMETER9);
    
    {
        ViChar strg[50];
        ViInt16 flag = 0;
    
        strcpy(strg,":ROUT:COUP:PORT \0");
    	  	
        if (coupleport_cardno == 0)
    	    { 	
                strcat(strg,"0");
    	    }
    
        if (coupleport_cardno == 1)
    	    {
                strcat(strg,"1");
    	    }
    
        if (coupleport_cardno == 2)
    	    {
                strcat(strg,"2");
    	    }
    
        if (coupleport_cardno == 3)
    	    {
                strcat(strg,"3");
    	    }
    
        if (coupleport_cardno == 4)
    	    {
                strcat(strg,"4");
    	    }
    
        if (coupleport_cardno == 5)
    	    {
                strcat(strg,"ALL");
    	    }
    
        strcat(strg,",'");
    	       	
    
        if (port1==1)
    			  	
    	    {
                strcat(strg,"1");
                flag=1;
       	    }	
    
    
        if (port3==1)
    			  	
    	    {
                if (flag == 1)
                    strcat(strg,",3");
                else
                    strcat(strg,"3");
                flag=1;	
    	    }
    
    
        if (port5==1)
    			  	
    	    {
                if (flag == 1)
                    strcat(strg,",5");
                else
                    strcat(strg,"5");
                flag=1;	
    	    }
    
        if (port7==1)
    			  	
    	    {
                if (flag == 1)
                    strcat(strg,",7");
                else
                    strcat(strg,"7");
                flag=1;	
    	    }
    
        if (port9==1)
    			  	
    	    {
                if (flag == 1)
                    strcat(strg,",9");
                else
                    strcat(strg,"9");
                flag=1;	
    	    }
    
        if (port11==1)
    			  	
    	    {
                if (flag == 1)
                    strcat(strg,",11");
                else
                    strcat(strg,"11");
                flag=1;	
    	    }

        if (port13==1)
    			  	
    	    {
                if (flag == 1)
                    strcat(strg,",13");
                else
                    strcat(strg,"13");
                flag=1;	
    	    }

        strcat(strg,"'");
    
        errStatus = viPrintf(vi," %s\n",strg);
        if (errStatus < VI_SUCCESS)
    
            {
                agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
            }
    }
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_coupleState
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This function controls the  couple port mode for the specified
 *            card. 
 *           
 *            The   parameter  'couplestate_cardno'  specifies  one  or  all
 *            possible cards.
 *           
 *            The parameter  'couple_state'  specifies  couple port mode  is
 *            ON/OFF.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:COUP:STAT 0|1|2|3|4|ALL, ON|OFF
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 couplestate_cardno
 * IN        
 *            Select 0 for auto,1 for card1,2 for card2,3 for card3,
 *            4 for card4,5 for card all  - card number
 * 
 * PARAM 3 : ViInt16 couple_state
 * IN        
 *            Select 0 turn OFF couple port mode
 *            Select 1 turn ON couple port mode
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_coupleState_couplestate_cardno_a[] = {
    "0","1","2","3","4","ALL",0};
static const char * const agb220xa_coupleState_couple_state_a[] = {"OFF",
                                                                   "ON",0};
ViStatus _VI_FUNC agb220xa_coupleState(ViSession vi,
                                       ViInt16 couplestate_cardno,
                                       ViInt16 couple_state)
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_coupleState" );

    agb220xa_CHK_ENUM(couplestate_cardno,5,VI_ERROR_PARAMETER2);
    agb220xa_CHK_ENUM(couple_state,1,VI_ERROR_PARAMETER3);
    errStatus = viPrintf(vi,":ROUT:COUP:STAT %s,%s\n",agb220xa_coupleState_couplestate_cardno_a[couplestate_cardno],agb220xa_coupleState_couple_state_a[couple_state]);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
        }

    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_esr_Q
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE : 
 *            This function implements the following SCPI commands:
 *            *ESR?
 *           
 *           Returns the contents of the ESR register.
 *           The driver returns the equivalent messages:
 *               Bit Value        Message
 *           
 *                   0             ""
 *                   1           "ESR_OPC"
 *                   2           "ESR_RQL"
 *                   4           "ESR_QYE_ERROR"
 *                   8           "ESR_DEVICE_DEPENDENT_ERROR"
 *                  16           "ESR_EXECUTION _ERROR"
 *                  32           "ESR_COMMMAND_ERROR"
 *                  64           "ESR_URQ"
 *                 128           "ESR_PON"
 *               OTHERS          "ESR_MULTI_EVENT"
 *           
 *           
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViChar _VI_FAR errstr[]
 * OUT       
 *            Response from instrument
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC agb220xa_esr_Q(ViSession vi,
                                 ViChar _VI_FAR errstr[])
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_esr_Q" );

    {
     
        ViInt32 esrval;
        errStatus = viPrintf(vi,"*ESR?\n");
        if (errStatus < VI_SUCCESS)
            {
                agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
            }

        errStatus = viScanf(vi,"%ld%*t",&esrval);
        if (errStatus < VI_SUCCESS)
            {
                agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
            }
    
        switch (esrval)
            {
    
            case 0:
                strcpy( errstr, "" );
                break;
    
            case 1:
                strcpy( errstr, "ESR_OPC" );
                break;
    
            case 2:
                strcpy( errstr, "ESR_RQL");
                break; 
            case 4: 
                strcpy( errstr, "ESR_QYE_ERROR" );
                break;
    
            case 8:
                strcpy( errstr, "ESR_DEVICE_DEPENDENT_ERROR" );
                break; 
            case 16:
                strcpy( errstr, "ESR_EXECUTION_ERROR" );
                break; 
            case 32: 
                strcpy( errstr, "ESR_COMMAND_ERROR" );
                break;
           
            case 64:
                strcpy( errstr, "ESR_URQ");
                break; 
            case 128: 
                strcpy( errstr, "ESR_PON" );
                break; 
           
            default:
                strcpy( errstr, "ESR_MULTI_EVENT" );
                break;
            }
     
    }
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_func
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function  is  used  to set the  channel configuration to
 *            Auto Config Mode or Normal Config Mode.
 *           
 *            The   parameter  'channel_config'  specifies  Auto  or  Normal
 *            configuration.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:FUNC ACON|NCON
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 channel_config
 * IN        
 *            Select 0 for auto configuration
 *            Select 1 for normal configuration
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_func_channel_config_a[] = {"ACON",
                                                              "NCON",0};
ViStatus _VI_FUNC agb220xa_func(ViSession vi,
                                ViInt16 channel_config)
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_func" );

    agb220xa_CHK_ENUM(channel_config,1,VI_ERROR_PARAMETER2);
    errStatus = viPrintf(vi,":ROUT:FUNC %s\n",agb220xa_func_channel_config_a[channel_config]);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
        }

    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_openCard
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function will disconnect all input ports from all output
 *            ports  for the specified card.Then if Bias Mode is ON,connects
 *            the input Bias Port to all bias enabled output ports.
 *           
 *            The  parameter  'open_cardno'  specifies  one  or all possible
 *            cards.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:OPEN:CARD 0|1|2|3|4|ALL
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 open_cardno
 * IN        
 *            Select 0 for auto,1 for card1,2 for card2,3 for card3,
 *            4 for card4,5 for card all  - card number
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_openCard_open_cardno_a[] = {"0","1",
                                                               "2","3","4","ALL",0};
ViStatus _VI_FUNC agb220xa_openCard(ViSession vi,
                                    ViInt16 open_cardno)
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_openCard" );

    agb220xa_CHK_ENUM(open_cardno,5,VI_ERROR_PARAMETER2);
    errStatus = viPrintf(vi,":ROUT:OPEN:CARD %s\n",agb220xa_openCard_open_cardno_a[open_cardno]);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
        }

    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_openList
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This function will disconnect the input ports  from the output
 *            ports specified by the channel list.
 *           
 *            The  parameter 'openchan_list' is  an  array of integers  with
 *            each  integer representing one channel. The last number of the
 *            'openchan_list'  should be  "0" ( Numeric Zero )  to  identify
 *            the end of the list.The maximum number of channels that can be
 *            specified by the list is 100.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:OPEN (@openchan_list)
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt32 _VI_FAR openchan_list[]
 * IN        
 *           
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC agb220xa_openList(ViSession vi,
                                    ViInt32 _VI_FAR openchan_list[])
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_openList" );

    
    {
        ViChar string1[MAX_CHANNEL_LIST_ELEM * 6 + 20 ], tmp[10] ;
        ViInt16 elm;
    
        for (elm = 0 ; openchan_list[elm] != 0 ; elm++ );
        agb220xa_CHK_INT_RANGE(elm, 1, MAX_CHANNEL_LIST_ELEM, VI_ERROR_PARAMETER2);
    
    
        strcpy(string1,":ROUT:OPEN (@\0");
        
        while( *openchan_list != 0 )
        
            {
                sprintf(tmp ,"%ld",*openchan_list);
                strcat(string1,tmp );
    
                openchan_list++;
    			   
                if  (*openchan_list == 0)
                    strcat(string1,")");
                else
                    {
                        strcat(string1,",");
                    }  	
    
            }
    
    
        errStatus = viPrintf(vi," %s\n",string1);
        if (errStatus < VI_SUCCESS)
     
            {
                agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
            }
    	  
    }
    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_openList_Q
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This function will query the  instrument for the  channels open
 *            given in the 'openchan_list'.
 *           
 *            The  parameter  'openchan_list' is  an  array of integers  with
 *            each integer representing one  channel. The  last number of the
 *            'openchan_list' should be  "0" ( Numeric Zero ) to identify the
 *            end of the list. The  maximum  number  of  channels that can be
 *            specified by the list is 100.
 *           
 *            The 'open_status' parameter is an  array of integers containing
 *            the return values of the query.The 'open_status' array returned
 *            will  correspond  one to one with the 'openchan_list'
 *           parameter.
 *            This function implements the following SCPI commands:
 *            :ROUT:OPEN? (@openchan_list)
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt32 _VI_FAR openchan_list[]
 * IN        
 * 
 * PARAM 3 : ViInt32 _VI_FAR open_status[]
 * OUT       
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC agb220xa_openList_Q(ViSession vi,
                                      ViInt32 _VI_FAR openchan_list[],
                                      ViInt32 _VI_FAR open_status[])
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_openList_Q" );

    
    {
        ViChar string1[MAX_CHANNEL_LIST_ELEM * 6 + 20 ], tmp[10] ;
        ViChar string2[MAX_CHANNEL_LIST_ELEM * 2 + 20 ] ;
        ViInt16 elm,i;
    
        for (elm = 0 ; openchan_list[elm] != 0 ; elm++ );
        agb220xa_CHK_INT_RANGE(elm, 1, MAX_CHANNEL_LIST_ELEM, VI_ERROR_PARAMETER2);
    
        strcpy(string1,":ROUT:OPEN? (@\0");
         
        while( *openchan_list != 0 )
        
            {
                sprintf(tmp ,"%ld",*openchan_list);
                strcat(string1,tmp );
    
                openchan_list++;
    			   
                if  (*openchan_list == 0)
                    strcat(string1,")");
                else
                    {
                        strcat(string1,",");
                    }  	
    
            }
    
        errStatus = viPrintf(vi," %s\n",string1);
        if (errStatus < VI_SUCCESS)
    
            {
                agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
            }
    		
        errStatus = viScanf(vi,"%s",string2);
    
        if (errStatus < VI_SUCCESS)
     
            {
                agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
            }
    
    		 
        for(i=0;string2[i] != '\0' && i < 100;i++)
    		   
            {
                if (string2 [i] == '1' )
                    *open_status++ = 1;
                else if (string2 [i] == '0' ) 
                    *open_status++ = 0;
                else if( string2[i] == ',')
                    ;
                else
                    *open_status++ = -1;
            }
    		
    }
    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_testClear
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This function clears the test result for the specified relay
 *            card or the front panel or the controller.
 *           
 *            The  parameter 'framecard_clear' specifies the choice of the
 *            test result to be cleared.
 *           
 *            This function implements the following SCPI commands:
 *            :DIAG:TEST:CARD:CLE 1|2|3|4|ALL
 *            :DIAG:TEST:FRAM:CLE CONT|FPAN
 *           
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 framecard_clear
 * IN        
 *            Select 0 to clear test results of all relay cards,
 *            frontpanel and controller.
 *            Select 1 for relay card no. 1 only
 *            Select 2 for relay card no. 2 only
 *            Select 3 for relay card no. 3 only
 *            Select 4 for relay card no. 4 only
 *            Select 5 for all the 4 relay cards
 *            Select 6 for keyboard
 *            Select 7 for controller
 *            Select 8 for light pen
 *            Select 9 for GPIB
 *            Select 10 for LED
 *            Select 11 for Beeper
 *           
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_testClear_framecard_clear_a[] = {"0",
                                                                    "1","2","3","4","5","6","7","8","9","10",0};
ViStatus _VI_FUNC agb220xa_testClear(ViSession vi,
                                     ViInt16 framecard_clear)
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_testClear" );

    agb220xa_CHK_ENUM(framecard_clear,10,VI_ERROR_PARAMETER2);
     
    switch (framecard_clear)
        {
        case 0:
            errStatus = viPrintf(vi,":DIAG:TEST:CARD:CLE ALL;:DIAG:TEST:FRAM:CLE CONT;:DIAG:TEST:FRAM:CLE FPAN;:DIAG:TEST:FRAM:CLE LED;:DIAG:TEST:FRAM:CLE PEN;:DIAG:TEST:FRAM:CLE BEEP;:DIAG:TEST:FRAM:CLE GPIB\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 1:
            errStatus = viPrintf(vi,":DIAG:TEST:CARD:CLE 1\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 2:
            errStatus = viPrintf(vi,":DIAG:TEST:CARD:CLE 2\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 3:
            errStatus = viPrintf(vi,":DIAG:TEST:CARD:CLE 3\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 4:
            errStatus = viPrintf(vi,":DIAG:TEST:CARD:CLE 4\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 5:
            errStatus = viPrintf(vi,":DIAG:TEST:CARD:CLE ALL\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 6:
            errStatus = viPrintf(vi,":DIAG:TEST:FRAM:CLE FPAN\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 7:
            errStatus = viPrintf(vi,":DIAG:TEST:FRAM:CLE CONT\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 8:
            errStatus = viPrintf(vi,":DIAG:TEST:FRAM:CLE PEN\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 9:
            errStatus = viPrintf(vi,":DIAG:TEST:FRAM:CLE LED\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 10:
            errStatus = viPrintf(vi,":DIAG:TEST:FRAM:CLE BEEP\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        }       
               
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_testExec_Q
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This function will execute relay test for the specified card or
 *            the controller  or  the front panel. You must attach the  relay
 *            test adapter before executing the relay card test.
 *            The Front Panel  test  requires the  key to be  pressed  within
 *            10 seconds else the test will fail.
 *           
 *            The   parameter    'framecard_exec'   specifies  relay card  or
 *            frontpanel or controller.
 *           
 *            The  parameter  'exec_result'  will  store the pass fail
 *           result.
 *            This function implements the following SCPI commands:
 *            :DIAG:TEST:CARD? 1|2|3|4|ALL
 *            :DIAG:TEST:FRAM? FPAN|CONT
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 framecard_exec
 * IN        
 *            Select 1 for relay card no. 1 only
 *            Select 2 for relay card no. 2 only
 *            Select 3 for relay card no. 3 only
 *            Select 4 for relay card no. 4 only
 *            Select 5 for all the 4 relay cards
 *            Select 6 for keyboard
 *            Select 7 for controller
 *            Select 8 for light pen
 *            Select 9 for LED
 *            Select 10 for Beeper
 *           
 * 
 * PARAM 3 : ViPInt16 exec_result
 * OUT       
 *            Result of command.
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_testExec_Q_framecard_exec_a[] = {"","1",
                                                                    "2","3","4","5","6","7","8","9","10",0};
ViStatus _VI_FUNC agb220xa_testExec_Q(ViSession vi,
                                      ViInt16 framecard_exec,
                                      ViPInt16 exec_result)
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_testExec_Q" );

    agb220xa_CHK_INT_RANGE(framecard_exec,1,10,VI_ERROR_PARAMETER2);
    
    
    switch (framecard_exec)
    
        {
        case 1:
            thisPtr->blockSrqIO = VI_TRUE;
            errStatus = viPrintf(vi,":DIAG:TEST:CARD? 1\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            errStatus = viScanf(vi,"%hd%*t",exec_result);
            if( thisPtr ) doDelay(thisPtr->myDelay);
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 2:
            thisPtr->blockSrqIO = VI_TRUE;
            errStatus = viPrintf(vi,":DIAG:TEST:CARD? 2\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            errStatus = viScanf(vi,"%hd%*t",exec_result);
            if( thisPtr ) doDelay(thisPtr->myDelay);
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 3:
            thisPtr->blockSrqIO = VI_TRUE;
            errStatus = viPrintf(vi,":DIAG:TEST:CARD? 3\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            errStatus = viScanf(vi,"%hd%*t",exec_result);
            if( thisPtr ) doDelay(thisPtr->myDelay);
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 4:
            thisPtr->blockSrqIO = VI_TRUE;
            errStatus = viPrintf(vi,":DIAG:TEST:CARD? 4\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            errStatus = viScanf(vi,"%hd%*t",exec_result);
            if( thisPtr ) doDelay(thisPtr->myDelay);
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 5:
            thisPtr->blockSrqIO = VI_TRUE;
            errStatus = viPrintf(vi,":DIAG:TEST:CARD? ALL\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            errStatus = viScanf(vi,"%hd%*t",exec_result);
            if( thisPtr ) doDelay(thisPtr->myDelay);
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 6:
            thisPtr->blockSrqIO = VI_TRUE;
            errStatus = viPrintf(vi,":DIAG:TEST:FRAM? FPAN\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            errStatus = viScanf(vi,"%hd%*t",exec_result);
            if( thisPtr ) doDelay(thisPtr->myDelay);
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 7:
            thisPtr->blockSrqIO = VI_TRUE;
            errStatus = viPrintf(vi,":DIAG:TEST:FRAM? CONT\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            errStatus = viScanf(vi,"%hd%*t",exec_result);
            if( thisPtr ) doDelay(thisPtr->myDelay);
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 8:
            thisPtr->blockSrqIO = VI_TRUE;
            errStatus = viPrintf(vi,":DIAG:TEST:FRAM? PEN\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            errStatus = viScanf(vi,"%hd%*t",exec_result);
            if( thisPtr ) doDelay(thisPtr->myDelay);
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 9:
            thisPtr->blockSrqIO = VI_TRUE;
            errStatus = viPrintf(vi,":DIAG:TEST:FRAM? LED\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            errStatus = viScanf(vi,"%hd%*t",exec_result);
            if( thisPtr ) doDelay(thisPtr->myDelay);
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
        case 10:
            thisPtr->blockSrqIO = VI_TRUE;
            errStatus = viPrintf(vi,":DIAG:TEST:FRAM? BEEP\n");
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            errStatus = viScanf(vi,"%hd%*t",exec_result);
            if( thisPtr ) doDelay(thisPtr->myDelay);
            if (errStatus < VI_SUCCESS)
                {
                    agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
                }

            break;
    
        }
    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}


/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_selectCompenFile
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This function reads the user defined C compensation coefficient 
 *            data table file. And the driver internal coefficient data table 
 *            will be overwtitten by read data.
 *            At initial condition, the default coeficient data is used.
 *           
 *            The parameter 'file_name' specifies the data table file name 
 *            with absolute path.
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViString file_name
 * IN        
 *            Specify the data table file name with absolute path.
 *            If this parameter is NULL string, the default data table will 
 *            be used.
 *           
 * RETURN  : VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC agb220xa_selectCompenFile(ViSession vi,
                                            ViString file_name)
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS) {
        agb220xa_LOG_STATUS( vi, 0, errStatus );
    }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_selectCompenFile" );

    if ( strlen( file_name ) == 0 ) { 
        thisPtr->mother_b = def_mother_board;
        if (thisPtr->card_type == CARD_B2210A){
            thisPtr->matrix_h = def_matrix_h_2210A;
            thisPtr->matrix_l = def_matrix_l_2210A;
        } else {
            thisPtr->matrix_h = def_matrix_h_2211A;
            thisPtr->matrix_l = def_matrix_l_2211A;
        }
        thisPtr->cable1   = def_cable1;
        thisPtr->cable2_h = def_cable_2;
        thisPtr->cable2_l = def_cable_2;
        thisPtr->cable3_h = def_cable_3;
        thisPtr->cable3_l = def_cable_3;
    } else {
        FILE *fp;
        ViChar buf[256], tmp[256], tmp2[256], rev[256], cmd[16], rest[256];
       
        fp = fopen( file_name, "r" );
        if ( fp == NULL ) {
            agb220xa_LOG_STATUS( vi, thisPtr, agb220xa_FILE_OPEN_ERROR );
        }

        while (fgets(buf, 255, fp)){
            /* gets first token */
            sscanf(buf, "%s %s", cmd, rest);

            if (cmd[0] == '#'){
                /* comment. skip it */
                continue;
            }

            if (!strcmp(cmd, "format")){
                /* format revision */
                sscanf(buf, "%s %s %s", tmp, tmp2, rev);
                if (strcmp(rev, "1.0")){
                    fclose(fp);
                    agb220xa_LOG_STATUS( vi, thisPtr, agb220xa_FILE_REV_ERROR );
                }
                continue;
            }

            if (strstr(cmd, "B221")){
                /* matrix type */
                if (!strcmp(cmd, "B2210A")){
                    thisPtr->card_type = CARD_B2210A;
                } else if (!strcmp(cmd, "B2211A")){
                    thisPtr->card_type = CARD_B2211A;
                } else {
                    fclose(fp);
                    agb220xa_LOG_STATUS( vi, thisPtr, agb220xa_FILE_REV_ERROR );
                }
                continue;
            }

            /* connection type */
            if (!strcmp(cmd, "PCIF")){
                thisPtr->pcif = VI_TRUE;
                continue;
            } else if (!strcmp(cmd, "CABLE")){
                thisPtr->pcif = VI_FALSE;
                continue;
            }

            if (!strcmp(cmd, "MB")){
                sscanf(buf, "%s %le %le %le", tmp2,
                       &(thisPtr->mother_b.R), &(thisPtr->mother_b.L), &(thisPtr->mother_b.C));
                continue;
            }

            if (!strcmp(cmd, "MH")){
                sscanf(buf, "%s %le %le %le", tmp2,
                       &thisPtr->matrix_h.R, &thisPtr->matrix_h.L, &thisPtr->matrix_h.C);
                continue;
            }

            if (!strcmp(cmd, "ML")){
                sscanf(buf, "%s %le %le %le", tmp2,
                       &thisPtr->matrix_l.R, &thisPtr->matrix_l.L, &thisPtr->matrix_l.C);
                continue;
            }

            if (!strcmp(cmd, "C1")){
                sscanf(buf, "%s %le %le %le", tmp2,
                       &thisPtr->cable1.R, &thisPtr->cable1.L, &thisPtr->cable1.C);
                continue;
            }

            if (!strcmp(cmd, "C2H")){
                sscanf(buf, "%s %le %le %le", tmp2,
                       &thisPtr->cable2_h.R, &thisPtr->cable2_h.L, &thisPtr->cable2_h.C);
                continue;
            }

            if (!strcmp(cmd, "C2L")){
                sscanf(buf, "%s %le %le %le", tmp2,
                       &thisPtr->cable2_l.R, &thisPtr->cable2_l.L, &thisPtr->cable2_l.C);
                continue;
            }

            if (!strcmp(cmd, "C3H")){
                sscanf(buf, "%s %le %le %le", tmp2,
                       &thisPtr->cable3_h.R, &thisPtr->cable3_h.L, &thisPtr->cable3_h.C);
                continue;
            }

            if (!strcmp(cmd, "C3L")){
                sscanf(buf, "%s %le %le %le", tmp2,
                       &thisPtr->cable3_l.R, &thisPtr->cable3_l.L, &thisPtr->cable3_l.C);
                continue;
            }
        }
    }
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}


/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_compenC
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This function returns the compensated capacitance/conductance 
 *            value 
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViReal64 frequency
 * IN        
 *            Specify the signal frequency when raw data measured.
 *           
 * PARAM 3 : ViInt16  card_type
 * IN        
 *            Specify the card type.
 *
 * PARAM 4 : ViReal64 len_cable
 * IN        
 *            Specify the cable length[m].
 *           
 * PARAM 5 : ViReal64 raw_c
 * IN        
 *            Specify the measured capacitance value.
 *           
 * PARAM 6 : ViReal64 raw_g
 * IN        
 *            Specify the measured conductance value.
 *           
 * PARAM 7 : ViPReal64 compen_c
 * OUT        
 *            Specify the compensated capacitance value.
 *           
 * PARAM 8 : ViPReal64 compen_g
 * OUT        
 *            Specify the compensated conductance value.
 *           
 * RETURN  : VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC agb220xa_compenC(ViSession vi,
                                   ViReal64 frequency,
                                   ViReal64 raw_c,
                                   ViReal64 raw_g,
                                   ViPReal64 compen_c,
                                   ViPReal64 compen_g)
{
#define PI 3.14159265358979323846

    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;
    ViReal64 omega = 2.0 * PI * frequency;
#ifdef DEBUG_OUTPUT
    char title[16];
    fp = fopen("debug.log", "w+");
#endif

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS) {
        agb220xa_LOG_STATUS( vi, 0, errStatus );
    }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_compenC" );

    agb220xa_CHK_REAL_RANGE(frequency, agb220xa_FREQ_MIN, agb220xa_FREQ_MAX, 
                            VI_ERROR_PARAMETER2);
    
    {
	C2x2 FMat[10];
	CComp Zm, Zt, temp;
	C2x2 Fh, Fl;

	Zm = CComp(1, 0) / CComp(raw_g, omega * raw_c);

	FMat[0] = FMatrix(thisPtr->mother_b.R, thisPtr->mother_b.L, thisPtr->mother_b.C, omega);
	FMat[1] = FMatrix(thisPtr->matrix_h.R, thisPtr->matrix_h.L, thisPtr->matrix_h.C, omega);
	FMat[2] = FMatrix(thisPtr->cable1.R,   thisPtr->cable1.L,   thisPtr->cable1.C,   omega);
	FMat[3] = FMatrix(thisPtr->cable2_h.R, thisPtr->cable2_h.L, thisPtr->cable2_h.C, omega);
	FMat[4] = FMatrix(thisPtr->cable3_h.R, thisPtr->cable3_h.L, thisPtr->cable3_h.C, omega);

	FMat[5] = FMatrix(thisPtr->mother_b.R, thisPtr->mother_b.L, thisPtr->mother_b.C, omega);
	FMat[6] = FMatrix(thisPtr->matrix_l.R, thisPtr->matrix_l.L, thisPtr->matrix_l.C, omega);
	FMat[7] = FMatrix(thisPtr->cable1.R,   thisPtr->cable1.L,   thisPtr->cable1.C,   omega);
        FMat[8] = FMatrix(thisPtr->cable2_l.R, thisPtr->cable2_l.L, thisPtr->cable2_l.C, omega);
	FMat[9] = FMatrix(thisPtr->cable3_l.R, thisPtr->cable3_l.L, thisPtr->cable3_l.C, omega);

	Fh = FMat[0] * FMat[1] * FMat[2] * FMat[3] * FMat[4];
	Fl = FMat[9] * FMat[8] * FMat[7] * FMat[6] * FMat[5];

	Zt = (Fh.a * Fl.b + Fh.b * Fl.d + Zm) / (Fh.a * Fl.d);
	temp = CComp(1, 0) / Zt;
	*compen_c = temp.i / omega;
	*compen_g = temp.r;

#ifdef DEBUG_OUTPUT
        fprintf(fp, "mother_b.R  = %le\n", thisPtr->mother_b.R);
        fprintf(fp, "mother_b.L  = %le\n", thisPtr->mother_b.L);
        fprintf(fp, "mother_b.C  = %le\n", thisPtr->mother_b.C);
        fprintf(fp, "matrix_h.R  = %le\n", thisPtr->matrix_h.R);
        fprintf(fp, "matrix_h.L  = %le\n", thisPtr->matrix_h.L);
        fprintf(fp, "matrix_h.C  = %le\n", thisPtr->matrix_h.C);
        fprintf(fp, "matrix_l.R  = %le\n", thisPtr->matrix_l.R);
        fprintf(fp, "matrix_l.L  = %le\n", thisPtr->matrix_l.L);
        fprintf(fp, "matrix_l.C  = %le\n", thisPtr->matrix_l.C);
        fprintf(fp, "cable1.R    = %le\n", thisPtr->cable1.R);
        fprintf(fp, "cable1.L    = %le\n", thisPtr->cable1.L);
        fprintf(fp, "cable1.C    = %le\n", thisPtr->cable1.C);
        fprintf(fp, "cable2_h.R  = %le\n", thisPtr->cable2_h.R);
        fprintf(fp, "cable2_h.L  = %le\n", thisPtr->cable2_h.L);
        fprintf(fp, "cable2_h.C  = %le\n", thisPtr->cable2_h.C);
        fprintf(fp, "cable2_l.R  = %le\n", thisPtr->cable2_l.R);
        fprintf(fp, "cable2_l.L  = %le\n", thisPtr->cable2_l.L);
        fprintf(fp, "cable2_l.C  = %le\n", thisPtr->cable2_l.C);
        fprintf(fp, "cable3_h.R  = %le\n", thisPtr->cable3_h.R);
        fprintf(fp, "cable3_h.L  = %le\n", thisPtr->cable3_h.L);
        fprintf(fp, "cable3_h.C  = %le\n", thisPtr->cable3_h.C);
        fprintf(fp, "cable3_l.R  = %le\n", thisPtr->cable3_l.R);
        fprintf(fp, "cable3_l.L  = %le\n", thisPtr->cable3_l.L);
        fprintf(fp, "cable3_l.C  = %le\n", thisPtr->cable3_l.C);
        fprintf(fp, "pcif        = %d\n" , thisPtr->pcif);
        fprintf(fp, "card_type   = %d\n",  thisPtr->card_type);
        fprintf(fp, "\n");
        /* fclose(fp); */
#endif /* DEBUG_OUTPUT */
    }

    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_groundPort
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function  will  select which input port is the ground port
 *            on the specified card. For each card you can specify  the same
 *            or different Ground Port.
 *           
 *            The parameter 'groundport_cardno' specifies one or  all possible
 *            cards.
 *           
 *            The  parameter  'ground_port' specifies  the  input port  as the
 *            ground port.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:AGND:PORT 0|1|2|3|4|ALL, 1|2|3|4|5|6|7|8|9|10|11|12|13|14
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 groundport_cardno
 * IN        
 *            Select 0 for auto,1 for card1,2 for card2,3 for card3,
 *            4 for card4,5 for card all  - card number
 * 
 * PARAM 3 : ViInt16 ground_port
 * IN        
 *            Selects ground port
 *           
 * 
 *      MAX = agb220xa_PORT_MAX   14
 *      MIN = agb220xa_PORT_MIN   1
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_groundPort_groundport_cardno_a[] = {"0",
                                                                       "1","2","3","4","ALL",0};
ViStatus _VI_FUNC agb220xa_groundPort(ViSession vi,
                                      ViInt16 groundport_cardno,
                                      ViInt16 ground_port)
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_groundPort" );

    agb220xa_CHK_ENUM(groundport_cardno,5,VI_ERROR_PARAMETER2);
    agb220xa_CHK_INT_RANGE(ground_port
                           ,agb220xa_PORT_MIN
                           ,agb220xa_PORT_MAX
                           ,VI_ERROR_PARAMETER3);

    errStatus = viPrintf(vi,":ROUT:AGND:PORT %s,%d\n",agb220xa_groundPort_groundport_cardno_a[groundport_cardno],ground_port);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
        }
    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}


/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_groundChanCard
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function  will enable  or disable ground on all the output
 *            ports of the specified card.
 *           
 *            The  parameter  'disable_enable'  specifies  select disable or
 *            enable.
 *           
 *            The  parameter  'ground_cardno'  specifies  the choice of one or
 *            all cards.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:AGND:CHAN:ENAB|DIS:CARD 0|1|2|3|4|ALL
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 disable_enable
 * IN        
 *            Select 0 for enable card,1 for disable card
 * 
 * PARAM 3 : ViInt16 ground_cardno
 * IN        
 *            Select 0 for auto,1 for card1,2 for card2,3 for card3,
 *            4 for card4,5 for card all  - card number
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_groundChanCard_disable_enable_a[] = {
    "ENAB","DIS",0};
static const char * const agb220xa_groundChanCard_ground_cardno_a[] = {"0",
                                                                       "1","2","3","4","ALL",0};
ViStatus _VI_FUNC agb220xa_groundChanCard(ViSession vi,
                                          ViInt16 disable_enable,
                                          ViInt16 ground_cardno)
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_groundChanCard" );

    agb220xa_CHK_ENUM(disable_enable,1,VI_ERROR_PARAMETER2);
    agb220xa_CHK_ENUM(ground_cardno,5,VI_ERROR_PARAMETER3);
    errStatus = viPrintf(vi,":ROUT:AGND:CHAN:%s:CARD %s\n",agb220xa_groundChanCard_disable_enable_a[disable_enable],agb220xa_groundChanCard_ground_cardno_a[ground_cardno]);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
        }
    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_groundChanList
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function will  enable or disable ground on all the output
 *            ports specified by the groundchan_list.
 *           
 *            The parameter 'groundchan_disen' will select disable or enable.
 *           
 *            The parameter 'groundchan_list' is an array of integers  with
 *           each integer   representing  one  channel. The  last  number  of
 *            the 'groundchan_list'  should be "0" (Numeric zero)  to  identify
 *            the end of the list. The maximum number of channels that can be
 *            specified by the list is 100.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:AGND:CHAN:ENAB|DIS (@groundchan_list)
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 groundchan_disen
 * IN        
 *            Select 0 for enable,1 for disable
 * 
 * PARAM 3 : ViInt32 _VI_FAR groundchan_list[]
 * IN        
 *           
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_groundChanList_groundchan_disen_a[] = {
    "ENAB","DIS",0};
ViStatus _VI_FUNC agb220xa_groundChanList(ViSession vi,
                                          ViInt16 groundchan_disen,
                                          ViInt32 _VI_FAR groundchan_list[])
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_groundChanList" );

    agb220xa_CHK_ENUM(groundchan_disen,1,VI_ERROR_PARAMETER2);
    
    {
        ViChar string1[MAX_CHANNEL_LIST_ELEM * 6 + 20 ], tmp[10] ;
        ViInt16 elm;
    
        for (elm = 0 ; groundchan_list[elm] != 0 ; elm++ );

        agb220xa_CHK_INT_RANGE(elm, 1, MAX_CHANNEL_LIST_ELEM, VI_ERROR_PARAMETER3);
     
        if( groundchan_disen == 0 )
            strcpy(string1,":ROUT:AGND:CHAN:ENAB (@\0");
        if( groundchan_disen == 1 )
            strcpy(string1,":ROUT:AGND:CHAN:DIS (@\0");
    
        
        while( *groundchan_list != 0 )
        							  
            {
                sprintf(tmp ,"%ld",*groundchan_list);
                strcat(string1,tmp );
    
                groundchan_list++;
    			   
                if  (*groundchan_list == 0)
                    strcat(string1,")");
                else
                    {
                        strcat(string1,",");
                    }  	
            }
    
        errStatus = viPrintf(vi," %s\n",string1);
        if (errStatus < VI_SUCCESS)
    
            {
                agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
            }
    	  
    }
    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_groundChanList_Q
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function  will  query the instrument for the ground status
 *            for the channels given in the list.
 *           
 *            The  parameter  'gndchan_disen'  specifies  the  disable or enable
 *            status of the query.
 *           
 *            The parameter 'gndchan_list' is an array of integers with each
 *            integer  representing  one  channel. The  last  number  of  the
 *            'gndchan_list' should be "0"  ( Numeric Zero ) to identify the
 *            end of the list
 *           
 *            The  'gndchan_status' parameter is an array of integers containing
 *            the return values of the query.The 'gndchan_status' array returned
 *            will correspond one to one with  'gndchan_list parameter'. The
 *            maximum  number  of channels that can be  specified by the list
 *            is 100.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:AGND:CHAN:ENAB|DIS (@gndchan_list)
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 gndchan_disen
 * IN        
 *            Select 0 for enable,1 for disable
 * 
 * PARAM 3 : ViInt32 _VI_FAR gndchan_list[]
 * IN        
 *           
 * 
 * PARAM 4 : ViInt32 _VI_FAR gndchan_status[]
 * OUT       
 *           
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_groundChanList_Q_ground_disen_a[] = {
    "ENAB","DIS",0};
ViStatus _VI_FUNC agb220xa_groundChanList_Q(ViSession vi,
                                            ViInt16 gndchan_disen,
                                            ViInt32 _VI_FAR gndchan_list[],
                                            ViInt32 _VI_FAR gndchan_status[])
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS)
        {
            agb220xa_LOG_STATUS( vi, 0, errStatus );
        }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_groundChanList_Q" );

    agb220xa_CHK_ENUM(gndchan_disen,1,VI_ERROR_PARAMETER2);
    
    {
        ViChar string1[MAX_CHANNEL_LIST_ELEM * 6 + 20 ], tmp[10] ;
        ViChar string2[MAX_CHANNEL_LIST_ELEM * 2 + 20 ] ;
        ViInt16 elm,i;
    
        for (elm = 0 ; gndchan_list[elm] != 0 ; elm++ );
        agb220xa_CHK_INT_RANGE(elm, 1, MAX_CHANNEL_LIST_ELEM, VI_ERROR_PARAMETER3);
    
        if( gndchan_disen == 0 )
            strcpy(string1,":ROUT:AGND:CHAN:ENAB? (@\0");
        if( gndchan_disen == 1 )
            strcpy(string1,":ROUT:AGND:CHAN:DIS? (@\0");
    
        
        while( *gndchan_list != 0 )
        
            {
                sprintf(tmp ,"%ld",*gndchan_list);
                strcat(string1,tmp );
    
                gndchan_list++;
    			   
                if  (*gndchan_list == 0)
                    strcat(string1,")");
                else
                    {
                        strcat(string1,",");
                    }  	
            }
    
        errStatus = viPrintf(vi," %s\n",string1);
        if (errStatus < VI_SUCCESS)
    
            {
                agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
            }
    		
        errStatus = viScanf(vi,"%s",string2);
    
        if (errStatus < VI_SUCCESS)
     
            {
                agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
            }
    
    		 
        for(i=0;string2[i] != '\0' && i < 100;i++)
    		   
            {
                if (string2 [i] == '1' )
                    *gndchan_status++ = 1;
                else if (string2 [i] == '0' ) 
                    *gndchan_status++ = 0;
                else if( string2[i] == ',')
                    ;
                else
                    *gndchan_status++ = -1;
            }
    		
    }
    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}


/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_groundState
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function controls the ground mode for the specified card.
 *            When Ground Mode is ON, the  input Ground Port is connected to
 *            all  ground enabled  input/output ports that are not connected 
 *            to any other ports. 
 *           
 *            The  parameter   'groundstate_cardno'  specifies   one  or  all
 *            possible cards.
 *           
 *            The parameter 'state' specifies ground mode is ON/OFF.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:AGND:STAT 0|1|2|3|4|ALL, ON|OFF
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 groundstate_cardno
 * IN        
 *            Select 0 for auto,1 for card1,2 for card2,3 for card3,
 *            4 for card4,5 for card all  - card number
 * 
 * PARAM 3 : ViInt16 state
 * IN        
 *            Select 0 turn OFF bias state
 *            Select 1 turn ON bias state
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_groundState_groundstate_cardno_a[] = {"0",
                                                                         "1","2","3","4","ALL",0};
static const char * const agb220xa_groundState_state_a[] = {"OFF","ON",0};
ViStatus _VI_FUNC agb220xa_groundState(ViSession vi,
                                       ViInt16 groundstate_cardno,
                                       ViInt16 state)
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS) {
        agb220xa_LOG_STATUS( vi, 0, errStatus );
    }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_groundState" );

    agb220xa_CHK_ENUM(groundstate_cardno,5,VI_ERROR_PARAMETER2);
    agb220xa_CHK_ENUM(state,1,VI_ERROR_PARAMETER3);
    errStatus = viPrintf(vi,":ROUT:AGND:STAT %s,%s\n",agb220xa_groundState_groundstate_cardno_a[groundstate_cardno],agb220xa_groundState_state_a[state]);
    if (errStatus < VI_SUCCESS) {
        agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
    }
    
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_detectCouplePort
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function detects the input ports connected to the Kelvin cable,
 *            and assigns them as the input couple ports that will be used for the
 *            Kelvin connection.  The input couple port setting is effective for
 *            all the cards.  This command overwrites the previous couple port setting.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:COUP:PORT:DET
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
  * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */

ViStatus _VI_FUNC agb220xa_detectCouplePort(ViSession vi)
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS) {
        agb220xa_LOG_STATUS( vi, 0, errStatus );
    }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_detectCouplePort" );
	
    errStatus = viPrintf(vi,":ROUT:COUP:PORT:DET\n");
    if (errStatus < VI_SUCCESS) {
        agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
    }
      
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC agb220xa_unusedPort
 *-----------------------------------------------------------------------------
 * 
 * PURPOSE :  This  function will  specify the ground-enabled input ports for the 
 *            specified card.  When the Ground Mode is ON, the ground-enabled
 *            input ports that have not been connected to any other port are 
 *            connected to the input ground port.
 *           
 *            The parameter 'unusedport_list' is an array of integers  with
 *            each integer representing one port. The  last  number  of
 *            the 'unusedport_list'  should be "0" (Numeric zero)  to  identify
 *            the end of the list. The maximum number of channels that can be
 *            specified by the list is agb220xa_UNUSEDPORT_MAX.
 *           
 *            This function implements the following SCPI commands:
 *            :ROUT:AGND:UNUSED card_number, 'port_number'
 * 
 * PARAM 1 : ViSession vi
 * IN        
 *            Instrument Handle returned from agb220xa_init().
 * 
 * PARAM 2 : ViInt16 ununsedport_cardno
 * IN        
 *            Select 0 for auto,1 for card1,2 for card2,3 for card3,
 *            4 for card4,5 for card all  - card number
 * 
 * PARAM 3 : ViInt32 _VI_FAR unusedport_list[]
 * IN        
 * 
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To  determine  error message, pass the  return value
 *           to routine "agb220xa_error_message".
 * 
 *-----------------------------------------------------------------------------
 */
static const char * const agb220xa_unusedport_cardno_a[] = {"0","1","2","3","4","ALL",0};
ViStatus _VI_FUNC agb220xa_unusedPort(ViSession vi,  ViInt16 unusedport_cardno,
                                      ViInt32 _VI_FAR unusedport_list[])
{
    ViStatus errStatus = 0;
    struct agb220xa_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (errStatus < VI_SUCCESS) {
        agb220xa_LOG_STATUS( vi, 0, errStatus );
    }

    agb220xa_DEBUG_CHK_THIS( vi, thisPtr );
    agb220xa_CDE_INIT( "agb220xa_unusedPort" );   
    agb220xa_CHK_ENUM(unusedport_cardno,5,VI_ERROR_PARAMETER2);
    
    {
        ViChar string1[agb220xa_UNUSEDPORT_MAX * 6 + 20 ], tmp[10] ;
        ViInt16 elm;
    
        for (elm = 0 ; unusedport_list[elm] != 0 ; elm++ );

        agb220xa_CHK_INT_RANGE(elm, 1, agb220xa_UNUSEDPORT_MAX, VI_ERROR_PARAMETER3);    

        sprintf(string1 ,":ROUT:AGND:UNUSED %s, '\0",agb220xa_unusedport_cardno_a[unusedport_cardno]);
        
        while( *unusedport_list != 0 ) {
            sprintf(tmp ,"%ld",*unusedport_list);
            strcat(string1,tmp );
    
            unusedport_list++;
    			   
            if  (*unusedport_list == 0){
                strcat(string1,"'");
            } else {
                strcat(string1,",");
            }  	
        }
    
        errStatus = viPrintf(vi," %s\n",string1);
        if (errStatus < VI_SUCCESS) {
            agb220xa_LOG_STATUS( vi, thisPtr, errStatus );
        }
    }
    agb220xa_LOG_STATUS( vi, thisPtr, VI_SUCCESS );
}
