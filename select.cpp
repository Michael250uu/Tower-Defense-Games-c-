#include"select.h"
Select::Select(){}
Select::Select(QPoint pos,QString str,int i):m_pos(pos),m_path(str),type(i){}
bool Select::ContainPos(QPoint pos)
{
    bool xInHere=pos.x()>m_pos.x() && pos.x()<m_pos.x()+150;
    bool yInHere=pos.y()>m_pos.y() && pos.y()<m_pos.y()+150;
    return xInHere && yInHere;
}
QString Select::getpath()
{
    return m_path;
}
int Select::gettype()
{
    return type;
}
