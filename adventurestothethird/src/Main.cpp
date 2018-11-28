#include <glad/glad.h>
#include <SFML/Window.hpp>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Chunk.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char** argv)
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::Window window(sf::VideoMode(800, 600), "SFML works!", sf::Style::Default, settings);

    settings = window.getSettings();

    window.setActive(true);

    gladLoadGL();

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.1f, 0.4f, 0.0f);

    Shader shader("res/shaders/cube.vert.glsl", "res/shaders/cube.frag.glsl");
    unsigned int modelLocation = glGetUniformLocation(shader.getId(), "model");
    unsigned int viewLocation = glGetUniformLocation(shader.getId(), "view");
    unsigned int projectionLocation = glGetUniformLocation(shader.getId(), "projection");

    Camera cam;
    cam.position().x = 16.0f;
    cam.position().y = 20.0f;
    cam.position().z = 16.0f;

    glm::mat4 projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.01f, 400.0f);

    shader.Use();

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    sf::Vector2i prevMousePos = sf::Mouse::getPosition();

    Chunk chunk;

    constexpr float arrows[] = {
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // GREEN = X
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // RED = Y
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // BLUE = Zw                                                                     dwwwww
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    };

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(arrows), arrows, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader arrowShader("res/shaders/cross.vert.glsl", "res/shaders/cross.frag.glsl");

    sf::Clock frameTimer;
    sf::Clock fpsTimer;
    int frames = 0;

    while (window.isOpen())
    {
        auto time = frameTimer.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        frames++;
        if (fpsTimer.getElapsedTime().asMilliseconds() > 1000)
        {
            std::cout << frames * 1000 / fpsTimer.restart().asMilliseconds() << std::endl;
            frames = 0;
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition();
        cam.mouse(prevMousePos.x - mousePos.x, prevMousePos.y - mousePos.y);
        prevMousePos = mousePos;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            cam.moveLeft();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            cam.moveRight();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            cam.moveUp();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            cam.moveDown();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            cam.moveForward();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            cam.moveBackwards();
        }

        shader.Use();
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(cam.viewMatrix()));
        glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), cam.position().x, cam.position().y,
                    cam.position().z);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        chunk.render(shader.getId());

        arrowShader.Use();
        glm::mat4 mod = glm::translate(glm::mat4(1.0f), cam.center());
		mod = glm::scale(mod, glm::vec3(0.2f));
        glUniformMatrix4fv(glGetUniformLocation(arrowShader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mod));
        glUniformMatrix4fv(glGetUniformLocation(arrowShader.getId(), "view"), 1, GL_FALSE, glm::value_ptr(cam.viewMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(arrowShader.getId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(vao);
        glDrawArrays(GL_LINES, 0, 6);

        window.display();
    }
    return 0;
}
