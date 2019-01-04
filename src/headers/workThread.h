#ifndef WORKTHREAD_H
#define WORKTHREAD_H
#include <QThread>

/*
class TeXWorkingThread :public QThread{
    Q_OBJECT

    //(void)

};
*/


class EqImage
{
public :
    QString eqsrc;
    QString base64image;
    EqImage()
    {
        eqsrc="";
        base64image="";
    }

};

class EqImageDict
{
    QList<EqImage*> database;
public:
    EqImageDict()
    {
        database.clear();
    }
    void AddEqImage(QString eq, QString b64img)
    {
        EqImage* em = new EqImage;
        em->eqsrc=eq;
        em->base64image=b64img;
        database.append(em);
    }
    QString imageOf(QString eq)
    {
        if(database.count()>0)
        {
            foreach(EqImage* em, database)
            {
                if(em->eqsrc == eq)
                {
                    return em->base64image;
                }
            }
        }

        return "";
    }
};

class EqRender : public QThread
{
    Q_OBJECT
    QString eqsrc;          //the compressed source of the equation
    QString base64image;
    QString tmpfiletitle;

    EqImageDict theDict;

public :
    explicit EqRender(QObject* p=0);
    void	run ();
signals:
    void showEqImage(QString b64img);
public slots:
    void startRendering();
    void deliverResult();
protected:
    void getTmpFileName();//set a tmp file name by md5 didgesting
    QString getWholeTeXSrc();//generate the whole tex file including the equation source
    void render();//call latex/... to render equation, the path of dvipng should be set

};



#endif // WORKTHREAD_H
