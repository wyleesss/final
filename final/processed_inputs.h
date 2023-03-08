#ifndef PROCESSED_INPUTS_H
#define PROCESSED_INPUTS_H

#include <iostream>
#include <conio.h>

bool processed_input(std::string& str, bool restricted_input = 0)
{
	str.clear();

	char input_character = '\0';

	while (input_character = _getch())
	{
		if (input_character == 13)
		{
			if (!str.empty())
				break;
			else
				continue;
		}

		if (input_character == 27)
			return 0;

		if (input_character == 8)
		{
			if (!str.empty())
			{
				str.erase(str.end() - 1);
				std::cout << "\b \b";
				continue;
			}
		}

		if (restricted_input)
		{
			if (input_character == '#' || input_character == '~'
				|| input_character == '*' || input_character == ' '
				|| input_character == '$')
				continue;
		}

		std::cout << input_character;
		str += input_character;
	}

	return 1;
}

std::string& processed_input_allowed(std::string& str, const std::string& allowed_symbols)
{
	str.clear();

	char input_character = '\0';

	while (input_character = _getch())
	{
		if (input_character == 13)
		{
			if (!str.empty())
				break;
			else
				continue;
		}

		if (input_character == 8)
		{
			if (!str.empty())
			{
				str.erase(str.end() - 1);
				std::cout << "\b \b";
				continue;
			}
		}

		for (auto& i : allowed_symbols)
		{
			if (input_character == i)
			{
				std::cout << input_character;
				str += input_character;
			}
		}
	}

	return str;
}

#endif 