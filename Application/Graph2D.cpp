#include "Graph2D.h"

Graph2D::Graph2D(QCustomPlot *canvas, const QString &title) noexcept
    : _canvas { canvas }, _title { title }
{
    _canvas->clearGraphs(); // Очищаем все графики, если они были
    _canvas->legend->setVisible(true);  // Включаем легенду графика
    _canvas->setInteractions(QCP :: iRangeDrag | QCP :: iRangeZoom); // Делаем график перетаскиваемым + масштабирование колеса прокрутки

    if (!title.isEmpty())
    {
        _canvas->plotLayout()->insertRow(0);    // Вставляем строку для создания оглавления графика
        _canvas->plotLayout()->addElement (0, 0, new QCPTextElement(_canvas, _title, QFont("sans", 12)));   // Добавляем оглавление
    }

    _canvas->xAxis->setRange(-10, 10);
    _canvas->yAxis->setRange(-10, 10);
}

void Graph2D::drawDefiningPolygon(const std::vector<QPointF> &controlPoints, const QString &name, const QColor &color, const Qt::PenStyle &penStyle, double width) const noexcept
{
    QCPCurve *shape = new QCPCurve(_canvas->xAxis, _canvas->yAxis);
    shape->setLineStyle(QCPCurve::lsNone);  // Убираем линии
    shape->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 6)); // Формируем вид точек
    QPen pen;
    pen.setStyle(penStyle);
    shape->setPen(color);
    pen.setWidth(width);
    shape->setPen(pen);
    shape->setName(name);

    for (const auto &controlPoint: controlPoints)
        shape->addData(controlPoint.x(), controlPoint.y());

    shape->setLineStyle(QCPCurve::lsLine); // Добавляем линии
    _canvas->replot();
}

void Graph2D::drawCurve(const Curve &curve, const QString &name, const QColor &color, const Qt::PenStyle &penStyle, double width) const noexcept
{
    drawCurve(curve.getCurvePoints(), name, color, penStyle, width);
}

void Graph2D::drawCurve(const std::vector<CurvePoint> &curvePoints, const QString &name, const QColor &color, const Qt::PenStyle &penStyle, double width) const noexcept
{
    QCPCurve *shape = new QCPCurve(_canvas->xAxis, _canvas->yAxis);
    QPen pen;
    pen.setStyle(penStyle);
    pen.setColor(color);
    pen.setWidth(width);
    shape->setPen(pen);
    shape->setName(name);

    for (const auto &curvePoint: curvePoints)
        shape->addData(curvePoint.point.x(), curvePoint.point.y());

    _canvas->replot();
}

void Graph2D::drawTwoCurves(const std::pair<std::vector<CurvePoint>, std::vector<CurvePoint>> &twoCurves) const noexcept
{
    drawCurve(twoCurves.first, "", Qt::PenStyle::DashLine);
    drawCurve(twoCurves.second, "", Qt::PenStyle::DashLine);
}

void Graph2D::drawHodographFirstDeriv(const Curve &curve, const QString &name, const QColor &color, const Qt::PenStyle &penStyle, double width) const noexcept
{
    QCPCurve *shape = new QCPCurve(_canvas->xAxis, _canvas->yAxis);
    QPen pen;
    pen.setColor(QColor(color));
    pen.setStyle(penStyle);
    pen.setWidthF(width);
    shape->setPen(pen);

    for (const auto &curvePoint: curve.getCurvePoints())
        shape->addData(curvePoint.firstDeriv.x() + curvePoint.point.x(), curvePoint.firstDeriv.y() + curvePoint.point.y()); // С приращением координат кривой

    shape->setName(name);
    _canvas->replot();
}

void Graph2D::drawHodograpSecondDeriv(const Curve &curve, const QString &name, const QColor &color, const Qt::PenStyle &penStyle, double width) const noexcept
{
    QCPCurve *shape = new QCPCurve(_canvas->xAxis, _canvas->yAxis);
    QPen pen;
    pen.setColor(QColor(color));
    pen.setStyle(penStyle);
    pen.setWidthF(width);
    shape->setPen(pen);

    for (const auto &curvePoint: curve.getCurvePoints())
        shape->addData(curvePoint.secondDeriv.x() + curvePoint.point.x(), curvePoint.secondDeriv.y() + curvePoint.point.y());  // С приращением координат кривой

    shape->setName(name);
    _canvas->replot();
}

void Graph2D::drawPoint(QPointF point, double width, const QColor &color) const noexcept
{
    _canvas->addGraph();
    _canvas->graph()->setPen(color);
    _canvas->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, width)); // Формируем вид точек
    _canvas->graph()->setLineStyle(QCPGraph::lsNone);
    _canvas->graph()->addData(point.x(), point.y());
    _canvas->legend->removeItem(_canvas->legend->itemCount() - 1); // Удаляем точку из легенды
    _canvas->replot();
}

void Graph2D::drawLine(QPointF point1, QPointF point2, const QColor &color, const Qt::PenStyle &penStyle, double width) const noexcept
{
    QCPItemLine *shape = new QCPItemLine(_canvas);
    QPen pen;
    pen.setStyle(Qt::PenStyle::DashLine);
    pen.setColor(color);
    pen.setWidth(width);
    pen.setStyle(penStyle);
    shape->setPen(pen);
    shape->start->setCoords(point1.x(), point1.y());
    shape->end->setCoords(point2.x(), point2.y());
    _canvas->replot();
}

void Graph2D::drawLine(double x1, double y1, double x2, double y2, const QColor &color, const Qt::PenStyle &penStyle, double width) const noexcept
{
    drawLine(QPointF(x1, y1), QPointF(x2, y2), color, penStyle, width);
}

void Graph2D::drawLable(QPointF point, const QString &label, double fontSize) const noexcept
{
    QCPItemText *shape = new QCPItemText(_canvas);
    shape->setFont(QFont("sans", fontSize));
    shape->position->setCoords(point.x(), point.y());
    shape->setText(label);
    _canvas->replot();
}

void Graph2D::drawArrow(QPointF point1, QPointF point2) const noexcept
{
    QCPItemLine *shape = new QCPItemLine(_canvas);
    shape->setHead(QCPLineEnding::esFlatArrow);
    shape->start->setCoords(point1.x(), point1.y());
    shape->end->setCoords(point2.x(), point2.y());
    _canvas->replot();
}

void Graph2D::drawDoubleArrow(QPointF point1, QPointF point2) const noexcept
{
    QCPItemLine *shape = new QCPItemLine(_canvas);
    shape->setHead(QCPLineEnding::esFlatArrow);
    shape->setTail(QCPLineEnding::esFlatArrow);
    shape->start->setCoords(point1.x(), point1.y());
    shape->end->setCoords(point2.x(), point2.y());
    _canvas->replot();
}

void Graph2D::drawTangentArrowToCurve(const CurvePoint &curvePoint) const noexcept
{
    QCPItemLine *shape = new QCPItemLine(_canvas);
    shape->setHead(QCPLineEnding::esFlatArrow);
    shape->start->setCoords(curvePoint.point.x(), curvePoint.point.y());
    shape->end->setCoords(curvePoint.firstDeriv.x() + curvePoint.point.x(), curvePoint.firstDeriv.y() + curvePoint.point.y());
    _canvas->replot();
}

void Graph2D::drawTangentToCurve(const CurvePoint &curvePoint, const QColor &color, double width) const noexcept
{
    QCPItemLine *shape = new QCPItemLine(_canvas);
    QPen pen;
    pen.setColor(color);
    pen.setWidth(width);
    shape->setPen(pen);
    shape->start->setCoords(curvePoint.point.x() - curvePoint.firstDeriv.x() / 10, curvePoint.point.y() - curvePoint.firstDeriv.y() / 10);
    shape->end->setCoords(curvePoint.point.x() + curvePoint.firstDeriv.x() / 10, curvePoint.point.y() + curvePoint.firstDeriv.y() / 10);
    _canvas->replot();
}

/*
void Graph2D::drawNormalsBetweenCurves(const Curve &curve1, const NURBS &curve2, const QColor &color) const noexcept
{
    for (size_t i = 0; i < curve2.getPointsNURBS().size(); i += 2)
    {
        CurvePoint nearestPoint = curve1.findPointNURBS(curve2.getPointsNURBS()[i].point);
        // Рисуем прямые между точками двумя кривымии (перпендикуляры)
        drawLine(Point(curve2.getPointsNURBS()[i].point.x, curve2.getPointsNURBS()[i].point.y), Point(nearestPoint.point.x, nearestPoint.point.y), color);
    }
}
*/