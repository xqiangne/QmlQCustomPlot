#ifndef QABSTRACTPLOT_H
#define QABSTRACTPLOT_H

#include <QQuickPaintedItem>
#include "thirdparty/qcustomplot/qcustomplot.h"

class QAbstractPlot : public QQuickPaintedItem
{
    Q_OBJECT
public:
    QAbstractPlot(QQuickItem *parent = nullptr);
    ~QAbstractPlot();

    QCustomPlot* customPlot();

    void setOpenGL(bool enabled);

protected:
    virtual void paint(QPainter *painter);
    virtual void postMouseEvents(QMouseEvent* event);
    virtual void postWheelEvents(QWheelEvent* event);

    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent *event);

protected:
    void onPlotSizeChanged();
    void onPlotUpdate();

private:
    QCustomPlot* m_pCustomPlot;
};

#endif // QABSTRACTPLOT_H
