#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QShortcut>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_actionOpen_triggered();

    void on_actionSet_Sort_Folder_triggered();

    void on_rightButton_clicked();

    void on_leftButton_clicked();

    void on_sortButton_clicked();

    void on_actionMove_File_triggered();

    void on_actionSet_Sort_Folder_2_triggered();

    void on_sortButton2_clicked();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QStringList files;
    QString sortDir1 = "";
    QString sortDir2 = "";
    int currentPic;
    bool move = false;
    QShortcut *shortcutLeft;
    QShortcut *shortcutRight;
    QShortcut *shortcutSort1;
    QShortcut *shortcutSort2;
    qreal scale;
    bool folder1Set = false;
    bool folder2Set = false;
};

#endif // MAINWINDOW_H
