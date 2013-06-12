#ifndef TIMER_HPP
#define TIMER_HPP

class CTimer
{
public:
	CTimer(float);
	
	void Start();
	bool Elapsed();
	void Reset();
	void Reset(float);

private:
	float StartTime;
	float Amount;
};

#endif
