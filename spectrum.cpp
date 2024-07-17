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

    int nx = 16000 / 128 * (9.856);
    int ny = 128;

    m_pColorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
    m_pColorMap->data()->setRange(QCPRange(0, 10), QCPRange(0, 8000)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
    // now we assign some data, by accessing the QCPColorMapData instance of the color map:

    QFile file(":/data.txt");
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
    QCPColorGradient customGradient;
    customGradient.clearColorStops();
    customGradient.setColorInterpolation(QCPColorGradient::ciRGB);
    customGradient.setLevelCount(256); // 设置颜色级别的数量

    customGradient.setColorStopAt(0.0000,  "#010101");
    customGradient.setColorStopAt(0.0039,  "#010202");
    customGradient.setColorStopAt(0.0078,  "#020202");
    customGradient.setColorStopAt(0.0118,  "#020303");
    customGradient.setColorStopAt(0.0157,  "#020304");
    customGradient.setColorStopAt(0.0196,  "#020405");
    customGradient.setColorStopAt(0.0235,  "#020506");
    customGradient.setColorStopAt(0.0275,  "#030607");
    customGradient.setColorStopAt(0.0314,  "#030708");
    customGradient.setColorStopAt(0.0353,  "#03080A");
    customGradient.setColorStopAt(0.0392,  "#03090C");
    customGradient.setColorStopAt(0.0431,  "#030A0E");
    customGradient.setColorStopAt(0.0471,  "#030C10");
    customGradient.setColorStopAt(0.0510,  "#030D11");
    customGradient.setColorStopAt(0.0549,  "#030E13");
    customGradient.setColorStopAt(0.0588,  "#020F15");
    customGradient.setColorStopAt(0.0627,  "#021017");
    customGradient.setColorStopAt(0.0667,  "#021119");
    customGradient.setColorStopAt(0.0706,  "#02121B");
    customGradient.setColorStopAt(0.0745,  "#02131E");
    customGradient.setColorStopAt(0.0784,  "#011420");
    customGradient.setColorStopAt(0.0824,  "#011522");
    customGradient.setColorStopAt(0.0863,  "#011624");
    customGradient.setColorStopAt(0.0902,  "#011726");
    customGradient.setColorStopAt(0.0941,  "#011828");
    customGradient.setColorStopAt(0.0980,  "#00192B");
    customGradient.setColorStopAt(0.1020,  "#001A2D");
    customGradient.setColorStopAt(0.1059,  "#001B2F");
    customGradient.setColorStopAt(0.1098,  "#001B32");
    customGradient.setColorStopAt(0.1137,  "#001C34");
    customGradient.setColorStopAt(0.1176,  "#001D36");
    customGradient.setColorStopAt(0.1216,  "#001E39");
    customGradient.setColorStopAt(0.1255,  "#001E3B");
    customGradient.setColorStopAt(0.1294,  "#011F3E");
    customGradient.setColorStopAt(0.1333,  "#012040");
    customGradient.setColorStopAt(0.1373,  "#012043");
    customGradient.setColorStopAt(0.1412,  "#022145");
    customGradient.setColorStopAt(0.1451,  "#032148");
    customGradient.setColorStopAt(0.1490,  "#04224A");
    customGradient.setColorStopAt(0.1529,  "#05234D");
    customGradient.setColorStopAt(0.1569,  "#06234F");
    customGradient.setColorStopAt(0.1608,  "#082352");
    customGradient.setColorStopAt(0.1647,  "#092454");
    customGradient.setColorStopAt(0.1686,  "#0B2456");
    customGradient.setColorStopAt(0.1725,  "#0D2559");
    customGradient.setColorStopAt(0.1765,  "#0F255B");
    customGradient.setColorStopAt(0.1804,  "#11255E");
    customGradient.setColorStopAt(0.1843,  "#132560");
    customGradient.setColorStopAt(0.1882,  "#152663");
    customGradient.setColorStopAt(0.1922,  "#172665");
    customGradient.setColorStopAt(0.1961,  "#192668");
    customGradient.setColorStopAt(0.2000,  "#1B266A");
    customGradient.setColorStopAt(0.2039,  "#1D266C");
    customGradient.setColorStopAt(0.2078,  "#1F266F");
    customGradient.setColorStopAt(0.2118,  "#212671");
    customGradient.setColorStopAt(0.2157,  "#232673");
    customGradient.setColorStopAt(0.2196,  "#262676");
    customGradient.setColorStopAt(0.2235,  "#282678");
    customGradient.setColorStopAt(0.2275,  "#2A267A");
    customGradient.setColorStopAt(0.2314,  "#2C267C");
    customGradient.setColorStopAt(0.2353,  "#2E267E");
    customGradient.setColorStopAt(0.2392,  "#312680");
    customGradient.setColorStopAt(0.2431,  "#332682");
    customGradient.setColorStopAt(0.2471,  "#352584");
    customGradient.setColorStopAt(0.2510,  "#372586");
    customGradient.setColorStopAt(0.2549,  "#3A2588");
    customGradient.setColorStopAt(0.2588,  "#3C248A");
    customGradient.setColorStopAt(0.2627,  "#3E248B");
    customGradient.setColorStopAt(0.2667,  "#41248D");
    customGradient.setColorStopAt(0.2706,  "#43238F");
    customGradient.setColorStopAt(0.2745,  "#452390");
    customGradient.setColorStopAt(0.2784,  "#482392");
    customGradient.setColorStopAt(0.2824,  "#4A2293");
    customGradient.setColorStopAt(0.2863,  "#4C2295");
    customGradient.setColorStopAt(0.2902,  "#4F2196");
    customGradient.setColorStopAt(0.2941,  "#512197");
    customGradient.setColorStopAt(0.2980,  "#542098");
    customGradient.setColorStopAt(0.3020,  "#562099");
    customGradient.setColorStopAt(0.3059,  "#581F9A");
    customGradient.setColorStopAt(0.3098,  "#5B1F9B");
    customGradient.setColorStopAt(0.3137,  "#5D1E9C");
    customGradient.setColorStopAt(0.3176,  "#5F1D9D");
    customGradient.setColorStopAt(0.3216,  "#621D9E");
    customGradient.setColorStopAt(0.3255,  "#641C9F");
    customGradient.setColorStopAt(0.3294,  "#671C9F");
    customGradient.setColorStopAt(0.3333,  "#691BA0");
    customGradient.setColorStopAt(0.3373,  "#6B1BA0");
    customGradient.setColorStopAt(0.3412,  "#6E1AA1");
    customGradient.setColorStopAt(0.3451,  "#701AA1");
    customGradient.setColorStopAt(0.3490,  "#7219A1");
    customGradient.setColorStopAt(0.3529,  "#7519A2");
    customGradient.setColorStopAt(0.3569,  "#7718A2");
    customGradient.setColorStopAt(0.3608,  "#7918A2");
    customGradient.setColorStopAt(0.3647,  "#7C17A2");
    customGradient.setColorStopAt(0.3686,  "#7E17A2");
    customGradient.setColorStopAt(0.3725,  "#8017A2");
    customGradient.setColorStopAt(0.3765,  "#8316A1");
    customGradient.setColorStopAt(0.3804,  "#8516A1");
    customGradient.setColorStopAt(0.3843,  "#8716A1");
    customGradient.setColorStopAt(0.3882,  "#8916A1");
    customGradient.setColorStopAt(0.3922,  "#8C16A0");
    customGradient.setColorStopAt(0.3961,  "#8E16A0");
    customGradient.setColorStopAt(0.4000,  "#90169F");
    customGradient.setColorStopAt(0.4039,  "#92169F");
    customGradient.setColorStopAt(0.4078,  "#94169E");
    customGradient.setColorStopAt(0.4118,  "#96169D");
    customGradient.setColorStopAt(0.4157,  "#99169D");
    customGradient.setColorStopAt(0.4196,  "#9B179C");
    customGradient.setColorStopAt(0.4235,  "#9D179B");
    customGradient.setColorStopAt(0.4275,  "#9F179A");
    customGradient.setColorStopAt(0.4314,  "#A11899");
    customGradient.setColorStopAt(0.4353,  "#A31898");
    customGradient.setColorStopAt(0.4392,  "#A51997");
    customGradient.setColorStopAt(0.4431,  "#A71A96");
    customGradient.setColorStopAt(0.4471,  "#A91A95");
    customGradient.setColorStopAt(0.4510,  "#AB1B94");
    customGradient.setColorStopAt(0.4549,  "#AD1C93");
    customGradient.setColorStopAt(0.4588,  "#AF1D92");
    customGradient.setColorStopAt(0.4627,  "#B11D91");
    customGradient.setColorStopAt(0.4667,  "#B31E90");
    customGradient.setColorStopAt(0.4706,  "#B51F8E");
    customGradient.setColorStopAt(0.4745,  "#B7208D");
    customGradient.setColorStopAt(0.4784,  "#B8218C");
    customGradient.setColorStopAt(0.4824,  "#BA228B");
    customGradient.setColorStopAt(0.4863,  "#BC2389");
    customGradient.setColorStopAt(0.4902,  "#BE2488");
    customGradient.setColorStopAt(0.4941,  "#C02587");
    customGradient.setColorStopAt(0.4980,  "#C12785");
    customGradient.setColorStopAt(0.5020,  "#C32884");
    customGradient.setColorStopAt(0.5059,  "#C52982");
    customGradient.setColorStopAt(0.5098,  "#C62A81");
    customGradient.setColorStopAt(0.5137,  "#C82B80");
    customGradient.setColorStopAt(0.5176,  "#CA2D7E");
    customGradient.setColorStopAt(0.5216,  "#CB2E7D");
    customGradient.setColorStopAt(0.5255,  "#CD2F7B");
    customGradient.setColorStopAt(0.5294,  "#CE307A");
    customGradient.setColorStopAt(0.5333,  "#D03278");
    customGradient.setColorStopAt(0.5373,  "#D13377");
    customGradient.setColorStopAt(0.5412,  "#D33475");
    customGradient.setColorStopAt(0.5451,  "#D43674");
    customGradient.setColorStopAt(0.5490,  "#D63772");
    customGradient.setColorStopAt(0.5529,  "#D73971");
    customGradient.setColorStopAt(0.5569,  "#D93A6F");
    customGradient.setColorStopAt(0.5608,  "#DA3C6E");
    customGradient.setColorStopAt(0.5647,  "#DB3D6D");
    customGradient.setColorStopAt(0.5686,  "#DD3F6B");
    customGradient.setColorStopAt(0.5725,  "#DE406A");
    customGradient.setColorStopAt(0.5765,  "#DF4268");
    customGradient.setColorStopAt(0.5804,  "#E14367");
    customGradient.setColorStopAt(0.5843,  "#E24565");
    customGradient.setColorStopAt(0.5882,  "#E34664");
    customGradient.setColorStopAt(0.5922,  "#E44863");
    customGradient.setColorStopAt(0.5961,  "#E54961");
    customGradient.setColorStopAt(0.6000,  "#E64B60");
    customGradient.setColorStopAt(0.6039,  "#E74D5E");
    customGradient.setColorStopAt(0.6078,  "#E94E5D");
    customGradient.setColorStopAt(0.6118,  "#EA505C");
    customGradient.setColorStopAt(0.6157,  "#EB525B");
    customGradient.setColorStopAt(0.6196,  "#EC5359");
    customGradient.setColorStopAt(0.6235,  "#ED5558");
    customGradient.setColorStopAt(0.6275,  "#ED5757");
    customGradient.setColorStopAt(0.6314,  "#EE5956");
    customGradient.setColorStopAt(0.6353,  "#EF5A54");
    customGradient.setColorStopAt(0.6392,  "#F05C53");
    customGradient.setColorStopAt(0.6431,  "#F15E52");
    customGradient.setColorStopAt(0.6471,  "#F26051");
    customGradient.setColorStopAt(0.6510,  "#F26150");
    customGradient.setColorStopAt(0.6549,  "#F3634F");
    customGradient.setColorStopAt(0.6588,  "#F4654E");
    customGradient.setColorStopAt(0.6627,  "#F5674D");
    customGradient.setColorStopAt(0.6667,  "#F5694C");
    customGradient.setColorStopAt(0.6706,  "#F66B4B");
    customGradient.setColorStopAt(0.6745,  "#F66C4A");
    customGradient.setColorStopAt(0.6784,  "#F76E4A");
    customGradient.setColorStopAt(0.6824,  "#F87049");
    customGradient.setColorStopAt(0.6863,  "#F87248");
    customGradient.setColorStopAt(0.6902,  "#F87448");
    customGradient.setColorStopAt(0.6941,  "#F97647");
    customGradient.setColorStopAt(0.6980,  "#F97847");
    customGradient.setColorStopAt(0.7020,  "#FA7A46");
    customGradient.setColorStopAt(0.7059,  "#FA7C46");
    customGradient.setColorStopAt(0.7098,  "#FA7E46");
    customGradient.setColorStopAt(0.7137,  "#FB8046");
    customGradient.setColorStopAt(0.7176,  "#FB8245");
    customGradient.setColorStopAt(0.7216,  "#FB8446");
    customGradient.setColorStopAt(0.7255,  "#FB8646");
    customGradient.setColorStopAt(0.7294,  "#FB8846");
    customGradient.setColorStopAt(0.7333,  "#FC8A46");
    customGradient.setColorStopAt(0.7373,  "#FC8C46");
    customGradient.setColorStopAt(0.7412,  "#FC8E47");
    customGradient.setColorStopAt(0.7451,  "#FC9048");
    customGradient.setColorStopAt(0.7490,  "#FC9248");
    customGradient.setColorStopAt(0.7529,  "#FC9449");
    customGradient.setColorStopAt(0.7569,  "#FC964A");
    customGradient.setColorStopAt(0.7608,  "#FB984B");
    customGradient.setColorStopAt(0.7647,  "#FB9A4C");
    customGradient.setColorStopAt(0.7686,  "#FB9C4D");
    customGradient.setColorStopAt(0.7725,  "#FB9E4E");
    customGradient.setColorStopAt(0.7765,  "#FBA050");
    customGradient.setColorStopAt(0.7804,  "#FBA251");
    customGradient.setColorStopAt(0.7843,  "#FAA453");
    customGradient.setColorStopAt(0.7882,  "#FAA655");
    customGradient.setColorStopAt(0.7922,  "#FAA857");
    customGradient.setColorStopAt(0.7961,  "#F9AA58");
    customGradient.setColorStopAt(0.8000,  "#F9AC5A");
    customGradient.setColorStopAt(0.8039,  "#F8AE5D");
    customGradient.setColorStopAt(0.8078,  "#F8B05F");
    customGradient.setColorStopAt(0.8118,  "#F8B261");
    customGradient.setColorStopAt(0.8157,  "#F7B463");
    customGradient.setColorStopAt(0.8196,  "#F7B666");
    customGradient.setColorStopAt(0.8235,  "#F6B868");
    customGradient.setColorStopAt(0.8275,  "#F6BA6B");
    customGradient.setColorStopAt(0.8314,  "#F5BC6E");
    customGradient.setColorStopAt(0.8353,  "#F4BE70");
    customGradient.setColorStopAt(0.8392,  "#F4C073");
    customGradient.setColorStopAt(0.8431,  "#F3C276");
    customGradient.setColorStopAt(0.8471,  "#F3C379");
    customGradient.setColorStopAt(0.8510,  "#F2C57C");
    customGradient.setColorStopAt(0.8549,  "#F2C77F");
    customGradient.setColorStopAt(0.8588,  "#F1C983");
    customGradient.setColorStopAt(0.8627,  "#F0CB86");
    customGradient.setColorStopAt(0.8667,  "#F0CD89");
    customGradient.setColorStopAt(0.8706,  "#EFCF8C");
    customGradient.setColorStopAt(0.8745,  "#EFD090");
    customGradient.setColorStopAt(0.8784,  "#EED293");
    customGradient.setColorStopAt(0.8824,  "#EED497");
    customGradient.setColorStopAt(0.8863,  "#EDD59A");
    customGradient.setColorStopAt(0.8902,  "#EDD79E");
    customGradient.setColorStopAt(0.8941,  "#ECD9A1");
    customGradient.setColorStopAt(0.8980,  "#ECDAA5");
    customGradient.setColorStopAt(0.9020,  "#ECDCA9");
    customGradient.setColorStopAt(0.9059,  "#ECDEAC");
    customGradient.setColorStopAt(0.9098,  "#EBDFB0");
    customGradient.setColorStopAt(0.9137,  "#EBE1B4");
    customGradient.setColorStopAt(0.9176,  "#EBE2B7");
    customGradient.setColorStopAt(0.9216,  "#EBE4BB");
    customGradient.setColorStopAt(0.9255,  "#EBE5BF");
    customGradient.setColorStopAt(0.9294,  "#EBE6C2");
    customGradient.setColorStopAt(0.9333,  "#ECE8C6");
    customGradient.setColorStopAt(0.9373,  "#ECE9C9");
    customGradient.setColorStopAt(0.9412,  "#ECEACD");
    customGradient.setColorStopAt(0.9451,  "#EDECD0");
    customGradient.setColorStopAt(0.9490,  "#EDEDD4");
    customGradient.setColorStopAt(0.9529,  "#EEEED7");
    customGradient.setColorStopAt(0.9569,  "#EFEFDB");
    customGradient.setColorStopAt(0.9608,  "#F0F0DE");
    customGradient.setColorStopAt(0.9647,  "#F1F2E1");
    customGradient.setColorStopAt(0.9686,  "#F2F3E4");
    customGradient.setColorStopAt(0.9725,  "#F3F4E7");
    customGradient.setColorStopAt(0.9765,  "#F4F5EA");
    customGradient.setColorStopAt(0.9804,  "#F6F6ED");
    customGradient.setColorStopAt(0.9843,  "#F7F7F0");
    customGradient.setColorStopAt(0.9882,  "#F9F8F2");
    customGradient.setColorStopAt(0.9922,  "#FBF9F5");
    customGradient.setColorStopAt(0.9961,  "#FDFAF7");
    customGradient.setColorStopAt(1.0000,  "#FEFBF9");

    m_pColorMap->setGradient(customGradient);
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
