#include "XML.h"
#include "QDomDocument"
#include "QDomElement"
#include "QDomNode"
#include "QDebug"

/*------------------------------------------------------------------------------------------------*/
//сохранение исходных данных по электромиграционным испытаниям
void SaveEMXml(const QString &FileName, const EM* const EMTest)
{
    QString s;
    QFile file;
    QDomElement  elem;
    QDomDocument doc;
    QDomElement  MainElement,
                 SecondElement;
    QByteArray buff;

    QVector<const ViInt32>::iterator it;

    file.setFileName(FileName);

    if(file.exists()){
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(0, "Error", QString("Bad XML-file: Open."), QMessageBox::Ok);
            return;
        }

        buff = file.readAll();
        file.close();

        if(false == doc.setContent(buff)){
            QMessageBox::critical(0, "Error", QString("Bad XML-file: setContent."), QMessageBox::Ok);
            return;
        }

        MainElement = doc.documentElement();
    }
    else{
        QDomNode xmlNode = doc.createProcessingInstruction("xml",
                                         "version=\"1.0\" encoding=\"UTF-8\"");
        doc.insertBefore(xmlNode, doc.firstChild());

        MainElement = doc.createElement("TESTS");
        doc.appendChild(MainElement);
    }

    SecondElement = doc.createElement("EM");
    MainElement.appendChild(SecondElement);


    elem = doc.createElement("Num");
    elem.setAttribute("value", QString("%1").arg(EMTest->Number));
    SecondElement.appendChild(elem);

    elem = doc.createElement("ForceHigh");
    elem.setAttribute("SMU", QString("%1").arg(EMTest->forceH.Num));
    elem.setAttribute("Slot", QString("%1").arg(EMTest->forceH.Slot));
    it = EMTest->forceH.pins.begin();
    s = QString::number(*it);
    for(it+=1; it<EMTest->forceH.pins.end(); it++)
            s = s + "," + QString::number(*it);
    elem.setAttribute("Pins", s);
    SecondElement.appendChild(elem);

    elem = doc.createElement("ForceLow");
    elem.setAttribute("SMU", QString("%1").arg(EMTest->forceL.Num));
    elem.setAttribute("Slot", QString("%1").arg(EMTest->forceL.Slot));
    it = EMTest->forceL.pins.begin();
    s = QString::number(*it);
    for(it+=1; it<EMTest->forceL.pins.end(); it++)
            s = s + "," + QString::number(*it);
    elem.setAttribute("Pins", s);
    SecondElement.appendChild(elem);

    elem = doc.createElement("SenseHigh");
    elem.setAttribute("SMU", QString("%1").arg(EMTest->senseH.Num));
    elem.setAttribute("Slot", QString("%1").arg(EMTest->senseH.Slot));
    it = EMTest->senseH.pins.begin();
    s = QString::number(*it);
    for(it+=1; it<EMTest->senseH.pins.end(); it++)
            s = s + "," + QString::number(*it);
    elem.setAttribute("Pins", s);
    SecondElement.appendChild(elem);

    elem = doc.createElement("SenseLow");
    elem.setAttribute("SMU", QString("%1").arg(EMTest->senseL.Num));
    elem.setAttribute("Slot", QString("%1").arg(EMTest->senseL.Slot));
    it = EMTest->senseL.pins.begin();
    s = QString::number(*it);
    for(it+=1; it<EMTest->senseL.pins.end(); it++)
            s = s + "," + QString::number(*it);
    elem.setAttribute("Pins", s);
    SecondElement.appendChild(elem);

    elem = doc.createElement("ExtMon");
    elem.setAttribute("SMU", QString("%1").arg(EMTest->ExtMon.Num));
    elem.setAttribute("Slot", QString("%1").arg(EMTest->ExtMon.Slot));
    it = EMTest->ExtMon.pins.begin();
    s = QString::number(*it);
    for(it+=1; it<EMTest->ExtMon.pins.end(); it++)
            s = s + "," + QString::number(*it);
    elem.setAttribute("Pins", s);
    SecondElement.appendChild(elem);

    elem = doc.createElement("MeasExt");
    elem.setAttribute("value", QString("%1").arg(EMTest->bMeasExt));
    SecondElement.appendChild(elem);

    elem = doc.createElement("TestType");
    elem.setAttribute("value", QString("%1").arg(EMTest->TestType));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Material");
    elem.setAttribute("value", QString("%1").arg(EMTest->MetalType));
    SecondElement.appendChild(elem);

    elem = doc.createElement("W");
    elem.setAttribute("value", QString("%1").arg(EMTest->W));
    SecondElement.appendChild(elem);

    elem = doc.createElement("H");
    elem.setAttribute("value", QString("%1").arg(EMTest->H));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Tstress");
    elem.setAttribute("value", QString("%1").arg(EMTest->Tstress));
    SecondElement.appendChild(elem);

    elem = doc.createElement("StopCond");
    elem.setAttribute("value", QString("%1").arg(EMTest->StopCond));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Istress");
    elem.setAttribute("value", QString("%1").arg(EMTest->Istress));
    SecondElement.appendChild(elem);

    elem = doc.createElement("TCR");
    elem.setAttribute("value", QString("%1").arg(EMTest->TCRref));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Tref");
    elem.setAttribute("value", QString("%1").arg(EMTest->Tref));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Tchuck");
    elem.setAttribute("value", QString("%1").arg(EMTest->Tchuck));
    SecondElement.appendChild(elem);

    elem = doc.createElement("TimeLimit");
    elem.setAttribute("value", QString("%1").arg(EMTest->TimeLimit));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Ea");
    elem.setAttribute("value", QString("%1").arg(EMTest->Ea));
    SecondElement.appendChild(elem);

    elem = doc.createElement("n");
    elem.setAttribute("value", QString("%1").arg(EMTest->n));
    SecondElement.appendChild(elem);

    elem = doc.createElement("A");
    elem.setAttribute("value", QString("%1").arg(EMTest->A));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Iuse");
    elem.setAttribute("value", QString("%1").arg(EMTest->Iuse));
    SecondElement.appendChild(elem);

    elem = doc.createElement("PulseW");
    elem.setAttribute("value", QString("%1").arg(EMTest->PulseW));
    SecondElement.appendChild(elem);

    if(file.open(QIODevice::WriteOnly)){
        QTextStream(&file) << doc.toString();
        file.close();
    }
}
/*------------------------------------------------------------------------------------------------*/
//считывание исходных данных для электромиграционных испытаний
void ReadEMXml(const QString &FileName, EM* const EMTest)
{
    QDomDocument doc;
    QFile file;
    QDomElement MainElement, tmp1, tmp2;
    QDomNode MainNode, node;

    file.setFileName(FileName);
    if(!file.open(QIODevice::ReadOnly))
        return;

    if(doc.setContent(&file))
        file.close();
    else{
        QMessageBox::critical(0, "Error", QString("Bad XML-file: SetContent."), QMessageBox::Ok);
        return;
    }

    EMTest->Clear();
    MainElement = doc.documentElement();

    if("TESTS" != MainElement.tagName()){
        QMessageBox::critical(0, "Error", QString("Bad XML-file: Read."), QMessageBox::Ok);
        return;
    }

    MainNode = MainElement.firstChild();
    while(!MainNode.isNull()){
        if(MainNode.isElement() && "EM" == MainNode.toElement().tagName()){
            tmp1 = MainNode.toElement();
            //qDebug() << "NodeName: " << tmp.nodeName();
            if(!tmp1.isNull()){
                node = tmp1.firstChild();
                while(!node.isNull()){
                    if(node.isElement()){
                        tmp2 = node.toElement();
                        if(!tmp2.isNull()){
                            if(tmp2.tagName() == "Num"){
                                EMTest->Number = (tmp2. attribute("value", "")).toInt();
                                //qDebug() << tmp2.tagName() << EMTest->Number;
                            }
                            else if(tmp2.tagName() == "ForceHigh"){
                                EMTest->forceH.Slot = (tmp2. attribute("Slot", "")).toInt();
                                //qDebug() << tmp2.tagName() << EMTest->slot[0];
                                EMTest->forceH.Num = (tmp2. attribute("SMU", "")).toInt();
                                //qDebug() << tmp2.tagName() << EMTest->src[0];
                                //qDebug() << tmp2.tagName() << tmp2. attribute("Pins", "");
                                SetPinsXML(tmp2. attribute("Pins", ""), EMTest->forceH.pins);
                            }
                            else if(tmp2.tagName() == "ForceLow"){
                                EMTest->forceL.Slot = (tmp2. attribute("Slot", "")).toInt();
                                //qDebug() << tmp2.tagName() << EMTest->slot[1];
                                EMTest->forceL.Num = (tmp2. attribute("SMU", "")).toInt();
                                //qDebug() << tmp2.tagName() << EMTest->src[1];
                                //qDebug() << tmp2.tagName() << tmp2. attribute("Pins", "");
                                SetPinsXML(tmp2. attribute("Pins", ""), EMTest->forceL.pins);
                            }
                            else if(tmp2.tagName() == "SenseHigh"){
                                EMTest->senseH.Slot = (tmp2. attribute("Slot", "")).toInt();
                                //qDebug() << tmp2.tagName() << EMTest->slot[2];
                                EMTest->senseH.Num = (tmp2. attribute("SMU", "")).toInt();
                                //qDebug() << tmp2.tagName() << EMTest->src[2];
                                //qDebug() << tmp2.tagName() << tmp2. attribute("Pins", "");
                                SetPinsXML(tmp2. attribute("Pins", ""), EMTest->senseH.pins);
                            }
                            else if(tmp2.tagName() == "SenseLow"){
                                EMTest->senseL.Slot = (tmp2. attribute("Slot", "")).toInt();
                                //qDebug() << tmp2.tagName() << EMTest->slot[3];
                                EMTest->senseL.Num = (tmp2. attribute("SMU", "")).toInt();
                                //qDebug() << tmp2.tagName() << EMTest->src[3];
                                //qDebug() << tmp2.tagName() << tmp2. attribute("Pins", "");
                                SetPinsXML(tmp2. attribute("Pins", ""), EMTest->senseL.pins);
                            }
                            else if(tmp2.tagName() == "ExtMon"){
                                EMTest->ExtMon.Slot = (tmp2. attribute("Slot", "")).toInt();
                                //qDebug() << tmp2.tagName() << EMTest->slot[4];
                                EMTest->ExtMon.Num = (tmp2. attribute("SMU", "")).toInt();
                                //qDebug() << tmp2.tagName() << EMTest->src[4];
                                //qDebug() << tmp2.tagName() << tmp2. attribute("Pins", "");
                                SetPinsXML(tmp2. attribute("Pins", ""), EMTest->ExtMon.pins);
                            }
                            else if(tmp2.tagName() == "MeasExt"){
                                EMTest->bMeasExt = (tmp2. attribute("value", "")).toInt();
                                //qDebug() << tmp2.tagName() << EMTest->MeasExt;
                            }
                            else if(tmp2.tagName() == "TestType"){
                                EMTest->TestType = (tmp2. attribute("value", "")).toInt();
                                //qDebug() << tmp2.tagName() << EMTest->TestType;
                            }
                            else if(tmp2.tagName() == "Material"){
                                EMTest->MetalType = (tmp2. attribute("value", "")).toInt();
                                //qDebug() << tmp2.tagName() << EMTest->MetalType;
                            }
                            else if(tmp2.tagName() == "W"){
                                EMTest->W = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << EMTest->W;
                            }
                            else if(tmp2.tagName() == "H"){
                                EMTest->H = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << EMTest->H;
                            }
                            else if(tmp2.tagName() == "Tstress"){
                                EMTest->Tstress = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << EMTest->Tstress;
                            }
                            else if(tmp2.tagName() == "StopCond"){
                                EMTest->StopCond = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << EMTest->StopCond;
                            }
                            else if(tmp2.tagName() == "Istress"){
                                EMTest->Istress = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << EMTest->Istress;
                            }
                            else if(tmp2.tagName() == "TCR"){
                                EMTest->TCRref = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << EMTest->TCRref;
                            }
                            else if(tmp2.tagName() == "Tref"){
                                EMTest->Tref = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << EMTest->Tref;
                            }
                            else if(tmp2.tagName() == "Tchuck"){
                                EMTest->Tchuck = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << EMTest->Tchuck;
                            }
                            else if(tmp2.tagName() == "TimeLimit"){
                                EMTest->TimeLimit = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << EMTest->TimeLimit;
                            }
                            else if(tmp2.tagName() == "Ea"){
                                EMTest->Ea = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << EMTest->Ea;
                            }
                            else if(tmp2.tagName() == "n"){
                                EMTest->n = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << EMTest->n;
                            }
                            else if(tmp2.tagName() == "A"){
                                EMTest->A = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << EMTest->A;
                            }
                            else if(tmp2.tagName() == "Iuse"){
                                EMTest->Iuse = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << EMTest->A;
                            }
                            else if(tmp2.tagName() == "PulseW"){
                                EMTest->PulseW = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << EMTest->A;
                            }
                        }//end if(!tmp2.isNull())
                    }//end if(node.isElement())
                    node = node.nextSibling();
                }//end while(!node.isNull())
            }//end if(!tmp1.isNull())
        }
        MainNode = MainNode.nextSibling();
    }//end while(!MainNode.isNull())
}
/*------------------------------------------------------------------------------------------------*/
//сохранение исходных данных по испытаниям диэлектрика
/*void SaveVRDBXml(const QString &FileName, VRDB* const t, const int &Number)
{
    QString s;
    QFile file;
    int cnt;
    QDomElement  elem;
    QDomDocument doc;
    QDomElement  MainElement,
                 SecondElement;
    QByteArray buff;

    file.setFileName(FileName);

    if(file.exists()){
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(0, "Error", QString("Bad XML-file: Open."), QMessageBox::Ok);
            return;
        }

        buff = file.readAll();
        file.close();

        if(false == doc.setContent(buff)){
            QMessageBox::critical(0, "Error", QString("Bad XML-file: setContent."), QMessageBox::Ok);
            return;
        }

        MainElement = doc.documentElement();
    }
    else{
        QDomNode xmlNode = doc.createProcessingInstruction("xml",
                                         "version=\"1.0\" encoding=\"UTF-8\"");
        doc.insertBefore(xmlNode, doc.firstChild());

        MainElement = doc.createElement("TESTS");
        doc.appendChild(MainElement);
    }

    SecondElement = doc.createElement("VRDB");
    MainElement.appendChild(SecondElement);


    elem = doc.createElement("Num");
    elem.setAttribute("value", QString("%1").arg(Number));
    SecondElement.appendChild(elem);

    elem = doc.createElement("High");
    elem.setAttribute("SMU", QString("%1").arg(t->NumSMUHigh));
    elem.setAttribute("Slot", QString("%1").arg(t->NumSlotHigh));
    elem.setAttribute("Pins", QString("%1").arg(t->HighPin));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Low");
    elem.setAttribute("SMU", QString("%1").arg(t->NumSMULow));
    elem.setAttribute("Slot", QString("%1").arg(t->NumSlotLow));
    cnt = 0;
    s = "";
    for (int i=0; i<t->LowPin.size(); i++)
        if(t->LowPin[i] != 0){
            if(cnt == 0)
                s = s + QString::number(t->LowPin[i]);
            else
                s = s + "," + QString::number(t->LowPin[i]);
            cnt++;
        }
    elem.setAttribute("Pins", s);
    SecondElement.appendChild(elem);

    elem = doc.createElement("ChannType");
    elem.setAttribute("value", QString("%1").arg(t->TypeCh));
    SecondElement.appendChild(elem);

    elem = doc.createElement("TestType");
    elem.setAttribute("value", QString("%1").arg(t->TypeTest));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Temperature");
    elem.setAttribute("value", QString("%1").arg(t->T));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Width");
    elem.setAttribute("value", QString("%1").arg(t->W));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Length");
    elem.setAttribute("value", QString("%1").arg(t->L));
    SecondElement.appendChild(elem);

    elem = doc.createElement("tox");
    elem.setAttribute("value", QString("%1").arg(t->tox));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Vuse");
    elem.setAttribute("value", QString("%1").arg(t->Vuse));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Vstop");
    elem.setAttribute("value", QString("%1").arg(t->Vstop));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Rate");
    elem.setAttribute("value", QString("%1").arg(t->Rate));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Iuse");
    elem.setAttribute("value", QString("%1").arg(t->Iuse));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Slope");
    elem.setAttribute("value", QString("%1").arg(t->Slope));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Imax");
    elem.setAttribute("value", QString("%1").arg(t->Imax));
    SecondElement.appendChild(elem);

    if(file.open(QIODevice::WriteOnly)){
        QTextStream(&file) << doc.toString();
        file.close();
    }
}*/
/*------------------------------------------------------------------------------------------------*/
//считывание исходных данных для испытаний диэлектрика
/*void ReadVRDBXml(const QString &FileName, VRDB* const t)
{
    QDomDocument doc;
    QFile file;
    QDomElement MainElement, tmp1, tmp2;
    QDomNode MainNode, node;
    ViInt32 pins[NUM_CARD_PINS] = {0};

    file.setFileName(FileName);
    if(!file.open(QIODevice::ReadOnly))
        return;

    if(doc.setContent(&file))
        file.close();
    else{
        QMessageBox::critical(0, "Error", QString("Bad XML-file: SetContent."), QMessageBox::Ok);
        return;
    }

    t->Clear();
    MainElement = doc.documentElement();

    if("TESTS" != MainElement.tagName()){
        QMessageBox::critical(0, "Error", QString("Bad XML-file: Read."), QMessageBox::Ok);
        return;
    }

    MainNode = MainElement.firstChild();
    while(!MainNode.isNull()){
        if(MainNode.isElement() && "VRDB" == MainNode.toElement().tagName()){
            tmp1 = MainNode.toElement();
            //qDebug() << "NodeName: " << tmp.nodeName();
            if(!tmp1.isNull()){
                node = tmp1.firstChild();
                while(!node.isNull()){
                    if(node.isElement()){
                        tmp2 = node.toElement();
                        if(!tmp2.isNull()){
                            if(tmp2.tagName() == "Num"){
                                t->Number = (tmp2. attribute("value", "")).toInt();
                                //qDebug() << tmp2.tagName() << t->Number;
                            }
                            else if(tmp2.tagName() == "High"){
                                t->NumSlotHigh = (tmp2. attribute("Slot", "")).toInt();
                                //qDebug() << tmp2.tagName() << t->NumSlotHigh;
                                t->NumSMUHigh = (tmp2. attribute("SMU", "")).toInt();
                                //qDebug() << tmp2.tagName() << t->NumSMUHigh;
                                t->HighPin = (tmp2. attribute("Pins", "")).toInt();
                                //qDebug() << tmp2.tagName() << t->HighPin;
                            }
                            else if(tmp2.tagName() == "Low"){
                                t->NumSlotLow = (tmp2. attribute("Slot", "")).toInt();
                                //qDebug() << tmp2.tagName() << t->NumSlotLow;
                                t->NumSMULow = (tmp2. attribute("SMU", "")).toInt();
                                //qDebug() << tmp2.tagName() << t->NumSMULow;
                                //qDebug() << tmp2.tagName() << tmp2. attribute("Pins", "");
                                //!!!SetPinsXML(tmp2. attribute("Pins", ""), pins);
                                for(int i=0; i<NUM_CARD_PINS; i++)
                                    if(pins[i]!=0)
                                        t->LowPin.push_back(pins[i]);
                            }
                            else if(tmp2.tagName() == "ChannType"){
                                t->TypeCh = (tmp2. attribute("value", "")).toInt();
                                //qDebug() << tmp2.tagName() << t->TypeCh;
                            }
                            else if(tmp2.tagName() == "TestType"){
                                t->TypeTest = (tmp2. attribute("value", "")).toInt();
                                //qDebug() << tmp2.tagName() << t->TypeTest;
                            }
                            else if(tmp2.tagName() == "Temperature"){
                                t->T = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << t->T;
                            }
                            else if(tmp2.tagName() == "Width"){
                                t->W = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << t->W;
                            }
                            else if(tmp2.tagName() == "Length"){
                                t->L = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << t->L;
                            }
                            else if(tmp2.tagName() == "tox"){
                                t->tox = (tmp2. attribute("value", "")).toDouble();
                                //qDebug() << tmp2.tagName() << t->tox;
                            }
                            else if(tmp2.tagName() == "Vuse"){
                                t->Vuse = fabs((tmp2. attribute("value", "")).toDouble());
                                //qDebug() << tmp2.tagName() << t->Vuse;
                            }
                            else if(tmp2.tagName() == "Vstop"){
                                t->Vstop = fabs((tmp2. attribute("value", "")).toDouble());
                                //qDebug() << tmp2.tagName() << t->Vstop;
                            }
                            else if(tmp2.tagName() == "Rate"){
                                t->Rate = fabs((tmp2. attribute("value", "")).toDouble());
                                //qDebug() << tmp2.tagName() << t->Rate;
                            }
                            else if(tmp2.tagName() == "Iuse"){
                                t->Iuse = fabs((tmp2. attribute("value", "")).toDouble());
                                //qDebug() << tmp2.tagName() << t->Iuse;
                            }
                            else if(tmp2.tagName() == "Slope"){
                                t->Slope = fabs((tmp2. attribute("value", "")).toDouble());
                                //qDebug() << tmp2.tagName() << t->Iuse;
                            }
                            else if(tmp2.tagName() == "Imax"){
                                t->Imax = fabs((tmp2. attribute("value", "")).toDouble());
                                //qDebug() << tmp2.tagName() << t->Iuse;
                            }
                        }//end if(!tmp2.isNull())
                    }//end if(node.isElement())
                    node = node.nextSibling();
                }//end while(!node.isNull())
            }//end if(!tmp1.isNull())
        }
        MainNode = MainNode.nextSibling();
    }//end while(!MainNode.isNull())
}*/
/*------------------------------------------------------------------------------------------------*/
//сохранение исходных данных по испытаниям на горячие носители
void SaveHCXml(const QString &FileName, const HC* const hc)
{
    QString s;
    QFile file;
    int cnt;
    QDomElement  elem;
    QDomDocument doc;
    QDomElement  MainElement,
                 SecondElement,
                 ThirdElement;
    QByteArray buff;

    if(hc->MOS.size()<1){
        QMessageBox::critical(0, "Error", QString("No devices for settings saving."), QMessageBox::Ok);
        return;
    }

    file.setFileName(FileName);

    if(file.exists()){
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(0, "Error", QString("Bad XML-file: Open."), QMessageBox::Ok);
            return;
        }

        buff = file.readAll();
        file.close();

        if(false == doc.setContent(buff)){
            QMessageBox::critical(0, "Error", QString("Bad XML-file: setContent."), QMessageBox::Ok);
            return;
        }

        MainElement = doc.documentElement();
    }
    else{
        QDomNode xmlNode = doc.createProcessingInstruction("xml",
                                         "version=\"1.0\" encoding=\"UTF-8\"");
        doc.insertBefore(xmlNode, doc.firstChild());

        MainElement = doc.createElement("TESTS");
        doc.appendChild(MainElement);
    }

    SecondElement = doc.createElement("HC");
    MainElement.appendChild(SecondElement);
    ThirdElement = doc.createElement("Device");

    elem = doc.createElement("Num");
    elem.setAttribute("value", QString("%1").arg(hc->Number));
    SecondElement.appendChild(elem);

    elem = doc.createElement("Temperature");
    elem.setAttribute("value", QString("%1").arg(hc->T));
    SecondElement.appendChild(elem);

    elem = doc.createElement("TimeLimit");
    elem.setAttribute("value", QString("%1").arg(hc->TimeLimit));
    ThirdElement.appendChild(elem);

    elem = doc.createElement("Vgstress");
    elem.setAttribute("value", QString("%1").arg(hc->Vgstress));
    ThirdElement.appendChild(elem);

    elem = doc.createElement("Vdstress");
    elem.setAttribute("value", QString("%1").arg(hc->Vdstress));
    ThirdElement.appendChild(elem);

    elem = doc.createElement("Vsstress");
    elem.setAttribute("value", QString("%1").arg(hc->Vsstress));
    ThirdElement.appendChild(elem);

    elem = doc.createElement("Vbstress");
    elem.setAttribute("value", QString("%1").arg(hc->Vbstress));
    ThirdElement.appendChild(elem);

    for(auto it: hc->MOS){
        ThirdElement = doc.createElement("Device");
        elem = doc.createElement("Index");
        elem.setAttribute("value", QString("%1").arg(it.Index));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Drain");
        elem.setAttribute("SMU", QString("%1").arg(it.drain.Num));
        elem.setAttribute("Slot", QString("%1").arg(it.drain.Slot));
        cnt = 0;
        s = "";
        for(auto p: it.drain.pins)
            if(p > 0){
                if(cnt == 0)
                    s = s + QString::number(p);
                else
                    s = s + "," + QString::number(p);
                cnt++;
            }
        elem.setAttribute("Pins", s);
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Gate");
        elem.setAttribute("SMU", QString("%1").arg(it.gate.Num));
        elem.setAttribute("Slot", QString("%1").arg(it.gate.Slot));
        cnt = 0;
        s = "";
        for(auto p: it.gate.pins)
            if(p > 0){
                if(cnt == 0)
                    s = s + QString::number(p);
                else
                    s = s + "," + QString::number(p);
                cnt++;
            }
        elem.setAttribute("Pins", s);
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Source");
        elem.setAttribute("SMU", QString("%1").arg(it.source.Num));
        elem.setAttribute("Slot", QString("%1").arg(it.source.Slot));
        cnt = 0;
        s = "";
        for(auto p: it.source.pins)
            if(p > 0){
                if(cnt == 0)
                    s = s + QString::number(p);
                else
                    s = s + "," + QString::number(p);
                cnt++;
            }
        elem.setAttribute("Pins", s);
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Body");
        elem.setAttribute("SMU", QString("%1").arg(it.body.Num));
        elem.setAttribute("Slot", QString("%1").arg(it.body.Slot));
        cnt = 0;
        s = "";
        for(auto p: it.body.pins)
            if(p > 0){
                if(cnt == 0)
                    s = s + QString::number(p);
                else
                    s = s + "," + QString::number(p);
                cnt++;
            }
        elem.setAttribute("Pins", s);
        ThirdElement.appendChild(elem);

        elem = doc.createElement("ChannType");
        elem.setAttribute("value", QString("%1").arg(it.TypeCh));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Width");
        elem.setAttribute("value", QString("%1").arg(it.W));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Length");
        elem.setAttribute("value", QString("%1").arg(it.L));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Vth_lin");
        elem.setAttribute("value", QString("%1").arg(it.Vdlin));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Vth_sat");
        elem.setAttribute("value", QString("%1").arg(it.Vdsat));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Idrain_max");
        elem.setAttribute("value", QString("%1").arg(it.Idmax));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Vgate_start");
        elem.setAttribute("value", QString("%1").arg(it.Vgstart));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Vgate_stop");
        elem.setAttribute("value", QString("%1").arg(it.Vgstop));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Ithreshold");
        elem.setAttribute("value", QString("%1").arg(it.Ith_sq));
        ThirdElement.appendChild(elem);

        SecondElement.appendChild(ThirdElement);
    }//end for(it)

    if(file.open(QIODevice::WriteOnly)){
        QTextStream(&file) << doc.toString();
        file.close();
    }
}
/*------------------------------------------------------------------------------------------------*/
//считывание исходных данных по испытаниям на горячие носители
void ReadHCXml(const QString &FileName, HC* const hc)
{
    QDomDocument doc;
    QFile file;
    Transistor t;
    QDomElement MainElement, tmp1, tmp2, tmp3;
    QDomNode MainNode, node, sec_node;
    int number;

    file.setFileName(FileName);
    if(!file.open(QIODevice::ReadOnly))
        return;

    if(doc.setContent(&file))
        file.close();
    else{
        QMessageBox::critical(0, "Error", QString("Bad XML-file: SetContent."), QMessageBox::Ok);
        return;
    }

    MainElement = doc.documentElement();

    if("TESTS" != MainElement.tagName()){
        QMessageBox::critical(0, "Error", QString("Bad XML-file: Read."), QMessageBox::Ok);
        return;
    }

    MainNode = MainElement.firstChild();
    while(!MainNode.isNull()){
        if(MainNode.isElement() && "HC" == MainNode.toElement().tagName()){
            tmp1 = MainNode.toElement();
            if(!tmp1.isNull()){
                node = tmp1.firstChild();
                while(!node.isNull()){
                    if(node.isElement()){
                        tmp2 = node.toElement();
                        if(!tmp2.isNull()){
                            if(tmp2.tagName() == "Num"){
                                number = (tmp2. attribute("value", "")).toInt();
                                hc->Number = number;
                                //qDebug() << tmp2.tagName() << number;
                            }
                            else if(tmp2.tagName() == "Temperature"){
                                hc->T = (tmp2. attribute("value", "")).toDouble();
                            }
                            else if(tmp2.tagName() == "TimeLimit"){
                                hc->TimeLimit = (tmp2. attribute("value", "")).toDouble();
                            }
                            else if(tmp2.tagName() == "Vgstress"){
                                hc->Vgstress = fabs((tmp2. attribute("value", "")).toDouble());
                            }
                            else if(tmp2.tagName() == "Vdstress"){
                                hc->Vdstress = fabs((tmp2. attribute("value", "")).toDouble());
                            }
                            else if(tmp2.tagName() == "Vsstress"){
                                hc->Vsstress = fabs((tmp2. attribute("value", "")).toDouble());
                            }
                            else if(tmp2.tagName() == "Vbstress"){
                                hc->Vbstress = fabs((tmp2. attribute("value", "")).toDouble());
                            }
                            else
                                if(tmp2.tagName() == "Device"){
                                    sec_node = tmp2.firstChild();
                                    t.clear();
                                    while(!sec_node.isNull()){
                                        if(sec_node.isElement()){
                                            tmp3 = sec_node.toElement();
                                            if(!tmp3.isNull()){
                                                if(tmp3.tagName() == "Index"){
                                                    t.Index = (tmp3. attribute("value", "")).toInt();
                                                }
                                                else if(tmp3.tagName() == "Drain"){
                                                    t.drain.Num = (tmp3. attribute("SMU", "")).toInt();
                                                    t.drain.Slot = (tmp3. attribute("Slot", "")).toInt();
                                                    SetPinsXML(tmp3. attribute("Pins", ""), t.drain.pins);
                                                }
                                                else if(tmp3.tagName() == "Gate"){
                                                    t.gate.Num = (tmp3. attribute("SMU", "")).toInt();
                                                    t.gate.Slot = (tmp3. attribute("Slot", "")).toInt();
                                                    SetPinsXML(tmp3. attribute("Pins", ""), t.gate.pins);
                                                }
                                                else if(tmp3.tagName() == "Source"){
                                                    t.source.Num = (tmp3. attribute("SMU", "")).toInt();
                                                    t.source.Slot = (tmp3. attribute("Slot", "")).toInt();
                                                    SetPinsXML(tmp3. attribute("Pins", ""), t.source.pins);
                                                }
                                                else if(tmp3.tagName() == "Body"){
                                                    t.body.Num = (tmp3. attribute("SMU", "")).toInt();
                                                    t.body.Slot = (tmp3. attribute("Slot", "")).toInt();
                                                    SetPinsXML(tmp3. attribute("Pins", ""), t.body.pins);
                                                }
                                                else if(tmp3.tagName() == "ChannType"){
                                                    t.TypeCh = (tmp3. attribute("value", "")).toInt();
                                                }
                                                else if(tmp3.tagName() == "Width"){
                                                    t.W = (tmp3. attribute("value", "")).toDouble();
                                                }
                                                else if(tmp3.tagName() == "Length"){
                                                    t.L = (tmp3. attribute("value", "")).toDouble();
                                                }
                                                else if(tmp3.tagName() == "Vth_lin"){
                                                    t.Vdlin = fabs((tmp3. attribute("value", "")).toDouble());
                                                }
                                                else if(tmp3.tagName() == "Vth_sat"){
                                                    t.Vdsat = fabs((tmp3. attribute("value", "")).toDouble());
                                                }
                                                else if(tmp3.tagName() == "Idrain_max"){
                                                    t.Idmax = fabs((tmp3. attribute("value", "")).toDouble());
                                                }
                                                else if(tmp3.tagName() == "Vgate_start"){
                                                    t.Vgstart = fabs((tmp3. attribute("value", "")).toDouble());
                                                }
                                                else if(tmp3.tagName() == "Vgate_stop"){
                                                    t.Vgstop = fabs((tmp3. attribute("value", "")).toDouble());
                                                }
                                                else if(tmp3.tagName() == "Ithreshold"){
                                                    t.Ith_sq = fabs((tmp3. attribute("value", "")).toDouble());
                                                }
                                            }//end if(!tmp3.isNull())
                                        }
                                        sec_node = sec_node.nextSibling();
                                    }//end while(sec_node.isNull())
                                    hc->MOS.push_back(t);
                                    hc->TypeCh = t.TypeCh;
                                }//end if(tmp2.tagName()=="Device")
                        }//end if(!tmp2.isNull())
                    }//end if(node.isElement())
                    node = node.nextSibling();
                }//end while(!node.isNull())
            }//end if(!tmp1.isNull())
        }
        MainNode = MainNode.nextSibling();
    }//end while(!MainNode.isNull())
}
/*------------------------------------------------------------------------------------------------*/
//сохранение исходных данных по испытаниям диэлектрика
/*void SaveTDDBXml(const QString &FileName, QVector<TDDB>* const dev, const int &Number)
{
    QString s;
    QFile file;
    int cnt;
    QDomElement  elem;
    QDomDocument doc;
    QDomElement  MainElement,
                 SecondElement,
                 ThirdElement;
    QByteArray buff;
    QVector<TDDB>::iterator it;
    QVector<unsigned>::iterator p;

    if(dev->size()<1)
        return;

    file.setFileName(FileName);

    if(file.exists()){
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(0, "Error", QString("Bad XML-file: Open."), QMessageBox::Ok);
            return;
        }

        buff = file.readAll();
        file.close();

        if(false == doc.setContent(buff)){
            QMessageBox::critical(0, "Error", QString("Bad XML-file: setContent."), QMessageBox::Ok);
            return;
        }

        MainElement = doc.documentElement();
    }
    else{
        QDomNode xmlNode = doc.createProcessingInstruction("xml",
                                         "version=\"1.0\" encoding=\"UTF-8\"");
        doc.insertBefore(xmlNode, doc.firstChild());

        MainElement = doc.createElement("TESTS");
        doc.appendChild(MainElement);
    }

    SecondElement = doc.createElement("TDDB");
    MainElement.appendChild(SecondElement);
    ThirdElement = doc.createElement("Device");

    elem = doc.createElement("Num");
    elem.setAttribute("value", QString("%1").arg(Number));
    SecondElement.appendChild(elem);

    for(it=dev->begin(); it<dev->end(); it++){
        ThirdElement = doc.createElement("Device");
        elem = doc.createElement("Index");
        elem.setAttribute("value", QString("%1").arg(it->Index));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("High");
        elem.setAttribute("SMU", QString("%1").arg(it->NumSMU));
        elem.setAttribute("Slot", QString("%1").arg(it->NumSlot));
        elem.setAttribute("Pins", QString("%1").arg(it->HighPin));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Low");
        elem.setAttribute("SMU", QString("%1").arg(GNDU));
        elem.setAttribute("Slot", QString("%1").arg(SLOT_GNDU));
        cnt = 0;
        s = "";
        for(p=it->LowPin.begin(); p<it->LowPin.end(); p++)
            if((*p) > 0){
                if(cnt == 0)
                    s = s + QString::number(*p);
                else
                    s = s + "," + QString::number(*p);
                cnt++;
            }
        elem.setAttribute("Pins", s);
        ThirdElement.appendChild(elem);

        elem = doc.createElement("ChannType");
        elem.setAttribute("value", QString("%1").arg(it->TypeCh));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Width");
        elem.setAttribute("value", QString("%1").arg(it->W));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Length");
        elem.setAttribute("value", QString("%1").arg(it->L));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Temperature");
        elem.setAttribute("value", QString("%1").arg(it->T));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("TimeLimit");
        elem.setAttribute("value", QString("%1").arg(it->StressTime));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Iuse");
        elem.setAttribute("value", QString("%1").arg(it->Iuse_init));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("tox");
        elem.setAttribute("value", QString("%1").arg(it->tox));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Vsilc");
        elem.setAttribute("value", QString("%1").arg(it->Vsilc));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Vstress");
        elem.setAttribute("value", QString("%1").arg(it->Vstress));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Vuse");
        elem.setAttribute("value", QString("%1").arg(it->Vuse));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Istress");
        elem.setAttribute("value", QString("%1").arg(it->Istress_init));
        ThirdElement.appendChild(elem);


        SecondElement.appendChild(ThirdElement);
    }//end for(it)

    if(file.open(QIODevice::WriteOnly)){
        QTextStream(&file) << doc.toString();
        file.close();
    }
}*/
/*------------------------------------------------------------------------------------------------*/
//считывание исходных данных по испытаниям диэлектрика
/*void ReadTDDBXml(const QString &FileName, QVector<TDDB>* const dev)
{
    QDomDocument doc;
    QFile file;
    TDDB tddb;
    QDomElement MainElement, tmp1, tmp2, tmp3;
    QDomNode MainNode, node, sec_node;
    int number;
    ViInt32 pins[NUM_CARD_PINS] = {0};

    file.setFileName(FileName);
    if(!file.open(QIODevice::ReadOnly))
        return;

    if(doc.setContent(&file))
        file.close();
    else{
        QMessageBox::critical(0, "Error", QString("Bad XML-file: SetContent."), QMessageBox::Ok);
        return;
    }

    MainElement = doc.documentElement();

    if("TESTS" != MainElement.tagName()){
        QMessageBox::critical(0, "Error", QString("Bad XML-file: Read."), QMessageBox::Ok);
        return;
    }

    MainNode = MainElement.firstChild();
    while(!MainNode.isNull()){
        if(MainNode.isElement() && "TDDB" == MainNode.toElement().tagName()){
            tmp1 = MainNode.toElement();
            if(!tmp1.isNull()){
                node = tmp1.firstChild();
                while(!node.isNull()){
                    if(node.isElement()){
                        tmp2 = node.toElement();
                        if(!tmp2.isNull()){
                            if(tmp2.tagName() == "Num"){
                                number = (tmp2. attribute("value", "")).toInt();
                                //qDebug() << tmp2.tagName() << number;
                            }
                            else
                                if(tmp2.tagName() == "Device"){
                                    //qDebug() << tmp2.tagName();
                                    sec_node = tmp2.firstChild();
                                    tddb.Clear();
                                    tddb.Number = number;
                                    while(!sec_node.isNull()){
                                        if(sec_node.isElement()){
                                            tmp3 = sec_node.toElement();
                                            if(!tmp3.isNull()){
                                                if(tmp3.tagName() == "Index"){
                                                    tddb.Index = (tmp3. attribute("value", "")).toInt();
                                                    //qDebug() << tmp3.tagName() << tddb.Index;
                                                }
                                                else if(tmp3.tagName() == "High"){
                                                    tddb.NumSMU = (tmp3. attribute("SMU", "")).toInt();
                                                    //qDebug() << "SMU" << tddb.NumSMU;
                                                    tddb.NumSlot = (tmp3. attribute("Slot", "")).toInt();
                                                    //qDebug() << "Slot" << tddb.NumSlot;
                                                    tddb.HighPin = (tmp3. attribute("Pins", "")).toInt();
                                                    //qDebug() << "Pins" << tddb.HighPin;
                                                }
                                                else if(tmp3.tagName() == "Low"){
                                                    //qDebug() << "SMU" << (tmp3. attribute("SMU", "")).toInt();
                                                    //qDebug() << "Slot" << (tmp3. attribute("Slot", "")).toInt();
                                                    //qDebug() << "Pins" << tmp3. attribute("Pins", "");
                                                    //!!!SetPinsXML(tmp3. attribute("Pins", ""), pins);
                                                    for(int i=0; i<NUM_CARD_PINS; i++)
                                                        if(pins[i]!=0)
                                                            tddb.LowPin.push_back(pins[i]);
                                                }
                                                else if(tmp3.tagName() == "ChannType"){
                                                    tddb.TypeCh = (tmp3. attribute("value", "")).toInt();
                                                    //qDebug() << tmp3.tagName() << tddb.TypeCh;
                                                }
                                                else if(tmp3.tagName() == "Width"){
                                                    tddb.W = (tmp3. attribute("value", "")).toDouble();
                                                    //qDebug() << tmp3.tagName() << tddb.W;
                                                }
                                                else if(tmp3.tagName() == "Length"){
                                                    tddb.L = (tmp3. attribute("value", "")).toDouble();
                                                    //qDebug() << tmp3.tagName() << tddb.L;
                                                }
                                                else if(tmp3.tagName() == "Temperature"){
                                                    tddb.T = (tmp3. attribute("value", "")).toDouble();
                                                    //qDebug() << tmp3.tagName() << tddb.T;
                                                }
                                                else if(tmp3.tagName() == "TimeLimit"){
                                                    tddb.StressTime = (tmp3. attribute("value", "")).toDouble();
                                                    //qDebug() << tmp3.tagName() << tddb.StressTime;
                                                }
                                                else if(tmp3.tagName() == "Iuse"){
                                                    tddb.Iuse_init = fabs((tmp3. attribute("value", "")).toDouble())*1E+6;
                                                    //qDebug() << tmp3.tagName() << tddb.Iuse_init;
                                                }
                                                else if(tmp3.tagName() == "tox"){
                                                    tddb.tox = (tmp3. attribute("value", "")).toDouble();
                                                    //qDebug() << tmp3.tagName() << tddb.tox;
                                                }
                                                else if(tmp3.tagName() == "Vsilc"){
                                                    tddb.Vsilc = fabs((tmp3. attribute("value", "")).toDouble());
                                                    //qDebug() << tmp3.tagName() << tddb.Vsilc;
                                                }
                                                else if(tmp3.tagName() == "Vstress"){
                                                    tddb.Vstress = fabs((tmp3. attribute("value", "")).toDouble());
                                                    //qDebug() << tmp3.tagName() << tddb.Vstress;
                                                }
                                                else if(tmp3.tagName() == "Vuse"){
                                                    tddb.Vuse = fabs((tmp3. attribute("value", "")).toDouble());
                                                    //qDebug() << tmp3.tagName() << tddb.Vuse;
                                                }
                                                else if(tmp3.tagName() == "Istress"){
                                                    tddb.Istress_init = fabs((tmp3. attribute("value", "")).toDouble())*1E+3;
                                                    //qDebug() << tmp3.tagName() << tddb.Istress_init;
                                                }
                                            }//end if(!tmp3.isNull())
                                        }
                                        sec_node = sec_node.nextSibling();
                                    }//end while(sec_node.isNull())
                                    dev->push_back(tddb);
                                }//end if(tmp2.tagName()=="Device")
                        }//end if(!tmp2.isNull())
                    }//end if(node.isElement())
                    node = node.nextSibling();
                }//end while(!node.isNull())
            }//end if(!tmp1.isNull())
        }
        MainNode = MainNode.nextSibling();
    }//end while(!MainNode.isNull())
}*/
/*------------------------------------------------------------------------------------------------*/
//сохранение исходных данных по испытаниям на смещение при нагреве
/*void SaveNBTIXml(const QString &FileName, QVector<NBTI>* const dev, const int &Number)
{
    QString s;
    QFile file;
    int cnt;
    QDomElement  elem;
    QDomDocument doc;
    QDomElement  MainElement,
                 SecondElement,
                 ThirdElement;
    QByteArray buff;
    QVector<NBTI>::iterator it;
    QVector<unsigned>::iterator p;

    if(dev->size()<1)
        return;

    file.setFileName(FileName);

    if(file.exists()){
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(0, "Error", QString("Bad XML-file: Open."), QMessageBox::Ok);
            return;
        }

        buff = file.readAll();
        file.close();

        if(false == doc.setContent(buff)){
            QMessageBox::critical(0, "Error", QString("Bad XML-file: setContent."), QMessageBox::Ok);
            return;
        }

        MainElement = doc.documentElement();
    }
    else{
        QDomNode xmlNode = doc.createProcessingInstruction("xml",
                                         "version=\"1.0\" encoding=\"UTF-8\"");
        doc.insertBefore(xmlNode, doc.firstChild());

        MainElement = doc.createElement("TESTS");
        doc.appendChild(MainElement);
    }

    SecondElement = doc.createElement("NBTI");
    MainElement.appendChild(SecondElement);
    ThirdElement = doc.createElement("Device");

    elem = doc.createElement("Num");
    elem.setAttribute("value", QString("%1").arg(Number));
    SecondElement.appendChild(elem);

    for(it=dev->begin(); it<dev->end(); it++){
        ThirdElement = doc.createElement("Device");
        elem = doc.createElement("Index");
        elem.setAttribute("value", QString("%1").arg(it->Index));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Gate");
        elem.setAttribute("SMU", QString("%1").arg(it->gate_smu));
        elem.setAttribute("Slot", QString("%1").arg(it->gate));
        cnt = 0;
        s = "";
        for(p=it->gate_pins.begin(); p<it->gate_pins.end(); p++)
            if((*p) > 0){
                if(cnt == 0)
                    s = s + QString::number(*p);
                else
                    s = s + "," + QString::number(*p);
                cnt++;
            }
        elem.setAttribute("Pins", s);
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Drain");
        elem.setAttribute("SMU", QString("%1").arg(it->drain_smu));
        elem.setAttribute("Slot", QString("%1").arg(it->drain));
        cnt = 0;
        s = "";
        for(p=it->drain_pins.begin(); p<it->drain_pins.end(); p++)
            if((*p) > 0){
                if(cnt == 0)
                    s = s + QString::number(*p);
                else
                    s = s + "," + QString::number(*p);
                cnt++;
            }
        elem.setAttribute("Pins", s);
        ThirdElement.appendChild(elem);

        elem = doc.createElement("SourceBody");
        elem.setAttribute("SMU", QString("%1").arg(GNDU));
        elem.setAttribute("Slot", QString("%1").arg(SLOT_GNDU));
        cnt = 0;
        s = "";
        for(p=it->sb_pins.begin(); p<it->sb_pins.end(); p++)
            if((*p) > 0){
                if(cnt == 0)
                    s = s + QString::number(*p);
                else
                    s = s + "," + QString::number(*p);
                cnt++;
            }
        elem.setAttribute("Pins", s);
        ThirdElement.appendChild(elem);

        elem = doc.createElement("ChannType");
        elem.setAttribute("value", QString("%1").arg(it->TypeCh));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Width");
        elem.setAttribute("value", QString("%1").arg(it->W));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Length");
        elem.setAttribute("value", QString("%1").arg(it->L));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Temperature");
        elem.setAttribute("value", QString("%1").arg(it->T));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Vth_lin");
        elem.setAttribute("value", QString("%1").arg(it->Vtlin));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Vth_sat");
        elem.setAttribute("value", QString("%1").arg(it->Vtsat));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("StressTime");
        elem.setAttribute("value", QString("%1").arg(it->StressTime));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("ClassicTest");
        if(it->ClassicTest == 1)
            elem.setAttribute("value", QString("%1").arg(1));
        else
            elem.setAttribute("value", QString("%1").arg(0));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Vgstress");
        elem.setAttribute("value", QString("%1").arg(it->Vgstress));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Vdstress");
        elem.setAttribute("value", QString("%1").arg(it->Vdstress));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Idrain_max");
        elem.setAttribute("value", QString("%1").arg(it->Idmax));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Vgate_start");
        elem.setAttribute("value", QString("%1").arg(it->Vgstart));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Vgate_stop");
        elem.setAttribute("value", QString("%1").arg(it->Vgstop));
        ThirdElement.appendChild(elem);

        elem = doc.createElement("Ithreshold");
        elem.setAttribute("value", QString("%1").arg(it->Ith_sq));
        ThirdElement.appendChild(elem);

        SecondElement.appendChild(ThirdElement);
    }//end for(it)

    if(file.open(QIODevice::WriteOnly)){
        QTextStream(&file) << doc.toString();
        file.close();
    }
}*/
/*------------------------------------------------------------------------------------------------*/
//считывание исходных данных по испытаниям на смещение при нагреве
/*void ReadNBTIXml(const QString &FileName, QVector<NBTI>* const dev)
{
    QDomDocument doc;
    QFile file;
    NBTI nbti;
    QDomElement MainElement, tmp1, tmp2, tmp3;
    QDomNode MainNode, node, sec_node;
    int number;
    ViInt32 pins[NUM_CARD_PINS] = {0};

    file.setFileName(FileName);
    if(!file.open(QIODevice::ReadOnly))
        return;

    if(doc.setContent(&file))
        file.close();
    else{
        QMessageBox::critical(0, "Error", QString("Bad XML-file: SetContent."), QMessageBox::Ok);
        return;
    }

    MainElement = doc.documentElement();

    if("TESTS" != MainElement.tagName()){
        QMessageBox::critical(0, "Error", QString("Bad XML-file: Read."), QMessageBox::Ok);
        return;
    }

    MainNode = MainElement.firstChild();
    while(!MainNode.isNull()){
        if(MainNode.isElement() && "NBTI" == MainNode.toElement().tagName()){
            tmp1 = MainNode.toElement();
            if(!tmp1.isNull()){
                node = tmp1.firstChild();
                while(!node.isNull()){
                    if(node.isElement()){
                        tmp2 = node.toElement();
                        if(!tmp2.isNull()){
                            if(tmp2.tagName() == "Num"){
                                number = (tmp2. attribute("value", "")).toInt();
                                //qDebug() << tmp2.tagName() << number;
                            }
                            else
                                if(tmp2.tagName() == "Device"){
                                    sec_node = tmp2.firstChild();
                                    nbti.Clear();
                                    nbti.Number = number;
                                    while(!sec_node.isNull()){
                                        if(sec_node.isElement()){
                                            tmp3 = sec_node.toElement();
                                            if(!tmp3.isNull()){
                                                if(tmp3.tagName() == "Index"){
                                                    nbti.Index = (tmp3. attribute("value", "")).toInt();
                                                    //qDebug() << tmp3.tagName() << nbti.Index;
                                                }
                                                else if(tmp3.tagName() == "Drain"){
                                                    nbti.drain_smu = (tmp3. attribute("SMU", "")).toInt();
                                                    //qDebug() << "SMU" << nbti.drain_smu;
                                                    nbti.drain = (tmp3. attribute("Slot", "")).toInt();
                                                    //qDebug() << "Slot" << nbti.drain;
                                                    //qDebug() << "Pins" << tmp3. attribute("Pins", "");
                                                    //!!!SetPinsXML(tmp3. attribute("Pins", ""), pins);
                                                    for(int i=0; i<NUM_CARD_PINS; i++)
                                                        if(pins[i]!=0)
                                                            nbti.drain_pins.push_back(pins[i]);
                                                }
                                                else if(tmp3.tagName() == "Gate"){
                                                    nbti.gate_smu = (tmp3. attribute("SMU", "")).toInt();
                                                    //qDebug() << "SMU" << nbti.gate_smu;
                                                    nbti.gate = (tmp3. attribute("Slot", "")).toInt();
                                                    //qDebug() << "Slot" << nbti.gate;
                                                    //qDebug() << "Pins" << tmp3. attribute("Pins", "");
                                                    //!!!SetPinsXML(tmp3. attribute("Pins", ""), pins);
                                                    for(int i=0; i<NUM_CARD_PINS; i++)
                                                        if(pins[i]!=0)
                                                            nbti.gate_pins.push_back(pins[i]);
                                                }
                                                else if(tmp3.tagName() == "SourceBody"){
                                                    //qDebug() << "SMU" << (tmp3. attribute("SMU", "")).toInt();
                                                    //qDebug() << "Slot" << (tmp3. attribute("Slot", "")).toInt();
                                                    //qDebug() << "Pins" << tmp3. attribute("Pins", "");
                                                    //!!!SetPinsXML(tmp3. attribute("Pins", ""), pins);
                                                    for(int i=0; i<NUM_CARD_PINS; i++)
                                                        if(pins[i]!=0)
                                                            nbti.sb_pins.push_back(pins[i]);
                                                }
                                                else if(tmp3.tagName() == "ChannType"){
                                                    nbti.TypeCh = (tmp3. attribute("value", "")).toInt();
                                                    //qDebug() << tmp3.tagName() << nbti.TypeCh;
                                                }
                                                else if(tmp3.tagName() == "Width"){
                                                    nbti.W = (tmp3. attribute("value", "")).toDouble();
                                                    //qDebug() << tmp3.tagName() << nbti.W;
                                                }
                                                else if(tmp3.tagName() == "Length"){
                                                    nbti.L = (tmp3. attribute("value", "")).toDouble();
                                                    //qDebug() << tmp3.tagName() << nbti.L;
                                                }
                                                else if(tmp3.tagName() == "Temperature"){
                                                    nbti.T = (tmp3. attribute("value", "")).toDouble();
                                                    //qDebug() << tmp3.tagName() << nbti.T;
                                                }
                                                else if(tmp3.tagName() == "Vth_lin"){
                                                    nbti.Vtlin = fabs((tmp3. attribute("value", "")).toDouble());
                                                    //qDebug() << tmp3.tagName() << nbti.Vtlin;
                                                }
                                                else if(tmp3.tagName() == "Vth_sat"){
                                                    nbti.Vtsat = fabs((tmp3. attribute("value", "")).toDouble());
                                                    //qDebug() << tmp3.tagName() << nbti.Vtsat;
                                                }
                                                else if(tmp3.tagName() == "StressTime"){
                                                    nbti.StressTime = (tmp3. attribute("value", "")).toDouble();
                                                    //qDebug() << tmp3.tagName() << nbti.StressTime;
                                                }
                                                else if(tmp3.tagName() == "Vgstress"){
                                                    nbti.Vgstress = fabs((tmp3. attribute("value", "")).toDouble());
                                                    //qDebug() << tmp3.tagName() << nbti.Vgstress;
                                                }
                                                else if(tmp3.tagName() == "Vdstress"){
                                                    nbti.Vdstress = fabs((tmp3. attribute("value", "")).toDouble());
                                                    //qDebug() << tmp3.tagName() << nbti.Vdstress;
                                                }
                                                else if(tmp3.tagName() == "Idrain_max"){
                                                    nbti.Idmax = fabs((tmp3. attribute("value", "")).toDouble());
                                                    //qDebug() << tmp3.tagName() << nbti.Idmax;
                                                }
                                                else if(tmp3.tagName() == "Vgate_start"){
                                                    nbti.Vgstart = fabs((tmp3. attribute("value", "")).toDouble());
                                                    //qDebug() << tmp3.tagName() << nbti.Vgstart;
                                                }
                                                else if(tmp3.tagName() == "Vgate_stop"){
                                                    nbti.Vgstop = fabs((tmp3. attribute("value", "")).toDouble());
                                                    //qDebug() << tmp3.tagName() << nbti.Vgstop;
                                                }
                                                else if(tmp3.tagName() == "Ithreshold"){
                                                    nbti.Ith_sq = fabs((tmp3. attribute("value", "")).toDouble());
                                                    //qDebug() << tmp3.tagName() << nbti.Ith_sq;
                                                }
                                                else if(tmp3.tagName() == "ClassicTest"){
                                                    nbti.ClassicTest = abs((tmp3. attribute("value", "")).toInt());
                                                }
                                            }//end if(!tmp3.isNull())
                                        }
                                        sec_node = sec_node.nextSibling();
                                    }//end while(sec_node.isNull())
                                    dev->push_back(nbti);
                                }//end if(tmp2.tagName()=="Device")
                        }//end if(!tmp2.isNull())
                    }//end if(node.isElement())
                    node = node.nextSibling();
                }//end while(!node.isNull())
            }//end if(!tmp1.isNull())
        }
        MainNode = MainNode.nextSibling();
    }//end while(!MainNode.isNull())
}*/
/*------------------------------------------------------------------------------------------------*/
