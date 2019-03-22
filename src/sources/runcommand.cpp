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
#include <QDebug>

#include <QFile>
#ifdef USE_WEBKIT
#include <QWebFrame>
#endif


//QtConcurrent::run

void asyncRun(MainWindow* wnd, QString& prog, QStringList& arglist){
    wnd->proc = new QProcess;
    QObject::connect(wnd->proc,SIGNAL(readyReadStandardOutput()),
                     wnd,SLOT(readProcOutput())
                     );
    wnd->proc->setProcessChannelMode(QProcess::ForwardedChannels);
    wnd->proc->startDetached(prog,arglist);

}

#include <QFile>
#include <QDir>

#ifdef USE_WEBKIT
#include <QWebFrame>
#include <QWebElement>
#include <QWebElementCollection>
#endif

QString getPage(WebView* v, QString* pg){
    //"pageNumber"
#ifdef USE_WEBKIT
    QWebElement ec = v->page()->mainFrame()->findFirstElement("#pageNumber");
    if(ec.isNull()){
        *pg = "0";
        return *pg;
    }
    else {
        //QString page = ec.attribute("value");
        *pg = ec.evaluateJavaScript("this.value").toString();
        //qDebug()<<"Page = "<<page<<",  xpage = "<<xpage<<endl;
        return *pg;
    }
#endif

#ifdef USE_QT_WEB_ENGINE
    *pg ="0";
    v->page()->runJavaScript(
                "function getpage(){var v = document.getElementById('pageNumber');return v.value;} getpage();",
                [pg](const QVariant &rs) { qDebug() << rs.toString(); *pg=rs.toString();}
    );
    return *pg;
#endif

}

void MainWindow::onCmdFinish(){
    runCommand->setIcon(QIcon(":/icons/icons/24x24/media-playback-start.png"));
    runCommand->setStatusTip(tr("Run: %1").arg(Settings.theCommand));
    runCommand->setText(tr("Build"));

#ifdef USE_WEBKIT
    QWebSettings::clearMemoryCaches ();//!!!
#endif

    if(!wikiview->isHidden()){
        if(wikiview->windowTitle().toLower().contains("pdf")){
            pdfpage = getPage(wiki, &pdfpage);
#ifdef USE_WEBKIT
            QWebSettings::clearMemoryCaches ();//!!!
#endif
            QString basePath = QDir::homePath()+"/.TeXpen/pdf-js/";
            QFileInfo ff(FileName);
            //QString ur = "file://"+basePath+"web/viewer.html?file="+ff.absolutePath()+"/"+ff.baseName()+".pdf#"+page;
            QString ur= QUrl::fromLocalFile(basePath+"web/viewer.html").toString();//.toString()+"?file="+ff.absolutePath()+"/"+ff.baseName()+".pdf#"+page;

            QFile pdf(ff.absolutePath()+"/"+ff.baseName()+".pdf");
            if(pdf.open(QIODevice::ReadOnly)){
                pdfBase64 =pdf.readAll();
                pdf.close();
            }else{
                pdfBase64 ="";
            }
            //pdfBase64 = "data:application/pdf;base64,"+QString::fromLocal8Bit(cnt.toBase64());
            bindJSobj();
            wiki->load(QUrl(ur));
            return;
        }
    }


}

void MainWindow::onCmdFailed()
{
    onCmdFinish();
    equation->setHtml("Building error.");
}

void MainWindow::onRun(){

    //run->start();
    ApplySettings();//update system variables in the command
    if(run->isCmdRunning()){
        //run->getProc()->kill();
        run->StopAll();
        runCommand->setIcon(QIcon(":/icons/icons/24x24/media-playback-start.png"));
        runCommand->setStatusTip(tr("Run: %1").arg(Settings.theCommand));
        runCommand->setText(tr("Build"));
        equation->setHtml("Building terminated.");
        //equationview->hide();
    }else{
        //qDebug()<<"RUN Clicked"<<endl;
        equationview->setWindowTitle("Build Information");
        equation->setHtml("running...<br>");
        equationview->show();
        runCommand->setIcon(QIcon(":/icons/icons/24x24/media-playback-stop.png"));
        runCommand->setStatusTip(tr("Stop building"));
        runCommand->setText(tr("Stop building"));
        run->run();
    }
    return;
}

void MainWindow::showMsg(QString str){
    QString html="<font face=\"courier\" size=\"2\">";
    html += str;
    html.replace("\n","<br>");
    html.append("</font>");
#ifdef USE_WEBKIT
    QPoint sp = equation->page()->mainFrame()->scrollPosition();
#endif
#ifdef USE_QT_WEB_ENGINE
    QPointF sp = equation->page()->scrollPosition();
#endif
    if(sp.x() >= 25535){
        int pos = html.length()/2;
        html=html.mid(pos);
    }else{
#ifdef USE_WEBKIT
        html = equation->page()->mainFrame()->toHtml()+html;
#endif
#ifdef USE_QT_WEB_ENGINE
        equation->page()->toHtml([&html](const QString rs){html = rs+html;});
#endif
    }


    equation->setHtml(html);
    //equation->scroll(0,2536);

    sp += QPoint(0,25530);
#ifdef USE_WEBKIT
    equation->page()->mainFrame()->setScrollPosition(sp);
#endif
#ifdef USE_QT_WEB_ENGINE
    //equation->scroll((int)sp.x,(int)sp.y);
#endif

    try{
    equationview->show();
    }catch(std::exception e){
        QString xxmsg(e.what());
        qDebug()<<xxmsg<<endl;
    }
    qDebug()<<"show msg over."<<endl;
}

void MainWindow::readProcOutput(){
    //qDebug()<<"MainWindow::readProcOutput()..."<<endl;
//    QByteArray ba = proc->readAllStandardOutput();
//    ba += proc->readAllStandardError();
//    ba += proc->readAll();
//    qDebug()<<"ba="<<ba<<endl;
    QByteArray ba = run->getProc()->readAll();
    QString html="<font face=\"courier\" size=\"2\">";
    html.append(ba);
    html.replace("\n","<br>");
    html.append("</font>");
#ifdef USE_WEBKIT
    QPoint sp = equation->page()->mainFrame()->scrollPosition();
#endif
#ifdef USE_QT_WEB_ENGINE
    QPointF sp = equation->page()->scrollPosition();
#endif
    if(sp.x() >= 25535){
        int pos = html.length()/2;
        html=html.mid(pos);
    }else{
#ifdef USE_WEBKIT
        html = equation->page()->mainFrame()->toHtml()+html;
        equation->setHtml(html);
#endif
#ifdef USE_QT_WEB_ENGINE
        equation->page()->runJavaScript("document.body.innerHTML += '" + html + "';" + "window.scrollTo(0,document.body.scrollHeight);");
#endif
    }

    //equation->scroll(0,2536);

    sp += QPoint(0,25530);
#ifdef USE_WEBKIT
    equation->page()->mainFrame()->setScrollPosition(sp);
#endif
#ifdef USE_QT_WEB_ENGINE
    //equation->scroll((int)sp.x,(int)sp.y);
#endif
    equationview->show();
}
