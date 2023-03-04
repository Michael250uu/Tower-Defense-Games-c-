#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QPainter>
#include <QMainWindow>
#include<QTimer>
#include <QMouseEvent>
#include<QLabel>
#include<QMovie>
#include"monster.h"
#include<QDebug>
#include"plantpos.h"
#include"plant.h"
#include"select.h"
#include"bullet.h"
#include<QPropertyAnimation>
#include<QMediaPlayer>
class StartScene;
class FileSelect;
class menu;
extern int blocksize;
namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void removeEnemy(Monster * enemy);//敌人死亡，在mainwindow中要移除
    void removeBullet(Bullet * bullet);
    void addBullet(Bullet * bullet);
    QList<Monster*> getEnemyList();
    QList<Plant*> getPlantList();
    QList<PlantPos* >& getPlantPositionList2();
    QList<PlantPos* >& getPlantPositionList1();
    void dec_Hp();
    void removePlant(Plant*);
    void GameStart();
    void ImportMap(FileSelect*fileselect);
    bool copy_map(QTextStream&);
    void init_map();
    void ExportMap(FileSelect*fileselect);
    QString writeout_map();
    void inc_money();
    void dec_money(int);
    void stop_game();//计时器关
    void clear_game();//清场
    void GameOver(QString,StartScene * );//结算动画。
    void AddKuang(int );
    void AddBing(int);
    void AddQun(int);
    void AddXie(int);
    void ZanTing();
    void resume();
    void quit();
    void baozha(QPoint);

protected:
    void mousePressEvent(QMouseEvent *);//鼠标点击类函数
    void mouseMoveEvent(QMouseEvent *);//实时鼠标位置
    void mouseReleaseEvent(QMouseEvent *event);//
private:
    Ui::MainWindow *ui;
    QList<Monster*> MonsterVec;           //怪物数组
    void DrawMapArr(QPainter&);
    void DrawMonster(QPainter&);            //画出怪物
    void ShowPlant();//画植物
    void ShowMon();//画怪
//void updateMap();//跟新怪移动
    void paintEvent(QPaintEvent *);         //绘图事件
    void loadTowerPosition1();//用来加载防御塔坑的函数
    void loadSelect();//加载选择卡槽
    QList<PlantPos* > m_plantPositionList1;//用来储存防御塔坑的list(远程)
    QList<PlantPos* > m_plantPositionList2;//用来储存防御塔坑的list(近战)
    QList<Select>m_selectList;
    QList<Plant*>m_plantList;//用来储存防御塔的list
    int mouseflag=0;//鼠标是否现实图片1远程，2近战
    QString mpic;//鼠标现实的图片位置
    QPoint mpos;//鼠标位置
    int m_waves;//波数
    bool m_gameWin=0;
    bool m_gameLose=0;
int kuangs=0;//狂暴个数
int bings=0;//冰冻个数
int quns=0;//群伤个数
int xies=0;//流血个数
    QVector<QPoint>Path;//怪物路径
    int wavec;//一波怪的数量
    QList<Bullet *> m_bulletList;//用来储存子弹的list
    int Money;
    int Hp;

    QTimer * timer2;//产生怪物
    QTimer * timer1;//跟新怪物位置（植物索敌，地图实时跟新）
    QTimer * timer3;//怪物波束
    //QTimer * timer;//实施跟新
    QTimer*game_end;//检查是否结束
    //QTimer*MonAttTime;//僵尸攻击频率
    int Map[20][20];//地图。

    int chang,kuan;
    QPoint begin;
    QLabel *label_GameOver;
    QPropertyAnimation*animation;
    int kills=0;//消失的敌人数
    QMediaPlayer*player_start;
    QMediaPlayer*player_dadou;
 bool zanting=0;
    menu*caidan;
    bool ChanMon=1;
    QTimer*xunhuan;//打斗音乐的循环
private slots:
    //私有信号槽，将Enemy，Tower和后续的Bullet连接
    void updateMon();//怪物移动
void MonAtt();//怪物攻击
    void addMon();

 signals:
    void stop_appear();
    void Hp_change(QString );
    void Money_change(QString);
    void Kuang_change(QString);
};

#endif // MAINWINDOW_H
