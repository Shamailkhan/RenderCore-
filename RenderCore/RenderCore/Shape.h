#pragma once

#include <glm/glm.hpp>
#include <vector>

#include <QColor>
#include <QString>

// ============================================================
// SHAPE TYPE
// ============================================================

enum class ShapeType
{
    Line,
    Polygon,
    Rectangle,
    Circle
};

inline const char* ShapeTypeToString(ShapeType type)
{
    switch (type)
    {
        case ShapeType::Line:      return "Line";
        case ShapeType::Polygon:   return "Polygon";
        case ShapeType::Rectangle: return "Rectangle";
        case ShapeType::Circle:    return "Circle";
    }
    return "Unknown";
}


// ============================================================
// GEO POINT (Latitude / Longitude)
// ============================================================

struct GeoPoint
{
    double lat = 0.0;
    double lon = 0.0;
};


// ============================================================
// SHAPE
// ============================================================
//
// A Shape is the CPU-side description of something the user
// has drawn (either interactively on the canvas, or entered
// via lat/long fields). It is converted into LineVertex data
// and pushed into the LineRenderer as a set of static lines.
//
struct Shape
{
    ShapeType type = ShapeType::Line;

    QString label;
    QColor  color = Qt::white;
    float   thickness = 2.0f;

    // World-space points that fully describe the shape.
    //
    //   Line      -> exactly 2 points (start, end)
    //   Rectangle -> exactly 2 points (opposite corners)
    //   Polygon   -> N points (>= 2), NOT closed automatically
    //                (closing segment is added when finalized)
    //   Circle    -> points[0] is the center, radius below is
    //                used to tessellate the circle outline
    //
    std::vector<glm::vec3> points;

    // Only meaningful for ShapeType::Circle.
    float radius = 0.0f;

    // Optional: the lat/long points this shape was created from,
    // if it was entered/derived from geographic coordinates.
    bool hasGeo = false;
    std::vector<GeoPoint> geoPoints;
};
