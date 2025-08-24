 #include "TcpMgr.h"
#include <QTcpSocket>
#include "singleton.h"
#include "global.h"

TcpMgr::TcpMgr():_host(" "),_port(0),_message_id(0),_message_len(0),_b_recv_pending(false)
{
    //下面是对于粘包状态的处理方式,通过将message-len的长度放在头部
    QObject::connect(&_socket,&QTcpSocket::connected,[&](){
        qDebug()<<"success connected!";
        emit(sig_con_success(true));
    });

    QObject::connect(&_socket,&QTcpSocket::readyRead,[&]()
    {
        _buffer.append(_socket.readAll());
        QDataStream stream(&_buffer,QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_5_14);
        forever
        {
            if(!_b_recv_pending)
            {
                if(_buffer.size()<static_cast<int>(sizeof (quint16)*2))
                {
                    return;
                }
                stream>>_message_id>>_message_len;
                _buffer=_buffer.mid(sizeof(quint16)*2);
                qDebug()<<_message_id<<" "<<_message_len;

            }
            if(_buffer.size()<_message_len)
            {
                _b_recv_pending=true;
                return;
            }
            _b_recv_pending=false;
            QByteArray messageBody=_buffer.mid(0,_message_len);
            qDebug()<<"recv message is:"<<messageBody;
            _buffer=_buffer.mid(_message_len);

            handleMsg(ReqId(_message_id),_message_len,messageBody);//传入消息
        }

    });
    // 处理错误（适用于Qt 5.15之前的版本）
    QObject::connect(&_socket, static_cast<void (QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error),
                        [&](QTcpSocket::SocketError socketError) {
           qDebug() << "Error:" << _socket.errorString() ;
           switch (socketError) {
               case QTcpSocket::ConnectionRefusedError:
                   qDebug() << "Connection Refused!";
                   emit sig_con_success(false);
                   break;
               case QTcpSocket::RemoteHostClosedError:
                   qDebug() << "Remote Host Closed Connection!";
                   break;
               case QTcpSocket::HostNotFoundError:
                   qDebug() << "Host Not Found!";
                   emit sig_con_success(false);
                   break;
               case QTcpSocket::SocketTimeoutError:
                   qDebug() << "Connection Timeout!";
                   emit sig_con_success(false);
                   break;
               case QTcpSocket::NetworkError:
                   qDebug() << "Network Error!";
                   break;
               default:
                   qDebug() << "Other Error!";
                   break;
           }
     });
     QObject::connect(&_socket,&QTcpSocket::disconnected,[&]()
     {
         qDebug()<<"is disconnect!";
     });
     //连接发送信号用来发送数据
     QObject::connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);
     //注册消息
//     initHandlers();
}

void TcpMgr::slot_tcp_connect(ServerInfo)
{

}
void TcpMgr::slot_send_data(ReqId reqId, QString data)
{

}
void TcpMgr::handleMsg(ReqId id, int len, QByteArray data)
{
    qDebug()<<1;
}


