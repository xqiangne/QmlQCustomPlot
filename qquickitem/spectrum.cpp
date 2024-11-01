#include "spectrum.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

Spectrum::Spectrum(QQuickItem* parent)
    : QQuickPaintedItem{parent}
{
    // 创建QCustomPlot对象
    m_pCustomPlot = new QCustomPlot();

    // configure axis rect:
    m_pCustomPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
    m_pCustomPlot->axisRect()->setupFullAxesBox(true);
    m_pCustomPlot->setOpenGl(true);

    m_pCustomPlot->xAxis->setLabel("时间(T/s)");
    m_pCustomPlot->yAxis->setLabel("频率(F/Hz)");
    m_pCustomPlot->xAxis->setVisible(false);
    m_pCustomPlot->yAxis->setVisible(false);
    m_pCustomPlot->xAxis2->setVisible(false);
    m_pCustomPlot->yAxis2->setVisible(false);

    // 创建QCPColorMap
    m_pColorMap = new QCPColorMap(m_pCustomPlot->xAxis, m_pCustomPlot->yAxis);

    int nx = 1232;
    int ny = 128;

    m_pColorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
    m_pColorMap->data()->setRange(QCPRange(0, 10), QCPRange(0, 8000)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
    // now we assign some data, by accessing the QCPColorMapData instance of the color map:

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
    // while(!file.atEnd())
    // {
    //     QByteArray byteArray = file.readLine();
    //     QString str = QString(byteArray);
    //     str.remove("\n");

    //     QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    //     QJsonObject obj = doc.object();
    //     m_pColorMap->data()->setCell(obj["t"].toDouble(), obj["f"].toDouble(), obj["s"].toDouble());
    // }
    file.close();

    // double x, y, z;
    // for (int xIndex=0; xIndex<nx; ++xIndex)
    // {
    //     for (int yIndex=0; yIndex<ny; ++yIndex)
    //     {
    //         m_pColorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
    //         double r = 3*qSqrt(x*x+y*y)+1e-2;
    //         z = 2*x*(qCos(r+2)/r-qSin(r+2)/r); // the B field strength of dipole radiation (modulo physical constants)
    //         m_pColorMap->data()->setCell(xIndex, yIndex, z);
    //     }
    // }

    // add a color scale:
    // QCPColorScale *colorScale = new QCPColorScale(m_pCustomPlot);
    // m_pCustomPlot->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
    // colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    // m_pColorMap->setColorScale(colorScale); // associate the color map with the color scale
    // colorScale->axis()->setLabel("Magnetic Field Strength");

    // set the color gradient of the color map to one of the presets:
    this->setGradient();

    // we could have also created a QCPColorGradient instance and added own colors to
    // the gradient, see the documentation of QCPColorGradient for what's possible.

    // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
    m_pColorMap->rescaleDataRange();

    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup = new QCPMarginGroup(m_pCustomPlot);
    m_pCustomPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    m_pCustomPlot->axisRect()->setAutoMargins(QCP::msNone);
    m_pCustomPlot->axisRect()->setMargins(QMargins(0, 0, 0, 0));
    m_pCustomPlot->plotLayout()->setMargins(QMargins(0, 0, 0, 0));

    m_pCustomPlot->replot();
    // colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // rescale the key (x) and value (y) axes so the whole color map is visible:
    m_pCustomPlot->rescaleAxes();
}

Spectrum::~Spectrum()
{
    m_pCustomPlot->removePlottable(m_pColorMap);
    delete m_pCustomPlot;
}

void Spectrum::paint(QPainter *painter)
{
    m_pCustomPlot->setGeometry(0,0,this->width(),this->height());
    painter->drawPixmap(0,0,this->width(),this->height(), m_pCustomPlot->toPixmap());
}

void Spectrum::setGradient()
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
