TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/main.cxx \
    src/PixyLocationProvider.cxx \
    src/RobotManager.cxx \
    src/TaskManager.cxx \
    src/UWBLocationProvider.cxx
    src/main.cxx \
    src/PixyLocationProvider.cxx \
    src/RobotManager.cxx \
    src/TaskManager.cxx \
    src/UWBLocationProvider.cxx \

HEADERS += \
    include/LocationProvider.hxx \
    src/PixyLocationProvider.hxx \
    src/RobotManager.hxx \
    src/TaskManager.hxx \
    src/UWBLocationProvider.hxx

