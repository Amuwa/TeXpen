#include "cstructuredata.h"
#include <QStringList>
#include <QDebug>
#include <QTime>

CStructureData::CStructureData()
{

    int c = AllLeafs.size();
    for(int i=0;i<c;i++){
        CLeaf* p = AllLeafs.at(i);
        delete p;
    }
    AllLeafs.clear();
}


QTreeWidgetItem* CStructureData::createWidgetItem(QList<QTreeWidgetItem*>* itemlist, CLeaf* aleaf){
    if(itemlist==NULL){
        return NULL;
    }
    QTreeWidgetItem* aItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("%1").arg( aleaf->title )));
    aItem->setData(0,Qt::UserRole,aleaf->position);
    itemlist->append(aItem);

    if(aleaf->children.count()>0){
        for(int c=0;c<aleaf->children.count();c++){
            CLeaf* child = aleaf->children.at(c);
            QTreeWidgetItem* sItem = createWidgetItem(itemlist, child);
            aItem->addChild(sItem);
        }
    }
    return aItem;
}

void CStructureData::BuildQTree(QList<QTreeWidgetItem*>* itemlist){
    itemlist->clear();
    for(int i=0;i<AllLeafs.count();i++){
        if(AllLeafs.at(i)->parent == NULL){
            createWidgetItem(itemlist,AllLeafs.at(i));
        }
    }
}



bool CStructureData::isHigherLevel(QString lv1, QString lv0){
    if(lv1.toLower()== "chapter"){
        if(lv0.toLower() != "chapter"){
            return true;//lv1 higher than lv0
        }else{
            return false;
        }
    }else {
        if(lv0.toLower() == "chapter"){
            return false; //lv1 equal to or less than lv0; but no higher than lv0
        }else {
            if(lv1.length() < lv0.length() ){
                return true;
            }else {
                return false;
            }
        }
    }
    return false;
}

////////
/// \brief CStructureData::guessParent  try to get the parent of a coming node
/// \param level
/// \param position
/// \return
///
CLeaf* CStructureData::guessParent(QString level, int position){
    CLeaf* rs = NULL;
    int nearestPos = -1;
    int L = AllLeafs.count();
    for(int i=0;i<L;i++){
        QString curLv =AllLeafs.at(i)->level ;
        int curPos = AllLeafs.at(i)->position;
        if( isHigherLevel(curLv, level) &&  curPos < position && curPos > nearestPos ){
            rs = AllLeafs.at(i);
            nearestPos = curPos;
        }
    }
    return rs;
}


void CStructureData::constructTree(){
    int C=AllLeafs.count();
    for(int i=0;i<C;i++){
        QString lev = AllLeafs.at(i)->level;
        int pos = AllLeafs.at(i)->position;
        CLeaf* parent = guessParent(lev,pos);
        if(parent != NULL){
            parent->AddChild(AllLeafs.at(i));
        }
    }
}


void CStructureData::Clear(){
    for(int i=0;i<AllLeafs.count();i++){
        CLeaf* leaf = AllLeafs.at(i);
        delete leaf;
    }
    AllLeafs.clear();
}

extern bool isTexComment(QString& ct, int pos);
extern QString TextInBrace(QString& doc, int pos, QChar begin='{', QChar end='}');


void CStructureData::getLeaf(QString& cntx, QString tag){
    int tagpos = -1;
    QStringList tags ;
    tags.append("\\"+tag+"{");
    tags.append("\\"+tag+"*{");
    tags.append("\\"+tag+"* {");
    tags.append("\\"+tag+" {");
    tags.append("\\"+tag+"  {");
    tags.append("\\"+tag+"   {");
    tags.append("\\"+tag+"\n{");
    QString nl = QString::fromUtf8("\u2029");
    QString cnt = cntx;
    while(cnt.contains(nl)){
        cnt.replace(nl,"\n");
    }

    for(int i=0;i<tags.count();i++){
        tag = tags.at(i);
        tagpos = cnt.indexOf(tag,0);
        while (tagpos>=0 ){
            if (!isTexComment(cnt,tagpos)){
                CLeaf* aleaf = new CLeaf();
                aleaf->parent = NULL;
                aleaf->level = tag.toLower();
                aleaf->position = tagpos;
                aleaf->title = TextInBrace(cnt,tagpos);
                AllLeafs.append(aleaf);
            }
            tagpos = cnt.indexOf(tag, tagpos+1);
        }
    }
}


void CStructureData::Parse(QString& cnt){
    Clear();
    //if(!cnt.contains("\\")){
    //    return;
    //}
    getLeaf(cnt,"subsubsection");
    getLeaf(cnt,"subsection");
    getLeaf(cnt,"section");
    getLeaf(cnt,"chapter");
}





StructureThread::StructureThread(QObject *parent):QThread(parent)
{
    content ="";
    itemlist=NULL;
    data.Clear();
}
StructureThread::StructureThread(QString& cnt, QList<QTreeWidgetItem*>* itmlst){
    content = cnt;
    itemlist = itmlst;
}

void StructureThread::setString(QString& cnt){
    content= cnt;
    //qDebug()<<cnt.mid(0,20);
    //qDebug()<<content.mid(0,20);
}

void StructureThread::setItemList(QList<QTreeWidgetItem*>* itmlst){
    itemlist = itmlst;
}


void StructureThread::run(){
    if(content.length()<10){
        return;
    }
    QTime bg = QTime::currentTime();
    QTime ed ;
    data.Parse(content); //qDebug()<<"Parse OK."<<endl;
    data.constructTree();//qDebug()<<"Constructure Tree OK."<<endl;
    data.BuildQTree(itemlist); //qDebug()<<"Build QTree OK."<<endl;
    ed = QTime::currentTime();

    //qDebug()<<"PARSE:\n  BG: "<<bg.toString("mm:ss.zzz")<<"\n  ED: "<<ed.toString("mm:ss.zzz")<<endl;

    if(itemlist->count()>0){
        emit(nowShowTheTree()); qDebug()<<"Emit nowShowTheTree OK.\nFrom "<<bg.toString("mm:ss.zzz")<<" --> "<< ed.toString("mm:ss.zzz")<<" --> "<<QTime::currentTime().toString("mm:ss.zzz")<<endl;
    }
}

