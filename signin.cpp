#include "signin.h"
#include "ui_signin.h"
#include "usersql.h"
signin::signin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::signin)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    ui->networkerror->hide();
    ui->notexist->hide();
    //系统托盘的实现
    systemtrayicon=new QSystemTrayIcon(this);
    QIcon icon=QIcon(":/lib/qq.jpg");
    systemtrayicon->setIcon(icon);//添加图标
    this->setWindowIcon(icon);
    systemtrayicon->setToolTip(QObject::tr("球球"));//进行文字显示
    systemtrayicon->show();//进行图标显示
    //不显示密码
    ui->lineEdit->setEchoMode(QLineEdit::Password);
    //托盘菜单显示
    menu=new QMenu(this);
    m_showaction=new QAction("打开主界面");
    m_closeaction=new QAction("关闭");
    menu->addAction(m_showaction);
    menu->addSeparator();
    menu->addAction(m_closeaction);
    systemtrayicon->setContextMenu(menu);
    connect(m_showaction,&QAction::triggered,this,&signin::showwidget);
    connect(m_closeaction,&QAction::triggered,this,&signin::closewidget);

    //实现登录界面动态图片播放
    m_movie = new QMovie(":/lib/qqsignin.gif");//所需的gif
    m_si.setWidth(400);
    m_si.setHeight(230);
    m_movie->setScaledSize(m_si);
    ui->main_label->setMovie(m_movie);
    m_movie->start();
    //密码文本框
    QAction *searchAction=new QAction(ui->lineEdit);
    searchAction->setIcon(QIcon(":/lib/keyb.png"));
    ui->lineEdit->addAction(searchAction,QLineEdit::TrailingPosition);
    QAction *searchAction_2=new QAction(ui->lineEdit);
    searchAction_2->setIcon(QIcon(":/lib/lock.png"));
    ui->lineEdit->addAction(searchAction_2,QLineEdit::LeadingPosition);
    //把combox的选择显示在账号上
//    connect(ui->comboBox,&QComboBox::textActivated,this,&signin::on_comboBox_currentIndexChanged);
    //号码文本框icon暂时不做修改
    //进行下拉框的处理
    QString fileName1=QCoreApplication::applicationDirPath()+"//..//Userfile";
    infoList=getDirNameList(fileName1);
    //加载自定义的下拉框
    m_Account=new QListWidget(this);
    m_Account->setFocusPolicy(Qt::NoFocus);
    ui->comboBox->setModel(m_Account->model());//共享数据模型
    ui->comboBox->setView(m_Account);//改变视图

    for(int i=0;i<infoList.size();i++)
    {
        QString local_account=infoList.at(i);//遍历
        QString local_name;
        QString local_passwd;//获取密码
        QString ic=fileName1+QString("//%1//%2.jpg").arg(infoList.at(i)).arg(infoList.at(i));
        this->icon.append(ic);//添加用户头像的地址

        QFile file_my(fileName1+QString("//%1//data.txt").arg(infoList.at(i)));
        if(!file_my.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            qDebug()<<"文件打开失败";
        }
        this->ispasswd=false;
        while(!file_my.atEnd())
        {
            infopasswd.append("");
            QByteArray line=file_my.readLine();
            QString str(QString::fromUtf8(line.data()));//将passwd数据转化为str
            if(this->ispasswd)
            {
                infopasswd.insert(i,str);
                local_passwd.append(str);
                this->ispasswd=false;
            }
            else
            {
                local_name.append(str);
                this->ispasswd=true;
            }

        }
        QHBoxLayout *horLayout=new QHBoxLayout();//水平布局
        QLabel *la=new QLabel();
        QString s=ic;
        la->setStyleSheet(QString("border-image: url(%1);border-radius:17px;").arg(s));
        la->setFixedSize(34,34);
        QLabel * la2 = new QLabel(QString("%1\n%2").arg(local_name.left(local_name.length()-1),local_account));
        QPushButton * b1 = new QPushButton();
        b1->setFixedSize(35,50);
        b1->setStyleSheet("QPushButton{background:rgba(200,200,200,0);border-style:solid;border-image: url(:/lib/delete.png);}"
        "QPushButton:hover{background:rgba(200,200,200,0);border-style:solid;border-image: url(:/lib/delete2.png);}");
        horLayout->addWidget(la);
        horLayout->addWidget(la2);
        horLayout->addWidget(b1);
        QWidget *widget =new QWidget(this);
        widget->setLayout(horLayout);
        QListWidgetItem * Listitem = new QListWidgetItem(m_Account);
        m_Account->setItemWidget(Listitem,widget);
    }

}
//析构
signin::~signin()
{
    delete ui;
}
//网络连接确认
void signin::onSocketErrorChange(QAbstractSocket::SocketError)
{
    network=false;
    qDebug()<<"网络连接失败";
}
//菜单控制
void signin::showwidget()
{
    this->show();
}
void signin::closewidget()
{
    this->close();
    QApplication::quit();
}
//主界面的显示和缩小
void signin::on_toolButton_del_clicked()//进行关闭窗口
{
    this->close();
    QApplication::quit();
}
void signin::on_toolButton_min_clicked()//进行最小化窗口到菜单栏
{
    this->hide();
}
//对于鼠标的拖动窗口的实现
void signin::mousePressEvent(QMouseEvent *event)
{
        isPressedWidget=true;
        last=event->globalPos();
}
void signin::mouseReleaseEvent(QMouseEvent *event)
{

    int dx=event->globalX()-last.x();
    int dy=event->globalY()-last.y();
    move(x()+dx,y()+dy);
    isPressedWidget=false;
}
//需要优化
void signin::mouseMoveEvent(QMouseEvent *event)
{
    if(isPressedWidget&&childAt(event->pos())==nullptr)
    {
        int dx=event->globalX()-last.x();
        int dy=event->globalY()-last.y();
        last=event->globalPos();
        move(x()+dx,y()+dy);
    }
}
//进行登录按钮的实现
void signin::on_pushButton_clicked()
{ 
    QNetworkConfigurationManager manager;
    qDebug()<<manager.isOnline();
    if(!manager.isOnline())//网络错误
    {
        ui->networkerror->show();
    }
    else
    {
        this->network = new QNetworkAccessManager(this);//进行网络初始化
        //line_edit 为密码框 line2为账号
        db->conndata();//进行数据库连接
        bool isuser=db->queryuser(ui->lineEdit_2->text(),ui->lineEdit->text());//确认是否为数据库内用户
        if(isuser)
        {
            QString account=ui->lineEdit_2->text();
            db->queryUserInfo(account);
            QString name=db->getUsername();
            QString passwd="";
            if(ui->checkBox_2->isChecked())
            {
                passwd=db->getPasswd();
            }

            QPixmap icon=db->getIcon();

            QString fileName=QCoreApplication::applicationDirPath();//获取当前的程序目录
            QString add="//..//Userfile";
            fileName=fileName+add+QString("//%1").arg(account);
            QDir *file=new QDir;
            bool isexist=file->exists(fileName);//判断是否存在这个路径
            if(isexist)//已经存在这个路径,直接进行创建txt文件
            {
//                qDebug()<<"这个路径已经存在";
                QFile file(fileName+"//data.txt");
//                qDebug()<<fileName+"//data.txt";
                if(file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate))
                {
                    qDebug()<<"txt文件成功创建";
                }
                QTextStream stream(&file);
                stream.setCodec("UTF-8");
                if(passwd=="")
                {
                    stream<<name;
                }
                else
                {
                    stream<<name<<"\n"<<passwd;
                }
//                qDebug()<<icon.save(fileName+QString("//%1").arg(account),"JPG");
                file.close();
            }
            else//不存在这个路径,创建路径
            {
                bool ok=file->mkpath(fileName);//对于mkdir是创建单级目录 mkpath是创建多级目录的
                if(ok)//若成功创建路径
                {
                    QFile file(fileName+"//data.txt");
                    if(file.open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text))
                    {
                        qDebug()<<"txt文件成功创建";
                    }
                    QTextStream stream(&file);
                    stream.setCodec("UTF-8");//才能输入中文
                    if(passwd=="")
                    {
                        stream<<name;
                    }
                    else
                    {
                        stream<<name<<"\n"<<passwd;
                    }
                    QString filePath = QDir(fileName).filePath(QString("//%1.jpg").arg(account));
                    icon.save(filePath, "JPG");  // 明确指定格式
                    file.close();
                }
                else
                {
                    qDebug()<<"创建路径失败";
                }
            }
            if(this->network)//网络正常连接
            {
                this->hide();
                systemtrayicon->hide();
                w = new MainWindow(account,db);
                w->show();
                w->showicon(account);
                w->setAttribute(Qt::WA_DeleteOnClose);//如果窗口设置了Qt::WA_DeleteOnClose 这个属性，在窗口接受了关闭事件后，Qt会释放这个窗口所占用的资源。
            }
        }
        else
        {
            ui->notexist->show();
        }
    }
}
//实现把下拉框的value放在账号文本框
void signin::on_comboBox_currentIndexChanged(int index)
{
    ui->checkBox_2->setChecked(false);
    ui->lineEdit_2->setText(infoList.at(index));
    ui->lineEdit->setText("");
    if(infoList.at(index)!="")
    {
        ui->lineEdit->setText(infopasswd.at(index));
        ui->checkBox_2->setChecked(true);
    }
    QString icon1=icon.at(index);
    //可能需要进行修改
    ui->label_3->setStyleSheet(QString("border-image: url(%1);width: 200px;height: 200px;border-radius: 50%;border: 2px solid rgb(255, 255, 255);").arg(icon1));
}
//下拉框自定义
QStringList signin::getDirNameList(const QString &strDirpath)
{
    QDir dir(strDirpath);
    QFileInfoList Info_list=dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);//获取符合条件的
    QListIterator<QFileInfo>Iterator(Info_list);//迭代器遍历
    QStringList strDirNameList;
    QFileInfo Info;
    while(Iterator.hasNext())
    {
        Info=Iterator.next();
        if(Info.isDir())
        {
            strDirNameList<<QDir(Info.absoluteFilePath()).dirName();
        }

    }
    return strDirNameList;
}
void signin::on_pushButton_2_clicked()
{
    ui->networkerror->hide();
}
void signin::on_pushButton_3_clicked()
{
    ui->notexist->hide();
}
