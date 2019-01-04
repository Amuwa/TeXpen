#include "../headers/FileSaver.h"
#include <QFile>
#include <QTextStream>

FileSaver::FileSaver(QObject* p):
    QThread(p)
{
    filepath = "";
    content="";
}
void FileSaver::run()
{
    save();
}
QFileInfo FileSaver::getFileInfo(){
    QFile File(filepath);
    info = QFileInfo(File);
    return info;
}

void FileSaver::save()
{
    if(filepath!="")
    {
        QFile File(filepath);
        info = QFileInfo(File);

        if (File.open(QFile::WriteOnly | QFile::Text))
        {
            QTextStream Save(&File);
            Save.setCodec("UTF-8");//setAutoDetectUnicode(true);//setCodec("UTF-8");
            Save << content;
            File.close();
        }
    }
}
void FileSaver::setPara(QString pth, QString cnt)
{
    filepath = pth;
    content = cnt;
}
