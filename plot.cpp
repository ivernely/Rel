#include "mainwindow.h"
#include "ui_mainwindow.h"


#define Y_PLOT_MIN -16.2
#define Y_PLOT_MAX 2.5
/*-----------------------------------------------------------------------------------------------*/
Distrib::Distrib()
{
}
/*-----------------------------------------------------------------------------------------------*/
void Distrib::Clear()
{
    data.clear();
    Z.clear();

    color = QColor(255,255,255);

    name = QString("");

    Number = -1;
    RegrNumber = -1;

    MTTF = 0.0;
    sigma = 0.0;
    R2 = 0.0;
    A = 0.0;
    B = 0.0;
}
/*-----------------------------------------------------------------------------------------------*/
void SetPlotArea(QCustomPlot *widget)
{
    QFont Font;
    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);


    widget->clearGraphs();

    widget->setContextMenuPolicy(Qt::CustomContextMenu);
    widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);

    widget->setLocale(QLocale::system());
    widget->yAxis->setLabel("F, %");
    widget->xAxis->setLabel("Time, sec");
    widget->xAxis->setScaleType(QCPAxis::stLogarithmic);
    widget->xAxis->setTicker(logTicker);

    Font.setPointSize(13);
    Font.setFamily("Calibri");

    widget->yAxis->setTickLabelFont(Font);
    widget->xAxis->setTickLabelFont(Font);
    widget->yAxis->setLabelFont(Font);
    widget->xAxis->setLabelFont(Font);
    widget->yAxis->labelFont().setBold(true);
    widget->xAxis->labelFont().setBold(true);
    Font.setPointSize(10);
    widget->legend->setFont(Font);
    widget->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
    widget->legend->setBorderPen(Qt::NoPen);

    widget->xAxis->setRange(0, 10000, Qt::AlignLeft);

    widget->rescaleAxes();

    widget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
//ограничение на масштабирование по оси Х (только положительная часть)
void MainWindow::xAxisRangeChanged(const QCPRange &newRange, const QCPRange &)
{
    if(newRange.lower < 0)
        ui->plotWidget->xAxis->setRangeLower( 0 );

     if(newRange.upper > 1E+20)
        ui->plotWidget->xAxis->setRangeUpper( 1E+20 );
}
/*-----------------------------------------------------------------------------------------------*/
//ограничение на масштабирование по оси Y (от 0 до 100)
void MainWindow::yAxisRangeChanged(const QCPRange &newRange, const QCPRange &)
{
    if(newRange.lower < Y_PLOT_MIN)
        ui->plotWidget->yAxis->setRangeLower( Y_PLOT_MIN );

    if(newRange.upper > Y_PLOT_MAX)
        ui->plotWidget->yAxis->setRangeUpper( Y_PLOT_MAX );
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::mouseWheel(QWheelEvent*)
{
    if (ui->plotWidget->xAxis->selectedParts().testFlag(QCPAxis::spAxis)){
            ui->plotWidget->axisRect()->setRangeZoomAxes(ui->plotWidget->xAxis,ui->plotWidget->yAxis);
            ui->plotWidget->axisRect()->setRangeZoom(ui->plotWidget->xAxis->orientation());
          }
          else if (ui->plotWidget->yAxis->selectedParts().testFlag(QCPAxis::spAxis)){
            ui->plotWidget->axisRect()->setRangeZoomAxes(ui->plotWidget->xAxis,ui->plotWidget->yAxis);
            ui->plotWidget->axisRect()->setRangeZoom(ui->plotWidget->yAxis->orientation());
          }
          else
            ui->plotWidget->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}
/*-----------------------------------------------------------------------------------------------*/
//функция вычисления статистики
double StFnc(int i, int N)
{
    //используем медианную статистику
    return (i+0.7)/(N+0.4);
}
/*-----------------------------------------------------------------------------------------------*/
bool FindWeibullParams(Distrib *w)
{
    int i,
        N;

    double  t,
            Ft,
            a,                               //параметр для построения прямой
            b;                               //параметр для построения прямой

    double  nSumti = 0.0,                     //сумма времён отказов
            nSumti2 = 0.0,                    //сумма квадратов времён отказов
            nSumLnLn = 0.0,                   //сумма двойных логарифмов  lnln(1/(1-F))
            nSumLntiLnLn = 0.0,               //сумма произведений логарифма времени отказа и двойного логарифма
            nSumLnti2 = 0.0,                  //сумма квадратов логарифмов времён отказов
            nSumLnti = 0.0,                   //сумма логарифмов времён отказов
            nSumLnLn2 = 0.0,
            sigmaX,
            sigmaY;

    N = w->data.size();
    w->Z.clear();

    if(N < 2)
        return false;

    for(i=0; i<N; i++)
    {
        t = w->data.at(i);
        Ft = StFnc(i, N);
        w->Z.push_back(log(log(1/(1-Ft))));

        nSumti = nSumti + t;
        nSumti2 = nSumti2 + pow(t,2);
        nSumLnti = nSumLnti + log(t);
        nSumLnLn = nSumLnLn + log(log(1/(1-Ft)));
        nSumLnLn2 = nSumLnLn2 + log(log(1/(1-Ft)))*log(log(1/(1-Ft)));
        nSumLntiLnLn = nSumLntiLnLn + log(t)*log(log(1/(1-Ft)));
        nSumLnti2 = nSumLnti2 + pow(log(t), 2);
    }


    a = (N*nSumLntiLnLn - nSumLnti*nSumLnLn)/(N*nSumLnti2 - nSumLnti*nSumLnti);
    b = (nSumLnLn - a*nSumLnti)/N;
    sigmaX = nSumLnti2/N - pow(nSumLnti/N, 2);
    sigmaY = nSumLnLn2/N - pow(nSumLnLn/N, 2);

    w->A = a;
    w->B = b;
    w->MTTF = exp((-b)/a);
    w->sigma = 1/a;
    w->R2 = a*sqrt(sigmaX)/sqrt(sigmaY)*a*sqrt(sigmaX)/sqrt(sigmaY);

    return true;
}
/*-----------------------------------------------------------------------------------------------*/
bool FindLogNParams(Distrib *w)
{
    int i,
        N;

    double  t,
            k,
            Ft,
            a,                               //параметр для построения прямой
            b;                               //параметр для построения прямой

    double  nSumti = 0.0,                     //сумма времён отказов
            nSumti2 = 0.0,                    //сумма квадратов времён отказов
            nSumLnLn = 0.0,                   //сумма двойных логарифмов  lnln(1/(1-F))
            nSumLntiLnLn = 0.0,               //сумма произведений логарифма времени отказа и двойного логарифма
            nSumLnti2 = 0.0,                  //сумма квадратов логарифмов времён отказов
            nSumLnti = 0.0,                   //сумма логарифмов времён отказов
            nSumLnLn2 = 0.0,
            sigmaX,
            sigmaY;


    N = w->data.size();
    w->Z.clear();

    if(N < 2)
        return false;


    for(i=0; i<N; i++)
    {
        t = w->data.at(i);
        Ft = StFnc(i, N);
        k = inv_normalDF(Ft);
        w->Z.push_back(k);

        //w->Ft.push_back(Ft);
        //w->Z.push_back(k);

        nSumti = nSumti + t;
        nSumti2 = nSumti2 + pow(t,2);
        nSumLnti = nSumLnti + log(t);
        nSumLnLn = nSumLnLn + k;
        nSumLnLn2 = nSumLnLn2 + pow(k, 2);
        nSumLntiLnLn = nSumLntiLnLn + log(t)*k;
        nSumLnti2 = nSumLnti2 + pow(log(t), 2);
    }


    a = (N*nSumLntiLnLn - nSumLnti*nSumLnLn)/(N*nSumLnti2 - nSumLnti*nSumLnti);
    b = (nSumLnLn - a*nSumLnti)/N;
    sigmaX = nSumLnti2/N - pow(nSumLnti/N, 2);
    sigmaY = nSumLnLn2/N - pow(nSumLnLn/N, 2);

    w->A = a;
    w->B = b;
    w->MTTF = exp((-b)/a);
    w->sigma = 1/a;
    w->R2 = a*sqrt(sigmaX)/sqrt(sigmaY)*a*sqrt(sigmaX)/sqrt(sigmaY);

    return true;
}
/*-----------------------------------------------------------------------------------------------*/
bool MainWindow::ReadTextFile(QString FileName, Distrib *w)
{
    QFile file;
    QString str;
    int i;

    file.setFileName(FileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    i=0;
    while(!file.atEnd()){
        str = file.readLine();  //читаем строку

        if(i==0)
            w->name = str.simplified();  //в первой строке название массива
        else{
            str = str.simplified();//str.split("\n"); //делим строку
            if(str.length())
                w->data.push_back(str.toDouble());
        }
        i++;
    }

    qSort(w->data);

    if(ui->rbWeibull->isChecked())
        if(!FindWeibullParams(w))
                return false;

    if(ui->rbLogN->isChecked())
        if(!FindLogNParams(w))
            return false;


    switch(ColorCnt){
            case 0: w->color = QColor(Qt::red);break;
            case 1: w->color = QColor(Qt::blue);break;
            case 2: w->color = QColor(Qt::green);break;
            case 3: w->color = QColor(Qt::cyan);break;
            case 4: w->color = QColor(Qt::yellow);break;
            case 5: w->color = QColor(Qt::magenta);break;
            case 6: w->color = QColor(Qt::gray);break;
            case 7: w->color = QColor(Qt::black);break;
            case 8: w->color = QColor(Qt::darkRed);break;
            case 9: w->color = QColor(Qt::darkBlue);break;
            case 10: w->color = QColor(Qt::darkGreen);break;
            case 11: w->color = QColor(Qt::darkCyan);break;
            case 12: w->color = QColor(Qt::darkYellow);break;
            case 13: w->color = QColor(Qt::darkMagenta);break;
            case 14: w->color = QColor(Qt::darkGray);break;
            case 15: w->color = QColor(Qt::lightGray);break;
            default: w->color = QColor(Qt::black);break;
    };

    ColorCnt++;
    if(ColorCnt>=15)
        ColorCnt = 0;

    return true;
}
/*-----------------------------------------------------------------------------------------------*/
//настройка области построения для графика Вейбулла
void MainWindow::SetPlotAreaWeibull(QCustomPlot *customplot)
{
    QSharedPointer<QCPAxisTickerText> YtextTicker(new QCPAxisTickerText);
    double Ft;

    customplot->clearGraphs();

    Ft = 1e-5;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 1e-4;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 0.001;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 0.005;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 0.01;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 0.05;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 0.1;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 0.3;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 1;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 2;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 5;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 10;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 30;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 63.2;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 90;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 99;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 99.9;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 99.99;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));
    Ft = 99.999;
    YtextTicker->addTick(log(log(1/(1 - Ft/100))), QString("%1").arg(Ft));

    YtextTicker->setTickCount(10);
    YtextTicker->setSubTickCount(5);
    customplot->yAxis->setTicker(YtextTicker);

    if(ui->chbHorGrid->isChecked())
        customplot->yAxis->grid()->setVisible(true);
    else
        customplot->yAxis->grid()->setVisible(false);

    if(ui->chbHorGridAdd->isChecked())
        customplot->yAxis->grid()->setSubGridVisible(true);
    else
        customplot->yAxis->grid()->setSubGridVisible(false);

    if(ui->chbVertGrid->isChecked())
        customplot->xAxis->grid()->setVisible(true);
    else
        customplot->xAxis->grid()->setVisible(false);

    if(ui->chbVertGridAdd->isChecked())
        customplot->xAxis->grid()->setSubGridVisible(true);
    else
        customplot->xAxis->grid()->setSubGridVisible(false);

    customplot->yAxis->setRange(-1.93, 6.91, Qt::AlignCenter);
    customplot->replot();
}
/*-----------------------------------------------------------------------------------------------*/
//настройка области построения для логнормального распределения
void MainWindow::SetPlotAreaLogN(QCustomPlot *customplot)
{
    QSharedPointer<QCPAxisTickerText> YtextTicker(new QCPAxisTickerText);

    double Ft;
    customplot->clearGraphs();

    /*вставить значения на оси Y*/
    //inv_normalDF(Ft);
    customplot->yAxis->setRange(inv_normalDF(0.001), inv_normalDF(0.999));
    Ft = 1e-5;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));
    Ft = 1e-4;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));
    Ft = 0.001;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));
    Ft = 0.01;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));
    Ft = 0.1;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));
    Ft = 0.3;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));
    Ft = 1;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));
    Ft = 3;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));
    Ft = 5;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));
    Ft = 10;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));
    Ft = 30;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));
    Ft = 50;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));
    Ft = 90;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));
    Ft = 99;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));
    Ft = 99.9;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));
    Ft = 99.99;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));
    Ft = 99.999;
    YtextTicker->addTick(inv_normalDF(Ft/100), QString("%1").arg(Ft));

    YtextTicker->setTickCount(10);
    YtextTicker->setSubTickCount(5);
    customplot->yAxis->setTicker(YtextTicker);

    if(ui->chbHorGrid->isChecked())
        customplot->yAxis->grid()->setVisible(true);
    else
        customplot->yAxis->grid()->setVisible(false);

    if(ui->chbHorGridAdd->isChecked())
        customplot->yAxis->grid()->setSubGridVisible(true);
    else
        customplot->yAxis->grid()->setSubGridVisible(false);

    if(ui->chbVertGrid->isChecked())
        customplot->xAxis->grid()->setVisible(true);
    else
        customplot->xAxis->grid()->setVisible(false);

    if(ui->chbVertGridAdd->isChecked())
        customplot->xAxis->grid()->setSubGridVisible(true);
    else
        customplot->xAxis->grid()->setSubGridVisible(false);
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::DistribPlot(QCustomPlot *customplot, Distrib *w)
{
    int N = w->data.size();
    int i, Num;

    double min, max;

    QVector<QCPGraphData> dataLinear(N);
    QVector<QCPGraphData> dataRegr(2);

    //Вычисляем наши данные
    for (i=0; i<N; i++){//Пробегаем по всем точкам
        dataLinear[i].key = w->data.at(i);
        dataLinear[i].value = w->Z.at(i);
    }

    Num = customplot->graphCount();
    w->Number = Num;

    //Добавляем один график в widget
    customplot->addGraph();

    customplot->graph(Num)->setName(w->name);
    customplot->graph(Num)->setPen(QPen(w->color));
    customplot->graph(Num)->rescaleAxes();

    //формируем вид точек
    customplot->graph(Num)->setLineStyle(QCPGraph::lsNone);
    customplot->graph(Num)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,
                                                            w->color,
                                                            w->color,
                                                            7));

    //Отрисовать график
    customplot->graph(Num)->data()->set(dataLinear);

    //оптимизация области построения
    //по Y
    min = Min(w->Z);
    min<0 ? min=min*SCALE_EXT : min=min/SCALE_EXT;
    max = Max(w->Z);
    max<0 ? max=max/SCALE_EXT : max=max*SCALE_EXT;
    customplot->yAxis->setRange(min, max);
    //по X
    min = Min(w->data);
    min<0 ? min=min*SCALE_EXT : min=min/SCALE_EXT;
    max = Max(w->data);
    max<0 ? max=max/SCALE_EXT : max=max*SCALE_EXT;
    customplot->xAxis->setRange(min, max);


    //добавление линии регрессии
    Num = customplot->graphCount();
    w->RegrNumber = Num;
    customplot->addGraph();

    if(!ui->chbRegessionLine->isChecked()){
        customplot->graph(Num)->setVisible(false);
        customplot->graph(Num)->removeFromLegend();
    }


    dataRegr[0].key = w->data.at(0)/1.2;
    dataRegr[0].value = w->A*log(dataRegr[0].key) + w->B;

    dataRegr[1].key = w->data.at(N-1)*1.2;
    dataRegr[1].value = w->A*log(dataRegr[1].key) + w->B;

    customplot->graph(Num)->setName(QString("%1_regress").arg(w->name));
    customplot->graph(Num)->setPen(QPen(w->color));
    customplot->graph(Num)->data()->set(dataRegr);

    //И перерисуем график на нашем widget
    customplot->replot();
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_sbSubTicksCnt_valueChanged(int arg1)
{
    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    logTicker->setSubTickCount(arg1);
    ui->plotWidget->xAxis->setTicker(logTicker);
    ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
void MainWindow::on_leXAxeLabel_returnPressed()
{
     ui->plotWidget->xAxis->setLabel(ui->leXAxeLabel->text());
     ui->plotWidget->replot();
}
/*-----------------------------------------------------------------------------------------------*/
