#pragma once

#include "inc/physics/shape.hpp"
#include <vector>

const unsigned maxPolygonVertices = 255;

namespace phy {
class PolygonShape : public Shape {
    Vec2f centroid;
    std::vector<Vec2f> normals;
    float density;
public:
    std::vector<Vec2f> vertices;
    PolygonShape(float dens);
    /**
     * Create a convex polygon using the given local points.
     *
     * The number of points must be in the range [3, maxPolygonVertices]
     * @param vertices_ A list of points in local coordinates for all vertices.
     */
    void set(const std::vector<Vec2f> &vertices_);

    /**
     * Set the polygon to be an axis-aligned box.
     *
     * @param length A vector of half the desired length of x and y edges.
     */
    void setBox(const Vec2f &length);

    /**
     * Set the polygon to be a box with orientation.
     *
     * @param length A vector of half the desired lengths of x and y edges.
     * @param center The center of the box (local coordinates).
     * @param angle The orientation of the box in radians.
     */
    void setBox(const Vec2f &length, const Vec2f &center, float angle);
    virtual bool testPoint(const Transform &transform, const Vec2f &pos) const override;
    virtual void getAABB() const override;
    virtual MassProperties getMassProps() const override;
    virtual void print(std::ostream &out) const override;
private:
    Vec2f calculateCentroid() const;
    float calculateArea() const;
};
} /* namespace phy */
