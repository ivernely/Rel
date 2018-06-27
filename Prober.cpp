#ifdef __hpux
#define _fcvt    fcvt
#endif

#ifdef WIN16
#else
#define _fmemcpy memcpy
#define _huge
#endif

#ifdef __hpux
#else
#include <windows.h>
#endif

#ifdef WIN32
#include <winbase.h>
#else
#endif

#include "visa.h"
#include "Prober.h"

#define csc12000k_IDN_STRING  "Cascade Microtech, Summit 12000 Theta"

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
    { csc12000k_INSTR_ERROR_PARAMETER9,    VI_ERROR_PARAMETER9_MSG 		},
    { csc12000k_INSTR_ERROR_PARAMETER10,     VI_ERROR_PARAMETER10_MSG 		},
    { csc12000k_INSTR_ERROR_PARAMETER11,     VI_ERROR_PARAMETER11_MSG 		},
    { csc12000k_INSTR_ERROR_PARAMETER12,     VI_ERROR_PARAMETER12_MSG 		},
    { csc12000k_INSTR_ERROR_PARAMETER13,     VI_ERROR_PARAMETER13_MSG 		},
    { csc12000k_INSTR_ERROR_PARAMETER14,     VI_ERROR_PARAMETER14_MSG 		},
    { csc12000k_INSTR_ERROR_PARAMETER15,     VI_ERROR_PARAMETER15_MSG 		},
    { csc12000k_INSTR_ERROR_PARAMETER16,     VI_ERROR_PARAMETER16_MSG 		},
    { csc12000k_INSTR_ERROR_PARAMETER17,     VI_ERROR_PARAMETER17_MSG 		},
    { csc12000k_INSTR_ERROR_PARAMETER18,     VI_ERROR_PARAMETER18_MSG 		},
    { VI_ERROR_FAIL_ID_QUERY,                VI_ERROR_FAIL_ID_QUERY_MSG 	},
    { csc12000k_INSTR_ERROR_INV_SESSION,     INSTR_ERROR_INV_SESSION_MSG 	},
    { csc12000k_INSTR_ERROR_NULL_PTR,        INSTR_ERROR_NULL_PTR_MSG 		},
    { csc12000k_INSTR_ERROR_RESET_FAILED,    INSTR_ERROR_RESET_FAILED_MSG 	},
    { csc12000k_INSTR_ERROR_UNEXPECTED,      INSTR_ERROR_UNEXPECTED_MSG 	},
    { csc12000k_INSTR_ERROR_DETECTED,        INSTR_ERROR_DETECTED_MSG 		},
    { csc12000k_INSTR_ERROR_LOOKUP,          INSTR_ERROR_LOOKUP_MSG 		}
};

struct csc12000k_globals
{
    ViSession    defRMSession;

    ViStatus     errNumber;

    ViChar       errFuncName[40];
    ViChar       errMessage[160];

    ViBoolean    errQueryDetect;

    ViBoolean    blockSrqIO;
    ViInt32      countSrqIO;

    ViInt32      myDelay;

    ViUInt16     interfaceType;
};

#define GET_GLOBALS(vi,thisPtr)                                                \
{                                                                              \
    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);      \
    if (errStatus < VI_SUCCESS)                                                \
        return statusUpdate(vi, NULL, errStatus);                              \
}

#define csc12000k_CDE_INIT( funcname )                                          \
    strcpy(thisPtr->errFuncName, funcname);                                     \
    thisPtr->errNumber = VI_SUCCESS;                                            \
    thisPtr->errMessage[0] = 0;                                                 \

#define csc12000k_CHK_LONG_RANGE(my_val, min, max, err)          \
    if (csc12000k_chk_long_range(thisPtr, my_val, min, max))     \
        return statusUpdate(vi, thisPtr, err);

#define csc12000k_CHK_BOOLEAN(my_val, err)                 \
    if (csc12000k_chk_boolean(thisPtr, my_val))                \
        return statusUpdate(vi, thisPtr, err);

/*------------------------------------------------------------------------------------------------*/
Prober::Prober(QWidget *)
{
    ClearProb();
}
/*------------------------------------------------------------------------------------------------*/
void Prober::ClearProb()
{
    vi = VI_NULL;

    NumDie = 0;
    NumSubdie = 0;
    NumTestDie = 0;
    NumTestSubdie = 0;

    TestSubdie.clear();
}
/*------------------------------------------------------------------------------------------------*/
//законтактироваться
void Prober::Contact()
{
    ViStatus ret;
    ViChar str[256] = {0};

    ret = csc12000k_cmdString_Q(vi, ViString(":mov:cont 2"), 255, str);

    if(ret == VI_SUCCESS)
        emit cmd_finished();
    else
        emit cmd_interrupt();
}
/*------------------------------------------------------------------------------------------------*/
//расконтактироваться
void Prober::UnContact()
{
    ViStatus ret;
    ViChar str[256] = {0};

    ret = csc12000k_cmdString_Q(vi, ViString(":mov:sep 2"), 255, str);

    if(ret == VI_SUCCESS)
        emit cmd_finished();
    else
        emit cmd_interrupt();
}
/*------------------------------------------------------------------------------------------------*/
//вкл/выкл свет
void Prober::Light()
{
    ViStatus ret;
    ViChar str[256] = {0};

    ret = csc12000k_cmdString_Q(vi, ViString(":set:ligh?"), 255, str);
    if(ret==VI_SUCCESS){
        if(strstr(str, "ON"))
            ret = csc12000k_cmdString_Q(vi, ViString(":set:ligh OFF"), 255, str);
        else
            ret = csc12000k_cmdString_Q(vi, ViString(":set:ligh ON"), 255, str);
    }

    if(ret == VI_SUCCESS)
        emit cmd_finished();
    else
        emit cmd_interrupt();
}
/*------------------------------------------------------------------------------------------------*/
//выкл свет
void Prober::LightOff()
{
    ViStatus ret;
    ViChar str[256] = {0};

    ret = csc12000k_cmdString_Q(vi, ViString(":set:ligh OFF"), 255, str);

    if(ret == VI_SUCCESS)
        emit cmd_finished();
    else
        emit cmd_interrupt();
}
/*------------------------------------------------------------------------------------------------*/
//перемещение на следующий чип
void Prober::NextDie()
{
    ViStatus ret;
    ViChar str[256] = {0};

    ret = csc12000k_cmdString_Q(vi, ViString(":mov:prob:next:die"), 255, str);

    if(ret == VI_SUCCESS)
        emit cmd_finished();
    else
        emit cmd_interrupt();
}
/*------------------------------------------------------------------------------------------------*/
//перемещение на следующую структуру
void Prober::NextSubDie()
{
    ViStatus ret;
    ViChar str[256] = {0};

    ret = csc12000k_cmdString_Q(vi, ViString(":mov:prob:next:subs"), 255, str);

    if(ret == VI_SUCCESS)
        emit cmd_finished();
    else
        emit cmd_interrupt();
}
/*------------------------------------------------------------------------------------------------*/
//перемещение на первый чип
void Prober::FirstDie()
{
    ViStatus ret;
    ViChar str[256] = {0};

    ret = csc12000k_cmdString_Q(vi, ViString(":mov:prob:firs:die"), 255, str);

    if(ret == VI_SUCCESS)
        emit cmd_finished();
    else
        emit cmd_interrupt();
}
/*------------------------------------------------------------------------------------------------*/
//загрузка и определение параметров тестплана с зонда
void Prober::Probeplan(QString PlanName)
{
    ViStatus ret;
    QString s;
    ViChar str[256] = {0};

    s = QString(":prob:load C:\Documents and Settings\All Users"
                "\Documents\Nucleus\!!!Wafer maps\%1.wfd").arg(PlanName);

    ret = csc12000k_cmdString_Q(vi, s.toLocal8Bit().data(), 255, str);

    ret = csc12000k_cmdString_Q(vi, ViString(":prob:alig OFF -11778 -80621"), 255, str);

    if(ret == VI_SUCCESS)
        emit cmd_finished();
    else
        emit cmd_interrupt();

    /*!!!для отметки чипов для тестирования использовать команду :prob:test:die*/
}
/*------------------------------------------------------------------------------------------------*/
//определение измеряемых параметров
void Prober::TestSequence()
{
    ViStatus ret;
    ViChar str[256] = {0};
    char cmd[64];
    uint i;
    QString res;

    TestSubdie.clear();

    //общее количество чипов в тестплане для тестирования
    ret = csc12000k_cmdString_Q(vi, ViString(":prob:ntes?"), 255, str);
    NumTestDie = atoi(str);
    if(NumTestDie>255)
        NumTestDie = 0;

    //количество макросов в тестплане для тестирования
    ret = csc12000k_cmdString_Q(vi, ViString(":prob:nsub?"), 255, str);
    NumTestSubdie = atoi(str);
    if(NumTestSubdie>255)
        NumTestSubdie = 0;

    //общее количество макросов в тестплане
    ret = csc12000k_cmdString_Q(vi, ViString(":prob:subs:tot?"), 255, str);
    NumSubdie = atoi(str);
    if(NumSubdie>255)
        NumSubdie = 0;

    for(i=0; i<NumSubdie; i++){
        sprintf(cmd, ":prob:set:subs:lab:act? %d", i);
        csc12000k_cmdString_Q(vi, ViString(cmd), 255, str);
        res = QString("%1").arg(str).simplified();
        if(res=="ON"){
            TestSubdie.push_back(i);
        }
    }

    if(ret == VI_SUCCESS)
        emit cmd_finished();
    else
        emit cmd_interrupt();
}
/*------------------------------------------------------------------------------------------------*/
//определение названия измеряемого макроса
QString Prober::SubsiteLabel(ushort n)
{
    ViStatus ret;
    ViChar str[256] = {0};
    ViChar cmd[256] = {0};
    QString result = QString("none %1").arg(n);

    sprintf(cmd, ":prob:subs:lab? %d", n);
    ret = csc12000k_cmdString_Q(vi, ViString(cmd), 255, str);

    if(ret == VI_SUCCESS){
        emit cmd_finished();
        result = QString("%1").arg(str);
        result = result.simplified();
    }
    else
        emit cmd_interrupt();

    return result;
}
/*------------------------------------------------------------------------------------------------*/
static ViBoolean csc12000k_chk_long_range(
                                        struct  csc12000k_globals *thisPtr,
                                        ViInt32 my_val,
                                        ViInt32 min,
                                        ViInt32 max)
{
    ViChar message[csc12000k_ERR_MSG_LENGTH];

    if ( (my_val < min) || (my_val > max) )
    {
        sprintf(message, csc12000k_MSG_LONG, min, max, my_val);
        strcpy(thisPtr->errMessage, message);
        return VI_TRUE;
    }

    return VI_FALSE;
}
/*------------------------------------------------------------------------------------------------*/
static ViBoolean csc12000k_chk_boolean(
                                    struct        csc12000k_globals *thisPtr,
                                    ViBoolean      my_val)
{
    ViChar message[csc12000k_ERR_MSG_LENGTH];

    if ( (my_val != VI_TRUE) && (my_val != VI_FALSE) )
    {
        sprintf(message, csc12000k_MSG_BOOLEAN, my_val);
        strcpy(thisPtr->errMessage, message);
        return VI_TRUE;
    }

    return VI_FALSE;
}
/*------------------------------------------------------------------------------------------------*/
void doDelay_12k(ViInt32 delayTime)
{
    long   ticksToCount;

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

#ifdef WIN16

    DWORD    msTicks;

    if (delayTime == 0)
        return;

    ticksToCount = (long)((1E3/1E6)*delayTime) + 1;
    msTicks = GetTickCount();
    while (GetTickCount() - msTicks < (DWORD)ticksToCount);

#endif

#ifdef __hpux

    struct timeval t0, t1;

    if (delayTime == 0)
        return;

    gettimeofday(&t0, NULL);
    t0.tv_usec += delayTime;
    t0.tv_sec += t0.tv_usec / 1000000;
    t0.tv_usec = t0.tv_usec % 1000000;

    gettimeofday(&t1, NULL);
    while (t1.tv_sec < t0.tv_sec)
        gettimeofday(&t1, NULL);

    if (t1.tv_sec > t0.tv_sec) return;

    while (t1.tv_usec < t0.tv_usec)
        gettimeofday(&t1, NULL);

#endif
    return;
}
/*------------------------------------------------------------------------------------------------*/
ViStatus delayScanf_12k(
                    ViSession  vi,
                    ViInt32    delayTime,
                    ViString   readFmt,
                    ... )
{
    ViStatus  errStatus;
    va_list   va;

    va_start(va, readFmt);
    errStatus = viVScanf(vi, readFmt, va);
    va_end(va);

    doDelay_12k(delayTime);

    return errStatus;
}
/*------------------------------------------------------------------------------------------------*/
ViStatus delayRead_12k(
                   ViSession  vi,
                   ViInt32    delayTime,
                   ViPBuf     buf,
                   ViUInt32   cnt,
                   ViPUInt32  retCnt)
{
    ViStatus  errStatus;

    errStatus = viRead(vi, buf, cnt, retCnt);

    doDelay_12k(delayTime);

    return errStatus;
}
/*------------------------------------------------------------------------------------------------*/
ViStatus statusUpdate(
                      ViSession  vi,
                      struct     csc12000k_globals *thisPtr,
                      ViStatus   s )
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

        if ((errStatus = delayRead_12k(vi, thisPtr->myDelay,
                                   (ViPBuf)lc, 20, &rc)) < VI_SUCCESS)
            return VI_ERROR_SYSTEM_ERROR;

        lc[rc]='\0';
        eventQ=atoi(lc);

        if( (0x04  | 0x08  | 0x10  | 0x20) & eventQ )
            return csc12000k_INSTR_ERROR_DETECTED;
    }

    return s;
}
/*------------------------------------------------------------------------------------------------*/
ViStatus _VI_FUNC initError_csc12000k(
                            ViPSession vi,
                            ViStatus   errStatus)
{
    viClose(*vi);
    *vi=VI_NULL;
    return errStatus;
}
/*------------------------------------------------------------------------------------------------*/
//ret = agb220xa_init(ViRsrc(address), VI_TRUE, VI_TRUE, &vi);
ViStatus _VI_FUNC csc12000k_init(
                            ViRsrc 		InstrDesc,
                            ViBoolean 	id_query,
                            ViBoolean 	do_reset,
                            ViPSession 	vi)

{
    struct     csc12000k_globals *thisPtr;
    ViStatus   errStatus;
    ViSession  defRM;
    ViChar     idn_buf[256];

    *vi = VI_NULL;

    errStatus = viOpenDefaultRM(&defRM);
    if (VI_SUCCESS > errStatus)
    return errStatus;

    errStatus = viOpen(defRM, InstrDesc, VI_NULL, VI_NULL, vi);
    if (VI_SUCCESS > errStatus)
        {
            viClose(defRM);
            *vi=VI_NULL;
            return errStatus;
        }

    thisPtr = (struct csc12000k_globals *)malloc(sizeof( struct csc12000k_globals) );
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

    thisPtr->defRMSession = defRM;
    thisPtr->errNumber = VI_SUCCESS;
    thisPtr->errFuncName[0] = 0;
    thisPtr->errMessage[0] = 0;
    thisPtr->errQueryDetect = VI_FALSE;
    thisPtr->blockSrqIO = VI_FALSE;
    thisPtr->countSrqIO = 0;
    thisPtr->myDelay = 0;

    errStatus = viGetAttribute(*vi, VI_ATTR_INTF_TYPE, &thisPtr->interfaceType);
    if( VI_SUCCESS > errStatus)
        return initError_csc12000k(vi, errStatus);


    if( VI_TRUE == do_reset )
        {
            if (thisPtr->interfaceType == VI_INTF_GPIB)
                if (viClear(*vi) <  VI_SUCCESS)
                    return initError_csc12000k(vi, csc12000k_INSTR_ERROR_RESET_FAILED);

            if (csc12000k_reset(*vi) <  VI_SUCCESS)
                return initError_csc12000k(vi, csc12000k_INSTR_ERROR_RESET_FAILED);
        }

    if( VI_TRUE == id_query )
        {
            if (thisPtr->interfaceType == VI_INTF_GPIB)
                if (viClear(*vi) <  VI_SUCCESS)
                    return initError_csc12000k(vi, csc12000k_INSTR_ERROR_RESET_FAILED);

            if (   (viPrintf(*vi, "*IDN?\n") < VI_SUCCESS )
                   || (delayScanf_12k(*vi, 100, "%t", idn_buf) < VI_SUCCESS )
                   || (strncmp(idn_buf, csc12000k_IDN_STRING, strlen(csc12000k_IDN_STRING))) )
                return initError_csc12000k(vi, VI_ERROR_FAIL_ID_QUERY);
        }

    return statusUpdate(*vi, thisPtr, errStatus);
}
/*------------------------------------------------------------------------------------------------*/
ViStatus _VI_FUNC csc12000k_reset(ViSession vi)
{
    struct csc12000k_globals *thisPtr;
    ViStatus errStatus;

    GET_GLOBALS(vi,thisPtr);

    csc12000k_CDE_INIT( "csc12000k_reset" );

    if ((errStatus = viPrintf(vi, "*RST\n")) < VI_SUCCESS)
        return statusUpdate(vi, thisPtr, errStatus);

    return statusUpdate(vi, thisPtr, errStatus);
}
/*------------------------------------------------------------------------------------------------*/
ViStatus _VI_FUNC csc12000k_error_message(
                                         ViSession  		vi,
                                         ViStatus   		driver_error_number,
                                         ViChar _VI_FAR 	driver_error_message[])
{
    struct    csc12000k_globals *thisPtr;
    ViStatus  errStatus;
    ViUInt32   idx;

    driver_error_message[0] = 0;

    thisPtr = NULL;
    if (vi != VI_NULL)
        GET_GLOBALS(vi,thisPtr);

    if (driver_error_number == VI_SUCCESS)
        {
            sprintf(driver_error_message, csc12000k_MSG_NO_ERRORS);
            return statusUpdate(vi, thisPtr, VI_SUCCESS);
        }

    for (idx=0; idx < (sizeof instrErrMsgTable /
                       sizeof(struct instrErrStruct)); idx++) {
        if (instrErrMsgTable[idx].errStatus == driver_error_number)
            {
                if ((thisPtr) && (thisPtr->errNumber == driver_error_number))
                    {
                        sprintf(driver_error_message, "%s " csc12000k_MSG_IN_FUNCTION " %s() %s",
                                instrErrMsgTable[idx].errMessage,
                                thisPtr->errFuncName,
                                thisPtr->errMessage);
                    }
                else
                    strcpy(driver_error_message, instrErrMsgTable[idx].errMessage);

                return statusUpdate(vi, thisPtr, VI_SUCCESS);
            }
    }

    errStatus = viStatusDesc(vi, driver_error_number, driver_error_message);
    if (errStatus == VI_SUCCESS)
        {
            if( (thisPtr) && (thisPtr->errNumber == driver_error_number))
                {
                    strcat(driver_error_message, " ");
                    strcat(driver_error_message, csc12000k_MSG_IN_FUNCTION);
                    strcat(driver_error_message, " ");
                    strcat(driver_error_message, thisPtr->errFuncName);
                    strcat(driver_error_message, "() ");
                    strcat(driver_error_message, thisPtr->errMessage);
                }
            return statusUpdate(vi, thisPtr, VI_SUCCESS);
        }

    if( VI_NULL == vi )
        {
            strcpy(driver_error_message, csc12000k_MSG_VI_OPEN_ERR);
            return statusUpdate(vi, thisPtr, VI_SUCCESS);
        }

    sprintf(driver_error_message, csc12000k_MSG_INVALID_STATUS "  %ld"
            csc12000k_MSG_INVALID_STATUS_VALUE, (long)driver_error_number);

    return statusUpdate(vi, thisPtr, VI_ERROR_PARAMETER2);
}
/*------------------------------------------------------------------------------------------------*/
ViStatus _VI_FUNC csc12000k_error_query(
                                       ViSession  		vi,
                                       ViPInt32   		inst_error_number,
                                       ViChar _VI_FAR 	inst_error_message[])
{
    struct csc12000k_globals *thisPtr;
    ViStatus errStatus;

    *inst_error_number = -1;
    inst_error_message[0] = 0;

    GET_GLOBALS(vi,thisPtr);
    csc12000k_CDE_INIT( "csc12000k_error_query" );

    thisPtr->blockSrqIO = VI_TRUE;

    if ((errStatus = viPrintf( vi, "SYST:ERR?\n")) < VI_SUCCESS)
        return statusUpdate(vi, thisPtr, errStatus);

    if ((errStatus = delayScanf_12k(vi, thisPtr->myDelay, "%ld,%t",
                                inst_error_number, inst_error_message)) < VI_SUCCESS)
        {
            *inst_error_number = -1;
            inst_error_message[0] = 0;
            return statusUpdate(vi, thisPtr, errStatus);
        }

    /* get rid of extra LF at the end of the error_message */
    inst_error_message[strlen(inst_error_message)-1] = 0;

    return statusUpdate(vi, thisPtr, errStatus);
}
/*------------------------------------------------------------------------------------------------*/
ViStatus _VI_FUNC csc12000k_close(ViSession vi)
{
    struct csc12000k_globals *thisPtr;
    ViStatus errStatus;
    ViSession defRM;

    GET_GLOBALS(vi,thisPtr);

    defRM = thisPtr->defRMSession;

    if( thisPtr )
        free(thisPtr);

    return viClose(defRM);
}
/*------------------------------------------------------------------------------------------------*/
ViStatus _VI_FUNC csc12000k_cmd(
                                ViSession vi,
                                ViString  cmd_string)
{
    ViStatus errStatus;
    struct   csc12000k_globals *thisPtr;


    GET_GLOBALS(vi,thisPtr)
    csc12000k_CDE_INIT( "csc12000k_cmd" );

#ifdef NO_FORMAT_IO
    {
    ViUInt32 rtn_size;
    errStatus = viWrite(vi, (ViBuf)cmd_string, strlen(cmd_string), &rtn_size );
    }
#else
    errStatus = viPrintf(vi,"%s\n", cmd_string);
#endif

    return statusUpdate( vi, thisPtr, errStatus );
}
/*------------------------------------------------------------------------------------------------*/
ViStatus _VI_FUNC csc12000k_cmdString_Q(
                                        ViSession       vi,
                                        ViString        sq_cmd_string,
                                        ViInt32         sq_size,
                                        ViChar _VI_FAR  sq_result[] )
{
    ViStatus errStatus;
    struct   csc12000k_globals *thisPtr;


        /* Command strings must have at least one non-null character */
    if (sq_size < 2)     return statusUpdate(vi,VI_NULL,VI_ERROR_PARAMETER3);

#ifdef WIN16
        /* Strings in 16-bit windows are limited to 32K characters. */
    if (sq_size > 32767) return statusUpdate(vi,VI_NULL,VI_ERROR_PARAMETER3);
#endif

    GET_GLOBALS(vi,thisPtr);
    csc12000k_CDE_INIT( "csc12000k_cmdString_Q" );

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

    if ((errStatus = delayScanf_12k(vi, thisPtr->myDelay, "%#c", &sz,sq_result)) < VI_SUCCESS)
        return statusUpdate(vi,thisPtr,errStatus);
    sq_result[sz]='\0';
    }
#endif

    return statusUpdate( vi, thisPtr, errStatus );
}
/*------------------------------------------------------------------------------------------------*/
ViStatus _VI_FUNC csc12000k_timeOut (
                                    ViSession  vi,
                                    ViInt32    time_out)
{
    ViStatus errStatus = 0;
    struct csc12000k_globals *thisPtr;

    GET_GLOBALS(vi,thisPtr)
        csc12000k_CDE_INIT( "csc12000k_timeOut" );

    csc12000k_CHK_LONG_RANGE(time_out, csc12000k_TIMEOUT_MIN,
                            csc12000k_TIMEOUT_MAX, VI_ERROR_PARAMETER2);

    if ((errStatus = viSetAttribute(vi, VI_ATTR_TMO_VALUE, time_out)) < VI_SUCCESS)
        return statusUpdate(vi, thisPtr, errStatus);

    return statusUpdate(vi, thisPtr, errStatus);
}        /* ----- end of function ----- */
/*------------------------------------------------------------------------------------------------*/
ViStatus _VI_FUNC csc12000k_errorQueryDetect(
                                            ViSession  vi,
                                            ViBoolean  eq_detect)
{
    ViStatus errStatus = 0;

    struct csc12000k_globals *thisPtr;


    GET_GLOBALS(vi,thisPtr)
    csc12000k_CDE_INIT( "csc12000k_errorQueryDetect" );

    csc12000k_CHK_BOOLEAN(eq_detect, VI_ERROR_PARAMETER2);

    thisPtr->errQueryDetect = eq_detect;

    return statusUpdate( vi, thisPtr, VI_SUCCESS);
}
/*------------------------------------------------------------------------------------------------*/
ViStatus _VI_FUNC csc12000k_dcl( ViSession  vi)
{
    ViStatus errStatus = 0;
    struct csc12000k_globals *thisPtr;


    GET_GLOBALS(vi,thisPtr)
    csc12000k_CDE_INIT( "csc12000k_dcl" );

    if (thisPtr->interfaceType == VI_INTF_GPIB) {
        if ((errStatus = viClear(vi)) < VI_SUCCESS)
            return statusUpdate( vi, thisPtr, errStatus);
    }

    return statusUpdate( vi, thisPtr, errStatus);
}
/*------------------------------------------------------------------------------------------------*/

