# Copyright of Current Edition(TeXPen): Copyright (C) 2013  Amuwa King, <phd.wang.mch@gmail.com>
# Original TextPad: Copyright (C) 2013  Alex S. <alex_sr97@outlook.com>
# See License.txt file that comes with this distribution
QT       += core gui network quick qml
QT       += printsupport
TARGET    = texpen
TEMPLATE  = app

greaterThan(QT_MAJOR_VERSION, 4): { #Qt5
    QT += widgets
    DEFINES += QT5SDK

    greaterThan(QT_MINOR_VERSION,4):{ # >= Qt 5.5
        QT += webenginewidgets
        DEFINES += USE_QT_WEB_ENGINE
    }else:{
        QT += webkitwidgets script
        DEFINES += USE_WEBKIT
    }
}

lessThan(QT_MAJOR_VERSION, 5){
    QT += script webkitwidgets
    DEFINES += USE_WEBKIT
}



unix:!macx {
    INCLUDEPATH += /usr/include/hunspell   #/path/to/hunspell/include
    LIBS += /usr/lib/x86_64-linux-gnu/libhunspell-1.6.a  #/path/to/hunspell/lib/libhunspell-1.2.a
}


SOURCES += \
    sources/mainwindow.cpp \
    sources/main.cpp \
    sources/searchdialog.cpp \
    sources/widgetcreator.cpp \
    sources/colorschemes.cpp \
    sources/document.cpp \
    sources/syntaxhighlighter.cpp \
    sources/linenumbers.cpp \
    sources/settings.cpp \
    sources/structure.cpp \
    sources/equationviewer.cpp \
    sources/runcommand.cpp \
    sources/autocompletion.cpp \
    sources/runthread.cpp \
    sources/cmdconfigdialog.cpp \
    sources/qtexedit.cpp \
    3rd-party/ginger/ginger.cpp \
    sources/cstructuredata.cpp \
    sources/WebView.cpp \
    sources/AWLdict.cpp \
    sources/workThread.cpp \
    sources/FileSaver.cpp \
    3rd-party/AutoWriter/autowriter.cpp \
    3rd-party/cookie/autosaver.cpp \
    3rd-party/cookie/cookiejar.cpp \
    sources/shortcmds.cpp \
    headers/qqtexedit.cpp

HEADERS += \
    headers/mainwindow.h \
    headers/searchdialog.h \
    headers/highlighter.h  \
    headers/runthread.h \
    headers/cmdconfigdialog.h \
    headers/qtexedit.h \
    headers/workThread.h \
    3rd-party/ginger/ginger.h \
    sources/cstructuredata.h \
    headers/WebView.h \
    headers/FileSaver.h \
    3rd-party/AutoWriter/autowriter.h \
    3rd-party/cookie/autosaver.h \
    3rd-party/cookie/cookiejar.h \
    headers/qqtexedit.h

RESOURCES += \
    images/images.qrc \
    3rd-party/dicts.qrc \
    Templates/templates.qrc \
    3rd-party/pdf-js.qrc \
    3rd-party/qwl.qrc \
    3rd-party/mathjax.qrc \
    quick.qrc

FORMS += \
    ui/cmdconfigdialog.ui

win32{

    SOURCES += \
    3rd-party/hunspell/utf_info.cxx \
    3rd-party/hunspell/suggestmgr.cxx \
    3rd-party/hunspell/phonet.cxx \
    3rd-party/hunspell/hunzip.cxx \
    3rd-party/hunspell/hunspell.cxx \
    3rd-party/hunspell/hashmgr.cxx \
    3rd-party/hunspell/filemgr.cxx \
    3rd-party/hunspell/dictmgr.cxx \
    3rd-party/hunspell/csutil.cxx \
    3rd-party/hunspell/affixmgr.cxx \
    3rd-party/hunspell/affentry.cxx

    HEADERS += \
    3rd-party/hunspell/w_char.hxx \
    3rd-party/hunspell/suggestmgr.hxx \
    3rd-party/hunspell/phonet.hxx \
    3rd-party/hunspell/langnum.hxx \
    3rd-party/hunspell/hunzip.hxx \
    3rd-party/hunspell/hunspell.hxx \
    3rd-party/hunspell/hunspell.h \
    3rd-party/hunspell/htypes.hxx \
    3rd-party/hunspell/hashmgr.hxx \
    3rd-party/hunspell/filemgr.hxx \
    3rd-party/hunspell/dictmgr.hxx \
    3rd-party/hunspell/csutil.hxx \
    3rd-party/hunspell/baseaffix.hxx \
    3rd-party/hunspell/atypes.hxx \
    3rd-party/hunspell/affixmgr.hxx \
    3rd-party/hunspell/affentry.hxx


    RC_FILE = icon.rc


}else : macx {

    SOURCES += \
    3rd-party/hunspell/utf_info.cxx \
    3rd-party/hunspell/suggestmgr.cxx \
    3rd-party/hunspell/phonet.cxx \
    3rd-party/hunspell/hunzip.cxx \
    3rd-party/hunspell/hunspell.cxx \
    3rd-party/hunspell/hashmgr.cxx \
    3rd-party/hunspell/filemgr.cxx \
    3rd-party/hunspell/dictmgr.cxx \
    3rd-party/hunspell/csutil.cxx \
    3rd-party/hunspell/affixmgr.cxx \
    3rd-party/hunspell/affentry.cxx

    HEADERS += \
    3rd-party/hunspell/w_char.hxx \
    3rd-party/hunspell/suggestmgr.hxx \
    3rd-party/hunspell/phonet.hxx \
    3rd-party/hunspell/langnum.hxx \
    3rd-party/hunspell/hunzip.hxx \
    3rd-party/hunspell/hunspell.hxx \
    3rd-party/hunspell/hunspell.h \
    3rd-party/hunspell/htypes.hxx \
    3rd-party/hunspell/hashmgr.hxx \
    3rd-party/hunspell/filemgr.hxx \
    3rd-party/hunspell/dictmgr.hxx \
    3rd-party/hunspell/csutil.hxx \
    3rd-party/hunspell/baseaffix.hxx \
    3rd-party/hunspell/atypes.hxx \
    3rd-party/hunspell/affixmgr.hxx \
    3rd-party/hunspell/affentry.hxx


    ICON = images/icons/texpen.icns
    greaterThan(QT_MAJOR_VERSION, 4): {
        QMAKE_MAC_SDK = macosx
    }else: {
        #QMAKE_MAC_SDK += "/Xcode3.2.3/SDKs/MacOSX10.6.sdk"
        #:/Xcode3.2.3/usr/bin
        QMAKE_MAC_SDK += "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/"
    }
}

OTHER_FILES +=

DISTFILES += \
    sources/ExpMainWindow.qml \
    sources/SettingPage.qml

