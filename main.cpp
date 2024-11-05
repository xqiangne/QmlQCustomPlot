#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "qquickitem/timedomainplot.h"
#include "qquickitem/audiowaveform.h"
#include "qquickitem/spectrum.h"
#include "qquickitem/spectrumplot.h"
#include "qquickitem/waveformplot.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);

    qmlRegisterType<TimeDomainPlot>("myqml",1,0,"TimeDomainPlot");
    qmlRegisterType<AudioWaveform>("myqml",1,0,"AudioWaveform");
    qmlRegisterType<Spectrum>("myqml",1,0,"Spectrum");

    qmlRegisterType<SpectrumPlot>("myqml",1,0,"SpectrumPlot");
    qmlRegisterType<WaveformPlot>("myqml",1,0,"WaveformPlot");

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
