#ifndef SPECTRUMPLOT_H
#define SPECTRUMPLOT_H

#include "qabstractplot.h"

class SpectrumPlot : public QAbstractPlot
{
    Q_OBJECT
public:
    SpectrumPlot();
    ~SpectrumPlot();

private:
    QCPColorMap* m_pColorMap;
    void setGradient();

};

#endif // SPECTRUMPLOT_H
