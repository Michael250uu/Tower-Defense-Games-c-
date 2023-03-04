#include "plant.h"
#include"monster.h"
#include"mainwindow.h"
#include <QPoint>
#include <QPainter>
#include <QString>
#include<QMovie>
#include<QVector2D>
#include"utility.h"
const QSize Plant::m_fixedSize(150,150);
Plant::Plant()
{
}

Plant::~Plant()
{
    auto it=m_game->getPlantPositionList2().begin();
    while(it!=m_game->getPlantPositionList2().end())//遍历所有的防御塔坑
    {
        if(getPos()==(*it)->getCenterPos())
        {(*it)->setHasTower(0);(*it)->plant=nullptr;break;}
        it++;

    }
    auto i=m_game->getPlantPositionList1().begin();
    while(i!=m_game->getPlantPositionList1().end())//遍历所有的防御塔坑
    {
        if(getPos()==(*i)->getCenterPos())
        {(*i)->setHasTower(0);(*i) ->plant=nullptr;break;}
        i++;

    }
    if(kuang)
        m_game->AddKuang(1);
    if(bing)
        m_game->AddBing(1);
    if(qun)
        m_game->AddQun(1);
    m_game=NULL;
    m_chooseEnemy=NULL;
    m_fireRateTime=NULL;
    attackers.clear();
    if(m_movie){
        m_movie->stop();
        delete m_movie;
        m_movie = 0;
    }
}
QPoint Plant::getPos()
{
    return m_pos;
}

Plant::Plant(QPoint pos,MainWindow * game,int type,QMainWindow*parent):
    m_pos(pos),
    type(type),
    //m_attackRange(450),//根据地图的大小，确定攻击范围
    m_game(game),
    QLabel(parent),
    m_chooseEnemy(NULL),
     m_fireRate(1000),//1000ms
     m_movie(0)
   //m_damage(20)//攻击力10
{
       m_fireRateTime=new QTimer(this);
       connect(m_fireRateTime,SIGNAL(timeout()),this,SLOT(shootWeapon()));
    switch (type)
    {
    case 1:m_path=":/pic/images/g1.gif";m_currentHp=100;m_maxHp=100;m_attackRange=350;m_damage=10;break;
    case 2:m_path=":/pic/images/sexy.gif";m_currentHp=200;m_maxHp=200;m_attackRange=150;m_damage=15; break;
    }
}

void Plant::draw()
{
    if(m_movie){
        m_movie->stop();
        delete m_movie;
        m_movie = 0;
    }
    m_movie = new QMovie(m_path);
    m_movie->start();
    setMovie(m_movie);
    setGeometry(m_pos.x()-m_fixedSize.width()/2,m_pos.y()-m_fixedSize.height()/2,150,150);
    setScaledContents(true);
    show();

}
void Plant::drawblood(QPainter&painter)
{
    painter.save();
        //下面准备绘画敌人的血条
        static const int healthBarWidth=140;//设置血条的长度
        QPoint healthBarPoint=m_pos+QPoint(-75,-80);//设置血条的左上点
        painter.setPen(QPen(Qt::black,3,Qt::SolidLine));//画笔的颜色

        QRect healthBarBackRect(healthBarPoint,QSize(healthBarWidth,5));
        painter.drawRect(healthBarBackRect);//画出血条

        //由于敌人被攻击后会扣血，因此还要画一个显示敌人当前血量的柱形
        painter.setBrush(Qt::yellow);
        QRect healthBarRect(healthBarPoint,QSize((double)healthBarWidth*m_currentHp/m_maxHp,5));
        painter.drawRect(healthBarRect);//画出当前血量血条
        painter.restore();
        if(kuang)
            painter.drawPixmap(m_pos.x()-75,m_pos.y()-75-45, 40, 40,
                QPixmap(":/pic/images/km.png"));
        if(bing)
            painter.drawPixmap(m_pos.x()-75+40,m_pos.y()-75-45, 40, 40,
                QPixmap(":/pic/images/bing.png"));
        if(qun)
            painter.drawPixmap(m_pos.x()-75+81,m_pos.y()-75-45, 40, 40,
                QPixmap(":/pic/images/qun.png"));
        if(xie)
            painter.drawPixmap(m_pos.x()-75,m_pos.y()-75-45, 40, 40,
                QPixmap(":/pic/images/xie.png"));


}

void Plant::checkEnemyInRange()//检查敌人是否在攻击范围内
{
    if(m_chooseEnemy)//如果有了攻击的敌人
            {
                QVector2D normalized(m_chooseEnemy->getPos()-m_pos);
                normalized.normalize();
                if(!collisionWithCircle(m_pos,m_attackRange,m_chooseEnemy->getPos(),1))//当敌人不在范围内的时候
                {
                    lostSightOfEnemy();
                }
            }
            else//如果没有攻击的敌人，就遍历enemylist，找到在攻击范围内的敌人
            {
                QList<Monster * > enemyList=m_game->getEnemyList();
                foreach(Monster * enemy,enemyList)
                    if(collisionWithCircle(m_pos,m_attackRange,enemy->getPos(),1))
                    {

                        chooseEnemyFromAttack(enemy);
                        break;
                    }
            }

}
void Plant::lostSightOfEnemy()
{
    m_chooseEnemy->getLostSight(this);
    if(m_chooseEnemy)
    {
        m_chooseEnemy=NULL;
    }
    m_fireRateTime->stop();
}

void Plant::chooseEnemyFromAttack(Monster *enemy)
{
    m_chooseEnemy=enemy;
    attackEnemy();
    m_chooseEnemy->getAttacked(this);//该敌人受到该防御塔的攻击
}

void Plant::attackEnemy()
{
    m_fireRateTime->start(m_fireRate);//开始攻击
}
void Plant::targetKilled()
{
    if(m_chooseEnemy)
    {
        m_chooseEnemy=NULL;
    }
    m_fireRateTime->stop();//敌人死亡，停止开火
}
void Plant::shootWeapon()
{
    Bullet * bullet=new Bullet(m_pos,m_chooseEnemy->getPos(),m_damage,m_chooseEnemy,m_game,type);//构造一个子弹，准备攻击敌人
    if(bing)
        bullet->SetBing(1);
    if(qun)
        bullet->SetQun(1);
    if(xie)
        bullet->SetXie(1);
    bullet->move();
    m_game->addBullet(bullet);//将该子弹添加到mainwindow中
}
void Plant::getAttacked(Monster*m)
{
    attackers.push_back(m);
}
void Plant::getDamaged(double d)
{
    m_currentHp-=d;
        if(m_currentHp<=0)
        {


           getRemoved();
        }
}
void Plant::getRemoved()
{

    /*auto it=m_game->getPlantPositionList2().begin();
    while(it!=m_game->getPlantPositionList2().end())//遍历所有的防御塔坑
    {
        if(getPos()==it->getCenterPos())
        {it->setHasTower(0);break;}
        it++;
    }*/
    if(m_chooseEnemy)
        m_chooseEnemy->RemoveFromAttacker(this);
    if(attackers.empty())
    {
        m_game->removePlant(this);
    }
    else
    {
        //qDebug()<<attackers.size();
        foreach(Monster * m,attackers)
                    m->targetKilled();
        m_game->removePlant(this);

    }
}
void Plant::RemoveFromAttacker(Monster*m)
{
    if(attackers.indexOf(m )!=-1)
    attackers.removeOne(m);
}

void Plant::SetKuang(bool k)
{
    kuang=k;
}
void Plant::SetDamage(int d)
{
    m_damage=d;
}
void Plant::SetRate(int r)
{
    if(m_chooseEnemy)
    {
        m_fireRateTime->start(r); m_fireRate=r;
    }
    else
        m_fireRate=r;

}
void Plant::SetBing(bool b)
{
    bing=b;
}
void Plant::SetQun(bool q)
{
    qun=q;
}

void Plant::SetXie(bool x)
{
    xie=x;
}
void Plant::stopAtt()
{
    m_fireRateTime->stop();
}
void Plant::jixu()
{
    if(m_chooseEnemy)
        m_fireRateTime->start(m_fireRate);
}
void Plant::baoza()
{
    m_currentHp=m_maxHp;
}
