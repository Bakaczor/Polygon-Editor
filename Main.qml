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

    readonly property int offset: 6

    Connections {
        target: SceneManager
        function onImageChanged() {
            image.reload();
        }
    }

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
                source: "image://SceneManager/image"
                cache: false

                function reload() {
                    var oldSource = source;
                    source = "";
                    source = oldSource;
                }

               MouseArea {
                   id: mouseArea
                   anchors.fill: parent
                   hoverEnabled: true

                   onPressed: SceneManager.checkObjects(mouseArea.mouseX + offset, mouseArea.mouseY + offset)
                   onReleased: SceneManager.todo(mouseArea.mouseX + offset, mouseArea.mouseY + offset)
                   onDoubleClicked: {
                       if (mouseArea.pressedButtons == Qt.LeftButton) {
                           SceneManager.addVertex(mouseArea.mouseX + offset, mouseArea.mouseY + offset)
                       }
                       // WHY IT DOESN"T WORK
                       //if (mouseArea.pressedButtons == Qt.RightButton) {
                       //    SceneManager.stopBuilding(mouseArea.mouseX + offset, mouseArea.mouseY + offset)
                       //}
                   }
                   onPositionChanged: SceneManager.drawProjection(mouseArea.mouseX + offset, mouseArea.mouseY + offset)
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
