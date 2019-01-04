#ifndef FINDDIALOG_H
#define FINDDIALOG_H

//#include "src/objects.hh"
#include "headers/mainwindow.h"
#include "headers/qtexedit.h"
#include <QCheckBox>
#include <QSpacerItem>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
class MainWindow;

class FindDialog: public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent = 0);

    QTextCursor *TextCursor;
    //QPlainTextEdit *TextEdit;
    QTeXEdit* TextEdit;
    MainWindow* mw;

public slots:
    void SetTextEdit(QTeXEdit *TextEdit_);
    void show();

private slots:
    void CreateWidgets();
    void SearchText();
    void ReplaceAll();
    void ReplaceFirstOccurence();
    void OnCancel();
    void FindNext();
    void ReadyFind();

private:
    QLabel *FindLabel;
    QLabel *ReplaceLabel;
    QCheckBox *MatchCaseCheckbox;
    QLineEdit *FindLineEdit;
    QLineEdit *ReplaceLineEdit;
    QGridLayout *Layout;
    QPushButton *NextButton;
    QPushButton *CancelButton;
    QPushButton *ReplaceAllButton;
    QPushButton *ReplaceButton;
    QSpacerItem *VerticalSpacer;
};

#endif
