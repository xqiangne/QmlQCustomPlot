#include "TimeDomainPlot.h"
#include <QQuickItem>

const int TimeDomainPlot::DATA_SIZE = 512;
const double TimeDomainPlot::XAXIS_RANGE_START = 0;
const double TimeDomainPlot::XAXIS_RANGE_END = 400;

TimeDomainPlot::TimeDomainPlot(QQuickItem *parent): QQuickPaintedItem(parent)
{
    _customPlot = new QCustomPlot();
    _customPlot->setBackground(QBrush(QColor("#cde8ff")));
    _voltageGraph = _customPlot->addGraph();
    _currentGraph = _customPlot->addGraph(_customPlot->xAxis, _customPlot->yAxis2);
    //设置颜色
    _voltageGraph->setPen(QPen(Qt::red));
    _currentGraph->setPen(QPen(Qt::blue));
    //设置Y轴范围
    _customPlot->yAxis->setNumberFormat("gbc");/* g 灵活的格式，b 漂亮的指数形式，c 乘号改成 x */
    _customPlot->yAxis->setNumberPrecision(1);/* 精度 1 */
    _customPlot->xAxis->setRange(XAXIS_RANGE_START, XAXIS_RANGE_END);
    _customPlot->yAxis2->setNumberFormat("gbc");/* g 灵活的格式，b 漂亮的指数形式，c 乘号改成 x */
    _customPlot->yAxis2->setNumberPrecision(1);/* 精度 1 */
    _customPlot->yAxis2->setVisible(true);
    //x轴名字
    _customPlot->xAxis->setLabel("时间 / ms");
    //Y轴名字
    _customPlot->yAxis->setLabel("电压 / V");
    _customPlot->yAxis->setTickLabelColor("red");
    _customPlot->yAxis->setLabelColor("red");
    _customPlot->yAxis2->setLabel("电流 / mA");
    _customPlot->yAxis2->setTickLabelColor("blue");
    _customPlot->yAxis2->setLabelColor("blue");
}

TimeDomainPlot::~TimeDomainPlot()
{
    delete _customPlot;
}

void TimeDomainPlot::paint(QPainter *painter)
{
    _customPlot->setGeometry(0,0,this->width(),this->height());
    painter->drawPixmap(0,0,this->width(),this->height(), _customPlot->toPixmap());
}

void TimeDomainPlot::setVoltageGraphData(const QVector<double> &keys, const QVector<double> &values)
{
    _voltageGraph->setData(keys, values);
}

void TimeDomainPlot::setCurrentGraphData(const QVector<double> &keys, const QVector<double> &values)
{
    _currentGraph->setData(keys, values);
}

void TimeDomainPlot::setVoltageRange(double min, double max)
{
    _customPlot->yAxis->setRange(min, max);
}

void TimeDomainPlot::setCurrentRange(double min, double max)
{
    _customPlot->yAxis2->setRange(min, max);
}

void TimeDomainPlot::replot()
{
    update(QRect(x(),y(),width(), height()));
}

