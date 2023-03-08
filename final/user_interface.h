#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <iostream>
#include <string>
#include <windows.h>

#include "processed_inputs.h"
#include "local_file_db.h"
#include "user.h"
#include "account.h"

bool check_email_correctness(const std::string& str)
{
	if (!isalpha(str[0]))
		return 0;

	int dot_it = -1;
	int at_it = -1;
	bool dot_is_there = 0;
	bool at_is_there = 0;

	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '.')
		{
			if (dot_is_there)
				return 0;

			dot_is_there = 1;
			dot_it = i;
		}

		else if (str[i] == '@')
		{
			if (at_is_there)
				return 0;

			at_is_there = 1;
			at_it = i;
		}
	}

	if (dot_it == -1 || at_it == -1
		|| at_it > dot_it 
		|| dot_it == str.size() - 1
		|| at_it - 1 == dot_it)
		return 0;

	return 1;
}

void counter_sleep(int seconds)
{
	while (seconds > 0)
	{
		std::cout << seconds--;
		Sleep(1000);
		std::cout << "\b \b";
	}
}

bool exit_question()
{
	int user_choice = 0;
	std::string buffer("");

	do
	{
		system("CLS");
		std::cout << "ви впевнен≥ що хочете вийти?\n\n"
			"<1> - так\n"
			"<0> - н≥\n"
			"-> ";
		user_choice = stoi(processed_input_allowed(buffer, "10"));
	} while (user_choice != 1 && user_choice != 0);

	return user_choice;
}

bool login(User& obj, const LocalFileDB& file)
{
	std::string login("");
	std::string password("");
	bool error = 0;

	do
	{
		system("CLS");

		if (!error)
			error = 1;
		else
			std::cout << "нев≥рний лог≥н/пароль!\n";

		std::cout << "<- назад (ESC)\n\n"
			"лог≥н:\n";
		if (!processed_input(login))
		{
			system("CLS");
			return 0;
		}

		std::cout << "\n\nпароль:\n";
		if (!processed_input(password))
		{
			system("CLS");
			return 0;
		}
	} while (!file.check_login_info(login, password));

	system("CLS");
	std::cout << "вх≥д виконано усп≥шно!\n";
	counter_sleep(3);

	file.read_user(login, obj);
	file.read_user_accounts(obj);

	return 1;
}

bool registration(User& obj, const LocalFileDB& file)
{
	std::string input("");
	std::string ua("…÷” ≈Ќ√Ўў«’ѓ‘≤¬јѕ–ќЋƒ∆™я„—ћ»“№Ѕёйцукенгшщзхњф≥вапролджЇ€чсмитьбю'");
	std::string eng("QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm");
	std::string num("1234567890");

	bool error = 0;

	do
	{
		system("CLS");
		
		if (!error)
			error = 1;
		else if (input.size() < 3)
			std::cout << "занадто короткий лог≥н! (м≥н. 3)\n";
		else 
			std::cout << "такий лог≥н вже ≥снуЇ!\n";

		std::cout << "<- назад (ESC)\n\n"
			"придумайте лог≥н:\n";
		if (!processed_input(input, 1))
		{
			system("CLS");
			return 0;
		}

	} while (!file.check_login_uniqueness(input) || input.size() < 3);
	error = 0;
	obj.set_login(input);

	do
	{
		system("CLS");

		if (!error)
			error = 1;
		else 
			std::cout << "нев≥рно вказане ≥м'€! (м≥н. 2 букви)\n";

		std::cout << "заповн≥ть своњ дан≥:\n\n"
			"≥м'€:\n";
		processed_input_allowed(input, ua + eng);

	} while (input.size() < 2);
	error = 0;
	obj.set_name(input);

	do
	{
		system("CLS");

		if (!error)
			error = 1;
		else 
			std::cout << "нев≥рно вказане пр≥звище! (м≥н. 2 букви)\n";

		std::cout << "заповн≥ть своњ дан≥:\n\n"
			"пр≥звище:\n";
		processed_input_allowed(input, ua + eng);

	} while (input.size() < 2);
	error = 0;
	obj.set_surname(input);

	do
	{
		system("CLS");

		if (!error)
			error = 1;
		else 
			std::cout << "нев≥рно вказана електронна пошта!\n";

		std::cout << "заповн≥ть своњ дан≥:\n\n"
			"електронна пошта:\n";
		processed_input_allowed(input, num + eng + '@' + '.');

	} while (!check_email_correctness(input));
	error = 0;
	obj.set_gmail(input);

	do
	{
		system("CLS");

		if (!error)
			error = 1;
		else 
			std::cout << "нев≥рно вказаний номер телефону!\n";

		std::cout << "заповн≥ть своњ дан≥:\n\n"
			"номер телефону:\n";
		processed_input_allowed(input, num);

	} while (input.size() != 9);
	error = 0;
	obj.set_phone_number(input);

	do
	{
		system("CLS");

		if (!error)
			error = 1;
		else
			std::cout << "занадто короткий пароль! (м≥н. 6)\n";

		std::cout << "придумайте пароль:\n";
		std::cin >> input;

	} while (input.size() < 6);
	obj.set_password(input);

	system("CLS");
	std::cout << "аккаунт зареЇтровано усп≥шно!\n";
	counter_sleep(3);

	file.write_user(obj);

	return 1;
}

int main_menu()
{
	int user_choice = 0;
	std::string buffer("");

	do
	{
		system("CLS");

		std::cout << "головне меню\n\n"
			<< "<1> - додати рахунок\n"
			<< "<2> - перегл€нути активн≥ рахунки\n"
			<< "<3> - керуванн€ рахунками\n"
			<< "<4> - журнал операц≥й\n"
			<< "<5> - ≥нструменти\n"
			<< "<0> - вих≥д\n"
			<< "-> ";
		user_choice = stoi(processed_input_allowed(buffer, "123450"));
	} while (user_choice != 1 && user_choice != 2 && user_choice != 3 
			&& user_choice != 4 && user_choice != 5 && user_choice != 0 );

	return user_choice;
}

void add_account(User& obj, const LocalFileDB& file)
{
	std::string ua("…÷” ≈Ќ√Ўў«’ѓ‘≤¬јѕ–ќЋƒ∆™я„—ћ»“№Ѕёйцукенгшщзхњф≥вапролджЇ€чсмитьбю'");
	std::string eng("QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm");
	std::string num("1234567890");
	std::string type_choice("");
	std::string buffer("");
	bool error = 0;
	short day = 0;
	short month = 0;
	short year = 0;
	Account* A;

	do
	{
		system("CLS");

		std::cout << "<- назад(ESC)\n"
			<< "обер≥ть тип рахунку:\n\n"
			<< "<1> - поточний\n"
			<< "<2> - депозитний\n"
			<< "<3> - кредитний\n"
			<< "-> ";

		if (!processed_input(type_choice))
		{
			system("CLS");
			return;
		}

	} while (type_choice != "1" && type_choice != "2" && type_choice != "3");

	if (type_choice == "1")
		A = new Account;
	else if (type_choice == "2")
		A = new DepositAccount;
	else
		A = new CreditAccount;

	system("CLS");
	std::cout << "введ≥ть номер рахунку:\n";
	processed_input_allowed(buffer, ua + eng + num);
	A->set_number(buffer);

	system("CLS");
	std::cout << "введ≥ть назву установи:\n";
	processed_input_allowed(buffer, ua + eng + num);
	A->set_bank_name(buffer);

	do
	{
		system("CLS");
		std::cout << "обер≥ть валюту:\n\n"
			<< "<1> - гривн€\n"
			<< "<2> - доллар\n"
			<< "<3> - Ївро\n"
			<< "-> ";
		processed_input_allowed(buffer, "123");
	} while (buffer != "1" && buffer != "2" && buffer != "3");
	A->set_currency_type(buffer == "1" ? Currency::UAH : buffer == "2" ? Currency::USD : Currency::EUR);

	system("CLS");
	std::cout << "введ≥ть поточний залишок:\n";
	processed_input_allowed(buffer, num);
	A->set_balance(stoll(buffer));

	do
	{
		system("CLS");
		std::cout << "бажаЇте додати номер карти?\n\n"
			<< "<1> - так\n"
			<< "<0> - н≥\n"
			<< "-> ";
		processed_input_allowed(buffer, "10");
	} while (buffer != "1" && buffer != "0");

	if (buffer == "1")
	{
		do
		{
			system("CLS");

			if (!error)
				error = 1;
			else
				std::cout << "нев≥рний номер карти!\n";

			std::cout << "введ≥ть номер карти:\n";
			processed_input_allowed(buffer, num);
			A->set_card_number(buffer);
		} while (buffer.size() != 16);
		error = 0;
	}

	switch (stoi(type_choice))
	{
	case 1:

		A->set_type("поточний");

		system("CLS");
		std::cout << "рахунок додано усп≥шно!\n";
		counter_sleep(3);
		file.write_default_account(obj.get_login(), *A);
		obj.add_account(std::move(A));
		break;

	case 2:

		do
		{
			system("CLS");
			std::cout << "обер≥ть тип депозиту:\n\n"
				<< "<1> - накопичувальний\n"
				<< "<2> - ощадний\n"
				<< "<3> - ун≥версальний\n"
				<< "-> ";
			processed_input_allowed(buffer, "123");
		} while (buffer != "1" && buffer != "2" && buffer != "3");
		dynamic_cast<DepositAccount*>(A)->set_deposit_type((buffer == "1"? DepositType::Accumulative
			                                          : buffer == "2" ? DepositType::Saving : DepositType::Universal));
		dynamic_cast<DepositAccount*>(A)->set_type((buffer == "1" ? "депозитний накопичувальний" :
			buffer == "2" ? "депозитний ощадний" : "депозитний ун≥версальний"));

		system("CLS");
		std::cout << "введ≥ть стартову суму депозиту:\n";
		processed_input_allowed(buffer, num);
		dynamic_cast<DepositAccount*>(A)->set_start_sum(stoll(buffer));

		system("CLS");
		std::cout << "введ≥ть в≥дсоток:\n";
		processed_input_allowed(buffer, num);
		dynamic_cast<DepositAccount*>(A)->set_percent(stoi(buffer));

		do
		{
			system("CLS");

			if (!error)
				error = 1;
			else
				std::cout << "дата вказано нев≥рно!\n";

			std::cout << "введ≥ть дату в≥дкритт€ депозиту:\n\n"
				<< "день -> ";
			processed_input_allowed(buffer, num);
			day = stoi(buffer);

			std::cout << "\nм≥с€ць -> ";
			processed_input_allowed(buffer, num);
			month = stoi(buffer);

			std::cout << "\nр≥к -> ";
			processed_input_allowed(buffer, num);
			year = stoi(buffer);
		} while (!check_date_corectness(day, month, year));
		dynamic_cast<DepositAccount*>(A)->set_begin_date(Date(day, month, year));
		error = 0;

		do
		{
			system("CLS");

			if (!error)
				error = 1;
			else
				std::cout << "дата вказано нев≥рно!\n";

			std::cout << "введ≥ть дату зак≥нченн€ депозиту:\n\n"
				<< "\nдень -> ";
			processed_input_allowed(buffer, num);
			day = stoi(buffer);

			std::cout << "\nм≥с€ць -> ";
			processed_input_allowed(buffer, num);
			month = stoi(buffer);

			std::cout << "\nр≥к -> ";
			processed_input_allowed(buffer, num);
			year = stoi(buffer);
		} while (!check_date_corectness(day, month, year)
			|| dynamic_cast<DepositAccount*>(A)->get_begin_date() >= Date(day, month, year));
		dynamic_cast<DepositAccount*>(A)->set_end_date(Date(day, month, year));
		dynamic_cast<DepositAccount*>(A)->date_synchronize_percents();

		system("CLS");
		std::cout << "рахунок додано усп≥шно!\n";
		counter_sleep(3);
		file.write_deposit_account(obj.get_login(), *dynamic_cast<DepositAccount*>(A));
		obj.add_account(std::move(A));
		break;

	case 3:

		A->set_type("кредитний");

		system("CLS");
		std::cout << "введ≥ть л≥м≥т:\n";
		processed_input_allowed(buffer, num);
		dynamic_cast<CreditAccount*>(A)->set_limit(stol(buffer));

		system("CLS");
		std::cout << "введ≥ть в≥дсоток:\n";
		processed_input_allowed(buffer, num);
		dynamic_cast<CreditAccount*>(A)->set_percent(stoi(buffer));

		do
		{
			system("CLS");

			if(!error)
				error = 1;
			else
				std::cout << "дата вказано нев≥рно!\n";

			std::cout << "введ≥ть дату в≥дкритт€ кредиту:\n\n"
				<< "день -> ";
			processed_input_allowed(buffer, num);
			day = stoi(buffer);

			std::cout << "\nм≥с€ць -> ";
			processed_input_allowed(buffer, num);
			month = stoi(buffer);

			std::cout << "\nр≥к -> ";
			processed_input_allowed(buffer, num);
			year = stoi(buffer);
		} while (!check_date_corectness(day, month, year));
		dynamic_cast<CreditAccount*>(A)->set_begin_date(Date(day, month, year));
		error = 0;

		do
		{
			system("CLS");

			if (!error)
				error = 1;
			else
				std::cout << "дата вказано нев≥рно!\n";

			std::cout << "введ≥ть дату зак≥нченн€ кредиту:\n\n"
				<< "день -> ";
			processed_input_allowed(buffer, num);
			day = stoi(buffer);

			std::cout << "\nм≥с€ць -> ";
			processed_input_allowed(buffer, num);
			month = stoi(buffer);

			std::cout << "\nр≥к -> ";
			processed_input_allowed(buffer, num);
			year = stoi(buffer);
		} while (!check_date_corectness(day, month, year) 
			|| dynamic_cast<CreditAccount*>(A)->get_begin_date() >= Date(day, month, year));
		dynamic_cast<CreditAccount*>(A)->set_end_date(Date(day, month, year));

		system("CLS");
		std::cout << "рахунок додано усп≥шно!\n";
		counter_sleep(3);
		file.write_credit_account(obj.get_login(), *dynamic_cast<CreditAccount*>(A));
		obj.add_account(std::move(A));
		break;

	default:
		break;
	}

}

void income_calculator()
{
	bool error = 0;
	std::string buffer("");
	long long sum = 0;
	short percent = 0;
	int day = 0;
	int month = 0;
	int year = 0;
	Date begin;
	Date end;

	system("CLS");
	std::cout << "введ≥ть початкову суму -> ";
	processed_input_allowed(buffer, "1234567890");
	sum = stoll(buffer);

	system("CLS");
	std::cout << "введ≥ть в≥дсотки -> ";
	processed_input_allowed(buffer, "1234567890");
	percent = stoi(buffer);

	do
	{
		system("CLS");

		if (!error)
			error = 1;
		else
			std::cout << "дата вказано нев≥рно!\n";

		std::cout << "введ≥ть дату в≥дкритт€ депозиту:\n\n"
			<< "день -> ";
		processed_input_allowed(buffer, "1234567890");
		day = stoi(buffer);

		std::cout << "\nм≥с€ць -> ";
		processed_input_allowed(buffer, "1234567890");
		month = stoi(buffer);

		std::cout << "\nр≥к -> ";
		processed_input_allowed(buffer, "1234567890");
		year = stoi(buffer);
	} while (!check_date_corectness(day, month, year));
	begin.Day = day;
	begin.Month = month;
	begin.Year = year;
	error = 0;

	do
	{
		system("CLS");

		if (!error)
			error = 1;
		else
			std::cout << "дата вказано нев≥рно!\n";

		std::cout << "введ≥ть дату зак≥нченн€ депозиту:\n\n"
			<< "\nдень -> ";
		processed_input_allowed(buffer, "1234567890");
		day = stoi(buffer);

		std::cout << "\nм≥с€ць -> ";
		processed_input_allowed(buffer, "1234567890");
		month = stoi(buffer);

		std::cout << "\nр≥к -> ";
		processed_input_allowed(buffer, "1234567890");
		year = stoi(buffer);
	} while (!check_date_corectness(day, month, year) || begin >= Date(day, month, year));
	end.Day = day;
	end.Month = month;
	end.Year = year;

	system("CLS");
	std::cout << "результат:\n";
	std::cout << calculate_income(sum, percent, begin, end) << '\n';
	system("PAUSE");
}

void account_operation(User& user, LocalFileDB& file)
{
	int id = 0;
	int user_choice = 0;
	long long sum = 0;
	std::string buffer("");

	if (user.get_accounts_size() == 0)
	{
		std::cout << "нараз≥ немаЇ жодного рахунку";
		return;
	}

	user.print_accounts();
	std::cout << "\nввед≥ть ≥днтиф≥катор -> ";
	processed_input_allowed(buffer, "1234567890");
	id = stoi(buffer) - 1;

	do
	{
		system("CLS");
		std::cout << "обер≥ть операц≥ю:\n\n"
			<< "<1> - надходженн€\n"
			<< "<2> - видаток\n"
			<< "<3> - продовженн€ терм≥ну д≥њ\n"
			<< "-> ";
		processed_input_allowed(buffer, "123");
	} while (buffer != "1" && buffer != "2" && buffer != "3");
	user_choice = stoi(buffer);

	system("CLS");

	if (user_choice == 1 || user_choice == 2)
	{
		system("CLS");
		std::cout << "введ≥ть суму:\n";
		processed_input_allowed(buffer, "1234567890");
		sum = stoll(buffer);
		system("CLS");

		if (user_choice == 1)
		{
			if (user.income(id, sum))
				file.write_income(user.get_login(), *user[id], sum);
			
		}
		else
		{
			if (user.expense(id, sum))
				file.write_expense(user.get_login(), *user[id], sum);
			
		}
	}
	else
	{
		bool error = 0;
		int day = 0;
		int month = 0;
		int year = 0;

		do
		{
			system("CLS");

			if (!error)
				error = 1;
			else
				std::cout << "дата вказано нев≥рно!\n";

			std::cout << "введ≥ть дату зак≥нченн€ депозиту:\n\n"
				<< "\nдень -> ";
			processed_input_allowed(buffer, "1234567890");
			day = stoi(buffer);

			std::cout << "\nм≥с€ць -> ";
			processed_input_allowed(buffer, "1234567890");
			month = stoi(buffer);

			std::cout << "\nр≥к -> ";
			processed_input_allowed(buffer, "1234567890");
			year = stoi(buffer);
		} while (!check_date_corectness(day, month, year));

		system("CLS");

		user.extend_end_date(id, Date(day, month, year));
	}
}

#endif