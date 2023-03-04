#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QPoint>
#include <QPainter>
#include <QString>
#include <QSize>

#include "monster.h"
#include "plant.h"
//#include "mainwindow.h"

//class Enemy;
class MainWindow;
class Plant;

class Bullet :public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPoint m_currentPos READ getCurrentPos WRITE setCurrentPos)//子弹动态移动
public:
    Bullet();
    Bullet(QPoint startPos,QPoint targetPos,int damage,Monster * targetEnemy,MainWindow * game,int type);
    ~Bullet();
    QPoint getCurrentPos();//得到子弹的当前位置
    void setCurrentPos(QPoint pos);//设置子弹的当前位置

    void move();//子弹的移动
    void draw(QPainter * painter)const;//绘画子弹
    void SetBing(bool);
    void SetQun(bool);
    void SetXie(bool);
private slots:
    void hitTarget();//私有信号槽，击中敌人的时候触发

private:
    QPoint m_startPos;
    QPoint m_targetPos;
    QPoint m_currentPos;
    int m_damage;
    QString m_path;

    Monster * m_targetEnemy;
    MainWindow * m_game;
    bool bing=0;
    bool qun=0;
bool xie=0;
    static const QSize m_fixedSize;
};

#endif // BULLET_H
