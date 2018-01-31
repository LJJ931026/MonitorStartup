#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);


    ui->pushButton_monitor->setFocus();
    ui->pushButton_monitor->setDefault(true);

    connect(&mthread, SIGNAL(sendMsg(QString)), this, SLOT(ReceiveMsg(QString)));
    connect(this, SIGNAL(sendMsg(QString)), &mthread, SLOT(ReceiveMsg(QString)));
    connect(&mSysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), // 最小化托盘
            this, SLOT(ActivateSystemTray(QSystemTrayIcon::ActivationReason)));

    CreateMenu(); // 创建菜单

}

Widget::~Widget()
{
    delete ui;
}

// 打开文件
void Widget::on_pushButton_open_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), NULL, tr("txtFile (*.* *.exe)"));
    if(!fileName.isEmpty()) {
        ui->lineEdit->setText(fileName);
        ui->textEdit_info->append("选择了路径...");
    }
}

// 监控
void Widget::on_pushButton_monitor_clicked()
{
    QString path = ui->lineEdit->text();
    if(path == "") {
        QMessageBox::information(NULL, QString("提示"), QString("路径不能为空，请选择路径！"));
        return;
    }
    if(!path.contains(".exe")) {
        QMessageBox::information(NULL, QString("提示"), QString("路径错误,请检查！"));
        return;
    }
    if(ui->pushButton_monitor->text() == "开始监控") {
        mthread.start();
        emit sendMsg(path);
        emit sendMsg("monitor");
        ui->pushButton_monitor->setText("停止监控");
        ui->lineEdit->setEnabled(false);
        ui->pushButton_open->setEnabled(false);
    }
    else {
        emit sendMsg("exit");
        ui->pushButton_monitor->setText("开始监控");
        ui->lineEdit->setEnabled(true);
        ui->pushButton_open->setEnabled(true);
    }
}

// 程序信息编辑结束
void Widget::on_lineEdit_editingFinished()
{
    QString path = ui->lineEdit->text();
    if(path == "") {
        ui->textEdit_info->append("路径不能为空，请选择路径！");
        return;
    }
    if(!path.contains(".exe")) {
        ui->textEdit_info->append("路径错误,请检查！");
        return;
    }
}

void Widget::ReceiveMsg(QString msg)
{
//    QTextDocument *doc = ui->textEdit_info->document();
//    if(doc->lineCount() > 9999)
//        ui->textEdit_info->clear();
    if(ui->textEdit_info->document()->lineCount() > 999)
        ui->textEdit_info->clear();
    ui->textEdit_info->append(msg);
}

void Widget::closeEvent(QCloseEvent *e)
{
    if(QMessageBox::information(NULL, QString("提示"), QString("是否最小化托盘?\n\n选择是最小化托盘，选择否退出"),
                    QMessageBox::Ok | QMessageBox::No) == QMessageBox::Ok) {
        e->ignore();
        MiniTray();
    }
    else {
        emit sendMsg("exit");
        mthread.wait(5000);
        mthread.quit();

        e->accept();
    }
}

void Widget::MiniTray()
{
    this->hide();

    QIcon icon = QIcon(":/icon/Monitor.png");
    mSysTrayIcon.setIcon(icon); //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon.setToolTip("检测软件启动"); //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon.show(); //在系统托盘显示此对象
    mSysTrayIcon.showMessage("提示", "单击或双击显示界面");
}

void Widget::ActivateSystemTray(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    {
        mSysTrayIcon.hide();
        this->show();
    }
        break;
    case QSystemTrayIcon::DoubleClick:
    {
        mSysTrayIcon.hide();
        this->show();
    }
        break;
    default:
        break;
    }
}

void Widget::CreateMenu()
{
    MainFace = new QAction("主界面", this);
    connect(MainFace, SIGNAL(triggered()), this, SLOT(MainInterface()));

    Exit = new QAction("退出", this);
    connect(Exit, SIGNAL(triggered()), this, SLOT(ExitProgram()));

    menu = new QMenu(this);
    menu->addAction(MainFace);
    menu->addAction(Exit);
    mSysTrayIcon.setContextMenu(menu);
}

void Widget::MainInterface()
{
    this->show();
    mSysTrayIcon.hide();
}

void Widget::ExitProgram()
{
    this->close();
}




