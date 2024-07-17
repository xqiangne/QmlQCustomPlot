#ifndef TIMEDOMAINPLOT_H
#define TIMEDOMAINPLOT_H

#include "qcustomplot.h"
#include <QRect>
#include <QQuickPaintedItem>

class TimeDomainPlot: public QQuickPaintedItem
{

public:
    explicit TimeDomainPlot(QQuickItem* parent = nullptr);
    ~TimeDomainPlot();

    const static int DATA_SIZE;
    const static double XAXIS_RANGE_START;
    const static double XAXIS_RANGE_END;

    virtual void paint(QPainter *painter);

    void setVoltageGraphData(const QVector<double> &keys, const QVector<double> &values);
    void setCurrentGraphData(const QVector<double> &keys, const QVector<double> &values);
    void setVoltageRange(double min, double max);
    void setCurrentRange(double min, double max);
    void replot();

private:
    QCustomPlot *_customPlot;
    QCPGraph* _voltageGraph;
    QCPGraph* _currentGraph;
};

#endif // TIMEDOMAINPLOT_H

