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
    //把复制析构函数和=操作符也设为私有，防止被复制
    LoginInfoInstance(const LoginInfoInstance&);
    LoginInfoInstance& operator=(const LoginInfoInstance&);

    //它的唯一工作就是在析构函数中删除Singleton的实例
    class Recollect
    {
    public:
        ~Recollect()
        {
            //释放堆区空间
            LoginInfoInstance::destroy();
        }
    };

    //定义一个静态成员变量，程序结束时,系统会自动调用它的析构函数
    //static类的析构函数在main()退出后调用
    static Recollect temp;//静态数据成员，类中声明，类外定义

    //静态数据成员，类中声明，类外必须定义
    static LoginInfoInstance *instance;

    QString user; //当前登录用户
    QString token; //登录token
    QString ip; //web服务器ip
    QString port; //web服务器端口
};

#endif // LOGIN_INFO_INSTANCE_H
