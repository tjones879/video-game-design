#pragma once

#include "inc/physics/polygon.hpp"

enum class MessageType : char {
    Render,
    INVALID
};

struct Message {
    virtual MessageType getType() const = 0;
};

struct RenderMessage : Message {
    std::unique_ptr<std::vector<std::pair<phy::PolygonShape, phy::Transform>>> shapes;

    virtual MessageType getType() const override {
        return MessageType::Render;
    }
};
