#pragma once

#include <chrono>
using namespace std::chrono;

class Timer
{
public:
	Timer();

	/// <summary></summary><returns>The time since last frame</returns>
	float GetDeltaTime();
	/// <summary>Once update has completed, store time of last frame (complete update)</summary>
	void Tick();
private:
	/// <summary>
	/// Using steady clock as in default casts to seconds, can track with higher fidelity and isn't system dependent.
	/// </summary>
	steady_clock::time_point lastFrame;
};

