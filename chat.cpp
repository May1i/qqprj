#include "chat.h"
#include "ui_chat.h"
#include"chat_client.h"
chat::chat(const QString &myAccount, const QString &friendAccount, usersql *db,QWidget *parent) :
    QWidget(parent),myAcc(myAccount),friendAcc(friendAccount),m_db(db),
ui(new Ui::chat)
{
    ui->setupUi(this);
    setWindowTitle("与 " + friendAcc + " 的聊天");
    connect(m_db, &usersql::friendInfo,[this](const QString &name, const QString &account, const QPixmap &icon)
    {
        showFriendInfo(name,account,icon);
    });
    chat::loadFriendInfo();

}

chat::~chat()
{
    delete ui;
}
void chat::showFriendInfo(const QString &name, const QString &account, const QPixmap &icon)
{
    // 显示头像
    ui->friendIcon->setPixmap(icon.scaled(80, 80, Qt::KeepAspectRatio));
    //显示账号名称
//    ui->userAccLabel->setText("账号:"+account);
    ui->friendName->setText(name);
//    ui->friendAcc
//    QString friendAcc=account;
    //圆形头像实现
    QPixmap circularPixmap(80, 80);
    circularPixmap.fill(Qt::transparent);
    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addEllipse(0, 0, 80, 80);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, icon.scaled(80, 80, Qt::KeepAspectRatio));

    ui->friendIcon->setPixmap(circularPixmap);
}
//聊天对象的info不进行改变的函数
void chat::loadFriendInfo() {
    // 使用成员变量保存连接
    m_connection = connect(m_db, &usersql::userFound, this,
        [this](const QString &name, const QString &acc, const QPixmap &icon) {
            if (acc == this->friendAcc) {  // 关键：只处理当前窗口的好友
                showFriendInfo(name, acc, icon);
                disconnect(m_connection);  // 断开当前连接
            }
        },
        Qt::UniqueConnection  // 防止重复连接
    );
    m_db->searchUserInfo(friendAcc);
}



void chat::on_sendBtn_clicked()
{
    QString sendEdit = ui->sendEdit->toPlainText().toUtf8();
    qDebug()<<sendEdit;
    QString friend_Acc=friendAcc;
    ui->sendEdit->clear();
    auto client = Client::GetInstance();
    client->sendToServer(friend_Acc,sendEdit);
}
