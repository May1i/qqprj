#include "chat.h"
#include "ui_chat.h"

chat::chat(const QString &myAccount, const QString &friendAccount, usersql *db,QWidget *parent) :
    QWidget(parent),myAcc(myAccount),friendAcc(friendAccount),m_db(db),
ui(new Ui::chat)
{
    ui->setupUi(this);
    setWindowTitle("与 " + friendAcc + " 的聊天");
    connect(m_db, &usersql::userFound,[this](const QString &name, const QString &account, const QPixmap &icon)
    {
        showFriendInfo(name,account,icon);
    });
    m_db->searchUserInfo(friendAcc);
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
void chat::loadFriendInfo()
{
    auto conn = std::make_shared<QMetaObject::Connection>();
       *conn = connect(m_db, &usersql::userFound, this, [this, conn](QString  name, QString  acc,QPixmap icon) {
           emit m_db->friendInfo(name, acc, icon);
           disconnect(*conn);  // 只断开当前连接
       });
       m_db->searchUserInfo(friendAcc);
}
