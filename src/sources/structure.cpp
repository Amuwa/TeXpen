/*
 * Copyright (c) MengChang Wang <wangmengchang@gmail.com>
 * Webpage:  http://www.journalhome.com/texpen/
 *
 *
*/

#include "headers/mainwindow.h"
#include <QTreeWidgetItem>
#include <QAction>
#include <QTextDocument>
#include <QTextCursor>
#include <QVector>
#include <QDebug>
#include <QTime>


#include <iostream>
#include "cstructuredata.h"


QString TextInBrace(QString& doc, int pos, QChar begin='{', QChar end='}'){
    QString title = "";
    if(pos<0){pos=0;}
    int MC = doc.length()-pos;
    int bracecount = 0;
    int mc=0;
    int start=-1;
    int length =-1;
    bool inbrace = false;
    for(;mc<MC;mc++){
        QChar ch = doc.at(pos+mc);
        title += ch;
        if(ch == begin){
            bracecount ++;
            inbrace=true;
            if(start<0){
                start = mc;
            }
        }
        if(ch == end){            
            bracecount --;
        }
        if(bracecount ==0 && inbrace){
            if(start>=0){
                length = mc-start;
            }
            break;
        }
    }
    if(length>2){
        title = title.mid (start+1, length-1);
    }else if(length>100){
        title = title.mid(start+1,80) +"...";
    }
    return title;
}

bool isTexComment(QString& blk){
    if(!blk.contains("\%")){
        return false;
    }
    while(blk.startsWith(" ")){
        blk = blk.mid(1);
    }

    QString nl = QString::fromUtf8("\u2029");
    if(blk.contains(nl)){
        blk.replace(nl,"\n");
    }

    int s=0;
    int idx = blk.indexOf("\%",s);
    while(idx>=0){
        if(idx==0 || blk.at(idx-1)!='\\'){
            return true;
        }
        s=idx+1;
        idx = blk.indexOf("\%",s);
    }
    return false;
}

bool isTexComment(QTextCursor& tc){
    QString bc = tc.block().text().mid(0,tc.positionInBlock());
    return(isTexComment(bc));
}

bool isTexComment(QString& ct, int pos){
    if(pos<0 || ct.isEmpty()){
        return false;
    }
    int idx = ct.indexOf("\n");
    if(idx<0){
        QString nl = QString::fromUtf8("\u2029");
        idx = ct.indexOf(nl);
    }
    if(idx>pos || idx<0){
        //only one line in ct
        QString md = ct.mid(0,pos);
        return isTexComment(md);
    }else{
        int np = idx;
        while(idx<pos && idx>=0){ //&& idx>=0 , here is the key for a section/subsection/.. at the last line!!!
            np = idx;
            idx=ct.indexOf("\n",np+1);
            if(idx<0){
                QString nl = QString::fromUtf8("\u2029");
                idx = ct.indexOf(nl,np+1);
            }
        }
        QString md = ct.mid(np+1,pos-np);
        return isTexComment(md);
    }
    return false;
}


bool isProcessing = false;

void MainWindow::reloadTree(){
    /*
    if(!isProcessing){
        return;
    }*/

    structure->clear();
    structure->setHeaderHidden(true);

    if(ItemList.isEmpty()){
        if(Settings.OutlineVisible){
            outline->hide();
        }
        isProcessing = false;
        return;
    }else{
        QTime showt = QTime::currentTime();
        QTime endt;
        structure->insertTopLevelItems(0, ItemList);
        structure->expandAll();
        endt = QTime::currentTime();
        QString str = "Refresh stucture tree:\n Begin: "+showt.toString("mm.ss.zzz")+"\n End:  "+endt.toString("mm.ss.zzz")+"\n";
        //qDebug()<<str<<endl;

        ApplySettings();
    }
    //isProcessing = false;

}


void
MainWindow::
reloadStructure(QString& cnt){
    if(structureThread == NULL){
        structureThread = new StructureThread();
        connect(structureThread,SIGNAL(nowShowTheTree()),this,   SLOT(reloadTree()));
    }
    if(structureThread->isRunning()){
        return;
    }
    ItemList.clear();

    structureThread->setItemList(&ItemList);
    structureThread->setString(cnt);
    structureThread->start();
}




void MainWindow::jumpToTag(QTreeWidgetItem* item, int col){
    Q_UNUSED(col);

    if(item==NULL){
        return;
    }
    int pos = item->data(0,Qt::UserRole).toInt();
    QTextCursor crs(TextEdit->document());
    crs.setPosition(pos);
    TextEdit->setTextCursor(crs);
    TextEdit->centerCursor ();
    TextEdit->setFocus();
}

//////////////////////////////////////////
//////////////////////////////////////////
///
///
///
///
///



