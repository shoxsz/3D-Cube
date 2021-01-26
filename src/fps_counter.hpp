#ifndef _FPS_COUNTER_HPP_
#define _FPS_COUNTER_HPP_

#include <chrono>
#include <thread>

using namespace std::chrono;

#define CURRENT_TIME duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()

class FPSCounter{
public:

	FPSCounter() {}
	FPSCounter(unsigned int fps){ setFPS(fps); }

	void delay(){
		unsigned int current = CURRENT_TIME;
		unsigned int spent = current - startT;
		startT = current;

		calcFps(spent);
		applyDelay(spent);
	}

	bool shouldRender(){
		return excess <= 0;
	}

	void setFPS(unsigned int fps){ 
		this->fps = fps; 
		fpsDelay = 1000 / fps;
		startT = CURRENT_TIME;
	}

	unsigned int getFPS() const {
		return fps;
	}

	unsigned int getActualFPS() const {
		return actualFPS;
	}

	float getFrameDelta() const {
		return lastFrameDelta;
	}

private:
	
	void calcFps(unsigned int spent) {
		secCount += spent;
		frameCount++;

		if (secCount >= 1000) {
			actualFPS = frameCount;
			frameCount = 0;
			secCount = secCount - 1000;
		}
	}

	void applyDelay(unsigned int spent) {

		if (excess > 0) {
			excess -= fpsDelay;
			if (excess <= 0)
				excess = 0;
		}
		else {
			if (fpsDelay > spent) {
				std::this_thread::sleep_for(std::chrono::milliseconds(fpsDelay - spent));
			}
			else {
				excess += spent - fpsDelay;
			}
		}

		lastFrameDelta = spent / 1000.0f;
	}

	unsigned int startT;
	long int excess = 0;

	unsigned int fps = 0;
	unsigned int fpsDelay = 0;

	float lastFrameDelta = 0.0f;

	unsigned int secCount = 0;
	unsigned int frameCount = 0;
	unsigned int actualFPS = 0;
};

#endif