#include "headers/mainwindow.h"

void MainWindow::insertEquation(){
    QString eq="\n\\begin{equation}\n\tE=mc^{2}\n\\end{equation}\n";
    TextEdit->insertPlainText(eq);
    UpdateWindowTitle();
}


QStringList cmds;

void setupCmds(){
    if(cmds.isEmpty()){
        //<<"::figure"<<"::table"<<"::eq"
        cmds.append("::figure");
        cmds.append("::table");
        cmds.append("::equation");
        cmds.append("::article");
        cmds.append("::beamer");
    }
}
