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
#include "headers/highlighter.h"

/****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#include <QtGui>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#ifdef QT5SDK
#include <QtConcurrent/QtConcurrent>
#else
#include <QtConcurrentRun>
#endif

bool inEquation=false;

bool inTexComment(const QString &text, const int pos)
{
    QString cnt0 = text.mid(0,pos);
    return isTexComment(cnt0);
}


extern QString commentColor;

LaTeXHighlighter::LaTeXHighlighter(QTextDocument *parent)
    :QSyntaxHighlighter(parent)
{
    theEditor=NULL;
    isParsing = false;
    isEquationOpen= false;
    //connect(this,SIGNAL(pleaseRender(int)), this, SLOT(RenderBlock(int)));
    QStringList keywordPatterns;
    /*
    keywordPatterns<< "\\\\section\\b"<<"\\\\item\\b"<<"\\\\subsection\\b"<<"\\\\subsubsection\\b"
                   << "\\\\chapter\\b"<<"\\\\begin\\b"<<"\\\\end\\b"<<"\\\\usepackage\\b"<<"\\{document\\}"<<"\\\\document\\b"
                   <<"\\\\cite\\b"<<"\\\\citet\\b"<<"\\\\citep\\b"<<"\\\\upcite\\b"<<"\\\\ref\\b"<<"\\\\label\\b"<<"\\\\caption\\b"
                   <<"\\\\hline\\b"<<"\\\\cline\\b"<<"\\\\toprule\\b"<<"\\\\bottomrule\\b"<<"\\\\\\\\"
                   <<"\\{flalign\\}"<<"\\&\\b"<<"\\\\;"
                   <<"\\{subarray\\}"<<"\\{equation\\}"<<"\\{cases\\}"<<"\\{array\\}"
                   <<"\\\\bibliographystyle\\b"<<"\\\\title\\b"<<"\\\\footnote\\b"<<"\\\\includegraphics\\b"
                   <<"\\\\centering\\b"<<"\\\\vspace\\b"<<"\\\\scriptsize\\b"
                   <<"\\\\bibitem"<<"\\{thebibliography\\}"<<"\\{abstract\\}\\b"<<"\\\\\%"
                   <<"\\\\newcommand\\b"<<"\\\\newfontinstance\\b"<<"\\\\newtheoremstyle\\b"<<"\\\\renewcommand\\b"
                   <<"\\\\setmainfont\\b"<<"\\\\definecolor\\b"<<"\\\\pagestyle\\b"<<"\\\\titleformat\\b"<<"\\\\newpage\\b"
                   <<"\\\\setcounter\\b"<<"\\\\input\\b"<<"\\\\clearpage\\b"<<"\\\\phantomsection\\b"<<"\\\\addcontentsline\\b"
                   <<"\\\\bibliography\\b"<<"\\\\documentclass\\b"
                   <<"\\$"
                   ;

    */

    keywordPatterns<< "\\\\section"<<"\\\\item"<<"\\\\subsection"<<"\\\\subsubsection"
                       << "\\\\chapter"<<"\\\\begin"<<"\\\\end"<<"\\\\usepackage"<<"\\{document\\}"<<"\\\\document"
                       <<"\\\\cite"<<"\\\\citet"<<"\\\\citep"<<"\\\\upcite"<<"\\\\ref"<<"\\\\label"<<"\\\\caption"
                       <<"\\\\hline"<<"\\\\cline"<<"\\\\toprule"<<"\\\\bottomrule"<<"\\\\midrule"
                       <<"{flalign}"<<"\\&"<<"\\\\;"<<"{abstract}"
                       <<"{subarray}"<<"{equation}"<<"\\{cases}"<<"\\{array}"
                       <<"\\\\bibliographystyle"<<"\\\\title"<<"\\\\footnote"<<"\\\\includegraphics"
                       <<"\\\\centering"<<"\\\\vspace"<<"\\\\scriptsize"
                       <<"\\\\bibitem"<<"{thebibliography}"<<"\\{abstract}"<<"\\\\\%"
                       <<"\\\\newcommand"<<"\\\\newfontinstance"<<"\\\\newtheoremstyle"<<"\\\\renewcommand"
                       <<"\\\\setmainfont"<<"\\\\definecolor"<<"\\\\pagestyle"<<"\\\\titleformat"<<"\\\\newpage"
                       <<"\\\\setcounter"<<"\\\\input"<<"\\\\clearpage"<<"\\\\phantomsection"<<"\\\\addcontentsline"
                       <<"\\\\bibliography"<<"\\\\documentclass"
                        <<"\\\\author"<<"\\\\thanks"<<"\\\\small"<<"\\\\textit"<<"\\\\textbf"<<"\\\\texttt"<<"\\\\date"<<"\\\\maketitle"
                       ;


    foreach (const QString &pattern, keywordPatterns)
    {
        Keywords.append( QRegExp(pattern) );
    }
    //Keywords.append( QRegExp("\\\\*\\b") );
    QStringList greeksPatterns;
    greeksPatterns<<"\\\\alpha"<<"\\\\beta"<<"\\\\gamma"<<"\\\\delta"<<"\\\\epsilon"<<"\\\\zeta"<<"\\\\eta"
                  <<"\\\\theta"<<"\\\\iota"<<"\\\\kappa"<<"\\\\lambda"<<"\\\\mu"<<"\\\\xi"<<"\\\\nu"<<"\\\\pi"
                  <<"\\\\rho"<<"\\\\sigma"<<"\\\\tau"<<"\\\\upsilon"<<"\\\\phi"<<"\\\\chi"
                  <<"\\\\psi"<<"\\\\omega"<<"\\\\varGamma"<<"\\\\varSigma"<<"\\\\varTheta"
                  <<"\\\\varLambda"<<"\\\\varkappa"<<"\\\\varpi"<<"\\\\varsigma"
                  <<"\\\\varepsilon"<<"\\\\varOmega"<<"\\\\emptyset"<<"\\\\square"<<"\\\\varPhi"
                  ;
    foreach (const QString &pattern, greeksPatterns)
    {
        Greeks.append(QRegExp(pattern));
    }
    QStringList escCharList;
    /*
    escCharList<<"\\\\ldots"<<"\\\\cdots"<<"\\\\ddots"<<"\\\\times"<<"\\\\leq"<<"\\\\geq"<<"\\\\equiv"
               <<"\\\\ll"<<"\\\\gg"<<"\\\\doteq"<<"\\\\prec"<<"\\\\succ"<<"\\\\sim"<<"\\\\preceq"<<"\\\\succeq"<<"\\\\simeq"
               <<"\\\\subset"<<"\\\\supset"<<"\\\\approx"<<"\\\\in"<<"\\\\ni"<<"\\\\owns"
               <<"\\\\propto"<<"\\\\vdash"<<"\\\\dashv"<<"\\\\mid"
               <<"\\\\parallel"<<"\\\\perp"<<"\\\\notin"<<"\\\\neq"<<"\\\\ne"
               <<"\\\\pm"<<"\\\\div"<<"\\\\star"<<"\\\\ast"<<"\\\\cup"<<"\\\\cap"<<"\\\\circ"<<"\\\\vee"<<"\\\\lor"
               <<"\\\\wedge"<<"\\\\land"<<"\\ullet"<<"\\\\diamond"
               <<"\\\\sum"<<"\\igcup"<<"\\igvee"<<"\\igoplus"<<"\\\\prod"<<"\\igcap"<<"\\igwedge"<<"\\\\coprod"
               <<"\\\\int"<<"\\\\oint"
               <<"\\\\leftarrow"<<"\\\\gets"<<"\\\\longleftarrow"<<"\\\\uparrow"<<"\\\\rightarrow"<<"\\\\to"
               <<"\\\\longrightarrow"<<"\\\\downarrow"<<"\\\\leftrightarrow"<<"\\\\longleftrightarrow"<<"\\\\updownarrow"
               <<"\\\\Leftarrow"<<"\\\\Longleftarrow"<<"\\\\Uparrow"<<"\\\\Rightarrow"<<"\\\\Longrightarrow"<<"\\\\Downarrow"
               <<"\\\\Leftrightarrow"<<"\\\\Longleftrightarrow"<<"\\\\Updownarrow"<<"\\\\mapsto"<<"\\\\longmapsto"<<"\\\\nearrow"
               <<"\\\\searrow"<<"\\\\swarrow"<<"\\\\nwarrow"
               <<"\\\\lbrack"<<"\\\\rbrack"<<"\\\\lbrace"<<"\\\\rbrace"<<"\\\\langle"<<"\\\\rangle"<<"\\\\vert"<<"\\\\Vert"
               <<"\\\\lfloor"<<"\\\\rfloor"<<"\\\\lceil"<<"\\\\rceil"<<"\\ackslash"
               <<"\\\\lgroup"<<"\\\\rgroup"<<"\\\\lmoustache"<<"\\\\rmoustache"<<"\\\\arrowvert"<<"\\\\Arrowvert"<<"\\racevert"
               <<"\\\\dots"<<"\\\\vdots"
               <<"\\\\hbar"<<"\\\\imath"<<"\\\\jmath"<<"\\\\ell"<<"\\\\Re"<<"\\\\Im"<<"\\\\aleph"<<"\\\\wp"
               <<"\\\\forall"<<"\\\\exists"<<"\\\\partial"<<"\\\\prime"<<"\\\\emptyset"<<"\\\\infty"
               <<"\\\\nabla"<<"\\\\triangle"<<"\\\\angle"<<"\\\\surd"<<"\\\\diamondsuit"<<"\\\\heartsuit"<<"\\\\clubsuit"<<"\\\\spadesuit"
               <<"\\\\neg"<<"\\\\lnot"<<"\\\\flat"<<"\\\\natural"<<"\\\\sharp"
               <<"\\\\copyright"<<"\\\\S\\b"<<"\\\\P\\b"<<"\\\\pounds"<<"\\\\dag"<<"\\\\ddag"
               <<"\\\\square"<<"\\lacksquare"<<"\\\\circledS\\b"<<"\\\\varnothing"<<"\\\\eth"<<"\\\\mho"<<"\\igstar"
               <<"\\\\bm\\b"<<"\\\\text\\b"<<"\\\\overset\\b"<<"\\\\sin"<<"\\\\cos"<<"\\\\tan"<<"\\\\sqrt"<<"\\\\over\\b"<<"\\\\frac\\b"
               ;
    */

    escCharList<<"\\\\ldots"<<"\\\\cdots"<<"\\\\ddots"<<"\\\\times"<<"\\\\leq"<<"\\\\geq"<<"\\\\equiv"
                   <<"\\\\ll"<<"\\\\gg"<<"\\\\doteq"<<"\\\\prec"<<"\\\\succ"<<"\\\\sim"<<"\\\\preceq"<<"\\\\succeq"<<"\\\\simeq"
                   <<"\\\\subset"<<"\\\\supset"<<"\\\\approx"<<"\\\\in"<<"\\\\ni"<<"\\\\owns"
                   <<"\\\\propto"<<"\\\\vdash"<<"\\\\dashv"<<"\\\\mid"
                   <<"\\\\parallel"<<"\\\\perp"<<"\\\\notin"<<"\\\\neq"<<"\\\\ne"
                   <<"\\\\pm"<<"\\\\div"<<"\\\\star"<<"\\\\ast"<<"\\\\cup"<<"\\\\cap"<<"\\\\circ"<<"\\\\vee"<<"\\\\lor"
                   <<"\\\\wedge"<<"\\\\land"<<"\\ullet"<<"\\\\diamond"
                   <<"\\\\sum"<<"\\igcup"<<"\\igvee"<<"\\igoplus"<<"\\\\prod"<<"\\igcap"<<"\\igwedge"<<"\\\\coprod"
                   <<"\\\\int"<<"\\\\oint"
                   <<"\\\\leftarrow"<<"\\\\gets"<<"\\\\longleftarrow"<<"\\\\uparrow"<<"\\\\rightarrow"<<"\\\\to"
                   <<"\\\\longrightarrow"<<"\\\\downarrow"<<"\\\\leftrightarrow"<<"\\\\longleftrightarrow"<<"\\\\updownarrow"
                   <<"\\\\Leftarrow"<<"\\\\Longleftarrow"<<"\\\\Uparrow"<<"\\\\Rightarrow"<<"\\\\Longrightarrow"<<"\\\\Downarrow"
                   <<"\\\\Leftrightarrow"<<"\\\\Longleftrightarrow"<<"\\\\Updownarrow"<<"\\\\mapsto"<<"\\\\longmapsto"<<"\\\\nearrow"
                   <<"\\\\searrow"<<"\\\\swarrow"<<"\\\\nwarrow"
                   <<"\\\\lbrack"<<"\\\\rbrack"<<"\\\\lbrace"<<"\\\\rbrace"<<"\\\\langle"<<"\\\\rangle"<<"\\\\vert"<<"\\\\Vert"
                   <<"\\\\lfloor"<<"\\\\rfloor"<<"\\\\lceil"<<"\\\\rceil"<<"\\ackslash"
                   <<"\\\\lgroup"<<"\\\\rgroup"<<"\\\\lmoustache"<<"\\\\rmoustache"<<"\\\\arrowvert"<<"\\\\Arrowvert"<<"\\racevert"
                   <<"\\\\dots"<<"\\\\vdots"
                   <<"\\\\hbar"<<"\\\\imath"<<"\\\\jmath"<<"\\\\ell"<<"\\\\Re"<<"\\\\Im"<<"\\\\aleph"<<"\\\\wp"
                   <<"\\\\forall"<<"\\\\exists"<<"\\\\partial"<<"\\\\prime"<<"\\\\emptyset"<<"\\\\infty"
                   <<"\\\\nabla"<<"\\\\triangle"<<"\\\\angle"<<"\\\\surd"<<"\\\\diamondsuit"<<"\\\\heartsuit"<<"\\\\clubsuit"<<"\\\\spadesuit"
                   <<"\\\\neg"<<"\\\\lnot"<<"\\\\flat"<<"\\\\natural"<<"\\\\sharp"
                   <<"\\\\copyright"<<"\\\\S"<<"\\\\P"<<"\\\\pounds"<<"\\\\dag"<<"\\\\ddag"
                   <<"\\\\square"<<"\\lacksquare"<<"\\\\circledS"<<"\\\\varnothing"<<"\\\\eth"<<"\\\\mho"<<"\\igstar"
                   <<"\\\\bm"<<"\\\\text"<<"\\\\overset"<<"\\\\sin"<<"\\\\cos"<<"\\\\tan"<<"\\\\sqrt"<<"\\\\over"<<"\\\\frac"
                   ;

    foreach (const QString& pp, escCharList)
    {
        EscChars.append(QRegExp(pp));
    }
    keywordFormat.setForeground(Qt::blue);
    commentFormat.setForeground(Qt::green);
    commentFormat.setFontItalic(true);
    equationFormat.setForeground(Qt::magenta);
    greeksFormat.setForeground(Qt::cyan);
    greeksFormat.setFontItalic(false);
    escCharFormat.setForeground(Qt::yellow);
    escCharFormat.setFontItalic(false);
    spellErrorFormat.setUnderlineColor(Qt::red);
    spellErrorFormat.setUnderlineStyle(QTextCharFormat::DotLine);
    sChecker=NULL;
    QString basePath = QDir::homePath()+"/.TeXpen/dicts/";
    QDir a(basePath);

    if(!a.exists(basePath))
    {
        a.mkpath(basePath);
    }

    QString dicfile=QDir::homePath()+"/.TeXpen/dicts/en_GB.dic";
    QString afffile=QDir::homePath()+"/.TeXpen/dicts/en_GB.aff";
    QString dicfile2=QDir::homePath()+"/.TeXpen/dicts/en_US.dic";
    QString afffile2=QDir::homePath()+"/.TeXpen/dicts/en_US.aff";

    //QFile b(dicfile);
    if(!QFile::exists(dicfile))
    {
        QFile::copy(":/dicts/en_GB.dic", dicfile);
        QFile::copy(":/dicts/en_GB.aff", afffile);
    }

    if(!QFile::exists(dicfile2))
    {
        QFile::copy(":/dicts/en_US.dic", dicfile2);
        QFile::copy(":/dicts/en_US.aff", afffile2);
    }

    setDict(dicfile2);
    spellCheckActive=true;
    findFormat.setBackground(QColor(Qt::yellow).lighter(160));
    findFormat.setForeground(QColor(Qt::black));
    FindStr = "";
}

void LaTeXHighlighter::useTheme(int themeId){
    switch (themeId) {
    case 1:
        keywordFormat.setForeground(QColor("#88ff77"));
        commentFormat.setForeground(QColor("#ddcc88"));
        equationFormat.setForeground(Qt::magenta);
        greeksFormat.setForeground(Qt::cyan);
        escCharFormat.setForeground(QColor("#dd88fa"));
        break;
    case 2:
        //dusk
        keywordFormat.setForeground(QColor("#ff5566"));
        commentFormat.setForeground(QColor("#888888"));
        equationFormat.setForeground(Qt::magenta);
        greeksFormat.setForeground(QColor("#128523"));
        escCharFormat.setForeground(QColor("#ff22ed"));
        break;
    case 3:
        //hacher
        keywordFormat.setForeground(Qt::red);
        commentFormat.setForeground(QColor("#ddcc88"));
        equationFormat.setForeground(Qt::magenta);
        greeksFormat.setForeground(Qt::cyan);
        escCharFormat.setForeground(Qt::yellow);
        break;
    case 4:
        //light
        keywordFormat.setForeground(Qt::blue);
        commentFormat.setForeground(QColor("#cc9977"));
        equationFormat.setForeground(Qt::magenta);
        greeksFormat.setForeground(QColor("#128523"));
        escCharFormat.setForeground(QColor("#fd55fe"));
        break;
    case 5:
        //navy
        keywordFormat.setForeground(QColor("#ffaa88"));
        commentFormat.setForeground(QColor("#77ca88"));
        equationFormat.setForeground(Qt::magenta);
        greeksFormat.setForeground(Qt::cyan);
        escCharFormat.setForeground(Qt::yellow);
        break;
    case 6:
        //night
        keywordFormat.setForeground(Qt::red);
        commentFormat.setForeground(Qt::green);
        equationFormat.setForeground(Qt::magenta);
        greeksFormat.setForeground(Qt::cyan);
        escCharFormat.setForeground(Qt::yellow);
        break;
    case 7:
        //normal
        keywordFormat.setForeground(Qt::blue);
        commentFormat.setForeground(QColor("#559955"));
        equationFormat.setForeground(Qt::magenta);
        greeksFormat.setForeground(QColor("#885533"));
        escCharFormat.setForeground(QColor("#ff5566"));
        break;
    case 8:
        //paper
        keywordFormat.setForeground(Qt::blue);
        commentFormat.setForeground(QColor("#559955"));
        equationFormat.setForeground(Qt::magenta);
        greeksFormat.setForeground(QColor("#885533"));
        escCharFormat.setForeground(QColor("#ff5566"));
        break;
    default:
        keywordFormat.setForeground(Qt::blue);
        commentFormat.setForeground(Qt::green);
        equationFormat.setForeground(Qt::magenta);
        greeksFormat.setForeground(Qt::cyan);
        escCharFormat.setForeground(Qt::yellow);
        break;
    }
}



void LaTeXHighlighter::ParseDocument()
{
    if(isParsing)
    {
        return;
    }

    isParsing = true;
    QTextDocument* doc = document();
    int bc = doc->blockCount();

    for(int b=0; b<bc; b++)
    {
        QTextBlock blk = doc->findBlockByNumber(b);
        ParseTextBlock(blk);
    }

    //
    isParsing = false;
    emit (pleaseRender(1));
}

void LaTeXHighlighter::setTeXDoc(bool isTex)
{
    isTeXDoc=isTex;
}


void LaTeXHighlighter::ParseTextBlock(QTextBlock blk)
{
//    HighlightDataList *HList = (HighlightDataList *)( blk.userData());
//    if(HList==NULL){
//        return;
//    }
    Q_UNUSED(blk);
}

void LaTeXHighlighter::RenderBlock(int bn)
{
    QTextBlock blk = document()->findBlockByNumber(bn);

    //rehighlightBlock(blk);
    //Q_UNUSED(bn);
    if(blk.isVisible())
    {
        rehighlight();
    }
}

void LaTeXHighlighter::RenderCurrentBlock()
{
    HighlightDataList *HList = (HighlightDataList *)( currentBlockUserData());

    if(HList == NULL)
    {
        return;
    }

    foreach(HighlightData hd, HList->getHlData())
    {
        QTextCharFormat fmt;

        switch(hd.HType)
        {
        case HighlightData::Keyword:
            fmt = keywordFormat;
            break;

        default:
            break;
        }

        setFormat(hd.begin,hd.length,fmt);
    }
}


inline int NextNonLetterPos(const QString& txt, int pos){
    //int rs = -1;
    for(int i=pos+1;i<txt.length();i++){
        if(!txt.at(i).isLetter()){
            return i;
        }
    }
    return -1;
}


void LaTeXHighlighter::realHighlight(const QString& text )
{

    int pos=0;
    int len = text.length();
    QChar ch='\0';

    while(pos<len && text.at(pos)!='\%')
    {
        ch = text.at(pos);

        if(ch==' ')
        {
            pos++;
        }
        else if(ch=='$')
        {
            //to deal with equations
            if(pos+1<len)
            {
                if(text.at((pos+1))=='$')
                {
                    setFormat(pos,2,keywordFormat);
                    pos+=2;
                }
                else
                {
                    setFormat(pos,1,keywordFormat);
                    pos++;
                }
            }
            else
            {
                setFormat(pos,1,keywordFormat);
                pos++;
            }
        }
        else if(ch=='\\')
        {
            if(pos+1<len && text.at(pos+1)=='[')
            {
                //begin equation mode "\["
                setFormat(pos,2,keywordFormat);
                pos+=2;
                continue;
            }
            else if(pos+1<len && text.at(pos+1)==']')
            {
                //begin equation mode "\["
                setFormat(pos,2,keywordFormat);
                pos+=2;
                continue;
            }
            else if(pos+1<len && text.at(pos+1)=='\\')
            {
                //new line "\\"
                setFormat(pos,2,keywordFormat);
                pos+=2;
                continue;
            }
            else if(pos+1<len && text.at(pos+1)=='\%')
            {
                //"\%"
                pos+=2;
                continue;
            }
            else if(pos+1<len && text.at(pos+1)=='$')
            {
                //"\%"
                pos+=2;
                continue;
            }
            else
            {
                bool set=false;
                QString cand = text.mid(pos,30);
                //greak
                foreach (QRegExp reg, Greeks)
                {
                    QString p = reg.pattern();
                    p.replace("\\\\","\\");
                    p.replace("\\\\","\\");

                    //qDebug()<<cand<<"<--->"<<p<<endl;
                    if(cand.startsWith(p))
                    {
                        ////
                        int length = p.length();
                        if(pos+length<len){
                            QChar xc = text.at(pos+length);
                            if(xc.isLetter()){
                                continue;
                            }else{
                                setFormat(pos,p.length(),greeksFormat);
                                pos+= p.length();
                                set = true;
                                break;
                            }
                        }else{
                            setFormat(pos,p.length(),greeksFormat);
                            pos+= p.length();
                            set = true;
                            break;
                        }

                    }
                }

                if(set)
                {
                    continue;
                }

                //keywords
                foreach (QRegExp reg, Keywords)
                {
                    QString p = reg.pattern();
                    p.replace("\\\\","\\");
                    //qDebug()<<cand<<"<--->"<<p<<endl;

                    if(cand.startsWith(p))
                    {
                        int length = p.length();
                        if(pos+length<len){
                            QChar xc = text.at(pos+length);
                            if(xc.isLetter()){
                                continue;
                            }else{
                                setFormat(pos,p.length(),keywordFormat);
                                pos+= p.length();
                                set = true;
                                break;
                            }
                        }else{
                            setFormat(pos,p.length(),keywordFormat);
                            pos+= p.length();
                            set = true;
                            break;
                        }

                    }
                    ///

                }

                if(set)
                {
                    continue;
                }

                //escape chars
                foreach (QRegExp reg, EscChars)
                {
                    QString p = reg.pattern();
                    p.replace("\\\\","\\");
                    //qDebug()<<cand<<"<--->"<<p<<endl;

                    if(cand.startsWith(p))
                    {

                        int length = p.length();
                        if(pos+length<len){
                            QChar xc = text.at(pos+length);
                            if(xc.isLetter()){
                                continue;
                            }else{
                                setFormat(pos,p.length(),escCharFormat);
                                pos+= p.length();
                                set = true;
                                break;
                            }
                        }else{
                            setFormat(pos,p.length(),escCharFormat);
                            pos+= p.length();
                            set = true;
                            break;
                        }
                    }
                }

                if(set)
                {
                    continue;
                }
                else
                {
                    pos++;
                }
            }
        }
        else if(!ch.isLetter()){
            pos++;
        }
        else
        {
            /*
            int idx = text.indexOf(" ",pos);
            int idx2= text.indexOf("\\",pos); if((idx>idx2||idx<0)&& idx2>pos){idx=idx2;}
            int idx3=text.indexOf("$",pos);if((idx>idx3||idx<0)&& idx3>pos){idx=idx3;}
            */

            int idx = NextNonLetterPos(text,pos);

            if(idx>pos)
            {
                QString word = text.mid(pos,idx-pos);
                if(bSpellCheck)
                {
                    if(!checkWord(word)){
                        setFormat(pos, word.length(), spellErrorFormat);
                    }
                }
                pos=idx;
            }
            else
            {
                //end reached
                return;
            }
        }
    }

    if(pos<len)
    {
        //commentFormat.setForeground(QColor(commentColor));//(Qt::green);
        commentFormat.setFontItalic(true);
        setFormat(pos,len-pos, commentFormat);
    }
}

void LaTeXHighlighter::highlightBlock(const QString &text)
{
    if(theEditor!=NULL){
        if(currentBlock().blockNumber()<theEditor->firstVisibleBlockNumber()-10){
            return;
        }else if(currentBlock().blockNumber()>theEditor->lastVisibleBlockNumber()+2 && currentBlock().blockNumber()>theEditor->firstVisibleBlockNumber()+20){
            return;
        }
    }

    realHighlight(text);
    return;

    // if(currentBlock().isVisible()){
    if(bSpellCheck)
    {
        ParseSpellError(text);
    }

    //ParseEquation(text);

    if(isTeXDoc)
    {
        ParseGreeks(text);
        ParseEscapeChar(text);
        ParseComment(text);
        ParseKeyword(text);
    }

    // }
    //QtConcurrent::run(this, & LaTeXHighlighter::ParseKeyword, text);
}



void LaTeXHighlighter::ParseKeyword(QString txt)
{
    foreach (QRegExp reg, Keywords)
    {
        QRegExp expression(reg);
        int index = expression.indexIn(txt);

        while (index >= 0)
        {
            int length = expression.matchedLength();

            if(!inTexComment(txt,index))
            {
                setFormat(index,length,keywordFormat);
            }

            index = expression.indexIn(txt, index + length);
        }
    }
}


void LaTeXHighlighter::parseEquation(const QString& txt, QRegExp begin, QRegExp end)
{
    int start = -1;
    int finish = -1;
    int length = -1;
    setCurrentBlockState(0);
//    if(previousBlockState() == 1){
//        isEquationOpen = true;
//    }else{
//        isEquationOpen = false;
//    }

    if(!isEquationOpen)
    {
        start = begin.indexIn(txt);

        if(start< 0)
        {
            setCurrentBlockState(0);
            return;
        }
        else
        {
            while(start>=0)
            {
                if(inTexComment(txt, start))
                {
                    setCurrentBlockState(0);
                    break;
                }

                isEquationOpen = true;
                setCurrentBlockState(1);
                finish = end.indexIn(txt,start+1);

                if(finish<0 || inTexComment(txt,finish))
                {
                    break;
                }

                length = finish - start;
                setFormat(start,length,equationFormat);
                isEquationOpen = false; //close a equation
                setCurrentBlockState(0);
                start = begin.indexIn(txt, finish+1);
            }
        }
    }
    else
    {
        //isEquationOpen==true
        finish = end.indexIn(txt);

        if(finish<0 || inTexComment(txt,finish))
        {
            start = 0;
            length = txt.length();
            setFormat(start,length,equationFormat);
            isEquationOpen = true;
            setCurrentBlockState(1);
        }
        else
        {
            start = 0;
            length = finish;
            setFormat(start,length,equationFormat);
            isEquationOpen = false;
            setCurrentBlockState(0);
            start = begin.indexIn(txt, finish+1);

            if(start< 0)
            {
                return;
            }
            else
            {
                while(start>=0)
                {
                    if(inTexComment(txt, start))
                    {
                        break;
                    }

                    isEquationOpen = true;
                    setCurrentBlockState(1);
                    finish = end.indexIn(txt,start+1);

                    if(finish<0 || inTexComment(txt,finish))
                    {
                        break;
                    }

                    length = finish - start;
                    setFormat(start,length,equationFormat);
                    isEquationOpen = false; //close a equation
                    setCurrentBlockState(0);
                    start = begin.indexIn(txt, finish+1);
                }
            }
        }
    }

    return;
}


void LaTeXHighlighter::ParseEquation(const QString &txt)
{
    parseEquation(txt,QRegExp("\\\\begin\\{flalign\\}"),QRegExp("\\\\end\\{flalign\\}"));
    parseEquation(txt,QRegExp("\\\\begin\\{align\\}"),QRegExp("\\\\end\\{align\\}"));
    parseEquation(txt,QRegExp("\\\\begin\\{equation\\}"),QRegExp("\\\\end\\{equation\\}"));
    parseEquation(txt,QRegExp("\\\\\\["),QRegExp("\\\\\\]"));
    //parseEquation(txt,QRegExp("\\$\\$"),QRegExp("\\$\\$"));
    //parseEquation(txt,QRegExp("\\$"),QRegExp("\\$"));
}
void LaTeXHighlighter::ParseComment(const QString &txt)
{
    int idx = txt.indexOf("\%");

    while(idx>0 && txt.at(idx-1)=='\\')
    {
        idx = txt.indexOf("\%", idx +1);
    }

    if(idx<0)
    {
        return;
    }

    commentFormat.setForeground(QColor(commentColor));//(Qt::green);
    commentFormat.setFontItalic(true);
    setFormat(idx,txt.length()-idx, commentFormat);
}
void LaTeXHighlighter::ParseGreeks(const QString &text)
{
    foreach (QRegExp reg, Greeks)
    {
        QRegExp expression(reg);
        int index = expression.indexIn(text);

        while (index >= 0)
        {
            int length = expression.matchedLength();

            if(!inTexComment(text,index))
            {
                setFormat(index,length,greeksFormat);
            }

            index = expression.indexIn(text, index + length);
        }
    }
}
void LaTeXHighlighter::ParseEscapeChar(const QString &text)
{
    foreach (QRegExp reg, EscChars)
    {
        QRegExp expression(reg);
        int index = expression.indexIn(text);

        while (index >= 0)
        {
            int length = expression.matchedLength();

            if(!inTexComment(text,index))
            {
                setFormat(index,length,escCharFormat);
            }

            index = expression.indexIn(text, index + length);
        }
    }
}
void LaTeXHighlighter::ParseSpellError(const QString &text)
{
    if (spellCheckActive)
    {
        // split text into words
        QString str = text.simplified();

        if (!str.isEmpty())
        {
            QStringList Checkliste = str.split(QRegExp("([^\\w,^\\\\]|(?=\\\\))+"),
                                               QString::SkipEmptyParts);
            int l,number;

            // check all words
            for (int i=0; i<Checkliste.size(); ++i)
            {
                str = Checkliste.at(i);

                if (str.length()>1 &&!str.startsWith('\\'))
                {
                    if (!checkWord(str))
                    {
                        number = text.count(QRegExp("\\b" + str + "\\b"));
                        l=-1;

                        // underline all incorrect occurences of misspelled word
                        for (int j=0; j < number; ++j)
                        {
                            l = text.indexOf(QRegExp("\\b" + str + "\\b"),l+1);

                            if (l>=0)
                                if(!inTexComment(text,l))
                                {
                                    setFormat(l, str.length(), spellErrorFormat);
                                }
                        } // for j
                    } // if spell chek error
                } // if str.length > 1
            } // for
        } // if str.isEmpty
    } // initial check
}

bool LaTeXHighlighter::setDict(const QString SpellDic)
{
    //for ubuntu 14.04 test
    //return false;
    //-end for

    bool spell;

    if(SpellDic!="")
    {
        //mWords.clear();
        spell_dic=SpellDic.left(SpellDic.length()-4);

        if(sChecker!=NULL)
        {
            delete sChecker;
            sChecker=NULL;
        }

        sChecker = new Hunspell(spell_dic.toLatin1()+".aff",spell_dic.toLatin1()+".dic");
        spell_encoding=QString(sChecker->get_dic_encoding());
        codec = QTextCodec::codecForName(spell_encoding.toLatin1());
        QFileInfo fi(SpellDic);

        if (fi.exists() && fi.isReadable()) spell=true;
        else spell=false;

        // get user config dictionary
        //temperarily commented 07-21
        /*
        QSettings setting;
        QString filePath=QFileInfo(setting.fileName()).absoluteFilePath();
        filePath=filePath+"/User_"+QFileInfo(spell_dic.toLatin1()+".dic").fileName();
        qDebug()<< qPrintable(filePath) << endl;
        fi=QFileInfo(filePath);
        if (fi.exists() && fi.isReadable()){
            sChecker->add_dic(filePath.toLatin1());
        }
        else filePath="";
        */
        //spellErrorFormat.setForeground(Qt::red);//faster Cursoroperation ...
        //spellCheckFormat.setUnderlineColor(QColor(Qt::red));
        spellErrorFormat.setUnderlineStyle(QTextCharFormat::DotLine);//QTextCharFormat::SpellCheckUnderline);
    }
    else spell=false;

    spellerError=!spell;
    spellCheckActive=spellCheckActive && spell;
    rehighlight();
    return spell;
}

bool LaTeXHighlighter::checkWord(QString word)
{
    //for ubuntu 14.04 issue, disable checker
    //return true;
    //-for
    int check;
    QByteArray encodedString;
    encodedString = codec->fromUnicode(word);
    check = sChecker->spell(encodedString.data());
    return bool(check);
}

QStringList LaTeXHighlighter::spellSuggestion(QString word)
{
    QStringList rs;
    rs.clear();

    if(checkWord(word))
    {
        return rs;
    }

    char ** sug;
    QByteArray encodedString;
    encodedString = codec->fromUnicode(word);
    int count = sChecker->suggest(&sug,encodedString.data());

    if(count>0)
    {
        for(int i=0; i<count; i++)
        {
            //char * str = sug[i][0];
            rs << codec->toUnicode(sug[i]);
            free(sug[i]);
        }
    }
    else
    {
        rs.clear();
        return rs;
    }

    if(rs.count()>0)
    {
        qDebug()<<count<<"sugestions:\n"<<rs<<endl;
    }

    return rs;
}


void LaTeXHighlighter::enableSpellCheck(bool b) //set spell checking
{
    bSpellCheck=b;
    rehighlight();
}

void LaTeXHighlighter::setFind(QString sel, bool isCase)  //set find string highlighting
{
    FindStr = sel;

    if(!FindStr.isEmpty())
    {
        isCaseSensetive= isCase;
        rehighlight();
    }

    rehighlight();
}

////////////////////////////

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    connect(this,SIGNAL(fillOk(QString,QVector<HighlightCmd>)),
            this,SLOT(rendering(QString,QVector<HighlightCmd>)));
    bSpellCheck = false;
    HighlightingRule rule;
    keywordFormat.setForeground(Qt::blue);
    //keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns
    /*
    << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
            << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
            << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
            << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
            << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
            << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
            << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
            << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
            << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
            << "\\bvoid\\b" << "\\bvolatile\\b"
               */
            << "\\\\section\\b"<<"\\\\item\\b"<<"\\\\subsection"<<"\\\\subsubsection"
            << "\\\\chapter"<<"\\\\begin"<<"\\\\end"<<"\\\\usepackage"<<"\\\\document"
            <<"\\\\cite"<<"\\\\upcite"<<"\\\\ref"<<"\\\\label"<<"\\\\caption"
            <<"\\\\hline"<<"\\\\cline"<<"\\\\toprule"<<"\\\\bottomrule"<<"\\\\\\\\"
            <<"\\{flalign\\}"<<"\\&"<<"\\\\;"
            <<"\\{subarray\\}"<<"\\{equation\\}"<<"\\{cases\\}"<<"\\{array\\}"
            <<"\\\\bibliographystyle"<<"\\\\title"<<"\\\\footnote"<<"\\\\includegraphics"
            <<"\\\\centering"<<"\\\\vspace"<<"\\\\scriptsize"
            <<"\\\\bibitem"<<"\\{thebibliography\\}"<<"\\{abstract\\}"<<"\\\\\%"
            ;
    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
    //greaks
    equationGreeks.setForeground(Qt::darkMagenta);
    equationGreeks.setFontItalic(false);
    QStringList greekList;
    greekList<<"\\\\alpha"<<"\\\\beta"<<"\\\\gamma"<<"\\\\delta"<<"\\\\epsilon"<<"\\\\zeta"<<"\\\\eta"
             <<"\\\\theta"<<"\\\\iota"<<"\\\\kappa"<<"\\\\lambda"<<"\\\\mu"<<"\\\\xi"<<"\\\\nu"<<"\\\\pi"
             <<"\\\\rho"<<"\\\\sigma"<<"\\\\tau"<<"\\\\upsilon"<<"\\\\phi"<<"\\\\chi"
             <<"\\\\psi"<<"\\\\omega"<<"\\\\varGamma"<<"\\\\varSigma"<<"\\\\varTheta"
             <<"\\\\varLambda"<<"\\\\varkappa"<<"\\\\varpi"<<"\\\\varsigma"
             <<"\\\\varepsilon"<<"\\\\varOmega"<<"\\\\emptyset"<<"\\\\square"
             ;
    foreach (const QString &pattern, greekList)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = equationGreeks;
        highlightingRules.append(rule);
    }
    //keywords in equations
    equationKeyword.setForeground(Qt::darkGreen);
    QStringList eqkeyList;
    eqkeyList<<"\\\\sum"<<"\\\\in\\b"<<"\\\\notin"<<"\\\\leq\\b"<<"\\\\geq\\b"<<"\\\\equiv\\b"<<"\\\\sin"<<"\\\\cos"
             <<"\\\\frac"<<"\\\\bm"<<"\\\\ldots\\b"<<"\\\\sim"<<"\\\\text"<<"\\\\prod"<<"\\\\cdot"
             <<"\\\\infty"<<"\\\\limits"<<"\\\\nonumber"<<"\\\\forall"<<"\\\\cup"<<"\\\\wedge"
             <<"\\\\left"<<"\\\\right"<<"\\\\lfloor"<<"\\\\rfloor"<<"\\\\lceil"<<"\\\\rceil"
             <<"\\\\mathcal"<<"\\\\max"<<"\\\\min"<<"\\\\int"<<"\\\\times"<<"\\\\cap\\b"
             <<"\\\\mathop"<<"\\\\sup"<<"\\\\leftarrow"<<"\\\\rightarrow"<<"\\\\exp\\b"
             <<"\\\\Rightarrow"<<"\\\\Leftarrow"<<"\\\\bar"<<"\\\\hat"<<"\\\\tilde"
             ;
    foreach (const QString &pattern, eqkeyList)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = equationKeyword;
        highlightingRules.append(rule);
    }
    //////
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);
    //actually equations in LaTeX
    multiLineCommentFormat.setForeground(Qt::darkYellow);//magenta);
    multiLineCommentFormat.setFontItalic(false);
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
    //quotationFormat.setForeground(Qt::darkGreen);
    //rule.pattern = QRegExp("\\$.*\\$");
    //rule.format = quotationFormat;
    //highlightingRules.append(rule);
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    //highlightingRules.append(rule);
    commentStartExpression = QRegExp("\\$");//("/\\*");
    commentEndExpression = QRegExp("\\$");
    singleLineCommentFormat.setForeground(Qt::green);
    singleLineCommentFormat.setFontItalic(true);
    /*  not enough
    //rule.pattern = QRegExp("//[^\n]*");
    rule.pattern = QRegExp("\%[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
    */
    spellErrorFormat.setUnderlineColor(Qt::red);
    spellErrorFormat.setUnderlineStyle(QTextCharFormat::DotLine);
    sChecker=NULL;
    QString basePath = QDir::homePath()+"/.TeXpen/dicts/";
    QDir a(basePath);

    if(!a.exists(basePath))
    {
        a.mkpath(basePath);
    }

    QString dicfile=QDir::homePath()+"/.TeXpen/dicts/en_GB.dic";
    QString afffile=QDir::homePath()+"/.TeXpen/dicts/en_GB.aff";
    QString dicfile2=QDir::homePath()+"/.TeXpen/dicts/en_US.dic";
    QString afffile2=QDir::homePath()+"/.TeXpen/dicts/en_US.aff";

    //QFile b(dicfile);
    if(!QFile::exists(dicfile))
    {
        QFile::copy(":/dicts/en_GB.dic", dicfile);
        QFile::copy(":/dicts/en_GB.aff", afffile);
    }

    if(!QFile::exists(dicfile2))
    {
        QFile::copy(":/dicts/en_US.dic", dicfile2);
        QFile::copy(":/dicts/en_US.aff", afffile2);
    }

    setDict(dicfile2);
    spellCheckActive=true;
    findFormat.setBackground(QColor(Qt::yellow).lighter(160));
    findFormat.setForeground(QColor(Qt::black));
    FindStr = "";
}


void Highlighter::enableSpellCheck(bool b)
{
    bSpellCheck=b;
    rehighlight();
}

void Highlighter::setFind(QString sel, bool isCase)
{
    FindStr = sel;

    if(!FindStr.isEmpty())
    {
        isCaseSensetive= isCase;
        rehighlight();
    }

    //QtConcurrent::run(this, &Highlighter::rehighlight);
    rehighlight();
}



void Highlighter::highlightComment(const QString &text)
{
    int idx = text.indexOf("\%");

    if(idx<0)
    {
        return;
    }

    int pos = -1;

    if(idx==0)
    {
        //the whole block
        pos = 0;
    }
    else if(text.at(idx-1) != '\\')
    {
        //from idx to '\n'
        pos = idx;
    }
    else
    {
        while(idx>0 && text.at(idx-1) == '\\')
        {
            idx = text.indexOf("%", idx+1);
        }

        pos = idx;
    }

    if(idx>=0)
    {
        int end = text.length();//indexOf("\n",pos);
        setFormat(pos, end-pos, singleLineCommentFormat);
    }
}




void Highlighter::highlightTexEquations(const QString &text, QString beginToken, QString endToken)
{
    int startIdx = text.indexOf(beginToken);
    int endIdx   = text.indexOf(endToken);
    int length = -1;

    if(startIdx <0 && (!inEquation) )
    {
        return;
    }
    else
    {
        if(startIdx>=0 && (! inTexComment(text,startIdx)))
        {
            inEquation = true;
            //qDebug()<<"Entering equation..."<<endl;
        }

        if(endIdx>=0)
        {
            if (! inTexComment(text,endIdx))
            {
                length = endIdx - startIdx + 15; //\end{equation} length 15
                inEquation = false;
                //qDebug()<<"end of an equation."<<endl;
            }
            else
            {
                //qDebug()<<"(a commented \\end{equation}) still in equation..."<<endl;
                length = text.length();
            }
        }
        else
        {
            //qDebug()<<"still in equation..."<<endl;
            length = text.length();
        }

        if(startIdx<0)
        {
            startIdx = 0;
        }

        if(length<0)
        {
            length = 0;
        }

        setFormat(startIdx, length, multiLineCommentFormat);
    }
}


void Highlighter::fillCmdList(const QString &text)
{
    QVector<HighlightCmd> CList;
    QVector<HighlightCmd> eq =  getTexEquations(text);

    for(int i=0; i<eq.size(); i++)
    {
        CList.append(eq.at(i));
    }

    QVector<HighlightCmd> sp = getSpellErrors(text) ;

    for(int i=0; i<sp.size(); i++)
    {
        CList.append(sp.at(i));
    }

    emit(  fillOk(text,CList) );
}

QVector<HighlightCmd> Highlighter::getTexEquations(const QString &text)
{
    QVector<HighlightCmd> CList;
    int startIdx = text.indexOf("\\begin{equation}");
    int endIdx   = text.indexOf("\\end{equation}");
    int length = -1;

    if(startIdx <0 && (!inEquation) )
    {
        //return;
    }
    else
    {
        if(startIdx>=0 && (! inTexComment(text,startIdx)))
        {
            inEquation = true;
            //qDebug()<<"Entering equation..."<<endl;
        }

        if(endIdx>=0)
        {
            if (! inTexComment(text,endIdx))
            {
                length = endIdx - startIdx + 15; //\end{equation} length 15
                inEquation = false;
                //qDebug()<<"end of an equation."<<endl;
            }
            else
            {
                //qDebug()<<"(a commented \\end{equation}) still in equation..."<<endl;
                length = text.length();
            }
        }
        else
        {
            //qDebug()<<"still in equation..."<<endl;
            length = text.length();
        }

        if(startIdx<0)
        {
            startIdx = 0;
        }

        if(length<0)
        {
            length = 0;
        }

        setFormat(startIdx, length, multiLineCommentFormat);
        HighlightCmd ac;
        ac.begin=startIdx;
        ac.length = length;
        ac.format = multiLineCommentFormat;
        CList.push_back(ac);
    }

    //$$
    startIdx = text.indexOf("$$");
    endIdx   = text.indexOf("$$");
    length = -1;

    if(startIdx <0 && (!inEquation) )
    {
        //return;
    }
    else
    {
        if(startIdx>=0 && (! inTexComment(text,startIdx)))
        {
            inEquation = true;
            //qDebug()<<"Entering equation..."<<endl;
        }

        if(endIdx>=0)
        {
            if (! inTexComment(text,endIdx))
            {
                length = endIdx - startIdx + 2; //$$ length 2
                inEquation = false;
                //qDebug()<<"end of an equation."<<endl;
            }
            else
            {
                //qDebug()<<"(a commented \\end{equation}) still in equation..."<<endl;
                length = text.length();
            }
        }
        else
        {
            //qDebug()<<"still in equation..."<<endl;
            length = text.length();
        }

        if(startIdx<0)
        {
            startIdx = 0;
        }

        if(length<0)
        {
            length = 0;
        }

        //setFormat(startIdx, length, multiLineCommentFormat);
        HighlightCmd ac;
        ac.begin=startIdx;
        ac.length = length;
        ac.format = multiLineCommentFormat;
        CList.push_back(ac);
    }

    //\[ \]
    startIdx = text.indexOf("\\[");
    endIdx   = text.indexOf("\\]");
    length = -1;

    if(startIdx <0 && (!inEquation) )
    {
        //return;
    }
    else
    {
        if(startIdx>=0 && (! inTexComment(text,startIdx)))
        {
            inEquation = true;
            //qDebug()<<"Entering equation..."<<endl;
        }

        if(endIdx>=0)
        {
            if (! inTexComment(text,endIdx))
            {
                length = endIdx - startIdx + 2; //\] length 2
                inEquation = false;
                //qDebug()<<"end of an equation."<<endl;
            }
            else
            {
                //qDebug()<<"(a commented \\end{equation}) still in equation..."<<endl;
                length = text.length();
            }
        }
        else
        {
            //qDebug()<<"still in equation..."<<endl;
            length = text.length();
        }

        if(startIdx<0)
        {
            startIdx = 0;
        }

        if(length<0)
        {
            length = 0;
        }

        //setFormat(startIdx, length, multiLineCommentFormat);
        HighlightCmd ac;
        ac.begin=startIdx;
        ac.length = length;
        ac.format = multiLineCommentFormat;
        CList.push_back(ac);
    }

    return CList;
}


void Highlighter::highlightTexEquations(const QString &text)
{
    ///  \begin{equation}....\end{equation}
    ///   $$ ... $$
    ///  \begin{falign} ... \end{falign}
    QRegExp begin = QRegExp("\\\\begin\\{equation\\}");//QRegExp("^\\\\begin\\s*\{\\s*equation\\s*}$");
    QRegExp end = QRegExp("\\\\end\\{equation\\}");//("^\\\\end\\s*\\{\\s*equation\\s*\\}$");
    int startIdx = text.indexOf("\\begin{equation}");
    int endIdx   = text.indexOf("\\end{equation}");
    int length = -1;

    if(startIdx <0 && (!inEquation) )
    {
        //return;
    }
    else
    {
        if(startIdx>=0 && (! inTexComment(text,startIdx)))
        {
            inEquation = true;
            //qDebug()<<"Entering equation..."<<endl;
        }

        if(endIdx>=0)
        {
            if (! inTexComment(text,endIdx))
            {
                length = endIdx - startIdx + 15; //\end{equation} length 15
                inEquation = false;
                //qDebug()<<"end of an equation."<<endl;
            }
            else
            {
                //qDebug()<<"(a commented \\end{equation}) still in equation..."<<endl;
                length = text.length();
            }
        }
        else
        {
            //qDebug()<<"still in equation..."<<endl;
            length = text.length();
        }

        if(startIdx<0)
        {
            startIdx = 0;
        }

        if(length<0)
        {
            length = 0;
        }

        setFormat(startIdx, length, multiLineCommentFormat);
    }

    //$$
    startIdx = text.indexOf("$$");
    endIdx   = text.indexOf("$$");
    length = -1;

    if(startIdx <0 && (!inEquation) )
    {
        //return;
    }
    else
    {
        if(startIdx>=0 && (! inTexComment(text,startIdx)))
        {
            inEquation = true;
            //qDebug()<<"Entering equation..."<<endl;
        }

        if(endIdx>=0)
        {
            if (! inTexComment(text,endIdx))
            {
                length = endIdx - startIdx + 2; //$$ length 2
                inEquation = false;
                //qDebug()<<"end of an equation."<<endl;
            }
            else
            {
                //qDebug()<<"(a commented \\end{equation}) still in equation..."<<endl;
                length = text.length();
            }
        }
        else
        {
            //qDebug()<<"still in equation..."<<endl;
            length = text.length();
        }

        if(startIdx<0)
        {
            startIdx = 0;
        }

        if(length<0)
        {
            length = 0;
        }

        setFormat(startIdx, length, multiLineCommentFormat);
    }

    //\[ \]
    startIdx = text.indexOf("\\[");
    endIdx   = text.indexOf("\\]");
    length = -1;

    if(startIdx <0 && (!inEquation) )
    {
        //return;
    }
    else
    {
        if(startIdx>=0 && (! inTexComment(text,startIdx)))
        {
            inEquation = true;
            //qDebug()<<"Entering equation..."<<endl;
        }

        if(endIdx>=0)
        {
            if (! inTexComment(text,endIdx))
            {
                length = endIdx - startIdx + 2; //\] length 2
                inEquation = false;
                //qDebug()<<"end of an equation."<<endl;
            }
            else
            {
                //qDebug()<<"(a commented \\end{equation}) still in equation..."<<endl;
                length = text.length();
            }
        }
        else
        {
            //qDebug()<<"still in equation..."<<endl;
            length = text.length();
        }

        if(startIdx<0)
        {
            startIdx = 0;
        }

        if(length<0)
        {
            length = 0;
        }

        setFormat(startIdx, length, multiLineCommentFormat);
    }

    return;
    setCurrentBlockState(0);
    int startIndex = 0;

    if (previousBlockState() != 1)
    {
        startIndex = begin.indexIn(text);
        qDebug()<<"startIndex="<<startIndex<<endl;

        if(! inTexComment(text,startIndex))
        {
            startIndex = -1;
        }
    }

    //while (startIndex >= 0 || previousBlockState() == 1) {
    int endIndex = end.indexIn(text, startIndex+1);
    qDebug()<<"endIndex="<<endIndex<<endl;

    if(! inTexComment(text,endIndex))
    {
        endIndex = -1;
    }

    int commentLength;

    if (endIndex == -1)
    {
        setCurrentBlockState(1);
        commentLength = text.length() - startIndex;
    }
    else
    {
        commentLength = endIndex - startIndex
                        + end.matchedLength();
        setCurrentBlockState(0);
    }

    if(! inTexComment(text,startIndex))
    {
        setFormat(startIndex, commentLength, multiLineCommentFormat);
    }

    //   startIndex = begin.indexIn(text, startIndex + commentLength +1);//+1 wangmenchang
    //}
}



QVector<HighlightCmd> Highlighter::getSpellErrors(const QString &text)
{
    QVector<HighlightCmd> CList;

    if (spellCheckActive)
    {
        // split text into words
        QString str = text.simplified();

        if (!str.isEmpty())
        {
            QStringList Checkliste = str.split(QRegExp("([^\\w,^\\\\]|(?=\\\\))+"),
                                               QString::SkipEmptyParts);
            int l,number;

            // check all words
            for (int i=0; i<Checkliste.size(); ++i)
            {
                str = Checkliste.at(i);

                if (str.length()>1 &&!str.startsWith('\\'))
                {
                    if (!checkWord(str))
                    {
                        number = text.count(QRegExp("\\b" + str + "\\b"));
                        l=-1;

                        // underline all incorrect occurences of misspelled word
                        for (int j=0; j < number; ++j)
                        {
                            l = text.indexOf(QRegExp("\\b" + str + "\\b"),l+1);

                            if (l>=0)
                                if(!inTexComment(text,l))
                                {
                                    //setFormat(l, str.length(), spellErrorFormat);
                                    HighlightCmd ac;
                                    ac.begin=l;
                                    ac.length = str.length();
                                    ac.format = spellErrorFormat;
                                    CList.push_back(ac);
                                }
                        } // for j
                    } // if spell chek error
                } // if str.length > 1
            } // for
        } // if str.isEmpty
    } // initial check

    return CList;
}

void Highlighter::spellCheck(const QString &text)
{
    if (spellCheckActive)
    {
        // split text into words
        QString str = text.simplified();

        if (!str.isEmpty())
        {
            QStringList Checkliste = str.split(QRegExp("([^\\w,^\\\\]|(?=\\\\))+"),
                                               QString::SkipEmptyParts);
            int l,number;

            // check all words
            for (int i=0; i<Checkliste.size(); ++i)
            {
                str = Checkliste.at(i);

                if (str.length()>1 &&!str.startsWith('\\'))
                {
                    if (!checkWord(str))
                    {
                        number = text.count(QRegExp("\\b" + str + "\\b"));
                        l=-1;

                        // underline all incorrect occurences of misspelled word
                        for (int j=0; j < number; ++j)
                        {
                            l = text.indexOf(QRegExp("\\b" + str + "\\b"),l+1);

                            if (l>=0)
                                if(!inTexComment(text,l))
                                {
                                    setFormat(l, str.length(), spellErrorFormat);
                                    //spellSuggestion(str);//debug!!!
                                }
                        } // for j
                    } // if spell chek error
                } // if str.length > 1
            } // for
        } // if str.isEmpty
    } // initial check
}

bool Highlighter::checkWord(QString word)
{
    int check;
    QByteArray encodedString;
    encodedString = codec->fromUnicode(word);
    check = sChecker->spell(encodedString.data());
    return bool(check);
}


bool Highlighter::setDict(const QString SpellDic)
{
    bool spell;

    if(SpellDic!="")
    {
        //mWords.clear();
        spell_dic=SpellDic.left(SpellDic.length()-4);

        if(sChecker!=NULL)
        {
            delete sChecker;
            sChecker=NULL;
        }

        sChecker = new Hunspell(spell_dic.toLatin1()+".aff",spell_dic.toLatin1()+".dic");
        spell_encoding=QString(sChecker->get_dic_encoding());
        codec = QTextCodec::codecForName(spell_encoding.toLatin1());
        QFileInfo fi(SpellDic);

        if (fi.exists() && fi.isReadable()) spell=true;
        else spell=false;

        // get user config dictionary
        //temperarily commented 07-21
        /*
        QSettings setting;
        QString filePath=QFileInfo(setting.fileName()).absoluteFilePath();
        filePath=filePath+"/User_"+QFileInfo(spell_dic.toLatin1()+".dic").fileName();
        qDebug()<< qPrintable(filePath) << endl;
        fi=QFileInfo(filePath);
        if (fi.exists() && fi.isReadable()){
            sChecker->add_dic(filePath.toLatin1());
        }
        else filePath="";
        */
        //spellErrorFormat.setForeground(Qt::red);//faster Cursoroperation ...
        //spellCheckFormat.setUnderlineColor(QColor(Qt::red));
        spellErrorFormat.setUnderlineStyle(QTextCharFormat::DotLine);//QTextCharFormat::SpellCheckUnderline);
    }
    else spell=false;

    spellerError=!spell;
    spellCheckActive=spellCheckActive && spell;
    rehighlight();
    return spell;
}


void Highlighter::rendering(const QString &text,QVector<HighlightCmd> clist)
{
    Q_UNUSED(text);

    for(int i=0; i<clist.size(); i++)
    {
        HighlightCmd cm = clist.at(i);
        setFormat(cm.begin,cm.length,cm.format);
    }
}


void Highlighter::highlightIt(const QString &text)
{
    highlightTexEquations(text);
    setCurrentBlockState(0);
    int startIndex = 0;

    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0)
    {
        int endIndex = commentEndExpression.indexIn(text, startIndex+1);
        int commentLength;

        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }

        if(! inTexComment(text,startIndex))
        {
            setFormat(startIndex, commentLength, multiLineCommentFormat);
        }

        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength +1);//+1 wangmenchang
    }

    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);

        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    highlightComment(text);

    if(bSpellCheck)
    {
        spellCheck(text);
    }

    if(!FindStr.isEmpty())
    {
        int fdpos =-1;

        if(isCaseSensetive)
        {
            fdpos=text.indexOf(FindStr,0,Qt::CaseSensitive);
        }
        else
        {
            fdpos=text.indexOf(FindStr,0,Qt::CaseInsensitive);
        }

        while(fdpos>=0)
        {
            setFormat(fdpos,FindStr.length(),findFormat);

            if(isCaseSensetive)
            {
                fdpos = text.indexOf(FindStr,fdpos+FindStr.length(),Qt::CaseSensitive);
            }
            else
            {
                fdpos = text.indexOf(FindStr,fdpos+FindStr.length(),Qt::CaseInsensitive);
            }
        }
    }
}




void Highlighter::highlightBlock(const QString &text)
{
    return;
    highlightIt(text);
    //QtConcurrent::run(this,&Highlighter::highlightIt, text);
}



// TODO: Check how to to this.
