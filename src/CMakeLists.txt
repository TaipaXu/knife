cmake_minimum_required(VERSION 3.5)

find_package(QT NAMES Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt6LinguistTools)
find_package(Threads)

set(TS_FILES i18n/knife_zh_CN.ts)
qt6_create_translation(TRANS_FILE ${CMAKE_SOURCE_DIR} ${TS_FILES})
set(SOURCE_DIR ${CMAKE_SOURCE_DIR})
configure_file(assets.qrc assets.qrc @ONLY)
configure_file(./config.hpp.in ./config.hpp)

set(TARGET knife)
set(SOURCES
        main.cpp
        core.cpp
        core.hpp
        widgets/mainWindow.cpp
        widgets/mainWindow.hpp
        widgets/settingsDialog.cpp
        widgets/settingsDialog.hpp
        widgets/drop.cpp
        widgets/drop.hpp
        widgets/loading.cpp
        widgets/loading.hpp
        widgets/pdfTable.hpp
        widgets/combine.cpp
        widgets/combine.hpp
        widgets/split.cpp
        widgets/split.hpp
        widgets/splitMode.cpp
        widgets/splitMode.hpp
        widgets/combineRange.cpp
        widgets/combineRange.hpp
        widgets/pdfTable.hpp
        models/pdf.cpp
        models/pdf.hpp
        models/splitPdf.cpp
        models/splitPdf.hpp
        models/combinePdf.cpp
        models/combinePdf.hpp
        models/language.cpp
        models/language.hpp
        qmodels/controlModel.hpp
        qmodels/combine.cpp
        qmodels/combine.hpp
        qmodels/split.cpp
        qmodels/split.hpp
        qmodels/combineDelegate.cpp
        qmodels/combineDelegate.hpp
        qmodels/splitDelegate.cpp
        qmodels/splitDelegate.hpp
        persistence/settings.cpp
        persistence/settings.hpp
        utils/util.cpp
        utils/util.hpp
        pdf.cpp
        pdf.hpp
        ${TRANS_FILE}
        ${CMAKE_CURRENT_BINARY_DIR}/assets.qrc
)

message('name' ${PROJECT_NAME})

if(WIN32)
    message(STATUS "win")
    include_directories(../3rd/mupdf/include)
    link_directories(../3rd/mupdf/platform/win32/x64/Release)
endif()
if(UNIX)
    message(STATUS "unix")
    include_directories(../3rd/mupdf/include)
    link_directories(../3rd/mupdf/build/release)
endif()

if(APPLE)
    message('mac')
    set(CMAKE_MACOSX_BUNDLE ON)
endif()

if(${WIN32})
    add_executable(
        ${TARGET}
            WIN32
        ${SOURCES}
    )
else()
add_executable(
    ${TARGET}
    ${SOURCES}
)
endif()

if(APPLE)
    set_target_properties(${TARGET} PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "Knife"
        MACOSX_BUNDLE_GUI_IDENTIFIER taipaxu
        MACOSX_BUNDLE_BUNDLE_VERSION 1
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
        MACOSX_BUNDLE_ICON_FILE icon
    )

    configure_file(${PROJECT_SOURCE_DIR}/package/macos/icon.icns ${CMAKE_CURRENT_BINARY_DIR}/knife.app/Contents/Resources/icon.icns  COPYONLY)
endif()

if(UNIX)
    target_link_libraries(${TARGET}
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        ${CMAKE_THREAD_LIBS_INIT}
        optimized libmupdf.a
        optimized libmupdf-third.a
    )
endif()
if(WIN32)
    include_directories(../3rd/mupdf/include)
        target_link_libraries(${TARGET}
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        ${CMAKE_THREAD_LIBS_INIT}
        optimized libmupdf.lib
        optimized libthirdparty.lib
    )
endif()
