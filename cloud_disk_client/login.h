#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "common/common.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

protected:
    //绘图事件函数
    void paintEvent(QPaintEvent *event);

private slots:
    void on_reg_button_clicked();

    void on_set_button_clicked(bool checked);

    //注册信息-->json
    QByteArray getRegJson(QString user, QString nick, QString pwd, QString phone, QString email);

    //登录信息-->json
    QByteArray getLoginJson(QString user, QString password);
    QStringList parseLoginJson(QByteArray json);

    //初始化登陆界面

    void on_loginButton_clicked();

private:
    Ui::Login *ui;
    Common m_cm;
};

#endif // LOGIN_H
