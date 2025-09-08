#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H
// 聊天程序客户端

//#include"pch.h"//预编译头
//#include<iostream>
#include<Winsock2.h>//socket头文件
#include <WS2tcpip.h>
#include<Singleton.h>
#include"global.h"
#include<QTcpSocket>
#include"usersql.h"

class Client:public QObject,public Singleton<Client>
{
    Q_OBJECT
    friend class Singleton<Client>;  // 允许单例模板访问私有构造函数
public:
    explicit Client(std::shared_ptr<usersql>db,QObject *parent=nullptr);
    Client(){}
    ~Client();
    void initialize(usersql *db); // 添加初始化方法
    bool clientConnect();
    void disconnect();
    QTcpSocket* getSocket()const ;
    bool isConnected()const; //判断连接
    void sendToServer(const QString &account,const QString &message);
    void makeJsonSend();
public slots:
    void onReadyRead();
signals:
    void connectEstablish();
    void connectFailed(const QString &error);
    void messageReceived(const QByteArray &msg);
    void connnectLost();
private:
    QTcpSocket *m_socket;
    bool m_connected;
    static const int BUFFER_SIZE=1024;
    static std::shared_ptr<Client> _instance;
//    usersql *m_db;
    std::shared_ptr<usersql> m_db; // 使用智能指针

};

#endif // CHAT_Client_H
