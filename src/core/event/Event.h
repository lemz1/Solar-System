#pragma once

#include <stdint.h>
#include <functional>
#include <string>
#include <iostream>

#define Flag(x) (1 << x)

enum class EventType : uint32_t
{
    None = 0,
    WindowClose, WindowResize, WindowMoved, WindowFocus,
    KeyJustPressed, KeyPressed, KeyReleased,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
};

enum EventFlag : uint32_t
{
    None = 0,
    EventFlagWindow = Flag(1),
    EventFlagKeyboard = Flag(2),
    EventFlagMouse = Flag(3),
};

#define EventClassType(type) \
    static EventType GetStaticEventType() { return EventType::type; } \
    EventType GetEventType() const override { return GetStaticEventType(); } \
    const char* GetName() const override { return #type; }

#define EventClassFlag(flag) uint32_t GetFlag() const override { return flag; }

#define BindEventFunction(func) std::bind(&func, this, std::placeholders::_1)

class Event
{
public:
    virtual ~Event() = default;

    virtual EventType GetEventType() const = 0;
    virtual uint32_t GetFlag() const = 0;
    virtual const char* GetName() const = 0;
    virtual std::string ToString() const { return GetName(); }

    bool IsFlag(const EventFlag flag) { return GetFlag() & flag; }

    template<typename T, typename F>
    static bool Dispatch(
        const Event& event, 
        const F& func
    )
    {
        static_assert(std::is_base_of<Event, T>::value, "T must be a subclass of Event!");

        if (event.GetEventType() == T::GetStaticEventType())
        {
            func((const T&)event);
            return true;
        }
        return false;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Event& event) 
{
    return os << event.ToString();
}
