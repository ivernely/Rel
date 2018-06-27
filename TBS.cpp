#include "measclasses.h"
#include "eq_control.h"

/*-----------------------------------------------------------------------------------------------*/
TBS::TBS()
{
   Clear();
}
/*-----------------------------------------------------------------------------------------------*/
void TBS::Clear()
{
    HighPin = 0;
    LowPin.clear();
    NumSMUHigh = 0;
    NumSlotHigh = 0;
    NumSMULow = 0;
    NumSlotLow = 0;

    TypeCh = 1;

    W = 0.0;
    L = 0.0;
    Tstart = 0.0;
    Tend = 0.0;
    tox = 0.0;
    Eox = 0.0;
    tstress = 0.0;
    Vuse = 0.0;

    Vg = 0.0;
    Cox = 0.0;

    Vt_pos = 0.0;
    Vt_neg = 0.0;
}
/*-----------------------------------------------------------------------------------------------*/
int TBS_CapacitanceMeas(ViSession &vi_B1500, ViSession &vi_2200A, TBS *t)
{
    int Result = 1;

    /*ViStatus ret;
    ViReal64    values[2002],
                mon[2002];

    agb1500_dcl(vi_B1500);

    //коммутация матрицы, если матрица подключена
    if(vi_2200A > VI_NULL){
        OpenCommMatrix(vi_2200A);
        if(!ConnectPorts2200AVect(vi_2200A, CMUL, t->LowPin) ||
            !ConnectPort2200A(vi_2200A, CMUH, t->HighPin)){
            QMessageBox::critical(0, "Error", QString("Failure during TBS matrix commutation!"),
                                   QMessageBox::Ok);
            return 2;
        }
    }//end if(vi_2200A > VI_NULL)

    ret = agb1500_setSwitch(vi_B1500, agb1500_CH_CMU, ON);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        return ERR_SWITCH_ON;
    }

    ret = agb1500_setCmuInteg(vi_B1500, agb1500_INTEG_PLC, 3);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        return ERR_ADC;
    }

    ret = agb1500_setCmuFreq(vi_B1500, agb1500_CH_CMU, 100000); //100kHz
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        return ERR_CMUFREQ;
    }

    ret = agb1500_forceCmuAcLevel(vi_B1500, agb1500_CH_CMU, 0.03);  //30mV
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        return ERR_CMUACLEVEL;
    }

    ret = agb1500_forceCmuDcBias(vi_B1500, agb1500_CH_CMU, t->Vuse);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        return ERR_CMUDCBIAS;
    }

    //измерение емкости
    ret = agb1500_spotCmuMeas(vi_B1500, agb1500_CH_CMU, agb1500_CMUM_CS_RS, agb1500_AUTO_RANGE, &values[0],
                                NULL, &mon[0], NULL, NULL);

    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        return ERR_SPOTCMU;
    }

    //!!!
    //!!!
    //!!!обработать данные, если Rs > 100 Ом -> перемеряем в режиме agb1500_CMUM_CP_RP
    //!!!
    //!!!

    ret = agb1500_zeroOutput(vi_B1500, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        agb1500_setSwitch(vi_B1500, agb1500_CH_ALL, 0);
        return ERR_ZERO;
    }

    ret = agb1500_setSwitch(vi_B1500, agb1500_CH_ALL, OFF);
    if(ret!=VI_SUCCESS){
        check_err_B1500(vi_B1500, ret);
        return ERR_SWITCH_OFF;
    }*/

    return Result;
}
/*-----------------------------------------------------------------------------------------------*/
