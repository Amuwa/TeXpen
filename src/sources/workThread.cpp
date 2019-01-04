#include "../headers/workThread.h"

EqRender::EqRender(QObject* p)
    :QThread(p)
{

}
void	EqRender::run (){}
void EqRender::startRendering(){}
void EqRender::deliverResult(){}

void EqRender::getTmpFileName()//set a tmp file name by md5 didgesting
{

}

QString EqRender::getWholeTeXSrc()//generate the whole tex file including the equation source
{
    QString rs="";
    return rs;
}

void EqRender::render(){//call latex/... to render equation, the path of dvipng should be set
}

