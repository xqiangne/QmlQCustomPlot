#ifndef WAVEFORMPLOT_H
#define WAVEFORMPLOT_H

#include "qabstractplot.h"

class WaveformPlot : public QAbstractPlot
{
    Q_OBJECT
public:
    WaveformPlot(QAbstractPlot* parent = nullptr);

    Q_INVOKABLE void paintWaveForm();

private:
    QCPGraph* m_pAudioWaveformGraph;
};

#endif // WAVEFORMPLOT_H
