#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H
// 聊天程序客户端

//#include"pch.h"//预编译头
//#include<iostream>
#include<Winsock2.h>//socket头文件
#include <WS2tcpip.h>
#include<Singleton.h >
#include"global.h"
#include<QTcpSocket>
class Client:public QObject,public Singleton<Client>
{
    Q_OBJECT
    friend class Singleton<Client>;  // 允许单例模板访问私有构造函数
public:
    explicit Client(QObject *parent=nullptr);
    ~Client();
    bool clientConnect();
    void disconnect();
    bool isConnected()const; //判断连接
    void recvMsgThread(LPVOID lpParameter);

signals:
    void connectEstablish();
    void connectFailed(const QString &error);
    void messageReceived(const QByteArray &msg);
    void connnectLost();
private:
    QTcpSocket *m_socket;
    bool m_connected;
    static const int BUFFER_SIZE=1024;
};

#endif // CHAT_Client_H
