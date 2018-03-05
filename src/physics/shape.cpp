#include "inc/physics/shape.hpp"

namespace phy {

ShapeSpec::ShapeSpec() : friction(0), density(0) {}

ShapeType Shape::getShapeType() const
{
    return shapeType;
}

std::ostream &operator<<(std::ostream &out, const Shape &shape)
{
    shape.print(out);
    return out;
}
} /* namespace phy */
