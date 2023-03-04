#include"monster.h"
#include<QPainter>
#include<QSize>
#include<QMovie>
#include"mainwindow.h"
#include<QColor>
#include"utility.h"
Monster::Monster(QVector<QPoint> waypoint,  double x, double y, int fid,QMainWindow*parent,MainWindow*game):Waypoint(waypoint),mx(x),my(y),id(fid)
  ,QLabel(parent),m_game(game),m_active(1),m_choosePlant(NULL),m_movie(0)

{
cdTime=new QTimer(this);
    switch (id)
       {
    case 1: //小恐龙怪1
        m_maxHp = 200;   //生命值
m_currentHp=200;
m_damage=0.2;
        ImgPath = ":/pic/images/zom.Png";
        mspeed=0.5;
     break;
    case 2: //小恐龙怪1
        m_maxHp = 300;   //生命值
m_currentHp=300;
m_damage=500;
        ImgPath = ":/pic/images/car.png";
        mspeed=0.5;
     break;
       }

    bingCD=new QTimer(this);
    connect(bingCD,&QTimer::timeout,[=]()
    {
        m_active=1;
        AttackAble=1;
        bingStatus=0;
        bingCD->stop();


    });
    loseBlood=new QTimer(this);
    connect(loseBlood,&QTimer::timeout,[=]()
    {
        m_currentHp-=0.1;
        if(m_currentHp<=0)
        {
            //loseBlood->stop();
            //BloodTime->stop();
            m_game->inc_money();
            if(kuang)
                m_game->AddKuang(1);
            if(bing)
                m_game->AddBing(1);
            if(qun)
                m_game->AddQun(1);
            if(xie)
                m_game->AddXie(1);


           getRemoved();
        }

    });
    BloodTime=new QTimer(this);
    connect(BloodTime,&QTimer::timeout,[=]()
    {
        loseBlood->stop();
        BloodTime->stop();
        bloodStatus=0;

    });

    t=new QTimer(this);
        connect(t,&QTimer::timeout,[=]()
        {
            daojishi-=0.01;
            cdLabel->setParent(m_game);
            cdLabel->setText(QString::number(daojishi));
            cdLabel->setGeometry(mx+80, my-40, 40, 40);
            cdLabel->show();
        });//闪现小叔子变化

        connect(cdTime,&QTimer::timeout,[=]()
        {
            CD=1;
            cdTime->stop();
            t->stop();
            cdLabel->hide();
            daojishi=5;
            //delete cdTime;

        });//闪现五秒cd
}

Monster::Monster():QLabel()
{}

Monster::~Monster()
{

    m_attackerTowerList.clear();//清楚在攻击该敌人的防御塔
    //m_destinationWayPoint=NULL;//全部设为空指针
    m_game=NULL;
    m_choosePlant=NULL;
    delete cdLabel;
    if(m_movie){
        m_movie->stop();
        delete m_movie;
        m_movie = 0;

    }


}
bool Monster::Move()
{
    if(m_active==0)
        return true;

    if(id==2)
    {
       foreach(PlantPos*it,m_game->getPlantPositionList2())
        {
            if(getPos()==it->getCenterPos())
            {
                it->SetIce();
                break;
            }


        }
    }
    if(Waypoint.empty())
    {
        if(Alive())
        {
            m_game->dec_Hp();
        }
       getRemoved();
        return true;
    }

    //如果第一个路径点的y小于怪物原本的路径点，则怪物向下走
    if (Waypoint[0].y() > my) //下
    {
        my += mspeed;
        //return false;
    }

    else if (Waypoint[0].x() < mx) //左
    {
        mx -= mspeed;
        //return false;
    }

    else if (Waypoint[0].x() > mx) //右
    {
        mx += mspeed;
        //return false;
    }

    else if (Waypoint[0].y() < my) //上
    {
        my -= mspeed;
        //return false;
    }

    //如果怪物的坐标和路径点坐标几乎重合，则删除这个路径点
//    if (Waypoint.at(0)->y >= my && Waypoint.at(0)->y <= my + 14 && Waypoint.at(0)->x >= mx && Waypoint.at(0)->x <= mx + 14)
    if (Waypoint[0].y() == my && Waypoint[0].x() == mx)
    {
        delete Waypoint.begin();                //释放坐标点内存
        Waypoint.erase(Waypoint.begin());       //从数组中删除

//        return false;
    }


    return false;
}


QString Monster::GetImgPath() const //获取图片路径
{
    return ImgPath;
}

double Monster::GetX() const       //获取横坐标
{
    return mx;
}

double Monster::GetY() const       //获取横坐标
{
    return my;
}
QPoint Monster::getPos()
{
    return QPoint(mx+75,my+75);
}
void Monster::draw(QPainter&painter)
{
if(id==1)
    painter.drawPixmap(mx, my, 150, 150,
        QPixmap(ImgPath));
else
    painter.drawPixmap(mx, my, 200, 200,
        QPixmap(ImgPath));
    if(SpeedUp)
        painter.drawPixmap(mx, my-40, 40, 40,QPixmap(":/pic/images/jipao.png"));
    if(YouShan())
    {
        if(CD)
            painter.drawPixmap(mx+40, my-40, 40, 40,QPixmap(":/pic/images/shan.png"));
        else
            painter.drawPixmap(mx+40, my-40, 40, 40,QPixmap(":/pic/images/shancd.png"));


    }
    if(bingStatus)
        painter.drawPixmap(mx+100, my-40, 40, 40,
            QPixmap(":/pic/images/bing.png"));
    if(bloodStatus)
        painter.drawPixmap(mx+100, my+5, 40, 40,
            QPixmap(":/pic/images/xie.png"));

    if(kuang)
        painter.drawPixmap(mx+20, my+60, 40, 40,
            QPixmap(":/pic/images/km.png"));
    if(bing)
        painter.drawPixmap(mx+20, my+60, 40, 40,
            QPixmap(":/pic/images/bing.png"));
    if(qun)
        painter.drawPixmap(mx+20, my+60, 40, 40,
            QPixmap(":/pic/images/qun.png"));
    if(xie)
            painter.drawPixmap(mx+20, my+60, 40, 40,
                QPixmap(":/pic/images/xie.png"));

 }
void Monster::doActive()
{
    m_active=true;
}
void Monster::drawblood(QPainter&painter)
{
    painter.save();
        //下面准备绘画敌人的血条
        static const int healthBarWidth=140;//设置血条的长度
        QPoint healthBarPoint=QPoint(mx,my);//设置血条的左上点
        painter.setPen(QPen(Qt::black,3,Qt::SolidLine));//画笔的颜色

        QRect healthBarBackRect(healthBarPoint,QSize(healthBarWidth,5));
        painter.drawRect(healthBarBackRect);//画出血条

        //由于敌人被攻击后会扣血，因此还要画一个显示敌人当前血量的柱形
        painter.setBrush(Qt::red);
        QRect healthBarRect(healthBarPoint,QSize((double)healthBarWidth*m_currentHp/m_maxHp,5));
        painter.drawRect(healthBarRect);//画出当前血量血条
        painter.restore();
}


void Monster::getDamaged(int damage,bool bingbullet,bool xiebullet)
{
    m_currentHp-=damage;
    if(bingbullet&&id!=2)
    {
        m_active=0;
        AttackAble=0;
        bingStatus=1;
        //m_choosePlant=nullptr;
        bingCD->start(5000);

    }
    if(xiebullet)
    {
        bloodStatus=1;
        loseBlood->start(10);
        BloodTime->start(4000);
    }
        if(m_currentHp<=0)
        {
            m_game->inc_money();
            if(kuang)
                m_game->AddKuang(1);
            if(bing)
                m_game->AddBing(1);
            if(qun)
                m_game->AddQun(1);
            if(xie)
                m_game->AddXie(1);


           getRemoved();
        }
}
void Monster::getRemoved()
{
    if(m_choosePlant)
        m_choosePlant->RemoveFromAttacker(this);
    if(m_attackerTowerList.empty())
    {
        m_game->removeEnemy(this);
    }
    else
    {
        foreach(Plant * tower,m_attackerTowerList)
                    tower->targetKilled();
        m_game->removeEnemy(this);

    }
}


void Monster::getLostSight(Plant * tower)
{
    m_attackerTowerList.removeOne(tower);
}
void Monster::getAttacked(Plant *tower)
{
    m_attackerTowerList.push_back(tower);
}

void Monster::checkPlantInRange()//检查敌人是否在攻击范围内
{
    if(AttackAble==0)return;
    if(m_choosePlant)//如果有了攻击的敌人
            {

                    if(YouShan()&&CD) ShanXian();

                    else m_choosePlant->getDamaged(m_damage);

            }
            else//如果没有攻击的敌人，就遍历enemylist，找到在攻击范围内的敌人
            {
                QList<Plant * > plantList=m_game->getPlantList();
                foreach(Plant * enemy,plantList)
                    if(collisionWithCircle(getPos(),50,enemy->getPos(),1))
                    {
                        m_choosePlant=enemy;
                        if(YouShan()&&CD)
                        {
                            ShanXian();
                            break;
                        }
                        m_active=0;

                        m_choosePlant->getAttacked(this);
                        m_choosePlant->getDamaged(m_damage );
                        break;
                    }
            }

}
void Monster::targetKilled()
{
    m_choosePlant=NULL;
    if(!bingStatus)
        doActive();
}
void Monster::RemoveFromAttacker(Plant*p)
{
    if(m_attackerTowerList.indexOf(p )!=-1)
    m_attackerTowerList.removeOne(p);
}
bool Monster::Alive()
{
    if(m_currentHp>0)
        return 1;
    else
        return 0;

}


void Monster::SetSpeed()
{
    mspeed=1;
    SpeedUp=1;
}

bool Monster::YouShan()
{
    return Shan;
}
void Monster::SetShan(bool s)
{
    Shan=s;
}
void Monster::ShanXian()
{
    if(m_choosePlant->getPos()==Waypoint[0]+QPoint(75,75))//植物在拐点上
    {
        if(Waypoint.size()==1)//植物在终点则直接攻击基地
        {
            delete Waypoint.begin();                //释放坐标点内存
            Waypoint.erase(Waypoint.begin());
            if(Alive())
            {
                m_game->dec_Hp();
            }
           getRemoved();
           return;
        }
        else
        {
            if(Waypoint[0].x()==Waypoint[1].x())
            {
                if(Waypoint[1].y()<Waypoint[0].y())//上拐
                {
                    mx=Waypoint[0].x();
                    my=Waypoint[0].y()-55;
                }
                else //下拐
                {
                    mx=Waypoint[0].x();
                    my=Waypoint[0].y()+55;
                }
            }
            else//左右拐
            {

                    my=Waypoint[0].y();
                    mx=Waypoint[0].x()-55;


            }
        }
        delete Waypoint.begin();                //释放坐标点内存
        Waypoint.erase(Waypoint.begin());
    }
    else//植物不在拐点上
    {
        if(my==Waypoint[0].y())//左走
        {
            mx=m_choosePlant->getPos().x()-75-55;
        }
        else
        {
            if(my>Waypoint[0].y())//上走
                my=m_choosePlant->getPos().y()-75-55;
            else //下走
                my=m_choosePlant->getPos().y()-75+55;


        }
    }
m_choosePlant=NULL;
CD=0;
doActive();


t->start(10);



cdTime->start(5000);




}
void Monster::SetCD(bool c)
{
    CD=c;
}

void Monster::SetK(bool k)
{
    kuang=k;
}
void Monster::SetB(bool b)
{
    bing=b;
}
void Monster::SetActive(bool a)
{
    m_active=a;
}
void Monster::SetAttAble(bool a)
{
    AttackAble=a;
}
void Monster::SetQ(bool q)
{
    qun=q;
}
void Monster::SetX(bool x)
{
    xie=x;
}
void Monster::Zanting()
{
    if(Shan&&CD==0)
    {
        cdTime->stop();
        t->stop();
    }
    if(bingStatus)
    {
        bingCD->stop();
    }
    if(bloodStatus)
    {
        BloodTime->stop();
        loseBlood->stop();
    }
}
void Monster::jixu()
{
    if(Shan&&CD==0)
    {
        cdTime->start(daojishi*1000);
        //qDebug()<<"倒计时:"<<daojishi<<endl;
        t->start(10);
    }
    if(bingStatus)
    {
        bingCD->start(5000);
    }
    if(bloodStatus)
    {
        loseBlood->start(10);
        BloodTime->start(4000);
    }
}
