project(sail-music CXX)
cmake_minimum_required(VERSION 3.5)

find_package (Qt5 COMPONENTS Core Network Qml Gui Quick Multimedia Xml REQUIRED)

include(FindPkgConfig)
pkg_search_module(SAILFISH sailfishapp REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE CPP_SOURCES "src/*.cpp")

add_executable(sail-music ${CPP_SOURCES})

option(SANITIZER "Build with sanitizer" none)
if(SANITIZER STREQUAL "address")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    target_compile_options(sail-music PRIVATE -fsanitize=address)
    target_link_options(sail-music PRIVATE -fsanitize=address)
elseif(SANITIZER STREQUAL "undefined")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
elseif(SANITIZER STREQUAL "thread")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
else()
    set(SANITIZER "none")
endif()

set(JSON_DTO_TEST OFF)
set(JSON_DTO_SAMPLE OFF)
set(JSON_DTO_INSTALL OFF)

add_subdirectory("../json_dto/dev" "build")

target_include_directories(sail-music PRIVATE "src" "../json_dto/dev/rapidjson/include" "../json_dto/dev")

target_compile_definitions(sail-music PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_include_directories(sail-music PRIVATE
    $<BUILD_INTERFACE:
    ${SAILFISH_INCLUDE_DIRS}
>)
target_link_libraries(sail-music
    Qt5::Quick
    Qt5::Multimedia
    Qt5::Xml
    ${SAILFISH_LDFLAGS}
    json-dto
)

install(TARGETS sail-music
    RUNTIME DESTINATION bin
)
install(DIRECTORY qml
    DESTINATION share/sail-music
)
install(DIRECTORY translations
    DESTINATION share/sail-music
    FILES_MATCHING PATTERN "*.qm"
)
install(FILES sail-music.desktop
    DESTINATION share/applications
)
install(FILES icons/86x86/sail-music.png
    DESTINATION share/icons/hicolor/86x86/apps
)
install(FILES icons/108x108/sail-music.png
    DESTINATION share/icons/hicolor/108x108/apps
)
install(FILES icons/128x128/sail-music.png
    DESTINATION share/icons/hicolor/128x128/apps
)
install(FILES icons/172x172/sail-music.png
    DESTINATION share/icons/hicolor/172x172/apps
)

# Get the other files reachable from the project tree in Qt Creator
FILE(GLOB TsFiles "translations/*.ts")
add_custom_target(distfiles
    SOURCES
        sail-music.desktop
        qml/sail-music.qml
        qml/cover/CoverPage.qml
        qml/pages/FirstPage.qml
        qml/pages/SecondPage.qml
        rpm/sail-music.changes.in
        rpm/sail-music.changes.run.in
        rpm/sail-music.spec
        rpm/sail-music.yaml
        ${TsFiles})

# Tell Qt Creator where the application executable(s) would be located on the
# device.
#
# It is not necessary to list other deployables than executables (runtime
# targets) here. The deployment process of Sailfish OS projects is opaque to
# Qt Creator and the information contained in QtCreatorDeployment.txt is only
# used to locate the executable associated with the active run configuration
# on the device in order to run it.
#
# Search the Qt Creator Manual to learn about the QtCreatorDeployment.txt file
# format.
file(WRITE "${CMAKE_BINARY_DIR}/QtCreatorDeployment.txt"
    "${CMAKE_INSTALL_PREFIX}
${CMAKE_BINARY_DIR}/sail-music:bin
")

message(STATUS " Sanitizer ............................... ${SANITIZER}")
