#ifndef SYSTEM_WORK_H
#define SYSTEM_WORK_H

#include <string>
#include <ctime>

#include "date.h"

Date current_utc_date()
{
	time_t now = time(0);
	#pragma warning(suppress : 4996)
	tm* time = gmtime(&now);

	return Date(time->tm_mday, time->tm_mon + 1, time->tm_year + 1900);
}

std::string str_current_utc_date()
{
	time_t now = time(0);
	#pragma warning(suppress : 4996)
	tm* time = gmtime(&now);

	return std::string((std::to_string(time->tm_mday) + '.' + std::to_string(time->tm_mon + 1) + '.' + std::to_string(time->tm_year + 1900)));
}

std::string str_current_utc_time()
{
	time_t now = time(0);
	#pragma warning(suppress : 4996)
	tm* time = gmtime(&now);

	return std::string((std::to_string(time->tm_hour) + ':' + std::to_string(time->tm_min)));
}

#endif 