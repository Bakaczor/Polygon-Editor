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
                focus: true

                function reload() {
                    var oldSource = source;
                    source = "";
                    source = oldSource;
                }

                Keys.onPressed: (event) => {
                    if (event.key === Qt.Key_Delete) {
                        SceneManager.removeSelected();
                    }
                }

               MouseArea {
                   id: mouseArea
                   anchors.fill: parent
                   hoverEnabled: true
                   acceptedButtons: Qt.AllButtons

                   onPressed: {
                        if (!SceneManager.isBuilding) {
                            SceneManager.checkObjects(mouseArea.mouseX + offset, mouseArea.mouseY + offset)
                            if (mouseArea.pressedButtons == Qt.RightButton) {
                               SceneManager.startDragging(mouseArea.mouseX + offset, mouseArea.mouseY + offset)
                            } else if (mouseArea.pressedButtons == Qt.MiddleButton) {
                               SceneManager.insertVertex(mouseArea.mouseX + offset, mouseArea.mouseY + offset)
                            }
                        }
                   }
                   onReleased: {
                        if (SceneManager.isDragging) {
                            SceneManager.stopDragging(mouseArea.mouseX + offset, mouseArea.mouseY + offset)
                        }
                   }
                   onDoubleClicked: {
                        if (mouseArea.pressedButtons == Qt.LeftButton) {
                           SceneManager.addVertex(mouseArea.mouseX + offset, mouseArea.mouseY + offset)
                        } else if (mouseArea.pressedButtons == Qt.RightButton && SceneManager.isBuilding) {
                           SceneManager.stopBuilding(mouseArea.mouseX + offset, mouseArea.mouseY + offset)
                        }
                   }
                   onPositionChanged:
                   {
                       if (SceneManager.isBuilding) {
                           SceneManager.drawProjection(mouseArea.mouseX + offset, mouseArea.mouseY + offset)
                        } else if (SceneManager.isDragging) {
                            SceneManager.moveObject(mouseArea.mouseX + offset, mouseArea.mouseY + offset)
                        }
                   }
                }
            }

            // dodawanie i usuwanie np. za pośrednictwem kółka od myszy (na krawędzi dodaj, na wierzchołku usuń)
            // usuwanie wielokąta za pośrednictwem delete

            Rectangle {
                width: 200
                height: parent.height
                color: "grey"
            }
        }
    }
}
