#include "Metrics.h"
#include <QDebug>

double Metrics::calcCurveCurvature(const std::vector<CurvePoint> &curvePoints)
{
    QPointF secondDeriv;
    double square = 0, height = 0;  // Общая площадь и высота прямоугольника соответственно
    double split = curvePoints[1].parameter; // Основнаие прямоугольника

    for (int i = 0; i < curvePoints.size() - 1; ++i)
    {
        secondDeriv.setX(curvePoints[i].secondDeriv.x() * curvePoints[i].secondDeriv.x());
        secondDeriv.setY(curvePoints[i].secondDeriv.y() * curvePoints[i].secondDeriv.y());
        height = sqrt(secondDeriv.x() * secondDeriv.x() + secondDeriv.y() * secondDeriv.y());
        square += height * split;
    }

    return square;
}

double Metrics::calcHausdorffMetric(const Curve &Curve1, const Curve &Curve2)
{
    double distanceFromFirstCurve = findMaxLenBetweenCurves(Curve1, Curve2);    // Находим наибольшее расстояние от первой кривой ко второй кривой
    double distanceFromSecondCurve = findMaxLenBetweenCurves(Curve2, Curve1);   // Находим наибольшее расстояние от второй кривой к первой кривой

    return distanceFromFirstCurve > distanceFromSecondCurve ? distanceFromFirstCurve : distanceFromSecondCurve;
}

double Metrics::calcQuadraticDifference(const Curve &Curve1, const Curve &Curve2)
{
    double x = 0, y = 0;
    std::vector<CurvePoint> curvePoints1 = Curve1.getCurvePoints();
    std::vector<CurvePoint> curvePoints2 = Curve2.getCurvePoints();

    if (curvePoints1.size() != curvePoints2.size())
        qDebug() << "Error! calcQuadraticDifference: curvePoints1.size() != curvePoints2.size()!";

    for (size_t i = 0; i < curvePoints1.size(); ++i)
    {
        x += pow((curvePoints1[i].point.x() - curvePoints2[i].point.x()), 2);
        y += pow((curvePoints1[i].point.y() - curvePoints2[i].point.y()), 2);
    }

    return sqrt(x * x + y * y);
}
