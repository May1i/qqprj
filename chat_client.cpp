#include"chat_client.h"

// 初始化静态成员变量
std::shared_ptr<Client> Client::_instance = nullptr;
Client::Client(std::shared_ptr<usersql> db,QObject *parent):m_db(db)
{

}
Client::~Client()
{

}
bool Client::clientConnect()
{
    m_socket=new QTcpSocket(this);
    int timeoutMs=5000;
    const QHostAddress addr = QHostAddress("127.0.0.1");
    const quint16 port = 12345;
    // 连接信号槽
    QObject::connect(m_socket, &QTcpSocket::connected, this, [&]()
    {
        qDebug()<<"成功连接到服务端";
        m_connected=true;
        emit connectEstablish();
    });
    QObject::connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, [&]()
    {
        qDebug()<<"连接到服务端失败";
        m_connected=false;
        emit connectFailed("error ");
    });
    QObject::connect(m_socket,&QTcpSocket::readyRead,this,&Client::onReadyRead);
    QObject::connect(m_socket,&QTcpSocket::disconnected,this,[&]()
    {
        qDebug()<<"与服务端断开连接,服务端主动关闭";
    });

    m_socket->connectToHost(addr,port);
    // 设置连接超时定时器
    QTimer::singleShot(timeoutMs, this, [this]()
    {
        if (m_socket->state() == QAbstractSocket::ConnectingState)
        {
            qDebug() << "Connection timeout";
            m_socket->abort();
            emit connectFailed("time out");
        }
    });
    return true;
}

QTcpSocket *Client::getSocket()const
{
    return m_socket;
}

bool Client::isConnected() const
{
    return m_connected;
}
//void Client::sendToServer(const QString &account, const QString &message)
//{
//    qDebug()<<m_db;
//}

void Client::sendToServer(const QString &account, const QString &message)
{
    // 使用智能指针来管理连接
    auto connPtr = std::make_shared<QMetaObject::Connection>();

    *connPtr = QObject::connect(m_db.get(), &usersql::friendNetInfo, this,
        [this, message, connPtr](const QString &Ip, const QString &Port)
        {
            QString friendNetInfo = Ip + "/" + Port;
            qDebug() << "查询到好友网络信息:" << friendNetInfo;

            if (m_socket && m_socket->state() == QAbstractSocket::ConnectedState)
            {
                QJsonObject json;
                json["content"] = message;
                json["timestamp"] = QDateTime::currentDateTime().toString();
                json["friendNetInfo"] = friendNetInfo;
                json["type"] = "chat_message";
                QJsonDocument doc(json);
                m_socket->write(doc.toJson());
                qDebug() << "已发送消息到服务端:" << message;
            } else {
                qDebug() << "未连接到服务端，无法发送消息";
        }

            // 断开连接
            QObject::disconnect(*connPtr);
        });

    m_db->searchFriendInfo(account);
}
//进行数据接收
void Client::onReadyRead()
{
    while(m_socket->bytesAvailable()>0)
    {
        QByteArray data=m_socket->readAll();
        data=data.replace('\x00',"");
        //解码GBK中文
        QString message =QString::fromUtf8(data);
        qDebug()<<"接收:"<<message;
    }
}
void Client::initialize(usersql *db)
{
    if (!db) {
        qDebug() << "错误：传入的db为nullptr";
        return;
    }

    // 将普通指针转换为智能指针
    m_db = std::shared_ptr<usersql>(db, [](usersql*){}); // 使用空删除器
    // 或者如果不需要管理生命周期：
    // m_db = std::shared_ptr<usersql>(db, [](usersql*){});

    qDebug() << "Client db已设置";
}
