#include "measclasses.h"
#include "eq_control.h"
#include "visa.h"
#include "utils.h"


/*=====================================================================================================*/
SMU::SMU()
{
    clear();
}
/*=====================================================================================================*/
void SMU::clear()
{
    Name = "";
    Model = 0;
    Num = 0;
    Slot = 0;
    ForceMode = agb1500_IM_MODE;
    MeasMode = agb1500_VF_MODE;
    Range = agb1500_AUTO_RANGE;

    Compl = 0.0;

    pins.clear();
}
/*=====================================================================================================*/
Transistor::Transistor()
{
    clear();
}
/*=====================================================================================================*/
void Transistor::clear()
{
    drain.clear();
    gate.clear();
    source.clear();
    body.clear();

    Index = 0;

    comment = "";
    TypeCh = NCHANN;

    Idmax = 0.0;
    W = L = Vdlin = Vdsat = 0.0;
    Vgstart = Vgstop = Ith_sq = Ith = 0.0;

    clear_data();
}
/*=====================================================================================================*/
void Transistor::clear_data()
{    
    Idfl.clear();
    Idfs.clear();
    Irfs.clear();
    Vlin.clear();
    Vsat.clear();
    Gm.clear();
    Vt_Gmmax.clear();
}
/*=====================================================================================================*/
ViStatus b1500_FindSMU(QVector<SMU> &smu)
{
    ViStatus    errStatus = VI_SUCCESS;
    ViUInt32    received;
    SMU         *curr_smu;
    QVector<SMU>::iterator it;
    ViSession vi;

    char        *ptr;

    char    Modules[BUF_MODULES] = {0};
    int     cnt_smu,
            cnt_slot;

    //инициализация приборов
    errStatus = Init_B1500(vi);
    if(errStatus != VI_SUCCESS)
        return errStatus;

    try{
        errStatus = viPrintf(vi, ViString("UNT? 0\n"));
        errStatus = viRead(vi, (ViBuf)Modules, BUF_MODULES-1, &received);
    }
    catch(...){
        return errStatus;
    }

    agb1500_dcl(vi);
    agb1500_close(vi);
    vi = VI_NULL;

    cnt_smu = 0;
    cnt_slot = 0;

    ptr = strtok(Modules,";,\n");
    cnt_slot++;
    curr_smu = new SMU();
    smu.clear();

    if(ptr!=NULL){
        if(strstr(ptr, "15") && !strstr(ptr, "1525")){
            cnt_smu++;
            curr_smu->Model = atoi(ptr);
            curr_smu->Num = cnt_smu;
            curr_smu->Slot = cnt_slot;
            smu.push_back(*curr_smu);
        }
        ptr = strtok(NULL, ";,\n");
    }

    while(ptr!=NULL){
        ptr = strtok(NULL, ";,\n");
        cnt_slot++;
        if(ptr!=NULL){
            if(strstr(ptr, "15") && !strstr(ptr, "1525")){
                cnt_smu++;
                curr_smu->Model = (QString("%1").arg(ptr)).toInt();
                curr_smu->Num = cnt_smu;
                curr_smu->Slot = cnt_slot;
                smu.push_back(*curr_smu);
            }
            ptr = strtok(NULL, ";,\n");
        }
    }//end while

    curr_smu->Model = GNDSMU;
    curr_smu->Num = GNDU;
    curr_smu->Slot = SLOT_GNDU;
    curr_smu->Name = QString("GNDU");
    smu.push_back(*curr_smu);
    delete curr_smu;

    for(it = smu.begin(); it<smu.end(); it++){
        switch(it->Model){
            case HPSMU: it->Name = QString("HP:SMU%1").arg(it->Num);break;//high-power module
            case MPSMU: it->Name = QString("MP:SMU%1").arg(it->Num);break;//medium-power module
            case HRSMU: it->Name = QString("HR:SMU%1").arg(it->Num);break;//high-resolution module
            case MCSMU: it->Name = QString("MC:SMU%1").arg(it->Num);break;//medium-current module
            default: continue; break;
        };
    }

    if(smu.size()<2)
       errStatus = agb1500_INSTR_ERROR_DETECTED;

    return errStatus;
}
/*=====================================================================================================*/
QString SetDelim(const char &symbol, const int &num)
{
    QString s;

    s = "";
    for(int i=0; i<num; i++)
        s = s + QString("%1").arg(symbol);

    return s;
}
/*=====================================================================================================*/
Capacitor::Capacitor()
{
    clear();
}
/*=====================================================================================================*/
void Capacitor::clear()
{
    High.clear();
    Low.clear();

    comment= "";

    Index = 0;
    TypeCh = NCHANN;  //тип проводимости канала - 1=N, 0=P

    W = 0.0;
    L = 0.0;
    tox = 0.0;
    Vsilc = 0.0;
    Vuse = 0.0;
    Vstress = 0.0;
    twait = 0.0;

    Iuse_init = 0.0;
    Istress_init = 0.0;

    clear_data();
}
/*=====================================================================================================*/
void Capacitor::clear_data()
{
    Isilc_init = 0.0;
    FailureType = 0;

    Iinit = Iend = 0.0;
    tbd_hard = tbd_soft = tbd_noise = 0.0;

    Inoise_hard = Inoise_soft = Inoise_noise = 0.0;
    qbd_hard = qbd_soft = qbd_noise = 0.0;
    Ibd_hard = Ibd_soft = Ibd_noise = 0.0;
    Isilc_hard = Isilc_soft = Isilc_noise = 0.0;

    Istress.clear();
    Isilc.clear();
    Qbd.clear();
    Inoise.clear();
}
/*=====================================================================================================*/
