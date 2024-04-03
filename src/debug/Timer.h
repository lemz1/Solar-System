#pragma once

#include <chrono>
#include <iostream>

#ifdef DEBUG
#define PROFILE_FUNCTION(name) ScopedTimer profileTimer = ScopedTimer(name)
#else
#define PROFILE_FUNCTION(name) 
#endif

class Timer
{
public:
	Timer()
	{
		Reset();
	}

	void Reset()
	{
		m_StartTime = std::chrono::high_resolution_clock::now();
	}

	float ElapsedSeconds()
	{
		return ElapsedMilliseconds() * .001f;
	}

	float ElapsedMilliseconds()
	{
		return ElapsedMicroseconds() * .001f;
	}

	float ElapsedMicroseconds()
	{
		return ElapsedNanoseconds() * .001f;
	}

	float ElapsedNanoseconds()
	{
		return static_cast<float>(
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				std::chrono::high_resolution_clock::now() - m_StartTime
			).count()
		);
	}
;	private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
};

class ScopedTimer : public Timer
{
public:
	ScopedTimer(const char* name)
		: m_Name(name)
	{}

	~ScopedTimer()
	{
		std::cout << "[TIMER] " << m_Name << ": " 
				  << m_Timer.ElapsedMicroseconds() << "us" 
				  << "(" << m_Timer.ElapsedMilliseconds() << "ms)" 
				  << std::endl;
	}
private:
	const char* m_Name;
	Timer m_Timer;
};
