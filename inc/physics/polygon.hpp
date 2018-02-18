#pragma once

#include "inc/physics/shape.hpp"
#include <vector>

const unsigned maxPolygonVertices = 255;

namespace phy {
class PolygonShape : public Shape {
    Vec2 centroid;
    std::vector<Vec2> vertices;
    std::vector<Vec2> normals;
public:
    PolygonShape();
    /**
     * Create a convex polygon using the given local points.
     *
     * The number of points must be in the range [3, maxPolygonVertices]
     * @param points A list of points in local coordinates for all vertices.
     */
    void set(std::vector<Vec2> points);
    /**
     * Set the polygon to be an axis-aligned box.
     *
     * @param length A vector of half the desired length of x and y edges.
     */
    void setBox(const Vec2 &length);
    /**
     * Set the polygon to be a box with orientation.
     *
     * @param length A vector of half the desired lengths of x and y edges.
     * @param center The center of the box (local coordinates).
     * @param angle The orientation of the box in radians.
     */
    void setBox(const Vec2 &length, const Vec2 &center, float angle);
    virtual bool testPoint(const Transform &transform, const Vec2 &pos) const override;
    virtual void getAABB() const override;
    virtual MassProperties getMassProps() const override;
private:
    Vec2 calculateCentroid() const;
};
} /* namespace phy */
