#pragma once
#include <string>
#include <chrono>
#include <map>
#include <vector>
#include <iostream>


/*

class: GlboalTimer
Description: timer class to measure threading success

*/

/// timer enum
enum class TIME_MEASURMENT {
	Nano,
	Milli,
	Second
};

/// typedef to make types shorter
using TIME_REGISTER = std::map <std::string,std::chrono::time_point<std::chrono::steady_clock>>;
using TIME_VECTOR_PAIR = std::pair<std::string, long long>;
using TIME_VECTOR = std::vector<TIME_VECTOR_PAIR>;

class GlobalTimer
{
public:
	GlobalTimer() {};
	~GlobalTimer() {};
	/// add timer to register
	void addStartTime(TIME_MEASURMENT, std::string displayName);
	/// tell timer to finish for id
	void addFinishTime(std::string id);
	/// print all registered timrs set and finished
	void printFinalTimeSheet();

private:

	TIME_REGISTER m_beginTimerList;
	TIME_REGISTER m_finishTimerList;
	TIME_VECTOR m_finalTimerSheetSec;
	TIME_VECTOR m_finalTimerSheetMs;
	TIME_VECTOR m_finalTimerSheetNs;
};