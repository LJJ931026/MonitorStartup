#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QTextDocument>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include "monitorsaftware.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

signals:
    void sendMsg(QString PathName);

private slots:
    void on_pushButton_monitor_clicked();

    void on_pushButton_open_clicked();

    void on_lineEdit_editingFinished();

    void ReceiveMsg(QString msg);

    void closeEvent(QCloseEvent *e);

    void ActivateSystemTray(QSystemTrayIcon::ActivationReason reason);

    void MainInterface();

    void ExitProgram();

private:
    Ui::Widget *ui;
    MonitorSaftware mthread;
    QSystemTrayIcon mSysTrayIcon;
    QMenu *menu;
    QAction *MainFace;
    QAction *Exit;

    void MiniTray();
    void CreateMenu();
};

#endif // WIDGET_H
