import QtQuick 2.0
import Sailfish.Silica 1.0
import org.k_bsp.sailmusic 1.0


Item {
    property var playlist

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
    }

    SilicaListView {
        id: tracksView
        anchors.fill: parent

        spacing: 50

        model: playlist

        delegate: TrackDelegate {
            width: tracksView.width
        }


        VerticalScrollDecorator {}
    }

}
