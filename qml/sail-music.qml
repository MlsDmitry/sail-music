import QtQuick 2.0
import Sailfish.Silica 1.0
import QtQml.StateMachine 1.0 as DSM
import org.k_bsp.sailmusic 1.0

import "pages"

ApplicationWindow {
    initialPage: Component { MainPage {} }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    DSM.StateMachine {
        id: yaclientSM

        initialState: s1
        running: true

        property var playlistSM

        property bool ready: sReadyToPlay.active
        property bool playing: stPlaying.active
        property bool paused: stPaused.active

        property bool waitingPlay: false
        property int index: 0

        signal loading();
        signal change_playlist();
        signal playlist_ready();
        signal track_end();
        signal play();
        signal pause();

        DSM.State {
            id: s1

            DSM.SignalTransition {
                targetState: sPlaylistInitialized

                signal: yaclientSM.change_playlist
            }

        }

        DSM.State {
            id: sPlaylistInitialized


            DSM.SignalTransition {
                targetState: sLoading

                signal: yaclientSM.loading

            }

            onEntered: {
                console.log("sPlaylistInitialized");
                yaclientSM.playlistSM.init();
            }
        }

        DSM.State {
            id: sLoading

            DSM.SignalTransition {
                targetState: sReadyToPlay

                signal: yaclientSM.playlist_ready
            }

            onEntered: {
                console.log("sLoading");
            }
        }


        DSM.State {
            id: sReadyToPlay

            DSM.SignalTransition {
                targetState: stPlaying

                signal: yaclientSM.play
            }

            DSM.SignalTransition {
                targetState: sPlaylistInitialized

                signal: yaclientSM.change_playlist
            }

            DSM.SignalTransition {
                targetState: sLoading
                signal: yaclientSM.loading
            }

            onEntered: {
                console.log("sReadyToPlay");
                if (yaclientSM.index !== 0) {
                    YaClient.currentPlaylist.setIndex(yaclientSM.index);
                    yaclientSM.index = 0;
                }

            }
        }

        DSM.State {
            id: stPlaying

            onEntered: {

                var track = YaClient.currentPlaylist.currentTrack;

                if (YaClient.player.state === YaClient.PausedState) {
                    console.log("Continuing play.");
                    YaClient.player.play();
                } else {
                    console.log("Playing...");
                    YaClient.play(track.getDirectDownloadLink(track.getMaxBitrateAvailable()));
                }
            }

            DSM.SignalTransition {
                targetState: sLoading
                signal: yaclientSM.loading
            }

            DSM.SignalTransition {
                targetState: stTrackEnd
                signal: yaclientSM.track_end
            }

            DSM.SignalTransition {
                targetState: stPaused
                signal: yaclientSM.pause
            }

            DSM.SignalTransition {
                targetState: sPlaylistInitialized
                signal: yaclientSM.change_playlist
            }
        }

        DSM.State {
            id: stPaused

            DSM.SignalTransition {
                targetState: stPlaying
                signal: yaclientSM.play
            }

            DSM.SignalTransition {
                targetState: sPlaylistInitialized
                signal: yaclientSM.change_playlist
            }

            DSM.SignalTransition {
                targetState: sLoading
                signal: yaclientSM.loading
            }

            onEntered: {
                console.log("Paused track");
                YaClient.player.pause();
            }

        }


        DSM.State {
            id: stTrackEnd

            onEntered: {
                console.log("End of track");
                var nextTrackIndex = YaClient.currentPlaylist.currentIndex + 1
                yaclientSM.waitingPlay = true;
                change_song(1);
                yaclientSM.playlist_ready();
            }

            DSM.SignalTransition {
                targetState: sLoading
                signal: yaclientSM.loading
            }

            DSM.SignalTransition {
                targetState: sPlaylistInitialized
                signal: yaclientSM.change_playlist
            }
        }

        function change_song(index) {
            var pl = YaClient.currentPlaylist;
            YaClient.player.stop();

            if (pl.currentIndex + index < 0)
                return;

            if (pl.currentIndex + index >= pl.rowCount()) {
                yaclientSM.playlistSM.load_tracks();
                yaclientSM.index = pl.currentIndex + index;
            } else {
                YaClient.currentPlaylist.setIndex(pl.currentIndex + index);

            }
        }
    }

    Connections {
        target: YaClient

        onPlaylistUpdated: {
            yaclientSM.change_playlist();
        }
    }


    Connections {
        target: YaClient.player
        onMediaStatusChanged: {
            if (status === YaClient.EndOfMedia && radio.currentTrack.duration <= radio.currentTrack.totalPlayedMs) {
                yaclientSM.track_end();
            }
        }
    }

}
