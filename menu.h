#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
class  MainWindow;
namespace Ui {
class menu;
}

class menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = nullptr,MainWindow*game=nullptr);
    ~menu();
    void paintEvent(QPaintEvent *);

private:
    Ui::menu *ui;
    MainWindow*m_game;
};

#endif // MENU_H
