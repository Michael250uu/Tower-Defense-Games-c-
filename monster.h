#ifndef MONSTER_H
#define MONSTER_H
#include <QVector>
#include <QString>
#include<QPoint>
#include<QLabel>
#include"plant.h"
#include <QMainWindow>
//#include"mainwindow.h"
//怪物类
//class MainWindow;
class Monster:public QLabel
{
private:

    QVector<QPoint> Waypoint;  //存储怪物路径点数组
    double mx, my;                  //怪物坐标  左上角
    QString ImgPath;             //怪物图片路径
    int id;                      //怪物编号
    double m_currentHp     ;           //怪物当前生命值
    double m_maxHp;//怪物最大生命值
    double mspeed ;       //怪物移动速度
    //static int count;
    bool m_active;//是否可以移动
    MainWindow*m_game;
    Plant *m_choosePlant;
     QList<Plant *> m_attackerTowerList;//正在攻击该敌人的防御塔list
     QMovie*m_movie;
     bool SpeedUp=0;
     bool Shan=0;
     bool CD=0;
     bool kuang=0;//携带狂暴
     bool bing=0;//携带冰冻
     bool qun=0;//携带群伤
bool xie=0;//携带流血
     bool AttackAble=1;//攻击能力
     bool bingStatus=0;//冰冻状态
     bool bloodStatus=0;//流血状态
     QLabel*cdLabel=new QLabel(this);//闪现cd倒计时标签
     QTimer*cdTime;//闪现cd 倒计时
     double daojishi=5;
     QTimer*bingCD;
     QTimer*loseBlood;
     QTimer*BloodTime;
     QTimer*t;
     double m_damage;
public:
    //参数：路径点数组、路径点的个数、怪物初始坐标、怪物宽度、怪物图片路径
    Monster(QVector<QPoint> waypoint,  double x, double y, int fid,QMainWindow*parent,MainWindow*game);  //构造
    Monster();  //构造
    ~Monster();
    bool Move();            //怪物移动函数

    double GetX() const;       //获取横坐标左上
    double GetY() const;       //获取横坐标左上
    QPoint getPos();//中心位置
    QString GetImgPath() const; //获取图片路径
    int GetId() const;      //获取编号
    int GetHealth() const;  //获取生命值
    void SetHealth(int);    //设置生命值

    void draw(QPainter&);//画一个怪
    void drawblood(QPainter&);//画血条
    void getDamaged(int damage,bool bingbullet,bool xiebullet);//敌人被攻击受到伤害
    void getLostSight(Plant * tower);//敌人脱离tower的攻击范围
    void getAttacked(Plant *tower);
    void targetKilled();
    void checkPlantInRange();//检查敌人是否在攻击范围内
    void RemoveFromAttacker(Plant*);
    void SetSpeed();
    bool YouShan();
    void SetShan(bool);
    void ShanXian();
    void SetCD(bool);
    void SetK(bool);
    void SetB(bool);
    void SetQ(bool);
    void SetX(bool);
    void SetActive(bool);
    void SetAttAble(bool);
    void Zanting();
    void jixu();

private slots:
    void doActive();//私有信号槽，敌人是否可以移动
    void getRemoved();
bool Alive();//shi'fou'cun'huo


};

#endif // MONSTER_H
