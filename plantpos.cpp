#include "plantpos.h"

#include <QSize>
#include <QPainter>
#include <QPixmap>
#include<QTimer>
#include"mainwindow.h"
const QSize PlantPos::m_fixedSize(150,150);//设置图片的大小
PlantPos::PlantPos(){};
PlantPos::PlantPos(QPoint pos):
    m_pos(pos),



    m_hasTower(false)
{
    iceCD=new QTimer(this);

    connect(iceCD,&QTimer::timeout,[=]()
    {
        ice=0;
        iceCD->stop();


    });
}


bool PlantPos::hasTower()
{
    return m_hasTower;
}

void PlantPos::setHasTower(bool hasTower)
{
    m_hasTower=hasTower;
}

QPoint PlantPos::getCenterPos()
{
    QPoint tmp;
    tmp.setX(m_pos.x()+m_fixedSize.width()/2);
    tmp.setY(m_pos.y()+m_fixedSize.height()/2);
    return tmp;
}


QPoint PlantPos::getPos()
{
    return m_pos;
}

bool PlantPos::ContainPos(QPoint pos)
{
    bool xInHere=pos.x()>m_pos.x() && pos.x()<m_pos.x()+m_fixedSize.width();
    bool yInHere=pos.y()>m_pos.y() && pos.y()<m_pos.y()+m_fixedSize.height();
    return xInHere && yInHere;
}

void PlantPos::draw(QPainter *painter) const
{
    painter->drawPixmap(m_pos.x(),m_pos.y(),m_path);
}

void PlantPos::SetIce()
{
    ice=1;
    iceCD->start(10000);
}
