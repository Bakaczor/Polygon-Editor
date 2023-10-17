import QtQuick
import QtQuick.Window
import QtQuick.Controls
import Painter

ApplicationWindow {
    id: main_window

    visible: true
    minimumWidth: 800
    maximumWidth: 800
    minimumHeight: 600
    maximumHeight: 600
    title: "Polygon Editor"

    Rectangle {
        anchors.fill: parent
        color: "lightgrey"

        Row {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5

            Image {
                id: image
                width: parent.width - 205
                height: parent.height
                source: myPainter.getScene()

               MouseArea {
                   anchors.fill: parent
                   onClicked: {
                       print("clicked: " + mouse.x + " " + mouse.y)
                   }
                   onDoubleClicked: {
                       print("double clicked: " + mouse.x + " " + mouse.y)
                   }
               }
            }

            Rectangle {
                width: 200
                height: parent.height
                color: "grey"
            }
        }

        Painter {
            id: myPainter
        }
    }
}
