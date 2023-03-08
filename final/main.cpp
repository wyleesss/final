#include <iostream>
#include <string>
#include <windows.h>
#include <memory>

#include "user_interface.h"
#include "processed_inputs.h"
#include "local_file_db.h"
#include "user.h"

using std::string;
using std::unique_ptr;
using std::cout;
using std::cin;
using std::endl;

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	unique_ptr<LocalFileDB> DBFile(new LocalFileDB("programm_data.txt"));
	unique_ptr<User> CurrentMainUser(new User());
	
	bool success = 0;
	bool run = 1;
	string buffer("");
	int user_choice = 0;

	do
	{
		do
		{
			system("CLS");

			cout << "вітаємо!\n\n"
				"введіть <1> для входу в існуючий аккаунт\n"
				"введіть <2> для реєстрації нового аккаунту\n"
				"введіть <0> для виходу з програми\n"
				"-> ";
			user_choice = stoi(processed_input_allowed(buffer, "120"));
		} while (user_choice != 1 && user_choice != 2 && user_choice != 0);

		switch (user_choice)
		{
		case 1:

			success = login(*CurrentMainUser, *DBFile);
			break;

		case 2:

			success = registration(*CurrentMainUser, *DBFile);
			break;

		default:

			run = !(success = exit_question());
			break;
		}
	}
	while(!success);

	while (run)
	{
		system("CLS");
		switch (main_menu())
		{
		case 1:

			system("CLS");
			add_account(*CurrentMainUser, *DBFile);
			break;

		case 2:

			system("CLS");
			if (CurrentMainUser->get_accounts_size() == 0)
				cout << "наразі немає жодного рахунку";
			else
				CurrentMainUser->print_accounts();
			cout << '\n';
			system("PAUSE");
			break;

		case 3:

			system("CLS");
			account_operation(*CurrentMainUser, *DBFile);

			cout << '\n';
			counter_sleep(3);

			break;

		case 4:

			buffer = DBFile->get_user_activities(*CurrentMainUser);

			system("CLS");
			cout << (buffer.empty() ? "пусто." : buffer) << '\n';
			system("PAUSE");

			buffer.clear();
			break;

		case 5:

			do
			{
				system("CLS");
				cout << "введіть:\n\n"
					<< "<1> - обчислити прибуток депозиту\n"
					<< "-> ";
				processed_input_allowed(buffer, "1234567890");
			} while (buffer != "1");

			income_calculator();
			break;

		default:
			run = !exit_question();
			break;
		}
	}

	system("CLS");
	cout << "програму завершено.";
	
}