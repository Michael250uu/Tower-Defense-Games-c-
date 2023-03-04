#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"monster.h"
#include"startscene.h"
#include <QPushButton>
#include"fileselect.h"
#include "ui_fileselect.h"
#include"QMessageBox"
#include<QTextStream>
#include<QTime>
#include<QMediaPlayer>
#include"menu.h"
#include"utility.h"
#include<QMovie>
#include<QLabel>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
   m_waves(0),
 wavec(0)
  ,m_gameWin(false)
  ,m_gameLose(false)
,chang(12)
,kuan(8)
,Money(300)
,Hp(100)
,begin(QPoint(11,5))
{
        player_start = new QMediaPlayer;
        connect(player_start, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
        player_start->setMedia(QUrl::fromLocalFile("E:\\pvz\\images\\start.mp3"));
        player_start->setVolume(5);
        player_start->play();

        player_dadou = new QMediaPlayer;
        connect(player_dadou, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
        player_dadou->setMedia(QUrl::fromLocalFile("E:\\pvz\\images\\dadou.mp3"));
        player_dadou->setVolume(5);
        xunhuan=new QTimer(this);
        connect(xunhuan,&QTimer::timeout,[=]()
        {
            player_dadou->stop();
            player_dadou->play();


        });
        //player_start->play();
        caidan=new menu(this,this);
    StartScene *start=new StartScene(this,this);
    start->show();
    ui->setupUi(this);
    //设置固定窗口大小
        setFixedSize(1950, 1350);
        /*QPushButton *btn=new QPushButton(this);
        connect(btn,&QPushButton::clicked,this,[=](){
            StartScene *start=new StartScene(this);
            this->hide();
            start->show();
        });*/

        //设置标题
        init_map();
        setWindowTitle("游戏界面");
        loadTowerPosition1();
        loadSelect();
        //设置生命值
timer2=new QTimer(this);
timer1=new QTimer(this);
 timer3=new QTimer(this);
 //timer =new QTimer(this);
  //MonAttTime=new QTimer(this);

  //GameStart();
        //QLabel *label_hp = new QLabel(this);
            ui->label_hp->setParent(this);
           ui->label_hp->setText("hp: "+QString::number(Hp));
           QFont ft;
           ft.setPointSize(12);
           ui->label_hp->setFont(ft);
            ui->label_hp->setStyleSheet("#label_hp{border:1px solid red}");
            ui->label_hp->setFixedSize(150,75);
            ui->label_hp->setGeometry(1800,0,150,75);
            connect(this,&MainWindow::Hp_change,ui->label_hp,&QLabel::setText);//基地血量

            ui->label_money->setParent(this);
           ui->label_money->setText("money: "+QString::number(Money));
           QFont ft2;
           ft2.setPointSize(12);
           ui->label_money->setFont(ft2);
            ui->label_money->setStyleSheet("#label_money{border:1px solid yellow}");
            ui->label_money->setFixedSize(150,75);
            ui->label_money->setGeometry(1800,75,150,75);
            connect(this,&MainWindow::Money_change,ui->label_money,&QLabel::setText);
            //设置金钱

            connect(this,&MainWindow::Kuang_change,ui->kuang_num,&QLabel::setText);//狂暴个数

        QPoint p1(1000,400);
           QPoint p2(1500,750);


           Path.push_back(p2);
           Path.push_back(QPoint(1500,900));
           Path.push_back(QPoint(1050,900));
           Path.push_back(QPoint(1050,600));
           Path.push_back(QPoint(600,600));

           Path.push_back(QPoint(600,900));
            Path.push_back(QPoint(0,900));

           Monster *n=new Monster(Path,1650,750,1,this,this);

           //MonsterVec.push_back(n);


           connect(timer2,SIGNAL(timeout()),this,SLOT(addMon()));




           connect(timer1,SIGNAL(timeout()),this,SLOT(updateMon()));//怪物位置跟新（含植物攻击）

//connect(MonAttTime,SIGNAL(timeout()),this,SLOT(MonAtt()));//怪物攻击

            connect(timer3,&QTimer::timeout,[=]()
            {
                timer2->start();
                ChanMon=1;
                m_waves++;
                if(m_waves==3)//共 该数+1波
                    timer3->stop();
            });//怪物跟新


           connect(this,&MainWindow::stop_appear,timer2,&QTimer::stop);//一波5个就停止


           //connect(timer,SIGNAL(timeout()),this,SLOT(update()));//地图实时跟新


           game_end=new QTimer(this);

           connect(game_end,&QTimer::timeout,[=]()
           {
               if(m_gameWin)
               {
                   stop_game();
                   GameOver(":/pic/images/w.png",start);game_end->stop();
               }
               else if(m_gameLose)
               {
                   stop_game();
                   GameOver(":/pic/images/loss.jpg",start);game_end->stop();
               }
               //
           });


//
ui->stop->setGeometry(1800,150,150,150);
           connect(ui->stop,&QPushButton::clicked,[=](){
                 //stop_game();
               if(zanting==0)
               {
                   ZanTing();
               }
              });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DrawMapArr(QPainter& painter)
{
    //地图数组  第一关

    //第二关

    //int Map[8][12];    //用于拷贝不同的关卡数组


       // memcpy(Map, Map_1, sizeof(Map));

    for (int j = 0; j < kuan; j++)
        for (int i = 0; i < chang; i++)
        {
            switch (Map[j][i])
            {
            case 0:     /*路径*/
                painter.drawPixmap(i * 150, j * 150, 150, 150,
                    QPixmap(":/pic/images/path.png"));
                break;
            case 1:     /*草坪*/
                painter.drawPixmap(i * 150, j * 150, 150, 150,
                    QPixmap(":/pic/images/grass.png"));
                break;
            case 2:     /*水*/
                painter.drawPixmap(i * 150, j * 150, 150, 150,
                    QPixmap(":/pic/images/water.png"));
                //防御塔坑坐标初始化塔坑坐标，插入到塔坑对象数组
                //TowerPitVec.push_back(new DefenseTowerPit(i * 40, j * 40));
                break;

            }
        }
    foreach(PlantPos* p,m_plantPositionList2)
    {
        if(p->ice)
            painter.drawPixmap(p->getPos().x(),p->getPos().y(), 150, 150,
                QPixmap(":/pic/images/ice.png"));
    }

    painter.drawPixmap(0 * 150, kuan * 150, 150, 150,
        QPixmap(":/pic/images/select1.png"));//远程
    painter.drawPixmap(1 * 150, kuan * 150, 150, 150,
        QPixmap(":/pic/images/select2.png"));//近战

    painter.drawPixmap(2 * 150, kuan * 150, 150, 75,
        QPixmap(":/pic/images/kuang.png"));//狂暴

    painter.drawPixmap(3 * 150, kuan * 150, 150, 75,
        QPixmap(":/pic/images/chan.png"));//删除狂暴
    ui->del_kuang->setParent(this);
     ui->del_kuang->setText("卸下狂暴");
     ui->del_kuang->setGeometry(450,kuan*150+75,150,75);
     ui->del_kuang->show();//删除狂暴

     ui->kuang_num->setParent(this);
    ui->kuang_num->setText("个数: "+QString::number(kuangs));
     ui->kuang_num->setGeometry(300,kuan*150+75,150,75);
     ui->kuang_num->show();
     //-------------------------------------------------------------------------------
     painter.drawPixmap(4 * 150, kuan * 150, 150, 75,
         QPixmap(":/pic/images/bing.png"));//冰冻
     painter.drawPixmap(5 * 150, kuan * 150, 150, 75,
         QPixmap(":/pic/images/chan.png"));//删除冰冻
     ui->del_bing->setParent(this);
      ui->del_bing->setText("卸下冰冻");
      ui->del_bing->setGeometry(750,kuan*150+75,150,75);
      ui->del_bing->show();//删除群伤

      ui->bing_num->setParent(this);
     ui->bing_num->setText("个数: "+QString::number(bings));
      ui->bing_num->setGeometry(600,kuan*150+75,150,75);
      ui->bing_num->show();
      //--------------------------------------------------------------------------------
      painter.drawPixmap(6 * 150, kuan * 150, 150, 75,
          QPixmap(":/pic/images/qun.png"));//群伤
      painter.drawPixmap(7 * 150, kuan * 150, 150, 75,
          QPixmap(":/pic/images/chan.png"));//删除群伤
      ui->del_qun->setParent(this);
       ui->del_qun->setText("卸下群伤");
       ui->del_qun->setGeometry(1050,kuan*150+75,150,75);
       ui->del_qun->show();//删除群伤

       ui->qun_num->setParent(this);
      ui->qun_num->setText("个数: "+QString::number(quns));
       ui->qun_num->setGeometry(900,kuan*150+75,150,75);
       ui->qun_num->show();
       //--------------------------------------------------------------------------------
       painter.drawPixmap(8 * 150, kuan * 150, 150, 75,
           QPixmap(":/pic/images/xie.png"));//流血
       painter.drawPixmap(9 * 150, kuan * 150, 150, 75,
           QPixmap(":/pic/images/chan.png"));//删除流血
       ui->del_xie->setParent(this);
        ui->del_xie->setText("卸下流血");
        ui->del_xie->setGeometry(1350,kuan*150+75,150,75);
        ui->del_xie->show();//删除流血

        ui->xie_num->setParent(this);
       ui->xie_num->setText("个数: "+QString::number(xies));
        ui->xie_num->setGeometry(1200,kuan*150+75,150,75);
        ui->xie_num->show();

        //-------------------------------------------------------------------------
        painter.drawPixmap(12 * 150, 350, 150, 150,
            QPixmap(":/pic/images/chan.png"));//铲植物用

        //--------------------------------------------------------------------------
        painter.drawPixmap(10 * 150, kuan * 150, 150, 150,
            QPixmap(":/pic/images/boom.png"));//炸弹
}



void MainWindow::paintEvent(QPaintEvent *)
{
     QPainter painter(this);     //创建画家类


         DrawMapArr(painter);  //画出地图

    if(mouseflag==1||mouseflag==2)
        painter.drawPixmap(mpos.x(),mpos.y(),150,150,mpic);//画鼠标上的图片
    else if(mouseflag==3||mouseflag==5||mouseflag==7||mouseflag==9)
        painter.drawPixmap(mpos.x(),mpos.y(),50,50,mpic);//画鼠标上的图片(不需要太大)
    else if(mouseflag!=0)
        painter.drawPixmap(mpos.x(),mpos.y(),100,100,mpic);//画鼠标上的图片(中等大小)

    foreach( Plant * plant,m_plantList)
            plant->drawblood(painter);
    foreach(const Bullet * bullet,m_bulletList)
            bullet->draw(&painter);
   ShowMon();
//ShowPlant();
}

void MainWindow::loadTowerPosition1()
{
    //这里和找航点是一样的，制作者需要自己不断尝试
    //找到比较合适的防御塔坑点
    QPoint pos1[]=
    {
        QPoint(0,750),
        QPoint(0,1050),
        QPoint(8*150,3*150),
        //QPoint(439,98),
        //QPoint(105,215),
        //QPoint(186,215),
        //QPoint(314,215),
        //QPoint(105,321),
        //QPoint(223,323),
        //QPoint(365,319)
    };
    int len1=sizeof(pos1)/sizeof(pos1[0]);
    for(int i=0;i<len1;i++)
    {   PlantPos*p=new PlantPos(pos1[i]);
        m_plantPositionList1.push_back(p);
    }
    QPoint pos2[]=
    {
        QPoint(150,900),

        QPoint(8*150,6*150),
        //QPoint(105,215),
        //QPoint(186,215),
        //QPoint(314,215),
        //QPoint(105,321),
        //QPoint(223,323),
        //QPoint(365,319)
    };
    int len2=sizeof(pos2)/sizeof(pos2[0]);
    for(int i=0;i<len2;i++)
    {   PlantPos*p=new PlantPos(pos2[i]);
        m_plantPositionList2.push_back(p);
    }
}


void MainWindow::loadSelect()
{
    Select s1(QPoint(0,1200),":/pic/images/m1.png",1);//远程
    Select s2(QPoint(150,1200),":/pic/images/m2.png",2);//近战
Select s3(QPoint(300,1200),":/pic/images/km.png",3);//狂暴
Select s4(QPoint(450,1200),":/pic/images/chan.png",4);//卸载狂暴
Select s5(QPoint(600,1200),":/pic/images/bing.png",5);//冰冻
Select s6(QPoint(750,1200),":/pic/images/chan.png",6);//卸载冰冻
Select s7(QPoint(900,1200),":/pic/images/qun.png",7);//群伤
Select s8(QPoint(1050,1200),":/pic/images/chan.png",8);//群伤
Select s9(QPoint(1200,1200),":/pic/images/xie.png",9);//流血
Select s10(QPoint(1350,1200),":/pic/images/chan.png",10);//卸载流血
Select s11(QPoint(12*150,350),":/pic/images/chan.png",11);//铲除植物
Select s12(QPoint(10*150,8*150),":/pic/images/zha.Png",12);//炸弹
    m_selectList.push_back(s1);
    m_selectList.push_back(s2);
    m_selectList.push_back(s3);
    m_selectList.push_back(s4);
    m_selectList.push_back(s5);
    m_selectList.push_back(s6);
    m_selectList.push_back(s7);
    m_selectList.push_back(s8);
    m_selectList.push_back(s9);
    m_selectList.push_back(s10);
    m_selectList.push_back(s11);
     m_selectList.push_back(s12);
}




void MainWindow::mousePressEvent(QMouseEvent * event)
{if(zanting)return;
    if(Qt::LeftButton==event->button())//如果是鼠标左键点击
    {
        QPoint pressPos=event->pos();//得到鼠标点击的位置

        auto i=m_selectList.begin();

        while(i!=m_selectList.end())
        {


                if(i->ContainPos(pressPos))
                {
                    mpos=event->pos();
                    mouseflag=i->gettype();
                    mpic=i->getpath();
                    break;
                }

            i++;
        }
   }
}


void MainWindow::mouseMoveEvent(QMouseEvent * event)
{
    mpos=event->pos();
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(mouseflag==1)//远程

    {
        mouseflag=0;
        auto it=m_plantPositionList1.begin();
        while(it!=m_plantPositionList1.end())//遍历所有的防御塔坑
            {


                    if((*it)->ContainPos(mpos) && !(*it)->hasTower()&&Money>=100)//如果鼠标点击的位置在防御塔坑的范围内,并且没有防御塔，并且钱够
                    {
                        dec_money(100);
                        Plant * tower=new Plant((*it)->getCenterPos(),this,1,this);//创建一个新的防御塔
                        m_plantList.push_back(tower);//把这个防御塔放到储存防御塔的list中
                        //update();//更新地图
                        (*it)->setHasTower(true);//设置这个防御塔坑内有防御塔了
                        (*it)->plant=tower;
                        tower->draw();
                        break;//进行了一次操作，可以直接退出循环了
                    }

                ++it;
            }
    }
    else if(mouseflag==2)//近战
    {
        mouseflag=0;
        auto it=m_plantPositionList2.begin();
        while(it!=m_plantPositionList2.end())//遍历所有的防御塔坑
            {


                    if((*it)->ContainPos(mpos) &&Money>=50&&(*it)->ice!=1)//如果鼠标点击的位置在防御塔坑的范围内,并且没有防御塔
                    {
                        if(!(*it)->hasTower())
                        {
                            dec_money(50);
                        Plant * tower=new Plant((*it)->getCenterPos(),this,2,this);//创建一个新的防御塔

                        m_plantList.push_back(tower);//把这个防御塔放到储存防御塔的list中

                        (*it)->setHasTower(true);//设置这个防御塔坑内有防御塔了
                        (*it)->plant=tower;
                        tower->draw();
                        }
                        else
                        {
                             dec_money(50);
                            (*it)->plant->baoza();
                        }
                        break;//进行了一次操作，可以直接退出循环了
                    }

                ++it;
            }
    }
    else if(mouseflag==3)//近狂暴
    {
        mouseflag=0;
        auto it=m_plantPositionList2.begin();
        while(it!=m_plantPositionList2.end())//遍历所有的精湛防御塔坑
            {


                    if((*it)->ContainPos(mpos) && (*it)->hasTower()&&kuangs>=1&&(*it)->plant&&(*it)->plant->kuang==0&&!((*it)->plant->qun==1&&(*it)->plant->bing==1) )//如果鼠标点击的位置在防御塔坑的范围内,并且没有防御塔
                    {
                        (*it)->plant->SetKuang(1);
                        (*it)->plant->SetDamage(30);
                        (*it)->plant->SetRate(500);
                        AddKuang(-1);

                        break;//进行了一次操作，可以直接退出循环了
                    }

                ++it;
            }
    }
    else if(mouseflag==4)//卸下狂暴
    {
        mouseflag=0;
        auto it=m_plantPositionList2.begin();
        while(it!=m_plantPositionList2.end())//遍历所有的精湛防御塔坑
            {


                    if((*it)->ContainPos(mpos) && (*it)->hasTower()&&(*it)->plant&&(*it)->plant->kuang==1 )//如果鼠标点击的位置在防御塔坑的范围内,并且没有防御塔
                    {
                        (*it)->plant->SetKuang(0);
                        (*it)->plant->SetDamage(15);
                        (*it)->plant->SetRate(1000);
                        AddKuang(1);

                        break;//进行了一次操作，可以直接退出循环了
                    }

                ++it;
            }
    }
    else if(mouseflag==5)//冰冻
    {
        mouseflag=0;
        auto it=m_plantPositionList2.begin();
        while(it!=m_plantPositionList2.end())//遍历所有的精湛防御塔坑
            {


                    if((*it)->ContainPos(mpos) && (*it)->hasTower()&&bings>=1&&(*it)->plant&&(*it)->plant->bing==0&&!((*it)->plant->qun==1&&(*it)->plant->kuang==1) )//如果鼠标点击的位置在防御塔坑的范围内,并且没有防御塔
                    {
                        (*it)->plant->SetBing(1);

                        AddBing(-1);

                        break;//进行了一次操作，可以直接退出循环了
                    }

                ++it;
            }
    }
    else if(mouseflag==6)//卸载冰冻
    {
        mouseflag=0;
        auto it=m_plantPositionList2.begin();
        while(it!=m_plantPositionList2.end())//遍历所有的精湛防御塔坑
            {


                    if((*it)->ContainPos(mpos) && (*it)->hasTower()&&(*it)->plant&&(*it)->plant->bing==1 )//如果鼠标点击的位置在防御塔坑的范围内,并且没有防御塔
                    {
                        (*it)->plant->SetBing(0);

                        AddBing(1);

                        break;//进行了一次操作，可以直接退出循环了
                    }

                ++it;
            }
    }
    else if(mouseflag==7)//群伤
    {
        mouseflag=0;
        auto it=m_plantPositionList2.begin();
        while(it!=m_plantPositionList2.end())//遍历所有的精湛防御塔坑
            {


                    if(quns>0&&(*it)->ContainPos(mpos) && (*it)->hasTower()&&(*it)->plant&&(*it)->plant->qun==0&&!((*it)->plant->bing==1&&(*it)->plant->kuang==1) )//如果鼠标点击的位置在防御塔坑的范围内,并且没有防御塔
                    {
                        (*it)->plant->SetQun(1);

                        AddQun(-1);

                        break;//进行了一次操作，可以直接退出循环了
                    }

                ++it;
            }
    }
    else if(mouseflag==8)//卸下群伤
    {
        mouseflag=0;
        auto it=m_plantPositionList2.begin();
        while(it!=m_plantPositionList2.end())//遍历所有的精湛防御塔坑
            {


                    if((*it)->ContainPos(mpos) && (*it)->hasTower()&&(*it)->plant&&(*it)->plant->qun==1 )//如果鼠标点击的位置在防御塔坑的范围内,并且没有防御塔
                    {
                        (*it)->plant->SetQun(0);

                        AddQun(1);

                        break;//进行了一次操作，可以直接退出循环了
                    }

                ++it;
            }

    }
    else if(mouseflag==9)//流血
    {
        mouseflag=0;
        auto it=m_plantPositionList1.begin();
        while(it!=m_plantPositionList1.end())//遍历所有的远程防御塔坑
            {


                    if(xies>0&&(*it)->ContainPos(mpos) && (*it)->hasTower()&&(*it)->plant&&(*it)->plant->xie==0 )//如果鼠标点击的位置在防御塔坑的范围内,并且没有防御塔
                    {
                        (*it)->plant->SetXie(1);

                        AddXie(- 1);

                        break;//进行了一次操作，可以直接退出循环了
                    }

                ++it;
            }
    }
    else if(mouseflag==10)//流血
    {
        mouseflag=0;
        auto it=m_plantPositionList1.begin();
        while(it!=m_plantPositionList1.end())//遍历所有的远程防御塔坑
            {


                    if((*it)->ContainPos(mpos) && (*it)->hasTower()&&(*it)->plant&&(*it)->plant->xie==1)//如果鼠标点击的位置在防御塔坑的范围内,并且没有防御塔
                    {
                        (*it)->plant->SetXie(0);

                        AddXie(1);

                        break;//进行了一次操作，可以直接退出循环了
                    }

                ++it;
            }
    }
    else if(mouseflag==11)//
    {
        mouseflag=0;
        auto it=m_plantPositionList1.begin();
        while(it!=m_plantPositionList1.end())//遍历所有的远程防御塔坑
            {


                    if((*it)->ContainPos(mpos) && (*it)->hasTower()&&(*it)->plant)//如果鼠标点击的位置在防御塔坑的范围内,并且没有防御塔
                    {
                        (*it)->plant->getRemoved();

                        break;//进行了一次操作，可以直接退出循环了
                    }

                ++it;
            }
        auto it1=m_plantPositionList2.begin();
        while(it1!=m_plantPositionList2.end())//遍历所有的远程防御塔坑
            {


                    if((*it1)->ContainPos(mpos) && (*it1)->hasTower()&&(*it1)->plant)//如果鼠标点击的位置在防御塔坑的范围内,并且没有防御塔
                    {
                        (*it1)->plant->getRemoved();
                        break;//进行了一次操作，可以直接退出循环了
                    }

                ++it1;
            }
    }
    else if(mouseflag==12)//炸弹
    {
        mouseflag=0;
        auto it=m_plantPositionList2.begin();
        while(it!=m_plantPositionList2.end())//遍历所有的近战防御塔坑
            {


                    if((*it)->ContainPos(mpos) && !(*it)->hasTower()&&(*it)->ice!=1&&Money>=150)//如果鼠标点击的位置在防御塔坑的范围内,并且没有防御塔
                    {

                        dec_money(150);
                        baozha((*it)->getCenterPos());

                        break;//进行了一次操作，可以直接退出循环了
                    }

                ++it;
            }
    }

    update();
}
void MainWindow::ShowPlant()
{
    foreach( Plant * tower, m_plantList)
            tower->draw();//画植物
}


void MainWindow::updateMon()
{

    foreach(Monster * enemy,MonsterVec)
        enemy->Move();
    foreach(Plant * tower,m_plantList)
            tower->checkEnemyInRange();
    foreach(Monster * mon,MonsterVec)
            mon->checkPlantInRange();


    update();

}


void MainWindow::MonAtt()
{
    foreach(Monster * mon,MonsterVec)
            mon->checkPlantInRange();
}
void MainWindow::ShowMon()//画怪
{QPainter painter(this);
    foreach(Monster * enemy,MonsterVec)
    {
        enemy->draw(painter);
        enemy->drawblood(painter);
    }
}

void MainWindow::removeEnemy(Monster *enemy)
{

    Q_ASSERT(enemy);
    MonsterVec.removeOne(enemy);//死亡的敌人从enemylist中移除
    delete enemy;
kills++;
qDebug()<<"死亡："<<kills<<endl;
if(kills==20)
    m_gameWin=1;
    /*if(MonsterVec.empty())
        {
            m_waves++;
        qDebug()<<m_waves;
            wavec=0;
            if(!loadWaves())
            {
                m_gameWin=true;
            }
        }*/
}
void MainWindow::addMon()
{
    //qsrand(QTime::currentTime().msec());	//设置种子，该种子作为qrand生成随机数的起始值，RAND_MAX为32767，即随机数在种子值到32767之间
int r_id=qrand()%5;
int id=1;
    if(r_id==0)//id==2的兵车僵尸
        id=2;
        Monster * enemy=new Monster(Path,begin.x()*150,begin.y()*150,id,this,this);//创建一个新得enemy
    int jipao=qrand()%5;
    int shan=qrand()%5;
    int xiedai=qrand()%5;
//qDebug()<<jipao<<" "<<shan<<endl;


    if(jipao<=1&&id==1)
        enemy->SetSpeed();
    if(shan<=1&&id==1)
        {enemy->SetShan(1);enemy->SetCD(1);}

    if(xiedai<1)//携带狂暴
        enemy->SetK(1);
    else if(xiedai==1)//携带冰冻
        enemy->SetB(1);
    else if(xiedai==2)//携带群伤
        enemy->SetQ(1);
    else
        enemy->SetX(1);
    MonsterVec.push_back(enemy);




    wavec++;
    if(wavec==5)//每波怪的个数
    {emit stop_appear();wavec=0;ChanMon=0;}
    //qDebug()<<MonsterVec.size();
}
QList<Monster*> MainWindow::getEnemyList()
{
    return MonsterVec;
}

void MainWindow::removeBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);
    m_bulletList.removeOne(bullet);

}
void MainWindow::addBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);
    m_bulletList.push_back(bullet);
}

QList<Plant*> MainWindow::getPlantList()
{
    return m_plantList;
}
void MainWindow::removePlant(Plant*p)
{
    Q_ASSERT(p);
    m_plantList.removeOne(p);//死亡的敌人从enemylist中移除
    delete p;
    //ShowPlant();
}
QList<PlantPos* >& MainWindow::getPlantPositionList2()
{
    return m_plantPositionList2;
}
QList<PlantPos* >& MainWindow::getPlantPositionList1()
{
    return m_plantPositionList1;
}

void MainWindow::dec_Hp()
{
    Hp-=20;
    if(Hp<0)
        m_gameLose=1;
    emit Hp_change("hp: "+QString::number(Hp));
}

void MainWindow::inc_money()
{
    Money+=50;
    emit Money_change("money: "+QString::number(Money));
}

void MainWindow::dec_money(int a)
{
    Money-=a;
    emit Money_change("money: "+QString::number(Money));
}
void MainWindow::GameStart()
{
    player_start->stop();
    player_dadou->play();
    xunhuan->start(120000);
    timer2->start(3000);//产生怪物
    timer1->start(10);//怪物移动
    timer3->start(1000*20);//波束控制
    //timer->start(1);//实时跟新
    game_end->start(1000);//检查游戏是否结束

    //MonAttTime->start(10);

}


void MainWindow::ImportMap(FileSelect*fileselect)
{
fileselect->ui->lineEdit->clear();
fileselect->ui->textEdit->clear();
        fileselect->setWindowTitle("导入地图");
        fileselect->ui->ConfirmBtn->setText("确认导入");
        connect(fileselect->ui->ConfirmBtn,&QPushButton::clicked,[=](){
            QFile file(fileselect->GetFilename());
            file.open(QIODevice::ReadWrite);
            QTextStream stream(&file);
            if(copy_map(stream))
            {
               QMessageBox::information(fileselect,"提示","导入成功！");
               fileselect->hide();
            }
            else
            {
               QMessageBox::critical(fileselect,"提示","导入失败！");
               fileselect->hide();
            }
            file.close();
        });
        //fileselect->hide();
    //导入地图

}
bool MainWindow::copy_map(QTextStream &stream)
{
    QString temp;
    stream>>temp;
    if(temp!="size:")//尺寸的读入
        return 0;
    stream>>temp;//chang
    chang=temp.toInt();
    stream>>temp;
    if(temp!="*")
        return 0;
    stream>>temp;//kuan
    kuan=temp.toInt();


    stream>>temp;
    if(temp!="begin:")//起点坐标的读入
        return 0;
    stream>>temp;
    if(temp!="(")
        return 0;
    stream>>temp;
    int x=temp.toInt();
    stream>>temp;
    if(temp!=",")
        return 0;
    stream>>temp;
    int y=temp.toInt();
    stream>>temp;
    if(temp!=")")
        return 0;
    begin=QPoint(x,y);


    QVector<QPoint>P;
    stream>>temp;
    if(temp!="turning_point:")//拐点的读入
        return 0;
    stream>>temp;
    while(temp!="map:")
    {
        if(temp!="(")
            return 0;
        stream>>temp;
         x=temp.toInt();
        stream>>temp;
        if(temp!=",")
            return 0;
        stream>>temp;
         y=temp.toInt();
        stream>>temp;
        if(temp!=")")
            return 0;
        P.push_back(QPoint(x*150,y*150));
        stream>>temp;
    }
    Path=P;


    QList<PlantPos* > m1;//用来储存防御塔坑的list(远程)
    QList<PlantPos* > m2;//用来储存防御塔坑的list(近战)
    for(int i=0;i<kuan;i++)
        for(int j=0;j<chang;j++)
        {
            stream>>Map[i][j];
            if(Map[i][j]==1)

            {
                PlantPos*p=new PlantPos(QPoint(j*150,i*150));
                m1.push_back(p);
            }
            else if(Map[i][j]==0)
            {
                 PlantPos*p=new PlantPos(QPoint(j*150,i*150));
                m2.push_back(p);
            }

        }
    m_plantPositionList1=m1;
    m_plantPositionList2=m2;
    return 1;
}

void MainWindow::init_map()
{
    int Map_1[8][12] =
    {
        2,2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,1,1,1,1,1,1,2,2,2,
        2,2,2,1,0,0,0,0,1,1,1,1,
        1,1,1,1,0,1,1,0,1,1,0,0,
        0,0,0,0,0,1,1,0,0,0,0,1,
        1,1,1,1,1,1,1,1,1,1,1,1,

    };
    for(int i=0;i<kuan;i++)
        for(int j=0;j<chang;j++)
            Map[i][j]=Map_1[i][j];
}

void MainWindow::ExportMap(FileSelect*fileselect)
{
    fileselect->ui->lineEdit->clear();
    fileselect->ui->textEdit->clear();
    fileselect->setWindowTitle("导出地图");
       fileselect->ui->ConfirmBtn->setText("确认导出");
       fileselect->ui->textEdit->setText(writeout_map());
       connect(fileselect->ui->ConfirmBtn,&QPushButton::clicked,[=](){
           //没有打开新文件进行修改,就把TextEdit里的内容导出到NewMap.txt
           QString filename=fileselect->ui->lineEdit->displayText();
           if(filename==""){
               QFile file("NewMap.txt");
               file.open(QIODevice::ReadWrite);
               QString temp=fileselect->ui->textEdit->toPlainText();
               QByteArray array =temp.toLatin1();
               file.write(array);
           }
           //如果用户自己输入了文件路径filename，则导出到filename里
           else{
               QFile file(filename);
               file.open(QIODevice::WriteOnly);
               QString temp=fileselect->ui->textEdit->toPlainText();
               QByteArray array =temp.toLatin1();
               file.write(array);
           }
           QMessageBox::information(fileselect,"提示","导出成功！");
           fileselect->hide();
       });

}

QString MainWindow::writeout_map(){
    QString content;
    content+="size: "+QString::number(chang)+" * "+QString::number(kuan)+'\n';
    content+="begin: ( "+QString::number(begin.x())+" , "+QString::number(begin.y())+" )"+'\n';
    content+="turning_point: ";
    foreach(QPoint p,Path)
    {
        content+="( "+QString::number(p.x()/150)+" , "+QString::number(p.y()/150)+" ) ";
    }
    content+='\n';
    content+="map: \n";
    for(int i=0;i<kuan;i++)
    {
        for(int j=0;j<chang;j++)
            content+=QString::number(Map[i][j])+" ";
        content+='\n';
    }
    return content;
}//将map的基本信息都写入string
void MainWindow::stop_game()
{
    timer2->stop();//产生怪物
    timer1->stop();//怪物移动
    timer3->stop();//波束控制
    //timer->stop();//实时跟新
   // MonAttTime->stop();//怪物攻击频率
}

void MainWindow::clear_game()
{

    player_dadou->stop();
    xunhuan->stop();
    player_start->play();
for(auto&i:MonsterVec)
{
    removeEnemy(i);
}
for(auto &i:m_plantList)
{
    removePlant(i);
}
for(auto &i:m_bulletList)
{
    removeBullet(i);
}
    MonsterVec.clear();
    m_plantList.clear();
    kills=0;
    m_waves=0;
    wavec=0;
    m_bulletList.clear();
    Money=300;
    Hp=100;
    m_gameWin=0;
    m_gameLose=0;
    kuangs=0;
    bings=0;
    quns=0;
    xies=0;
    zanting=0;
    ChanMon=1;
    emit Money_change("money: "+QString::number(Money));
    emit Hp_change("hp: "+QString::number(Hp));

}
void MainWindow::GameOver(QString filename,StartScene *s)
{
    label_GameOver=new QLabel(this);
    QPixmap pix;
    pix.load(filename);
    label_GameOver->setGeometry(0,0,pix.width(),pix.height());
    label_GameOver->setPixmap(pix);
    label_GameOver->move((this->width()-pix.width())*0.5,-pix.height()+400);
    //动态显示
    animation=new QPropertyAnimation(label_GameOver,"geometry");
    animation->setDuration(1000);
    animation->setStartValue(QRect(label_GameOver->x(),label_GameOver->y(),label_GameOver->width(),label_GameOver->height()));
    animation->setEndValue(QRect(label_GameOver->x(),label_GameOver->y()+400,label_GameOver->width(),label_GameOver->height()));
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
    label_GameOver->show();
    QTimer* t=new QTimer(this);
    t->start(5000);
    connect(t,&QTimer::timeout,[=]()
    {

        clear_game();
        delete label_GameOver;
        label_GameOver=nullptr;
        hide();
        s->show();
        t->stop();
        delete t;

    });
}//游戏结束时的画面
void MainWindow::AddKuang(int n)
{
    kuangs+=n;
}
void MainWindow::AddBing(int b)
{
    bings+=b;
}
void MainWindow::AddQun(int q)
{
    quns+=q;
}
void MainWindow::AddXie(int x)
{
    xies+=x;
}

void MainWindow::ZanTing()
{

        zanting=1;
        foreach(Plant * tower,m_plantList)
            tower->stopAtt();
        foreach(Monster*m,MonsterVec)
            m->Zanting();
        timer1->stop();
        timer2->stop();
        timer3->stop();
        caidan->show();


}
void MainWindow::resume()
{
    zanting=0;
    foreach(Plant * tower,m_plantList)
        tower->jixu();
    foreach(Monster*m,MonsterVec)
        m->jixu();
    if(ChanMon)
        timer2->start(3000);//产生怪物
    timer1->start(10);//怪物移动
    if(m_waves<3)
        timer3->start(1000*20);//波束控制
    caidan->hide();
}
void MainWindow::quit()
{
    m_gameLose=1;
    caidan->hide();

}
void MainWindow::baozha(QPoint pos)
{
    QMovie *movie = new QMovie(":/pic/images/zha.gif");
    QMovie*movie2=new QMovie(":/pic/images/po.gif");
    QLabel*label=new QLabel(this);
    movie->start();
    movie2->start();
    label->setMovie(movie);
    label->setGeometry(pos.x()-150/2,pos.y()-150/2,150,150);
    label->setScaledContents(true);
    label->show();
    QTimer *bao=new QTimer(this);
    bao->start(1000);
    connect(bao,&QTimer::timeout,[=]()
    {
        bao->stop();
        foreach(Monster*mon,MonsterVec)
            if(collisionWithCircle(pos,150,mon->getPos(),50))
                mon->getDamaged(1000,0,0);
        label->setMovie(movie2);
        //label->setGeometry(pos.x()-150/2,pos.y()-150/2,150,150);
        //label->setScaledContents(true);
        //label->show();
        QTimer*houxu=new QTimer(this);
        houxu->start(500);
        connect(houxu,&QTimer::timeout,[=]()
        {
           houxu->stop();
           delete label;
           if(movie)
           {
               movie->stop();
               delete movie;
           }
           if(movie2)
           {
               movie2->stop();
               delete movie2;
           }

        });



    });


}
