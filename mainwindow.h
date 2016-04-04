#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QApplication>
#include <QLabel>
#include <QTextBrowser>
#include <correlationmodel.h>
#include <chrono>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QPushButton* saveButton;
    QPushButton* corrButton;
    QPushButton* timeButton;
    QTextBrowser* browser;
    QVector<float> data;
    QLabel* label;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void saveToFile();
    void getVector();
    void getVectorWithImage();
};

#endif // MAINWINDOW_H
