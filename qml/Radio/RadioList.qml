import QtQuick 2.0
import Sailfish.Silica 1.0
import org.k_bsp.sailmusic 1.0

import "../components"

Item {

    property string listName

    Column {

        anchors.fill: parent

        SectionHeader {
            id: radioListLabel

            text: listName

            font {
                pixelSize: Theme.fontSizeLarge
                italic: true
            }
        }

        SilicaListView {
            id: radioList
            anchors {
                topMargin: Theme.paddingLarge
            }

            width: parent.width
            height: parent.height - Theme.paddingLarge - radioListLabel.height

            spacing: 50

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

            delegate: RadioAlbum {
                width: radioList.width / 2
                height: radioList.height

                coverUrl: radioCoverUrl
                title: radioName
            }


            HorizontalScrollDecorator {
                flickable: radioList
            }
        }
    }
}
