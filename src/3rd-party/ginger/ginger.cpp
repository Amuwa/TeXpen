#include "ginger.h"
#include <QDebug>
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>
#include <QWebFrame>


Ginger::Ginger(QObject *para):QObject(para)
{
    reply=NULL;
    netmgr=NULL;
    //page =NULL;// new QWebPage();
    //connect(page, SIGNAL(loadFinished(bool)),this,SLOT(onDataReady(bool)));
}
Ginger::~Ginger(){
    //netmgr->deleteLater();
}

void Ginger::test(QString sec){
    QString word = sec.replace(" ","+");
    QString rq= QString("http://services.gingersoftware.com/Ginger/correct/json/GingerTheTextFull?callback=&apiKey=6ae0c3a0-afdc-4532-a810-82ded0054236&lang=US&clientVersion=2.0&text=%1&_=13")
            .arg(word);
    netmgr= new QNetworkAccessManager(this);
    reply=netmgr->get(QNetworkRequest(QUrl(rq)));  qDebug()<<"\tconnecting server..."<<endl;
    connect(reply, SIGNAL(finished()),this,SLOT(onDataReady()));  qDebug()<<"\tconbine signals..."<<endl;
    connect(reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(onDataReady()));
}

void Ginger::onDataReady(){

    qDebug()<<"\nData arrived."<<endl;
    QString rs = reply->readAll();

    reply->deleteLater();
    netmgr->deleteLater();

    if(!rs.contains("English")){
        qDebug()<<"No data received."<<endl;
        return;
    }

    qDebug()<<rs<<endl;
    qDebug()<<"\nParsing JSON...."<<endl;
    JSON json;
    QVariantMap jsonObject = json.parse(rs);
    //QString serilized = json.serialize(jsonObject);
    //qDebug()<<serilized<<endl;

    /*
    QScriptEngine engine;
    QScriptValue json = engine.evaluate("var = " + rs);
    QScriptValueIterator it(json);

    while (it.hasNext()) {
        it.next();
        qDebug() << it.name() << ": " << it.value().toString();
    }
    */

    QMap<QString, QVariant>::const_iterator it= jsonObject.constBegin();
    while (it!=jsonObject.constEnd()){
        qDebug() << it.key() << ": " << it.value().toString() << endl;
        it++;
    }


    qDebug()<<"\nParsing JSON end."<<endl;

    emit(result(rs));
    deleteLater();
}

///////////////////////////
////  JSON Parsing
///
///
///

///
///// Parsing json strings
///QString jsonString = ....
///QVariantMap jsonObject = JSON::instance().parse(jsonString);

///// Serializing json objects
///QVariantMap jsonObject = ...
///QString jsonString = JSON::instance().serialize(jsonObject)
////

JSON::JSON()
{
    d = new JSONData;

    const QString script = "function parse_json(string) { return JSON.parse(string); }\n"
                           "function serialize_json(object) { return JSON.stringify(object); }";
    QScriptValue result = d->engine.evaluate(script);

    d->parseFn = d->engine.globalObject().property("parse_json");
    d->serializeFn = d->engine.globalObject().property("serialize_json");
}

JSON::~JSON(){
    //qDebug()<<"~JSON() is called."<<endl;
    delete d;
}

/*
JSON& JSON::instance()
{
    static JSON theInstance;
    return theInstance;
}*/

QVariantMap JSON::parse(const QString& string)// const
{
    QScriptValue result = d->parseFn.call(QScriptValue(), QScriptValueList() << QScriptValue(string));
    QVariantMap resultMap = result.toVariant().toMap();
    return resultMap;
}

QString JSON::serialize(const QVariant& value)// const
{
    QScriptValue arg = CreateValue(value, d->engine);
    QScriptValue result = d->serializeFn.call(QScriptValue(), QScriptValueList() << arg);
    QString resultString = result.toString();
    return resultString;
}


QString JSON::toString(const QString& string){
    QVariantMap map = parse(string);
    QString rs ="";// toString(map);

    qDebug()<<map<<"\n\n"<<endl;

    QVariantMap::const_iterator it = map.begin();
    QVariantMap::const_iterator end = map.end();
    while(it != end)
    {
        rs += it.key()+": \n";
        rs += toString( it.value())+"\n";

        it++;
    }


    qDebug()<<rs<<endl;
    return "";
}


QVariantList findValuesOf(QString key, const QVariant& value){

    QVariantList rs;
    rs.clear();

    if(value.type() == QVariant::Map)
    {
        QVariantMap map = value.toMap();
        QVariantMap::const_iterator it = map.begin();
        QVariantMap::const_iterator end = map.end();
        while(it != end)
        {
            if(it.key() == key){
                rs.append( it.value());
            }else{
                QVariantList sublist = findValuesOf(key, it.value());
                if(!sublist.isEmpty()){
                    foreach(QVariant qv, sublist){
                        rs.append(qv);
                    }
                }
            }
            ++it;
        }
    }else if(value.type() == QVariant::List)
    {
        QVariantList list = value.toList();
        for(int i=0; i<list.count(); i++){
                QVariantList sublist = findValuesOf(key, list.at(i));
                if(!sublist.isEmpty()){
                    foreach(QVariant qv, sublist){
                        rs.append(qv);
                    }
                }

        }
    }
    else {
        //return QVariant("NULL");
    }


    return rs;
}

QVariantList JSON::findValuesOf(QString key, const QVariantMap& map){
    QVariantList rs;
    rs.clear();

    QVariantMap::const_iterator it = map.begin();
    QVariantMap::const_iterator end = map.end();
    while(it != end)
    {
        if(it.key() == key){
            rs.append( it.value());
        }else{
            QVariantList sublist = ::findValuesOf(key, it.value());
            if(!sublist.isEmpty()){
                foreach(QVariant qv, sublist){
                    rs.append(qv);
                }
            }
        }
        it++;
    }

    return rs;
}



QString JSON::getMistakes(QString json, QString org){
    QVariantMap map = parse(json);
    QVariantList list = findValuesOf("Mistakes",map);

    //qDebug()<<"Mistakes: "<<list<<endl;

    QList<Correction*> mistakes;
    mistakes.clear();

    QString rs = "";
    foreach(QVariant qv, list){
        QVariantList fromlist = ::findValuesOf(QString("From"), qv);
        QVariantList  tolist = ::findValuesOf(QString("To"),qv);
        //QVariantList suglist = ::findValuesOf(QString("Text"),qv);

        int C = fromlist.count();
        if(C != tolist.count()){
            qDebug()<<"Error: getMistakes(): from and to mismatch.!"<<endl;
            return "Error: getMistakes(): from and to mismatch.!";
        }

        Correction* AP = new Correction;

        int lastto=-1;
        for(int i=0;i<C;i++){
            int from = fromlist.at(i).toInt();
            int to = tolist.at(i).toInt();
            if(lastto>=0 && from <= lastto+2){
                int xx = AP->periods.count()-1;
                AP->periods.at(xx)->To = to;
            }else{
                period* ap = new period;
                ap->From=from;
                ap->To=to;
                AP->periods.append(ap);
            }
            lastto = to;
        }

        mistakes.append(AP);

    }


    QVariantList slist = findValuesOf("Suggestions",map);

    rs+="suggestions:\n";
    int i=0;
    foreach(QVariant qv, slist){
        QVariantList suglist = ::findValuesOf(QString("Text"), qv);
        QStringList sugs;
        foreach(QVariant fv, suglist){
            sugs.append(fv.toString());
        }

        if(i<mistakes.count()){
            foreach(QString ss, sugs){
                mistakes.at(i)->suggestions.append(ss);
            }
        }
        i++;
    }

    rs = "";

    foreach(Correction* pd, mistakes){

        foreach (period* pi, pd->periods) {
            rs += QString("[%1 --> %2]").arg(pi->From).arg(pi->To)+"\n";
        }
        foreach(QString ss, pd->suggestions){
            rs+= QString("\t")+ ss+"\n";
        }
    }

    ///////  Now mistakes contains all the corrections
    ///
    ///
    QString orghtml="";
    QString sughtml="";
    QString sughtml2="";
    int LastTo=-1;
    foreach(Correction* pd, mistakes){
        int from =pd->periods.at(0)->From;
        int to =pd->periods.at(0)->To;

        QString keep = org.mid(LastTo+1, from-(LastTo+1));
        orghtml += keep;
        sughtml += keep;
        sughtml2+= keep;

        orghtml += "<font color=\"red\"><i>"+org.mid(from,to-from+1)+"</i></font>";
        if(pd->suggestions.count()>0){
            sughtml += "<font color=\"green\"><b>"+pd->suggestions.at(0)+"</b></font>";
        }
        if(pd->suggestions.count()>1){
            sughtml2+="<font color=\"green\"><b>"+pd->suggestions.at(1)+"</b></font>";
        }else{
            if(pd->suggestions.count()>0){
                sughtml2 += "<font color=\"green\"><b>"+pd->suggestions.at(0)+"</b></font>";
            }
        }
        LastTo = to;
    }

    if(LastTo>0 && LastTo<org.length()){
        orghtml += org.mid(LastTo+1, org.length());
        sughtml += org.mid(LastTo+1, org.length());
        sughtml2 += org.mid(LastTo+1, org.length());
    }


    rs = "<b><font color=\"red\">Original Text:</font></b><br>"+orghtml+"<br><br><b><font color=\"green\">Suggestion:</font></b><br>"+sughtml+"<br>";
    if(sughtml2 != sughtml){
        rs += "<br><b><font color=\"green\">Suggestion 2:</font></b><br>"+sughtml2+"<br>";
    }

    qDebug()<<rs<<endl;

    return rs;
}


QString JSON::toString(const QVariant &value){
    QString rs = "";
    if(value.type() == QVariant::Map)
    {
        QVariantMap map = value.toMap();
        QVariantMap::const_iterator it = map.begin();
        QVariantMap::const_iterator end = map.end();
        while(it != end)
        {
            rs += "<"+it.key()+">";
            rs += "  "+toString( it.value());
            rs += "</"+it.key()+">"+"\n";

            ++it;
        }
    }else if(value.type() == QVariant::List)
    {
        QVariantList list = value.toList();
        for(int i=0; i<list.count(); i++){
            rs += "  "+ toString(list.at(i))+"\n";
        }
    }
    else {

        bool b =true;
        int k=0;
        unsigned int uk=0;
        switch(value.type())
        {
        case QVariant::String:
            rs += (value.toString());break;
        case QVariant::Int:
            k=value.toInt();
            rs += QString("%1").arg(k);break;
        case QVariant::UInt:
            uk = value.toUInt();
            rs += QString("%1").arg(uk);break;
        case QVariant::Bool:
            b =value.toBool();
            if(b){rs+="TRUE";}else{rs+="FALSE";}
            break;
        case QVariant::ByteArray:
            rs += (QLatin1String(value.toByteArray()));
            break;
        case QVariant::Double:
            rs += ((qsreal)value.toDouble());
            break;
        default:
            break;
        }

    }
    return rs;
}

QScriptValue JSON::CreateValue(const QVariant& value, QScriptEngine& engine)
{
    qDebug()<<"JSON::CreateValue()..."<<endl;
    if(value.type() == QVariant::Map)
    {
        QScriptValue obj = engine.newObject();

        QVariantMap map = value.toMap();
        QVariantMap::const_iterator it = map.begin();
        QVariantMap::const_iterator end = map.end();
        while(it != end)
        {
            obj.setProperty( it.key(), CreateValue(it.value(), engine) );
            qDebug()<<"Key "<<it.key()<<" is created."<<endl;
            ++it;
        }

        return obj;
    }

    if(value.type() == QVariant::List)
    {
        QVariantList list = value.toList();
        QScriptValue array = engine.newArray(list.length());
        for(int i=0; i<list.count(); i++){
            array.setProperty(i, CreateValue(list.at(i),engine));
            qDebug()<<"something is created."<<endl;
        }

        return array;
    }

    switch(value.type())
    {
    case QVariant::String:
        return QScriptValue(value.toString());
    case QVariant::Int:
        return QScriptValue(value.toInt());
    case QVariant::UInt:
        return QScriptValue(value.toUInt());
    case QVariant::Bool:
        return QScriptValue(value.toBool());
    case QVariant::ByteArray:
        return QScriptValue(QLatin1String(value.toByteArray()));
    case QVariant::Double:
        return QScriptValue((qsreal)value.toDouble());
    default:
        break;
    }

    qDebug()<<"Judge value type...."<<endl;

    if(value.isNull())
        return QScriptValue(QScriptValue::NullValue);

    return engine.newVariant(value);
}




