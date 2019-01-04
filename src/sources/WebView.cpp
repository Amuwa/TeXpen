#include "headers/WebView.h"

#ifdef USE_WEBKIT
#include <QWebPage>
#include <QWebFrame>
#endif


#include <QAction>
#include "3rd-party/cookie/cookiejar.h"


#ifdef USE_WEBKIT
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
#endif


#ifdef USE_WEBKIT
WebView::WebView(QWidget* para):QWebView(para)
{
    WebPage *p=new WebPage(this);
    setPage(p);
}
#elif USE_QT_WEB_ENGINE
WebView::WebView(QWidget* para):QWebEngineView(para)
{

}
#endif

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

#ifdef USE_WEBKIT
    QWebView::keyReleaseEvent(e);
#endif
#ifdef USE_QT_WEB_ENGINE
    QWebEngineView::keyReleaseEvent(e);
#endif


}

void WebView::contextMenuEvent ( QContextMenuEvent * ev ){
    //
    if(this->selectedText().contains("@") && this->selectedText().contains("{") && this->selectedText().contains("}")){
        QString bibitem = selectedText();
        //emit(askToAddBibItem(bibitem));
        QAction* addBib = new QAction("Add to Bib database...",this);
        addBib->setData(bibitem);
    }

#ifdef USE_WEBKIT
    QWebView::contextMenuEvent(ev);
#endif
#ifdef USE_QT_WEB_ENGINE
    QWebEngineView::contextMenuEvent(ev);
#endif
}
