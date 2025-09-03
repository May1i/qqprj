#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTcpSocket>
#include <QScrollArea>
#include <QMenu>
#include "usersql.h"
#include "search_friend.h"
#include <QListWidgetItem>
#include"chat.h"
#include <QMap>

#include"chat_client.h"
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
    QMap<QString, chat*> m_chatWindows; // 账号到窗口的映射
public:
    MainWindow(QString account,usersql *db,QTcpSocket *m_socket=nullptr,QWidget *parent = nullptr);
    ~MainWindow();
    void showicon(QString account);
    void addFriendToList(const QString &username, const QString &account, const QPixmap &icon);
    void onFriendItemClicked(QListWidgetItem *item);
    //主界面显示用户信息
    void showUserInfo(const QString &username, const QString &account, const QPixmap &icon);
    void loadMainUserInfo();
private slots:
    void on_pushButton_menu_clicked();

private:
    usersql *m_db;
    QString account;
    Ui::MainWindow *ui;
    QTcpSocket *tcpClient;
};
#endif // MAINWINDOW_H
