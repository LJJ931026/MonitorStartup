#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
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

private:
    Ui::Widget *ui;
    MonitorSaftware mthread;
};

#endif // WIDGET_H
