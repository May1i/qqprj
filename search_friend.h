#ifndef SEARCH_FRIEND_H
#define SEARCH_FRIEND_H

#include <QWidget>

namespace Ui {
class search_friend;
}

class search_friend : public QWidget
{
    Q_OBJECT

public:
    explicit search_friend(QWidget *parent = nullptr);
    ~search_friend();

private:
    Ui::search_friend *ui;
};

#endif // SEARCH_FRIEND_H
