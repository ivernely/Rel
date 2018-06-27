#include "eq_control.h"
#include "utils.h"
#include "visa.h"
#include <QDateTime>

/*-----------------------------------------------------------------------------------------------*/
bool SetPins(QLineEdit *le, ViInt32 pins[])
{
    unsigned        cnt,
                    pin,
                    max_pins;

    bool Ok = false;
    char *pSt;

    QByteArray ba;

    if(le->text().isEmpty())
        return Ok;

    max_pins = NUM_CARD_PINS;//NUM_CHAN_IN_MTX_CARD*NUM_MTX_CARDS;
    cnt = 0;

    pin=0;
    ba = le->text().toLatin1();
    pSt = strtok(ba.data(), ";, ");
    if(pSt)
        pin = atoi(pSt);

    if(pin>0 && pin<=max_pins)
    {
        pins[cnt++] = pin;

        while(pSt)
        {
            pSt = strtok(NULL, " ;,");
            if(pSt)
            {
                pin = atoi(pSt);
                if(pin>0 && pin<=max_pins)
                    pins[cnt++] = pin;
            }
        }
    }

    for(unsigned i=0; i<cnt; i++)
        if(pins[i] > 0)
        {
            Ok = true;
            break;
        }

    return Ok;
}
/*-----------------------------------------------------------------------------------------------*/
bool SetPinsVect(const QString &txt, QVector<ViInt32> &pins)
{
    unsigned        pin,
                    max_pins;

    bool Ok = false;
    char *pSt;

    QByteArray ba;

    if(txt.isEmpty())
        return Ok;

    max_pins = NUM_CARD_PINS;//NUM_CHAN_IN_MTX_CARD*NUM_MTX_CARDS;

    ba = txt.toLatin1();
    pSt = strtok(ba.data(), " ;,");
    if(pSt)
        pin = atoi(pSt);

    if(pin>0 && pin<=max_pins)
    {
        pins.push_back(pin);

        while(pSt)
        {
            pSt = strtok(NULL, ";, ");
            if(pSt)
            {
                pin = atoi(pSt);
                if(pin>0 && pin<=max_pins)
                    pins.push_back(pin);
            }
        }
    }

    if(pins.size()>0)
        Ok = true;

    return Ok;
}
/*-----------------------------------------------------------------------------------------------*/
bool SetChipsVect(QLineEdit *le, QVector<unsigned> &chips)
{
    unsigned        chip,
                    max_chips;

    bool Ok = false;
    char *pSt;

    QByteArray ba;

    if(le->text().isEmpty())
        return Ok;

    max_chips = MAX_CHIP_NUM;

    ba = le->text().toLatin1();
    pSt = strtok(ba.data(), " ;,");
    if(pSt)
        chip = atoi(pSt);

    if(chip>0 && chip<=max_chips)
    {
        chips.push_back(chip);

        while(pSt)
        {
            pSt = strtok(NULL, ";, ");
            if(pSt)
            {
                chip = atoi(pSt);
                if(chip>0 && chip<=max_chips)
                    chips.push_back(chip);
            }
        }
    }

    if(chips.size()>0)
        Ok = true;

    return Ok;
}
/*-----------------------------------------------------------------------------------------------*/
bool SetPinsXML(QString str, QVector<ViInt32> &pins)
{
    unsigned        pin,
                    max_pins;

    bool Ok = false;
    char *pSt;

    QByteArray ba;

    if(str.isEmpty())
        return Ok;

    pins.clear();
    max_pins = NUM_CARD_PINS;//NUM_CHAN_IN_MTX_CARD*NUM_MTX_CARDS;

    pin=0;
    ba = str.toLatin1();
    pSt = strtok(ba.data(), ";, ");
    if(pSt)
        pin = atoi(pSt);

    if(pin>0 && pin<=max_pins){
        pins.push_back(pin);
        while(pSt){
            pSt = strtok(NULL, ";, ");
            if(pSt){
                pin = atoi(pSt);
                if(pin>0 && pin<=max_pins)
                    pins.push_back(pin);
            }
        }
    }

    if(pins.size()<1)
        Ok = false;

    return Ok;
}
/*-----------------------------------------------------------------------------------------------*/
bool SetChipsXML(QString str, ViInt32 chips[])
{
    unsigned        cnt,
                    chip,
                    max_chips;

    bool Ok = false;
    char *pSt;

    QByteArray ba;

    if(str.isEmpty())
        return Ok;

    memset(chips, 0, MAX_CHIP_NUM*sizeof(unsigned));

    max_chips = MAX_CHIP_NUM;
    cnt = 0;

    chip=0;
    ba = str.toLatin1();
    pSt = strtok(ba.data(), ";, ");
    if(pSt)
        chip = atoi(pSt);

    if(chip>0 && chip<=max_chips){
        chips[cnt++] = chip;

        while(pSt){
            pSt = strtok(NULL, ";, ");
            if(pSt){
                chip = atoi(pSt);
                if(chip>0 && chip<=max_chips)
                    chips[cnt++] = chip;
            }
        }
    }

    for(unsigned i=0; i<cnt; i++)
        if(chips[i] > 0){
            Ok = true;
            break;
        }

    return Ok;
}
/*-----------------------------------------------------------------------------------------------*/
ViStatus Init_B1500(ViSession &vi)
{
    ViStatus ret;
    ViChar err_msg[INPUT_BUF_SIZE];
    char address[32] = {0};

    sprintf(address, "GPIB%d::%d::INSTR", GPIB_CHANNEL, B1500_GPIB_ADDRESS);

    /* Starting the session */
    ret = agb1500_init(ViRsrc(address), VI_TRUE, VI_TRUE, &vi);
    if ( ( ret < VI_SUCCESS ) || ( vi == VI_NULL ) ){
        QMessageBox::critical(0, QString("Connection error"), QString("Agilent B1500 initialization failure. Status code: %1.").arg(ret), QMessageBox::Ok);
        if ( vi != VI_NULL ){
            agb1500_error_message(vi, ret, err_msg);
            QMessageBox::critical(0, QString("Connection error"), QString("Agilent B1500 error: %1\n%2").arg(ret).arg(err_msg), QMessageBox::Ok);
        }
     }

    return ret;
}
/*-----------------------------------------------------------------------------------------------*/
void check_err_B1500 (const ViSession &vi, ViStatus &ret)
{
    ViInt32 inst_err;
    ViChar err_msg[INPUT_BUF_SIZE];

    if(VI_SUCCESS > ret)
    {
        if ( agb1500_INSTR_ERROR_DETECTED == ret )
            agb1500_error_query(vi, &inst_err, err_msg);
        else
            agb1500_error_message(vi, ret, err_msg);
    }
}
/*-----------------------------------------------------------------------------------------------*/
ViStatus Init_2200A(ViSession &vi)
{
    ViStatus ret;
    ViChar err_msg[INPUT_BUF_SIZE];
    char address[32] = {0};

    sprintf(address, "GPIB%d::%d::INSTR", GPIB_CHANNEL, A2200_GPIB_ADDRESS);

    /* Starting the session */
    ret = agb220xa_init(ViRsrc(address), VI_TRUE, VI_TRUE, &vi);
    if ( ( ret < VI_SUCCESS ) || ( vi == VI_NULL ) )
    {
        QMessageBox::critical(0, "Matrix error", QString("Agilent 2200A initialization failure. Status code: %1.").arg(ret),
                              QMessageBox::Ok);
        if ( vi != VI_NULL )
        {
            agb220xa_error_message(vi, ret, err_msg);
            QMessageBox::critical(0, "Matrix Error", QString("Agilent 2200A0 error: %1\n%2").arg(ret).arg(err_msg),
                                  QMessageBox::Ok);
        }
    }
    else
    {
        ret = agb220xa_func(vi, 1);
        check_err_2200A(vi, ret);

        ret = agb220xa_connRuleSeq(vi, 5, 0, 1);
        check_err_2200A(vi, ret) ;
    }

    return ret;
}
/*-----------------------------------------------------------------------------------------------*/
bool CheckConfiguration(ViSession &vi)
{
    ViStatus    errStatus = 0;
    ViUInt32    received;
    bool        Result;
    char        read_buf[513] = {0};

    errStatus = GPIB_cmd(vi, "UNT? 1");

    if(errStatus == VI_SUCCESS){
        try{
            viRead(vi, (ViBuf)read_buf, 512, &received);
            Result = true;
        }
        catch(...){
            Result = false;
        }
    }
    else
        Result = false;

    return Result;
}
/*-----------------------------------------------------------------------------------------------*/
void check_err_2200A (const ViSession &vi, ViStatus &ret)
{
    ViInt32 inst_err;
    ViChar err_msg[INPUT_BUF_SIZE];

    if(VI_SUCCESS > ret)
    {
        if ( agb220xa_INSTR_ERROR_DETECTED == ret )
            agb220xa_error_query(vi, &inst_err, err_msg);
        else
            agb220xa_error_message(vi, ret, err_msg);
    }
}
/*-----------------------------------------------------------------------------------------------*/
bool Init_Cascade12k(ViSession &vi)
{
    ViStatus ret;
    ViChar err_msg[INPUT_BUF_SIZE];
    char address[256] = {0};

    sprintf(address, "GPIB%d::%d::INSTR", GPIB_CHANNEL, SUMMIT12K_GPIB_ADDRESS);

    /* Starting the session */
    ret = csc12000k_init(ViRsrc(address), VI_TRUE, VI_FALSE, &vi);

    if ( ( ret < VI_SUCCESS ) || ( vi == VI_NULL ) ){
        QMessageBox::critical(0, "Prober error", QString("Cascade Summit 12k initialization failure. Status code: %1.").arg(ret),
                              QMessageBox::Ok);

        if ( vi != VI_NULL ){
            csc12000k_error_message(vi, ret, err_msg);
            QMessageBox::critical(0, "Prober error", QString("Cascade Summit 12k error: %1\n%2").arg(ret).arg(err_msg),
                                  QMessageBox::Ok);
        }

        return false;
    }
    else
        return true;
}
/*-----------------------------------------------------------------------------------------------*/
void check_err_12K (const ViSession &vi, ViStatus &ret)
{
    ViInt32 inst_err;
    ViChar err_msg[INPUT_BUF_SIZE];

    if(VI_SUCCESS > ret)
    {
        if ( agb220xa_INSTR_ERROR_DETECTED == ret )
            csc12000k_error_query(vi, &inst_err, err_msg);
        else
            csc12000k_error_message(vi, ret, err_msg);
    }
}
/*-----------------------------------------------------------------------------------------------*/
bool ConnectPorts2200AVect(ViSession &vi, ViInt32 &In, QVector<ViInt32> &Out)
{
    ViStatus    ret;
    ViInt32     *commArray;
    ViChar      err_msg[INPUT_BUF_SIZE];

    unsigned    N = Out.size();//NUM_CARD_PINS;

    commArray = new ViInt32[N+1];

    if(!commArray)
        return false;

    memset(commArray, 0, (N+1)*sizeof(ViInt32));

    for (uint i = 0; i < N; i++)
        if(Out.at(i) > 0)
            commArray[i] = InOut2CommProbeCard(In, Out.at(i));


    ret = agb220xa_closeList(vi, commArray);

    delete[] commArray;

    if(VI_SUCCESS > ret){
        agb220xa_error_message(vi, ret, err_msg);
        return false;
    }
    else
        return true;
}
/*-----------------------------------------------------------------------------------------------*/
bool ConnectPorts2200A(ViSession &vi, unsigned In, ViInt32 Out[])
{
    ViStatus    ret;
    ViInt32     *commArray;
    ViChar      err_msg[INPUT_BUF_SIZE];

    unsigned    N = NUM_CARD_PINS;

    commArray = new ViInt32[N+1];

    if(!commArray)
        return false;

    memset(commArray, 0, (N+1)*sizeof(ViInt32));

    for (unsigned i = 0; i < NUM_CARD_PINS; i++)
        if(Out[i] != 0)
            commArray[i] = InOut2CommProbeCard(In, Out[i]);


    ret = agb220xa_closeList(vi, commArray);

    if(commArray)
        delete[] commArray;

    if(VI_SUCCESS > ret){
        agb220xa_error_message(vi, ret, err_msg);
        return false;
    }
    else
        return true;
}
/*-----------------------------------------------------------------------------------------------*/
bool ConnectPort2200A(ViSession &vi, unsigned In, unsigned Out)
{
    ViStatus    ret;
    ViInt32     commPort[2] = {0};
    ViChar      err_msg[INPUT_BUF_SIZE];

    if(Out != 0)
        commPort[0] = InOut2CommProbeCard(In, Out);

    ret = agb220xa_closeList(vi, commPort);

    if(VI_SUCCESS > ret){
        agb220xa_error_message(vi, ret, err_msg);
        return false;
    }
    else
        return true;
}
/*-----------------------------------------------------------------------------------------------*/
bool OpenPort2200A(ViSession &vi, unsigned In, unsigned Out)
{
    ViStatus    ret;
    ViInt32     commPort[1];
    ViChar      err_msg[INPUT_BUF_SIZE];

    commPort[0] = 0;

    if(Out != 0)
        commPort[0] = InOut2CommProbeCard(In, Out);

    ret = agb220xa_openList(vi, commPort);

    if(VI_SUCCESS > ret){
        agb220xa_error_message(vi, ret, err_msg);
        return false;
    }
    else
        return true;
}
/*-----------------------------------------------------------------------------------------------*/
bool OpenPorts2200A(ViSession &vi, unsigned In, unsigned Out[])
{
    ViStatus    ret;
    ViInt32     *commArray;
    ViChar      err_msg[INPUT_BUF_SIZE];

    unsigned    N = NUM_CARD_PINS;

    commArray = new ViInt32[N+1];

    if(!commArray)
        return false;

    memset(commArray, 0, (N+1)*sizeof(ViInt32));

    for (unsigned i = 0; i < NUM_CARD_PINS; i++)
        if(Out[i] != 0)
            commArray[i] = InOut2CommProbeCard(In, Out[i]);


    ret = agb220xa_openList(vi, commArray);

    if(commArray)
        delete[] commArray;

    if(VI_SUCCESS > ret){
        agb220xa_error_message(vi, ret, err_msg);
        return false;
    }
    else
        return true;
}
/*-----------------------------------------------------------------------------------------------*/
bool OpenPorts2200AVect(ViSession &vi, unsigned In, QVector<unsigned> Out)
{
    ViStatus    ret;
    ViInt32     *commArray;
    ViChar      err_msg[INPUT_BUF_SIZE];

    unsigned    N = Out.size();//NUM_CARD_PINS;

    commArray = new ViInt32[N+1];

    if(!commArray)
        return false;

    memset(commArray, 0, (N+1)*sizeof(ViInt32));

    for (unsigned i = 0; i < N; i++)
        if(Out[i] != 0)
            commArray[i] = InOut2CommProbeCard(In, Out[i]);


    ret = agb220xa_openList(vi, commArray);

    if(commArray)
        delete[] commArray;

    if(VI_SUCCESS > ret){
        agb220xa_error_message(vi, ret, err_msg);
        return false;
    }
    else
        return true;
}
/*-----------------------------------------------------------------------------------------------*/
void OpenCommMatrix(ViSession &vi)
{
    ViStatus ret;
    ViChar err_msg[INPUT_BUF_SIZE];

    if(vi == VI_NULL)
        return;

    ret = agb220xa_openCard(vi, 5);       //open all
    if(VI_SUCCESS > ret)
        agb220xa_error_message(vi, ret, err_msg);
}
/*-----------------------------------------------------------------------------------------------*/
unsigned InOut2CommProbeCard(unsigned in, unsigned out)
{
    unsigned	comm,
                card,
                chann;

    card = (out/NUM_CHAN_IN_MTX_CARD)+1;
    chann =(out%NUM_CHAN_IN_MTX_CARD);

    if(!chann)
    {
        chann = NUM_CHAN_IN_MTX_CARD;
        card--;
    }

    if (card>NUM_MTX_CARDS || chann>NUM_MTX_INPUTS)
        comm = 0;
    else
        comm = card*10000 + in*100 + chann;

    return comm;
}
/*-----------------------------------------------------------------------------------------------*/
void CloseConnection(ViSession &b1500, ViSession &mtx)
{
    if(b1500 != VI_NULL){
        agb1500_dcl(b1500);
        agb1500_close(b1500);
        QThread::msleep(1000);
    }

    if(mtx != VI_NULL){
        agb220xa_close(mtx);
        QThread::msleep(1000);
    }

    b1500 = VI_NULL;
    mtx = VI_NULL;
}
/*-----------------------------------------------------------------------------------------------*/
void CloseCascadeConnection(ViSession &vi_12k)
{
    if(vi_12k != VI_NULL){
        csc12000k_close(vi_12k);
        QThread::msleep(500);
    }

    vi_12k = VI_NULL;
}
/*-----------------------------------------------------------------------------------------------*/
//сумма элементов массива от cnt1 до cnt2
double Summ(QVector<double> &in, const int &cnt1, const int &cnt2)
{
    double out = 0.0;

    for (int  i = cnt1; i < cnt2; i++){
        out += fabs(in[i]);
    }

    return out;
}
/*-----------------------------------------------------------------------------------------------*/
//сумма разностей элементов массива от cnt1 до cnt2
double SummDiff(QVector<double> &in, const int &cnt1, const int &cnt2)
{
    double out = 0.0;

    for (int  i = cnt1; i < cnt2-1; i++){
        out += fabs(in[i+1] - in[i]);
    }

    return out;
}
/*-----------------------------------------------------------------------------------------------*/
//сумма квадратов элементов массива от cnt1 до cnt2
double Summ2(QVector<double> &in, const int &cnt1, const int &cnt2)
{
    double out = 0;

    for (int  i = cnt1; i < cnt2; i++){
        out += in[i]*in[i];
    }

    return out;
}
/*-----------------------------------------------------------------------------------------------*/
//сумма квадрата разности элементов массива от cnt1 до cnt2
double Summ2Diff(QVector<double> &in, const int &cnt1, const int &cnt2)
{
    double out = 0;

    for (int  i = cnt1; i < cnt2-1; i++){
        out += (in[i+1]*in[i+1] - in[i]*in[i]);
    }

    return out;
}
/*-----------------------------------------------------------------------------------------------*/
double Average(QVector<double> &in, const int &cnt1, const int &cnt2)
{
    return Summ(in, cnt1, cnt2)/(cnt2-cnt1);
}
/*-----------------------------------------------------------------------------------------------*/
//линейная аппроксимация в виде Alin*x+Blin
bool Approx(QVector<double> &x, QVector<double> &y, const int &cnt1, const int &cnt2, double &R, double &T)
{
    double  Kxy,
            Dx,
            Mx,
            My,
            A;

    bool    Ok;

    Kxy = 0.0;
    Dx = 0.0;

    Mx = Average(x, cnt1, cnt2);
    My = Average(y, cnt1, cnt2);

    for (int  i = cnt1; i < cnt2; i++){
        Kxy += (x[i]-Mx)*(y[i]-My);
        Dx+=(x[i]-Mx)*(x[i]-Mx);
    }

    if(fabs(Dx)>1e-12)                                                  //why we use 1E-9????????????
    {
        A = Kxy/Dx ;
        R = A;
        T = My - A*Mx;

        Ok = true;
    }
    else
    {
        R = 0.0;
        T = 0.0;

        Ok = false;
    }

    return Ok;
}
/*-----------------------------------------------------------------------------------------------*/
double MaxAbs(double in[], const int &size)
{
    double m;

    m = fabs(in[0]);
    for(int i=1; i<size; i++)
        if(m>fabs(in[i]))
            m = in[i];

    return m;
}
/*-----------------------------------------------------------------------------------------------*/
ViStatus VthBinary(const ViSession &vi, const ViInt32 &drain, const ViInt32 &gate, const double &Vdrain, const double &Vstart, const double &Vstop,
                            const double &Icompl, const double &Itarget, const int &Steps,  double &Vth)
{
    int N;
    ViStatus ret;
    ViInt32 status;

    double  Imeas,
            Itgt,
            Vst;
    double  D,
            Vg;

    N = 0;
    D = Vstop - Vstart;
    Vg = 0.0;
    Imeas = 0.0;
    Itgt = fabs(Itarget);
    Vst = fabs(Vstop);

    ret = agb1500_force(vi, drain, agb1500_VF_MODE, agb1500_AUTO_RANGE, Vdrain, Icompl, agb1500_POL_AUTO);
    if(ret<VI_SUCCESS)
        return ret;

    while(N<Steps){

        N++;

        if(fabs(Imeas) > Itgt)
            Vg = Vg - D/pow(2,N);
        else
            Vg = Vg + D/pow(2,N);

        if(fabs(Vg)<=Vst)
            ret = agb1500_force(vi, gate, agb1500_VF_MODE, agb1500_AUTO_RANGE, Vg, Icompl, agb1500_POL_AUTO);

        ret = agb1500_spotMeas(vi, drain, agb1500_IM_MODE, agb1500_LMA_10nA_RANGE, &Imeas, &status, NULL);
        if(status!=0)
            ret = agb1500_spotMeas(vi, drain, agb1500_IM_MODE, agb1500_AUTO_RANGE, &Imeas, NULL, NULL);
    }

    if(ret!=VI_SUCCESS)
        return ret;

    Vth = Vg;

    return VI_SUCCESS;
}
/*-----------------------------------------------------------------------------------------------*/
ViStatus VthBinaryM(const ViSession &vi, QVector<ViInt32> &drain, QVector<ViInt32> &gate, QVector<double> &Vdrain, QVector<double> &Vstart, QVector<double> &Vstop,
                           QVector<double> &Icompl, QVector<double> &Itarget, const int &Steps,  QVector<double> &Vth)
{
    int     N,
            i,
            Size = drain.size();

    ViStatus ret;

    ViInt32     mode[Size],
                ch[Size+1];

    ViReal64    rng[Size],
                Imeas[Size];


    QVector<double> D,
                    Itgt,
                    Vst,
                    Vg;

    N = 0;
    Vth.clear();
    memset(ch, 0, (Size+1)*sizeof(ViInt32));

    ret = agb1500_zeroOutput(vi, agb1500_CH_ALL);
    if(ret!=VI_SUCCESS)
        return ret;

    for(i=0; i<Size; i++){
        D.push_back(Vstop[i] - Vstart[i]);
        Imeas[i] = 0.0;
        Vg.push_back(0.0);
        Itgt.push_back(fabs(Itarget[i]));
        Vst.push_back(fabs(Vstop[i]));

        ch[i] = drain[i];
        mode[i] = agb1500_IM_MODE;
        rng[i] = agb1500_AUTO_RANGE;
    }


    for(i=0; i<Size; i++){
        ret = agb1500_force(vi, ch[i], agb1500_VF_MODE, agb1500_AUTO_RANGE, Vdrain[i], Icompl[i], agb1500_POL_AUTO);
        if(ret!=VI_SUCCESS)
            return ret;
    }

    while(N<Steps){

        N++;

        //анализ измерений
        for(i=0; i<Size; i++){
            if(fabs(Imeas[i]) > Itgt[i])
                Vg[i] = Vg[i] - D[i]/pow(2,N);
            else
                Vg[i] = Vg[i] + D[i]/pow(2,N);

            if(fabs(Vg[i])<=Vst[i])
                ret = agb1500_force(vi, gate[i], agb1500_VF_MODE, agb1500_AUTO_RANGE, Vg[i], Icompl[i], agb1500_POL_AUTO);
        }

        ret = agb1500_measureM(vi, ch, mode, rng, &Imeas[0],  NULL,  NULL);
    }

    if(ret<VI_SUCCESS)
        return ret;

    for(i=0; i<Size; i++)
        Vth.push_back(Vg[i]);

    return VI_SUCCESS;
}
/*-----------------------------------------------------------------------------------------------*/
double MaxGm(double *Idrain, double Vstart, double Vstep, double Vdsat, int N, bool bRegime, double &Vt_Gmmax)
{
    double MaxGm = 0, Gm;
    int i;
    double *V = new double[N];

    for(i=0; i<N; i++)
        V[i] = Vstart + Vstep*i;

    if(bRegime==HC_LINEAR_REGIME){
        for(i=1; i<N; i++){
           Gm = fabs(Idrain[i]-Idrain[i-1])/fabs(V[i]-V[i-1]);

           if(Gm > MaxGm){
               MaxGm = Gm;
               Vt_Gmmax = V[i]-Vdsat/2.0;
           }
        }
    }//end if

    if(bRegime==HC_SATURATION_REGIME){
        for(i=1; i<N; i++){
           Gm = fabs(sqrt(fabs(Idrain[i]))-sqrt(fabs(Idrain[i-1])))/fabs(V[i]-V[i-1]);

           if(Gm > MaxGm){
               MaxGm = Gm;
               Vt_Gmmax = V[i];
           }
        }
    }//end if



    delete[] V;
    return MaxGm;
}
/*-----------------------------------------------------------------------------------------------*/
double SetVcompliance(const double &Iforce)
{
    double I = fabs(Iforce);
    double V;

    if(I<0.02)
        V = 100.0;
    else
        if(I<0.05)
            V = 40.0;
        else
            V = 20.0;

    return V;
}
/*-----------------------------------------------------------------------------------------------*/
double SetHIcompliance(const double &Vforce)
{
    double V = fabs(Vforce);
    double I;

    if(V<20.0)
        I = 1.0;
    else
        if(V<40.0)
            I = 0.5;
        else
            if(V<100.0)
                I = 0.125;
            else
                I = 0.05;

    return I;
}
/*-----------------------------------------------------------------------------------------------*/
double SetIcompliance(const double &Vforce)
{
    double V = fabs(Vforce);
    double I;

    if(V<20.0)
        I = 0.1;
    else
        if(V<40.0)
            I = 0.05;
        else
            I = 0.02;

    return I;
}
/*-----------------------------------------------------------------------------------------------*/
double SetHVcompliance(const double &Iforce)
{
    double I = fabs(Iforce);
    double V;

    if(I<0.05)
        V = 200.0;
    else
        if(I<0.125)
            V = 100.0;
        else
            if(I<0.5)
                V = 40.0;
            else
                V = 20.0;

    return V;
}
/*-----------------------------------------------------------------------------------------------*/
double SetFixIrange(const double &Iforce)
{
    double I = fabs(Iforce);

    if(I<=1E-9)
        return agb1500_FIX_1nA_RANGE;
    if(I<=1E-8)
        return agb1500_FIX_10nA_RANGE;
    if(I<=1E-7)
        return agb1500_FIX_100nA_RANGE;
    if(I<=1E-6)
        return agb1500_FIX_1uA_RANGE;
    if(I<=1E-5)
        return agb1500_FIX_10uA_RANGE;
    if(I<=1E-4)
        return agb1500_FIX_100uA_RANGE;
    if(I<=1E-3)
        return agb1500_FIX_1mA_RANGE;
    if(I<=1E-2)
        return agb1500_FIX_10mA_RANGE;
    if(I<=1E-1)
        return agb1500_FIX_100mA_RANGE;

    return agb1500_AUTO_RANGE;
}
/*-----------------------------------------------------------------------------------------------*/
double SetLimIrange(const double &Iforce)
{
    double I = fabs(Iforce);
    double Lim;

    if(I<=1E-9)
        Lim = agb1500_LMA_1nA_RANGE;
    else
    if(I<=1E-8)
        Lim = agb1500_LMA_10nA_RANGE;
    else
    if(I<=1E-7)
        Lim = agb1500_LMA_100nA_RANGE;
    else
    if(I<=1E-6)
        Lim = agb1500_LMA_1uA_RANGE;
    else
    if(I<=1E-5)
        Lim = agb1500_LMA_10uA_RANGE;
    else
    if(I<=1E-4)
        Lim = agb1500_LMA_100uA_RANGE;
    else
    if(I<=1E-3)
        Lim = agb1500_LMA_1mA_RANGE;
    else
    if(I<=1E-2)
        Lim = agb1500_LMA_10mA_RANGE;
    else
    if(I<=1E-1)
        Lim = agb1500_LMA_100mA_RANGE;
    else
        Lim = agb1500_AUTO_RANGE;

    return Lim;
}
/*-----------------------------------------------------------------------------------------------*/
double SetLimVrange(const double &Vforce)
{
    double V = fabs(Vforce);

    if(V<=2.0)
        return agb1500_LMA_2V_RANGE;
    if(V<=20.0)
        return agb1500_LMA_20V_RANGE;
    if(V<=40.0)
        return agb1500_LMA_40V_RANGE;
    if(V<=100.0)
        return agb1500_LMA_100V_RANGE;

    return agb1500_AUTO_RANGE;
}
/*-----------------------------------------------------------------------------------------------*/
int SetLimVrangeGPIB(const double &Vforce)
{
    int Vrng;
    double V;

    V = fabs(Vforce);

    if(V<=2.0)
        Vrng = 11;
    else
    if(V<=20.0)
        Vrng = 12;
    else
    if(V<=40.0)
        Vrng = 13;
    else
    if(V<=100.0)
        Vrng = 14;
    else
        Vrng = 0;

    return Vrng;
}
/*-----------------------------------------------------------------------------------------------*/
int SetFixIrangeGPIB(const double &Iforce)
{
    int Irng;
    double I;
    I = fabs(Iforce);

    if(I<=1E-9)
        Irng = -11;
    else
    if(I<=1E-8)
        Irng = -12;
    else
    if(I<=1E-7)
        Irng = -13;
    else
    if(I<=1E-6)
        Irng = -14;
    else
    if(I<=1E-5)
        Irng = -15;
    else
    if(I<=1E-4)
        Irng = -16;
    else
    if(I<=1E-3)
        Irng = -17;
    else
    if(I<=1E-2)
        Irng = -18;
    else
    if(I<=1E-1)
        Irng = -19;
    else
        Irng = 0;

    return Irng;
}
/*-----------------------------------------------------------------------------------------------*/
QString CurrentDate()
{
    QDateTime currTime = QDateTime::currentDateTime();

    return currTime.toString("hh:mm:ss dd.MM.yyyy");
}
/*-----------------------------------------------------------------------------------------------*/
ViStatus HighSpeedIMeas(const ViSession &vi, int channel, double Vforce, double Irange, double Icompl,
                                                                    double &value, double &time)
{
    ViStatus    errStatus = 0;
    ViUInt32    received;
    char        read_buf[40] = {0};
    ViInt32     Vrng, Irng;
    double      V, I;

    I = fabs(Irange);

    if(I<=1E-9)
        Irng = 11;
    else
    if(I<=1E-8)
        Irng = 12;
    else
    if(I<=1E-7)
        Irng = 13;
    else
    if(I<=1E-6)
        Irng = 14;
    else
    if(I<=1E-5)
        Irng = 15;
    else
    if(I<=1E-4)
        Irng = 16;
    else
    if(I<=1E-3)
        Irng = 17;
    else
    if(I<=1E-2)
        Irng = 18;
    else
    if(I<=1E-1)
        Irng = 19;
    else
        Irng = 0;


    value = -777777.77;

    V = fabs(Vforce);

    if(V<=2.0)
        Vrng = 11;
    else
    if(V<=20.0)
        Vrng = 12;
    else
    if(V<=40.0)
        Vrng = 13;
    else
    if(V<=100.0)
        Vrng = 14;
    else
        Vrng = 0;

    errStatus = GPIB_cmd(vi, "FMT 22,0");
    errStatus = GPIB_cmd(vi, "DV %d,%d,%e,%e,0,0", channel, Vrng, V, Icompl);
    errStatus = GPIB_cmd(vi, "TSR");
    errStatus = GPIB_cmd(vi, "TTI %d,%d", channel, Irng);
    errStatus = GPIB_cmd(vi, "TSQ");

   try{
        viRead(vi, (ViBuf)read_buf, 36, &received);
        time = atof(strtok(read_buf,","));
        value = atof(strtok(NULL,"\r\n"));
    }
    catch(...){
        return ERR_MEAS;
    }

    return errStatus;
}
/*-----------------------------------------------------------------------------------------------*/
ViStatus PulseSpotMeas(const ViSession &vi, int channel, double Vpulse, double Vbase, double Irange,
                                        double Icompl, double hold, double width, double &value, int &status)
{
    ViStatus    errStatus = 0;
    ViUInt32    received;
    char        read_buf[20] = {0};
    double      V, I;
    ViInt32     Vrng, Irng;


    I = fabs(Irange);

    if(I<=1E-9)
        Irng = -11;
    else
    if(I<=1E-8)
        Irng = -12;
    else
    if(I<=1E-7)
        Irng = -13;
    else
    if(I<=1E-6)
        Irng = -14;
    else
    if(I<=1E-5)
        Irng = -15;
    else
    if(I<=1E-4)
        Irng = -16;
    else
    if(I<=1E-3)
        Irng = -17;
    else
    if(I<=1E-2)
        Irng = -18;
    else
    if(I<=1E-1)
        Irng = -19;
    else
        Irng = 0;

    value = -777777.77;

    V = fabs(Vpulse);
    if(V<=2.0)
        Vrng = 11;
    else
    if(V<=20.0)
        Vrng = 12;
    else
    if(V<=40.0)
        Vrng = 13;
    else
    if(V<=100.0)
        Vrng = 14;
    else
        Vrng = 0;


    errStatus = GPIB_cmd(vi, "FMT 21,0");
    errStatus = GPIB_cmd(vi, "PT %e,%e,0,0", hold, width);
    errStatus = GPIB_cmd(vi, "PV %d,%d,%e,%e", channel, Vrng, Vbase, Vpulse, Icompl);
    errStatus = GPIB_cmd(vi, "MM 3,%d", channel);
    errStatus = GPIB_cmd(vi, "CMM %d,1", channel);
    errStatus = GPIB_cmd(vi, "RI %d,%d",channel, Irng);
    errStatus = GPIB_cmd(vi, "XE");

    try{
        viRead(vi, (ViBuf)read_buf, 3, &received);
        status = atoi(read_buf)%2;
        viRead(vi, (ViBuf)read_buf, 2, &received);
        viRead(vi, (ViBuf)read_buf, 17, &received);
        value = atof(strtok(read_buf,"\r\n"));
    }
    catch(...)
    {
        return ERR_MEAS;
    }

    return errStatus;
}
/*-----------------------------------------------------------------------------------------------*/
ViStatus GPIB_cmd(const ViSession &vi, const char* s,...)
{
  char* cmd;
  va_list ap;
  ViStatus errStatus;
  cmd = (char*) calloc(INPUT_BUF_SIZE,sizeof(char));
  va_start(ap,s);
  vsprintf(cmd,s,ap);

  errStatus = viPrintf(vi, ViString("%s\n"), cmd);
  va_end(ap);

  free(cmd);

  return errStatus;
}
/*-----------------------------------------------------------------------------------------------*/
ViStatus C_SweepMeas(const ViSession &vi, const ViInt32 &Freq, const double &OscLevel, const double &Vstart,
                                                        const double &Vstop, const ViInt32 &Points)
{
    ViStatus ret;
    ViInt32 N;

    ViReal64    source[1002] = {0},
                value[2003]={0};

    ret = agb1500_setCmuInteg(vi, agb1500_INTEG_PLC, 1);
    ret = agb1500_setCmuFreq(vi, agb1500_CH_CMU, Freq);
    if(ret!=VI_SUCCESS)
        return ERR_CMUFREQ;
    ret = agb1500_forceCmuAcLevel(vi, agb1500_CH_CMU, OscLevel);
    if(ret!=VI_SUCCESS)
        return ERR_CMUACLEVEL;

    ret = agb1500_setCv(vi, agb1500_CH_CMU, agb1500_SWP_VF_SGLLIN, Vstart, Vstop, Points, 0.0, 0.0, 0.01);
    if(ret!=VI_SUCCESS)
        return ERR_SETCV;


    agb1500_stopMode(vi, agb1500_FLAG_ON, agb1500_LAST_START);

    agb1500_resetTimestamp(vi);

    ret = agb1500_sweepCv(vi, agb1500_CH_CMU, agb1500_CMUM_CP_RP, agb1500_AUTO_RANGE, &N, &source[0],
    &value[0], NULL, NULL, NULL, NULL);


    agb1500_zeroOutput(vi, agb1500_CH_ALL);
    agb1500_setSwitch(vi, agb1500_CH_ALL, 0);

    return VI_SUCCESS;
}
/*-----------------------------------------------------------------------------------------------*/
