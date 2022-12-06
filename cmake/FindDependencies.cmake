list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake )

find_package(Qt5 COMPONENTS Core Widgets Gui Sql Concurrent PrintSupport REQUIRED)

if(MSVC)
    find_library(PTHREAD_LIB pthreadVC2.lib HINTS ENV PTHREAD_PATH REQUIRED)
    message(STATUS "Found PThread in ${PTHREAD_LIB}")
else()
    find_package(Threads REQUIRED)
endif()

include_directories( ${Qt5Core_INCLUDE_DIRS}         )
include_directories( ${Qt5Widgets_INCLUDE_DIRS}      )
include_directories( ${Qt5Gui_INCLUDE_DIRS}          )
include_directories( ${Qt5Sql_INCLUDE_DIRS}          )
include_directories( ${Qt5Concurrent_INCLUDE_DIRS}   )
include_directories( ${Qt5PrintSupport_INCLUDE_DIRS} )

set(QT_LIBS Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Sql Qt5::Concurrent Qt5::PrintSupport)
