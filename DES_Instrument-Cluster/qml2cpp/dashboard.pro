TEMPLATE = app
TARGET = dashboard_new
INCLUDEPATH += .
QT += quick dbus
QT += dbus


SOURCES += \
    backend.cpp \
    main.cpp

RESOURCES +=

OTHER_FILES += \
    qml/main.qml

target.path = $$[QT_INSTALL_EXAMPLES]/quickcontrols/extras/dashboard_new
INSTALLS += target

DISTFILES +=

HEADERS += \
    backend.h
