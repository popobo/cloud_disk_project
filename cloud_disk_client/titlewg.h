#ifndef TITLEWG_H
#define TITLEWG_H

#include <QWidget>

namespace Ui {
class TitleWg;
}

class TitleWg : public QWidget
{
    Q_OBJECT

public:
    explicit TitleWg(QWidget *parent = nullptr);
    ~TitleWg();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent * event);

signals:
    //信号不需要实现 下一级
    void showSetWg();
    void showRegWg();
    void closeWindow();

private:
    Ui::TitleWg *ui;
    QPoint m_pt; //差值: 鼠标当前位置 - 窗口左上角点
    QWidget *m_parent;

};

#endif // TITLEWG_H
