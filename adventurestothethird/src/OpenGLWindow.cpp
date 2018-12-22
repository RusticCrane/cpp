#include "pch.h"

#include "OpenGLWindow.h"

OpenGLWindow::OpenGLWindow(int width, int height, const std::string& title)
{
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        throw "GLFW window could not be created";
    }
    //glfwSetWindowUserPointer(m_window, this);
    //glfwSetWindowPosCallback();
    //glfwSetWindowSizeCallback();
    //glfwSetWindowCloseCallback();
    //glfwSetWindowRefreshCallback();
    //glfwSetWindowFocusCallback();
    //glfwSetWindowIconifyCallback();
    //glfwSetFramebufferSizeCallback();

    //glfwSetKeyCallback();
    //glfwSetCharCallback();
    //glfwSetCharModsCallback();
    //glfwSetMouseButtonCallback();
    //glfwSetCursorPosCallback();
    //glfwSetCursorEnterCallback();
    //glfwSetScrollCallback();
    //glfwSetDropCallback();
    //glfwSetJoystickCallback();
}

OpenGLWindow::~OpenGLWindow()
{
    glfwDestroyWindow(m_window);
}

void OpenGLWindow::Init()
{
	if (!glfwInit())
	{
		throw "GLFW could not be initialised";
	}
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	GLFWwindow * offscreenContext = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    if (!offscreenContext)
    {
		throw "GLFW error";
    }
	glfwMakeContextCurrent(offscreenContext);
	if (!gladLoadGL())
	{
		throw "Could not load OpenGL";
	}
	glfwDestroyWindow(offscreenContext);
	glfwDefaultWindowHints();
}

void OpenGLWindow::PollEvents()
{
    glfwPollEvents();
}

void OpenGLWindow::Swap()
{
    glfwSwapBuffers(m_window);
}
