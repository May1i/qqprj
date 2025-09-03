#pragma once
#include <QDebug>
#include "usersql.h"
#include "signal.h"
#include <QHBoxLayout>
#include <QLabel>
#include<QPainter>

#include<QObject>
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

#include <QMap>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTcpSocket>
#include <QScrollArea>
#include <QMenu>

#include <QWidget>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include<QPainter>

#include<QThread>
#include<QTimer>
#include<QHostAddress>
#include<QTextCodec>

//操作Json文件所需要用到的头文件：
#include <QJsonObject> // { }
#include <QJsonArray> // [ ]
#include <QJsonDocument> // 解析Json
#include <QJsonValue> // int float double bool null { } [ ]
#include <QJsonParseError>

struct ServerInfo{
    QString Host;
    QString Port;
    QString Token;
    int Uid;
};
enum ReqId{
    ID_GET_VARIFY_CODE = 1001, //获取验证码
    ID_REG_USER = 1002, //注册用户
    ID_RESET_PWD = 1003, //重置密码
    ID_LOGIN_USER = 1004, //用户登录
    ID_CHAT_LOGIN = 1005, //登陆聊天服务器
    ID_CHAT_LOGIN_RSP= 1006, //登陆聊天服务器回包
    ID_SEARCH_USER_REQ = 1007, //用户搜索请求
    ID_SEARCH_USER_RSP = 1008, //搜索用户回包
    ID_ADD_FRIEND_REQ = 1009,  //添加好友申请
    ID_ADD_FRIEND_RSP = 1010, //申请添加好友回复
    ID_NOTIFY_ADD_FRIEND_REQ = 1011,  //通知用户添加好友申请
    ID_AUTH_FRIEND_REQ = 1013,  //认证好友请求
    ID_AUTH_FRIEND_RSP = 1014,  //认证好友回复
    ID_NOTIFY_AUTH_FRIEND_REQ = 1015, //通知用户认证好友申请
    ID_TEXT_CHAT_MSG_REQ  = 1017,  //文本聊天信息请求
    ID_TEXT_CHAT_MSG_RSP  = 1018,  //文本聊天信息回复
    ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019, //通知用户文本聊天信息
    ID_NOTIFY_OFF_LINE_REQ = 1021, //通知用户下线
    ID_HEART_BEAT_REQ = 1023,      //心跳请求
    ID_HEARTBEAT_RSP = 1024,       //心跳回复
};
