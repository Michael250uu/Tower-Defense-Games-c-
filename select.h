#ifndef SELECT_H
#define SELECT_H
#include<QPoint>
#include<QString>
class Plant;
class Select
{
public:
    Select(QPoint pos,QString str,int i);//图片的路径
    Select();
    QPoint getPos();//得到防御塔坑的左上点
    QString getpath();
    int gettype();//1远程 2近战 3狂暴 4卸载狂暴
    bool ContainPos(QPoint pos);//判断pos点是否在防御塔坑的范围内

 private:
    QPoint m_pos;
    QString m_path;
    int type;


};
#endif // SELECT_H
