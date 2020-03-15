/*
 * Copyright (c) MengChang Wang <wangmengchang@gmail.com>
 * Webpage:  http://www.journalhome.com/texpen/
 *
 *
*/

#include "headers/mainwindow.h"
#include <QProcess>
#include <QAction>
#include <QTextDocument>
#include <QTextCursor>
#include <QVector>
#include <QTextCursor>
#include <QDebug>

#include <QCompleter>
#include <QListWidget>
#include <QFontMetrics>


//To-Do
// Scan Labels , and listen to "\ref{"


extern QStringList AWL;
extern void setupAWL();

extern QStringList cmds;
extern void setupCmds();

QStringList AutoCompList;

void MainWindow::showCompletion(){
    if(AutoCompList.isEmpty()){
        AutoCompList<<"do"<<"document"<<"documentclass"
                   <<"use"<<"usepackage"
                   <<"section"<<"subsection"<<"subsubsection"<<"chapter"
                   <<"graphicx"<<"fontspec"
                  <<"new"<<"newcommand"<<"renew"<<"renewcommand"
                  <<"table"<<"tableofcontents"<<"item"<<"itemize"
                  <<"article"<<"book"<<"be"<<"begin"<<"end"<<"equation"<<"table"<<"figure"
                   <<"alpha"<<"beta"<<"gamma"<<"delta"<<"epsilon"<<"zeta"<<"eta"
                                   <<"theta"<<"iota"<<"kappa"<<"lambda"
                                 <<"rho"<<"sigma"<<"tau"<<"upsilon"<<"phi"<<"chi"
                                 <<"psi"<<"omega"<<"varGamma"<<"varSigma"<<"varTheta"
                                 <<"varLambda"<<"varkappa"<<"varpi"<<"varsigma"
                                <<"varepsilon"<<"varOmega"<<"emptyset"<<"square"
                               <<"sum"<<"not"<<"notin"<<"leq"<<"geq"<<"equiv"<<"sin"<<"cos"
                                            <<"frac"<<"ldots"<<"sim"<<"text"<<"prod"<<"cdot"
                                            <<"infty"<<"limits"<<"nonumber"<<"forall"<<"cup"<<"wedge"
                                            <<"left"<<"right"<<"lfloor"<<"rfloor"<<"lceil"<<"rceil"
                                            <<"mathcal"<<"max"<<"min"<<"int"<<"times"<<"cap"
                                           <<"mathop"<<"sup"<<"leftarrow"<<"rightarrow"<<"exp"
                                          <<"Rightarrow"<<"Leftarrow"<<"bar"<<"hat"<<"tilde"
                                         <<"begin{}\n\\end{}"<<"item"
                                           <<"begin{itemize}\n\\item \n\\end{itemize}"
                 <<"TeXpen"
                 <<"MengChang Wang"
                   ;
    }
    comSec="";

    autoComp->clear();
    autoComp->hide();

    QTextCursor cr = TextEdit->textCursor(); //cr.positionInBlock()
    int st = cr.positionInBlock()-50;
    int ed = cr.positionInBlock();
    if(st<0){st=0;}
    QString sec = cr.block().text().mid(st,ed-st);
    if(sec.endsWith("?") ||sec.endsWith(",") || sec.endsWith(".")   || sec.endsWith(":") || sec.endsWith(" ")
            || sec.endsWith("/")|| sec.endsWith("&")  || sec.endsWith("\%") || sec.endsWith("-")
            ){
        autoComp->clear();
        autoComp->hide();
        TextEdit->setFocus();
        return;
    }

    //find the "\"
    bool isLaTeXCommand = false;
    int spidx = sec.indexOf(QRegExp("\\{|\\[|\\\\"));
    int pos =-1 ;
    while(spidx >= 0){
        pos = spidx+1;
        spidx = sec.indexOf(QRegExp("\\{|\\[|\\\\"), pos);
    }
    if(pos<0){
        pos = sec.lastIndexOf(" ");
        if(pos<0){
            pos=0;
        }else{
            pos = pos+1;//word starts behind the space (" ")
        }
        isLaTeXCommand = false;
    }else{
        isLaTeXCommand = true;
    }

    sec = sec.mid(pos);

    int Len = 2;
    if(!isLaTeXCommand){
        Len = 4;
    }

    if(sec.isEmpty() || sec.length()<Len){
        return;
    }
    QRect rt = TextEdit->cursorRect();
    comSec=sec;

    QStringList cl;
    QStringList el;

    int ml = 0;
    QString maxstr ="";

    if(isLaTeXCommand){
        foreach (QString wd, AutoCompList){
            if(wd == sec){
                cl<<wd;
                if(wd.length() > ml){ml = wd.length();maxstr=wd;}
            }
            else if(wd.startsWith(sec)){
                el<<wd;
                if(wd.length() > ml){ml = wd.length();maxstr=wd;}
            }
        }
        cl<<el;
    }else{
        setupAWL();
        setupCmds();

        //TextEdit->refreshFWords();

        QStringList LST = AWL+TextEdit->frequentWords + cmds;

        foreach (QString wd, LST){
            if(wd == sec && !cl.contains(wd) && !el.contains(wd)){
                cl<<wd;
                if(wd.length() > ml){ml = wd.length();maxstr=wd;}
            }
            else if(wd.startsWith(sec) && !el.contains(wd) && !cl.contains(wd)){
                el<<wd;
                if(wd.length() > ml){ml = wd.length();maxstr=wd;}
            }
        }
        cl<<el;



    }

    if(cl.isEmpty()){
        autoComp->clear();
        autoComp->hide();
        TextEdit->setFocus();
        return;
    }
    int h = (cl.size()>10)?10:cl.size();

    QFontMetrics fm(Settings.Font);
    //int ww = fm.maxWidth(); if(ww>25){ww=fm.width('W');}qDebug()<<"www="<<ww<<endl;
    int hh = fm.height();
    int W= fm.width(maxstr)+30;//ww *(ml+2)  + 20;
    int H= hh *(h+0)  + 20;

    int Top = 0;
    int Left = 0;

    autoComp->setFont(Settings.Font);

    autoComp->clear();
    autoComp->addItems(cl);
    if(rt.bottom()+1 + H <= TextEdit->height()){
        Top = rt.bottom()+1;
    }else{
        Top = rt.top()-1-H;
    }
    if(rt.left()+W > TextEdit->width()){
        Left = TextEdit->width()-W;
    }else{
        Left = rt.left();
    }
    autoComp->setGeometry(Left,Top,W,H);
    autoComp->setCurrentRow(0);
    statusBar()->showMessage(tr("Key [space]: first item; Or Up/Down(or [Tab] then J/K/Up/Down), then [space]."));
    autoComp->show();
    if(comSec.startsWith("::")){
        autoComp->setFocus();
    }else{
        TextEdit->setFocus();
    }
}

void MainWindow::completeWith(QString txt){

    QTextCursor cr = TextEdit->textCursor();
    if(comSec == txt){
        cr.insertText(" ");
        return;
    }

    int l=comSec.length();

    for(int i=0;i<l;i++){
        cr.deletePreviousChar();
    }

    if(txt.startsWith(",,")){
        TextEdit->setFocus();
        autoComp->clear();
        autoComp->hide();
        comSec ="";
        statusBar()->showMessage(tr(""));
        if(txt.contains(",,open")){
            //
        }else if(txt.contains(",,font")){
            //
        }else if(txt.contains(",,exit")){
            //
        }else if(txt.contains(",,figure")){
            insertGraphic();
        }else if(txt.contains(",,table")){
            insertTable();
        }else if(txt.contains(",,equation")){
            insertEquation();
        }else if(txt.contains(",,article")){
            QFile file(":/article.tex");
            file.open(QFile::ReadOnly | QFile::Text);
            QTextStream Contents(&file);
            Contents.setCodec("UTF-8");
            QString cnt = Contents.readAll();
            file.close();

            TextEdit->insertPlainText(cnt);
            UpdateWindowTitle();
        }else if(txt.contains(",,beamer")){
            QFile file(":/slide.tex");
            file.open(QFile::ReadOnly | QFile::Text);
            QTextStream Contents(&file);
            Contents.setCodec("UTF-8");
            QString cnt = Contents.readAll();
            file.close();

            TextEdit->insertPlainText(cnt);
            UpdateWindowTitle();
        }


    }else{
        cr.insertText(txt);
        TextEdit->setFocus();
        autoComp->clear();
        autoComp->hide();
        comSec ="";
        statusBar()->showMessage(tr(""));
    }

}

#include "headers/qtexedit.h"
bool MainWindow::eventFilter(QObject *obj, QEvent *event){
    if(obj == TextEdit && event->type() == QEvent::KeyPress){
        qDebug()<<"capturing..."<<endl;
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        qDebug()<<"Key = "<<keyEvent->key()<<endl;
        if(keyEvent->key()+1 == Qt::Key_Enter || keyEvent->key() == Qt::Key_Enter){
            qDebug()<<"\t is Enter key..."<<endl;
            if(TextEdit->getCompletor()!=NULL && (!TextEdit->getCompletor()->isHidden())){
                qDebug()<<"Enter is captured here";
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(obj,event);
}

void MainWindow::keyReleaseEvent ( QKeyEvent * e ){
    if(e->key()==Qt::Key_E && (e->modifiers()==Qt::ControlModifier||e->modifiers()==Qt::MetaModifier)){
        TextEdit->setFocus();
    }else if(e->key()==Qt::Key_Escape){
        TextEdit->setFocus();
    }else if (e->key()==Qt::Key_F8){
        qDebug()<<"F8 pressed";
        realtimePreview();
    }
    QMainWindow::keyReleaseEvent(e);
}


