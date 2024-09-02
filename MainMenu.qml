import QtQuick
import Qt.labs.platform

MenuBar {
    Menu {
        id: menuFile
        title: qsTr("File")

        MenuItem {
            text: qsTr("New")
            shortcut: "Ctrl+N"
            onTriggered: {
                console.log("New file created");
            }
        }

        MenuItem {
            text: qsTr("Open")
            shortcut: "Ctrl+O"
            onTriggered: {
                console.log("File opened");
            }
        }

        MenuItem {
            // this one won't show up in File menu because of its `role`,
            // and it will actually appear in the main menu - under the application name,
            // where you'd usually expect About menu to be
            //text: qsTr("About")    // no need to set the text,
            role: MenuItem.AboutRole // because it gets generated
            onTriggered: console.debug("show your About window")
        }
    }

    Menu {
        id: menuEdit
        title: qsTr("Edit")

        MenuItem {
            text: qsTr("Undo")
            shortcut: "Ctrl+Z"
            onTriggered: {
                console.log("Undo action triggered");
            }
        }
        MenuItem {
            text: qsTr("Redo")
            shortcut: "Ctrl+Y"
            onTriggered: {
                console.log("Redo action triggered");
            }
        }
    }

    Menu {
        id: menuHelp
        title: qsTr("Help")
        MenuItem {
            text: "Contact"
            onTriggered: {
                console.log("Contact action triggered");
            }
        }
    }
}


