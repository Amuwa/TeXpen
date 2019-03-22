/*
 * Copyright (c) MengChang Wang <wangmengchang@gmail.com>
 * Webpage:  http://www.journalhome.com/texpen/
 *
 *
*/

#include "headers/mainwindow.h"
#include <QAction>
#include <QTextDocument>
#include <QTextCursor>
#include <QVector>
#include <QDebug>

QString eqRec="";

bool MainWindow::isInlineEquation(){
    QTextCursor cs = TextEdit->textCursor();
    int pos = cs.position();

    if(isTexComment(cs)){
        return false;
    }

    int stpos =pos-25000;// pos - 500;
    if(stpos<0){
        stpos=0;
    }
    QTextCursor pCur(TextEdit->document());
    pCur.setPosition(stpos);

    QTextCursor st = TextEdit->document()->find("$",pCur);
    if(st.isNull()){
        return false;
    }
        while(isTexComment(st)){
            st = TextEdit->document()->find("$",st.position()+1);
            if(st.isNull()){
                return false;
            }
        }
    QTextCursor et = TextEdit->document()->find("$",st.position()+1);
    if(et.isNull()){
        return false;
    }
        while(isTexComment(et)){
            et = TextEdit->document()->find("$",et.position()+1);
            if(et.isNull()){
                return false;
            }
        }
   //

        while(et.position() - st.position() <2
              || (st.position()<pos && et.position() < pos)

              ){
            st = TextEdit->document()->find("$",et.position()+1);
            if(st.isNull()){
                return false;
            }
                    while(isTexComment(st)){
                        st = TextEdit->document()->find("$",st.position()+1);
                        if(st.isNull()){
                            return false;
                        }
                    }
            et = TextEdit->document()->find("$",st.position()+1);
            if(et.isNull()){
                return false;
            }
                    while(isTexComment(et)){
                        et = TextEdit->document()->find("$",et.position()+1);
                        if(et.isNull()){
                            return false;
                        }
                    }
        }
        //
        if(et.position() - st.position() >= 2
                && (st.position()<pos && et.position() > pos)){
            eqRec=TextEdit->toPlainText().mid(st.position(), et.position() - st.position());
            return true;
        }
        return false;

}


bool MainWindow::isNowEquation() //judge wheather the cursor is in an equation
{
    eqRec="";
    //qDebug()<<"isNowEquation()"<<endl;
    QTextCursor cs = TextEdit->textCursor();
    int pos = cs.position(); //qDebug()<<"\tpos ="<<pos<<endl;

    if(isTexComment(cs)){
        return false;
    }


    QTextCursor st = TextEdit->document()->find("\\begin{equation}");
    if(st.isNull()){
        return false;
    }
        while(isTexComment(st)){
            st = TextEdit->document()->find("\\begin{equation}",st.position()+1);
            if(st.isNull()){
                return false;
            }
        }
    QTextCursor et = TextEdit->document()->find("\\end{equation}",st.position());
    if(et.isNull()){
            return false;
    }
        while(isTexComment(et)){
            et = TextEdit->document()->find("\\begin{equation}",et.position()+1);
            if(et.isNull()){
                break;
            }
        }
    if(st.isNull()){
        return false;
    }else if (et.isNull()){
        return false;
    }
    while(st.position() < pos && et.position()< pos){

        st = TextEdit->document()->find("\\begin{equation}",et.position() +1 );
                while(isTexComment(st)){
                    st = TextEdit->document()->find("\\begin{equation}",st.position()+1);
                    if(st.isNull()){
                        return false;
                    }
                }
        et = TextEdit->document()->find("\\end{equation}",st.position());
                while(isTexComment(et)){
                    et = TextEdit->document()->find("\\begin{equation}",et.position()+1);
                    if(et.isNull()){
                        break;
                    }
                }

        if( st.isNull() || et.isNull() ){
            return false;
        }
    }
    if(st.position() < pos && et.position()> pos){
        eqRec=TextEdit->toPlainText().mid(st.position(), et.position() - st.position());
        return true;
    }
    return false;
}

bool MainWindow::isNowEquation(QString sts, QString eds){
     QTextCursor cs = TextEdit->textCursor();
     if(isTexComment(cs)){
         return false;
     }
     int pos = cs.position();
     int stpos = pos - 2500;
     if(stpos<0){
         stpos=0;
     }
     QTextCursor pCur(TextEdit->document());
     pCur.setPosition(stpos);

     QTextCursor st = TextEdit->document()->find(sts,pCur);
     if(st.isNull()){
         return false;
     }
     while(isTexComment(st)){
         st = TextEdit->document()->find(sts,st.position()+1);
         if(st.isNull()){
             return false;
         }
     }

     QTextCursor et = TextEdit->document()->find(eds,st.position());
     if(et.isNull()){
             return false;
     }
     while(isTexComment(et)){
         et = TextEdit->document()->find(sts,et.position()+1);
         if(et.isNull()){
             break;
         }
     }

     if(st.isNull()){
         return false;
     }else if (et.isNull()){
         return false;
     }
     while(st.position() < pos && et.position()< pos){
         st = TextEdit->document()->find(sts,et.position() +1 );
                 while(isTexComment(st)){
                     st = TextEdit->document()->find(sts,st.position()+1);
                     if(st.isNull()){
                         return false;
                     }
                 }
         et = TextEdit->document()->find(eds,st.position());
                 while(isTexComment(et)){
                     et = TextEdit->document()->find(sts,et.position()+1);
                     if(et.isNull()){
                         break;
                     }
                 }

         if( st.isNull() || et.isNull() ){
             return false;
         }
     }
     if(st.position() < pos && et.position()> pos){
         eqRec=TextEdit->toPlainText().mid(st.position(), et.position() - st.position());
         return true;
     }
     return false;
}




QString MainWindow::equationString()//get the equation source
{//remove comments //remove labels
    QString rs = eqRec;
    rs.remove(QRegExp("\\\\label\\{*\\}"));
    rs.remove(QRegExp("\%[^\n]*"));
    rs.remove("\\end{equation}");
    rs.replace("\n", " ");//remove("\n");

    rs.remove("\\]");
    rs.remove("\\[");
    rs.remove("$$");
    if(!rs.endsWith("\\$")){
        if(rs.endsWith("$")){
            int l=rs.length();
            rs.remove(l-1,1);
        }
    }

    if(rs.endsWith("\\end{flalign}")){
        rs = "\\begin{flalign}" + rs;
    }else if(rs.endsWith("\\end{flalign*}")){
        rs = "\\begin{flalign*}" + rs;
    }else if(rs.endsWith("\\end{align}")){
        rs = "\\begin{align}" + rs;
    }else if(rs.endsWith("\\end{align*}")){
        rs = "\\begin{align*}" + rs;
    }else if(rs.endsWith("\\end{gather}")){
        rs = "\\begin{gather}" + rs;
    }else if(rs.endsWith("\\end{multline}")){
        rs = "\\begin{multline}" + rs;
    }

    QRegExp lb=QRegExp("\\\\label\\{.*\\}");
    if(rs.contains(lb)){
        rs.remove(lb);
    }

    return rs;
}

void MainWindow::renderEquation()//call api-s to render this source into images
{

    //for test
    //return;
    //



    eqRec="";
    if( isNowEquation("$$","$$") ||
            isNowEquation("\\begin{equation}","\\end{equation}") ||
            isNowEquation("\\[","\\]")||
            isNowEquation("\\begin{flalign}","\\end{flalign}")||
            isNowEquation("\\begin{flalign*}","\\end{flalign*}")||
            isNowEquation("\\begin{align}","\\end{align}")||
            isNowEquation("\\begin{align*}","\\end{align*}")||
            isNowEquation("\\begin{gather}","\\end{gather}")||
            isNowEquation("\\begin{multline}","\\end{multline}")||
            isInlineEquation()
            ){

        equationview->setWindowTitle("Preview");
        /*
        QString ur = "http://latex.numberempire.com/render?";
        ur += equationString();
        */
        QString ur = "http://chart.apis.google.com/chart?cht=tx&chl=";
        ur += QUrl::toPercentEncoding(equationString());
        equation->setHtml(QString::fromUtf8("<center><img src=\"%1\" align=\"center\"></center>").arg(ur)
                          );
        equationview->show();
        //TextEdit->centerCursor();
    }else{
        equationview->hide();
    }
}

//Table ---> Preview


//Figure ---> Preview

#include "3rd-party/ginger/ginger.h"




QString dictstr="";
void MainWindow::showDict(QString str){
    if(str==""|| str == dictstr){
        dictstr="";
        wikiview->hide();
        return;
    }
    dictstr=str;//.replace(" ","+");

    QString ur="";

    if(str.toLower().indexOf(QRegExp("[aeioustdhlg]"))<0){
        //ur= "http://dict.cn/mini.php?q="+str;
        ur="http://dict.youdao.com/m/search?q="+str;
    }else{
        //ur= "http://www.oxforddictionaries.com/definition/english/"+str;
        //ur="http://www.merriam-webster.com/dictionary/"+str;

        //ur="http://www.learnersdictionary.com/definition/"+str;//***** preferred
        //ur="http://i.word.com/idictionary/"+str;

        str=str.replace(" ","\%20");
        //ur="http://m.youdao.com/dict?le=eng&q="+str;
        ur="http://dict.youdao.com/m/search?q="+str;
        //ur= "http://dict.cn/mini.php?q="+str;
        //ur="http://www.webster-dictionary.org/definition/"+str;
        //str = str.replace(" ","-");
        //ur= "http://dictionary.cambridge.org/dictionary/british/"+str;
    }
    //qDebug()<<"ur = "<<ur<<endl;

    wiki->load(QUrl(ur));
    //wiki->setZoomFactor(0.9);
    QString tt = str.mid(0,20);
    wikiview->setMaximumWidth(360);
    wikiview->setWindowTitle(tt+" ...");
    wikiview->show();

    //if(highlighter!=NULL){
    //    highlighter->spellSuggestion(str);
    //}
}

QString wikistr="";
//Ginger* ag =NULL;
void MainWindow::showWiki(QString str){
    /*
    qDebug()<<"showWiki(QString str) is called..."<<endl;
    if(ag==NULL){
        ag = new Ginger(this);
    }
    ag->test(str);
    return;
    */

    if(str==""|| str == wikistr){
        wikistr="";
        wikiview->hide();
        return;
    }
    wikistr=str;//.replace(" ","+");

    //QString ur= "http://www.dreye.com.cn/mws/dict.php?ua=dc_cont&hidden_codepage=01&project=nd&w="+wikistr;
    QString ur= "https://en.m.wikipedia.org/wiki/"+wikistr;

    wiki->load(QUrl(ur));
    //wiki->setZoomFactor(0.9);
    QString tt = str.mid(0,20);
    wikiview->setMaximumWidth(400);
    wikiview->setWindowTitle("Wikipedia: "+tt+" ...");
    wikiview->show();
}


bool isGSearch=false;
void MainWindow::goToGoogleSearch(){
    if(isGSearch){
        wikiview->hide();
        isGSearch = false;
        TextEdit->setFocus();
        return;
    }

    wiki->load(QUrl(QString("https://scholar.google.com/scholar?btnG=&q=%1").arg(TextEdit->keywordForSearch)));
    //wiki->setZoomFactor(0.9);
    //wiki->setHtml("<html><body>A..<br><input class=\" image\" src=\"https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif\" name=\"submit\" alt=\"PayPal\" border=\"0\" type=\"image\"><br>B..</body><html>");
    wikiview->setWindowTitle("Google Scholar ...");
    wikiview->setMaximumWidth(640);
    //wikiview->setGeometry();
    wikiview->show();
    wiki->setFocus();
    isGSearch = true;
}


QString don="<html><body>"
        "<h3>Please Suppoert TeXpen</h3><br><hr><br>"
        "<b>PayPal donation:</b><br>"
        "<form action=\"https://www.paypal.com/cgi-bin/webscr\" method=\"post\"> "
        "<p style=\"text-align: center;\">"
        "<input name=\"cmd\" value=\"_donations\" type=\"hidden\"> "
        "<input name=\"business\" value=\"wangmengchang@gmail.com\" type=\"hidden\"> "
        "<input name=\"lc\" value=\"C2\" type=\"hidden\"> "
        "<input name=\"item_name\" value=\"Support TeXpen\" type=\"hidden\"> "
        "<input name=\"no_note\" value=\"0\" type=\"hidden\"> <input name=\"currency_code\" value=\"USD\" type=\"hidden\"> "
        "<input name=\"bn\" value=\"PP-DonationsBF:btn_donateCC_LG.gif:NonHostedGuest\" type=\"hidden\"> "
        "<input alt=\"PayPal\" name=\"submit\" src=\"https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif\" border=\"0\" type=\"image\"> "
        "<img alt=\"\" src=\"https://www.paypalobjects.com/en_US/i/scr/pixel.gif\" border=\"0\" height=\"1\" width=\"1\"></p>"
        "</form>"
        "</body></html>";

#include <QDesktopServices>
void MainWindow::showDonate(){
    QDesktopServices::openUrl(QUrl("http://www.journalhome.com/texpen/406963/considering-donation.html"));
}


void MainWindow::hideSideView(){
    if(!wikiview->isHidden()){
        wiki->setHtml("");
        wikiview->hide();
    }
    TextEdit->setFocus();
    return;
}




//only to show results on wiki, not to manage the on/off of side view (wikiview)
#ifdef USE_WEBKIT
#include <QWebFrame>
#endif

#include <3rd-party/ginger/ginger.h>


void MainWindow::bindJSobj(){
    qDebug()<<"binding JS obj..."<<endl;
#ifdef USE_WEBKIT
    wiki->page()->mainFrame()->addToJavaScriptWindowObject("PDFFILE",this);
    wiki->page()->mainFrame()->evaluateJavaScript("PDFFILE.Debug('Binded.')");
#endif
}

void MainWindow::Debug(QString info){
    qDebug()<<"From JS : "<<info<<endl;
}

QByteArray MainWindow::getPDF(){
    return pdfBase64;
}
int MainWindow::getPDFPage(){
    return pdfpage.toInt();
}

//test:
//He have been right for many yeas with his insignts      for both theory and practicex.
QString parseGJson(QString orgsec, QString json){
    QString rs = "";
#ifdef USE_WEBKIT
    JSON js;
    rs +=
            js.getMistakes(json, orgsec);
    rs+="<br><br><br><br><hr><b>WARNING:</b><br><font color=\"crimson\">Suggested by machines (www.gingersoftware.com), so please take your own risks.</font><br><br>";
#endif

#ifdef USE_QT_WEB_ENGINE
    rs = "<b>Not implemented</b>";
#endif
    return rs;
}


QString lastGingerSec="";

QString lastWords="";
void MainWindow::ParseGinger(){
    if(wikiview->isHidden()){
        return;
    }
    else{
        QString url =wiki->url().toString().toLower();
        if(!url.contains("ginger") || url.contains("about:blank") || url.isEmpty()){
            return;
        }else{
#ifdef USE_WEBKIT
            QString json = wiki->page()->mainFrame()->toPlainText();
#endif
#ifdef USE_QT_WEB_ENGINE
            QString json ="";
            wiki->page()->toPlainText([&json](const QString &rs){json=rs;});//[textEdit](const QString &result){ textEdit->setPlainText(result); }
#endif

            QString nosug = "<h3>No Suggstions</h3><br>May be the phrase: <br><br><font color=\"green\"><i>"+lastGingerSec+"</i></font><br><br><b>is correct.</b><br><br><b>OR</b><br><br><font color=\"grey\">the server makes a mistake (try again?)</font>";
            QString res = nosug;
            if(json.contains("{") && json.contains("}")){
                //
                if(json.contains("\"Mistakes\"")){
                    res = parseGJson(lastWords,json);
                }
                if(res.isEmpty()){
                    res = nosug;
                }

                wiki->setHtml(res);

            }else{
                wiki->setHtml(res);
            }

        }
    }
}


QString removeComment(QString & sec){


    QString nl = QString::fromUtf8("\u2029");//unicode 2029, paragraph character

    QString tp = sec.replace(nl,"\n");
    sec = tp;

    int pos =0;

    pos = sec.indexOf("%",pos);
    while(pos>=0){
        //qDebug()<<"Pos =" <<pos<<endl;
        while(pos>0 && sec.at(pos-1) == '\\'){
            pos = sec.indexOf("%",pos+1);
        }

        if(pos>=0){
            int end = sec.indexOf("\n",pos); //qDebug()<<"line break at "<<end<<endl;
            if(end<pos){
                end = sec.length();
            }
            //qDebug()<<"pos ="<<pos<<", end="<<end;
            QString tmp =sec.remove(pos,end-pos);
            sec=tmp;
            pos = sec.indexOf("%");
        }
    }

    //QString tmp2 = sec.replace("\n"," ");
    //sec = tmp2;

    return sec;
}

void MainWindow::showGinger(QString sec){
    if(!wikiview->isHidden()){
        if(sec == lastGingerSec && sec.isEmpty()){
            lastGingerSec="";
            wikiview->hide();
            return;
        }
    }

    lastGingerSec=sec;

    QString word = removeComment(sec);
    lastWords = word;

    qDebug()<<"word="<<word<<endl;
    word=word.replace(" ","+");
    word=word.replace("\n"," ");

    QString rq= QString("http://services.gingersoftware.com/Ginger/correct/json/GingerTheTextFull?callback=&apiKey=6ae0c3a0-afdc-4532-a810-82ded0054236&lang=US&clientVersion=2.0&text=%1&_=13")
            .arg(word);
    wiki->load(QUrl(rq));
    wikiview->setWindowTitle("Getting Suggestions...");
    wikiview->setMaximumWidth(400);
    wikiview->show();
    TextEdit->setFocus();
}


void MainWindow::realTimeGinger(QString ch){
    Q_UNUSED(ch);
    //get the last sentence
    QString para = TextEdit->textCursor().block().text();
    para = removeComment(para);
    /*
    int periodpos = para.indexOf(ch);
    if(periodpos<0){return;}
    int c=0;
    while(periodpos>=0 && periodpos < para.length()-1){
        para = para.mid(periodpos+1);
        periodpos = para.indexOf(ch);
        c++;
        if(c>200){return;}
    }*/
    if(para.length()>5){
        showGinger(para);
    }
}



//preview PDF
#include <QDir>
#include <QFile>


bool containDir(QFileInfoList& list){
    for(int i=0;i<list.size();i++){
        if(list.at(i).isDir()){
            return true;
        }
    }
    return false;
}

QFileInfoList getInfoList(QFileInfo info){
    //qDebug()<<"Get Info List of "<<info.absoluteFilePath()<<" ..."<<endl;
    QFileInfoList rs;
    rs.clear();
    if(info.isDir()){
        QFileInfoList sublist = QDir(info.absoluteFilePath()).entryInfoList(); //qDebug()<<"\t -->"<<endl;
        foreach(QFileInfo ii, sublist){

                QFileInfoList sslist = getInfoList(ii);
                foreach(QFileInfo iii, sslist){
                    rs.append(iii); //qDebug()<< iii.absoluteFilePath()<<endl;
                }

        }

    }else{
        rs.append(info); //qDebug()<< info.absoluteFilePath()<<endl;
    }

    return rs;
}


void preparePDFjs(){
    QString basePath = QDir::homePath()+"/.TeXpen/pdf-js/";

    qDebug()<<basePath<<endl;

    QDir dir(":/pdf-js/");

    QFileInfoList list = dir.entryInfoList();
    QFileInfoList totallist;
    foreach(QFileInfo fi, list){
        QFileInfoList alist = getInfoList(fi);
        foreach(QFileInfo ii, alist){
            totallist.append(ii);
        }
    }

    foreach(QFileInfo fx, totallist){
        //qDebug()<<fx.absoluteFilePath()<<endl;
        QString src = fx.absoluteFilePath();
        QString dest =src;
                dest = basePath+ dest.replace(":/pdf-js/","");
        QString srcdir = fx.absoluteDir().absolutePath();
        QString destdir = srcdir;
        destdir = basePath+ destdir.replace(":/pdf-js/","");

        QDir a(destdir);
        if(!a.exists(destdir)){
            a.mkpath(destdir);
        }

        //qDebug()<<"copy "<<src<<" as "<< dest<<endl;
        QFile::copy(src,dest);
        QFile::setPermissions(dest,QFile::ReadUser|QFile::ReadGroup|QFile::ReadOther);
    }





    //qDebug()<<"Over."<<endl;

    return;

}


extern QString getPage(WebView* v);
#include <QDir>
#include <QFrame>

void MainWindow::preview(){

    //wikiview->hide();

    int w =int(TextEdit->geometry().width()*0.95/2);
    qDebug()<<"w0 = "<<w<<endl;    

    if(!wikiview->isHidden()){
        if(wikiview->windowTitle().toLower().contains("pdf")){
            pdfpage = getPage(wiki);
            wikiview->hide();            
            return;
        }

        w= int((TextEdit->geometry().width()+wikiview->geometry().width())*0.95/2);
    }
    //if ~/.TeXpen/pdf-js/

    qDebug()<<"w1 = "<<w<<endl;

    QString basePath = QDir::homePath()+"/.TeXpen/pdf-js/";//QDir::tempPath () +"/.TeXpen/pdf-js/";//
    QDir a(basePath);
    if(!a.exists(basePath)){
        a.mkpath(basePath);
        preparePDFjs();
    }


   // wikiview->hide();

    wikiview->setWindowTitle("PDF Preview");
    wikiview->setFixedWidth(w);

    wikiview->show();

    if(FileName.isEmpty()){
        pdfBase64 ="";
        //QString ur = "qrc:/pdf-js/web/viewer.html";//"file://C:/viewer.html";//+a.absolutePath()+"/web/viewer.html";
        QString ur = a.absolutePath()+"/web/viewer.html";
        QUrl u = QUrl::fromLocalFile(ur);//QUrl("http://mozilla.github.io/pdf.js/web/viewer.html");//
        qDebug()<<u.toString()<<endl;
        wiki->load(u);
    }else{

        QFileInfo ff(FileName);        
        QString fu = ff.absolutePath()+"/"+ff.baseName()+".pdf";//QUrl::fromLocalFile(ff.absolutePath()+"/"+ff.baseName()+".pdf").toString();//

#ifdef USE_WEBKIT
        QFile pdf(fu);
        if(pdf.open(QIODevice::ReadOnly)){
            pdfBase64 =pdf.readAll();
            pdf.close();
        }else{
            qDebug()<<"Open "+fu+" Failed."<<endl;
            pdfBase64 ="";
        }
        //QString base64 = "data:application/pdf;base64,"+QString::fromLocal8Bit(cnt.toBase64());
        QString ur = a.absolutePath()+"/web/viewer.html";// //"qrc:/pdf-js/web/viewer.html?file="+fu+"#"+pdfpage;  //
        //qDebug()<<ur<<endl;
        QUrl u = QUrl::fromLocalFile(ur);//QUrl(ur);//
        //u=QUrl(u.toString()+"?file="+base64+"#"+pdfpage);
        //this->pdfBase64 = base64;
        qDebug()<<u.toString()<<endl;
        qDebug()<<"C++ getPDF():\n"<<getPDF()<<"\n=====\n"<<endl;
        qDebug()<<QString("%1").arg(getPDF().count())<<" bytes"<<endl;
        bindJSobj();

        wiki->page()->mainFrame()->load(u);
#endif
#ifdef USE_QT_WEB_ENGINE
        QString pathToPDFjs = "qrc:///pdf-js/web/viewer.html";//a.absolutePath()+"/web/viewer.html";//
        QString loc = pathToPDFjs + QString("?file=file://") + fu;
        qDebug()<<"loading ["+loc+"]..."<<endl;
        QUrl u = QUrl::fromUserInput(loc);
        wiki->page()->load(u);
#endif
    }



    wikiview->setMinimumWidth(1);
    wikiview->setMaximumWidth(1200);
    TextEdit->setFocus();   
}

////// to use MaxJax show realtime content
///
///
///


QString removeCommentx(QString cnt)
{
    QString rs = cnt;
    int pos =rs.indexOf("\%");
    int retpos = -1;
    qDebug()<<"pos="<<pos<<endl;

    while(pos>=0)
    {
        if(pos==0)
        {
            retpos = rs.indexOf("\n",pos);

            if(retpos>=0)
            {
                rs.remove(pos,retpos-pos+1);
                rs.insert(pos," ");
                pos = 0;
                retpos = -1;
            }
        }
        else if(pos>0)
        {
            if(rs.at(pos-1)=='\\')
            {
                pos=pos+1;
            }
            else
            {
                retpos = rs.indexOf("\n",pos);

                if(retpos>=pos)
                {
                    rs.remove(pos,retpos-pos+1);
                    rs.insert(pos," ");
                    qDebug()<<"pos="<<pos<<endl;
                    pos = 0;
                    retpos = -1;
                }
            }
        }
        else
        {
            break;
        }

        pos=rs.indexOf("\%",pos);
    }
    return rs;//???? June 13
}


void MainWindow::realtimePreview(){
    int w =int(TextEdit->geometry().width()*0.95/2);
    if(!wikiview->isHidden()){
//        if(wikiview->windowTitle().toLower().contains("preview")){
//            pdfpage = getPage(wiki);
//            wikiview->hide();
//            return;
//        }

        w= int((TextEdit->geometry().width()+wikiview->geometry().width())*0.95/2);
    }


    QFile file(QLatin1String(":/maxjax.html"));
    bool isOpened = file.open(QIODevice::ReadOnly);
    Q_ASSERT(isOpened);
    Q_UNUSED(isOpened)

    QString txt =TextEdit->toPlainText();
    int i0= txt.indexOf("\begin{document}");
    if(i0>=0){
        txt=txt.mid(i0);
    }

    txt = removeComment(txt);
            while(txt.contains(" \n")){
                txt.replace(" \n","\n");
            }
            while(txt.contains("\n\n\n")){
                txt.replace("\n\n\n","\n\n");
            }

            txt.replace("\n\n","<p>");
            txt.replace("\\begin{itemize}", "<p>");
            txt.replace("\\end{itemize}", "<p>");
            txt.replace("\\item", "<br>");



    QString cnt = QLatin1String(file.readAll());

    QString html = cnt.replace("####",txt);

#ifdef USE_WEBKIT
    wiki->page()->mainFrame()->setHtml(html);
#endif
#ifdef USE_QT_WEB_ENGINE
    wiki->page()->setHtml(html);
#endif

     wikiview->show();

    wikiview->setWindowTitle("Realtime Preview");
    wikiview->setFixedWidth(w);

}





