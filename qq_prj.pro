QT += core gui network sql widgets
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32 {
  DEFINES += _WIN32_WINNT=0x0601
  LIBS += -lws2_32
}

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    TcpMgr.cpp \
    chat.cpp \
    chat_client.cpp \
    main.cpp \
    mainwindow.cpp \
    search_friend.cpp \
    signin.cpp \
    usersql.cpp

HEADERS += \
    TcpMgr.h \
    chat.h \
    chat_client.h \
    global.h \
    mainwindow.h \
    search_friend.h \
    signin.h \
    singleton.h \
    usersql.h

FORMS += \
    chat.ui \
    mainwindow.ui \
    search_friend.ui \
    signin.ui

RESOURCES += \
    signin1.qrc

# 移除这些不必要的行
# DISTFILES += \
#     C:/Users/DELL/Desktop/qq.jpg \
#     Networkerror.qml \
#     NetworkerrorForm.ui.qml \
#     lib/qq.jpg
