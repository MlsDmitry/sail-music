import QtQuick 2.0
import Sailfish.Silica 1.0
import QtQml.StateMachine 1.0 as DSM

import "../components"

Page {
    id: playlistPage

    property var playlistModel
    property var radioSM

    property string playlistCover
    property string playlistTitle


    Label {
        id: albumLabel

        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: Theme.paddingLarge
        }
        text: playlistTitle
    }

    Image {
        id: albumCover
        anchors {
            top: albumLabel.bottom
            topMargin: Theme.paddingLarge
            horizontalCenter: parent.horizontalCenter

        }
        source: playlistCover
    }


    Loader {
        id: loader

        anchors {
            top: albumCover.bottom
            left: playlistPage.left
            right: playlistPage.right
            bottom: playlistPage.bottom
        }

        BusyIndicator {
            anchors {
                centerIn: parent
            }

            size: BusyIndicatorSize.Large

            running: !radioSM.children[3].active
        }

    }

    Component {
        id: playlistTracksComponent

        PlaylistTracks {
            playlist: playlistModel
        }
    }

    Connections {
        target: radioSM.children[3]
        onActiveChanged: {
            if (active)
                loader.sourceComponent = playlistTracksComponent
        }
    }

    Component.onCompleted: {
        if (radioSM.children[3].active)
            loader.sourceComponent = playlistTracksComponent
    }
}
