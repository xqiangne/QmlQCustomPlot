#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QObject>
#include <QQuickPaintedItem>
#include "thirdparty/qcustomplot/qcustomplot.h"

class Spectrum : public QQuickPaintedItem
{
    Q_OBJECT
public:
    Spectrum(QQuickItem* parent = nullptr);
    ~Spectrum();
    virtual void paint(QPainter *painter);

private:
    QCustomPlot* m_pCustomPlot;
    QCPColorMap* m_pColorMap;

    void setGradient();

};

#endif // SPECTRUM_H
