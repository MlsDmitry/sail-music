import QtQuick 2.0
import Sailfish.Silica 1.0

Item {

    property var radio
    Label {
        id: radioListLabel

        text: "Radios for you :)"
        font {
            pixelSize: Theme.fontSizeLarge
            italic: true
        }

        anchors.centerIn: parent
    }

    SilicaListView {
        id: radioList
        anchors {
            top: radioListLabel.bottom
        }

        width: parent.width
        height: parent.height

        spacing: 10


        orientation: ListView.Horizontal

        model: ListModel {
            ListElement {
                station: "user:onyourwave"
                radioCoverUrl: "https://avatars.yandex.net/get-music-misc/70850/rotor-personal-station-icon/400x400"
                radioName: "My Vibe"
                radioColor: "#2AA75B"
            }

            ListElement {
                station: "local:moscow"
                radioCoverUrl: "https://avatars.yandex.net/get-music-misc/29541/rotor-local-moscow-icon/400x400"
                radioName: "Popular in Moscow"
                radioColor: "black"
            }
            ListElement {
                station: "genre:doommetal"
                radioCoverUrl: "https://avatars.yandex.net/get-music-misc/34161/rotor-genre-metal-icon/400x400"
                radioName: "Doom metal"
                radioColor: "black"
            }
            ListElement {
                station: "genre:bassgenre"
                radioCoverUrl: "https://avatars.yandex.net/get-music-misc/40584/rotor-genre-dubstep-icon/400x400"
                radioName: "Bass"
                radioColor: "black"
            }
            ListElement {
                station: "genre:rap"
                radioCoverUrl: "https://avatars.yandex.net/get-music-misc/34161/rotor-genre-rusrap-icon/400x400"
                radioName: "Russian Rap"
                radioColor: "black"
            }
        }

        delegate: Item {
            id: someItem
            width: radioList.width / 2
            height: parent.height

            Rectangle {
                id: itemContainer
                anchors.fill: parent

                anchors.centerIn: parent
                color: radioColor

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    onClicked: {
                        radio.changeStation(station);
                        radio.sendTracksRequest();

                    }
                }

                anchors {
                    centerIn: parent
                }

                Image {
                    id: cover

                    anchors.centerIn: parent

                    source: radioCoverUrl
                }
                Label {
                    id: radioLabel

                    anchors {
                        horizontalCenter: parent.horizontalCenter
                    }

                    anchors.top: cover.bottom

                    text: radioName
                }
            }
        }


        HorizontalScrollDecorator {
            flickable: radioList
        }
    }
}