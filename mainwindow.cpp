#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    saveButton = new QPushButton("Save", this);
    corrButton = new QPushButton("Corr", this);
    timeButton = new QPushButton("Time", this);
    browser = new QTextBrowser(this);
    label = new QLabel(this);
    browser->move(100,100);
    browser->resize(200, 300);
    corrButton->move(0, 100);
    timeButton->move(0, 150);
    label->move(0, 300);
    resize(600, 600);


    QObject::connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveToFile);
    QObject::connect(corrButton, SIGNAL(clicked(bool)), SLOT(getVector()));
    QObject::connect(timeButton, &QPushButton::clicked, this, &MainWindow::getVectorWithImage);
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
    auto timePoint = std::chrono::steady_clock::now();

    QImage workImage(QApplication::applicationDirPath() + "/Pics/test.bmp");
    QImage sampleImage(QApplication::applicationDirPath() + "/Pics/sample.bmp");

    CorrelationModel::calculateSampleMEandMSD(sampleImage);
    int width = CorrelationModel::evalCorr2D(workImage, sampleImage, data);

    std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timePoint);

    browser->append(QString::number(width));

    QString str;

    for (auto& elem : data) {
        str.push_back(QString::number(elem) + " ");
    }

    browser->append(str);
    QPair<float, QPair<int, int>> pair = std::move(CorrelationModel::getMaxFromData(data));
    browser->append(QString::number(pair.first) + " x: " + QString::number(pair.second.first) + " y; " + QString::number(pair.second.second));
    browser->append(QString::number(duration.count()));
}

void MainWindow::getVectorWithImage()
{
    //auto timePoint = std::chrono::steady_clock::now();
    QImage workImage(QApplication::applicationDirPath() + "/Pics/test.bmp");
    QImage sampleImage(QApplication::applicationDirPath() + "/Pics/sample.bmp");

    auto timePoint = std::chrono::steady_clock::now();
    CorrelationModel::calculateSampleMEandMSD(sampleImage);
    int width = CorrelationModel::evalCorr2D(workImage, sampleImage, data);

    QImage test = std::move(CorrelationModel::getImageFromData(data));
    //test.save(QApplication::applicationDirPath() + "/Pics/test_bmp.bmp", "BMP");
    label->setPixmap(QPixmap::fromImage(test));

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timePoint);

    browser->append(QString::number(duration.count()));
}
