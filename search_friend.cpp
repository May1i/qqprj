#include "search_friend.h"
#include "ui_search_friend.h"

search_friend::search_friend(usersql *db,QWidget *parent) :
    QWidget(parent),m_db(db),
    ui(new Ui::search_friend)
{
    ui->setupUi(this);
    connect(m_db, &usersql::userFound, this, &search_friend::addUserToList);
}

search_friend::~search_friend()
{
    delete ui;
}

void search_friend::on_searchBtn_clicked()
{
    ui->listWidget->clear();
    QString str=ui->searchLine->toPlainText().toUtf8();
    m_db->searchUserInfo(str);
}

void search_friend::addUserToList(const QString &username, const QString &account,const QPixmap &icon)
{
    //防止进行重复添加同一个用户
//    bool userExists = false;
//        for (int i = 0; i < ui->listWidget->count(); ++i)
//        {
//            QListWidgetItem *item = ui->listWidget->item(i);
//            QWidget *widget = ui->listWidget->itemWidget(item);
//            if (widget)
//            {
//                // 获取用户名和账号（假设你的 QLabel 格式是 "用户名\n账号"）
//                QLabel *textLabel = widget->findChild<QLabel*>();
//                if (textLabel)
//                {
//                    QStringList parts = textLabel->text().split("\n");
//                    if (parts.size() >= 2 && parts[1] == account)
//                    { // 检查账号是否相同
//                        userExists = true;
//                        break;
//                    }
//                }
//            }
//        }
//        if (userExists)
//        {
//            qDebug() << "用户已存在，不重复添加：" << username << account;
//            return;
//        }


    // 创建列表项
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    // 创建自定义Widget（包含头像和文字）
    QWidget *itemWidget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(itemWidget);
    // 添加头像
    QLabel *iconLabel = new QLabel();
    //圆形头像实现
    QPixmap circularPixmap(40, 40);
    circularPixmap.fill(Qt::transparent);
    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addEllipse(0, 0, 40, 40);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, icon.scaled(40, 40, Qt::KeepAspectRatio));

    iconLabel->setPixmap(circularPixmap);
    // 添加文字（用户名 + 账号）
    QLabel *textLabel = new QLabel();
    textLabel->setText(QString("%1\n%2").arg(username).arg(account));
    // 组合布局
    layout->addWidget(iconLabel);
    layout->addWidget(textLabel);
    layout->setContentsMargins(5, 5, 5, 5);
    // 设置列表项
    item->setSizeHint(itemWidget->sizeHint());
    ui->listWidget->setItemWidget(item, itemWidget);
}
