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

#include "QPrinter"
#include "QDateTime"
#include "QFileDialog"
#include "QTextStream"
#include "QPrintDialog"
#include <QLabel>
#include <QTime>

#include "headers/mainwindow.h"


bool MainWindow::Save() {
    if      (FileName == "") return SaveAs();
    else if (FileName != "") return SaveFile(FileName);

    return false;
}

bool MainWindow::SaveAs() {
    QFileDialog Dialog;
    Dialog.setDefaultSuffix("tex");

    return SaveFile(Dialog.getSaveFileName(this, tr("Save As.."), QDir::homePath()));
}

bool MainWindow::SaveFile(const QString FileLocation) {
    if (FileLocation != "") {
/*
        QString cnt = TextEdit->document()->toPlainText();
        theFileSaver->setPara(FileLocation,cnt);
        theFileSaver->start();

        TextEdit->setDocumentTitle(theFileSaver->getFileInfo().fileName());//(QFileInfo(File).fileName());
        FileName = FileLocation;
        isStar=false;
        UpdateWindowTitle();
        addToRecentList(FileLocation);
        reloadStructure(cnt);
        isStar=false;
        emit (nowHighlight());
        isStar=false;
        return true;
    }
*/
        QFile File(FileLocation);
        if (File.open(QFile::WriteOnly | QFile::Text)) {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            QApplication::processEvents();
            //QApplication::restoreOverrideCursor();

            //QTextStream Save(&File);
            //Save.setCodec("UTF-8");//setAutoDetectUnicode(true);//setCodec("UTF-8");
            QString cnt = TextEdit->document()->toPlainText(); //qDebug()<<"SaveFile:\n"<<cnt<<"\n-------"<<endl;
            //Save << cnt;

            File.write(cnt.toUtf8());

            File.close();

            TextEdit->frequentWords.clear();
            TextEdit->setDocumentTitle(QFileInfo(File).fileName());
            FileName = FileLocation;
            isStar=false;
            UpdateWindowTitle();
            addToRecentList(FileLocation);

            reloadStructure(cnt);

            emit (nowHighlight());
            isStar=false;
            UpdateWindowTitle();

            //QApplication::setOverrideCursor(Qt::WaitCursor);
            QApplication::restoreOverrideCursor();

            return true;
        }
        else {
            QMessageBox::warning(this, tr("Warning"),
                                 tr("There was an error when trying to save the file, "
                                    "please save the file under a different name."));
        }
    }

    isStar=true;
    return false;
}

void MainWindow::addToRecentList(QString fpath){
    //firstly remove it , if it exsits in the list
    removeFromRecent(fpath);
    //then add it to the fisrt place
    Settings.recentFiles.insert(0,fpath);
    while(Settings.recentFiles.count()>10){
        Settings.recentFiles.pop_back();
    }
    ApplySettings();
    //qDebug()<<Settings.recentFiles<<endl;
}

void MainWindow::removeFromRecent(QString fpath){
    int L = Settings.recentFiles.count();
    for(int i=0;i<L;i++){
        if(Settings.recentFiles.at(i) == fpath){
            Settings.recentFiles.removeAt(i);
            i=0;
            L = Settings.recentFiles.count();
        }
    }
    ApplySettings();
}

void MainWindow::openRecent(){
    qDebug()<<"-> openRecent ()"<<endl;
    if (QAction *action = qobject_cast<QAction*>(sender())) {
            QString fpath=action->data().toString();
            if(!fpath.isEmpty()){
                if(fpath!=FileName){
                    qDebug()<<"Try to openRecent ("<<fpath<<")..."<<endl;
                    switch (MaybeSave()) {
                    case QMessageBox::Save: {
                        if (Save()) OpenFile(fpath);
                        break;
                    }
                    case QMessageBox::Discard:
                        OpenFile(fpath);
                        break;
                    default:
                        break;
                    }
                }
            }
    }
    ApplySettings();
}


int MainWindow::MaybeSave() {
    //if (TextEdit->document()->isModified() == true) {
    if(isStar){
        QString Message;
        Message = "<h3>" + tr("Save changes to document ''") + TextEdit->documentTitle()
                + tr("'' before closing?") + "</h3>"
                + tr("If you don't save, the changes you made will be permanently lost.");

        return QMessageBox::warning(this, " ",
                                    Message, (QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard));
    }
    return QMessageBox::Discard;
}

void MainWindow::CountWords() {
    QString SizeString;

    // Count the number of words
    int WordCount = TextEdit->toPlainText().split(QRegExp("(\\s|\\n|\\r)+"),
                                                  QString::SkipEmptyParts).count();
    int Line = TextEdit->textCursor().blockNumber();

    // Calculate the size of the document and choose the best units to display the size
    int DocumentSize = TextEdit->toPlainText().length();
    int Byte = DocumentSize;
    int Kilobyte = Byte / 1024;
    int Megabyte = Kilobyte / 1024;
    int Gigabyte = Megabyte / 1024;

    if (DocumentSize <  1024) SizeString = QString::number(Byte) + " Bytes";
    if (DocumentSize >= 1024) SizeString = QString::number(Kilobyte) + " Kilobytes";
    if (DocumentSize >= 1024 * 1024) SizeString = QString::number(Megabyte) + " Megabytes";
    if (DocumentSize >= 1024 * 1024 * 1024) SizeString = QString::number(Gigabyte) + " Gigabytes";

    // Update the DocumentSizeLabel
    DocumentSizeLabel->setText("Line:"+QString::number(Line)+tr(", Words: ") + QString::number(WordCount) + tr(" (about ")
                               + SizeString + ")");
}

void MainWindow::CreateNewDocument() {
    FileName.clear();
    TextEdit->clear();
    TextEdit->setDocumentTitle(tr("Untitled"));
    hideSideView();

    Settings.ReadOnly = false;
    isStar=false;
    ApplySettings();
    UpdateWindowTitle();
    pdfpage="0";
}

extern bool isProcessing;

bool MainWindow::OpenFile(const QString filePath){
    //qDebug()<<"Try to OpenFile("<<filePath<<")..."<<endl;
    hideSideView();
    if(!filePath.isEmpty()){
        QFile file(filePath);
        if(!file.exists()){
            QMessageBox::warning(this, tr("Warning"),
                                 tr("The file '%1' does not exist.\n "
                                    "Please check whether it has been deleted.").arg(filePath)
                                 );
            removeFromRecent(filePath);
            ApplySettings();
            return false;
        }
        if(file.open(QFile::ReadWrite | QFile::Text)){
            QApplication::setOverrideCursor(Qt::WaitCursor);
            QApplication::processEvents();
            //QApplication::restoreOverrideCursor();

            if(highlighter!=NULL){
                delete highlighter;
                highlighter= NULL;
                TextEdit->setHighlighter(NULL);
            }
            //file.map(0,100);
            if(structureThread!=NULL){
                if(structureThread->isRunning()){
                    structureThread->exit(0);
                }
            }

            //QTime bg = QTime::currentTime();
            //QTime readtime;
            //QTime showtime;
            //QTime rendertime;
            //uchar* memory = file.map(0,file.size());


            QString cnt ="";

            //QTextStream Contents(&file);
            //Contents.setCodec("UTF-8");//setAutoDetectUnicode(true);//setCodec("UTF-8");
            //QString cnt = Contents.readAll();


            cnt = QObject::trUtf8(file.readAll().data());


            TextEdit->setPlainText(cnt);
            TextEdit->setDocumentTitle(QFileInfo(file).fileName());
            //showtime = QTime::currentTime();

            file.close();
            isStar=false;
            FileName = filePath;
            Settings.ReadOnly = false;
            addToRecentList(filePath);
            ApplySettings();
            UpdateWindowTitle();

            pdfpage="0";
            reloadStructure(cnt);
            emit (nowHighlight());

            //rendertime=QTime::currentTime();

            isProcessing = false;comSec =""; //onTextChange(1,1,1);
            //QtConcurrent::run(this, &MainWindow::reloadStructure, cnt);//reloadStructure();//refresh the outline

            //QString timestr = "BEGIN: ";
            //timestr += bg.toString("hh:mm:ss.zzz")+"\n"+"READ: "+readtime.toString("hh:mm:ss.zzz")+"\nSHOW: "+showtime.toString("hh:mm:ss.zzz")+"\nRender: "+rendertime.toString("hh:mm:ss.zzz")+"\n";

            //qDebug()<<timestr<<endl;
            //QApplication::setOverrideCursor(Qt::WaitCursor);
            QApplication::restoreOverrideCursor();

            return true;
        }
        else {
            QMessageBox::warning(this, tr("Warning"),
                                 tr("There was an error trying to open this file, "
                                    "you may not have the necessary privileges to edit this file."));
            return false;
        }
    }
    return false;
}

void MainWindow::LoadFile() {
    QString FileLocation = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath());
    OpenFile(FileLocation);
    return;


    if (FileLocation != "") {
        QFile File(FileLocation);
        if (File.open(QFile::ReadWrite | QFile::Text)) {

            QApplication::setOverrideCursor(Qt::WaitCursor);
            QApplication::processEvents();
            //QApplication::restoreOverrideCursor();

            if(highlighter!=NULL){
                delete highlighter;
                highlighter= NULL;
                TextEdit->setHighlighter(NULL);
            }

            /*
            QTextStream Contents(&File);
            Contents.setCodec("UTF-8");//setAutoDetectUnicode(true);//setCodec("UTF-8");

            QString cnt = Contents.readAll();
            */

            /*
            QString cnt = "";
            while(!Contents.atEnd()){
                cnt += Contents.readLine()+"\n";
            }*/

            QString cnt = QObject::trUtf8(File.readAll().data());
            TextEdit->setPlainText(cnt);
            TextEdit->setDocumentTitle(QFileInfo(File).fileName());
            isStar=false;
            FileName = FileLocation;
            Settings.ReadOnly = false;
            addToRecentList(FileName);
            ApplySettings();
            isStar=false;
            UpdateWindowTitle();

            pdfpage="0";
            reloadStructure(cnt);

            isProcessing = false;comSec =""; //onTextChange(1,1,1);
            TextEdit->manuallyHighlight();
            emit (nowHighlight());
            //QtConcurrent::run(this, &MainWindow::reloadStructure, cnt);//reloadStructure();//refresh the outline
            //qDebug()<<"LoadFile end."<<endl;
            //QApplication::setOverrideCursor(Qt::WaitCursor);
            QApplication::restoreOverrideCursor();
        }
        else {
            QMessageBox::warning(this, tr("Warning"),
                                 tr("There was an error trying to open this file, "
                                    "you may not have the necessary privileges to edit this file."));
        }
    }
}

void MainWindow::NewDocument() {
    switch (MaybeSave()) {
    case QMessageBox::Save:
        Save();
        CreateNewDocument();
        break;
    case QMessageBox::Discard:
        CreateNewDocument();
        break;
    }
}

void MainWindow::NewArticleDocument(){
    NewDocument();

    QFile file(":/article.tex");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream Contents(&file);
    Contents.setCodec("UTF-8");
    QString cnt = Contents.readAll();
    file.close();

    TextEdit->insertPlainText(cnt);
    TextEdit->manuallyHighlight();
    UpdateWindowTitle();
}
void MainWindow::NewSlideDocument(){
    NewDocument();

    QFile file(":/slide.tex");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream Contents(&file);
    Contents.setCodec("UTF-8");
    QString cnt = Contents.readAll();
    file.close();

    TextEdit->insertPlainText(cnt);
    TextEdit->manuallyHighlight();
    UpdateWindowTitle();    
}


void MainWindow::OpenFile() {
    switch (MaybeSave()) {
    case QMessageBox::Save:
        if (Save() == true) {
            LoadFile();
        }
        break;
    case QMessageBox::Discard:
        LoadFile();
        break;
    }
}

void MainWindow::PrintDocument() {
    QTextDocument *Document = TextEdit->document();
    QPrinter Printer;
    QPrintDialog *Dialog = new QPrintDialog(&Printer, this);

    if (Dialog->exec() == QDialog::Accepted) Document->print(&Printer);
}

void bindDoc(LaTeXHighlighter* hl, QTextDocument* doc){

    hl->setDocument(doc);

}

bool isTeXdocument(QString filepath){
    if(filepath.toLower().endsWith(".tex")){
        return true;
    }
    return false;
}

void MainWindow::setupHighlighter(){
    if(highlighter!=NULL){
        TextEdit->setHighlighter(NULL);
        delete highlighter;
        highlighter= NULL;
    }
    highlighter= new LaTeXHighlighter;//TextEdit->document());
    highlighter->setTeXDoc(isTeXdocument(FileName));
    if(Settings.SpellCheckLang == "en_GB"){
            QString dicfile=QDir::homePath()+"/.TeXpen/dicts/en_GB.dic";
            highlighter->setDict(dicfile);
            highlighter->enableSpellCheck(true);
            //highlighter->rehighlight();

    }else if(Settings.SpellCheckLang == "en_US"){
            QString dicfile=QDir::homePath()+"/.TeXpen/dicts/en_US.dic";
            highlighter->setDict(dicfile);
            highlighter->enableSpellCheck(true);
            //highlighter->rehighlight();
    }else{
            highlighter->enableSpellCheck(false);
            //highlighter->rehighlight();
    }
    TextEdit->setHighlighter(highlighter);
    QtConcurrent::run(bindDoc,highlighter,TextEdit->document());
}


