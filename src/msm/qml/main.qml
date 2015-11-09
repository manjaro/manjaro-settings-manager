import QtQuick 2.2
import QtPositioning 5.2
import QtLocation 5.3

GridView {
    id: grid
    anchors.fill: parent
    cellWidth: 150
    cellHeight: 150

    signal itemClicked(string msg)

    model: ListModel {
        ListElement {
            name: "Kernel"
            themeIcon: "image://icon/preferences-system"
            module: "msm_kernel"
        }
        ListElement {
            name: "Keyboard"
            themeIcon: "image://icon/preferences-desktop-keyboard"
            module: "msm_keyboard"
        }
        ListElement {
            name: "Language Packages"
            themeIcon: "image://icon/preferences-desktop-locale"
            module: "msm_language_packages"
        }
        ListElement {
            name: "Locale"
            themeIcon: "image://icon/preferences-desktop-locale"
            module: "msm_locale"
        }
        ListElement {
            name: "MHWD"
            themeIcon: "image://icon/audio-card"
            module: "msm_mhwd"
        }
        ListElement {
            name: "Timedate"
            themeIcon: "image://icon/preferences-system-time"
            module: "msm_timedate"
        }
        ListElement {
            name: "Users"
            themeIcon: "image://icon/preferences-desktop-user"
            module: "msm_users"
        }
    }

    delegate: Rectangle {
        width: grid.cellWidth; height: grid.cellHeight

        Image {
            id: icon
            source: themeIcon
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text {
            text: name
            horizontalAlignment: Text.AlignHCenter
            width: 100
            wrapMode: Text.Wrap
            maximumLineCount: 2
            elide: Text.ElideMiddle
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: icon.bottom
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                grid.itemClicked(module)
            }
        }
    }
}

