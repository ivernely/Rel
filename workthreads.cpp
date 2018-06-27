#include "workthreads.h"

/*-----------------------------------------------------*/
WorkThreads::WorkThreads(QObject *parent) :
    QObject(parent)
{
    em = NULL;
}
/*-----------------------------------------------------*/
WorkThreads::~WorkThreads()
{
    if(em != NULL)
        delete em;
}
/*-----------------------------------------------------*/
void WorkThreads::process()
{
    inTesting = true;

    em = new EMTconst();
    emit finished();
    return;
}
/*-----------------------------------------------------*/
