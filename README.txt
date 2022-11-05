Introduction
============

This is Music client for Yandex.Music service. The
application is developed for Sailfish OS. It's available on
openrepos and on github, feel free to download latest
release and enjoy listening to music :)


Progress
==============

Implemented futures:

    Radio
        :Play any radio station.
        :List of suggested radio stations.

Planed feautures:

    Playlists
        :List of suggested/favourite playlists.
        :Operations on playlists(add, remove, etc)
    Playback
        :Like/dislike/skip tracks


For Developers
==============

Code structure
    .
    ├── qml
    │   ├── components
    │   ├── cover
    │   └── pages
    ├── src
    │   ├── models
    │   ├── units
    │   └── utils
    └── translations 
        ...

    src/models      Classes inherited from QAbstractListModel.
    src/units       Classes inherited from QObject.
    src/utils       Classes/methods used as helpers or
                    individual componenets.

    qml/componenets individual components.
    qml/pages       Qml code inside Page component.
