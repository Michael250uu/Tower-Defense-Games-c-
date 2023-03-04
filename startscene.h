#ifndef STARTSCENE_H
#define STARTSCENE_H
class MainWindow;
#include <QMainWindow>
class FileSelect;
namespace Ui {
class StartScene;
}

class StartScene : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartScene(QWidget *parent = nullptr,MainWindow*game=nullptr);
    ~StartScene();

private:
    Ui::StartScene *ui;
    void paintEvent(QPaintEvent*);
    MainWindow*m_game;
};

#endif // STARTSCENE_H
