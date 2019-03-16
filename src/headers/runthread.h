#ifndef RUNTHREAD_H
#define RUNTHREAD_H

#include <QThread>
#include <QProcess>
#include "headers/mainwindow.h"
class MainWindow;

struct Command{
    QString program;
    QStringList arglist;
};


class RunThread : public QThread
{
    Q_OBJECT
    QProcess *proc;
    QString dir;

    MainWindow* wnd;
    QList<Command> commands;
    int currentCMDidx;
public:
    explicit RunThread(QObject *parent = 0);
    void	run ();
    QProcess* getProc(){return proc;}
    void setWindow(MainWindow* w){wnd =w;}
    bool isCmdRunning();
    void AddCommand(QString cmd,QStringList args);
    void getCommandFrom(QString cmdstr);
    void StopAll();
signals:
    void nextCMD();
    void updateStatus(QString);
    
public slots:
    void read();
    //void setProg(QString& p);
    void setDir(QString& d);
    //void addArg(QString& ag);
    void reset();
    void onFinish(int ec);
    //void onState(QProcess::ProcessState);
    void onReadStd();
    void onReadChannel();
    void onError(QProcess::ProcessError er);
    void execute(Command cmd);
    void runNext();
};

#endif // RUNTHREAD_H
