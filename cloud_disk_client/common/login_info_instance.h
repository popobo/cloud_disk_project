#ifndef LOGIN_INFO_INSTANCE_H
#define LOGIN_INFO_INSTANCE_H

#include <QString>
#include "common.h"

//单例模式，主要保存当前登录用户，服务器信息
class LoginInfoInstance
{
public:
    static LoginInfoInstance *getInstance();
    static void destroy(); //释放堆空间

    void setLoginInfo(QString tempUser, QString tempIp, QString tempPort, QString token=""); //设置登录
    QString getUser() const; //获取登录用户
    QString getIp() const; //获取服务器IP
    QString getPort() const; //获取服务器端口
    QString getToken() const; //获取登录token
private:
    //构造函数和析构函数为私有的
    LoginInfoInstance();
    ~LoginInfoInstance();

};

#endif // LOGIN_INFO_INSTANCE_H
