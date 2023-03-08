#ifndef DATE_H
#define DATE_H

#include <ostream>
#include <ctime>

bool check_date_corectness(short day, short month, short year);

struct Date
{
	short Day;
	short Month;
	short Year;

	Date()
		:Day(), Month(), Year()
	{}

	Date(short day, short month, short year)
	{
		if (!check_date_corectness(day, month, year))
		{
			day = 0;
			month = 0;
			year = 0;
		}

		Day = day;
		Month = month;
		Year = year;
	}

	friend std::ostream& operator << (std::ostream& os, const Date& obj)
	{ return os << obj.Day << '.' << obj.Month << '.' << obj.Year; }
};

bool operator > (const Date& f, const Date& s)
{
	if (f.Year == s.Year)
	{
		if (f.Month == s.Month)
			return f.Day > s.Day;

		return f.Month > s.Month;
	}

	return f.Year > s.Year;
}

bool operator >= (const Date& f, const Date& s)
{
	if (f.Year == s.Year)
	{
		if (f.Month == s.Month)
			return f.Day >= s.Day;

		return f.Month >= s.Month;
	}

	return f.Year >= s.Year;
}

bool operator <= (const Date& f, const Date& s)
{
	if (f.Year == s.Year)
	{
		if (f.Month == s.Month)
			return f.Day <= s.Day;

		return f.Month <= s.Month;
	}

	return f.Year <= s.Year;
}

bool operator < (const Date& f, const Date& s)
{
	if (f.Year == s.Year)
	{
		if (f.Month == s.Month)
			return f.Day < s.Day;

		return f.Month < s.Month;
	}

	return f.Year < s.Year;
}

bool operator == (const Date& f, const Date& s)
{
	if (f.Year == s.Year)
	{
		if (f.Month == s.Month)
			return f.Day == s.Day;

		return 0;
	}

	return 0;
}

bool operator != (const Date& f, const Date& s)
{
	if (f.Year == s.Year)
	{
		if (f.Month == s.Month)
			return f.Day != s.Day;

		return 1;
	}

	return 1;
}

bool check_date_corectness(short day, short month, short year)
{
	if (day < 0 || month < 0 || month > 12)
		return 0;

	switch (month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return day <= 31;
	case 4:
	case 6:
	case 9:
	case 11:
		return day <= 30;
	case 2:
		if (year % 4 == 0)
			return day <= 29;
		else
			return day <= 28;
	default:
		return 0;
	}
}

int get_days_difference(const Date& f, const Date& s)
{
	struct std::tm this_date = { 0, 0, 0, f.Month, f.Day, f.Year - 1900};
	struct std::tm other_date = { 0, 0, 0, s.Month, s.Day, s.Year - 1900 };

	std::time_t x = std::mktime(&this_date);
	std::time_t y = std::mktime(&other_date);

	if (f > s)
		return std::difftime(x, y) / (60 * 60 * 24);
	else
		return std::difftime(y, x) / (60 * 60 * 24);
}

Date string_to_date(std::string str)
{
	short day = 0, month = 0, year = 0;

	if (str.find('.') == std::string::npos)
		return Date(0, 0, 0);

	for (auto& i : str)
		if(!isdigit(i) && i != '.')
			return Date(0, 0, 0);

	day = stoi(str.substr(0, str.find('.') + 1));
	str.erase(0, str.find('.') + 1);

	if (str.find('.') == std::string::npos)
		Date(0, 0, 0);

	for (auto& i : str)
		if (!isdigit(i) && i != '.')
			return Date(0, 0, 0);

	month = stoi(str.substr(0, str.find('.') + 1));
	str.erase(0, str.find('.') + 1);

	for (auto& i : str)
		if (!isdigit(i))
			return Date(0, 0, 0);

	year = stoi(str);

	return Date(day, month, year);

}

#endif