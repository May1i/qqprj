#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include "usersql.h"
namespace Ui {
class chat;
}

class chat : public QWidget
{
    Q_OBJECT

public:
    explicit chat(const QString &myAccount, const QString &friendAccount, usersql *db,QWidget *parent = nullptr);
    ~chat();

private:
    QString myAcc;
    QString friendAcc;
    usersql *m_db;
    Ui::chat *ui;
};

#endif // CHAT_H
