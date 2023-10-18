import QtQuick
import QtQuick.Window
import QtQuick.Controls

ApplicationWindow {
    id: main_window

    visible: true
    minimumWidth: 800
    maximumWidth: 800
    minimumHeight: 600
    maximumHeight: 600
    title: "Polygon Editor"

    Connections {
        target: SceneManager
        onNewFrameReceived: image.reload();
    }

    Rectangle {
        anchors.fill: parent
        color: "lightgrey"

        Row {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5

            Rectangle {
                width: parent.width - 205
                height: parent.height
                color: "white"

                Image {
                    id: image
                    width: parent.width
                    height: parent.height
                    source: "image://SceneManager/image"
                    cache: false

                    function reload() {
                        var oldSource = source;
                        source = "";
                        source = oldSource;
                    }

                   MouseArea {
                       anchors.fill: parent
                       onPressed: {
                           print("clicked: " + mouse.x + " " + mouse.y)
                       }
                       onClicked: {
                           print("clicked: " + mouse.x + " " + mouse.y)
                       }
                       onDoubleClicked: {
                           print("double clicked: " + mouse.x + " " + mouse.y)
                       }
                   }
                }
            }

            Rectangle {
                width: 200
                height: parent.height
                color: "grey"
            }
        }
    }
}
