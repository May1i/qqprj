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
    connect(m_socket, &QTcpSocket::connected, this, [&]()
    {
        qDebug()<<"成功连接到服务端";
        m_connected=true;
        emit connectEstablish();
    });
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, [&]()
    {
        qDebug()<<"连接到服务端失败";
        m_connected=false;
        emit connectFailed("error ");
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

void Client::recvMsgThread(LPVOID lpParameter)
{
    QThread *recvThread=new QThread;
    m_socket->moveToThread(recvThread);
    while (1)
    {
        char buffer[BUFFER_SIZE] = { 0 };//字符缓冲区，用于接收和发送消息
        int nrecv = QTcpSocket::recv(m_socket, buffer, sizeof(buffer), 0);//nrecv是接收到的字节数
        if (nrecv > 0)//如果接收到的字符数大于0
        {
            cout << buffer << endl;
        }
        else if (nrecv < 0)//如果接收到的字符数小于0就说明断开连接
        {
            cout << "与服务器断开连接" << endl;
            break;s
        }
    }
}


