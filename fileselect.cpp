#include "fileselect.h"
#include "ui_fileselect.h"
#include<QFileDialog>
#include<QMediaPlayer>
FileSelect::FileSelect(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FileSelect)
{




    filename="";
    ui->setupUi(this);
    setAttribute(Qt::WidgetAttribute());
    //setWindowTitle("导入地图");
    connect(ui->SelectFileBtn,&QPushButton::clicked,[=](){
          filename=QFileDialog::getOpenFileName(this,"打开文件");
          ui->lineEdit->setText(filename);

          QFile file(filename);
          file.open(QIODevice::ReadWrite);
          QByteArray array=file.readAll();
          ui->textEdit->setText(array);
          file.close();
       });
}

FileSelect::~FileSelect()
{
    delete ui;
}

QString FileSelect::GetFilename()
{
    return filename;
}
