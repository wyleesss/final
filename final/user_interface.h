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
		std::cout << "�� ������� �� ������ �����?\n\n"
			"<1> - ���\n"
			"<0> - �\n"
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
			std::cout << "������� ����/������!\n";

		std::cout << "<- ����� (ESC)\n\n"
			"����:\n";
		if (!processed_input(login))
		{
			system("CLS");
			return 0;
		}

		std::cout << "\n\n������:\n";
		if (!processed_input(password))
		{
			system("CLS");
			return 0;
		}
	} while (!file.check_login_info(login, password));

	system("CLS");
	std::cout << "���� �������� ������!\n";
	counter_sleep(3);

	file.read_user(login, obj);
	file.read_user_accounts(obj);

	return 1;
}

bool registration(User& obj, const LocalFileDB& file)
{
	std::string input("");
	std::string ua("����������կԲ�������ƪ����������������������������������������'");
	std::string eng("QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm");
	std::string num("1234567890");

	bool error = 0;

	do
	{
		system("CLS");
		
		if (!error)
			error = 1;
		else if (input.size() < 3)
			std::cout << "������� �������� ����! (��. 3)\n";
		else 
			std::cout << "����� ���� ��� ����!\n";

		std::cout << "<- ����� (ESC)\n\n"
			"���������� ����:\n";
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
			std::cout << "������ ������� ��'�! (��. 2 �����)\n";

		std::cout << "�������� ��� ���:\n\n"
			"��'�:\n";
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
			std::cout << "������ ������� �������! (��. 2 �����)\n";

		std::cout << "�������� ��� ���:\n\n"
			"�������:\n";
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
			std::cout << "������ ������� ���������� �����!\n";

		std::cout << "�������� ��� ���:\n\n"
			"���������� �����:\n";
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
			std::cout << "������ �������� ����� ��������!\n";

		std::cout << "�������� ��� ���:\n\n"
			"����� ��������:\n";
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
			std::cout << "������� �������� ������! (��. 6)\n";

		std::cout << "���������� ������:\n";
		std::cin >> input;

	} while (input.size() < 6);
	obj.set_password(input);

	system("CLS");
	std::cout << "������� ����������� ������!\n";
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

		std::cout << "������� ����\n\n"
			<< "<1> - ������ �������\n"
			<< "<2> - ����������� ������ �������\n"
			<< "<3> - ��������� ���������\n"
			<< "<4> - ������ ��������\n"
			<< "<5> - �����������\n"
			<< "<0> - �����\n"
			<< "-> ";
		user_choice = stoi(processed_input_allowed(buffer, "123450"));
	} while (user_choice != 1 && user_choice != 2 && user_choice != 3 
			&& user_choice != 4 && user_choice != 5 && user_choice != 0 );

	return user_choice;
}

void add_account(User& obj, const LocalFileDB& file)
{
	std::string ua("����������կԲ�������ƪ����������������������������������������'");
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

		std::cout << "<- �����(ESC)\n"
			<< "������ ��� �������:\n\n"
			<< "<1> - ��������\n"
			<< "<2> - ����������\n"
			<< "<3> - ���������\n"
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
	std::cout << "������ ����� �������:\n";
	processed_input_allowed(buffer, ua + eng + num);
	A->set_number(buffer);

	system("CLS");
	std::cout << "������ ����� ��������:\n";
	processed_input_allowed(buffer, ua + eng + num);
	A->set_bank_name(buffer);

	do
	{
		system("CLS");
		std::cout << "������ ������:\n\n"
			<< "<1> - ������\n"
			<< "<2> - ������\n"
			<< "<3> - ����\n"
			<< "-> ";
		processed_input_allowed(buffer, "123");
	} while (buffer != "1" && buffer != "2" && buffer != "3");
	A->set_currency_type(buffer == "1" ? Currency::UAH : buffer == "2" ? Currency::USD : Currency::EUR);

	system("CLS");
	std::cout << "������ �������� �������:\n";
	processed_input_allowed(buffer, num);
	A->set_balance(stoll(buffer));

	do
	{
		system("CLS");
		std::cout << "������ ������ ����� �����?\n\n"
			<< "<1> - ���\n"
			<< "<0> - �\n"
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
				std::cout << "������� ����� �����!\n";

			std::cout << "������ ����� �����:\n";
			processed_input_allowed(buffer, num);
			A->set_card_number(buffer);
		} while (buffer.size() != 16);
		error = 0;
	}

	switch (stoi(type_choice))
	{
	case 1:

		A->set_type("��������");

		system("CLS");
		std::cout << "������� ������ ������!\n";
		counter_sleep(3);
		file.write_default_account(obj.get_login(), *A);
		obj.add_account(std::move(A));
		break;

	case 2:

		do
		{
			system("CLS");
			std::cout << "������ ��� ��������:\n\n"
				<< "<1> - ���������������\n"
				<< "<2> - �������\n"
				<< "<3> - ������������\n"
				<< "-> ";
			processed_input_allowed(buffer, "123");
		} while (buffer != "1" && buffer != "2" && buffer != "3");
		dynamic_cast<DepositAccount*>(A)->set_deposit_type((buffer == "1"? DepositType::Accumulative
			                                          : buffer == "2" ? DepositType::Saving : DepositType::Universal));
		dynamic_cast<DepositAccount*>(A)->set_type((buffer == "1" ? "���������� ���������������" :
			buffer == "2" ? "���������� �������" : "���������� ������������"));

		system("CLS");
		std::cout << "������ �������� ���� ��������:\n";
		processed_input_allowed(buffer, num);
		dynamic_cast<DepositAccount*>(A)->set_start_sum(stoll(buffer));

		system("CLS");
		std::cout << "������ �������:\n";
		processed_input_allowed(buffer, num);
		dynamic_cast<DepositAccount*>(A)->set_percent(stoi(buffer));

		do
		{
			system("CLS");

			if (!error)
				error = 1;
			else
				std::cout << "���� ������� ������!\n";

			std::cout << "������ ���� �������� ��������:\n\n"
				<< "���� -> ";
			processed_input_allowed(buffer, num);
			day = stoi(buffer);

			std::cout << "\n����� -> ";
			processed_input_allowed(buffer, num);
			month = stoi(buffer);

			std::cout << "\n�� -> ";
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
				std::cout << "���� ������� ������!\n";

			std::cout << "������ ���� ��������� ��������:\n\n"
				<< "\n���� -> ";
			processed_input_allowed(buffer, num);
			day = stoi(buffer);

			std::cout << "\n����� -> ";
			processed_input_allowed(buffer, num);
			month = stoi(buffer);

			std::cout << "\n�� -> ";
			processed_input_allowed(buffer, num);
			year = stoi(buffer);
		} while (!check_date_corectness(day, month, year)
			|| dynamic_cast<DepositAccount*>(A)->get_begin_date() >= Date(day, month, year));
		dynamic_cast<DepositAccount*>(A)->set_end_date(Date(day, month, year));
		dynamic_cast<DepositAccount*>(A)->date_synchronize_percents();

		system("CLS");
		std::cout << "������� ������ ������!\n";
		counter_sleep(3);
		file.write_deposit_account(obj.get_login(), *dynamic_cast<DepositAccount*>(A));
		obj.add_account(std::move(A));
		break;

	case 3:

		A->set_type("���������");

		system("CLS");
		std::cout << "������ ���:\n";
		processed_input_allowed(buffer, num);
		dynamic_cast<CreditAccount*>(A)->set_limit(stol(buffer));

		system("CLS");
		std::cout << "������ �������:\n";
		processed_input_allowed(buffer, num);
		dynamic_cast<CreditAccount*>(A)->set_percent(stoi(buffer));

		do
		{
			system("CLS");

			if(!error)
				error = 1;
			else
				std::cout << "���� ������� ������!\n";

			std::cout << "������ ���� �������� �������:\n\n"
				<< "���� -> ";
			processed_input_allowed(buffer, num);
			day = stoi(buffer);

			std::cout << "\n����� -> ";
			processed_input_allowed(buffer, num);
			month = stoi(buffer);

			std::cout << "\n�� -> ";
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
				std::cout << "���� ������� ������!\n";

			std::cout << "������ ���� ��������� �������:\n\n"
				<< "���� -> ";
			processed_input_allowed(buffer, num);
			day = stoi(buffer);

			std::cout << "\n����� -> ";
			processed_input_allowed(buffer, num);
			month = stoi(buffer);

			std::cout << "\n�� -> ";
			processed_input_allowed(buffer, num);
			year = stoi(buffer);
		} while (!check_date_corectness(day, month, year) 
			|| dynamic_cast<CreditAccount*>(A)->get_begin_date() >= Date(day, month, year));
		dynamic_cast<CreditAccount*>(A)->set_end_date(Date(day, month, year));

		system("CLS");
		std::cout << "������� ������ ������!\n";
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
	std::cout << "������ ��������� ���� -> ";
	processed_input_allowed(buffer, "1234567890");
	sum = stoll(buffer);

	system("CLS");
	std::cout << "������ ������� -> ";
	processed_input_allowed(buffer, "1234567890");
	percent = stoi(buffer);

	do
	{
		system("CLS");

		if (!error)
			error = 1;
		else
			std::cout << "���� ������� ������!\n";

		std::cout << "������ ���� �������� ��������:\n\n"
			<< "���� -> ";
		processed_input_allowed(buffer, "1234567890");
		day = stoi(buffer);

		std::cout << "\n����� -> ";
		processed_input_allowed(buffer, "1234567890");
		month = stoi(buffer);

		std::cout << "\n�� -> ";
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
			std::cout << "���� ������� ������!\n";

		std::cout << "������ ���� ��������� ��������:\n\n"
			<< "\n���� -> ";
		processed_input_allowed(buffer, "1234567890");
		day = stoi(buffer);

		std::cout << "\n����� -> ";
		processed_input_allowed(buffer, "1234567890");
		month = stoi(buffer);

		std::cout << "\n�� -> ";
		processed_input_allowed(buffer, "1234567890");
		year = stoi(buffer);
	} while (!check_date_corectness(day, month, year) || begin >= Date(day, month, year));
	end.Day = day;
	end.Month = month;
	end.Year = year;

	system("CLS");
	std::cout << "���������:\n";
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
		std::cout << "����� ���� ������� �������";
		return;
	}

	user.print_accounts();
	std::cout << "\n������ ������������ -> ";
	processed_input_allowed(buffer, "1234567890");
	id = stoi(buffer) - 1;

	do
	{
		system("CLS");
		std::cout << "������ ��������:\n\n"
			<< "<1> - �����������\n"
			<< "<2> - �������\n"
			<< "<3> - ����������� ������ 䳿\n"
			<< "-> ";
		processed_input_allowed(buffer, "123");
	} while (buffer != "1" && buffer != "2" && buffer != "3");
	user_choice = stoi(buffer);

	system("CLS");

	if (user_choice == 1 || user_choice == 2)
	{
		system("CLS");
		std::cout << "������ ����:\n";
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
				std::cout << "���� ������� ������!\n";

			std::cout << "������ ���� ��������� ��������:\n\n"
				<< "\n���� -> ";
			processed_input_allowed(buffer, "1234567890");
			day = stoi(buffer);

			std::cout << "\n����� -> ";
			processed_input_allowed(buffer, "1234567890");
			month = stoi(buffer);

			std::cout << "\n�� -> ";
			processed_input_allowed(buffer, "1234567890");
			year = stoi(buffer);
		} while (!check_date_corectness(day, month, year));

		system("CLS");

		user.extend_end_date(id, Date(day, month, year));
	}
}

#endif