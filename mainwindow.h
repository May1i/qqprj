#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTcpSocket>
#include <QScrollArea>
#include <QMenu>
#include "usersql.h"
#include "search_friend.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    //进行操作是否添加好友
    QMenu *opMenu;//操作下拉框

//    usersql db;
private:
    //系统托盘
    QSystemTrayIcon *systemtrayicon;
    search_friend *search_w;
public:
    MainWindow(QString account,QWidget *parent = nullptr,QTcpSocket *tcpClient=nullptr);
    ~MainWindow();
    void showicon(QString account);
private slots:
    void on_pushButton_menu_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
