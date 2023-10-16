import QtQuick
import QtQuick.Window
import QtQuick.Controls

ApplicationWindow {
    id: main_window

    visible: true
    width: 800
    height: 600
    title: "Polygon Editor"

    Rectangle {
        anchors.fill: parent
        color: "lightgrey"

        Row {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5

            Rectangle {
                id: canvas
                width: parent.width - 205
                height: parent.height
                color: "white"

                signal sendCoordinates(int x, int y)

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
    }
}
