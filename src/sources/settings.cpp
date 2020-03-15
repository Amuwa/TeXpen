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

#include "QSettings"
#include "headers/mainwindow.h"
#include "headers/cmdconfigdialog.h"

#include <QFileInfo>
#include <QDir>

void MainWindow::ApplySettings() {
    // Fullscreen settings
    {
        if (Settings.FullScreen == false) {
            if(!isMaximized()){
                showNormal();
            }

            Toolbar->removeAction(SeparatorAction);
            Toolbar->removeAction(FullscreenAction);

        }
        else {
            showFullScreen();

            Toolbar->addAction(SeparatorAction);
            Toolbar->addAction(FullscreenAction);
        }
    }

    // Statusbar settings
    {
        if (Settings.StatusBarVisible == false) statusBar()->hide();
        else statusBar()->show();
    }

    // Toolbar settings
    {
        if (Settings.ToolbarVisible == false) Toolbar->hide();
        else Toolbar->show();
    }
    //Outline settings
    {
        if(Settings.OutlineVisible) {
            if(TextEdit->toPlainText().contains("\\chapter") ||
                    TextEdit->toPlainText().contains("\\section") ||
                    TextEdit->toPlainText().contains("\\subsection") ||
                    TextEdit->toPlainText().contains("\\subsubsection")
                    ){
                outline->show();
            }else{
                outline->hide();
            }
        }
        else outline->hide();
    }

    // Wordwrap settings
    {
        if (Settings.WordWrapEnabled == false) TextEdit->setWordWrapMode(QTextOption::NoWrap);
        else TextEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    }

    //spell-checking
    {
        /*
        if(Settings.SpellCheckEnabled){
            highlighter->enableSpellCheck(true);
            SpellCheck->setChecked(true);
            SpellCheck_GB->setEnabled(true);
            SpellCheck_US->setEnabled(true);
        }else{
            highlighter->enableSpellCheck(false);
            SpellCheck->setChecked(false);
            SpellCheck_GB->setEnabled(false);
            SpellCheck_US->setEnabled(false);
        }*/
        if(Settings.SpellCheckLang == "en_GB"){
            SpellCheck_GB->setChecked(true);
            SpellCheck_US->setChecked(false);
            SpellCheck_None->setChecked(false);
            if(highlighter!=NULL){
                QString dicfile=QDir::homePath()+"/.TeXpen/dicts/en_GB.dic";
                highlighter->setDict(dicfile);
                highlighter->enableSpellCheck(true);
                highlighter->rehighlight();
            }
        }else if(Settings.SpellCheckLang == "en_US"){
            SpellCheck_GB->setChecked(false);
            SpellCheck_US->setChecked(true);
            SpellCheck_None->setChecked(false);
            if(highlighter!=NULL){
                QString dicfile=QDir::homePath()+"/.TeXpen/dicts/en_US.dic";
                highlighter->setDict(dicfile);
                highlighter->enableSpellCheck(true);
                highlighter->useTheme(Settings.ColorScheme);
                highlighter->rehighlight();
            }
        }else{
            SpellCheck_None->setChecked(true);
            SpellCheck_GB->setChecked(false);
            SpellCheck_US->setChecked(false);
            if(highlighter!=NULL){
                highlighter->enableSpellCheck(false);
                highlighter->useTheme(Settings.ColorScheme);
                highlighter->rehighlight();
            }
        }
    }
    
    // Change the font of TextEdit
    TextEdit->setFont(Settings.Font);
    int ww = TextEdit->font().pixelSize();
    if(ww<0){
        ww =TextEdit->font().pointSize();
    }
    TextEdit->setTabStopWidth(ww*4);

    // UI Mode settings
    {
        if ((Settings.StatusBarVisible == false) & (Settings.ToolbarVisible == false))
        {
            MinimalModeAction->setChecked(true);
        }
        else if ((Settings.StatusBarVisible != false) & (Settings.ToolbarVisible != false))
        {
            NormalModeAction->setChecked(true);
        }
        else if (((Settings.StatusBarVisible != false) & (Settings.ToolbarVisible == false)) |
                 ((Settings.StatusBarVisible == false) & (Settings.ToolbarVisible != false)))
        {
            CustomModeAction->setChecked(true);
        }
    }

    // Set the read-only property of TextEdit
    if(Settings.ReadOnly){
        TextEdit->setReadOnly(Settings.ReadOnly);
    }else{
        TextEdit->setReadOnly(false);
        TextEdit->setAttribute(Qt::WA_InputMethodEnabled,true);
    }

    // Read-only settings
    {
        if (Settings.ReadOnly == false) {
            // Enable the editing actions
            InsertTable->setEnabled(true);
            InsertGraphic->setEnabled(true);
            DateTimeAction->setEnabled(true);
            UndoAction->setEnabled(true);
            RedoAction->setEnabled(true);
            CutAction->setEnabled(true);
            PasteAction->setEnabled(true);
            FindAction->setEnabled(true);

            // Change the window title
            //setWindowTitle(TextEdit->documentTitle() + " - TeXpen");
            UpdateWindowTitle();
        }

        else {
            // Disable the editing actions
            InsertTable->setEnabled(false);
            InsertGraphic->setEnabled(false);
            DateTimeAction->setEnabled(false);
            UndoAction->setEnabled(false);
            RedoAction->setEnabled(false);
            CutAction->setEnabled(false);
            PasteAction->setEnabled(false);
            FindAction->setEnabled(false);

            // Change the window title
            //setWindowTitle(TextEdit->documentTitle() + tr(" (Read-Only) ") + "- TeXpen");
            UpdateWindowTitle();
        }
    }

    // Coordinate the checkState of the actions with the settings
    {
        FullscreenAction->setChecked(Settings.FullScreen);
        StatusbarAction->setChecked(Settings.StatusBarVisible);
        SetReadOnlyAction->setChecked(Settings.ReadOnly);
        ToolbarAction->setChecked(Settings.ToolbarVisible);
        OutlineAction->setChecked(Settings.OutlineVisible);
        WordWrapAction->setChecked(Settings.WordWrapEnabled);
    }

    //build
    {
        //runCommand->setStatusTip(tr("Run: %1 [%2]").arg(Settings.theCommand).arg(Settings.cmdDir));
        run->reset();
        QString pr = Settings.theCommand;
        QString dr = Settings.cmdDir;
        if(!FileName.isEmpty()){
            QFileInfo ff(FileName);
            pr.replace("$basedoc",ff.baseName());
            pr.replace("$filename",ff.fileName());
            pr.replace("$doc",ff.absoluteFilePath());
            pr.replace("$dir",ff.absolutePath());

            dr.replace("$basedoc",ff.baseName());
            dr.replace("$filename",ff.fileName());
            dr.replace("$doc",ff.absoluteFilePath());
            dr.replace("$dir",ff.absolutePath());
        }

        //run->setProg(pr);
        run->getCommandFrom(pr);
        run->setDir(dr);
        runCommand->setStatusTip(tr("Run: %1 [%2]").arg(pr).arg(dr));
    }
    //

    //

    //fill recent file list
    {
        //recent file list
        RecentFiles->clear();
        int L = Settings.recentFiles.count();
        for(int i=0;i<L;i++){
            QString fpath = Settings.recentFiles.at(i);
            if(!fpath.isEmpty()){
                QAction* fa = new QAction(this);
                fa->setText(fpath);//may be shorter?
                fa->setData(fpath);
                connect(fa,SIGNAL(triggered()),this, SLOT(openRecent()));
                RecentFiles->addAction(fa); //qDebug()<<"   X "<<i<<endl;
            }
        }
    }

    WriteSettings();
}

void MainWindow::ReadSettings() {
    QSettings Configuration("TeX Pen", "Configuration");

    // Load the font settings
    Configuration.beginGroup("Fonts");
    Settings.Font.setFamily(Configuration.value("Font.Family", "Courier").toString());
    Settings.Font.setPointSize(Configuration.value("Font.Size", 10).toInt());
    Settings.Font.setBold(Configuration.value("Font.Bold", false).toBool());
    Settings.Font.setItalic(Configuration.value("Font.Italic", false).toBool());
    Settings.Font.setUnderline(Configuration.value("Font.Underline", false).toBool());
    Settings.Font.setStrikeOut(Configuration.value("Font.Strikeout", false).toBool());
    Configuration.endGroup();

    // Load other settings
    Configuration.beginGroup("Other Settings");
    Settings.FullScreen         = Configuration.value("FullscreenEnabled", false).toBool();
    Settings.StatusBarVisible   = Configuration.value("StatusbarVisible", true).toBool();
    Settings.ToolbarVisible     = Configuration.value("ToolbarVisible", true).toBool();
    Settings.OutlineVisible     = Configuration.value("OutlineVisible", true).toBool();
    Settings.WordWrapEnabled    = Configuration.value("WordWrapEnabled", true).toBool();
    Settings.SpellCheckEnabled  = Configuration.value("SpellCheckEnabled", true).toBool();
    Settings.SpellCheckLang     = Configuration.value("SpellCheckLang", QString("None")).toString();
    Settings.theCommand         = Configuration.value("theCommand", QString("pdflatex $filename|bibtex $basedoc|pdflatex $filename|pdflatex $filename")).toString();
    Settings.cmdDir             = Configuration.value("cmdDir", QString("$dir")).toString();

    // Set the color sheme of the TextEdit
    CreateStyle(Configuration.value("SelectedColorScheme", 7).toInt());

    // Change the window size
    setGeometry(Configuration.value("Geometry", QRect(100, 100, 800, 400)).toRect());

    //read recent file list
    for(int i=0;i<10;i++){
        QString token = QString("RecentFile%1").arg(i);
        QString fpath = Configuration.value(token, QString("")).toString();
        if(!fpath.isEmpty()){
            Settings.recentFiles.append(fpath);
        }
    }

    // Apply the settings
    ApplySettings();
    Configuration.endGroup();
}

void MainWindow::ToggleFullscreen() {
    Settings.FullScreen = ! Settings.FullScreen;
    ApplySettings();
}

void MainWindow::ToggleMinimalMode() {
    Settings.ToolbarVisible = false;
    Settings.StatusBarVisible = false;
    ApplySettings();
}

void MainWindow::ToggleNormalMode() {
    Settings.ToolbarVisible = true;
    Settings.StatusBarVisible = true;
    ApplySettings();
}

void MainWindow::ToggleReadOnly() {
    Settings.ReadOnly = !Settings.ReadOnly;
    ApplySettings();
}

void MainWindow::ToggleStatusBar() {
    Settings.StatusBarVisible = !Settings.StatusBarVisible;
    ApplySettings();
}

void MainWindow::ToggleToolbar() {
    Settings.ToolbarVisible = !Settings.ToolbarVisible;
    ApplySettings();
}

void MainWindow::ToggleOutline(){
    Settings.OutlineVisible = !Settings.OutlineVisible;
    //onTextChange(1,1,1);//to refresh the tree
    ApplySettings();
    QString cnt = TextEdit->document()->toPlainText();
    reloadStructure(cnt);
    //QtConcurrent::run(this, &MainWindow::reloadStructure, cnt);
}


void MainWindow::ToggleSpellcheck(){
    Settings.SpellCheckEnabled = !Settings.SpellCheckEnabled;
    ApplySettings();
}

void MainWindow::ToggleWordWrap() {
    Settings.WordWrapEnabled = !Settings.WordWrapEnabled;
    ApplySettings();
}

void MainWindow::onConfig(){
    //
    CmdConfigDialog abc;
    abc.setParas(Settings.theCommand,Settings.cmdDir);
    if(QDialog::Accepted==abc.exec()){
        Settings.theCommand=abc.Cmd;
        Settings.cmdDir    =abc.Dir;
    }
    ApplySettings();
}



void MainWindow::WriteSettings() {
    QSettings Configuration("TeX Pen", "Configuration");

    // Font settings
    Configuration.beginGroup("Fonts");
    Configuration.setValue("Font.Family", Settings.Font.family());
    Configuration.setValue("Font.Size", Settings.Font.pointSize());
    Configuration.setValue("Font.Bold", Settings.Font.bold());
    Configuration.setValue("Font.Italic", Settings.Font.italic());
    Configuration.setValue("Font.Underline", Settings.Font.underline());
    Configuration.setValue("Font.Strikeout", Settings.Font.strikeOut());
    Configuration.endGroup();

    // Other settings
    Configuration.beginGroup("Other Settings");
    Configuration.setValue("FullscreenEnabled", Settings.FullScreen);
    Configuration.setValue("SelectedColorScheme", Settings.ColorScheme);
    Configuration.setValue("StatusbarVisible", Settings.StatusBarVisible);
    Configuration.setValue("ToolbarVisible", Settings.ToolbarVisible);
    Configuration.setValue("OutlineVisible", Settings.OutlineVisible);
    Configuration.setValue("WordWrapEnabled", Settings.WordWrapEnabled);
    Configuration.setValue("SpellCheckEnabled", Settings.SpellCheckEnabled);
    Configuration.setValue("theCommand", Settings.theCommand);
    Configuration.setValue("SpellCheckLang", Settings.SpellCheckLang);
    Configuration.setValue("cmdDir", Settings.cmdDir);
    Configuration.setValue("Geometry", geometry());

    //save recent file list
    int L = Settings.recentFiles.count();
    for(int i=0;i<L;i++){
        QString token = QString("RecentFile%1").arg(i);
        QString fpath = Settings.recentFiles.at(i);
        if(!fpath.isEmpty()){
            Configuration.setValue(token, fpath);
        }
    }

        for(int i=L;i<10;i++){
            QString token = QString("RecentFile%1").arg(i);
            QString fpath = "";
            Configuration.setValue(token, fpath);
        }



    Configuration.endGroup();
}
