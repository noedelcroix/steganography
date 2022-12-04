TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c \
        scripts/lsb/lsb.c \
        scripts/zwnj-zws/zwnj-zws.c

HEADERS += \
        scripts/lsb/lsb.h \
        scripts/zwnj-zws/zwnj-zws.h
