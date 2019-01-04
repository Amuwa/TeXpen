/****************************************************************************
 **
 ** Copyright (C) 2013 MengChang Wang
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

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>
#include "3rd-party/hunspell/hunspell.hxx"
#include  <QVector>
#include <QTextBlockUserData>
#include<QStringList>
#include "qtexedit.h"

class QTextDocument;
class QTeXEdit;

struct HighlightData{
    enum {
        Keyword,
        Comment,
        Equation,
        Greeks,
        EscapeCharacter,
        SpellError
    } HType;
    int begin;
    int length;
};

class HighlightDataList:public QTextBlockUserData
{    //Q_OBJECT
 public:
     HighlightDataList(){}
     ~HighlightDataList(){ Clear();}
      QVector<HighlightData>& getHlData(){return HData;}
      void AddData(HighlightData hd){HData.push_back(hd);}
      void Clear(){HData.clear();}

 protected:
      QVector<HighlightData> HData;

};



class LaTeXHighlighter:public QSyntaxHighlighter
{
    Q_OBJECT

    //for spell checking control
    bool bSpellCheck;

    //for searching highlighting
    QString FindStr;
    bool isCaseSensetive;

    //for parsing document, a simple mutex lock
    bool isParsing;

    //for equation parsing
    bool isEquationOpen;

    bool isTeXDoc;

public:
    LaTeXHighlighter(QTextDocument *parent = 0);

    QTeXEdit* theEditor;


    void ParseDocument();

    void ParseTextBlock(QTextBlock blk);

    void setTeXDoc(bool isTex);


    void enableSpellCheck(bool b);//set spell checking
    void setFind(QString sel, bool isCase); //set find string highlighting

    bool setDict(const QString SpellDic);

    QStringList spellSuggestion(QString word);


signals:
    void pleaseRender(int blockNumber);
public slots:
    void RenderBlock(int blockNumber);
    void RenderCurrentBlock();

protected:

    void ParseKeyword(QString txt);
    void ParseEquation(const QString& txt);
    void ParseComment(const QString& txt);
    void ParseGreeks(const QString& text);
    void ParseEscapeChar(const QString &text);
    void ParseSpellError(const QString& text);


    void parseEquation(const QString& txt, QRegExp begin, QRegExp end);

    QVector<QRegExp> Keywords;
    QVector<QRegExp> Comments;
    QVector<QRegExp> Greeks;
    QVector<QRegExp> EscChars;

    QTextCharFormat keywordFormat;
    QTextCharFormat equationFormat;
    QTextCharFormat greeksFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat escCharFormat;
    QTextCharFormat spellErrorFormat;
    QTextCharFormat findFormat;



protected:
    void highlightBlock(const QString &text);
    void realHighlight(const QString& text );

protected:
    Hunspell* sChecker;
    bool spellCheckActive,spellerError;
    QString spell_dic, spell_encoding;
    void spellCheck(const QString &text);
    bool checkWord(QString word);
    QTextCodec* codec;


};








 struct HighlightCmd{
     QTextCharFormat format;
     int begin;
     int length;
 };





 //QVector<HighlightCmd> HCmdList;

 class Highlighter : public QSyntaxHighlighter
 {
     Q_OBJECT

     bool bSpellCheck;
     QString FindStr;
     bool isCaseSensetive;
 public:
     Highlighter(QTextDocument *parent = 0);
     void enableSpellCheck(bool b);
     void setFind(QString sel, bool isCase);

     void highlightIt(const QString &text);

     void fillCmdList(const QString &text); //fill the list

     QVector<HighlightCmd> getTexEquations(const QString &text);
     QVector<HighlightCmd> getSpellErrors(const QString &text);


 signals:
     void fillOk(const QString &text, QVector<HighlightCmd> clist);
 public slots:
     void rendering(const QString &text,QVector<HighlightCmd> clist);
 protected:
     void highlightBlock(const QString &text);

 private:
     struct HighlightingRule
     {
         QRegExp pattern;
         QTextCharFormat format;
     };
     QVector<HighlightingRule> highlightingRules;

     QRegExp commentStartExpression;
     QRegExp commentEndExpression;

     QTextCharFormat keywordFormat;
     QTextCharFormat equationKeyword;
     QTextCharFormat equationGreeks;
     QTextCharFormat equationOperators;

     QTextCharFormat classFormat;
     QTextCharFormat singleLineCommentFormat;
     QTextCharFormat multiLineCommentFormat;
     QTextCharFormat quotationFormat;
     QTextCharFormat functionFormat;

     QTextCharFormat spellErrorFormat;

     QTextCharFormat findFormat;

     void highlightTexEquations(const QString &text);
     void highlightTexEquations(const QString &text, QString beginToken, QString endToken);
     void highlightComment(const QString &text);

     Hunspell* sChecker;
     bool spellCheckActive,spellerError;
     QString spell_dic, spell_encoding;
     void spellCheck(const QString &text);
     bool checkWord(QString word);

     QTextCodec* codec;
 public:
     bool setDict(const QString SpellDic);


 };

 #endif

