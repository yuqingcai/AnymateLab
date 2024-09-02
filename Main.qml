import QtQuick
import QtQuick.Controls
import Qt.labs.platform
import AnymateLab

ApplicationWindow {
    id: mainWindow
    width: 1280
    height: 768
    visible: true
    // visibility: "Maximized"
    title: qsTr("AnymateLab")

    MainMenu {
        id: mainMenu
    }

    Rectangle {
        id: toolbar
        anchors.top: mainMenu.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height : 48
        color: Qt.rgba(1, 1, 1, 0.0)

        Rectangle {
            id: toolbarborderBottom
            y: parent.height - height
            width: parent.width
            height:1
            color: "lightgray"
        }
    }


    SplitView {
        id: splitView
        anchors.top: toolbar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        handle: Rectangle {
            id: handleDelegate1
            implicitWidth: 1
            implicitHeight: 1
            color: SplitHandle.pressed ? "lightgray"
                : (SplitHandle.hovered ? "lightgray" : "lightgray")

            containmentMask: Item {
                x: (handleDelegate1.width - width) / 2
                width: 16
                height: splitView.height
            }
        }


        SplitView {
            SplitView.preferredWidth: 250
            // SplitView.maximumWidth: 400
            orientation: Qt.Vertical

            handle: Rectangle {
                id: handleDelegate3
                implicitWidth: 1
                implicitHeight: 1
                color: SplitHandle.pressed ? "lightgray"
                    : (SplitHandle.hovered ? "lightgray" : "lightgray")

                containmentMask: Item {
                    y: (handleDelegate3.height - height) / 2
                    width: splitView.width
                    height: 16
                }
            }

            Rectangle {
                SplitView.fillWidth: true
                SplitView.preferredHeight: 500
                color: Qt.rgba(1, 1, 1, 0.0)
            }

            Rectangle {
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                color: Qt.rgba(1, 1, 1, 0.0)
            }

        }


        SplitView {
            id: splitView2
            SplitView.fillWidth: true
            SplitView.fillHeight: true
            orientation: Qt.Vertical


            handle: Rectangle {
                id: handleDelegate2
                implicitWidth: 1
                implicitHeight: 1
                color: SplitHandle.pressed ? "lightgray"
                    : (SplitHandle.hovered ? "lightgray" : "lightgray")

                containmentMask: Item {
                    y: (handleDelegate2.height - height) / 2
                    width: splitView.width
                    height: 16
                }
            }

            Stage {
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                sampleCount: 4 // normal MSAA, 8: high MSAA
                colorBufferFormat:RGBA8
                mirrorVertically: false
                // fixedColorBufferWidth: cbSize.checked ? slSize.value.toFixed(0) : 0
                // fixedColorBufferHeight: cbSize.checked ? slSize.value.toFixed(0) : 0
                alphaBlending: true

                NumberAnimation on angle {
                    from: 0
                    to: 360
                    duration: 5000
                    loops: Animation.Infinite
                    running: true
                }

            }

            // Stage {
            //     SplitView.fillWidth: true
            //     SplitView.fillHeight: true
            // }

            Rectangle {
                SplitView.preferredHeight: 200
                color: Qt.rgba(1, 1, 1, 0.0)
            }
        }

        SplitView {
            SplitView.preferredWidth: 250
            SplitView.maximumWidth: 250
        }
    }
}
