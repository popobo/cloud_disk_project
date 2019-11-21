#include "titlewg.h"
#include "ui_titlewg.h"
#include <QMouseEvent>

TitleWg::TitleWg(QWidget *parent) :
    QWidget(parent), //parent父窗口
    ui(new Ui::TitleWg)
{
    ui->setupUi(this);
    //logo图片
    ui->logo->setPixmap(QPixmap(":/images/logo.ico").scaled(40, 40));

    ui->wgtitle->setStyleSheet("color:rgb(255, 255, 225)");

    m_parent = parent;

    //按钮功能实现
    connect(ui->set, &QToolButton::clicked, [=]()
    {
        //发送自定义信号
        emit showSetWg();
    });
    connect(ui->min, &QToolButton::clicked, [=]()
    {
        m_parent->showMinimized();
    });
    connect(ui->close, &QToolButton::clicked, [=]()
    {
        emit closeWindow();
    });
}

TitleWg::~TitleWg()
{
    delete ui;
}

void TitleWg::mouseMoveEvent(QMouseEvent *event)
{
    //只允许左键拖
    if(event->buttons() & Qt::LeftButton)
    {
        //窗口跟着鼠标移动
        //求窗口左上角点 = 鼠标当前位置 - 差值窗口左上角点
        m_parent->move(event->globalPos() - m_pt);

    }
}

void TitleWg::mousePressEvent(QMouseEvent *event)
{
    //如果左键按下
    if(event->button() == Qt::LeftButton)
    {
        //求差值 = 鼠标当前位置 - 窗口左上角点
        m_pt = event->globalPos() - m_parent->geometry().topLeft();
    }
}
