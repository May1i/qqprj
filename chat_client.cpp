#include"chat_client.h"

Client::Client(QObject *parent)
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

bool Client::isConnected() const
{
    return m_connected;
}

void Client::sendToServer(const QString &message)
{

}
//进行数据接收
void Client::onReadyRead()
{
    while(m_socket->bytesAvailable()>0)
    {
        QByteArray data=m_socket->readAll();
        data=data.replace('\x00',"");
        //解码GBK中文
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QString message = codec->toUnicode(data);
        qDebug()<<"接收:"<<message;
    }
}

