#include "login.h"
#include "ui_login.h"
#include <QPainter>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    //去掉创建的边框
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //设置当前窗口所有字体
    this->setFont(QFont("黑体", 11, QFont::Bold, false));
    QString ip = m_cm.getCfgValue("web_server", "ip");
    QString port = m_cm.getCfgValue("web_server", "port");
    ui->ip->setText(ip);
    ui->port->setText(port);
    ui->stackedWidget->setCurrentWidget(ui->login_page);
    //titlewidget信号处理
    connect(ui->title_wg, &TitleWg::closeWindow, [=]()
    {
        //判断当前stackedWidget显示的页面
        if(ui->stackedWidget->currentWidget() == ui->set_page)
        {
            // 切换到登陆
            ui->stackedWidget->setCurrentWidget(ui->login_page);
            //清空控件数据
        }
        else if(ui->stackedWidget->currentWidget() == ui->reg_page){
            ui->stackedWidget->setCurrentWidget(ui->login_page);
            //清空控件数据
            ui->name_reg->clear();
        }
        else {
            this->close();
        }
    });

    connect(ui->title_wg, &TitleWg::showSetWg, [=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->set_page);
    });

    connect(ui->new_reg_button, &QToolButton::clicked, [=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->reg_page);
    });


}

Login::~Login()
{
    delete ui;
}

void Login::paintEvent(QPaintEvent *event)
{
    //给窗口画背景图
    QPainter p(this); //绘图设备
    QPixmap pixmap(":/images/login_bk.jpg");
    p.drawPixmap(0, 0, this->width(), this->height(), pixmap);
}

void Login::on_reg_button_clicked()
{
    //取数据
    QString ip = ui->ip->text();
    QString port = ui->port->text();
    //注册数据
    QString name = ui->name_reg->text();
    QString nick = ui->nike_name->text();
    QString passwd = ui->passwd_reg->text();
    QString confirm_pwd = ui->pwd_affirm_reg->text();
    QString phone = ui->phone->text();
    QString email = ui->email->text();
    //校验
    QRegExp exp(USER_REG);
    if(!exp.exactMatch(name))
    {
        QMessageBox::warning(this, "警告", "用户名不正确");
        ui->name_reg->clear();
        //设置控件焦点
        ui->name_reg->setFocus();
        return;
    }
    exp = QRegExp(PASSWD_REG);
    if(!exp.exactMatch(passwd))
    {
        QMessageBox::warning(this, "警告", "密码格式不正确");
        ui->passwd_reg->clear();
        //设置控件焦点
        ui->passwd_reg->setFocus();
        return;
    }
    if (passwd != confirm_pwd)
    {

    }
    exp = QRegExp(PHONE_REG);
    if(!exp.exactMatch(phone))
    {
        QMessageBox::warning(this, "警告", "手机号码格式不正确");
        ui->phone->clear();
        //设置控件焦点
        ui->phone->setFocus();
        return;
    }
    exp = QRegExp(EMAIL_REG);
    if(!exp.exactMatch(email))
    {
        QMessageBox::warning(this, "警告", "邮箱格式不正确");
        ui->email->clear();
        //设置控件焦点
        ui->email->setFocus();
        return;
    }
    //组织json格式的字符串
    //需要知道发给server什么样的数据
    //将用户输入的注册信息-->json对象
    QByteArray postData = getRegJson(name, nick, m_cm.getStrMd5(passwd), phone, email);
    //发送http请求协议
    QNetworkAccessManager *manager = Common::getNetManager();
    //http请求协议头
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, postData.size());
    //url
    QString urlString = QString("http://%1:%2/reg").arg(ip).arg(port);
//    cout << url << endl;
    QUrl url(urlString);
//    url.setPort(port.toInt());
    cout << url.toString() << endl;
    request.setUrl(url);
    //接收server返回的数据
    
    QNetworkReply *reply = manager->post(request, postData);
    connect(reply, &QNetworkReply::readyRead, [=]()
    {
        //读返回的数据
        //回复的数据格式：
        //成功{"code":"002"}
        //该用户已存在{"code":"003"}
        //失败{"code":"004"}
        QByteArray jsonData = reply->readAll();
        //解析json字符串
        QString status = m_cm.getCode(jsonData);
        //得到一个数 QString status
        if("002" == status)
        {
            //success
            //将当前注册用户信息填写到登陆的输入框中
            ui->user_login->setText(name);
            ui->passwd_reg->setText(passwd);
            //当前注册信息清楚掉
            //跳转到登陆页面
            ui->stackedWidget->setCurrentWidget(ui->login_page);
        }
        else if("003" == status)
        {
            //用户已存在
            QMessageBox::warning(this, "警告", "当前注册的用户已存在");
            return;
        }
        else if ("004" == status)
        {
            //注册失败
            QMessageBox::critical(this, "警告", "注册失败");
            return;
        }
    });
}


//服务器设置
void Login::on_set_button_clicked(bool checked)
{
    //获取控件数据
    QString ip = ui->ip->text();
    QString port = ui->port->text();
    //数据校验
    QRegExp exp(IP_REG);
    if(!exp.exactMatch(ip))
    {
        QMessageBox::warning(this, "警告", "IP格式不正确");
        ui->ip->clear();
        //设置控件焦点
        ui->ip->setFocus();
        return;
    }
    //端口校验
    exp.setPattern(PORT_REG);
    if(!exp.exactMatch(port))
    {
        QMessageBox::warning(this, "警告", "端口格式不正确");
        ui->port->clear();
        //设置控件焦点
        ui->port->setFocus();
        return;
    }
    //端口
    //数据写入配置文件
    m_cm.writeWebInfo(ip, port);
    //跳转到登陆页面
    ui->stackedWidget->setCurrentWidget(ui->login_page);
}

QByteArray Login:: getRegJson(QString user, QString nick, QString pwd, QString phone, QString email)
{
//    {
//        userName:xxxx,
//        nickName:xxx,
//        firstPwd:xxx,
//        phone:xxx,
//        email:xxx
//    }
    QJsonObject obj;
    obj.insert("userName", user);
    obj.insert("nickName", nick);
    obj.insert("firstPwd", pwd);
    obj.insert("phone", phone);
    obj.insert("email", email);
    //obj->jsondoc
    QJsonDocument doc = QJsonDocument(obj);
    cout << doc.toJson();
    return doc.toJson();
}

QByteArray Login::getLoginJson(QString user, QString password)
{
    QJsonObject obj;
    obj.insert("user", user);
    obj.insert("pwd", password);
    QJsonDocument doc(obj);
    return doc.toJson();
}

QStringList Login::parseLoginJson(QByteArray json)
{
    QStringList list;
    QJsonDocument doc = QJsonDocument::fromJson(json);
    if (doc.isObject())
    {
        QJsonObject obj = doc.object();
        list.append(obj.value("code").toString());
        list.append(obj.value("token").toString());
    }
    return  list;
}

void Login::on_loginButton_clicked()
{
    //取值
    QString user = ui->user_login->text();
    QString password = ui->pwd_login->text();
    QString address = ui->ip->text();
    QString port = ui->port->text();

    //数据校验
    //是否保存密码
    bool remember = ui->remember_pwd->isChecked();

    //将数据保存到配置文件
    m_cm.writeLoginInfo(user, password, remember);
    //登录的json数据包
    QByteArray data = getLoginJson(user, password);

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    QString url = QString("http://%1:%2/login").arg(address).arg(port);
    request.setUrl(QUrl(url));
    QNetworkReply *reply = manager->post(request, data);
    connect(reply, &QNetworkReply::readyRead, [=]()
    {
        QByteArray json = reply->readAll();
        //解析json字符串
        QStringList list = parseLoginJson(json);
        if ("000" == list.at(0))
        {
            //登录成功
            //当前用户信息存储起来

        }
    });

}
