#ifndef AUDIOWAVEFORM_H
#define AUDIOWAVEFORM_H

#include <QObject>
#include <QQuickPaintedItem>
#include "qcustomplot.h"

class AudioWaveform : public QQuickPaintedItem
{
    Q_OBJECT
public:
    AudioWaveform(QQuickItem* parent = nullptr);
    ~AudioWaveform();

    virtual void paint(QPainter *painter);

    void refresh();

    Q_INVOKABLE void paintWaveForm();

    void setData(QVector<double> xValue, QVector<double> yValue);


private:
    QCustomPlot* m_pCustomPlot;
    QCPGraph* m_pAudioWaveformGraph;


public slots:

};

#endif // AUDIOWAVEFORM_H
