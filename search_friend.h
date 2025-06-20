#ifndef SEARCH_FRIEND_H
#define SEARCH_FRIEND_H

#include <QWidget>
#include <QDebug>
#include "usersql.h"
namespace Ui {
class search_friend;
}

class search_friend : public QWidget
{
    Q_OBJECT

public:
    explicit search_friend(usersql *db,QWidget *parent = nullptr);
    ~search_friend();

private slots:
    void on_searchBtn_clicked();
private:
    usersql *m_db;
    Ui::search_friend *ui;

};

#endif // SEARCH_FRIEND_H
