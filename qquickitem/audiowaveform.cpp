#include "audiowaveform.h"
#include <QFile>

#define BITS_DEPTH_16

AudioWaveform::AudioWaveform(QQuickItem* parent)
    : QQuickPaintedItem{parent}
{
    m_pCustomPlot = new QCustomPlot;
    m_pAudioWaveformGraph = m_pCustomPlot->addGraph();
    m_pAudioWaveformGraph->setPen(QPen(Qt::red));

    m_pCustomPlot->yAxis->setNumberFormat("gbc");
    m_pCustomPlot->yAxis->setNumberPrecision(1);
    m_pCustomPlot->yAxis->setRange(-1, 1);

    m_pCustomPlot->setBackground(QBrush(QColor("#cde8ff")));
    //    m_pCustomPlot->setBackground(QBrush(QColor(Qt::lightGray)));

    m_pCustomPlot->xAxis->setVisible(false);
    m_pCustomPlot->yAxis->setVisible(false);

    m_pCustomPlot->setOpenGl(true);
}

AudioWaveform::~AudioWaveform()
{
    m_pAudioWaveformGraph->data().clear();
    m_pCustomPlot->clearGraphs();
    delete m_pCustomPlot;
}

void AudioWaveform::paint(QPainter *painter)
{
    m_pCustomPlot->setGeometry(0,0,this->width(),this->height());
    painter->drawPixmap(0,0,this->width(),this->height(), m_pCustomPlot->toPixmap());
}

void AudioWaveform::refresh()
{
    update(QRect(0,0,width(), height()));
}

void AudioWaveform::paintWaveForm()
{
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
    this->setData(xValue, yValue);
    this->refresh();
}

void AudioWaveform::setData(QVector<double> xValue, QVector<double> yValue)
{
    m_pAudioWaveformGraph->data().clear();
    m_pCustomPlot->xAxis->setRange(0, xValue.size());
    m_pAudioWaveformGraph->setData(xValue, yValue);
    this->refresh();
}




