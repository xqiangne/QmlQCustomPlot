#include "spectrumplot.h"

SpectrumPlot::SpectrumPlot()
{
    QCustomPlot* pCustomPlot = this->customPlot();
    // 创建QCPColorMap
    m_pColorMap = new QCPColorMap(pCustomPlot->xAxis, pCustomPlot->yAxis);
    // pCustomPlot->xAxis->setVisible(false);
    // pCustomPlot->yAxis->setVisible(false);
    // pCustomPlot->xAxis2->setVisible(false);
    // pCustomPlot->yAxis2->setVisible(false);

    pCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    int nx = 1232;
    int ny = 128;

    m_pColorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
    m_pColorMap->data()->setRange(QCPRange(0, 10), QCPRange(0, 8000)); // and span the coordinate range -4..4 in both key (x) and value

    QFile file(":/other/spectrumdata.txt");
    file.open(QIODevice::ReadOnly);
    quint64 num = 0;

    for (int xIndex=0; xIndex<nx; ++xIndex)
    {
        for (int yIndex=0; yIndex<ny; ++yIndex)
        {
            if(file.atEnd())
                goto end;
            QByteArray byteArray = file.readLine();
            QString str = QString(byteArray);
            str.remove("\n");
            QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
            QJsonObject obj = doc.object();
            m_pColorMap->data()->setCell(xIndex, yIndex, obj["s"].toDouble());
            ++num;
        }
    }
end:
    qDebug() << "num" << num;
    file.close();

    this->setGradient();
    m_pColorMap->rescaleDataRange();

    pCustomPlot->replot();
    pCustomPlot->rescaleAxes();
}

SpectrumPlot::~SpectrumPlot()
{
    customPlot()->removePlottable(m_pColorMap);
}

void SpectrumPlot::setGradient()
{
    QFile file(":/conf/gradient.json");

    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray byteArray = file.readAll();
        file.close();
        QJsonParseError jsonError;
        QJsonDocument doc = QJsonDocument::fromJson(byteArray, &jsonError);
        if(jsonError.error == QJsonParseError::NoError)
        {
            QJsonObject obj = doc.object();
            QJsonArray array = obj["gradient"].toArray();
            QJsonObject temp;

            QCPColorGradient customGradient;
            customGradient.clearColorStops();
            customGradient.setColorInterpolation(QCPColorGradient::ciRGB);
            customGradient.setLevelCount(256); // 设置颜色级别的数量
            for(int i=0;i<array.size();++i)
            {
                temp = array.at(i).toObject();
                customGradient.setColorStopAt(temp["position"].toDouble(), temp["color"].toString());
            }
            m_pColorMap->setGradient(customGradient);
        }
        else
            qDebug() << "load gradient conf error";

    }
    else
        qDebug() << file.errorString();
}

