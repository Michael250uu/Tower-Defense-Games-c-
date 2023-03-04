#include "startscene.h"
#include "ui_startscene.h"
#include<QPainter>
#include"mainwindow.h"
#include<QPushButton>
#include"fileselect.h"
StartScene::StartScene(QWidget *parent,MainWindow*game) :
    QMainWindow(parent),
    ui(new Ui::StartScene),
  m_game(game)
{
    //m_game->hide();
    ui->setupUi(this);
    setFixedSize(1950, 1350);

    //设置标题
    setWindowTitle("准备开始");
    // QPushButton *btn=new QPushButton(this);
    connect(ui->import_map,&QPushButton::clicked,this,[=](){
FileSelect *fileselect=new FileSelect(this);
       //hide();
        //m_game->show();
        //m_game->GameStart();

        fileselect->show();
        m_game->ImportMap(fileselect);
    });
    connect(ui->start,&QPushButton::clicked,this,[=](){

       //hide();
        //m_game->show();
        //m_game->GameStart();
        hide();
        m_game->show();
        m_game->GameStart();
    });
    connect(ui->export_map,&QPushButton::clicked,this,[=](){
FileSelect *fileselect=new FileSelect(this);
       //hide();
        //m_game->show();
        //m_game->GameStart();
        fileselect->show();
        m_game->ExportMap(fileselect);
    });


}

StartScene::~StartScene()
{
    delete ui;
}

void StartScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,1950,1350,QPixmap(":/pic/images/start.png"));
}
