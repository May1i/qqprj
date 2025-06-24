#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QString account,usersql *db,QWidget *parent,QTcpSocket *tcpClient)
    : QMainWindow(parent),m_db(db),account(account), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->scrollArea->setFrameShape(QFrame::NoFrame);

    connect(m_db, &usersql::userFound,[this](const QString &username, const QString &account, const QPixmap &icon)
    {
        qDebug()<<"1";
        showUserInfo(username,account,icon);
    });
    m_db->searchUserInfo(account);
    search_w=new search_friend(account,db,nullptr);
    search_w->hide();
//    QListWidgetItem *newItem=new QListWidgetItem(ui->listWidget);
//    ui->listWidget->addItem(newItem);
    //进行联系人显示

    connect(m_db, &usersql::friendFound, this, &MainWindow::addFriendToList);
    m_db->showFriends(account);
    //进行打开聊天窗口
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::onFriendItemClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showicon(QString account)
{
    //系统托盘的实现
    systemtrayicon=new QSystemTrayIcon(this);
    QIcon icon=QIcon(":/lib/qq.jpg");
    systemtrayicon->setIcon(icon);//添加图标
    this->setWindowIcon(icon);
    QString str_account="球球:"+account;
    systemtrayicon->setToolTip(str_account);//进行文字显示
    systemtrayicon->show();//进行图标显示
}

void MainWindow::on_pushButton_menu_clicked()
{
    opMenu=new QMenu(this);
    QAction *action1=opMenu->addAction("加好友");
    connect(ui->pushButton_menu, &QPushButton::clicked, [this](){
        QPoint globalPos = ui->pushButton_menu->mapToGlobal(QPoint(0, 0));

        // 计算菜单显示位置（按钮左下角）
        int menuX = globalPos.x();
        int menuY = globalPos.y() + ui->pushButton_menu->height();

        // 显示菜单在固定位置
        opMenu->exec(QPoint(menuX, menuY));
    });
    connect(action1,&QAction::triggered,[this]()
    {
        search_w->show();
    });

}
//联系人显示
void MainWindow::addFriendToList(const QString &username, const QString &account, const QPixmap &icon)
{

    // 创建列表项
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    item->setData(Qt::UserRole, account);
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
    // 添加文字（用户名  账号）
    QLabel *textLabel = new QLabel();
    textLabel->setText(QString("%1").arg(username));
    // 组合布局
    layout->addWidget(iconLabel);
    layout->addWidget(textLabel);
    layout->setContentsMargins(0, 5, 5, 5);
    // 设置列表项
    // 关键步骤：必须按此顺序
    ui->listWidget->addItem(item);            // 1. 先添加项
    item->setSizeHint(itemWidget->sizeHint()); // 2. 设置大小
    ui->listWidget->setItemWidget(item, itemWidget); // 3. 再关联Widget

}
//打开聊天窗口
void MainWindow::onFriendItemClicked(QListWidgetItem *item) {
    // 获取存储的账号
    QString friendAcc = item->data(Qt::UserRole).toString();

    // 创建或激活聊天窗口
    if (!m_chatWindows.contains(friendAcc))
    {
        chat *chatWin=new chat(account,friendAcc,m_db,nullptr);
        m_chatWindows.insert(friendAcc,chatWin);
        connect(chatWin, &chat::destroyed, [this, friendAcc]()
        {
            m_chatWindows.remove(friendAcc); // 窗口关闭时自动移除
        });
        chatWin->show();
    }
    else
    {
        m_chatWindows[friendAcc]->raise();
        m_chatWindows[friendAcc]->activateWindow();
    }

}
//主界面显示用户信息
void MainWindow::showUserInfo(const QString &username, const QString &account, const QPixmap &icon)
{
    // 显示头像
    ui->userIconLabel->setPixmap(icon.scaled(80, 80, Qt::KeepAspectRatio));
    //显示账号名称
    ui->userAccLabel->setText("账号:"+account);
    ui->userNameLabel->setText(username);
    //圆形头像实现
    QPixmap circularPixmap(80, 80);
    circularPixmap.fill(Qt::transparent);
    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addEllipse(0, 0, 80, 80);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, icon.scaled(80, 80, Qt::KeepAspectRatio));

    ui->userIconLabel->setPixmap(circularPixmap);
}
