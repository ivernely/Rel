#ifndef WORKTHREADS_H
#define WORKTHREADS_H

#include <QObject>
#include "measclasses.h"

class WorkThreads : public QObject
{
    Q_OBJECT
private:
    bool inTesting;
public:
    explicit WorkThreads(QObject *parent = 0);
    ~WorkThreads();

    EMTconst *em;

signals:
    void finished();

public slots:
    void process();
};


#endif // WORKTHREADS_H
