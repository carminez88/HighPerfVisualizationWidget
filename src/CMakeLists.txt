cmake_minimum_required( VERSION 3.20 )

project( HighPerfVisualizationWidget_App )

include( ../cmake/CompileOptions.cmake   )
include( ../cmake/CommonOptions.cmake    )
include( ../cmake/FindDependencies.cmake )

if(MSVC)
    add_compile_definitions(_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING)
    add_compile_definitions(_ENABLE_EXTENDED_ALIGNED_STORAGE)
endif()

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )
set( CMAKE_AUTORCC ON )

#### BUILD TARGET ####
set(TARGET_NAME "HighPerfVisualizationWidget")

add_executable( ${TARGET_NAME} main.cpp imageviewbase.h imageviewbase.cpp imageview.h imageview.cpp spammer.h spammer.cpp)
target_link_libraries ( ${TARGET_NAME} PRIVATE ${QT_LIBS} )

if(MSVC)
    target_link_libraries(${TARGET_NAME} PRIVATE ${PTHREAD_LIB} ws2_32 User32 shell32 Advapi32)
elseif(LINUX)
    target_link_libraries(${TARGET_NAME} PRIVATE Threads::Threads ${CMAKE_DL_LIBS} rt)
endif()
