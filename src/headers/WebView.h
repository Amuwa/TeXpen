#ifndef WEBVIEW_H
#define WEBVIEW_H
#include<QWebView>
#include<QKeyEvent>

class WebPage:public QWebPage{
    Q_OBJECT
public:
    WebPage(QObject * parent=0);
protected:
    QString userAgentForUrl(const QUrl & url) const;
};


class WebView:public QWebView{
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
