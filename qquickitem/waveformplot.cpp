#include "waveformplot.h"

#define BITS_DEPTH_16

WaveformPlot::WaveformPlot(QAbstractPlot* parent)
    : QAbstractPlot(parent)
{
    QCustomPlot* pCustomPlot = this->customPlot();
    m_pAudioWaveformGraph = pCustomPlot->addGraph();
    m_pAudioWaveformGraph->setPen(QPen(Qt::red));

    pCustomPlot->yAxis->setNumberFormat("gbc");
    pCustomPlot->yAxis->setNumberPrecision(1);
    pCustomPlot->yAxis->setRange(-1, 1);

    pCustomPlot->setBackground(QBrush(QColor("#cde8ff")));

    pCustomPlot->xAxis->setVisible(true);
    pCustomPlot->yAxis->setVisible(true);

    pCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    // pCustomPlot->axisRect()->setRangeZoomFactor(1.1, 1); // 设置缩放系数
    pCustomPlot->axisRect()->setRangeZoom(Qt::Horizontal);  // 只有水平方向可以缩放
    pCustomPlot->axisRect()->setRangeDrag(Qt::Horizontal);  // 只有水平方向可以拖动

    this->setOpenGL(true);
}

void WaveformPlot::paintWaveForm()
{
    QCustomPlot* pCustomPlot = this->customPlot();
    QVector<double> xValue;
    QVector<double> yValue;
    QFile file(":/audio/audio1.pcm");

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.errorString();
        return;
    }

    int maxSize = file.size() / 2;
    // 除以2是多绘制一些点，使波形看起来更加连续
    int step = maxSize / this->width() / 2;

    if (step < 1)
        step = 1;
    else if (step > maxSize)
        step = maxSize;

    short cur_max = 0;
    short cur_min = 0;
    int index_max = 0;
    int index_min = 0;
    quint64 index = 0;

#ifdef BITS_DEPTH_8
    char* buffer = new char[step];
    while(qint64 size = file.read(buffer, step))
    {

        cur_max = buffer[0];
        cur_min = buffer[0];
        index_max = 0;
        index_min = 0;
        for(int i=0;i<size;i++)
        {
            //遍历找这一段的最大最小值
            if (cur_max < buffer[i])
            {
                cur_max = buffer[i];
                index_max = i;
            }
            if (cur_min > buffer[i])
            {
                cur_min = buffer[i];
                index_min = i;
            }
        }
        //根据先后顺序存最大最小，相等就存一个
        if (index_max < index_min)
        {
            xValue.push_back(index);
            yValue.push_back(qreal(uchar(cur_max) - 128) / qreal(128));
            // m_vector.push_back(QPointF(index, qreal(uchar(cur_max) - 128) / qreal(128)));
            index++;
        }

        xValue.push_back(index);
        yValue.push_back(qreal(uchar(cur_min) - 128) / qreal(128));
        // m_vector.push_back(QPointF(index, qreal(uchar(cur_min) - 128) / qreal(128)));
        index++;
        if (index_max > index_min)
        {
            xValue.push_back(index);
            yValue.push_back(qreal(uchar(cur_max) - 128) / qreal(128));
            // m_vector.push_back(QPointF(index, qreal(uchar(cur_max) - 128) / qreal(128)));
            index++;
        }
    }
    delete[] buffer;
#endif

#ifdef BITS_DEPTH_16
    char* buffer = new char[step * 2];
    while(qint64 size = file.read(buffer, step * 2))
    {
        short* data = (short*)buffer;

        cur_max = data[0];
        cur_min = data[0];
        index_max = 0;
        index_min = 0;

        for(int i=0;i<size / 2;i++)
        {
            //遍历找这一段的最大最小值
            if (cur_max < data[i])
            {
                cur_max = data[i];
                index_max = i;
            }
            if (cur_min > data[i])
            {
                cur_min = data[i];
                index_min = i;
            }
        }
        //根据先后顺序存最大最小，相等就存一个
        if (index_max < index_min)
        {
            xValue.push_back(index);
            yValue.push_back(cur_max / 32768.0);
            // m_vector.push_back(QPointF(index, cur_max / 32768.0));
            index++;
        }
        xValue.push_back(index);
        yValue.push_back(cur_min / 32768.0);
        // m_vector.push_back(QPointF(index, cur_min / 32768.0));
        index++;
        if (index_max > index_min)
        {
            xValue.push_back(index);
            yValue.push_back(cur_max / 32768.0);
            // m_vector.push_back(QPointF(index, cur_max / 32768.0));
            index++;
        }

    }
    delete[] buffer;
#endif
    file.close();
    m_pAudioWaveformGraph->data().clear();
    pCustomPlot->xAxis->setRange(0, xValue.size());
    m_pAudioWaveformGraph->setData(xValue, yValue);
    this->update();
}
