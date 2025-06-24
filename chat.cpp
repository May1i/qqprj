#include "chat.h"
#include "ui_chat.h"

chat::chat(const QString &myAccount, const QString &friendAccount, usersql *db,QWidget *parent) :
    QWidget(parent),myAcc(myAccount),friendAcc(friendAccount),m_db(db),
ui(new Ui::chat)
{
    ui->setupUi(this);
    setWindowTitle("与 " + friendAcc + " 的聊天");
}

chat::~chat()
{
    delete ui;
}
