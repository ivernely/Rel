#include "viewdata.h"
#include "ui_viewdata.h"

//---------------------------------------------------------
ViewData::ViewData(QWidget *parent) : QDialog(parent), ui(new Ui::ViewData)
{
    ui->setupUi(this);
}
//---------------------------------------------------------
ViewData::~ViewData()
{
    delete ui;
}
//---------------------------------------------------------
void ViewData::setValuesToTable(QVector<Distrib> &distrib)
{
    const int NumCells = 7;
    int N, row;
    QVector<Distrib>::iterator it;

    N = distrib.size();

    if(N<1)
        return;

    ui->twData->setRowCount(N+2);

    for(it=distrib.begin(), row=0; it<distrib.end(); it++, row++){
        QTableWidgetItem** item = new QTableWidgetItem*[NumCells];

        item[0]->setText(it->name);
        item[0]->setTextAlignment(Qt::AlignCenter);
        ui->twData->setItem(row,0,item[0]);

        item[1]->setText(QString("%1").arg(it->data.size()));
        item[1]->setTextAlignment(Qt::AlignCenter);
        ui->twData->setItem(row,1,item[1]);

        if(it->B>0)
            item[2]->setText(QString("Y=%1Ln(t)+%2").arg(it->A).arg(it->B));
        else
            item[2]->setText(QString("Y=%1Ln(t)%2").arg(it->A).arg(it->B));
        item[2]->setTextAlignment(Qt::AlignCenter);
        ui->twData->setItem(row,2,item[2]);

        item[3]->setText(QString("%1").arg(it->sigma));
        item[3]->setTextAlignment(Qt::AlignCenter);
        ui->twData->setItem(row,3,item[3]);

        item[4]->setText(QString("%1").arg(it->R2));
        item[4]->setTextAlignment(Qt::AlignCenter);
        ui->twData->setItem(row,4,item[4]);

        item[5]->setText(QString("%1").arg(it->MTTF));
        item[5]->setTextAlignment(Qt::AlignCenter);
        ui->twData->setItem(row,5,item[5]);

        item[6]->setTextAlignment(Qt::AlignCenter);
        item[6]->setBackgroundColor(it->color);
        ui->twData->setItem(row,6,item[6]);

        //for(int i=0; i < NumCells; i++)
        //        delete [] item[i];
    }
}
