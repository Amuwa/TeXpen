#ifndef FILESAVER_H
#define FILESAVER_H
/*
 * A Thread to save file
 */

#include <QThread>
#include <QFile>
#include <QFileInfo>
class FileSaver:public QThread{
    Q_OBJECT
    QString filepath;
    QString content;

    QFileInfo info;
public:
    QFileInfo getFileInfo();
    FileSaver(QObject* p=0);
    void setPara(QString pth, QString cnt);
    void run();
    void save();
};


#endif // FILESAVER_H
