#ifndef QTEXEDIT_H
#define QTEXEDIT_H

#include <QPlainTextEdit>
#include <QListWidget>
#include <3rd-party/hunspell/hunspell.hxx>

//#include <Qsci/qscilexercpp.h>
#include "highlighter.h"
#include <QStringList>
#include <QWidget>

class QTeXEdit;
class LaTeXHighlighter;

class Completor:public QListWidget{
    Q_OBJECT


public:
    QTeXEdit* theEdit;
    explicit Completor(QWidget *parent = 0);
signals:
    void submit(QListWidgetItem*);
protected:
    virtual void keyPressEvent ( QKeyEvent * e );
    virtual void keyReleaseEvent ( QKeyEvent * e );
};

bool inTexComment(const QString &text, const int pos);
class QTeXEdit : public QPlainTextEdit
{
    Q_OBJECT

    Completor* cmp;
    Hunspell* sChecker;
    LaTeXHighlighter* highlighter;
    int themeId;
public:
    explicit QTeXEdit(QWidget *parent = 0);
    void setCompleter(Completor *w);
    void setHighlighter(LaTeXHighlighter* lgt);

    void applySpellDict(QString dicName, QString path="");
    
    Completor* getCompletor(){return cmp;}

    QStringList frequentWords;
    void refreshFWords();

    void manuallyHighlight();

    //line number
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    int firstVisibleBlockNumber(){return firstVisibleBlock().blockNumber();}
    int lastVisibleBlockNumber();

    QString keywordForSearch;

    void setThemeId(int thmid){themeId = thmid;}

signals:
    void askToComplete(QString);
    void askToWiki(QString);
    void askToDict(QString);
    void askToGoogleSearch();
    void askToGinger(QString);
    void askToRealTimePreview();
    
public slots:
    void activateItem(QListWidgetItem* it);
    void copy();
    void cut();
    void replaceCurrentWordWith(bool b=true);    

protected slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);

protected:
    virtual void keyPressEvent ( QKeyEvent * e );
    virtual void keyReleaseEvent ( QKeyEvent * e );
    virtual void inputMethodEvent ( QInputMethodEvent * e );
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    void resizeEvent(QResizeEvent *event);
    
private:
    QWidget *lineNumberArea;
};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(QTeXEdit *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    QTeXEdit *codeEditor;
};



#endif // QTEXEDIT_H
