#pragma once
#include "GlobalTimer.h"
#include <string>
#include <chrono>
#include <map>
#include <vector>
#include <iostream>

void GlobalTimer::addStartTime(TIME_MEASURMENT timeFrame, std::string displayName)
{
	auto startTimer = std::chrono::high_resolution_clock::now();
	// adding registered id to correct timer list with TIME_MEASUREMENT
	m_beginTimerList.insert(std::make_pair(displayName, startTimer));
	if (timeFrame == TIME_MEASURMENT::Nano)
		m_finalTimerSheetNs.push_back(TIME_VECTOR_PAIR(displayName, 0));
	if (timeFrame == TIME_MEASURMENT::Milli)
		m_finalTimerSheetMs.push_back(TIME_VECTOR_PAIR(displayName, 0));
	if (timeFrame == TIME_MEASURMENT::Second)
		m_finalTimerSheetSec.push_back(TIME_VECTOR_PAIR(displayName, 0));
};

void GlobalTimer::addFinishTime(std::string id)
{
	//register id as finished
	auto finishTimer = std::chrono::high_resolution_clock::now();
	m_finishTimerList.insert(std::make_pair(id, finishTimer));
};

void GlobalTimer::printFinalTimeSheet()
{
	//iterate overall time sheets and print all finished timers 
	for (auto obj : m_finalTimerSheetSec)
	{
		auto differenceInTime = m_finishTimerList[obj.first] - m_beginTimerList[obj.first];
		obj.second = std::chrono::duration_cast<std::chrono::seconds>(differenceInTime).count();
		std::cout << obj.first << " : " << std::to_string(obj.second) << "sec to execute\n" << std::endl;
	}
	for (auto obj : m_finalTimerSheetMs)
	{
		auto differenceInTime = m_finishTimerList[obj.first] - m_beginTimerList[obj.first];
		obj.second = std::chrono::duration_cast<std::chrono::milliseconds>(differenceInTime).count();
		std::cout << obj.first << " : " << std::to_string(obj.second) << "ms to execute\n" << std::endl;
	}
	for (auto obj : m_finalTimerSheetNs)
	{
		auto differenceInTime = m_finishTimerList[obj.first] - m_beginTimerList[obj.first];
		obj.second = std::chrono::duration_cast<std::chrono::nanoseconds>(differenceInTime).count();
		std::cout << obj.first << std::to_string(obj.second) << "ns to execute/n" << std::endl;
	}
}
