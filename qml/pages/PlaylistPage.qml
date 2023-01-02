import QtQuick 2.0
import Sailfish.Silica 1.0
import QtQml.StateMachine 1.0 as DSM
import org.k_bsp.sailmusic 1.0

import "../components"
import "../Playlist"

Page {
    id: playlistPage

    property var playlistModel
    property var radioSM

    property string playlistCover
    property string playlistTitle

    SilicaFlickable {
        anchors.fill: parent

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
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }

            BusyIndicator {
                id: indicator

                anchors {
                    centerIn: parent
                }

                size: BusyIndicatorSize.Large

                running: !radioSM.ready
            }

        }

        Connections {
            target: radioSM

            onReadyChanged: {
                loader.sourceComponent = playlistTracksComponent
            }
        }

        Component {
            id: playlistTracksComponent

            Playlist {
                playlist: playlistModel
            }
        }

        Component.onCompleted: {
            if (radioSM.ready)
                loader.sourceComponent = playlistTracksComponent
            else if (YaClient.currentPlaylist.rowCount() === 0) {
                radioSM.load_tracks();
            }
        }

        PushUpMenu {
            MenuItem {
                text: "Load more tracks"

                onClicked: radioSM.load_tracks()
            }
        }
    }

}
