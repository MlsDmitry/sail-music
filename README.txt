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

** Project structure **

./src
├── Adapters
├── Models
├── Radio
├── Services
└── Utils

src/Radio       Directory is named behind screen name responsible for
                Radio functionality. This folder contains code which is
                not used in other screens. 

src/Models      Classes which are inherited from QObject or
                QAbstractListModel and others. Models in this directory
                are shared with other screens that's why they are in
                root Models/ folder.

src/Services    Service classes for models from root Models/ folder.

src/Adapters    Adapter classes for models from root Models/ folder.

TIPS
    * All screens should have their own folder and named concisely.

    * Screen folders may have own Models/Services/Adapters folders if
      it has more than one class.

    * Models which are shared and used in other screens should go to
      root Models/ folder. Same idea is applied to Adapters and Services
      folder.

** Abstractions **

Adapters

Idea:
An abstraction layer responsible for parsing data from one type and
converting it to another.

Description: 
In this project I use json_dto library which allows to define C struct
representation of JSON data. The idea is simple. Define struct representing
JSON data from response or for request and then call
json_dto::from_json<T>(jsonStringToParse). Key things to consider when
writing own Adapter:

    * Adapter should hide all type conversions from Qt to standart
      types(json_dto only works with std types) inside it. 

    * Adapter should work with raw data QJsonValue, QByteArray, etc.

    * Define structs required for json_dto in Adapter header file.

    * If you need to return a lot of data, think about defining a struct
      for it instead of returning QList/QMap (Example ->
      https://github.com/MlsDmitry/sail-music/blob/master/src/Adapters/TrackAdapter.h#L78)

Example:
Check out src/Adapters/TrackAdapter.{h, cpp} files.


Services

Idea:
An abstraction level responsible for network requests.

Description:
I use Services as a way to shrink Model's code and hide some
implementation details from Model class. Key things to consider when
writing own Service:

    * Append "request" to the start of functions which are exported from
      class.

    * Inherit from QOjbect to be able to receive and send signals.

    * Have a private _transport field containg instance of ApiRequest.

Example:
Check out src/Services/TrackService.{h, cpp} files.
