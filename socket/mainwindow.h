#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked(bool checked);

    void SendDataSlot();

    void ReadDataSlot();

    void newConnectSlot();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_SendDataSlot_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpClient;
    QTcpServer *tcpServer;
    QString clientAddr;
    quint16 clientPort;

};
#endif // MAINWINDOW_H
