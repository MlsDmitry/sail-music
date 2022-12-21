import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    SilicaFlickable {
        anchors.fill: parent


        PullDownMenu {
            MenuItem {
                text: "Add playlist"
            }
        }

        PageHeader {
            id: header

            title: "Choose playlists"

            anchors {

            }
        }

        SilicaListView {

            width: parent.width
            height: parent.height

            anchors {
                top: header.bottom
                margins: Theme.paddingLarge

            }
            model: ListModel {
                ListElement {
                    playlistTitle: "favorite"
                }
                ListElement {
                    playlistTitle: "my guitar"
                }
                ListElement {
                    playlistTitle: "best rock"
                }
            }

            delegate: Item {
                property bool isFavorite: false

                width: parent.width
                height: Theme.itemSizeMedium

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        isFavorite = !isFavorite;
                    }
                }

                Row {
                    anchors.fill: parent

                    IconButton {
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }

                        icon.source: isFavorite ? 'image://theme/icon-m-favorite-selected' : 'image://theme/icon-m-favorite'

                    }

                    Label  {
                        text: playlistTitle

                        anchors {
                            verticalCenter: parent.verticalCenter
                        }
                    }
                }
            }
        }
    }

}
