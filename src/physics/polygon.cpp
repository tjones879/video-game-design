#include "inc/physics/polygon.hpp"

namespace phy {
PolygonShape::PolygonShape()
{

}

void PolygonShape::Set(std::vector<Vec2> vertices)
{
    if (vertices.size() < 3)
        return;
    if (vertices.size() > maxPolygonVertices)
        return;

    // Use Graham Scan algorithm to create a convex hull
    /** Algorithmic steps
     * 1. let N = number of points
     *    let points[N+1] = array of points
     *    swap points[1] with the point of the lowest y-coordinate
     *    sort points by polar angle with points[1]
     * 2. let points[0] = points[N]
     * 3. let M = 1 (The number of points on the convex hull
     * 4. for i = 2 to N:
     *      while ccw(points[M-1], points[M], points[i]) <= 0:
     *        if M > 1:
     *          M -= 1
     *          continue
     *        else if i == N:
     *          break
     *        else
     *          i += 1
     *
     *      M += 1
     *      swap points[M] with points[i]
     */
    auto ccw = [](Vec2 p1, Vec2 p2, Vec2 p3) {
        return (p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x);
    };
    auto comparePoints = [](Vec2 p1, Vec2 p2) {
        return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
    };
    {
        int N = vertices.size();
        
    }
}

void PolygonShape::SetBox(const Vec2 &length)
{
    vertices = std::vector<Vec2>(4);
    normals = std::vector<Vec2>(4);

    vertices[0].setVec(-length.x, -length.y);
    vertices[1].setVec(length.x, -length.y);
    vertices[2].setVec(length.x, length.y);
    vertices[3].setVec(-length.x, length.y);
    normals[0].setVec(0, -1);
    normals[1].setVec(1, 0);
    normals[2].setVec(0, 1);
    normals[3].setVec(-1, 0);
    centroid.zeroOut();
}

void PolygonShape::SetBox(const Vec2 &length, const Vec2 &center, float angle)
{
    SetBox(length);
    centroid = center;

    Transform transform(center, angle);
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i] = transform.translate(vertices[i]);
        normals[i] = transform.rotation.rotate(normals[i]);
    }
}

bool PolygonShape::testPoint(const Transform &transform, const Vec2 &pos) const
{
    return true;
}

void PolygonShape::getAABB() const
{

}

MassProperties PolygonShape::getMassProps() const
{
    return MassProperties();
}
} /* namespace phy */
