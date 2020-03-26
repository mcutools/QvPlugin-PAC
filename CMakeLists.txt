project(QvPlugin-PAC)
cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON )
set(CMAKE_AUTOMOC ON )
set(CMAKE_AUTOUIC ON )

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)

set(QvPACPlugin_HDRS
    PACPlugin.hpp
    common/HTTPRequestHelper.hpp
    ui/QvPACPluginSettingsWidget.hpp
    interface/QvPluginInterface.hpp
    pac-core/QvPACHandler.hpp
    3rdparty/cpp-httplib/httplib.h
    )

set(QvPACPlugin_SRCS
    PACPlugin.cpp
    common/HTTPRequestHelper.cpp
    ui/QvPACPluginSettingsWidget.cpp
    pac-core/QvGFWPACConverter.cpp
    pac-core/QvPACHandler.cpp
)

set(UIS ${UIS}
    ui/QvPACPluginSettingsWidget.ui
    )

QT5_ADD_RESOURCES(RSCS resx.qrc)

set ( QvPACPlugin_MOCS
        PACPlugin.hpp
	interface/QvPluginInterface.hpp
	)
QT5_WRAP_CPP(MOCS ${QvPACPlugin_MOCS})

add_library(QvPACPlugin MODULE ${QvPACPlugin_SRCS} ${QvPACPlugin_HDRS} ${UIS} ${RSCS} ${TRS} ${MOCS})
target_link_libraries(QvPACPlugin  Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)
