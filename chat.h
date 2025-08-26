#ifndef CHAT_H
#define CHAT_H
#include "usersql.h"
#include "global.h"
#include"chat_client.h"
namespace Ui {
class chat;
}

class chat : public QWidget
{
    Q_OBJECT

public:
    explicit chat(const QString &myAccount, const QString &friendAccount, usersql *db,QWidget *parent = nullptr);
    ~chat();
    void showFriendInfo(const QString &name, const QString &account, const QPixmap &icon);
    void loadFriendInfo();
private slots:
    void on_sendBtn_clicked();

private:
    QString myAcc;
    QString friendAcc;

    usersql *m_db;
    QMetaObject::Connection m_connection;
    Ui::chat *ui;
};

#endif // CHAT_H
