#pragma once
#include "../Components/ClockComponent.h"
#include "../Scenes/Restaurant.h"
#include <fstream>

class DayManager {
private:
	int day, maxDays;
	std::ifstream file;
	int dailyObjective;
	float clientFrequency, happinessFrequency, reputationDecrease, minimumThiefFrequency, maximumThiefFrequency;
	SoundEffect* timeUpSound;
	void readLine(std::string& line);
	static int to_int(std::string str);
	static float to_float(std::string str);
public:
	DayManager();
	~DayManager();
	void checkDayFinished();
	void nextDay();
	void setDay(int x);

	int getDay();
	int getDailyObjective();
	float getClientFrequency();
	float getHappinessFrequency();
	float getReputationDecrease();
	float getMinThiefFrequency();
	float getMaxThiefFrequency();
};