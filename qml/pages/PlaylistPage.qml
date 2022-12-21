import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {
    property var playlistModel
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

    PlaylistTracks {
        anchors {
            top: albumCover.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        playlist: playlistModel
    }
}
