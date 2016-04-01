#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    saveButton = new QPushButton("Save", this);
    corrButton = new QPushButton("Corr", this);
    browser = new QTextBrowser(this);
    browser->move(100,100);
    browser->resize(200, 300);
    corrButton->move(0, 100);
    resize(600, 600);
    QObject::connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveToFile);
    QObject::connect(corrButton, SIGNAL(clicked(bool)), SLOT(getVector()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::saveToFile()
{
    QImage image(QApplication::applicationDirPath() + "/Pics/2131.jpg");
    QImage mono = CorrelationModel::translateToGrayScale(image);
    mono.save(QApplication::applicationDirPath() + "/Pics/mono.jpg", "JPG");
}

void MainWindow::getVector()
{
    QImage workImage(QApplication::applicationDirPath() + "/Pics/test.bmp");
    QImage sampleImage(QApplication::applicationDirPath() + "/Pics/sample.bmp");

    CorrelationModel::calculateSampleMEandMSD(sampleImage);
    int width = CorrelationModel::evalCorr2D(workImage, sampleImage, data);

    browser->append(QString::number(width));

    QString str;

    for (auto& elem : data) {
        str.push_back(QString::number(elem) + " ");
    }

    browser->append(str);
    QPair<float, QPair<int, int>> pair = CorrelationModel::getMaxFromData(data);
    browser->append(QString::number(pair.first) + " x: " + QString::number(pair.second.first) + " y; " + QString::number(pair.second.second));
}
