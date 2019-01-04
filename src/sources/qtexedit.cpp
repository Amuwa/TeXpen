#include "../headers/qtexedit.h"
#include <QDebug>

Completor::Completor(QWidget *parent):
    QListWidget(parent)
{
    theEdit=NULL;
}

void Completor::keyPressEvent ( QKeyEvent * e ){
    if(e->key() == Qt::Key_Enter ||e->key() == Qt::Key_Return){
        //qDebug()<<"{Completor::keyPressEven()}Enter is pressed."<<endl;
        QListWidgetItem* it = this->currentItem();
        emit(submit(it));
        return;
    }else if(e->key() == Qt::Key_Space){
        //qDebug()<<"{Completor::keyPressEven()}Space is pressed."<<endl;
        QListWidgetItem* it = this->currentItem();
        emit(submit(it));
    }else if(e->key() == Qt::Key_Down){
        if(currentRow() >= count()-1){
            setCurrentRow(0);
            return;
        }
    }else if(e->key() == Qt::Key_Up){
        if(currentRow() == 0){
            setCurrentRow(count()-1);
            return;
        }
    }else if(e->key() == Qt::Key_K || e->key() == Qt::Key_W){
        if(currentRow() == 0){
            setCurrentRow(count()-1);
            return;
        }else{
            setCurrentRow(currentRow()-1);
            return;
        }
    }else if(e->key() == Qt::Key_J || e->key() == Qt::Key_S){
        if(currentRow() >= count()-1){
            setCurrentRow(0);
            return;
        }else{
            setCurrentRow(currentRow()+1);
            return;
        }
    }else if(e->key() == Qt::Key_Tab){
        if(theEdit!=NULL){
            theEdit->setFocus();
        }
        this->clear();
        this->hide();
        return;
    }
    QListWidget::keyPressEvent(e);
}
void Completor::keyReleaseEvent ( QKeyEvent * e ){
    QListWidget::keyReleaseEvent(e);
}

#include <QPainter>

QTeXEdit::QTeXEdit(QWidget *parent) :
    QPlainTextEdit(parent)
{
    cmp = NULL;
    sChecker = NULL;
    highlighter=NULL;
    frequentWords.clear();
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

    updateLineNumberAreaWidth(0);
}

int QTeXEdit::lineNumberAreaWidth(){
    int digits = 1;
    int max = qMax(1,   document()->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

int lastVTP=0;
int lastVBN=500;
int QTeXEdit::lastVisibleBlockNumber(){
    return lastVBN;
}



void QTeXEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);
    painter.setFont(this->font());

//![extraAreaPaintEvent_0]

//![extraAreaPaintEvent_1]
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();int thistop = blockNumber;
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
//![extraAreaPaintEvent_1]

//![extraAreaPaintEvent_2]
    while (block.isValid() && top<this->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top() &&  top <= event->rect().bottom()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top+fontMetrics().leading(), lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
    int thisbottom = blockNumber;
    if(highlighter!=NULL && (thistop != lastVTP || thisbottom != lastVBN )  ){
        highlighter->rehighlight();
        lastVTP = thistop;
        lastVBN = thisbottom;
    }
}


void QTeXEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
    if(highlighter!=NULL){
        highlighter->rehighlight();
    }
}

void QTeXEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void QTeXEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect())){
        updateLineNumberAreaWidth(0);
    }
}

void QTeXEdit::applySpellDict(QString dicName, QString path){
    if(sChecker!=NULL){
        delete sChecker;
        sChecker = NULL;
    }
    QString affpath= path+dicName+".aff";
    QString dicpath= path+dicName+".dic";
    sChecker = new Hunspell(affpath.toLatin1(),dicpath.toLatin1());

}

void QTeXEdit::setCompleter(Completor* w){
    cmp = w;
    w->theEdit = this;
    //connect(cmp,SIGNAL(itemActivated(QListWidgetItem*)),this,SLOT(activateItem(QListWidgetItem*)));
    connect(cmp, SIGNAL(submit(QListWidgetItem*)),this,SLOT(activateItem(QListWidgetItem*)));
}
void QTeXEdit::setHighlighter(LaTeXHighlighter* lgt){
    highlighter = lgt;
    if(lgt!=NULL){
        lgt->theEditor=this;
    }
}

#include <QAction>
void QTeXEdit::replaceCurrentWordWith(bool b){
    if(!b){;}
     if (QAction *action = qobject_cast<QAction*>(sender())) {
         QString newWd = action->data().toString();
         QString curword="" ;
         QTextCursor cr=textCursor();
         cr.select(QTextCursor::WordUnderCursor);
         curword=cr.selectedText();
         qDebug()<<"curword = "<<curword<<" is going to be: "<<newWd<<endl;
         cr.removeSelectedText();
         cr.insertText(newWd);
         setTextCursor(cr);
     }else {
        qDebug()<<"not ok!!!"<<endl;
     }
}


#include <QMenu>
void QTeXEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu= new QMenu(this);
    QMenu *stdmenu = createStandardContextMenu();
    //if(!textCursor().selectedText().isEmpty()){
    //    menu->addAction(tr("Check grammar/expression..."),this,SLOT());
    //}
    //...
    if(textCursor().selectedText().isEmpty()){
        //event->pos();
        //event->
    if(highlighter!=NULL){
        QString curword="" ;
        QTextCursor cr=textCursor();
        cr.select(QTextCursor::WordUnderCursor);
        curword=cr.selectedText();
        qDebug()<<"curword="<<curword<<endl;
        cr.movePosition(QTextCursor::EndOfWord);

        if(curword.length()>0){
            QStringList sugs = highlighter->spellSuggestion(curword);
            if(sugs.count()>0){
                int c=0;
                foreach(QString item, sugs){
                    (menu->addAction(item,this,SLOT(replaceCurrentWordWith(bool))))->setData(item);
                    c++;
                    if(c>15){break;}
                }
                //
                menu->setTitle("Spell Suggestion...");
                stdmenu->addSeparator();
                stdmenu->addMenu(menu);
            }
        }
    }
    }    
    stdmenu->exec(event->globalPos());



    delete menu;
}

void QTeXEdit:: manuallyHighlight(){
    if(highlighter!=NULL){
        highlighter->rehighlight();
    }
}

void QTeXEdit::refreshFWords(){
    //separate words
    //frequentWords.clear();
    QString txt = this->toPlainText();
    QString strx = txt.simplified();
    if(strx.isEmpty()){
        return;
    }
    QStringList Checkliste = strx.split(QRegExp("([^\\w,^\\\\]|(?=\\\\))+"),
                        QString::SkipEmptyParts);
    int number;
    // check all words
    for (int i=0; i<Checkliste.size(); ++i) {
        QString str = Checkliste.at(i);
        if(frequentWords.contains(str)){
            continue;
        }
        if (str.length()>8 &&
                !str.contains("$")&&
                !str.contains("\\") &&
                !str.contains(",") &&
                !str.contains("@") &&
                !str.contains("\%") &&
                !str.contains("(") &&
                !str.contains("!")
        )
        {
            number = strx.count(QRegExp("\\b" + str + "\\b"));
            if(number >= 10){
                frequentWords.append(str);
            }
        }
    }



}


void QTeXEdit::keyPressEvent ( QKeyEvent * e ){   
    if(cmp!=NULL && (! cmp->isHidden())){
        cmp->setCurrentRow(0);
        if(e->key()==Qt::Key_Up
                || (e->key()== Qt::Key_K && (e->modifiers()==Qt::ControlModifier||e->modifiers()==Qt::MetaModifier))
                ){
                cmp->setFocus();
                cmp->setCurrentRow(cmp->count()-1);
                return;
        }else if(e->key()==Qt::Key_Down
                 || (e->key()== Qt::Key_J && (e->modifiers()==Qt::ControlModifier||e->modifiers()==Qt::MetaModifier))
                     ||e->key()== Qt::Key_Tab
                 ){
                cmp->setFocus();
                int idx = 1;
                if(cmp->count()==1){idx=0;}
                cmp->setCurrentRow(idx);
                return;
        }else if(e->key()==Qt::Key_Left || e->key()== Qt::Key_Home || e->key()== Qt::Key_End ||
                 e->key()== Qt::Key_PageDown || e->key()== Qt::Key_PageUp ||e->key() == Qt::Key_Escape
                 ){
            cmp->clear();
            cmp->hide();
            this->setFocus();
            return;
        }else if(e->key()==Qt::Key_Right){
            QString txt = cmp->currentItem()->text();
            emit(askToComplete(txt));
            this->setFocus();
            cmp->clear();
            cmp->hide();
            this->setFocus();
            return;
        }
        else if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return){//e->key()  == Qt::Key_Space ||
            QString txt = cmp->currentItem()->text();
            emit(askToComplete(txt));
            this->setFocus();
            return;
        }else if(e->key() == Qt::Key_Escape){
            cmp->clear();
            cmp->hide();
            this->setFocus();
            return;
        }
    }

    if(e->matches(QKeySequence::Copy)){
        //qDebug()<<"QKeySequence::Copy pressd"<<endl;
        copy();
        return;
    }
    else if(e->matches(QKeySequence::Cut)){
        //qDebug()<<"QKeySequence::Cut pressd"<<endl;
        cut();
        return;
    }

    else if(e->key()==Qt::Key_Comma && (e->modifiers()==Qt::ControlModifier||e->modifiers()==Qt::MetaModifier)){
        //emit(askToGoogleSearch());
        return;
    }
    else if(e->key()==Qt::Key_Equal && (e->modifiers()==Qt::ControlModifier||e->modifiers()==Qt::MetaModifier)){
        return;
    }else if(e->key()==Qt::Key_Slash && (e->modifiers()==Qt::ControlModifier||e->modifiers()==Qt::MetaModifier)){
        return;
    }else if((e->key()==Qt::Key_M || e->key()==Qt::Key_E  ) && (e->modifiers()==Qt::ControlModifier||e->modifiers()==Qt::MetaModifier)){
        return;
    }
    else if (e->key()==Qt::Key_F8){
        qDebug()<<"F8 pressed";
        //realtimePreview();
        emit(askToRealTimePreview());
    }

    QPlainTextEdit::keyPressEvent(e);
}

void QTeXEdit::keyReleaseEvent ( QKeyEvent * e ){
    if(e->matches(QKeySequence::Copy)){
        this->copy();
        return;
    }
    else if(e->matches(QKeySequence::Cut)){
        //this->cut();//this would be a bug, since cut for the second time will clear the clipboard!!
        return;
    }

    else if((e->key()==Qt::Key_M || e->key()==Qt::Key_E  ) && (e->modifiers()==Qt::ControlModifier||e->modifiers()==Qt::MetaModifier)){
        QString word ="";
        if(textCursor().hasSelection()){
            word = textCursor().selectedText();
            emit(askToDict(word));
        }else{
            QTextCursor cur=textCursor();
            cur.movePosition(QTextCursor::StartOfWord);
            cur.movePosition(QTextCursor::EndOfWord,QTextCursor::KeepAnchor);//.select(QTextCursor::WordUnderCursor);
            setTextCursor(cur);
                word = cur.selectedText();
                //qDebug()<<"Word ="<<word<<"++"<<endl;

                for(int tt =0;tt<3;tt++){
                if(word =="" ||word == " " || word == "," || word == ";" || word == "." ||word == "-"){
                    cur=textCursor();
                    cur.movePosition(QTextCursor::PreviousWord);  //qDebug()<<"<--"<<endl;
                    cur.movePosition(QTextCursor::PreviousWord);  //qDebug()<<"<--"<<endl;
                    cur.movePosition(QTextCursor::StartOfWord); //qDebug()<<"|<-"<<endl;
                    cur.movePosition(QTextCursor::EndOfWord,QTextCursor::KeepAnchor);  //qDebug()<<"|-->|"<<endl;
                    setTextCursor(cur);
                    word = cur.selectedText();
                    //qDebug()<<"Word ="<<word<<"++"<<endl;
                }else{
                    break;
                }}

                if(word =="" ||word == " " || word == "," || word == ";" || word == "." ||word == "-"){
                    word ="";
                }
                cur.movePosition(QTextCursor::EndOfWord);
                setTextCursor(cur);
            emit(askToDict(word));
        }
        return;
    }else if(e->key()==Qt::Key_Equal && (e->modifiers()==Qt::ControlModifier||e->modifiers()==Qt::MetaModifier)){
        QString word ="";
        if(textCursor().hasSelection()){
            word = textCursor().selectedText();
            emit(askToWiki(word));
        }else{
            QTextCursor cur=textCursor();
            cur.movePosition(QTextCursor::StartOfWord);
            cur.movePosition(QTextCursor::EndOfWord,QTextCursor::KeepAnchor);//.select(QTextCursor::WordUnderCursor);
            setTextCursor(cur);
                word = cur.selectedText();
                //qDebug()<<"Word ="<<word<<"++"<<endl;

                for(int tt =0;tt<3;tt++){
                if(word =="" ||word == " " || word == "," || word == ";" || word == "." ||word == "-"){
                    cur=textCursor();
                    cur.movePosition(QTextCursor::PreviousWord);  //qDebug()<<"<--"<<endl;
                    cur.movePosition(QTextCursor::PreviousWord);  //qDebug()<<"<--"<<endl;
                    cur.movePosition(QTextCursor::StartOfWord); //qDebug()<<"|<-"<<endl;
                    cur.movePosition(QTextCursor::EndOfWord,QTextCursor::KeepAnchor);  //qDebug()<<"|-->|"<<endl;
                    setTextCursor(cur);
                    word = cur.selectedText();
                    //qDebug()<<"Word ="<<word<<"++"<<endl;
                }else{
                    break;
                }}

                if(word =="" ||word == " " || word == "," || word == ";" || word == "." ||word == "-"){
                    word ="";
                }
                cur.movePosition(QTextCursor::EndOfWord);
                setTextCursor(cur);
            emit(askToWiki(word));
        }
        return;
    }
    else if((e->key()==Qt::Key_Comma || e->key() == Qt::Key_Colon )&& (e->modifiers()==Qt::ControlModifier||e->modifiers()==Qt::MetaModifier)){
        keywordForSearch="";
        if(textCursor().hasSelection()){
            keywordForSearch = textCursor().selectedText();
        }else{
            QTextCursor cur=textCursor();
            cur.movePosition(QTextCursor::StartOfWord);
            cur.movePosition(QTextCursor::EndOfWord,QTextCursor::KeepAnchor);//.select(QTextCursor::WordUnderCursor);
            setTextCursor(cur);
                keywordForSearch = cur.selectedText();
                //qDebug()<<"Word ="<<word<<"++"<<endl;

                for(int tt =0;tt<3;tt++){
                if(keywordForSearch =="" ||keywordForSearch == " " || keywordForSearch == "," || keywordForSearch == ";" || keywordForSearch == "." ||keywordForSearch == "-"){
                    cur=textCursor();
                    cur.movePosition(QTextCursor::PreviousWord);  //qDebug()<<"<--"<<endl;
                    cur.movePosition(QTextCursor::PreviousWord);  //qDebug()<<"<--"<<endl;
                    cur.movePosition(QTextCursor::StartOfWord); //qDebug()<<"|<-"<<endl;
                    cur.movePosition(QTextCursor::EndOfWord,QTextCursor::KeepAnchor);  //qDebug()<<"|-->|"<<endl;
                    setTextCursor(cur);
                    keywordForSearch = cur.selectedText();
                    //qDebug()<<"Word ="<<word<<"++"<<endl;
                }else{
                    break;
                }}

                if(keywordForSearch =="" ||keywordForSearch == " " || keywordForSearch == "," || keywordForSearch == ";" || keywordForSearch == "." ||keywordForSearch == "-"){
                    keywordForSearch ="";
                }
                cur.movePosition(QTextCursor::EndOfWord);
                setTextCursor(cur);
        }
        emit(askToGoogleSearch());
        return;
    }else if(e->key()==Qt::Key_Slash && (e->modifiers()==Qt::ControlModifier||e->modifiers()==Qt::MetaModifier)){
        QString word ="";
        if(textCursor().hasSelection()){
            word = textCursor().selectedText();
        }
        emit(askToGinger(word));
        //emit(askToGinger(QString));
        return;
    }


    QPlainTextEdit::keyReleaseEvent(e);
}

void QTeXEdit::activateItem(QListWidgetItem* it){
    QString txt = it->text();
    emit(askToComplete(txt));
}

#include <QApplication>
#include <QClipboard>

void QTeXEdit::copy(){
    //qDebug()<<"My Copying...";
    QString txt = this->textCursor().selectedText();
    QString nl = QString::fromUtf8("\u2029");
    txt = txt.replace(nl,"\n");
    qApp->clipboard()->setText(txt);
    return;
}
void QTeXEdit::cut(){
    //qDebug()<<"My Cutting...";
    QString txt = this->textCursor().selectedText();
    QString nl = QString::fromUtf8("\u2029");
    txt = txt.replace(nl,"\n");

    QPlainTextEdit::cut();

    qApp->clipboard()->setText(txt);
    return;
}


void QTeXEdit::inputMethodEvent ( QInputMethodEvent * e ){
    //qDebug()<<"Commiting String: {"<<e->commitString()<<"}"<<endl;
    QPlainTextEdit::inputMethodEvent(e);
}

void QTeXEdit::mousePressEvent(QMouseEvent *e){
    if(cmp!=NULL && (! cmp->isHidden())){
        cmp->clear();
        cmp->hide();
        this->setFocus();
    }
    qDebug()<<"Mouse Pressed"<<endl;
    if(textCursor().selectedText().isEmpty()){
        //move cursor
        QTextCursor cr = cursorForPosition(e->pos());
        this->setTextCursor(cr);
    }


    QPlainTextEdit::mousePressEvent(e);
}
