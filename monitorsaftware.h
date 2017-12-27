#ifndef MONITORSAFTWARE_H
#define MONITORSAFTWARE_H

#include <QThread>
#include <QDebug>
#include <QProcess>

class MonitorSaftware : public QThread
{
    Q_OBJECT
public:
    explicit MonitorSaftware(QObject *parent = 0);

signals:
    void sendMsg(QString msg);

public slots:
    void ReceiveMsg(QString msg);

private:
    bool isExit;
    QString PathName;
    QString Name;


    void run();

};

#endif // MONITORSAFTWARE_H
