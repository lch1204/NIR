QT -= gui

QT += core network
QT += serialport
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += *.cpp\
        kx_pult/kx_protocol.cpp \
        kx_pult/qkx_coeffs.cpp \
        kx_pult/qpiconfig.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += *.h\
    kx_pult/kx_protocol.h \
    kx_pult/qkx_coeffs.h \
    kx_pult/qpiconfig.h
