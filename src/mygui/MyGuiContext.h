#pragma once

#include <imgui/imgui.h>

class MyGuiContext 
{
public:
    MyGuiContext();
    ~MyGuiContext();

    void StartFrame();
    void EndFrame();
private:
};
