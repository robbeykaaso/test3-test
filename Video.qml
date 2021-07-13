import QtMultimedia 5.12
import QtQuick 2.12
import "qml/gui/Basic"
import Pipeline 1.0

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
                Pipeline.run("testFrameProvider", "")
            }
        }
    ]
}
