/*
 * Copyright (c) MengChang Wang <wangmengchang@gmail.com>
 * Webpage:  http://www.journalhome.com/texpen/
 *
 *
*/

#include "../headers/runthread.h"
#include <QDebug>

#include <exception>
#include <iostream>

RunThread::RunThread(QObject *parent) :
    QThread(parent)
{
    proc = NULL;
    dir ="";
    currentCMDidx = 0;
    wnd=nullptr;
    connect(this,SIGNAL(nextCMD()),this,SLOT(runNext()));
    isrunning = false;
}

void RunThread::runNext(){
    qDebug()<<"runNext()..."<<endl;
    if(currentCMDidx>=0 && currentCMDidx<commands.count()){
        try{
            isrunning = true;
            execute(commands.at(currentCMDidx));
        }catch(QException ex){
            if(wnd != nullptr){
                wnd->showMsg(ex.what());
            }
        }
    } else {
        if(wnd!=NULL){
            qDebug()<<"runNext()... call onCmdFailed"<< endl;
            isrunning=false;
            wnd->onCmdFailed();
        }
    }
    qDebug()<<"runNext()...finished."<<endl;
}

void RunThread::execute(Command cmd){
    qDebug()<<"TO RUN:"<<cmd.program<<"   "<<cmd.arglist.join(",")<<"..."<<endl;
    isrunning = true;
    /*
    try{
        if(proc!=NULL){
            if(proc->state()==QProcess::Running){
                if(!proc->waitForFinished()){
                    //qDebug()<<"A proc is running, now terminate() it...";
                    proc->terminate();
                }
            }
            delete proc;proc=NULL;
        }
    }catch(std::exception e){
        if(proc != NULL){delete proc;}
        proc = NULL;
        QString msg(e.what());
        qDebug()<<"execute error :" << msg << endl;
    }*/

    try{
        //proc = new QProcess;
        isrunning = true;
        QProcess xproc;
        proc = &xproc;
        connect(proc,SIGNAL(readyRead()),wnd,
                SLOT(readProcOutput())
                );
        connect(proc, SIGNAL(finished(int)),
                this, SLOT(onFinish(int))
                );
        connect(proc, SIGNAL(readyReadStandardOutput()),
                this, SLOT(onReadStd())
                );

        connect(proc, SIGNAL(readChannelFinished()),
                this, SLOT(onReadChannel())
                );
        connect(proc, SIGNAL(error(QProcess::ProcessError)),this,SLOT(onError(QProcess::ProcessError)));
        proc->setWorkingDirectory(dir);
        if(cmd.arglist.isEmpty()){
            proc->start(cmd.program);
        }else{
            proc->start(cmd.program,cmd.arglist);
        }
        proc->waitForFinished(-1);
        //if(proc->state()==QProcess::Running){
        //    proc->terminate();
        //}
        proc=nullptr;
        qDebug()<<"RUN Ready."<<endl;
        isrunning = false;
    }catch(std::exception e){
        QString msg(e.what());
        qDebug()<<msg<<endl;
        isrunning = false;
    }

}

void RunThread::run (){
    //qDebug()<<"Try to run..."<<endl;
    currentCMDidx=0;
    runNext();
    /*
    qDebug()<<"now run()...";
    if(prog.isEmpty()){
        return;
    }
    if(proc!=NULL){
        if(proc->state()==QProcess::Running){
            if(!proc->waitForFinished()){
                qDebug()<<"A proc is running, now terminate() it...";
                proc->terminate();
            }
        }
        delete proc;proc=NULL;
    }

    proc = new QProcess;
    connect(proc,SIGNAL(readyRead()),wnd,
            SLOT(readProcOutput())
            );
    connect(proc, SIGNAL(finished(int)),
            this, SLOT(onFinish(int))
            );

    //connect(proc, SIGNAL(stateChanged(QProcess::ProcessState)),
    //        this, SLOT(onState(QProcess::ProcessState))
    //        );

    connect(proc, SIGNAL(readyReadStandardOutput()),
            this, SLOT(onReadStd())
            );

    connect(proc, SIGNAL(readChannelFinished()),
            this, SLOT(onReadChannel())
            );
    connect(proc, SIGNAL(error(QProcess::ProcessError)),this,SLOT(onError(QProcess::ProcessError)));

    //qDebug()<<prog<<arglist<<endl;
    if(arglist.isEmpty()){
        proc->setWorkingDirectory(dir);
        proc->start(prog);
        //QByteArray ba = prog.toUtf8();
        //system(ba.data());
    }else{
        proc->start(prog,arglist);
    }
    */
}

bool isQuoted(QString src, int pos){
    if(pos>= src.length()){
        return false;
    }
    bool in=false;
    for(int i=0;i<pos;i++){
        if(src.at(i) == QChar('\"')){
            in = !in;
        }
    }
    return in;
}

void RunThread::getCommandFrom(QString cmdstr){
    if(cmdstr.contains("|")){
        QStringList cmds = cmdstr.split("|");
        foreach(QString scmd, cmds){
            int pos = scmd.indexOf(" ");
            while(isQuoted(scmd,pos)){
                pos = scmd.indexOf(" ",pos+1);
            }
            if(pos<0){
                Command cmd;
                cmd.program = scmd;
                cmd.arglist.clear();
                commands.append(cmd);
            }else{
                Command cmd;
                cmd.program = scmd.mid(0,pos);
                cmd.arglist.clear();
                cmd.arglist.append(scmd.mid(pos+1));
                commands.append(cmd);
            }
        }
    }else{
        int pos = cmdstr.indexOf(" ");
        while(isQuoted(cmdstr,pos)){
            pos = cmdstr.indexOf(" ",pos+1);
        }
        if(pos<0){
            Command cmd;
            cmd.program = cmdstr;
            cmd.arglist.clear();
            commands.append(cmd);
        }else{
            Command cmd;
            cmd.program = cmdstr.mid(0,pos);
            cmd.arglist.clear();
            cmd.arglist.append(cmdstr.mid(pos+1));
            commands.append(cmd);
        }
    }
}

void RunThread::AddCommand(QString cmd,QStringList args){
    Command c;
    c.program=cmd;
    c.arglist.clear();
    foreach(QString a , args){
        c.arglist.append(a);
    }
    commands.append(c);
}

void RunThread::StopAll(){
    currentCMDidx = 2560;
    if(isRunning()){
        if(proc != nullptr){
            proc->kill();
        }
    }
    if(proc!=nullptr){
        //delete proc;
        proc = nullptr;
    }
}


bool RunThread::isCmdRunning(){
    if(proc==nullptr){
        return false;
    }
    else if(proc->state() == QProcess::Running || proc->state() ==QProcess::Starting){
        return true;
    }
    return isrunning;
}

void RunThread::read(){
    //qDebug()<<"RunThread::read() called."<<endl;
    if(proc != nullptr){
    qDebug()<< proc->readAll()<<endl;
    }
}

void RunThread::onReadStd(){
    //qDebug()<<"RunThread::onReadStd() called."<<endl;
    if(proc != nullptr){
    qDebug()<< proc->readAllStandardOutput()<<endl;
    }
}

void RunThread::onReadChannel(){
    //qDebug()<<"RunThread::onReadChannel() called."<<endl;
    if(proc != nullptr){
    qDebug()<< proc->readChannel()<<endl;
    }
}

void RunThread::onFinish(int ec){
    try{
        qDebug()<<"onFinish ..."<<endl;
        currentCMDidx++;
        if(currentCMDidx<commands.count()){
            emit(nextCMD());
        }else{
            isrunning = false;
            if(wnd!=NULL){
                wnd->onCmdFinish();
            }
            qDebug()<<"Finished, exit code ="<<ec<<endl;
        }
        qDebug()<<"onFinish ... finished."<<endl;
    }catch(std::exception e){
        QString msg(e.what());
        qDebug()<<"ERROR: "<<msg<<endl;
        isrunning = false;
    }
}

/*
void RunThread::onState(QProcess::ProcessState st){
    if(st == QProcess::NotRunning){
        qDebug()<<"Proc is not running.."<<endl;
    }
    else if(st == QProcess::Starting){
        qDebug()<<"Proc is try to start.."<<endl;
    }else if(st == QProcess::Running){
        qDebug()<<"Proc is now running.."<<endl;
    }else{
        qDebug()<<"Proc is doing what? Even the system does not know..."<<endl;
    }
}
*/


void RunThread::setDir(QString& d){
    dir = d;
}


void RunThread::reset(){
    commands.clear();
}

void RunThread::onError(QProcess::ProcessError er){
    qDebug()<<"onError called."<<endl;
    switch(er){
    case QProcess::FailedToStart:
        qDebug()<<"[Error] RunThread: FailedToStart"<<endl;
        break;
    case QProcess::Crashed:
        qDebug()<<"[Error] RunThread: Crashed"<<endl;
        break;
    case QProcess::Timedout:
        qDebug()<<"[Error] RunThread: Timed Out"<<endl;
        break;
    case QProcess::WriteError:
        qDebug()<<"[Error] RunThread: Write Error"<<endl;
        break;
    case QProcess::ReadError:
        qDebug()<<"[Error] RunThread: Read Error"<<endl;
        break;
    case QProcess::UnknownError:
        qDebug()<<"[Error] RunThread: Unknown Error"<<endl;
        break;
    default:
        break;
    }

    if(wnd!=NULL){
        wnd->onCmdFailed();
    }
}

