// Copyright (C) 2013 Alex-97
// This file is part of textpad-editor.
//
// textpad-editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// textpad-editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with textpad-editor.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QMainWindow"
#include "QMessageBox"
#include "QPlainTextEdit"
#include "QApplication"
#include <QAction>
#include <QSignalMapper>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#ifdef USE_QT_WEB_ENGINE
    #include <QWebEngineView>
#else
    #include <QWebView>
#endif

#include <QListWidget>
//#include <Qsci/qsciscintilla.h>

#include "headers/searchdialog.h"
#include "headers/highlighter.h"
#include "headers/runthread.h"
#include "headers/qtexedit.h"

#include <QDockWidget>
#include <QTreeWidget>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QList>
#include <QProcess>
#ifdef QT5SDK
    #include <QtConcurrent/QtConcurrent>
#else
    #include <QtConcurrentRun>
#endif
#include "sources/cstructuredata.h"
#include "3rd-party/ginger/ginger.h"
#include "WebView.h"
#include "headers/FileSaver.h"


class FindDialog;

//a function for tex parsing
bool isTexComment(QString& blk);
bool isTexComment(QTextCursor& tc);
class RunThread;

class MainWindow : public QMainWindow {
    Q_OBJECT

    bool isStar;//should show a '*' in the title?
    QString comSec;//word-section before auto-completion

    FileSaver* theFileSaver;

    bool realtimepolish;

public:
    MainWindow();
    QByteArray pdfBase64;

signals:
    void nowRefreshStructure();
    void nowHighlight();

protected:
    void closeEvent(QCloseEvent *Event);


private slots:
    // mainwindow.cpp
    void AboutTextpad();
    void SelectFont();
    void ViewHelp();
    void UpdateWindowTitle();
    void onTextChange(int position, int charsRemoved, int charsAdded);
    void onModificationChanged(bool mod);
    void onSpellLang();
    void onGingerPolish();

    // document.cpp
    bool Save();
    bool SaveAs();
    void CountWords();
    void NewDocument();
    void NewArticleDocument();
    void NewSlideDocument();
    void InsertDateTime();
    void insertTable();
    void insertGraphic();
    void OpenFile();
    void PrintDocument();
    void LoadFile();
    bool SaveFile(const QString FileLocation);
    int  MaybeSave();
    void addToRecentList(QString fpath);
    void removeFromRecent(QString fpath);
    void openRecent();
    void setupHighlighter();

    //shortCMD
    void insertEquation();

public slots:
    bool OpenFile(const QString filePath);
    void Debug(QString info);
    QByteArray getPDF();
    int getPDFPage();

private slots:

    // Settings.pp
    void ReadSettings();
    void ApplySettings();
    void WriteSettings();
    void ToggleFullscreen();
    void ToggleMinimalMode();
    void ToggleNormalMode();
    void ToggleReadOnly();
    void ToggleStatusBar();
    void ToggleToolbar();
    void ToggleOutline();
    void ToggleWordWrap();
    void ToggleSpellcheck();


    // widgetcreator.cpp
    void ConnectSlots();
    void CreateActions();
    void CreateMenus();
    void CreateNewDocument();
    void CreateStatusbar();
    void CreateToolbar();
    void Initialize();
    void CreateOutlineWidget();
    void CreateEquationview();
    void CreateWikiView();

    // colorthemes.cpp
    void CreateStyle(int SelectedTheme);

    //outline/structure
    void reloadStructure(QString &cnt);
    void jumpToTag(QTreeWidgetItem*item, int col);
    void reloadTree();

    //equationviewer.cpp
    bool isNowEquation(); //judge wheather the cursor is in an equation
    bool isNowEquation(QString sts, QString eds);
    bool isInlineEquation();//equation declared by sigle $
    QString equationString();//
    void renderEquation();

    void showDict(QString str);
    void showWiki(QString str);
    void goToGoogleSearch();
    void ParseGinger();
    void showGinger(QString sec);
    void realTimeGinger(QString ch=".");

    void showDonate();

    void hideSideView();

    void bindJSobj();


    //runcommand.cpp
public slots:
    void onCmdFinish();
    void onCmdUpdateStatus(QString msg);
private slots:
    void onRun();
    void onConfig();
    void preview();
    void realtimePreview();

    //autocompletion.cpp
    void showCompletion();
    void completeWith(QString txt);

private:
    QAction *NewAction;
    QAction *NewArticleAction;
    QAction *NewSlideAction;
    QAction *OpenAction;
    QAction *SaveAction;
    QAction *SaveAsAction;
    QAction *PrintAction;
    QAction *QuitAction;
    QAction *UndoAction;
    QAction *RedoAction;
    QAction *CutAction;
    QAction *CopyAction;
    QAction *PasteAction;
    QAction *FindAction;
    QAction *SetReadOnlyAction;
    QAction *SelectAllAction;

    QAction *InsertTable;
    QAction *InsertGraphic;

    QAction *DateTimeAction;
    QAction *FontsAction;
    QAction *WordWrapAction;
    QAction *FullscreenAction;
    QAction *ToolbarAction;
    QAction *OutlineAction;//toggle/hide the outline dockwindow
    QAction *StatusbarAction;
    QAction *NormalModeAction;
    QAction *MinimalModeAction;
    QAction *CustomModeAction;
    QAction *HelpAction;
    QAction *AboutQtAction;
    QAction *AboutAction;
    QAction *DonateAction;

    QAction *ConsoleColor;
    QAction *DuskColor;
    QAction *HackerColor;
    QAction *LightColor;
    QAction *NavyColor;
    QAction *NightColor;
    QAction *NormalColor;
    QAction *PaperColor;

    QAction *SeparatorAction;

    QAction *runCommand; //run a command, maybe 'make' 'qmake' 'latex' or other commands
    QAction *configCommand; //config it
    QAction *previewPdf;
    QString pdfpage;

    QAction *SpellCheck;
    QAction *SpellCheck_None;
    QAction *SpellCheck_GB;
    QAction *SpellCheck_US;
    QAction *ConfigSpellCheck;

    QAction *GingerPolish;

    QMenu *RecentFiles;


    QMenu *FileMenu;
    QMenu *EditMenu;
    QMenu *FormatMenu;
    QMenu *ViewMenu;
    QMenu *ToolMenu;
    QMenu *HelpMenu;

    QMenu *NewDocMenu;
    QMenu *ModesMenu;
    QMenu *ColorschemesMenu;
    QMenu *SpellMenu;

    QToolBar *Toolbar;
    //QPlainTextEdit *TextEdit;
    QTeXEdit* TextEdit;

    //QsciScintilla* sciTextEdit;

    QLabel *DocumentSizeLabel;

    struct Settings_T {
        int ColorScheme;
        bool FullScreen;
        bool StatusBarVisible;
        bool ReadOnly;
        bool ToolbarVisible;
        bool OutlineVisible;
        bool WordWrapEnabled;
        bool SpellCheckEnabled;
        QString SpellCheckLang;
        QString theCommand;
        QString cmdDir;
        QStringList recentFiles;
        QFont Font;
    };

    QString FileName;
    Settings_T Settings;

    FindDialog *SearchDialog;

    //Highlighter
    LaTeXHighlighter
        *highlighter;

    QDockWidget* outline;
        QTreeWidget* structure;
    QDockWidget* equationview;
#ifdef USE_WEBKIT
        QWebView* equation;
#endif
#ifdef USE_QT_WEB_ENGINE
        QWebEngineView* equation;
#endif
    QList<QTreeWidgetItem*> ItemList;

    QDockWidget* wikiview;
        WebView* wiki;
#ifdef USE_WEBKIT
        Ginger* ag;
#endif

    StructureThread* structureThread;

    Completor* autoComp;

public:
    QProcess* proc;
    RunThread* run;
public slots:
    //runcommand.cpp
    void readProcOutput();
    //Highlighter*
    LaTeXHighlighter* getHighlighter(){return highlighter;}

    void showMsg(QString str);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    virtual void keyReleaseEvent ( QKeyEvent * e );
};

#endif
