// This project is based on textpad-editor v0.5.1
// The following text is in the orignal source
//
// Copyright (C) 2013 Alex-97
// This file is part of textpad-editor.
//
// textpad-editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// textpad-editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with textpad-editor.  If not, see <http://www.gnu.org/licenses/>.

#include "QApplication"
#include "headers/mainwindow.h"
#include <QDebug>
#include <exception>


int main(int argc, char *argv[]) {
    QApplication Texpen(argc, argv);

    Texpen.setApplicationName("TeXpen");
    Texpen.setApplicationVersion("0.7.3-2");


    QWebSettings::globalSettings ()->setAttribute(QWebSettings::JavascriptEnabled,true);
    QWebSettings::globalSettings ()->setAttribute(QWebSettings::JavascriptCanOpenWindows,false);
    QWebSettings::globalSettings ()->setAttribute(QWebSettings::JavascriptCanAccessClipboard,true);

    QWebSettings::globalSettings ()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled,true);
    QWebSettings::globalSettings ()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled,true);
    QWebSettings::globalSettings ()->setAttribute(QWebSettings::LocalStorageEnabled,true);
    QWebSettings::globalSettings ()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls,true);
    QWebSettings::globalSettings ()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls,true);
    QWebSettings::globalSettings ()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled,true);
    QWebSettings::globalSettings ()->setAttribute(QWebSettings::PluginsEnabled,true);


    MainWindow Window;
    if(argc > 1){
        QString fn=QString::fromLocal8Bit(argv[1]);
                //fromUtf8(argv[1]);
        Window.OpenFile(fn);
    }
    Window.show();

    try{
        return Texpen.exec();
    }catch(...){
        qDebug()<<"Something goes wrong !!!"<<endl;
        return -1;
    }
}
