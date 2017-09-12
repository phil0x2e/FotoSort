#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QtCore>
#include <QFileDialog>
#include <QGraphicsView>
#include<QMessageBox>
#include <QShortcut>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);

    //Hide Second Button and Menu Entry when no Folder is Specified
    ui->sortButton2->setVisible(false);
    ui->actionSet_Sort_Folder_2->setVisible(false);

    //Create Shortcuts for Buttons
    shortcutLeft = new QShortcut(QKeySequence("Left"), this);
    QObject::connect(shortcutLeft, SIGNAL(activated()), ui->leftButton, SLOT(animateClick()));
    shortcutRight = new QShortcut(QKeySequence("Right"), this);
    QObject::connect(shortcutRight, SIGNAL(activated()), ui->rightButton, SLOT(animateClick()));
    shortcutSort1 = new QShortcut(QKeySequence("1"), this);
    QObject::connect(shortcutSort1, SIGNAL(activated()), ui->sortButton, SLOT(animateClick()));
    shortcutSort2 = new QShortcut(QKeySequence("2"), this);
    QObject::connect(shortcutSort2, SIGNAL(activated()), ui->sortButton2, SLOT(animateClick()));

    //scene->addPixmap(QPixmap("/home/phil/Bilder/Nerd Meme/13087934_10154685346106686_380250794284446333_n.jpg"));
    //ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    files = QFileDialog::getOpenFileNames(this, tr("Open Images"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));   //Choose Files Dialog
    if(files.size()==0) //return if e.g. cancel is clicked
        return;
    scene->addPixmap(QPixmap(files.at(0))); //Add first pic to the scene
    currentPic = 0;
    ui->graphicsView->setScene(scene);
    QString tmpFile = files.at(currentPic);
    tmpFile = tmpFile.remove(0, tmpFile.lastIndexOf("/")+1);
    MainWindow::setWindowTitle("PicSort - " + tmpFile + " (" +  QString::number(currentPic+1) + "/" + QString::number(files.size()) + ")");
}

void MainWindow::on_actionSet_Sort_Folder_triggered()
{
    sortDir1 = QFileDialog::getExistingDirectory(this, tr("Open Directory to sort out to"), QDir::homePath());
    if(sortDir1 != "")
    {
        folder1Set = true;
        ui->actionSet_Sort_Folder_2->setVisible(true);//Make option for Button 2 Visible
        QString dir = sortDir1;
        int index = dir.lastIndexOf("/");
        dir = dir.remove(0, index+1);//Cut away path to file
        ui->sortButton->setText("Sort to: \"" + dir + "\"");
    }
}


void MainWindow::on_actionSet_Sort_Folder_2_triggered()
{
    sortDir2 = QFileDialog::getExistingDirectory(this, tr("Open Second Directory to sort out to"), QDir::homePath());
    if(sortDir2 != "")
    {
        folder2Set = true;
        QString dir = sortDir2;
        int index = dir.lastIndexOf("/");
        dir = dir.remove(0, index+1);//Cut away path to file
        ui->sortButton2->setText("Sort to: \"" + dir + "\"");
        ui->sortButton2->setVisible(true);
    }

}

void MainWindow::on_rightButton_clicked()
{
    if(files.size() == 0) return;   //return if no pic is selected
    currentPic += 1;
    if(currentPic >= files.size()) currentPic = 0;  //If last pic is reached go back to first
    scene->clear();
    scene->setSceneRect(QPixmap(files.at(currentPic)).rect());
    scene->addPixmap(QPixmap(files.at(currentPic)));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);    //Resize view back to normal after changing pic

    QString tmpFile = files.at(currentPic);
    tmpFile = tmpFile.remove(0, tmpFile.lastIndexOf("/")+1);
    MainWindow::setWindowTitle("PicSort - " + tmpFile + " (" +  QString::number(currentPic+1) + "/" + QString::number(files.size()) + ")");
}


void MainWindow::on_leftButton_clicked()
{
    if(files.size() == 0) return;
    currentPic -= 1;
    if(currentPic < 0) currentPic = files.size()-1;
    scene->clear();
    scene->setSceneRect(QPixmap(files.at(currentPic)).rect());
    scene->addPixmap(QPixmap(files.at(currentPic)));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);    //Resize view back to normal after changing pic

    ui->graphicsView->setScene(scene);
    QString tmpFile = files.at(currentPic);
    tmpFile = tmpFile.remove(0, tmpFile.lastIndexOf("/")+1);
    MainWindow::setWindowTitle("PicSort - " + tmpFile + " (" +  QString::number(currentPic+1) + "/" + QString::number(files.size()) + ")");
}

void MainWindow::on_sortButton_clicked()
{
    if(!folder1Set) return;   //If no Directory specified: return
    if(files.size() == 0) return;
    if(move==false) //If fotos just need to be copied
    {
        QString file = files.at(currentPic);
        int index = file.lastIndexOf("/");
        file = file.remove(0, index);//Cut away path to file
        qDebug() << "Current " << files.at(currentPic) << "TO: " << sortDir1+file;
        if(!QFile::copy(files.at(currentPic), sortDir1+file))
            QMessageBox::warning(this, "Warning File could not be copied", "File couldn't be copied, check if you have the rights to the directory and the file.");

    }else   //If fotos are moved
    {
      QString file = files.at(currentPic);
      int index = file.lastIndexOf("/");
      file = file.remove(0, index);
      if(!QFile::copy(files.at(currentPic), sortDir1+file))
      {
          QMessageBox::warning(this, "Warning File could not be copied", "File couldn't be moved, check if you have the rights to the directory and the file.");
          return;
      }
      QFile::remove(files.at(currentPic));
     files.removeAt(currentPic);
    }

}

void MainWindow::on_sortButton2_clicked()
{
    if(!folder2Set) return;   //If no Directory specified: return
    if(files.size() == 0) return;
    if(move==false) //If fotos just need to be copied
    {
        QString file = files.at(currentPic);
        int index = file.lastIndexOf("/");
        file = file.remove(0, index);//Cut away path to file
        if(!QFile::copy(files.at(currentPic), sortDir2+file))
            QMessageBox::warning(this, "Warning File could not be copied", "File couldn't be copied, check if you have the rights to the directory and the file.");

    }else   //If fotos are moved
    {
      QString file = files.at(currentPic);
      int index = file.lastIndexOf("/");
      file = file.remove(0, index);
      if(!QFile::copy(files.at(currentPic), sortDir2+file))
      {
          QMessageBox::warning(this, "Warning File could not be copied", "File couldn't be moved, check if you have the rights to the directory and the file.");
          return;
      }
      QFile::remove(files.at(currentPic));
     files.removeAt(currentPic);
    }

}

void MainWindow::on_actionMove_File_triggered()
{
    if(ui->actionMove_File->isChecked())    //Whether files should be copied or moved
        move = true;
    else
        move = false;
}




void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About PicSort", "Created by Phillippe Ketteniss\n\n Version 0.1\n2016");
}
