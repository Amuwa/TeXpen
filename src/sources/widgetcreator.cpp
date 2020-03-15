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

#include "headers/mainwindow.h"
#include <QAction>
#include <QSignalMapper>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>


void MainWindow::ConnectSlots() {
    // Connect the all the actions with their respective slots
    connect(AboutAction,       SIGNAL(triggered()),   this,         SLOT(AboutTextpad()));
    connect(DonateAction,      SIGNAL(triggered()),   this,         SLOT(showDonate()));
    connect(AboutQtAction,     SIGNAL(triggered()),   qApp,         SLOT(aboutQt()));
    connect(InsertTable,       SIGNAL(triggered()),   this,         SLOT(insertTable()));
    connect(InsertGraphic,     SIGNAL(triggered()),   this,         SLOT(insertGraphic()));
    connect(DateTimeAction,    SIGNAL(triggered()),   this,         SLOT(InsertDateTime()));
    connect(FontsAction,       SIGNAL(triggered()),   this,         SLOT(SelectFont()));
    connect(FullscreenAction,  SIGNAL(triggered()),   this,         SLOT(ToggleFullscreen()));
    connect(HelpAction,        SIGNAL(triggered()),   this,         SLOT(ViewHelp()));
    connect(MinimalModeAction, SIGNAL(triggered()),   this,         SLOT(ToggleMinimalMode()));
    connect(NewAction,         SIGNAL(triggered()),   this,         SLOT(NewDocument()));
    connect(NewArticleAction,   SIGNAL(triggered()),    this,       SLOT(NewArticleDocument()));
    connect(NewSlideAction,     SIGNAL(triggered()),   this,        SLOT(NewSlideDocument()));
    connect(NormalModeAction,  SIGNAL(triggered()),   this,         SLOT(ToggleNormalMode()));
    connect(OpenAction,        SIGNAL(triggered()),   this,         SLOT(OpenFile()));
    connect(PrintAction,       SIGNAL(triggered()),   this,         SLOT(PrintDocument()));
    connect(QuitAction,        SIGNAL(triggered()),   this,         SLOT(close()));
    connect(SaveAction,        SIGNAL(triggered()),   this,         SLOT(Save()));
    connect(SaveAsAction,      SIGNAL(triggered()),   this,         SLOT(SaveAs()));
    connect(SetReadOnlyAction, SIGNAL(triggered()),   this,         SLOT(ToggleReadOnly()));
    connect(StatusbarAction,   SIGNAL(triggered()),   this,         SLOT(ToggleStatusBar()));
    connect(ToolbarAction,     SIGNAL(triggered()),   this,         SLOT(ToggleToolbar()));
    connect(OutlineAction,     SIGNAL(triggered()),   this,         SLOT(ToggleOutline()));
    connect(WordWrapAction,    SIGNAL(triggered()),   this,         SLOT(ToggleWordWrap()));
    connect(TextEdit,          SIGNAL(textChanged()), this,         SLOT(CountWords()));
    connect(TextEdit,          SIGNAL(cursorPositionChanged ()), this, SLOT(CountWords()));
    connect(CopyAction,        SIGNAL(triggered()),   TextEdit,     SLOT(copy()));
    connect(CutAction,         SIGNAL(triggered()),   TextEdit,     SLOT(cut()));
    connect(PasteAction,       SIGNAL(triggered()),   TextEdit,     SLOT(paste()));
    connect(RedoAction,        SIGNAL(triggered()),   TextEdit,     SLOT(redo()));
    connect(SelectAllAction,   SIGNAL(triggered()),   TextEdit,     SLOT(selectAll()));
    connect(UndoAction,        SIGNAL(triggered()),   TextEdit,     SLOT(undo()));
    connect(FindAction,        SIGNAL(triggered()),   SearchDialog, SLOT(show()));

    QSignalMapper* SignalMapper = new QSignalMapper(this) ;
    connect(ConsoleColor, SIGNAL(triggered()), SignalMapper, SLOT(map()));
    connect(DuskColor,    SIGNAL(triggered()), SignalMapper, SLOT(map()));
    connect(HackerColor,  SIGNAL(triggered()), SignalMapper, SLOT(map()));
    connect(LightColor,   SIGNAL(triggered()), SignalMapper, SLOT(map()));
    connect(NavyColor,    SIGNAL(triggered()), SignalMapper, SLOT(map()));
    connect(NightColor,   SIGNAL(triggered()), SignalMapper, SLOT(map()));
    connect(NormalColor,  SIGNAL(triggered()), SignalMapper, SLOT(map()));
    connect(PaperColor,   SIGNAL(triggered()), SignalMapper, SLOT(map()));

    SignalMapper->setMapping(ConsoleColor, 1);
    SignalMapper->setMapping(DuskColor,    2);
    SignalMapper->setMapping(HackerColor,  3);
    SignalMapper->setMapping(LightColor,   4);
    SignalMapper->setMapping(NavyColor,    5);
    SignalMapper->setMapping(NightColor,   6);
    SignalMapper->setMapping(NormalColor,  7);
    SignalMapper->setMapping(PaperColor,   8);

    connect(SignalMapper, SIGNAL(mapped(int)), this, SLOT(CreateStyle(int)));

    connect(structure,    SIGNAL(itemPressed(QTreeWidgetItem*,int)), this, SLOT(jumpToTag(QTreeWidgetItem*,int)));
    connect(this,SIGNAL(nowRefreshStructure()),this,
            SLOT(reloadTree())
            );

    connect(this, SIGNAL(nowHighlight()),this, SLOT(setupHighlighter()));

    //connect(TextEdit->document(),       SIGNAL(contentsChange (int,int,int)  ), this, SLOT(onTextChange(int,int,int)));
    connect(TextEdit->document(), SIGNAL(modificationChanged(bool)), this, SLOT(onModificationChanged(bool)));
    connect(TextEdit,      SIGNAL(cursorPositionChanged ()), this, SLOT(renderEquation()) );

    connect(runCommand, SIGNAL(triggered()), this, SLOT(onRun()));
    connect(previewPdf, SIGNAL(triggered()),this,SLOT(preview()));
    connect(configCommand, SIGNAL(triggered()), this, SLOT(onConfig())     );

    connect(TextEdit, SIGNAL(askToComplete(QString)), this, SLOT(completeWith(QString)));
    connect(TextEdit, SIGNAL(askToWiki(QString)), this, SLOT(showWiki(QString)));
    connect(TextEdit, SIGNAL(askToDict(QString)), this, SLOT(showDict(QString)));
    connect(TextEdit, SIGNAL(askToGoogleSearch()),this,SLOT(goToGoogleSearch())  );
    connect(TextEdit, SIGNAL(askToGinger(QString)), this, SLOT(showGinger(QString)));
    connect(TextEdit, SIGNAL(askToRealTimePreview()), this,SLOT(realtimePreview()));

    //connect(SpellCheck, SIGNAL(triggered()), this, SLOT(ToggleSpellcheck()));

    connect(SpellCheck_None,SIGNAL(triggered()), this, SLOT(onSpellLang()   ));
    connect(SpellCheck_GB,SIGNAL(triggered()), this, SLOT(onSpellLang()   ));
    connect(SpellCheck_US,SIGNAL(triggered()), this, SLOT(onSpellLang()   ));

    connect(GingerPolish, SIGNAL(triggered()),this, SLOT( onGingerPolish()   ));

}




void MainWindow::Initialize() {
    // Create a new TextEdit and a new SearchDialog
    //TextEdit = new QPlainTextEdit;
    theFileSaver = new FileSaver(this);
    TextEdit = new QTeXEdit;
    //TextEdit->installEventFilter(this);

    SearchDialog = new FindDialog(this);
    SearchDialog->SetTextEdit(TextEdit);

    autoComp = new Completor(TextEdit);
    autoComp->hide();

    TextEdit->setCompleter(autoComp);

    structureThread = NULL;

    realtimepolish=false;

    comSec="";//for auto-completion

    isStar=false;//check whether the content has changed

    proc=NULL;
    run = new RunThread(this);
    run->setWindow(this);
    //connect(run,SIGNAL(updateStatus(QString)),this,SLOT(onCmdUpdateStatus(QString)));
#ifdef USE_WEBKIT
    ag=NULL;
#endif

    // Set the TextEdit as the central widget of the window
    setCentralWidget(TextEdit);

    highlighter = NULL;//new Highlighter
            //new LaTeXHighlighter(TextEdit->document());//set a highlighter

    // Set the icon of the application
    qApp->setWindowIcon(QIcon(":/icons/icons/texpen.png"));

    // Create the rest of the GUI
    CreateActions();
    CreateMenus();
    CreateToolbar();
    CreateStatusbar();
    CreateOutlineWidget();//outline
    CreateEquationview();// equation preview
    CreateWikiView();

    // Connect the actions with the slots and load the settings
    ConnectSlots();
    ReadSettings();

    // Create a new document
    CreateNewDocument();

    if(highlighter!=NULL){
        delete highlighter;
        highlighter= NULL;
        TextEdit->setHighlighter(NULL);
    }
    emit (nowHighlight());


}

void MainWindow::CreateActions() {
    // Set the label and icon of each action
    AboutAction       = new QAction(QIcon(":/icons/icons/24x24/about.png"),       tr("About TeXpen"), this);
    DonateAction      = new QAction(QIcon(":/icons/icons/24x24/coin.png"),        tr("Make Donation..."), this);
    AboutQtAction     = new QAction(QIcon(""),                                    tr("About Qt"),      this);
    CopyAction        = new QAction(QIcon(":/icons/icons/32/editcopy.png"),        tr("Copy"),          this);
    CustomModeAction  = new QAction(QIcon(),                                      tr("Custom"),        this);
    CutAction         = new QAction(QIcon(":/icons/icons/32/editcut.png"),         tr("Cut"),           this);
    InsertTable       = new QAction(QIcon(":/icons/icons/24x24/table.png"),       tr("Insert Table"),  this);
    InsertGraphic     = new QAction(QIcon(":/icons/icons/24x24/graphic.png"),     tr("Insert Graphic"),this);
    DateTimeAction    = new QAction(QIcon(":/icons/icons/24x24/time.png"),        tr("Date/Time"),     this);
    FindAction        = new QAction(QIcon(":/icons/icons/32/filefind.png"),      tr("Search..."),     this);
    FontsAction       = new QAction(QIcon(":/icons/icons/24x24/font.png"),        tr("Fonts..."),      this);
    FullscreenAction  = new QAction(QIcon(":/icons/icons/32/gtk-fullscreen.png"),  tr("Fullscreen"),    this);
    HelpAction        = new QAction(QIcon(":/icons/icons/24x24/help.png"),        tr("View Help"),     this);
    MinimalModeAction = new QAction(QIcon(),                                      tr("Minimal Mode"),  this);
    NewAction         = new QAction(QIcon(":/icons/icons/32/document-new.png"),         tr("New"),           this);
    NewArticleAction  = new QAction(QIcon(),                                      tr("New Article..."),this);
    NewSlideAction  = new QAction(QIcon(),                                        tr("New Slide..."), this);
    NormalModeAction  = new QAction(QIcon(),                                      tr("Normal Mode"),   this);
    OpenAction        = new QAction(QIcon(":/icons/icons/32/document-open.png"),        tr("Open"),          this);
    PasteAction       = new QAction(QIcon(":/icons/icons/32/editpaste.png"),       tr("Paste"),         this);
    PrintAction       = new QAction(QIcon(":/icons/icons/32/document-print.png"),       tr("Print..."),      this);
    QuitAction        = new QAction(QIcon(":/icons/icons/24x24/close.png"),       tr("Exit"),          this);
    RedoAction        = new QAction(QIcon(":/icons/icons/32/edit-redo.png"),        tr("Redo"),          this);
    SaveAction        = new QAction(QIcon(":/icons/icons/32/document-save.png"),        tr("Save"),          this);
    SaveAsAction      = new QAction(QIcon(":/icons/icons/32/document-save-as.png"),     tr("Save As..."),    this);
    SelectAllAction   = new QAction(QIcon(":/icons/icons/24x24/select-all.png"),  tr("Select All..."), this);
    SetReadOnlyAction = new QAction(QIcon(":/icons/icons/24x24/lock.png"),        tr("Set Read Only"), this);
    StatusbarAction   = new QAction(QIcon(""),                                    tr("Statusbar"),     this);
    ToolbarAction     = new QAction(QIcon(""),                                    tr("Toolbar"),       this);
    OutlineAction     = new QAction(QIcon(""),                                    tr("Outline"),       this);
    UndoAction        = new QAction(QIcon(":/icons/icons/32/edit-undo.png"),        tr("Undo"),          this);
    WordWrapAction    = new QAction(QIcon(""),                                    tr("Wordwrap"),      this);

    ConsoleColor = new QAction(tr("Console color scheme."), this);
    DuskColor    = new QAction(tr("Dusk color scheme."),    this);
    HackerColor  = new QAction(tr("Hacker color scheme."),  this);
    LightColor   = new QAction(tr("Light color scheme."),   this);
    NavyColor    = new QAction(tr("Navy color scheme."),    this);
    NightColor   = new QAction(tr("Night color scheme."),   this);
    NormalColor  = new QAction(tr("Normal color scheme."),  this);
    PaperColor   = new QAction(tr("Paper color scheme,"),   this);

    SeparatorAction = new QAction(this);
    SeparatorAction->setSeparator(true);

    QActionGroup *ColorSchemeActions = new QActionGroup(this);
    ColorSchemeActions->addAction(ConsoleColor);
    ColorSchemeActions->addAction(DuskColor);
    ColorSchemeActions->addAction(HackerColor);
    ColorSchemeActions->addAction(LightColor);
    ColorSchemeActions->addAction(NavyColor);
    ColorSchemeActions->addAction(NightColor);
    ColorSchemeActions->addAction(NormalColor);
    ColorSchemeActions->addAction(PaperColor);

    QActionGroup *ModesActions = new QActionGroup(this);
    ModesActions->addAction(CustomModeAction);
    ModesActions->addAction(MinimalModeAction);
    ModesActions->addAction(NormalModeAction);

    // Set the statustip of each action (the text that the statusbar displays when an item is hovered)
    AboutAction->setStatusTip(tr("View information about TeXpen"));
    DonateAction->setStatusTip(tr("Make a donation to TeXpen, will open a web page..."));
    AboutQtAction->setStatusTip(tr("View information about the Qt Framework"));
    CopyAction->setStatusTip(tr("Copy the selected text"));
    InsertTable->setStatusTip(tr("Insert a Table at the current position"));
    InsertGraphic->setStatusTip(tr("Insert a Graphic at the current position"));
    CustomModeAction->setStatusTip(tr(""));
    CutAction->setStatusTip(tr("Cut the selected text"));
    DateTimeAction->setStatusTip(tr("Insert the current date and time at the end of the document"));
    FindAction->setStatusTip(tr("Search for something in the current document"));
    FontsAction->setStatusTip(tr("Select the font to be shown while editing documents"));
    HelpAction->setStatusTip(tr("View help"));
    MinimalModeAction->setStatusTip(tr("Hide the toolbar, statusbar and the menubar"));
    NewAction->setStatusTip(tr("Create a new document"));
    NewArticleAction->setStatusTip(tr("Create a new document using article template"));
    NewSlideAction->setStatusTip(tr("Create a new document using Beamer template (slides)"));
    NormalModeAction->setStatusTip(tr("Show the toolbar, statusbar and the menubar"));
    OpenAction->setStatusTip(tr("Open a file in your computer"));
    PasteAction->setStatusTip(tr("Paste text from the system's clipboard"));
    PrintAction->setStatusTip(tr("Send the current document to a printer"));
    QuitAction->setStatusTip(tr("Exit TeXpen"));
    RedoAction->setStatusTip(tr("Redo the last action"));
    SaveAction->setStatusTip(tr("Save the current document"));
    SaveAsAction->setStatusTip(tr("Save the current document with another name"));
    SelectAllAction->setStatusTip(tr("Select all the text in the document"));
    SetReadOnlyAction->setStatusTip(tr("Lock the current document so you can't edit it"));
    StatusbarAction->setStatusTip(tr("Enable/Disable the statusbar"));
    ToolbarAction->setStatusTip(tr("Enable/Disable the toolbar"));
    OutlineAction->setStatusTip(tr("Show/Hide the Outline dock pane"));
    UndoAction->setStatusTip(tr("Undo the last action"));
    WordWrapAction->setStatusTip(tr("Enable/Disable Word wrapping"));

    // Set the shortcuts of each action
    NewAction->setShortcut          (QKeySequence(Qt::CTRL + Qt::Key_N));
    OpenAction->setShortcut         (QKeySequence(Qt::CTRL + Qt::Key_O));
    SaveAction->setShortcut         (QKeySequence(Qt::CTRL + Qt::Key_S));
    SaveAsAction->setShortcut       (QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
    PrintAction->setShortcut        (QKeySequence(Qt::CTRL + Qt::Key_P));
    QuitAction->setShortcut         (QKeySequence(Qt::CTRL + Qt::Key_Q));
    UndoAction->setShortcut         (QKeySequence(Qt::CTRL + Qt::Key_Z));
    RedoAction->setShortcut         (QKeySequence(Qt::CTRL + Qt::Key_Y));
    CutAction->setShortcut          (QKeySequence(Qt::CTRL + Qt::Key_X));
    CopyAction->setShortcut         (QKeySequence(Qt::CTRL + Qt::Key_C));
    PasteAction->setShortcut        (QKeySequence(Qt::CTRL + Qt::Key_V));
    FindAction->setShortcut         (QKeySequence(Qt::CTRL + Qt::Key_F));
    SetReadOnlyAction->setShortcut  (QKeySequence(Qt::ALT  + Qt::Key_R));
    SelectAllAction->setShortcut    (QKeySequence(Qt::CTRL + Qt::Key_A));
    InsertTable->setShortcut        (QKeySequence(Qt::CTRL + Qt::Key_T));
    InsertGraphic->setShortcut      (QKeySequence(Qt::CTRL + Qt::Key_G));
    DateTimeAction->setShortcut     (QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_D));
    FontsAction->setShortcut        (QKeySequence(Qt::Key_F4));
    WordWrapAction->setShortcut     (QKeySequence(Qt::ALT  + Qt::Key_W));
    FullscreenAction->setShortcut   (QKeySequence(Qt::Key_F11));
    ToolbarAction->setShortcut      (QKeySequence(Qt::ALT  + Qt::Key_T));
    OutlineAction->setShortcut      (QKeySequence(Qt::ALT  + Qt::Key_O));
    StatusbarAction->setShortcut    (QKeySequence(Qt::ALT  + Qt::Key_S));
    NormalModeAction->setShortcut   (QKeySequence(Qt::ALT  + Qt::Key_N));
    MinimalModeAction->setShortcut  (QKeySequence(Qt::ALT  + Qt::Key_M));
    HelpAction->setShortcut         (QKeySequence(Qt::Key_F1));
    AboutQtAction->setShortcut      (QKeySequence(Qt::Key_F2));
    AboutAction->setShortcut        (QKeySequence(Qt::Key_F3));

    // Set additional properties of some actions
    CustomModeAction->setCheckable(true);
    CustomModeAction->setEnabled(false);
    FullscreenAction->setCheckable(true);
    MinimalModeAction->setCheckable(true);
    NormalModeAction->setCheckable(true);
    SetReadOnlyAction->setCheckable(true);
    StatusbarAction->setCheckable(true);
    ToolbarAction->setCheckable(true);
    OutlineAction->setCheckable(true);
    WordWrapAction->setCheckable(true);
    SetReadOnlyAction->setCheckable(true);

    // Color scheme actions
    ConsoleColor->setCheckable(true);
    DuskColor->setCheckable(true);
    HackerColor->setCheckable(true);
    LightColor->setCheckable(true);
    NavyColor->setCheckable(true);
    NightColor->setCheckable(true);
    NormalColor->setCheckable(true);
    PaperColor->setCheckable(true);

    //command actions
    runCommand=new QAction(QIcon(":/icons/icons/24x24/media-playback-start.png"),        tr("Build"),          this);
    configCommand=new QAction(QIcon(":/icons/icons/32/gnome-run.png"),        tr("Config"),          this);
    previewPdf = new QAction(QIcon(":/icons/icons/32/document-page-setup.png"),        tr("View PDF"),          this);

    runCommand->setShortcut(QKeySequence(Qt::Key_F6));
    configCommand->setShortcut        (QKeySequence(Qt::Key_F10));
    previewPdf->setShortcut(QKeySequence(Qt::Key_F7));

    runCommand->setStatusTip(tr("Run the command you specified(default is \"latex $thedoc\")"));
    configCommand->setStatusTip(tr("Specify the command and other configurations"));
    previewPdf->setStatusTip(tr("Preview the generated PDF file."));


    SpellCheck      = new QAction(tr("Spell-Check"), this);

    SpellCheck_None = new QAction(tr("None"), this); SpellCheck_None->setData(QString("None"));
    SpellCheck_GB   = new QAction(tr("en_GB"), this); SpellCheck_GB->setData(QString("en_GB"));
    SpellCheck_US   = new QAction(tr("en_US"), this); SpellCheck_US->setData(QString("en_US"));
    //SpellCheck->setCheckable(true);
    SpellCheck_None->setCheckable(true);
    SpellCheck_GB->setCheckable(true);
    SpellCheck_US->setCheckable(true);
    QActionGroup *SpellActions = new QActionGroup(this);
    //SpellActions->addAction(SpellCheck);
    SpellActions->addAction(SpellCheck_None);
    SpellActions->addAction(SpellCheck_GB);
    SpellActions->addAction(SpellCheck_US);

    GingerPolish = new QAction(tr("Style Check..."), this);
    GingerPolish->setCheckable(true);



    ConfigSpellCheck= new QAction(tr("Spell-checking language..."), this);


}

void MainWindow::CreateMenus() {
    // Create the main menus (File, Edit, Format, View and Help)
    FileMenu = menuBar()->addMenu(tr("File"));
    EditMenu = menuBar()->addMenu(tr("Edit"));
    FormatMenu = menuBar()->addMenu(tr("Format"));
    ViewMenu = menuBar()->addMenu(tr("View"));
    ToolMenu = menuBar()->addMenu(tr("Tool"));
    HelpMenu = menuBar()->addMenu(tr("Help"));

    // Setup the "File" Menu
    FileMenu->addAction(NewAction);
    NewDocMenu = FileMenu->addMenu(tr("New with a template..."));
    NewDocMenu->addAction(NewArticleAction);
    NewDocMenu->addAction(NewSlideAction);
    FileMenu->addAction(OpenAction);
    FileMenu->addAction(SaveAction);
    FileMenu->addSeparator();
    FileMenu->addAction(SaveAsAction);
    FileMenu->addSeparator();
    FileMenu->addAction(PrintAction);
    FileMenu->addSeparator();
    RecentFiles = FileMenu->addMenu(tr("Recent Files..."));
    FileMenu->addSeparator();
    FileMenu->addAction(QuitAction);

    // Setup the "Edit" Menu
    EditMenu->addAction(UndoAction);
    EditMenu->addAction(RedoAction);
    EditMenu->addSeparator();
    EditMenu->addAction(CutAction);
    EditMenu->addAction(CopyAction);
    EditMenu->addAction(PasteAction);
    EditMenu->addSeparator();
    EditMenu->addAction(FindAction);
    EditMenu->addSeparator();
    EditMenu->addAction(SelectAllAction);
    EditMenu->addSeparator();
    EditMenu->addAction(InsertTable);
    EditMenu->addAction(InsertGraphic);
    EditMenu->addAction(DateTimeAction);
    EditMenu->addSeparator();
    EditMenu->addAction(SetReadOnlyAction);

    // Setup the "Format" Menu
    FormatMenu->addAction(FontsAction);
    FormatMenu->addSeparator();
    FormatMenu->addAction(WordWrapAction);

    // Setup the "View" Menu
    ViewMenu->addAction(ToolbarAction);
    ViewMenu->addAction(OutlineAction);
    ViewMenu->addAction(StatusbarAction);
    ViewMenu->addSeparator();
    ModesMenu = ViewMenu->addMenu(tr("Modes"));
    ModesMenu->addAction(MinimalModeAction);
    ModesMenu->addAction(NormalModeAction);
    ModesMenu->addSeparator();
    ModesMenu->addAction(CustomModeAction);
    ViewMenu->addSeparator();
    ColorschemesMenu = ViewMenu->addMenu(tr("Color schemes"));
    ColorschemesMenu->addAction(NormalColor);
    ColorschemesMenu->addSeparator();
    ColorschemesMenu->addAction(ConsoleColor);
    ColorschemesMenu->addAction(DuskColor);
    ColorschemesMenu->addAction(HackerColor);
    ColorschemesMenu->addAction(LightColor);
    ColorschemesMenu->addAction(NavyColor);
    ColorschemesMenu->addAction(NightColor);
    ColorschemesMenu->addAction(PaperColor);
    ViewMenu->addSeparator();
    ViewMenu->addAction(FullscreenAction);

    //Setup the "Tool" Menu
    ToolMenu ->addAction(runCommand);
    ToolMenu->addAction(previewPdf);
    ToolMenu ->addAction(configCommand);
    ToolMenu->addSeparator();

    SpellMenu = ToolMenu->addMenu(tr("Spell-checking"));

    SpellMenu ->addAction(SpellCheck_None);
    SpellMenu->addSeparator();
    SpellMenu->addAction(SpellCheck_GB);
    SpellMenu->addAction(SpellCheck_US);

    //SpellMenu ->addAction(ConfigSpellCheck);

    ToolMenu->addAction(GingerPolish);

    // Setup the "Help" Menu
    HelpMenu->addAction(HelpAction);
    HelpMenu->addSeparator();
    HelpMenu->addAction(AboutAction);
    HelpMenu->addAction(AboutQtAction);
    HelpMenu->addAction(DonateAction);
}

void MainWindow::CreateToolbar() {
    // Create the Toolbar
    Toolbar = addToolBar("Toolbar");
    Toolbar->setObjectName("Toolbar");
    Toolbar->setMovable(false);

    //Toolbar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    Toolbar->setIconSize(QSize(16,16));
    setUnifiedTitleAndToolBarOnMac(true);

    // Add some actions to the toolbar
    Toolbar->addAction(NewAction);
    Toolbar->addAction(OpenAction);
    Toolbar->addAction(SaveAction);
    Toolbar->addSeparator();
    Toolbar->addAction(PrintAction);
    Toolbar->addSeparator();
    Toolbar->addAction(UndoAction);
    Toolbar->addAction(RedoAction);
    //Toolbar->addAction(FontsAction);
    Toolbar->addSeparator();
    Toolbar->addAction(CutAction);
    Toolbar->addAction(CopyAction);
    Toolbar->addAction(PasteAction);
    Toolbar->addSeparator();
    Toolbar->addAction(InsertTable);
    Toolbar->addAction(InsertGraphic);
    Toolbar->addSeparator();
    Toolbar->addAction(FindAction);
    Toolbar->addSeparator();
    Toolbar->addAction(runCommand);
    Toolbar->addAction(previewPdf);
    Toolbar->addAction(configCommand);
    Toolbar->addSeparator();
    Toolbar->addAction(HelpAction);
    Toolbar->addAction(DonateAction);
    Toolbar->addAction(AboutAction);

    Toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    //Toolbar->
}

#include <QTreeWidget>
void MainWindow::CreateOutlineWidget(){
    outline = new QDockWidget(this);
    outline->setWindowTitle("Outline");
    outline->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    outline->setFloating(false);
    outline ->setFeatures(QDockWidget::NoDockWidgetFeatures);
    structure = new QTreeWidget(outline);
    structure->setHeaderHidden(true);
    outline->setWidget(structure);
    addDockWidget(Qt::LeftDockWidgetArea, outline);
    structure->setColumnCount(1);
    structure->setHeaderLabel(QString::fromUtf8("Outline"));
    QFrame* af = new QFrame(this);
    af->setFixedHeight(1);
    outline->setTitleBarWidget(af);
}

void MainWindow::CreateEquationview(){
    //equationview


    equationview = new QDockWidget(this);
    equationview->setWindowTitle("Preview");
    equationview->setAllowedAreas(Qt::BottomDockWidgetArea );
    equationview->setFloating(false);
    equationview ->setFeatures(QDockWidget::NoDockWidgetFeatures);
    equationview ->setMaximumHeight(70);
    //equationview ->resize(this->width(),60);
    equationview ->setGeometry(0,0,this->width(),60);
            //setGeometry(0,0,200,50);
    QFrame* af = new QFrame(this);
    af->setFixedHeight(2);
    equationview->setTitleBarWidget(af);
#ifdef USE_WEBKIT
    equation = new QWebView;
#endif
#ifdef USE_QT_WEB_ENGINE
    equation = new QWebEngineView;
#endif
    equationview->setWidget(equation);
    addDockWidget(Qt::BottomDockWidgetArea, equationview);
    //equation->load(QUrl("http://chart.apis.google.com/chart?cht=tx&chl=\\sum \\alpha\\geq\\frac{\\beta}{\\sum a}\\text{good yes}"));
    //qDebug()<<"判断当前光标，是否处在公式环境中，如果是，则调用api"<<endl;
    equationview->hide();
}
#ifdef USE_WEBKIT
#include <QWebFrame>
#endif
void MainWindow::CreateWikiView(){
    wikiview = new QDockWidget(this);
    wikiview->setWindowTitle("Wikipedia");
        wikiview->setAllowedAreas(Qt::BottomDockWidgetArea );
        wikiview->setFloating(false);
        wikiview ->setFeatures(QDockWidget::NoDockWidgetFeatures);
        wikiview ->setMaximumWidth(300);
        wiki = new WebView;
        wikiview->setWidget(wiki);
        addDockWidget(Qt::RightDockWidgetArea, wikiview);
        wikiview->hide();
        connect(wiki, SIGNAL(askToHideSideWidget()),this,SLOT(hideSideView()));
        connect(wiki, SIGNAL(loadFinished(bool)),this,SLOT(ParseGinger()));
#ifdef USE_WEBKIT
        connect(wiki->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),this,SLOT(bindJSobj()));

        bindJSobj();

        QFrame* af = new QFrame(this);
        af->setFixedHeight(1);
        wikiview->setTitleBarWidget(af);
#endif

}

void MainWindow::CreateStatusbar() {
    // Add the DocumentSizeLabel (where the words and size of the document are shown)
    DocumentSizeLabel = new QLabel(this);
    DocumentSizeLabel->setAlignment(Qt::AlignRight);
    statusBar()->addPermanentWidget(DocumentSizeLabel);
}
