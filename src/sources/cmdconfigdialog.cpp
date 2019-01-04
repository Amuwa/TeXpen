#include "../headers/cmdconfigdialog.h"
#include "ui_cmdconfigdialog.h"
#include <QDebug>
#include <QFileDialog>

CmdConfigDialog::CmdConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CmdConfigDialog)
{
    ui->setupUi(this);
}

CmdConfigDialog::~CmdConfigDialog()
{
    delete ui;
}

void CmdConfigDialog::setParas(QString cmd, QString dir){
    Cmd = cmd;
    Dir =dir;
    ui->CmdEdit->setText(cmd);
    ui->DirEdit->setText(dir);
}

void CmdConfigDialog::on_cmdSelect_clicked()
{
    QFileDialog af;
    QString rs = af.getOpenFileName(this,tr("Select the Command..."),QDir::homePath());
    if(rs.isEmpty()){
        return;
    }
    ui->CmdEdit->setText(rs);
}

void CmdConfigDialog::on_dirSelect_clicked()
{
    QFileDialog af;
    QString rs = af.getExistingDirectory(this,tr("Select Working Directory..."),QDir::homePath());
    if(rs.isEmpty()){
        return;
    }
    ui->DirEdit->setText(rs);
}

void CmdConfigDialog::on_buttonBox_accepted()
{
    Cmd = ui->CmdEdit->text();
    Dir = ui->DirEdit->text();
    QDialog::accept();
}

void CmdConfigDialog::on_buttonBox_rejected()
{
    QDialog::reject();
}
