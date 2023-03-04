#include "bullet.h"
#include "plant.h"
#include "mainwindow.h"
#include "utility.h"
#include "monster.h"

#include <QPoint>
#include <QPainter>
#include <QString>
#include <QPropertyAnimation>

const QSize Bullet::m_fixedSize(8,8);

Bullet::Bullet()
{
}

Bullet::Bullet(QPoint startPos,QPoint targetPos,int damage,Monster * targetEnemy,MainWindow * game,int type):
    m_startPos(startPos),
    m_targetPos(targetPos),
    m_damage(damage),
    //m_path(path),
    m_targetEnemy(targetEnemy),
    m_game(game)
{
    switch(type)
    {
    case 1:m_path=":/pic/images/b1.png";break;
    case 2:m_path=":/pic/images/b3.png";break;
    }
}
Bullet::~Bullet()
{
    m_targetEnemy=NULL;
     m_game=NULL;
}
QPoint Bullet::getCurrentPos()
{
    return m_currentPos;
}

void Bullet::setCurrentPos(QPoint pos)
{
    m_currentPos=pos;
}

void Bullet::move()
{
    static int duration=200;//子弹飞行的时间，经过100ms击中敌人
    QPropertyAnimation * animation=new QPropertyAnimation(this,"m_currentPos");
    animation->setDuration(duration);//设置持续时间
    animation->setStartValue(m_startPos);//设置起始位置
    animation->setEndValue(m_targetPos);//设置目标位置
    connect(animation,SIGNAL(finished()),this,SLOT(hitTarget()));//连接信号槽，击中敌人后，子弹动态运动结束
    animation->start();
}

void Bullet::hitTarget()
{
    if(m_game->getEnemyList().indexOf(m_targetEnemy)!=-1)//如果mainwindow的敌人列表中，有子弹击中的这个敌人，该敌人受到相应的伤害
    {

        m_targetEnemy->getDamaged(m_damage,bing,xie);
        if(qun)
        {
            foreach(Monster*m,m_game->getEnemyList())
            {
                if(m!=m_targetEnemy)
                    if(collisionWithCircle(m_targetPos,150,m->getPos(),1))
                        m->getDamaged(m_damage,bing,xie);

            }
        }

    }
    m_game->removeBullet(this);//击中敌人后子弹就要消失
}

void Bullet::draw(QPainter *painter) const
{
    painter->drawPixmap(m_currentPos.x(),m_currentPos.y(),50,50,m_path);
}
void Bullet::SetBing(bool b)
{
    bing=b;
}
void Bullet::SetQun(bool q)
{
    qun=q;
}
void Bullet::SetXie(bool x)
{
    xie=x;
}
