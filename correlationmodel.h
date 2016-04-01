#ifndef CORRELATIONMODEL_H
#define CORRELATIONMODEL_H

#include <QImage>
#include <QVector>
#include <QRgb>
#include <QPair>
#include <algorithm>

//Мат ожидание - mathematical expectation (ME)
//СКО - mean square deviation (MSD)

//static realization
class CorrelationModel {
private:
    CorrelationModel();                     //не создавать объект класса! :)
    static float sampleME;                  //эталонное МО
    static float sampleMSD;                 //эталонное СКО
    static int correlationWidth;
    static int width;
    static int height;

    //вспомогательные функции
    static float calculateSampleME(const QImage& sample);
    static float calculateSampleMSD(const QImage& sample);
    static void zeroizeFields();

public:
    //преобразование изображения в монохромное
    static QImage translateToGrayScale(const QImage& image);

    //в вектор data записываются данные - т.е яркости пикселей
    static unsigned int evalCorr2D(const QImage& workImage, const QImage& sampleImage, QVector<float>& data);

    //разовое вычисление ME and MSD
    static void calculateSampleMEandMSD(const QImage& image);

    //нахождение максимума в векторе и возврат его i,j в изображении
    //first - max value, second - i,j
    static QPair<float, QPair<int, int>>&& getMaxFromData(QVector<float>& data);
    static QPair<float, QPair<int, int>>&& getMinFromData(QVector<float>& data);

    //сброка изображения из вектора
    static QImage&& getImageFromData(QVector<float>& data);

    //получение информации о размерах корреляционного изображения
    static QPair<int, int> getSize();

    //ручная установка размеров
    static void setSize(int _width, int _height);
    static void setSize(QPair<int, int> size);
};

#endif // CORRELATIONMODEL_H
