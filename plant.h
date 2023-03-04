
#ifndef PLANT_H
#define PLANT_H

//#include "mainwindow.h"
#include <QObject>
#include <QPoint>
#include <QSize>
#include <QString>
#include<QLabel>
#include<QWidget>
#include <QMainWindow>
//#include"monster.h"
class MainWindow;
class QPainter;
class QWidget;
class Monster;
class Plant: public QLabel
{
    Q_OBJECT
public:
    Plant(QPoint pos,MainWindow * game,int type,QMainWindow*parent);
    ~Plant();
    Plant();

    void draw();//画出防御塔
    void drawblood(QPainter&);//画血条
    void lostSightOfEnemy();
    void chooseEnemyFromAttack(Monster *enemy);
    void attackEnemy();
    void checkEnemyInRange();//检查敌人是否在攻击范围内
    void targetKilled();
    QPoint getPos();
    void getAttacked(Monster*);
    void getDamaged(double);
    void getRemoved();
    void RemoveFromAttacker(Monster*);
    void SetKuang(bool);
    void SetDamage(int);
    void SetRate(int);
    bool kuang=0;
    bool bing=0;
    bool qun=0;
    bool xie=0;
    void SetBing(bool);
    void SetQun(bool);
    void SetXie(bool);
    void stopAtt();
    void jixu();
    void baoza();
private:
    QPoint m_pos;//防御塔的中心点
    QString m_path;//防御塔图片的路径
    int type;//1为远程，2为近战
    int m_attackRange;//攻击范围
    static const QSize m_fixedSize;//防御塔图片的固定大小
    MainWindow * m_game;//指向mainwindow的指针
    double m_currentHp     ;           //植物当前生命值
    double m_maxHp;//植物最大生命值

    Monster * m_chooseEnemy;//正在攻击的敌人
    QTimer * m_fireRateTime;
    int m_fireRate;//射速
    int m_damage;//防御塔的攻击力
    QList<Monster*>attackers;
    QMovie*m_movie;

private slots:
    void shootWeapon();//私有信号槽，实现和子弹类的连接
};

#endif // PLANT_H
