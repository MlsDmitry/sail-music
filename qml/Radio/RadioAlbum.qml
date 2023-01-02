import QtQuick 2.0
import Sailfish.Silica 1.0
import org.k_bsp.sailmusic 1.0
import QtQml.StateMachine 1.0 as DSM

Item {
    property bool backgroundClicked: false
    property string coverUrl: ""
    property string title: ""
    property string station: ""


    RadioListModel {
        id: radio
    }


    DSM.StateMachine {
        id: radioSM
        initialState: s1
        running: true

        property bool ready: stReadyToPlay.active

        property string station: ""

        signal load_track_info();
        signal load_tracks();
        signal init();

        DSM.State {
            id: s1

            DSM.SignalTransition {
                targetState: sStationInitialized
                signal: radioSM.init
            }

            onEntered: {
                console.log("Initialized album");
            }

        }

        DSM.State {
            id: sStationInitialized

            DSM.SignalTransition {
                targetState: stLoadingTracks
                signal: radioSM.load_tracks
            }
        }

        DSM.State {
            id: stLoadingTracks

            DSM.SignalTransition {
                targetState: stReadyToPlay
                signal: radio.onTracksReceived
            }

            onEntered: {
                yaclientSM.loading();
                YaClient.currentPlaylist.getTracks();
            }
        }

        DSM.State {
            id: stLoadingTrack

            DSM.SignalTransition {
                targetState: stReadyToPlay
                signal: radio.onCurrentTrackLinkReady
            }

            onEntered: {
                yaclientSM.loading();
                console.log("Loading track...", radio.currentIndex);
                YaClient.currentPlaylist.prepareCurrentTrackToPlay();
            }
        }

        DSM.State {
            id: stReadyToPlay

            DSM.SignalTransition {
                targetState: stLoadingTracks
                signal: radioSM.load_tracks
            }
            
            DSM.SignalTransition {
                targetState: stLoadingTrack
                signal: radioSM.load_track_info
            }

            onEntered: {
                console.log("Album is ready.");
                yaclientSM.playlist_ready();
            }
        }
    }

    Connections {
        target: radio

        onTracksReceived: {
            radioSM.load_track_info();
        }
    }

    Rectangle {

        id: itemContainer
        anchors.fill: parent

        anchors.centerIn: parent
        color: Theme.darkPrimaryColor
        opacity: 0.7

        radius: 30

        //        z: 1

        Behavior on opacity {
            FadeAnimator {
                duration: 30
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {         
                radio.changeStation(station);
                yaclientSM.playlistSM = radioSM;
                YaClient.setPlaylist(radio);


                pageStack.push(Qt.resolvedUrl("../pages/PlaylistPage.qml"), { "radioSM": radioSM, "playlistModel": radio, "playlistCover": coverUrl, "playlistTitle": title });
//                pageStack.push(Qt.resolvedUrl("../pages/PlaylistPage.qml"), { "playlistModel": radio, "playlistCover": coverUrl, "playlistTitle": title });

            }
            onPressed: {
                itemContainer.opacity = 1.0;
                cover.width = cover.width - 10;
                cover.height = cover.height - 10;
            }
            onReleased: {
                itemContainer.opacity = 0.7;
                cover.width = cover.width + 10;
                cover.height = cover.height + 10;

            }
        }


        Column {
            id: playlistContent

            anchors {
                fill: parent
                margins: Theme.paddingMedium
            }

            //            z: 1

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                height: parent.height - radioLabel.height
                width: height

                source: coverUrl.replace("%%", "200x200")

            }


            Label {
                id: radioLabel

                anchors {
                    horizontalCenter: parent.horizontalCenter
                }

                text: title
            }
        }

    }
}
