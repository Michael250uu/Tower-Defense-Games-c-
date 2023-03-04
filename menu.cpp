#include "menu.h"
#include "ui_menu.h"
#include<QPainter>
#include<mainwindow.h>
menu::menu(QWidget *parent,MainWindow*game) :
    QMainWindow(parent),
    ui(new Ui::menu),
    m_game(game)
{
    setFixedSize(600, 750);
    ui->setupUi(this);
    setWindowTitle("Menu");
    ui->resume->setGeometry(200,300,200,50);
    ui->quit->setGeometry(200,400,200,50);
    connect(ui->resume,&QPushButton::clicked,[=](){
          m_game->resume();
       });
    connect(ui->quit,&QPushButton::clicked,[=](){
          m_game->quit();
       });
}

menu::~menu()
{
    delete ui;
}
void menu::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,600,750,QPixmap(":/pic/images/menu.jpg"));
}
