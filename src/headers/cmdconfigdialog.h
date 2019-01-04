#ifndef CMDCONFIGDIALOG_H
#define CMDCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class CmdConfigDialog;
}

class CmdConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CmdConfigDialog(QWidget *parent = 0);
    ~CmdConfigDialog();
    void setParas(QString cmd, QString dir);
    QString Cmd;
    QString Dir;
    
private slots:

    void on_cmdSelect_clicked();

    void on_dirSelect_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::CmdConfigDialog *ui;
};

#endif // CMDCONFIGDIALOG_H
