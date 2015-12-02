TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include/libusb-1.0
LIBS +=-lusb-1.0 -lpixyusb -lboost_thread -lboost_system -lboost_chrono -lpthread

SOURCES += main.cpp \
    pixylocationprovider.cpp \
    ipcpos.cpp

HEADERS += \
    pixylocationprovider.h \
    ipcpos.h

