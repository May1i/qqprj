#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QString account,usersql *db,QTcpSocket *m_socket,QWidget *parent)
    : QMainWindow(parent),m_db(db),account(account), tcpClient(m_socket),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->scrollArea->setFrameShape(QFrame::NoFrame);
    connect(m_db, &usersql::mainUserInfo,[this](const QString &username, const QString &account, const QPixmap &icon)
    {
        showUserInfo(username,account,icon);
    });
    MainWindow::loadMainUserInfo();
//    showUserInfo(username,account,icon);
    m_db->searchUserInfo(account);
    search_w=new search_friend(account,db,nullptr);
    search_w->hide();
//    QListWidgetItem *newItem=new QListWidgetItem(ui->listWidget);
//    ui->listWidget->addItem(newItem);
    //进行联系人显示

    connect(m_db, &usersql::friendFound, this, &MainWindow::addFriendToList);
    m_db->showFriends(account);
    //进行打开聊天窗口//修改多次打开
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
    //进行指定对方的ip和端口号
    // 获取存储的账号
    QString friendAcc = item->data(Qt::UserRole).toString();
    QString friendNetInfo;
    QString Ip,Port;
    QMetaObject::Connection conn = QObject::connect(m_db, &usersql::friendNetInfo, this, [&friendNetInfo](QString Ip, QString Port)
    {
        friendNetInfo=Ip+"/"+Port;
    });
    m_db->searchFriendInfo(friendAcc);
    if(friendNetInfo!="")
    {
        qDebug()<<friendNetInfo;
    }
    if(tcpClient==nullptr)
    {
        qDebug()<<"为空指针";
    }
    if (tcpClient &&tcpClient->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data = friendNetInfo.toUtf8(); // 转换为UTF-8编码
        tcpClient->write(data);
        tcpClient->flush(); // 确保立即发送
        qDebug() << "已发送消息到服务端:" << friendNetInfo;
    }
    else
    {
        qDebug() << "未连接到服务端，无法发送消息";
    }

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
void MainWindow::loadMainUserInfo()
{
    auto conn = std::make_shared<QMetaObject::Connection>();
       *conn = connect(m_db, &usersql::userFound, this, [this, conn](QString  name, QString  acc,QPixmap icon) {
           emit m_db->mainUserInfo(name, acc, icon);
           disconnect(*conn);  // 只断开当前连接
       });
       m_db->searchUserInfo(account);
}
