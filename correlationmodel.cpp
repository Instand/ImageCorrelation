#include "correlationmodel.h"

//private
CorrelationModel::CorrelationModel()
{

}

float CorrelationModel::calculateSampleME(const QImage &sample)
{
    float _sampleME = 0;

    //проходим по всем пикселям
    for (int row = 0; row < sample.height(); ++row) {
        const QRgb* line = reinterpret_cast<const QRgb*>(sample.scanLine(row));

        for (int lineIndex = 0; lineIndex < sample.width(); ++lineIndex, ++line) {
            _sampleME += qGreen(*line);
        }
    }

    return (_sampleME)/(sample.width() * sample.height());
}

float CorrelationModel::calculateSampleMSD(const QImage &sample)
{
    float _sampleMSD = 0;

    //проходим по всем пикселям
    for (int row = 0; row < sample.height(); ++row) {
        const QRgb* line = reinterpret_cast<const QRgb*>(sample.scanLine(row));

        for (int lineIndex = 0; lineIndex < sample.width(); ++lineIndex, ++line) {
            _sampleMSD += std::pow(qGreen(*line) - sampleME, 2.0f);
        }
    }

    return std::sqrt(_sampleMSD);
}

void CorrelationModel::zeroizeFields()
{
    correlationWidth = 0;
    sampleME = 0;
    sampleMSD = 0;
}

QImage CorrelationModel::translateToGrayScale(const QImage &image)
{
    QImage monoImage(image);

    for (int row = 0; row < monoImage.height(); ++row) {
        QRgb* line = reinterpret_cast<QRgb*>(monoImage.scanLine(row));
        for (int lineIndex = 0; lineIndex < monoImage.width(); ++lineIndex, ++line) {
            int Y = 0.3f * qRed(*line) + 0.59f * qGreen(*line) + 0.11f * qBlue(*line);
            *line = qRgb(Y, Y, Y);
        }
    }
    return monoImage;
}

QPair<float, QPair<int, int> > &&CorrelationModel::getMaxFromData(QVector<float> &data)
{
    QPair<float, QPair<int, int>> maxPair;
    QPair<int, int> maxIndex;
    auto iter = std::max_element(data.begin(), data.end());

    if (correlationWidth != 0) {

        int width = 0, height = 0;

        //ищем положение
        for (auto i = data.begin(); i!= data.end(); ++i) {

            if (i == iter) {
                height = width/correlationWidth;
                width -= height * correlationWidth;
                break;
            }

            ++width;
        }

        maxIndex = qMakePair(width, height);
    }

    maxPair = qMakePair(*iter, maxIndex);

    return std::move(maxPair);
}

QPair<float, QPair<int, int> > &&CorrelationModel::getMinFromData(QVector<float> &data)
{
    QPair<float, QPair<int, int>> minPair;
    QPair<int, int> minIndex;

    auto iter = std::min_element(data.begin(), data.end());

    if (correlationWidth != 0) {

        int width = 0, height = 0;

        //ищем положение
        for (auto i = data.begin(); i!= data.end(); ++i) {

            if (i == iter) {
                height = width/correlationWidth;
                width -= height * correlationWidth;
                break;
            }

            ++width;
        }

        minIndex = qMakePair(width, height);
    }

    minPair = qMakePair(*iter, minIndex);

    return std::move(minPair);
}

unsigned int CorrelationModel::evalCorr2D(const QImage& workImage, const QImage& sampleImage, QVector<float>& data)
{
    //базовая проверка
    if (workImage.size().isEmpty() || sampleImage.size().isEmpty()) {
        CorrelationModel::zeroizeFields();
        return 0;
    } else {

        if (!data.empty()) {
            data.clear();
        }

        //алгоритм вычисления элементов корреляционной матрицы
        int corMatrRowLength = workImage.height() - sampleImage.height();
        int corMatrColLength = workImage.width() - sampleImage.width();

        data.reserve(corMatrColLength * corMatrRowLength);

        //проходим всю высоту
        for (int corMatrRow = 0; corMatrRow < corMatrRowLength; ++corMatrRow) {

            //проходим всю длину
            for (int corMatrCol = 0; corMatrCol < corMatrColLength; ++corMatrCol) {
                //----------------------------------------------------------------------------------
                //положение строба

                //основные вычисления
                //----------------------------------------------------------------------------------------------------------------------
                //вычисляем i,j ME
                float correlationME = 0;
                float baseCorrelationME = 0;

                //сканирурем всю ширину
                int sampleFrameRowStop = corMatrRow + sampleImage.height();
                for (int sampleFrameRow = corMatrRow; sampleFrameRow < sampleFrameRowStop; ++sampleFrameRow) {

                        const QRgb* frameLine = reinterpret_cast<const QRgb*>(workImage.scanLine(sampleFrameRow));

                        int sampleFrameColStop = corMatrCol + sampleImage.width();

                        //cмещаем указатель на необходимое число пикселей
                        frameLine += corMatrCol;

                        //сканируем всю длину
                        for (int sampleFrameCol = corMatrCol; sampleFrameCol < sampleFrameColStop; ++sampleFrameCol, ++frameLine) {
                            //i,j
                            baseCorrelationME += qGreen(*frameLine);
                        }
                }

                correlationME = baseCorrelationME/(sampleImage.width() * sampleImage.height());

                //------------------------------------------------------------------------------------------------
                //вычисляем MSD
                float correlationMSD = 0;
                float baseCorrelationMSD = 0;

                //сканирурем всю ширину
                for (int sampleFrameRow = corMatrRow; sampleFrameRow < sampleFrameRowStop; ++sampleFrameRow) {

                        const QRgb* frameLine = reinterpret_cast<const QRgb*>(workImage.scanLine(sampleFrameRow));

                        int sampleFrameColStop = corMatrCol + sampleImage.width();

                        //cмещаем указатель на необходимое число пикселей
                        frameLine += corMatrCol;

                        //сканируем всю длину
                        for (int sampleFrameCol = corMatrCol; sampleFrameCol < sampleFrameColStop; ++sampleFrameCol, ++frameLine) {
                            //i,j
                            baseCorrelationMSD += std::pow(qGreen(*frameLine) - correlationME, 2.0f);
                        }
                }

                correlationMSD = std::sqrt(baseCorrelationMSD);
                //---------------------------------------------------------------------------------------------------
                //вычисляем корреляционный пиксел яркости
                float baseCorrelation = 0;
                float correlation = 0;

                //сканирурем всю ширину
                for (int sampleFrameRow = 0; sampleFrameRow < sampleImage.height(); ++sampleFrameRow) {

                        const QRgb* strobePix = reinterpret_cast<const QRgb*>(workImage.scanLine(sampleFrameRow + corMatrRow));

                        //проход по эталону
                        const QRgb* samplePix = reinterpret_cast<const QRgb*>(sampleImage.scanLine(sampleFrameRow));

                        strobePix += corMatrCol;
                        //сканируем всю длину
                        for (int sampleFrameCol = 0; sampleFrameCol < sampleImage.width(); ++sampleFrameCol, ++strobePix, ++samplePix) {

                            //i,j
                            float correlationPart = qGreen( *strobePix) - correlationME;
                            float samplePart = qGreen( *samplePix) - sampleME;

                            baseCorrelation += (correlationPart) * (samplePart);
                        }
                }

                //вычисляем конкретный корреляционный пиксел
                correlation = baseCorrelation/(0.01f +(correlationMSD * sampleMSD));
                data.push_back(correlation);
            }
        }
    }

    //возвращаем длину корреляционной матрицы
    correlationWidth = workImage.width() - sampleImage.width();
    return correlationWidth;
}

void CorrelationModel::calculateSampleMEandMSD(const QImage &image)
{
    sampleME = CorrelationModel::calculateSampleME(image);
    sampleMSD = CorrelationModel::calculateSampleMSD(image);
}


//статические поля
float CorrelationModel::sampleME = 0;
float CorrelationModel::sampleMSD = 0;
int CorrelationModel::correlationWidth = 0;


