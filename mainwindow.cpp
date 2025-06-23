#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QString account,QWidget *parent,QTcpSocket *tcpClient)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->scrollArea->setFrameShape(QFrame::NoFrame);
    usersql *db=new usersql();
    db->conndata();
    search_w=new search_friend(account,db,nullptr);
    search_w->hide();
    QListWidgetItem *newItem=new QListWidgetItem(ui->listWidget);
    newItem->setText("111");
    ui->listWidget->addItem(newItem);
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
