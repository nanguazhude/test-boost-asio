#TARGET = QtEventDispatcherSupport
#MODULE = eventdispatcher_support

#QT = core-private gui-private
#CONFIG += static internal_module

#DEFINES += QT_NO_CAST_FROM_ASCII
#PRECOMPILED_HEADER = ../../corelib/global/qt_pch.h

unix {
    DEFINES *= UNIX_QEVENT_DISPATCHER
    SOURCES += \
        $$PWD/qunixeventdispatcher.cpp \
        $$PWD/qgenericunixeventdispatcher.cpp

    HEADERS += \
        $$PWD/qunixeventdispatcher_qpa_p.h \
        $$PWD/qgenericunixeventdispatcher_p.h
} else {
    DEFINES *= WINDOWS_QEVENT_DISPATCHER
    SOURCES += \
        $$PWD/qwindowsguieventdispatcher.cpp

    HEADERS += \
        $$PWD/qwindowsguieventdispatcher_p.h
}

qtConfig(glib) {
    SOURCES += $$PWD/qeventdispatcher_glib.cpp
    HEADERS += $$PWD/qeventdispatcher_glib_p.h
#   QMAKE_USE_PRIVATE += glib
}

HEADERS += $$PWD/EventLoopDispatcherSelect.hpp \
    $$PWD/MainThreadEventDispatcher.hpp

SOURCES += \
    $$PWD/MainThreadEventDispatcher.cpp

#load(qt_module)
