#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked(bool checked)
{
    int select;
    QString ip;
    quint16 port;

    select = ui->comboBox->currentIndex();
    ip = ui->lineEdit->text();
    port = ui->lineEdit_2->text().toUShort();

    if(checked == 0)
    {
        ui->comboBox->setEnabled(true);
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);

        if(select == 0)    //0-服务器端
        {
            ui->pushButton->setText("打开");
            ui->statusbar->showMessage("监听已关闭！");
            tcpServer->disconnect();
            tcpServer->close();
            tcpClient->destroyed();
            delete tcpServer;
            delete tcpClient;
        }
        else if(select == 1)
        {
            ui->pushButton->setText("连接");
            ui->statusbar->showMessage("已断开连接！");
            tcpClient->disconnectFromHost();
            tcpClient->close();
            delete tcpClient;
        }
    }
    else if(checked == 1)
    {
        if(select == 0)    //0-服务器端
        {
            tcpServer = new QTcpServer(this);
            tcpClient = new QTcpSocket(this);

            ui->pushButton->setText("关闭");
            ui->comboBox->setEnabled(false);
            ui->lineEdit->setEnabled(false);
            ui->lineEdit_2->setEnabled(false);
            ui->statusbar->showMessage("监听已开启！");
            tcpServer->listen(QHostAddress::Any,port);
            connect(tcpServer,&QTcpServer::newConnection,this,&MainWindow::newConnectSlot);
        }
        else if(select == 1)
        {
            tcpClient = new QTcpSocket(this);

            tcpClient->connectToHost(ip,port);
            if(tcpClient->waitForConnected(500))
            {
                ui->pushButton->setText("断开");
                ui->comboBox->setEnabled(false);
                ui->lineEdit->setEnabled(false);
                ui->lineEdit_2->setEnabled(false);
                ui->statusbar->showMessage("已连接！");
                connect(tcpClient,&QTcpSocket::readyRead,this,&MainWindow::ReadDataSlot);
            }
            else
            {
                ui->statusbar->showMessage("连接失败！");
                ui->pushButton->setChecked(0);
            }
        }
    }
}

void MainWindow::SendDataSlot()
{
    QString data;

    data = ui->textEdit->toPlainText();
    tcpClient->write(data.toLatin1());
}

void MainWindow::ReadDataSlot()
{
    int select;
    QString buffer,temp;
    QDateTime curtime;

    buffer = tcpClient->readAll();
    curtime = QDateTime::currentDateTime();
    select = ui->comboBox->currentIndex();

    if(select == 0)
    {
       temp = " From:"+clientAddr+QString::number(clientPort);
    }
    else
    {
        temp = ':';
    }

    ui->textBrowser->setTextColor(QColor(109,109,109));
    ui->textBrowser->append(curtime.toString("[yyyy-MM-dd hh:mm:ss.zzz]")+" #Receive"+temp);
    ui->textBrowser->setTextColor(QColor(0,0,255));
    ui->textBrowser->append(buffer);
}

void MainWindow::newConnectSlot()
{
    tcpClient = tcpServer->nextPendingConnection();
    clientAddr = tcpClient->peerAddress().toString().split("::ffff:")[1];
    clientPort = tcpClient->peerPort();
    connect(tcpClient,&QTcpSocket::readyRead,this,&MainWindow::ReadDataSlot);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->textBrowser->clear();
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->textEdit->clear();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->label->setText("本地主机地址");
        ui->label_2->setText("本地主机端口");
        ui->lineEdit->setText("192.168.1.131");
        ui->lineEdit_2->setText("61245");
        ui->pushButton->setText("打开");
    }
    else if(index == 1)
    {
        ui->label->setText("远程主机地址");
        ui->label_2->setText("远程主机端口");
        ui->lineEdit->setText("114.242.62.15");
        ui->lineEdit_2->setText("61245");
        ui->pushButton->setText("连接");
    }
    ui->statusbar->clearMessage();
}
