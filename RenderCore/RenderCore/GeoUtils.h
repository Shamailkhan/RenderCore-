#pragma once

#include <glm/glm.hpp>
#include <cmath>

#include "Shape.h"

// ============================================================
// GEO UTILS
// ============================================================
//
// Very small helper that maps latitude/longitude onto the same
// flat XY world plane the LineRenderer draws on. This is a local
// equirectangular (flat-earth) projection around a fixed origin,
// which is sufficient for a debug/drawing tool operating over a
// small area. It is NOT suitable for large-scale mapping.
//
// 1 world unit == 1 kilometer, so shapes stay in a sane numeric
// range for the OpenGL camera instead of using raw meters
// (a couple of degrees of longitude would otherwise put you
// hundreds of thousands of units away from the origin).
//
namespace GeoUtils
{
    constexpr double kOriginLat = 0.0;
    constexpr double kOriginLon = 0.0;

    constexpr double kKmPerDegreeLat = 110.574;
    constexpr double kKmPerDegreeLonAtEquator = 111.320;

    // ------------------------------------------------------------
    // Lat/Lon (degrees) -> world space (kilometers, Z = 0 plane)
    // ------------------------------------------------------------
    inline glm::vec3 LatLonToWorld(double lat, double lon)
    {
        const double kmPerDegreeLon =
            kKmPerDegreeLonAtEquator *
            std::cos(glm::radians(kOriginLat));

        const double x = (lon - kOriginLon) * kmPerDegreeLon;
        const double y = (lat - kOriginLat) * kKmPerDegreeLat;

        return glm::vec3(
            static_cast<float>(x),
            static_cast<float>(y),
            0.0f
        );
    }

    inline glm::vec3 LatLonToWorld(const GeoPoint& p)
    {
        return LatLonToWorld(p.lat, p.lon);
    }

    // ------------------------------------------------------------
    // World space (kilometers, Z = 0 plane) -> Lat/Lon (degrees)
    // ------------------------------------------------------------
    inline GeoPoint WorldToLatLon(const glm::vec3& world)
    {
        const double kmPerDegreeLon =
            kKmPerDegreeLonAtEquator *
            std::cos(glm::radians(kOriginLat));

        GeoPoint p;
        p.lat = kOriginLat + (static_cast<double>(world.y) / kKmPerDegreeLat);
        p.lon = kOriginLon + (static_cast<double>(world.x) / kmPerDegreeLon);
        return p;
    }
}
