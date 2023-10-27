import QtQuick
import QtQuick.Window
import QtQuick.Controls

import com.algorithm.enum 1.0
import com.orientation.enum 1.0

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

        function onEdgeChanged(type) {
            horizontal.checked = false
            vertical.checked = false
            none.checked = false

            if (type === Orientation.Horizontal) {
                horizontal.checked = true
            } else if (type === Orientation.Vertical) {
                vertical.checked = true
            } else {
                none.checked = true
            }
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
                source: "image://ImageProvider/image"
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

            Rectangle {
                width: 200
                height: parent.height
                color: "deepskyblue"

                Column {
                    padding: 5
                    spacing: 10
                    Column {
                        focus: false
                        spacing: 2
                        Label {
                            text: "Choose line algorithm:"
                        }
                        RadioButton {
                            checked: true
                            text: "Library"
                            onClicked: {
                                SceneManager.setType(Algorithm.Library)
                            }
                            focusPolicy: Qt.NoFocus
                        }
                        RadioButton {
                            text: "Bresenham"
                            onClicked: {
                                SceneManager.setType(Algorithm.Bresenham)
                            }
                            focusPolicy: Qt.NoFocus
                        }
                    }
                    Column {
                        focus: false
                        spacing: 2
                        Label {
                            text: "Change edge orientation:"
                        }
                        RadioButton {
                            id: horizontal
                            text: "Horizontal"
                            onClicked: {
                                SceneManager.changeOrientation(Orientation.Horizontal)
                            }
                            focusPolicy: Qt.NoFocus
                        }
                        RadioButton {
                            id: vertical
                            text: "Vertical"
                            onClicked: {
                                SceneManager.changeOrientation(Orientation.Vertical)
                            }
                            focusPolicy: Qt.NoFocus
                        }
                        RadioButton {
                            id: none
                            text: "None"
                            onClicked: {
                                SceneManager.changeOrientation(Orientation.None)
                            }
                            focusPolicy: Qt.NoFocus
                            checked: true
                        }
                    }
                    Column {
                        spacing: 5
                        Label {
                            text: "Offset polygon:"
                        }
                        Slider {
                            implicitWidth: 150
                            from: 0
                            value: 0
                            to: 50
                            stepSize: 1
                            snapMode: Slider.SnapAlways
                            focusPolicy: Qt.NoFocus
                        }
                    }
                }
            }
        }
    }
}
