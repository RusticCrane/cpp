#include "Application.h"

#include <chrono>
#include <iostream>
#include <thread>


void Application::Start()
{
    bool running_ = true;

    //Timer fpsTimer;
	int frames = 0;

	//Timer frameTimer;

    while (running_)
    {
		//double deltaTime = frameTimer.Reset();

        //frames++;
        //if (fpsTimer.Elapsed() > 1.0)
        //{
        //    std::cout << static_cast<int>(frames / fpsTimer.Reset()) << std::endl;
        //    frames = 0;
        //}
        //
		//double frameTime = frameTimer.Elapsed();
        //std::this_thread::sleep_for(std::chrono::microseconds(16000 - static_cast<int>(frameTime * 10000000)));
    }
}
