#pragma once

#include "core/event/Event.h"
#include "util/Math.h"

#include <stdint.h>
#include <sstream>

class WindowCloseEvent : public Event
{
public:
    EventClassType(WindowClose)
    EventClassFlag(EventFlagKeyboard)
};

class WindowResizeEvent : public Event
{
public:
    EventClassType(WindowResize)
    EventClassFlag(EventFlagWindow)

    WindowResizeEvent(
        uint32_t width, 
        uint32_t height
    ) 
    :   _width(width), 
        _height(height)
    {}

    uint32_t GetWidth() const { return _width; }
    uint32_t GetHeight() const { return _height; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowResizeEvent (width, height): " << _width << ", " << _height;
        return ss.str();
    }
private:
    uint32_t _width;
    uint32_t _height;
};

class WindowMovedEvent : public Event
{
public:
    EventClassType(WindowMoved)
    EventClassFlag(EventFlagWindow)

    WindowMovedEvent(Vec2 windowPos)
    :   _windowPos(windowPos) 
    {}

    Vec2 GetWindowPos() const { return _windowPos; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowMovedEvent (x, y): " << _windowPos.x << ", " << _windowPos.y;
        return ss.str();
    }
private:
    Vec2 _windowPos;
};

class WindowFocusEvent : public Event
{
public:
    EventClassType(WindowFocus)
    EventClassFlag(EventFlagWindow)

    WindowFocusEvent(bool focus)
    :   _focus(focus)
    {}

    bool IsFocused() const { return _focus; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowFocusEvent: " << _focus;
        return ss.str();
    }
private:
    bool _focus;
};
