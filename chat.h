#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include "usersql.h"
#include<QPainter>
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
private:
    QString myAcc;
    QString friendAcc;
    usersql *m_db;
    Ui::chat *ui;
};

#endif // CHAT_H
