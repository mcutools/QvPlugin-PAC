project(QvPlugin-PAC)
cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON )
set(CMAKE_AUTOMOC ON )

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

set(QvPACPlugin_HDRS
    PACPlugin.hpp
    interface/QvPluginInterface.hpp
    )

set(QvPACPlugin_SRCS
    PACPlugin.cpp
)

set(QvPACPlugin_RSCS
    resx.qrc
    )

QT5_ADD_RESOURCES(RSCS ${QvPACPlugin_RSCS})

set ( QvPACPlugin_MOCS
        PACPlugin.hpp
	interface/QvPluginInterface.hpp
	)
QT5_WRAP_CPP(MOCS ${QvPACPlugin_MOCS})

add_library(QvPACPlugin MODULE ${QvPACPlugin_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS})
target_link_libraries(QvPACPlugin  Qt5::Core Qt5::Gui Qt5::Widgets)
