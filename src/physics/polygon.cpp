#include "inc/physics/polygon.hpp"
#include <algorithm>
#include <stack>

namespace phy {
PolygonShape::PolygonShape()
{

}

/**
 * Use triangulation to find the local center of the polygon.
 */
Vec2 PolygonShape::calculateCentroid() const
{
    Vec2 center, reference;
    float area = 0;

    const float inv3 = 1.0f / 3.0f;
    for (int i = 0; i < vertices.size(); i++) {
        Vec2 p1 = reference;
        Vec2 p2 = vertices[i];
        Vec2 p3 = vertices[(i + 1) % vertices.size()];

        Vec2 e1 = p2 - p1;
        Vec2 e2 = p3 - p1;
        float triangleArea = 0.5f * e2.cross(e1);
        area += triangleArea;

        center += (p1 + p2 + p3) * triangleArea * inv3;
    }

    center *= 1.0f / area;
    return center;
}

/**
 * Use the sum of cross products to calculate the area of the polygon.
 */
float PolygonShape::calculateArea() const
{
    int n = vertices.size();
    float area = 0.0f;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += vertices[j].cross(vertices[i]);
    }

    return area / 2.0f;
}

/**
 * Use Graham Scan to find the convex hull of the given points.
 *
 * @param vertices Vector of points with at least 3 elements.
 */
static std::vector<Vec2> convexHull(std::vector<Vec2> vertices)
{
    std::stack<Vec2> hull;

    auto ccw = [](Vec2 p1, Vec2 p2, Vec2 p3) {
        return (p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x);
    };

    // Find the element with the minimum x coordinate and swap to beginning
    auto minimum = std::min_element(std::begin(vertices), std::end(vertices),
            [](Vec2 p1, Vec2 p2) {
                return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
            });
    std::iter_swap(std::begin(vertices), minimum);

    // Sort the points by polar angle with vertices[0]
    // The sign of the cross product will tell us if a is to the left of/right of b
    std::sort(std::begin(vertices) + 1, std::end(vertices),
            [] (const Vec2 &a, const Vec2 &b) -> bool{
                return a.cross(b) > 0;
            });

    hull.push(vertices[0]);
    hull.push(vertices[1]);
    hull.push(vertices[2]);

    for (int i = 3; i < vertices.size(); i++) {
        Vec2 top = hull.top();
        hull.pop();
        while (ccw(hull.top(), top, vertices[i]) <= 0) {
            top = hull.top();
            hull.pop();
        }
        hull.push(top);
        hull.push(vertices[i]);
    }

    // Return a vector instead of a stack for easier use by the shape
    std::vector<Vec2> ret;
    while (!hull.empty()) {
        ret.push_back(hull.top());
        hull.pop();
    }
    return ret;
}

void PolygonShape::set(std::vector<Vec2> vertices_)
{
    if (vertices_.size() < 3)
        return;
    if (vertices_.size() > 255)
        return;

    vertices = convexHull(vertices_);

    for (int i = 0; i < vertices.size(); i++) {
        int i2 = (i + 1) % vertices.size();

        Vec2 edge = vertices[i2] - vertices[i];
        normals[i] = edge.cross(1.0f);
        normals[i].normalize();
    }

    centroid = calculateCentroid();
}

void PolygonShape::setBox(const Vec2 &length)
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

void PolygonShape::setBox(const Vec2 &length, const Vec2 &center, float angle)
{
    setBox(length);
    centroid = center;

    Transform transform(center, angle);
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i] = transform.translate(vertices[i]);
        normals[i] = transform.rotation.rotate(normals[i]);
    }
}

bool PolygonShape::testPoint(const Transform &transform, const Vec2 &pos) const
{
    Vec2 localPoint = transform.rotation.invRotate(pos - transform.position);

    for (int i = 0; i < vertices.size(); i++) {
        float dot = normals[i].dot(localPoint - vertices[i]);
        if (dot > 0.0f)
            return false;
    }
    return true;
}

void PolygonShape::getAABB() const
{

}

MassProperties PolygonShape::getMassProps() const
{
    MassProperties props;
    props.centroid = calculateCentroid();
    props.mass = density * calculateArea();
    // Sum the moment of inertia for each triangle in the polygon
    props.inertia = 0;
    return props;
}
} /* namespace phy */
