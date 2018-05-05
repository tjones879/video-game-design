#pragma once

struct Message;
struct RenderMessage;
struct InputMessage;
struct BodyMessage;
struct AudioMessage;

#include "inc/physics/polygon.hpp"
#include "inc/eventhandler.hpp"

enum class MessageType : char {
    Render,
    Input,
    Body,
    Audio,
    INVALID
};

struct Message {
    virtual MessageType getType() const {
        return MessageType::INVALID;
    }
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

/**
 * An input message should be sent from the event handler to
 * the physics engine to prompt any actions to be done.
 */
struct InputMessage : Message {
    std::unique_ptr<Command> command;

    InputMessage() {}
    InputMessage(std::unique_ptr<Command>&& cmd) : command(std::move(cmd)) {}

    virtual MessageType getType() const override {
        return MessageType::Input;
    }
};

struct BodyMessage : Message {
    std::weak_ptr<phy::Body> body;

    BodyMessage() {}
    BodyMessage(std::weak_ptr<phy::Body> bod) : body(bod) {}

    virtual MessageType getType() const override {
        return MessageType::Body;
    }
};

struct AudioMessage : Message {
    int posLeft;
    AudioMessage() {}
    AudioMessage(int left) : posLeft(left) {}

    virtual MessageType getType() const override {
        return MessageType::Audio;
    }
};
