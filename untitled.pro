TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt
LIBS+= -lGL -lGLU -lX11
SOURCES += \
        loadFrame.cpp \
        main.cpp

unix:!macx: LIBS += -L$$PWD/../../../lib/x86_64-linux-gnu/ -lGLEW

INCLUDEPATH += $$PWD/../../../lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../lib/x86_64-linux-gnu

unix:!macx: LIBS += -L$$PWD/../../../lib/x86_64-linux-gnu/ -lglfw

INCLUDEPATH += $$PWD/../../../lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../lib/x86_64-linux-gnu

unix:!macx: LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/ -lavcodec

INCLUDEPATH += $$PWD/../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../usr/lib/x86_64-linux-gnu

unix:!macx: LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/ -lavformat

INCLUDEPATH += $$PWD/../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../usr/lib/x86_64-linux-gnu

unix:!macx: LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/ -lavutil

INCLUDEPATH += $$PWD/../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../usr/lib/x86_64-linux-gnu

HEADERS += \
    Shader.h \
    loadFrame.h

DISTFILES += \
    fragment.glsl \
    vertex.glsl












unix:!macx: LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/ -lswscale

INCLUDEPATH += $$PWD/../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../usr/lib/x86_64-linux-gnu
