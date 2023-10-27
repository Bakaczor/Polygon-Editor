#include <QQuickPaintedItem>

#include "Enums.h"

class Icon : public QQuickPaintedItem
{
    Q_OBJECT
    public:
        explicit Icon(QQuickItem* parent = nullptr);
        void paint(QPainter* painter) override;
        void setSource(Orientation::Enum type);

    private:
        QString m_source;
        explicit Icon(const Icon& newIcon) = delete;
        Icon& operator=(const Icon& newIcon) = delete;
};
