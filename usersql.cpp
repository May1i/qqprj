#include "usersql.h"
usersql::usersql()
{
//    if (!conndata()) {
//           qDebug() << "程序启动时数据库连接失败！";
//            // 可以在这里添加错误处理（如显示警告框）
//    }
}
//连接数据库
bool usersql::conndata()
{
    if(QSqlDatabase::contains(QSqlDatabase::defaultConnection))//为如果存在默认的连接
    {
        this->dbconn=QSqlDatabase::database(QSqlDatabase::defaultConnection);//获取该对象,避免重复创建连接
    }
    else
    {
        this->dbconn=QSqlDatabase::addDatabase("QMYSQL");//若没有就创建一个QMysql的数据库
    }
    this->dbconn.setHostName("127.0.0.1");//主机名称
    this->dbconn.setDatabaseName("asuka_sql");
    if(this->dbconn.open("root", "123456"))//数据库名称和密码
    {
        //连接成功
        qDebug()<<"root连接成功";
        this->query=QSqlQuery(this->dbconn);//之后使用query进行操作
        return true;
    }
    else
    {
        qDebug()<<"连接失败"<<dbconn.lastError().text();
    }

    return false;

}
//数据库查询
bool usersql::queryuser(const QString &user,const QString &passwd)
{

    this->query.exec(cmd);

    if(query.size()==-1)
    {
        qDebug()<<"db_cursor.size()==-1出错"<<query.lastError().text();
    }

    else if(query.size()==0)
    {
        qDebug()<<"该数据库为空";
    }

    else
    {
        while(query.next())
        {
            if(this->query.value(2)==user)
            {
                if(this->query.value(3)==passwd)
                {
                    this->iconUrl = this->query.value(5).toString();//获取数据库中的icon url
                    qDebug()<<user<<"该账号存在";
                    return true;
                }
                else
                {
                    qDebug()<<user<<"该账号不存在";
                    return false;
                }
            }
//            qDebug()<<this->query.value(0).toString();//这是id
//            qDebug()<<this->query.value(2).toString();//这是账号
//            qDebug()<<this->query.value(3).toString();//这是密码
        }
    }
    return false;
}
//获取iconurl 进行下载
QPixmap usersql::get_iconurl(const QString &url1)
{
    QNetworkAccessManager manager;
    QEventLoop loop;
    this->iconUrl=url1;
    QUrl url(this->iconUrl);//转为url格式?
    QNetworkReply *reply=manager.get(QNetworkRequest(url));
    QObject::connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);//完成回应才退出循环事件
    //同步信号槽,后面要改成异步信号槽
    QLibrary libCrypto("libcrypto-1_1");
    QLibrary libSSL("libssl-1_1");
    loop.exec();
    QByteArray iconData=reply->readAll();
    this->icon.loadFromData(iconData);
    return icon;

}

QPixmap usersql::getPixmapIcon(const QString &url)
{
    QString strurl;
    this->query.exec(cmd);
    while(query.next())
    {
        if(url==(this->query.value(2).toString()))
        {
            strurl=this->query.value(5).toString();
        }

    }
    QPixmap pixmap=get_iconurl(strurl);
    qDebug()<<pixmap;
    pixmap=this->icon.scaled(QSize(pixmap.width(),pixmap.height()),Qt::IgnoreAspectRatio);//进行规模缩放
//    pixmap=PixmapToRound(pixmap, pixmap.width()/2); 反图片锯齿化
    return pixmap;
}
//根据账号号码获取信息
void usersql::queryUserInfo(const QString &account)
{
    this->query.exec(cmd);
    while(query.next())
    {
        if(this->query.value(2)==account)
        {
            this->username=this->query.value(1).toString();//名称
            this->icon=get_iconurl(this->query.value(5).toString());//iconurl
            this->account=this->query.value(2).toString();//账号
            this->passwd=this->query.value(3).toString();//密码
            this->build_time=this->query.value(4).toString();//创建时间
        }
    }
}

//进行数据库查询,用于添加用户
void usersql::searchUserInfo(const QString &account)
{
    this->query.exec(cmd);
    while (query.next())
    {
        if(this->query.value(2)==account)
        {
            this->searchUsername=this->query.value(1).toString();
            this->searchIcon=get_iconurl(this->query.value(5).toString());//iconurl
            qDebug()<<this->searchUsername;
            qDebug()<<this->searchIcon;
        }
    }
}
//get set

QPixmap usersql::getIcon() const
{
    return icon;
}
void usersql::setIcon(const QPixmap &value)
{
    icon=value;
}
QString usersql::getAccount() const
{
    return account;
}
void usersql::setAccount(const QString &value)
{
    account=value;
}
QString usersql::getPasswd() const
{
    return passwd;
}
void usersql::setPasswd(const QString &value)
{
    passwd=value;
}
QString usersql::getUsername() const
{
    return username;
}
void usersql::setUsername(const QString &value)
{
    username=value;
}
QString usersql::getIP()const
{
    return IP;
}
void usersql::setIP(const QString &value)
{
        IP=value;
}

