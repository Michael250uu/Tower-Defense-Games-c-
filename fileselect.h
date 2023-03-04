#ifndef FILESELECT_H
#define FILESELECT_H

#include <QMainWindow>
#include<QString>
namespace Ui {
class FileSelect;
}

class FileSelect : public QMainWindow
{
    Q_OBJECT

public:
    explicit FileSelect(QWidget *parent = nullptr);
    ~FileSelect();
    QString GetFilename();
 Ui::FileSelect *ui;
private:

    QString filename;
};

#endif // FILESELECT_H
