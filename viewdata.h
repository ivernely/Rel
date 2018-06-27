#ifndef VIEWDATA_H
#define VIEWDATA_H

#include <QDialog>

/*=========================================================================================*/
class Distrib
{
public:
    Distrib();

    void Clear();

    QString name;

    QVector<double> data,   //исходные данные
                    Z;      //функция по оси Y

    QColor          color;  //цвет на графике

    int             Number,     //порядковый номер
                    RegrNumber; //порядковый номер линии регрессии

    double          MTTF,
                    sigma,
                    R2,
                    A,      //уравнение вида Ax+B
                    B;
};
/*=========================================================================================*/

namespace Ui {
class ViewData;
}

class ViewData : public QDialog
{
    Q_OBJECT

public:
    explicit ViewData(QWidget *parent = 0);
    ~ViewData();

    void setValuesToTable(QVector<Distrib> &distrib);

private:
    Ui::ViewData *ui;
};

#endif // VIEWDATA_H
