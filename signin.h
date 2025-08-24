#ifndef SIGNIN_H
#define SIGNIN_H

#include <QWidget>
//进行鼠标对于signin widget的移动
#include<QPoint>
#include<QMouseEvent>

#include<QSystemTrayIcon>
#include<QMenu>

#include<QPushButton>
#include<QMovie>
//数据库操作
#include"usersql.h"

#include <QDir>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "mainwindow.h"
#include<QIODevice>
#include<QString>
#include <QListWidget>
#include<QStringList>
#include<QImageReader>
#include<QNetworkConfigurationManager>
namespace Ui {
class signin;
}

class signin : public QWidget
{
    Q_OBJECT
    //移动
    bool isPressedWidget;
    QPoint last;
    void mousePressEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;
    //系统托盘
    QSystemTrayIcon *systemtrayicon;
    //创建系统托盘的菜单显示
    QMenu *menu;
    QAction *m_showaction;
    QAction *m_closeaction;
    //对于动态图登入界面
    QSize m_si;//进行对于动态图的压缩大小
    QSize m_si2;
    QMovie *m_movie;
    //数据库
    usersql *db=new usersql();
    MainWindow *w;
    bool network;//确保是否正常连接上网络
    //自定义下拉框
    QStringList infoList;
    QListWidget *m_Account=NULL;
    QStringList icon;//用户头像保存地址
    QStringList infopasswd;//用户密码
    bool ispasswd;
    //连接网络
    QTcpSocket *tcpClient=NULL;
    //用户

public:
    explicit signin(QWidget *parent = nullptr);
    ~signin();
    void connectToServer();
    void onConnected();
    void onConnectionError();
private slots:
    void on_toolButton_del_clicked();

    void on_toolButton_min_clicked();

    void showwidget();

    void closewidget();

    void on_pushButton_clicked();
    
    void on_comboBox_currentIndexChanged(int index);

    void onSocketErrorChange(QAbstractSocket::SocketError);//确保连接网络
    QStringList getDirNameList(const QString &strDirpath);
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
private:
    Ui::signin *ui;
};

#endif // SIGNIN_H
