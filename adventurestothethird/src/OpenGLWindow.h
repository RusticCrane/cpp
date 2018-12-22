#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// TODO: thread safety for the context
class OpenGLWindow
{
public:
    /* Public methods */
    OpenGLWindow(int width, int height, const std::string& title);
    ~OpenGLWindow();

	static void Init();

    void PollEvents();
    void Swap();
private:
    /* Private members */
    GLFWwindow* m_window;
};
