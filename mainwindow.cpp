#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QString account,usersql *db,QWidget *parent,QTcpSocket *tcpClient)
    : QMainWindow(parent),m_db(db),account(account), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->scrollArea->setFrameShape(QFrame::NoFrame);
    search_w=new search_friend(account,db,nullptr);
    search_w->hide();
//    QListWidgetItem *newItem=new QListWidgetItem(ui->listWidget);
//    ui->listWidget->addItem(newItem);
    //进行联系人显示

    connect(m_db, &usersql::friendFound, this, &MainWindow::addFriendToList);
    m_db->showFriends(account);
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
    item->setSizeHint(itemWidget->sizeHint());
//    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, itemWidget);
//    if (ui->listWidget->count()==0) {
//        qDebug() << "列表为空";
//    }
}
