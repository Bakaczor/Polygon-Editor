#pragma once

#include "Polygon.h"

/*!
 * \brief The SceneManager class
 * This class manages the scene, with properties such as the image, painter and many more.
 */
class SceneManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isBuilding READ isBuilding NOTIFY isBuildingChanged)
    Q_PROPERTY(bool isDragging READ isDragging NOTIFY isDraggingChanged)
    Q_PROPERTY(Algorithm::Enum type READ type WRITE setType NOTIFY typeChanged)
    public:
        QSharedPointer<QPainter> painter;
        QSharedPointer<QImage> image;

        QPoint lastPosition;
        QSize size;

        QVector<Vertex> polyline;
        QVector<Polygon> polygons;

        Geometry currObject;
        int currVerIdx;
        int currEdgIdx;
        int currPolIdx;

        /*!
         * \brief Constructs a SceneManager object
         * \param parent The parent QObject
         */
        explicit SceneManager(QObject* parent = nullptr);

        /*!
         * \brief Paints the scene
         */
        void paint();

        /*!
         * \brief Checks if the user is building a polygon
         * \return True if the user is building a polygon, false otherwise
         */
        bool isBuilding() const;

        /*!
         * \brief Checks if the user is dragging an object
         * \return True if the user is dragging an object, false otherwise
         */
        bool isDragging() const;

        /*!
         * \brief Gets the type of line drawing algorithm
         * \return The type of the line drawing algorithm
         */
        Algorithm::Enum type() const;

    public slots:
        /*!
         * \brief Draws a projection of the next vertex and edge (while building)
         * \param x The x-coordinate of the next vertex
         * \param y The y-coordinate of the next vertex
         */
        void drawProjection(int x, int y);

        /*!
         * \brief Adds a vertex to the scene
         * \param x The x-coordinate of the vertex
         * \param y The y-coordinate of the vertex
         */
        void addVertex(int x, int y);

        /*!
         * \brief Cancels building of the polygon
         * \param x The x-coordinate of the stop point
         * \param y The y-coordinate of the stop point
         */
        void stopBuilding();

        /*!
         * \brief Checks which object in the scene was clicked
         * \param x The x-coordinate of the clicked point
         * \param y The y-coordinate of the clicked point
         */
        void checkObjects(int x, int y);

        /*!
         * \brief Chnages the state to dragging an object
         * \param x The x-coordinate of the start point
         * \param y The y-coordinate of the start point
         */
        void startDragging(int x, int y);

        /*!
         * \brief Moves an object in the scene
         * \param x The x-coordinate of the move point
         * \param y The y-coordinate of the move point
         */
        void moveObject(int x, int y);

        /*!
         * \brief Changes the state to not dragging an object
         * \param x The x-coordinate of the stop point
         * \param y The y-coordinate of the stop point
         */
        void stopDragging(int x, int y);

        /*!
         * \brief Inserts a vertex onto a polygon;s edge in the scene
         * \param x The x-coordinate of the vertex
         * \param y The y-coordinate of the vertex
         */
        void insertVertex(int x, int y);

        /*!
         * \brief Removes the selected object from the scene
         */
        void removeSelected();

        /*!
         * \brief Unselects all objects in the scene
         */
        void unselectObjects();

        /*!
         * \brief Sets the type of the line drawing algorithm
         * \param newType The new type of the line drawing algorithm
         */
        void setType(Algorithm::Enum newType);

        /*!
         * \brief Changes the orientation of a selected edge
         * \param orient The new orientation of the edge
         */
        void changeOrientation(Orientation::Enum orient);

        /*!
         * \brief Updates the offset of a selected polygon
         * \param offset The new offset for the polygon
         */
        void updateOffset(uint offset);

    signals:
        void imageChanged();
        void isBuildingChanged();
        void isDraggingChanged();
        void typeChanged();
        void edgeChanged(Orientation::Enum type);

    private:
        constexpr static QColor s_background = QColor(255, 255, 255, 255);

        bool m_isBuilding;
        bool m_isDragging;
        Algorithm::Enum m_type;
};
