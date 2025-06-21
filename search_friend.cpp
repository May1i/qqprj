#include "search_friend.h"
#include "ui_search_friend.h"

search_friend::search_friend(usersql *db,QWidget *parent) :
    QWidget(parent),m_db(db),
    ui(new Ui::search_friend)
{
    ui->setupUi(this);

}

search_friend::~search_friend()
{
    delete ui;
}

void search_friend::on_searchBtn_clicked()
{
    QString str=ui->searchLine->toPlainText().toUtf8();
    qDebug()<<"1";
}

