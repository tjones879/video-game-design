#pragma once

struct Message;
struct RenderMessage;
struct InputMessage;
struct BodyCreatedMessage;
struct CreateBodyMessage;
struct DestroyBodyMessage;
struct CollisionMessage;
struct AudioMessage;

#include "inc/physics/polygon.hpp"
#include "inc/eventhandler.hpp"

enum class MessageType : char {
    Render,
    Input,
    BodyCreated,
    CreateBody,
    DestroyBody,
    Audio,
    Collision,
    INVALID
};

enum class CharacterType : char {
    Player,
    Spawner,
    Boundary,
    Enemy,
    Projectile,
    Unknown
};

struct Message {
    virtual MessageType getType() const {
        return MessageType::INVALID;
    }
};

struct RenderMessage : Message {
    template <typename T>
    using ShapeList = std::vector<std::tuple<T, phy::Transform, SDL_Color>>;
    std::unique_ptr<ShapeList<phy::PolygonShape>> polygons;
    std::unique_ptr<ShapeList<phy::CircleShape>> circles;

    RenderMessage() {}

    RenderMessage(std::unique_ptr<ShapeList<phy::PolygonShape>>&& polygonList,
                  std::unique_ptr<ShapeList<phy::CircleShape>>&& circleList) {
        polygons = std::move(polygonList);
        circles = std::move(circleList);
    }

    RenderMessage(RenderMessage &&other) {
        polygons = std::move(other.polygons);
        circles = std::move(other.circles);
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

struct BodyCreatedMessage : Message {
    std::weak_ptr<phy::Body> body;
    CharacterType type;

    BodyCreatedMessage() : type(CharacterType::Unknown) {}
    BodyCreatedMessage(std::weak_ptr<phy::Body> bod) : body(bod), type(CharacterType::Unknown) {}
    BodyCreatedMessage(std::weak_ptr<phy::Body> bod, CharacterType charType)
        : body(bod), type(charType) {}

    virtual MessageType getType() const override {
        return MessageType::BodyCreated;
    }
};

struct CreateBodyMessage : Message {
    CharacterType type;
    phy::BodySpec bodySpec;

    CreateBodyMessage() : type(CharacterType::Unknown) {}
    CreateBodyMessage(phy::BodySpec spec, CharacterType charType)
        : type(charType), bodySpec(spec) {}

    virtual MessageType getType() const override {
        return MessageType::CreateBody;
    }
};

struct DestroyBodyMessage : Message {
    std::weak_ptr<phy::Body> body;

    DestroyBodyMessage() {}
    DestroyBodyMessage(std::weak_ptr<phy::Body> bod) : body(bod) {}
    virtual MessageType getType() const override {
        return MessageType::DestroyBody;
    }
};

struct CollisionMessage : Message {
    std::vector<std::pair<std::weak_ptr<phy::Body>, std::weak_ptr<phy::Body>>> bodies;

    CollisionMessage() {}
    CollisionMessage(std::vector<std::pair<std::weak_ptr<phy::Body>,
                                           std::weak_ptr<phy::Body>>> collisions)
        : bodies(collisions) {}

    virtual MessageType getType() const override {
        return MessageType::Collision;
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
