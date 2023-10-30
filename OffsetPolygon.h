#include <QPainter>

#include "Enums.h"

class OffsetPolygon
{
    public:
        OffsetPolygon(const QVector<QPoint>& points = {}, const int& offset = 0);
        void paint(QSharedPointer<QPainter> painter, const Algorithm::Enum& type) const;
        void update(const QVector<QPoint>& points, const int& offset);
        inline bool empty() const { return m_polygons.empty(); }

    private:
        constexpr static QColor s_color = QColor(0, 0, 0, 255);
        const static uint s_thicc = 2;
        QVector<QVector<QPoint>> m_polygons;
};

QVector<QPair<QPoint, QPoint>> generateOffsetSegments(const QVector<QPoint>& points, const int& offset);

QVector<QPoint> calculateBaseIntersections(const QVector<QPair<QPoint, QPoint>>& segments);

QVector<QPair<QPoint, bool>> calculateAllIntersections(const QVector<QPoint>& baseIntersections);

QVector<QVector<QPoint>> removeNeedlessAndGetLoops(QVector<QPair<QPoint, bool>>& allIntersections);

QVector<QPoint> removeCollinearAndDoubles(const QVector<QPair<QPoint, bool>>& allIntersections);
