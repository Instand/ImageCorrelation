#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QApplication>
#include <QTextBrowser>
#include <correlationmodel.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QPushButton* saveButton;
    QPushButton* corrButton;
    QTextBrowser* browser;
    QVector<float> data;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void saveToFile();
    void getVector();
};

#endif // MAINWINDOW_H
