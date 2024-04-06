#pragma once

#include "core/event/Event.h"
#include "core/Input.h"

#include <sstream>

class KeyJustPressedEvent : public Event
{
public:
    EventClassType(KeyJustPressed)
    EventClassFlag(EventFlagKeyboard)

    KeyJustPressedEvent(KeyCode keycode) 
    :   _key((Key)keycode)
    {}


    std::string ToString() const override 
    { 
        std::stringstream ss;
        ss << "KeyJustPressedEvent: " << (KeyCode)_key;
        return ss.str(); 
    }

    Key GetKeyCode() const { return _key; }  
private:
    Key _key;
};

class KeyPressedEvent : public Event
{
public:
    EventClassType(KeyPressed)
    EventClassFlag(EventFlagKeyboard)

    KeyPressedEvent(
        KeyCode keycode, 
        bool isRepeat = false
    )
    :   _key((Key)keycode), 
        _isRepeat(isRepeat)
    {}

    bool IsRepeat() { return _isRepeat; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << (KeyCode)_key << " (Repeat: " << _isRepeat << ")";
        return ss.str();
    }

    Key GetKeyCode() const { return _key; }  
private:
    Key _key;
    bool _isRepeat;
};

class KeyReleasedEvent : public Event
{
public:
    EventClassType(KeyReleased)
    EventClassFlag(EventFlagKeyboard)

    KeyReleasedEvent(KeyCode keycode)
    :   _key((Key)keycode)
    {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << (KeyCode)_key;
        return ss.str();
    }

    Key GetKeyCode() const { return _key; }
private:
    Key _key;
};
