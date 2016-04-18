import QtQuick 2.3
import QtQuick.Window 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

MenuBar {
    id: mainMenuBar
    Menu {

        title: "File"

        MenuItem {
            text: "Open..."
            onTriggered: {
                var component = Qt.createComponent("ULoadFileDialog.qml");
                var win = component.createObject(root);
                win.show();
            }
        }

        MenuItem {
            text: "Close"
        }
        MenuItem {
            text: "Save"
            onTriggered: {
                
                var component = Qt.createComponent("ULoadFileDialog.qml");
                var win = component.createObject(root);
                win.show();

            }

        }

    }

    Menu {
        title: "Edit"

        MenuItem {
            text: "Cut"
        }

        MenuItem {
            text: "Copy"
        }

        MenuItem {
            text: "Paste"
        }
    }

    Menu {
        title: "Code"

        MenuItem {
            text: "Generate"
            onTriggered: {
                var component = Qt.createComponent("UCodeGenerationDialogue.qml");
                var win = component.createObject(root);
                win.show();
            }
        }

    }

    Menu {
        title: "Help"

    }

    Menu {
        title: "About"
    }

}

