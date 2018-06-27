#ifndef XML_H
#define XML_H

#include "measclasses.h"
#include "eq_control.h"
#include "QFile"

void SaveEMXml(const QString &FileName, const EM* const EMTest);
void ReadEMXml(const QString &FileName, EM* const EMTest);

void SaveHCXml(const QString &FileName, const HC* const hc);
void ReadHCXml(const QString &FileName, HC* const hc);

/*void SaveVRDBXml(const QString &FileName, const VRDB* const t, const int &Number);
void ReadVRDBXml(const QString &FileName, VRDB* const t);

void SaveTDDBXml(const QString &FileName, QVector<TDDB>* const dev, const int &Number);
void ReadTDDBXml(const QString &FileName, QVector<TDDB>* const dev);

void SaveNBTIXml(const QString &FileName, QVector<NBTI>* const dev, const int &Number);
void ReadNBTIXml(const QString &FileName, QVector<NBTI>* const dev);*/

#endif // XML_H
