#ifndef JSONPARCER_H
#define JSONPARCER_H


#include <QList>
#include <QPointF>
#include <QPolygonF>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

static qreal signedArea(const QList<QPointF> &points)
{
    if (points.size() < 3) return 0.0;

    qreal area = 0.0;
    for (int i = 0; i < points.size(); ++i) {
        const QPointF &p1 = points[i];
        const QPointF &p2 = points[(i + 1) % points.size()];
        area += p1.x() * p2.y() - p2.x() * p1.y();
    }
    return area * 0.5;
}

class ContourJsonLoader
{
public:
    static QList<QPointF> load(const QString &jsonPath)
    {
        QList<QPointF> vertices;

        QFile file(jsonPath);
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "ContourJsonLoader: Не открыть файл" << jsonPath;
            return vertices;
        }

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
        if (error.error != QJsonParseError::NoError) {
            qWarning() << "ContourJsonLoader: JSON ошибка" << error.errorString();
            return vertices;
        }

        if (!doc.isObject() || !doc.object().contains("points") || !doc.object()["points"].isArray()) {
            qWarning() << "ContourJsonLoader: Нет массива 'points'";
            return vertices;
        }

        QJsonArray arr = doc.object()["points"].toArray();
        vertices.reserve(arr.size());

        for (const QJsonValue &v : arr) {
            if (v.isArray() && v.toArray().size() >= 2) {
                QJsonArray p = v.toArray();
                vertices.append(QPointF(p[0].toDouble(), p[1].toDouble()));
            }
        }

        qreal area = signedArea(vertices);
        if (area < 0) {
            std::reverse(vertices.begin(), vertices.end());
            qDebug() << "ContourJsonLoader: Контур был по часовой → перевёрнут (теперь CCW)";
        } else if (area > 0) {
            qDebug() << "ContourJsonLoader: Контур уже против часовой — всё ок";
        } else {
            qWarning() << "ContourJsonLoader: Нулевая площадь — возможно, самопересечение или вырождение";
        }

        qDebug() << "Загружено вершин:" << vertices.size() << "| Ориентация: CCW (против часовой)";
        return vertices;
    }

    static QPolygonF loadPolygon(const QString &jsonPath)
    {
        return QPolygonF::fromList(load(jsonPath));
    }

    static QVector<QPointF> loadVector(const QString &jsonPath)
    {
        return load(jsonPath).toVector();
    }
};

inline QList<QPointF>   loadContourJson(const QString &path) { return ContourJsonLoader::load(path); }
inline QPolygonF        loadContourPolygon(const QString &path) { return ContourJsonLoader::loadPolygon(path); }

#endif // JSONPARCER_H
