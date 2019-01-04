#include "headers/mainwindow.h"
#include "headers/qtexedit.h"
#include <QString>
#include <QFile>

QStringList AWL;

void setupAWL(){
    if(AWL.isEmpty()){
        QFile file (":/AWL/awl.txt");
        file.open(QIODevice::ReadOnly);
        QString word = QString::fromUtf8(file.readLine());
        while((!word.isEmpty())&& word !=" "){
            if(word.contains("\n")){
                word.replace("\n","");
            }
            AWL.append(word);
            word = QString::fromUtf8(file.readLine());
        }
        file.close();
    }
}

