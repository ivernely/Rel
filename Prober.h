#ifndef PROBER_H
#define PROBER_H

#include <QObject>
#include <QVector>
#include "vpptype.h"

class Prober : public QObject
{
    Q_OBJECT
public:
    Prober(QWidget *parent = 0);
    void ClearProb();

    ViSession vi;

    ushort  NumDie,
            NumSubdie,
            NumTestDie,
            NumTestSubdie;

    QVector<ushort> TestSubdie;

    QString SubsiteLabel(ushort n);

signals:
    void cmd_finished();    //прошла успешно
    void cmd_interrupt();   //ошибка

public slots:
    void Contact();
    void UnContact();
    void Light();
    void LightOff();
    void NextDie();
    void NextSubDie();
    void FirstDie();

    void Probeplan(QString PlanName);//необходимо вводить название тестплана
    void TestSequence();
};

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

#define csc12000k_TIMEOUT_MIN     1
#define csc12000k_TIMEOUT_MAX     2147483647

#define csc12000k_ERR_MSG_LENGTH  256       /* size of error message buffer */

#define csc12000k_INSTR_ERROR_NULL_PTR      (_VI_ERROR+0x3FFC0D02L) /* 0xBFFC0D02 */
#define csc12000k_INSTR_ERROR_RESET_FAILED  (_VI_ERROR+0x3FFC0D03L) /* 0xBFFC0D03 */
#define csc12000k_INSTR_ERROR_UNEXPECTED    (_VI_ERROR+0x3FFC0D04L) /* 0xBFFC0D04 */
#define csc12000k_INSTR_ERROR_INV_SESSION   (_VI_ERROR+0x3FFC0D05L) /* 0xBFFC0D05 */
#define csc12000k_INSTR_ERROR_LOOKUP        (_VI_ERROR+0x3FFC0D06L) /* 0xBFFC0D06 */
#define csc12000k_INSTR_ERROR_DETECTED      (_VI_ERROR+0x3FFC0D07L) /* 0xBFFC0D07 */

#define csc12000k_INSTR_ERROR_PARAMETER9    (_VI_ERROR+0x3FFC0D20L) /* 0xBFFC0D20 */
#define csc12000k_INSTR_ERROR_PARAMETER10   (_VI_ERROR+0x3FFC0D21L) /* 0xBFFC0D21 */
#define csc12000k_INSTR_ERROR_PARAMETER11   (_VI_ERROR+0x3FFC0D22L) /* 0xBFFC0D22 */
#define csc12000k_INSTR_ERROR_PARAMETER12   (_VI_ERROR+0x3FFC0D23L) /* 0xBFFC0D23 */
#define csc12000k_INSTR_ERROR_PARAMETER13   (_VI_ERROR+0x3FFC0D24L) /* 0xBFFC0D24 */
#define csc12000k_INSTR_ERROR_PARAMETER14   (_VI_ERROR+0x3FFC0D25L) /* 0xBFFC0D25 */
#define csc12000k_INSTR_ERROR_PARAMETER15   (_VI_ERROR+0x3FFC0D26L) /* 0xBFFC0D26 */
#define csc12000k_INSTR_ERROR_PARAMETER16   (_VI_ERROR+0x3FFC0D27L) /* 0xBFFC0D27 */
#define csc12000k_INSTR_ERROR_PARAMETER17   (_VI_ERROR+0x3FFC0D28L) /* 0xBFFC0D28 */
#define csc12000k_INSTR_ERROR_PARAMETER18   (_VI_ERROR+0x3FFC0D29L) /* 0xBFFC0D29 */

#define csc12000k_MSG_NO_ERRORS              "No Errors"
#define csc12000k_MSG_IN_FUNCTION            "in function"
#define csc12000k_MSG_INVALID_STATUS         "Parameter 2 is invalid in function csc12000k_error_message()."
#define csc12000k_MSG_INVALID_STATUS_VALUE   "is not a valid viStatus value."
#define csc12000k_MSG_VI_OPEN_ERR            "vi was zero.  Was the csc12000k_init() successful?"
#define csc12000k_MSG_LONG                   "Expected %ld to %ld; Got %ld"
#define csc12000k_MSG_BOOLEAN                "Expected 0 or 1; Got %d"

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


ViStatus _VI_FUNC initError_csc12000k(
                            ViPSession vi,
                            ViStatus   errStatus);

ViStatus _VI_FUNC csc12000k_init(
                            ViRsrc 		InstrDesc,
                            ViBoolean 	id_query,
                            ViBoolean 	do_reset,
                            ViPSession 	vi);

ViStatus _VI_FUNC csc12000k_reset(
                                 ViSession vi);

ViStatus _VI_FUNC csc12000k_error_message(
                                         ViSession  		vi,
                                         ViStatus   		driver_error_number,
                                         ViChar _VI_FAR 	driver_error_message[]);

ViStatus _VI_FUNC csc12000k_error_query(
                                       ViSession  		vi,
                                       ViPInt32   		inst_error_number,
                                       ViChar _VI_FAR 	inst_error_message[]);

ViStatus _VI_FUNC csc12000k_close(ViSession vi);

ViStatus _VI_FUNC csc12000k_cmd(
                                ViSession vi,
                                ViString  cmd_string);

ViStatus _VI_FUNC csc12000k_cmdString_Q(
                                        ViSession       vi,
                                        ViString        sq_cmd_string,
                                        ViInt32         sq_size,
                                        ViChar _VI_FAR  sq_result[] );

ViStatus _VI_FUNC csc12000k_timeOut(
                                    ViSession  vi,
                                    ViInt32    time_out);

ViStatus _VI_FUNC csc12000k_errorQueryDetect(
                                            ViSession  vi,
                                            ViBoolean  eq_detect);

ViStatus _VI_FUNC csc12000k_dcl( ViSession  vi);

#if defined(__cplusplus) || defined(__cplusplus__)
}    /* end of "C" externs for C++ */
#endif

#endif // PROBER_H
