import QtQuick 2.0
import Sailfish.Silica 1.0
import org.k_bsp.sailmusic 1.0

import "../components"

Item {

    property var radio
    property string listName: ""

//    anchors.fill: parent

    //    height: radioLabel.contentHeight + radioList.height
    //    height: radioListLabel.height + radioList.height

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
                    coverUrl: "https://avatars.yandex.net/get-music-misc/70850/rotor-personal-station-icon/400x400"
                    title: "My Vibe"
                    radioColor: "#2AA75B"
                }

                ListElement {
                    station: "local:moscow"
                    coverUrl: "https://avatars.yandex.net/get-music-misc/29541/rotor-local-moscow-icon/400x400"
                    title: "Popular in Moscow"
                    radioColor: "black"
                }
                ListElement {
                    station: "genre:doommetal"
                    coverUrl: "https://avatars.yandex.net/get-music-misc/34161/rotor-genre-metal-icon/400x400"
                    title: "Doom metal"
                    radioColor: "black"
                }
                ListElement {
                    station: "genre:bassgenre"
                    coverUrl: "https://avatars.yandex.net/get-music-misc/40584/rotor-genre-dubstep-icon/400x400"
                    title: "Bass"
                    radioColor: "black"
                }
                ListElement {
                    station: "genre:rap"
                    coverUrl: "https://avatars.yandex.net/get-music-misc/34161/rotor-genre-rusrap-icon/400x400"
                    title: "Russian Rap"
                    radioColor: "black"
                }
            }

            delegate: AlbumElement {
                width: radioList.width / 2
                height: radioList.height

                radioCoverUrl: coverUrl
                radioTitle: title

            }


            HorizontalScrollDecorator {
                flickable: radioList
            }
        }
    }
}
