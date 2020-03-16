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

#include "QFontDialog"
#include "headers/mainwindow.h"
#include "QApplication"
#include <QDateTime>

MainWindow::MainWindow()
{
    Initialize();
}

void MainWindow::AboutTextpad()
{
    QString Message = "<h2>" + qApp->applicationName() + " " + qApp->applicationVersion() + "</h2>"
                      + "<a href=\"https://github.com/Amuwa/TeXpen\">https://github.com/Amuwa/TeXpen</a><br><br>"
                      + "Copyright (C) 2013-2020 <a href=\"https://github.com/Amuwa\">Wang Mengchang</a><br><br>Based On <a href='http://sf.net/p/textpad-editor'><b>Textpad Editor</b></a> <br> Copyright (C) 2013 Alex S.<br>"
                      + tr("<br>The icons used in this application come from the "
                           "<a href = 'http://code.google.com/p/faenza-icon-theme/'>"
                           "Fenza Icon Theme</a>.<br><br>"
                           "Spell-checking is provided via <a href=\"http://sf.net/p/hunspell/\">Hunspell</a> and <br>"
                           " <a href=\"http://extensions.services.openoffice.org/dictionary\">Dictionaries</a> for OpenOffice<br><br>"
                           "And grammar check is provided by <a href=\'http://www.gingersoftware.com\'>www.gingersoftware.com</a><br><br>"
                           "Pdf preview is provide via PDF.js<br>"
                           "<br>The program is provided AS IS with NO WARRANTY OF ANY KIND,<br>"
                           "INCLUDING THE WARRANTY OF DESIGN, "
                           "MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.<br><br>"
                           "Read a copy of the <a href = 'http://www.gnu.org/licenses/gpl-3.0.txt'>"
                           "GPL 3.0 License</a> for more information.<br>"
                           "<hr>"
                           "Since Feb 20, 2019, TeXpen is developed as a response to Alibaba's <b>Three Hours for a Better World</b> initiative, which encourages people to do charity by contributing three hours of service to her/his community every year."
                           "<hr>"
                           "<a href='http://huati.weibo.com/k/TeXpen'>TeXpen on Weibo</a><br>"
                           "<a href='https://twitter.com/search?q=TeXpen%20latex'>TeXpen on Twitter</a><br>"
                          );
    QMessageBox::about(this, " ", Message);
}

void MainWindow::closeEvent(QCloseEvent *Event)
{
    WriteSettings();

    switch (MaybeSave())
    {
    case QMessageBox::Save:
    {
        if (Save() == true) Event->accept();
        else Event->ignore();

        break;
    }

    case QMessageBox::Discard:
        Event->accept();
        break;

    default:
        Event->ignore();
        break;
    }
}

void MainWindow::InsertDateTime()
{
    // Save the document title (to fix a bug where the document title is erased)
    //QString DocumentTitle = TextEdit->documentTitle();
    TextEdit->insertPlainText(QDateTime::currentDateTime().toString()+ "\n");
    //TextEdit->setDocumentTitle(DocumentTitle);
    UpdateWindowTitle();
}

void MainWindow::insertTable()
{
    QString tb="\n\\begin{table}[h]\n"
               "\\caption{Table Caption}\n"
               "\\label{A Label}\n"
               "\\begin{center}\n"
               "\\begin{tabular}{lccccc}\n"
               "\\hline\n"
               "  &A & B & C & D & E \\\\ \n"
               "\\hline\n"
               "1  & & &  & & \\\\\n"
               "2  & & &  & & \\\\\n"
               "3  & & &  & & \\\\\n"
               "\\hline\n"
               "\\end{tabular}\n"
               "\\end{center}\n"
               "\\end{table}\n";
    TextEdit->insertPlainText(tb);
    UpdateWindowTitle();
}
void MainWindow::insertGraphic()
{
    QString g="\n\\begin{figure}[hbt]\n"
              "\\begin{center}\n"
              "\\includegraphics[scale=1.0]{The-Picture-Path}\n"
              "\\end{center}\n"
              "\\caption{The-Figure-Titile}\n"
              "\\label{A-Figure-Label}\n"
              "\\end{figure}\n";
    TextEdit->insertPlainText(g);
    UpdateWindowTitle();
}



void MainWindow::SelectFont()
{
    QFontDialog FontDialog;
    FontDialog.setCurrentFont(Settings.Font);

    if (FontDialog.exec() == QFontDialog::Accepted)
    {
        Settings.Font = FontDialog.selectedFont();
        ApplySettings();
    }
}


extern bool initialized;
void MainWindow::UpdateWindowTitle()
{
    TextEdit->document()->setModified(false);

    if (FileName != "")
    {
        TextEdit->setStatusTip(FileName);
    }

    if (FileName == "") TextEdit->setStatusTip("Unsaved Document");

    if(isStar)
    {
        if (! Settings.ReadOnly )
        {
            setWindowTitle(QString("* ") + TextEdit->documentTitle() + " - TeXpen");
        }
        else
        {
            setWindowTitle(QString("* ") +TextEdit->documentTitle() + tr(" (Read-Only) ")
                           + "- TeXpen");
        }
    }
    else
    {
        if (! Settings.ReadOnly )
        {
            setWindowTitle(TextEdit->documentTitle() + " - TeXpen");
        }
        else
        {
            setWindowTitle(TextEdit->documentTitle() + tr(" (Read-Only) ")
                           + "- TeXpen");
        }
    }
}

void MainWindow::ViewHelp()
{
    // Launch the help window
    QString url = wiki->url().toString().toLower();

    if( (!wikiview->isHidden()) && url.contains("texpen.quora") )   //&& url.contains("help")
    {
        wiki->setHtml("");
        wikiview->hide();
        return;
    }

    wikiview->setWindowTitle("Help");
    wiki->load(QUrl("http://texpen.quora.com/TeXpen-Help"));
    wikiview->setMaximumWidth(600);
    wikiview->show();
}

int changeCount=0;
extern bool isProcessing;
void MainWindow::onTextChange(int position, int charsRemoved, int charsAdded)
{
    //QString cnt = TextEdit->document()->toPlainText(); qDebug()<<"onTextChange:\n"<<cnt<<"\n-------"<<endl;
    showCompletion();

    //if(isProcessing){
    //    return;
    //}

    if(charsAdded<1 && charsRemoved<1 && position>=0)
    {
        return;
    }
    else if(charsAdded==1 && charsRemoved==1 && position==1)
    {
        structure->clear();
    }
    else if(charsAdded>=1)
    {
        isStar=true;

        changeCount++;

        if(realtimepolish)
        {
            int pos = TextEdit->textCursor().positionInBlock();
            QString sec = TextEdit->textCursor().block().text().mid(pos-1,1);
            if(sec.contains(".")||sec.contains("?")||sec.contains("!"))
            {
                realTimeGinger();
            }
        }
    }
    else
    {
        if(charsAdded>=1 || charsRemoved>=1)
        {
            changeCount++;
            isStar=true;
        }
    }

    UpdateWindowTitle();

    if(changeCount>10){
        if((!wikiview->isHidden())&& wikiview->windowTitle().toLower().contains("real")){
            realtimePreview();
        }
        changeCount=0;
    }


    //QTextCursor cr =TextEdit->textCursor();
    //int pos = cr.position();if(pos<1){return;}
    //QChar ch = TextEdit->document()->toPlainText().at(pos-1);
    /*
    if(cr.atBlockStart() ||
            cr.block().text().contains("\\chapter") ||
            cr.block().text().contains("\\section") ||
            cr.block().text().contains("\\subsection") ||
            cr.block().text().contains("\\subsubsection")
      ){

        QtConcurrent::run(this, &MainWindow::reloadStructure,TextEdit->toPlainText());
      }
    */
}


void MainWindow::onModificationChanged(bool mod){
    if(!mod){return;}

    showCompletion();
    isStar=true;
    UpdateWindowTitle();
}


#include <QFile>
#include <QDir>
void MainWindow::onSpellLang()
{
    if (QAction *action = qobject_cast<QAction*>(sender()))
    {
        QString lang=action->data().toString();
        Settings.SpellCheckLang = lang;

        if(lang!="None")
        {
            Settings.SpellCheckEnabled = true;
            QString dicfile=QDir::homePath()+"/.TeXpen/dicts/"+lang+".dic";

            //qDebug()<<"Dict:"<<dicfile<<endl;
            if(highlighter!=NULL)
            {
                TextEdit->setHighlighter(highlighter);
                highlighter->setDict(dicfile);
            }
        }
        else
        {
            Settings.SpellCheckEnabled = false;
        }

        ApplySettings();
    }
}

#include "3rd-party/ginger/ginger.h"

void MainWindow::onGingerPolish()
{
    //
    /*
    qDebug()<<"Polishing...."<<endl;
    Ginger *gg=new Ginger();
    gg->test(" ");
    */
    //if(realtimepolish){
    realtimepolish=!realtimepolish;
    //}
}
