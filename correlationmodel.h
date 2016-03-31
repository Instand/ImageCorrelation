#ifndef CORRELATIONMODEL_H
#define CORRELATIONMODEL_H

#include <QImage>
#include <QVector>
#include <QRgb>

//Мат ожидание - mathematical expectation (ME)
//СКО - mean square deviation (MSD)

//static realization
class CorrelationModel {
private:
    CorrelationModel();                     //не создавать объект класса! :)
    static float sampleME;                  //эталонное МО
    static float sampleMSD;                 //эталонное СКО
    static float correlationWidth;

    //вспомогательные функции
    static float calculateSampleME(const QImage& sample);
    static float calculateSampleMSD(const QImage& sample);
    static void zeroizeFields();

public:
    //преобразование изображения в монохромное
    static QImage translateToGrayScale(const QImage& image);

    //в вектор data записываются данные - т.е яркости пикселей. Необходимо передавать пустой вектора обязательно!
    static unsigned int evalCorr2D(const QImage& workImage, const QImage& sampleImage, QVector<float>& data);
};

#endif // CORRELATIONMODEL_H
