#ifndef WEBVIEW_H
#define WEBVIEW_H

#ifdef USE_WEBKIT
#include<QWebView>
#elif USE_QT_WEB_ENGINE
#include <QWebEngineView>
#endif

#include<QKeyEvent>

#ifdef USE_WEBKIT
class WebPage:public QWebPage{
    Q_OBJECT
public:
    WebPage(QObject * parent=0);
protected:
    QString userAgentForUrl(const QUrl & url) const;
};
#endif


#ifdef USE_QT_WEB_ENGINE
class WebView: public QWebEngineView{
#elif
class WebView:public QWebView{
#endif

    Q_OBJECT
public:
    WebView(QWidget* para=NULL);

signals:

    void askToHideSideWidget();
    void askToAddBibItem(QString bib);

    void askToParseGinger(QString json);

protected:
    virtual void keyReleaseEvent ( QKeyEvent * e );
    virtual void contextMenuEvent ( QContextMenuEvent * ev );
};


#endif // WEBVIEW_H
