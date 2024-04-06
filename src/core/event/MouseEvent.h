#pragma once

#include "core/event/Event.h"
#include "core/Input.h"
#include "util/Math.h"

#include <sstream>

class MouseMovedEvent : public Event
{
public:
    EventClassType(MouseMoved)
    EventClassFlag(EventFlagMouse)

    MouseMovedEvent(Vec2 mousePos) 
    :   _mousePos(mousePos)
    {}

    Vec2 GetMousePos() const { return _mousePos; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseMovedEvent (x, y): " << _mousePos.x << ", " << _mousePos.y;
        return ss.str();
    }
private:
    Vec2 _mousePos;
};

class MouseScrolledEvent : public Event
{
public:
    EventClassType(MouseScrolled)
    EventClassFlag(EventFlagMouse)

    MouseScrolledEvent(Vec2 scroll)
    :   _scroll(scroll)
    {}

    Vec2 GetScroll() const { return _scroll; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseScrolledEvent (x, y): " << _scroll.x << ", " << _scroll.y;
        return ss.str();
    }
private:
    Vec2 _scroll;
};

class MouseButtonPressedEvent : public Event
{
public:
    EventClassType(MouseButtonPressed)
    EventClassFlag(EventFlagMouse)

    MouseButtonPressedEvent(MouseCode button)
    : _button((MouseButton)button)
    {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << (MouseCode)_button;
        return ss.str();
    }

    MouseButton GetButton() const { return _button; }
private:
    MouseButton _button;
};

class MouseButtonReleasedEvent : public Event
{
public:
    EventClassType(MouseButtonReleased)
    EventClassFlag(EventFlagMouse)
    
    MouseButtonReleasedEvent(MouseCode button)
        : _button((MouseButton)button)
    {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << (MouseCode)_button;
        return ss.str();
    }

    MouseButton GetButton() const { return _button; }
private:
    MouseButton _button;
};
