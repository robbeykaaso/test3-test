import QtMultimedia 5.12
import QtQuick 2.12
import "qml/gui/Basic"

TWindow{
    id: vdooutput
    content: Rectangle{
        anchors.fill: parent
        VideoOutput{
            anchors.fill: parent
            source: frameProvider
        }
    }
    footbuttons: [
        {
            cap: "test",
            func: function(){
                Pipelines().run("testFrameProvider", "")
            }
        }
    ]
}
