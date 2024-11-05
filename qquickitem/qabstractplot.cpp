#include "qabstractplot.h"

QAbstractPlot::QAbstractPlot(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_pCustomPlot(new QCustomPlot())
{
    setFlag(QQuickItem::ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);

    connect(this, &QQuickPaintedItem::widthChanged, this, &QAbstractPlot::onPlotSizeChanged);
    connect(this, &QQuickPaintedItem::heightChanged, this, &QAbstractPlot::onPlotSizeChanged);
    connect(m_pCustomPlot, &QCustomPlot::afterReplot,
            this, &QAbstractPlot::onPlotUpdate, Qt::UniqueConnection);
}

QAbstractPlot::~QAbstractPlot()
{
    delete m_pCustomPlot;
}

QCustomPlot *QAbstractPlot::customPlot()
{
    return m_pCustomPlot;
}


void QAbstractPlot::setOpenGL(bool enabled)
{
    if(m_pCustomPlot)
    {
        m_pCustomPlot->setOpenGl(true, 4);
    }
}

void QAbstractPlot::paint(QPainter *painter)
{
    if (!painter || !painter->isActive())
        return;

    // m_pCustomPlot->setGeometry(0, 0, this->width(), this->height());
    // painter->drawPixmap(0, 0, this->width(), this->height(), m_pCustomPlot->toPixmap());
    QPixmap pixmap(boundingRect().size().toSize());
    QCPPainter qcpPainter(&pixmap);
    m_pCustomPlot->toPainter(&qcpPainter);
    painter->drawPixmap(QPoint(), pixmap);
}

void QAbstractPlot::postMouseEvents(QMouseEvent *event)
{
    if(m_pCustomPlot)
    {
        QMouseEvent* newEvent = new QMouseEvent(event->type(), event->localPos(),
                                                event->button(), event->buttons(),
                                                event->modifiers());
        QCoreApplication::postEvent(m_pCustomPlot, newEvent);
    }
}

void QAbstractPlot::postWheelEvents(QWheelEvent *event)
{
    if(m_pCustomPlot)
    {
        // QWheelEvent* newEvent = new QWheelEvent(event->pos(),
        //                                         event->delta(),
        //                                         event->buttons(),
        //                                         event->modifiers(),
        //                                         event->orientation());

        QWheelEvent* newEvent = new QWheelEvent(event->position(), event->globalPosition(),
                                                event->pixelDelta(), event->angleDelta(),
                                                event->buttons(), event->modifiers(),
                                                event->phase(), event->inverted());
        QCoreApplication::postEvent(m_pCustomPlot, newEvent);
    }
}

void QAbstractPlot::mousePressEvent(QMouseEvent *event)
{
    this->postMouseEvents(event);
}

void QAbstractPlot::mouseReleaseEvent(QMouseEvent *event)
{
    this->postMouseEvents(event);
}

void QAbstractPlot::mouseMoveEvent(QMouseEvent *event)
{
    this->postMouseEvents(event);
}

void QAbstractPlot::mouseDoubleClickEvent(QMouseEvent *event)
{
    this->postMouseEvents(event);
}

void QAbstractPlot::wheelEvent(QWheelEvent *event)
{
    this->postWheelEvents(event);
}

void QAbstractPlot::onPlotSizeChanged()
{
    m_pCustomPlot->setGeometry(0, 0, (int)width(), (int)height());
#if 0
    QCPMarginGroup *marginGroup = new QCPMarginGroup(m_pCustomPlot);
    m_pCustomPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    m_pCustomPlot->axisRect()->setAutoMargins(QCP::msNone);
    m_pCustomPlot->axisRect()->setMargins(QMargins(0, 0, 0, 0));
    m_pCustomPlot->plotLayout()->setMargins(QMargins(0, 0, 0, 0));
#endif
    m_pCustomPlot->setViewport(QRect(0, 0, (int)width(), (int)height()));
    m_pCustomPlot->axisRect()->setOuterRect(QRect(0, 0, (int)width(), (int)height()));
    m_pCustomPlot->axisRect()->setMinimumMargins (QMargins(0, 0, 0, 0));
    m_pCustomPlot->axisRect()->setMargins(QMargins(0, 0, 0, 0));
}

void QAbstractPlot::onPlotUpdate()
{
    this->update();
}
