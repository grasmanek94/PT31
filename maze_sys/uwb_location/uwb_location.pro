TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    uwblocationprovider.cpp \
    ../common/LocationProvider/LocationProvider.cpp

HEADERS += \
    uwblocationprovider.h \
    ../common/LocationProvider/LocationProvider.h \
    ../common/LocationProvider/ipcpos.h

