import QtQuick 2.0
import Sailfish.Silica 1.0
import org.k_bsp.sailmusic 1.0
import QtQml.StateMachine 1.0 as DSM

Item {
    property bool backgroundClicked: false
    property string coverUrl: ""
    property string title: ""


    RadioListModel {
        id: radio
    }


    DSM.StateMachine {
        id: radioSM
        initialState: st1
        running: true

        property string station: ""

        signal change_station
        signal load_tracks

        DSM.State {
            id: st1

            DSM.SignalTransition {
                targetState: stPreInitialized
                signal: radioSM.change_station
            }

            onEntered: {
                console.log("First state");
            }

        }

        DSM.State {
            id: stPreInitialized

            DSM.SignalTransition {
                targetState: stLoading
                signal: radioSM.load_tracks
            }

            onEntered: {
                if (station.length !== 0) {
                    console.log("radio.changeStation");

                    YaClient.currentPlaylist = radio;
                    radio.changeStation(station);
                    radioSM.load_tracks();
                }
            }
        }

        DSM.State {
            id: stLoading

            DSM.SignalTransition {
                targetState: stInitialized
                signal: radio.onTracksReceived
            }

            onEntered: {
                console.log("radio.getTracks()");
                radio.getTracks();
            }
        }

        DSM.State {
            id: stInitialized

            // Load more tracks
            DSM.SignalTransition {
                targetState: stLoading
                signal: radioSM.load_tracks
            }

            onEntered: {
                console.log("Radio initialized with tracks.");
            }
        }
    }

    Connections {
        target: radio

        onTracksReceived: {
//            pageStack.push(Qt.resolvedUrl("../pages/PlaylistPage.qml"), { "playlistModel": radio, "playlistCover": coverUrl, "playlistTitle": title });
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
                radioSM.station = station;
                radioSM.change_station();
                pageStack.push(Qt.resolvedUrl("../pages/PlaylistPage.qml"), { "radioSM": radioSM, "playlistModel": radio, "playlistCover": coverUrl, "playlistTitle": title });
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
                //            anchors.centerIn: parent
                height: parent.height - radioLabel.height
                width: height

                source: coverUrl.replace("%%", "200x200")
            }


            Label {
                id: radioLabel

                anchors {
                    horizontalCenter: parent.horizontalCenter                }

                text: title
            }
        }

    }
}
