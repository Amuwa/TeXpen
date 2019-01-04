#ifndef GINGER_H
#define GINGER_H
//Class Ginger
//Provide interface to polish sentenses provided by Ginger.com
//
//  author: Maxwell Wang (wangmc@ntu.edu.sg)
//


#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

#ifdef USE_WEBKIT
#include <QWebPage>
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>


///////////////////////////////
///
///JSON things
///
/// http://blog.vcreatelogic.com/2011/12/simple-json-parser-streamer-in-qt/
///
///

//struct JSONData;

struct JSONData
{
    QScriptEngine engine;
    QScriptValue parseFn;
    QScriptValue serializeFn;
};

struct period{
    int From;
    int To;
};

struct Correction{
    QList<period*> periods;
    QStringList suggestions;
};


class JSON
{
public:
    //static JSON& instance();
    QScriptValue CreateValue(const QVariant& value, QScriptEngine& engine);
    ~JSON();

    QVariantMap parse(const QString& string);// const;
    QString serialize(const QVariant& value);// const;

    JSON();

    QString toString(const QVariant &qv);
    QString toString(const QString& string);

    QVariantList findValuesOf(QString key, const QVariantMap& map);

    QString getMistakes(QString json, QString org="");

private:
    JSONData* d;
};


#include <QList>


///////////////////////////


class Ginger:public QObject
{
     Q_OBJECT
public:
    Ginger(QObject* para=NULL);
    ~Ginger();
    void test(QString sec);
    QNetworkReply* reply;
    QNetworkAccessManager* netmgr;
    //QWebPage * page;

signals:
    void result(QString rs);
public slots:
    void onDataReady();
};

#endif //use webkit

#endif // GINGER_H
