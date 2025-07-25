#ifndef SEARCH_FRIEND_H
#define SEARCH_FRIEND_H

#include <QWidget>
#include <QDebug>
#include "usersql.h"
#include "signal.h"
#include <QHBoxLayout>
#include <QLabel>
#include<QPainter>
namespace Ui {
class search_friend;
}

class search_friend : public QWidget
{
    Q_OBJECT

public:
    explicit search_friend(QString userId,usersql *db,QWidget *parent = nullptr);
    ~search_friend();

private slots:
    void on_searchBtn_clicked();
    void addUserToList(const QString &username, const QString &account, const QPixmap &icon);
private:
    usersql *m_db;
    QString userId;

    Ui::search_friend *ui;

};

#endif // SEARCH_FRIEND_H
