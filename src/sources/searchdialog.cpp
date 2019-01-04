// Copyright (C) 2013  Alex S. <alex_sr97@outlook.com>
// See the "Copying.txt" file that comes with this distribution

#include "headers/searchdialog.h"
#include <QCheckBox>
#include <QSpacerItem>
#include <QGridLayout>
#ifdef QT5SDK
#include <QtConcurrent/QtConcurrent>
#else
#include <QtConcurrentRun>
#endif
FindDialog::FindDialog(QWidget *parent):
    QDialog(parent)
{
    setWindowTitle(tr("Search and Replace"));
    CreateWidgets();
    if(parent!=NULL){
        mw = (MainWindow*)parent;
    }else{
        mw = NULL;
    }
}

void FindDialog::CreateWidgets() {
    // Remove the window icon in Mac OS X
#ifdef Q_OS_MAC
    QIcon Icon;
    setWindowIcon(Icon);
#endif

    // Create the widgets
    Layout            = new QGridLayout(this);
    FindLabel         = new QLabel(this);
    ReplaceLabel      = new QLabel(this);
    MatchCaseCheckbox = new QCheckBox(this);
    FindLineEdit      = new QLineEdit(this);
    ReplaceLineEdit   = new QLineEdit(this);
    CancelButton      = new QPushButton(this);
    ReplaceAllButton  = new QPushButton(this);
    ReplaceButton     = new QPushButton(this);
    NextButton        = new QPushButton(this);

    // Change the text
    FindLabel->setText         (tr("Find what:"));
    ReplaceLabel->setText      (tr("Replace with:"));
    MatchCaseCheckbox->setText (tr("Match case"));
    CancelButton->setText      (tr("Cancel"));
    ReplaceAllButton->setText  (tr("Replace All"));
    ReplaceButton->setText     (tr("Replace"));
    NextButton->setText        (tr("Find"));

    // Set the VerticalSpacer
    VerticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    // Set the layout
    Layout->setSpacing(10);
    Layout->addWidget(FindLabel,         1, 1, 1, 1);
    Layout->addWidget(FindLineEdit,      1, 2, 1, 2);
    Layout->addWidget(NextButton,        1, 4, 1, 2);
    Layout->addWidget(ReplaceLabel,      2, 1, 1, 1);
    Layout->addWidget(ReplaceLineEdit,   2, 2, 1, 2);
    Layout->addWidget(ReplaceAllButton,  2, 5, 1, 1);
    Layout->addWidget(ReplaceButton,     2, 4, 1, 1);
    Layout->addWidget(CancelButton,      4, 4, 3, 3);
    Layout->addWidget(MatchCaseCheckbox, 4, 1, 3, 1);
    Layout->addItem  (VerticalSpacer,    3, 1, 3, 1);

    // Set the fixed size
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    // Connect the slots
    connect(CancelButton,      SIGNAL(clicked()),            this, SLOT(OnCancel()));
    connect(FindLineEdit,      SIGNAL(textChanged(QString)), this, SLOT(ReadyFind()));
    connect(MatchCaseCheckbox, SIGNAL(clicked()),            this, SLOT(ReadyFind()));
    connect(ReplaceAllButton,  SIGNAL(clicked()),            this, SLOT(ReplaceAll()));
    connect(ReplaceButton,     SIGNAL(clicked()),            this, SLOT(ReplaceFirstOccurence()));
    connect(NextButton,        SIGNAL(clicked()),            this, SLOT(FindNext()));

    // Disable Replace Functions
    ReplaceAllButton->setEnabled(false);
    ReplaceButton->setEnabled(false);
    ReplaceLineEdit->setEnabled(false);
    NextButton->setEnabled(false);
}

void FindDialog::SetTextEdit(QTeXEdit *TextEdit_) {
    TextEdit = TextEdit_;
}

void FindDialog::show(){
    NextButton->setText        (tr("Find"));
    FindLineEdit->setText("");
    ReplaceLineEdit->setText("");
    ReplaceAllButton->setEnabled(false);
    ReplaceButton->setEnabled(false);
    NextButton->setEnabled(false);
    ReplaceLineEdit->setEnabled(false);
    FindLineEdit->setFocus();
    QDialog::show();

}
void FindDialog::ReadyFind(){
    //searchingStr=FindLineEdit->text();
    NextButton->setText        (tr("Find"));
    QTextDocument::FindFlags Flags;
    if (MatchCaseCheckbox->isChecked()) {
        Flags |= QTextDocument::FindCaseSensitively;
    }
    TextEdit->moveCursor(QTextCursor::Start);
    bool rs = TextEdit->find(FindLineEdit->text(), Flags);
    if(!rs){
        ReplaceAllButton->setEnabled(false);
        ReplaceButton->setEnabled(false);
        NextButton->setEnabled(false);
        NextButton->setText(tr("Find"));
    }else{
        ReplaceAllButton->setEnabled(true);
        ReplaceButton->setEnabled(true);
        NextButton->setEnabled(true);
        ReplaceLineEdit->setEnabled(true);
        NextButton->setText(tr("Next"));
    }
}

void FindDialog::FindNext(){
    QTextDocument::FindFlags Flags;
    if (MatchCaseCheckbox->isChecked()) {
        Flags |= QTextDocument::FindCaseSensitively;
    }
    TextEdit->find(FindLineEdit->text(), Flags);
}

bool isSearching = false;
QString searchingStr="";
//void toHighlight(Highlighter* hl, QString txt, bool cs){
//    if(isSearching){
//        return;
//    }
//    isSearching = true;
//    hl->setFind(txt,cs);
////    if(searchingStr!=txt){
////        qDebug()<<"rerehighlighting..."<<endl;
////        txt = searchingStr;
////        hl->setFind(txt,cs);
////    }
//    isSearching = false;
//}

void FindDialog::SearchText() {

    searchingStr=FindLineEdit->text();

    if(isSearching){
        return;
    }

    QTextDocument::FindFlags Flags;
    if (MatchCaseCheckbox->isChecked()) {
        Flags |= QTextDocument::FindCaseSensitively;
    }

    if (FindLineEdit->text() != "") {
        ReplaceAllButton->setEnabled(true);
        ReplaceButton->setEnabled(true);
        ReplaceLineEdit->setEnabled(true);
        NextButton->setEnabled(true);

        // Search for something (and highlight the results)
        {
            //
            bool caseSensitive=false;
            if(Flags|QTextDocument::FindCaseSensitively){
                caseSensitive= true;
            }
            if(mw!=NULL){
                //mw->getHighlighter()->setFind(FindLineEdit->text(),caseSensitive);
                //QtConcurrent::run(toHighlight,mw->getHighlighter(), FindLineEdit->text(),caseSensitive);
            }
            TextEdit->moveCursor(QTextCursor::Start);
            bool rs = TextEdit->find(FindLineEdit->text(), Flags);
            if(!rs){
                ReplaceAllButton->setEnabled(false);
                ReplaceButton->setEnabled(false);
                NextButton->setEnabled(false);
            }else{
                ReplaceAllButton->setEnabled(true);
                ReplaceButton->setEnabled(true);
                NextButton->setEnabled(true);
                ReplaceLineEdit->setEnabled(true);
            }
            /*
            QList<QTextEdit::ExtraSelection> ExtraSelections;
            TextEdit->moveCursor(QTextCursor::End);

            while (TextEdit->find(FindLineEdit->text(), Flags | QTextDocument::FindBackward)) {
                QTextEdit::ExtraSelection Selection;
                Selection.format.setBackground(QColor(Qt::yellow).lighter(160));
                Selection.format.setForeground(QColor(Qt::black));
                Selection.cursor = TextEdit->textCursor();
                ExtraSelections.append(Selection);
            }
            TextEdit->setExtraSelections(ExtraSelections);
            */
        }
    }

    // Disable replace functions
    else {
        ReplaceAllButton->setEnabled(false);
        ReplaceButton->setEnabled(false);
        ReplaceLineEdit->setEnabled(false);
        if(mw!=NULL){
            mw->getHighlighter()->setFind("",true);
        }
        //TextEdit->setExtraSelections();
    }




}

void FindDialog::ReplaceAll() {
    QTextDocument::FindFlags Flags;
    if (MatchCaseCheckbox->isChecked()) {
        Flags |= QTextDocument::FindCaseSensitively;
    }

    int i = 0;
    while (TextEdit->find(FindLineEdit->text(), Flags)) {
        while (TextEdit->textCursor().hasSelection()) {
            TextEdit->moveCursor(QTextCursor::Start);
            while (TextEdit->find(FindLineEdit->text(), Flags)) {
                TextEdit->textCursor().insertText(ReplaceLineEdit->text());
                i++;
            }
        }
    }
}

void FindDialog::ReplaceFirstOccurence() {
    QTextDocument::FindFlags Flags;
    if (MatchCaseCheckbox->isChecked()) {
        Flags |= QTextDocument::FindCaseSensitively;
    }
    if (!TextEdit->textCursor().hasSelection()){
        TextEdit->find(FindLineEdit->text(), Flags);
    }
    //SearchText();
    if (TextEdit->textCursor().hasSelection()){
        TextEdit->textCursor().insertText(ReplaceLineEdit->text());
        TextEdit->find(FindLineEdit->text(), Flags);
    }

    else
        QMessageBox::warning(this, tr("Warning"), tr("No matches where found"));
}

void FindDialog::OnCancel(){
    //if(mw!=NULL){
    //    mw->getHighlighter()->setFind("",true);
    //}
    hide();
}
