cmake_minimum_required(VERSION 3.16)

project(reversi VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find all required Qt components.
# Ensure all components you link are listed here.
# Changed version to 6.7.3.
find_package(Qt6 6.5.3 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets Svg Xml) # Added Widgets

qt_standard_project_setup()

# Define the executable.
# Removed Android build files from the source list.
# MANUAL_FINALIZATION is used because we call qt_finalize_executable later.
qt_add_executable(reversi WIN32 MACOSX_BUNDLE
    cplayersettings.cpp cplayersettings.h
    creversigame.cpp creversigame.h
    creversiwidget.cpp creversiwidget.h # Typo fix: reversiwidgit -> reversiwidget
    main.cpp
    playersettings.ui
    reversiapp.cpp reversiapp.h reversiapp.ui
    android/AndroidManifest.xml android/build.gradle android/res/values/libs.xml android/gradle/wrapper/gradle-wrapper.jar android/gradle/wrapper/gradle-wrapper.properties android/gradle.properties android/gradlew android/gradlew.bat
    reversiapp.qrc
    MANUAL_FINALIZATION
    cgamesettings.h cgamesettings.cpp gamesettings.ui
    csettings.h csettings.cpp
    ceventfilter.h ceventfilter.cpp
    utility.h
    utility.cpp

)

# Link all required Qt libraries.
# Added Qt::Widgets and Qt::Svg (if truly used).
target_link_libraries(reversi PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets      # Necessary for .ui files
    Qt6::OpenGLWidgets
    Qt6::Svg          # Uncomment if you actually use the Svg module
    Qt6::Xml
)


# --- Android-specific Configuration ---
if(ANDROID)
    # Specify the Android package source directory.
    # Corrected usage of set_property (removed APPEND).
    set_property(TARGET reversi PROPERTY
        QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")

    # QT_ANDROID_MIN_SDK_VERSION:
    # Set to recommended minimum for Qt 6.5.3. (26 is actually but trying 24)
    set_property(TARGET reversi PROPERTY QT_ANDROID_MIN_SDK_VERSION 24)

    # QT_ANDROID_TARGET_SDK_VERSION:
    # Set to current recommended target.
    set_property(TARGET reversi PROPERTY QT_ANDROID_TARGET_SDK_VERSION 34)

    # QT_ANDROID_ABIS:
    # Enable all common ABIs for the App Bundle.
    set_property(TARGET reversi PROPERTY QT_ANDROID_ABIS "armeabi-v7a;arm64-v8a;x86;x86_64")

    # Manually finalize the executable for Android deployment.
    # This must be uncommented when using MANUAL_FINALIZATION.
    qt_finalize_executable(reversi)
endif()
# --- End Android-specific Configuration ---

# Install only on non-Android platforms (if applicable for desktop).
if(NOT ANDROID)
    install(TARGETS reversi
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION .
    )
endif()

# Deployment script generation (typically not needed for Android, remove if only Android)
qt_generate_deploy_app_script(
     TARGET reversi
     OUTPUT_SCRIPT deploy_script
     NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})
