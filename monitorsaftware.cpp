#include "monitorsaftware.h"

MonitorSaftware::MonitorSaftware(QObject *parent) :
    QThread(parent)
{
    isExit = false;

    Name = "imagename eq ";
}

void MonitorSaftware::run()
{
    static long cnt = 0;
    QProcess StartSW(this);
    QStringList pn = PathName.split("/");
    foreach (QString tmp, pn) {
        if(!tmp.contains(".exe")) continue;
        Name += tmp;
        break;
    }
    msleep(3000);
    while (1)
    {
        // D:\\Anke - LJJ\\Project\\微震联合定位\\采集软件\\UnionSample\\UnionSample\\Release\\UnionSample.exe
        /*
         * startDetached()中的程序路径必须"D:/software/wireShark/Wireshark.exe"或者"D:\\software\\wireShark\\Wireshark.exe"
         * 注：或者配置程序的环境变量, 直接填写程序名称即可：cmd.exe
         * 注：程序后面不加后缀也是可以的, "D:/software/wireShark/Wireshark"
         * 注：如果路径中带有空格是不能启动程序的
         * 注：如果路径中有空格,那个可以带参数便可以使用
         * 注：建议使用路径方式(获取是否运行信息时只能使用这种方式):"D:/Anke - LJJ/Project/微震联合定位/采集软件/UnionSample/UnionSample/Release/UnionSample.exe";
         * 注：这个需要在线程中实现,不能在注进程中写
         */
        QString str = QString("第%1次检测").arg(cnt);
//        PathName = "D:/Anke - LJJ/Project/微震联合定位/采集软件/UnionSample/UnionSample/Release/UnionSample.exe";
//        QString PathName = "cmd.exe";
        if(PathName == "") continue;
        if(!PathName.contains(".exe")) continue;
        if(Name == "") continue;
        if(!Name.contains(".exe")) continue;
        QString cmd = "tasklist";
        QStringList arg;
        arg << "-fi" << Name;
        StartSW.start(cmd, arg);
        StartSW.waitForFinished();
        QString info = QString::fromLocal8Bit(StartSW.readAllStandardOutput());
        qDebug() << "---" << info << "---";
        if(info.contains("没有运行的任务匹配指定标准")){
            StartSW.startDetached(PathName, QStringList(PathName));
            emit sendMsg(str + ", 没有见到程序运行，正在开启程序...");
        }
        else {
            qDebug() << "程序已经在运行！";
            emit sendMsg(str + ", 程序已经在运行...");
        }

        if(isExit)
            break;
        msleep(5000);
        cnt++;
    }
}


void MonitorSaftware::ReceiveMsg(QString msg)
{
    if(msg == "exit") {
        isExit = true;
        sendMsg("exit");
    }
    else if(msg == "monitor") {
        isExit = false;
        sendMsg("monitor");
    }
    else {
        PathName = msg;
    }
}
