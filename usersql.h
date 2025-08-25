#ifndef USERSQL_H
#define USERSQL_H
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QPixmap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QSqlError>
#include<QSize>
#include<QGlobalStatic>
#include<QNetworkAccessManager>
#include <QEventLoop>//事件循环
#include<QCoreApplication>
#include <QLibrary>
#if _MSC_VER>=1600
#pragma execution_character_set("utf-8")
#endif //防止中文乱码

class usersql:public QObject
{
    Q_OBJECT
private:

     QSqlDatabase dbconn;//进行数据库的链接
     QSqlQuery query;//进行查询操作

     QString IP;
     QString account;//账号
     QString passwd;//密码
     QString iconUrl;//头像地址
     QPixmap icon;//头像
     QString username;//名称
     QString build_time;//注册时间

     QString cmd=QString("select *from `user`");//如果没有用全部的名称,需要用反引号``来包围table
     //搜索功能
     QString searchUsername;
     QPixmap searchIcon;
     QString searchAcc;
public:
    explicit usersql(QObject *parent = nullptr);  // 建议传递parent
     ~usersql();
    bool conndata();//连接到数据库
    bool queryuser(const QString &user,const QString &passwd);//查询是否有该用户
    QPixmap get_iconurl(const QString &url);//下载图片
    QPixmap getPixmapIcon(const QString &acc);//通过账号获取地址
    void queryUserInfo(const QString &account);//获取信息
    bool addFriendDirectly(const QString &userId,const QString &friendId);//直接进行好友添加

    //get set
    QString getAccount()const;
    void setAccount(const QString &value);
    QPixmap getIcon()const;
    void setIcon(const QPixmap &value);
    QString getUsername()const;
    void setUsername(const QString &value);
    QString getPasswd()const;
    void setPasswd(const QString &value);
    QString getBuild_time()const;
    QString getIP()const;
    void setIP(const QString &value); 
    void searchUserInfo(const QString &account);//进行用户搜索
    void showFriends(const QString &account);//联系人显示
signals:
    void userFound(const QString &username, const QString &account, const QPixmap &icon);
    //搜索功能
    void friendFound(const QString &username, const QString &account, const QPixmap &icon);

    void mainUserInfo(const QString &username, const QString &account, const QPixmap &icon);
    void friendInfo(const QString &username, const QString &account, const QPixmap &icon);
};

#endif // USERSQL_H
