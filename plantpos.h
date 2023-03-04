#ifndef PLANTPOS_H
#define PLANTPOS_H

#include <QSize>
#include <QPainter>
#include <QString>
#include<QTimer>
#include <QMainWindow>
#include<QLabel>
#include <QObject>
class Plant;
class MainWindow;
class PlantPos:public QObject
{
Q_OBJECT
public:
    PlantPos(QPoint pos);
    PlantPos();
    ~PlantPos(){};
    QPoint getPos();//得到防御塔坑的左上点

    bool ContainPos(QPoint pos);//判断pos点是否在防御塔坑的范围内

    void draw(QPainter * painter) const;
    QPoint getCenterPos();//得到防御塔坑的中心点
    bool hasTower();//判断该防御塔坑内有没有防御塔
    void setHasTower(bool hasTower=true);//设置是否有防御塔
      Plant*plant=nullptr;
      bool ice=0;
      void SetIce();
      QTimer*iceCD;
 private:
    QPoint m_pos;
    QString m_path;

    //MainWindow*m_game;

    bool m_hasTower;
    static const QSize m_fixedSize;//防御塔坑的固定大小
};



#endif // PLANTPOSITION_H
