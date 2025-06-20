#include "search_friend.h"
#include "ui_search_friend.h"

search_friend::search_friend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::search_friend)
{
    ui->setupUi(this);
}

search_friend::~search_friend()
{
    delete ui;
}
