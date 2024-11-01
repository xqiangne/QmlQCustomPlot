import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import myqml 1.0

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Column {
        spacing: 10
        topPadding: 10
        AudioWaveform {
            id: waveform
            Timer {
                id: timer

                interval: 100
                repeat: false
                running: false
                onTriggered: {
                    waveform.paintWaveForm()
                }
            }

            width: window.width
            height: 100

            onWidthChanged: {
                timer.restart()
            }
        }

        Spectrum {
            id: spectrum
            width: window.width
            height: 100
        }

        TimeDomainPlot {
            width: window.width
            height: 200
        }
    }

}
