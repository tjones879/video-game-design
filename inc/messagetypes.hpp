#pragma once

namespace 

enum class MessageType : char {
    Render,
    INVALID
};

struct Message {
    virtual MessageType getType() const = 0;
};

struct RenderMessage : Message {
    virtual MessageType getType() const override {
        return MessageType::Render;
    }
};
