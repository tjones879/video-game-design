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
    using ShapeList = std::vector<std::pair<phy::PolygonShape, phy::Transform>>;
    std::unique_ptr<ShapeList> shapes;

    RenderMessage() {}

    RenderMessage(std::unique_ptr<ShapeList>&& shapesList) {
        shapes = std::move(shapesList);
    }

    RenderMessage(RenderMessage &&other) {
        shapes = std::move(other.shapes);
    }

    virtual MessageType getType() const override {
        return MessageType::Render;
    }
};

struct InputMessage : Message {
    int command;

    virtual MessageType getType() const override {
        return MessageType::INVALID;
    }
};
