#include "headers/WebView.h"
#include <QWebPage>
#include <QWebFrame>
#include <QAction>
#include "3rd-party/cookie/cookiejar.h"

CookieJar* jar=NULL;
WebPage::WebPage(QObject* para):QWebPage(para)
{
    if(jar==NULL){
        jar = new CookieJar;
        jar->loadSettings();
        networkAccessManager()->setCookieJar(jar);
    }

}

QString WebPage::userAgentForUrl(const QUrl & url) const{
    Q_UNUSED(url);
    QString rs = "Opera/9.80 (J2ME/MIDP; Opera Mini/9.80 (J2ME/22.478; U; en) Presto/2.5.25 Version/10.54";
    return rs;
}




WebView::WebView(QWidget* para):QWebView(para)
{
    WebPage *p=new WebPage(this);
    setPage(p);
}

void WebView::keyReleaseEvent ( QKeyEvent * e ){

    if(e->key()==Qt::Key_M && (e->modifiers()==Qt::ControlModifier||e->modifiers()==Qt::MetaModifier)){
        //emit(askToHideSideWidget());
        return;
    }
    else if(e->key()==Qt::Key_Comma && (e->modifiers()==Qt::ControlModifier||e->modifiers()==Qt::MetaModifier)){
        emit(askToHideSideWidget());
        return;
    }else if(e->key()==Qt::Key_Slash && (e->modifiers()==Qt::ControlModifier||e->modifiers()==Qt::MetaModifier)){
        emit(askToHideSideWidget());
        return;
    }else if(e->key()==Qt::Key_Equal && (e->modifiers()==Qt::ControlModifier||e->modifiers()==Qt::MetaModifier)){
        emit(askToHideSideWidget());
        return;
    }
    QWebView::keyReleaseEvent(e);
    //if(jar!=NULL){
    //    jar->save();
    //}
}

void WebView::contextMenuEvent ( QContextMenuEvent * ev ){
    //
    if(this->selectedText().contains("@") && this->selectedText().contains("{") && this->selectedText().contains("}")){
        QString bibitem = selectedText();
        //emit(askToAddBibItem(bibitem));
        QAction* addBib = new QAction("Add to Bib database...",this);
        addBib->setData(bibitem);
    }

    QWebView::contextMenuEvent(ev);

    //if(jar!=NULL){
    //    jar->save();
    //}
}
