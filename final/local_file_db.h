#ifndef LOCAL_FILE_DB_H
#define LOCAL_FILE_DB_H

#include <string>
#include <vector>
#include <fstream>

#include "user.h"
#include "account.h"
#include "date.h"
#include "system_work.h"

class LocalFileDB
{
	const std::string file_direction;

public:

	LocalFileDB(const std::string& dir)
		:file_direction(dir)
	{
	}

	void write_user(const User& obj) const
	{
		std::ofstream file(file_direction, std::ofstream::app);

		file << '\n'
			<< '*' << obj.get_login()
			<< "~N~" << obj.get_name()
			<< "~S~" << obj.get_surname()
			<< "~PN~" << obj.get_phone_number()
			<< "~G~" << obj.get_gmail()
			<< "~P~" << obj.get_password();

		file.close();
	}

	void write_default_account(const std::string& login, const Account& obj) const
	{
		std::ofstream file(file_direction, std::ofstream::app);

		file << '\n'
			<< "##" << login
			<< "~CT~" << obj.get_currency_type()
			<< "~T~" << obj.get_type()
			<< "~N~" << obj.get_number()
			<< "~CN~" << obj.get_card_number()
			<< "~BN~" << obj.get_bank_name()
			<< "~B~" << obj.get_balance();

		file.close();
	}

	void write_deposit_account(const std::string& login, const DepositAccount& obj) const
	{
		std::ofstream file(file_direction, std::ofstream::app);

		file << '\n'
			<< "#$" << login
			<< "~SS~" << obj.get_start_sum()
			<< "~CT~" << obj.get_currency_type()
			<< "~N~" << obj.get_number()
			<< "~CN~" << obj.get_card_number()
			<< "~BN~" << obj.get_bank_name()
			<< "~B~" << obj.get_balance()
			<< "~P~" << obj.get_percent()
			<< "~BD~" << obj.get_begin_date()
			<< "~ED~" << obj.get_end_date()
			<< "~DT~" << obj.get_deposit_type();

		file.close();
	}

	void write_credit_account(const std::string& login, const CreditAccount& obj) const
	{
		std::ofstream file(file_direction, std::ofstream::app);

		file << '\n'
			<< "$#" << login
			<< "~CT~" << obj.get_currency_type()
			<< "~N~" << obj.get_number()
			<< "~CN~" << obj.get_card_number()
			<< "~BN~" << obj.get_bank_name()
			<< "~B~" << obj.get_balance()
			<< "~L~" << obj.get_limit()
			<< "~BD~" << obj.get_begin_date()
			<< "~ED~" << obj.get_end_date()
			<< "~P~" << obj.get_percent();

		file.close();
	}

	void write_income(const std::string& login, const Account& account, const long long& amount) const
	{
		std::ofstream file(file_direction, std::ofstream::app);

		file << '\n'
			<< "$$$" << login 
			<< "~T~" << current_utc_date() << " "
			<< str_current_utc_time() << " (UTC)"
			<< "~N~" << "номер рахунку: " << account.get_number()
			<< "~BN~" << "установа: " << account.get_bank_name()
			<< "~CN~" << "номер карти: " << (account.get_card_number().empty() ? "немає" : account.get_card_number())
			<< "~I~" << "надходження -> +" << amount << account.get_currency_type()
			<< "~B~" << "залишок -> " << account.get_balance() << account.get_currency_type();

		file.close();
	}

	void write_expense(const std::string& login, const Account& account, const long long& amount) const
	{
		std::ofstream file(file_direction, std::ofstream::app);

		file << '\n'
			<< "$$$" << login
			<< "~T~" << current_utc_date() << " "
			<< str_current_utc_time() << " (UTC)"
			<< "~N~" << "номер рахунку: " << account.get_number()
			<< "~BN~" << "установа: " << account.get_bank_name()
			<< "~CN~" << "номер карти: " << (account.get_card_number().empty() ? "немає" : account.get_card_number())
			<< "~I~" << "видаток -> -" << amount << account.get_currency_type()
			<< "~B~" << "залишок -> " << account.get_balance() << account.get_currency_type();

		file.close();
	}

	void read_user(const std::string& login, User& obj) const
	{
		std::ifstream file(file_direction);
		std::string read_line("");

		while (getline(file, read_line))
		{
			if (read_line.find('*' + login) != std::string::npos)
			{
				obj.set_login(read_line.substr(read_line.find("*") + 1, read_line.find("~N~") - read_line.find("*") - 1));
				obj.set_name(read_line.substr(read_line.find("~N~") + 3, read_line.find("~S~") - read_line.find("~N~") - 3));
				obj.set_surname(read_line.substr(read_line.find("~S~") + 3, read_line.find("~PN~") - read_line.find("~S~") - 3));
				obj.set_phone_number(read_line.substr(read_line.find("~PN~") + 4, read_line.find("~G~") - read_line.find("~PN~") - 4));
				obj.set_gmail(read_line.substr(read_line.find("~G~") + 3, read_line.find("~P~") - read_line.find("~G~") - 3));
				obj.set_password(read_line.substr(read_line.find("~P~") + 3));
				break;
			}
		}

		file.close();
	}

	void read_user_accounts(User& obj) const
	{
		std::ifstream file(file_direction);
		std::string read_line("");

		while (getline(file, read_line))
		{
			if (read_line.find("##" + obj.get_login()) != std::string::npos)
			{
				std::string currency_type(read_line.substr(read_line.find("~CT~") + 4, read_line.find("~T~") - read_line.find("~CT~") - 4));

				Account* Pointer = new Account(
					read_line.substr(read_line.find("~T~") + 3, read_line.find("~N~") - read_line.find("~T~") - 3),
					read_line.substr(read_line.find("~N~") + 3, read_line.find("~CN~") - read_line.find("~N~") - 3),
					read_line.substr(read_line.find("~BN~") + 4, read_line.find("~B~") - read_line.find("~BN~") - 4),
					stoll(read_line.substr(read_line.find("~B~") + 3)),
					currency_type == "грн" ? Currency::UAH : currency_type == "дол" ? Currency::USD : Currency::EUR);

				Pointer->set_card_number(read_line.substr(read_line.find("~CN~") + 4, read_line.find("~BN~") - read_line.find("~CN~") - 4));

				obj.add_account(std::move(Pointer));
			}

			else if (read_line.find("#$" + obj.get_login()) != std::string::npos)
			{
				std::string currency_type(read_line.substr(read_line.find("~CT~") + 4, read_line.find("~N~") - read_line.find("~CT~") - 4));
				std::string deposit_type(read_line.substr(read_line.find("~DT~") + 4));

				DepositAccount* Pointer = new DepositAccount(
					read_line.substr(read_line.find("~N~") + 3, read_line.find("~CN~") - read_line.find("~N~") - 3),
					read_line.substr(read_line.find("~BN~") + 4, read_line.find("~B~") - read_line.find("~BN~") - 4),
					stoll(read_line.substr(read_line.find("~B~") + 3, read_line.find("~P~") - read_line.find("~B~") - 3)),
					currency_type == "грн" ? Currency::UAH : currency_type == "дол" ? Currency::USD : Currency::EUR,
					stoll(read_line.substr(read_line.find("~SS~") + 4, read_line.find("~CT~") - read_line.find("~SS~") - 4)),
					deposit_type == "накопичувальний" ? DepositType::Accumulative :
					deposit_type == "ощадний" ? DepositType::Saving : DepositType::Universal,
					stoi(read_line.substr(read_line.find("~P~") + 3, read_line.find("~BD~") - read_line.find("~P~") - 3)),
					string_to_date(read_line.substr(read_line.find("~BD~") + 4, read_line.find("~ED~") - read_line.find("~BD~") - 4)),
					string_to_date(read_line.substr(read_line.find("~ED~") + 4, read_line.find("~DT~") - read_line.find("~ED~") - 4))
				);

				Pointer->set_card_number(read_line.substr(read_line.find("~CN~") + 4, read_line.find("~BN~") - read_line.find("~CN~") - 4));

				obj.add_account(std::move(Pointer));
			}

			else if(read_line.find("$#" + obj.get_login()) != std::string::npos)
			{
				std::string currency_type(read_line.substr(read_line.find("~CT~") + 4, read_line.find("~N~") - read_line.find("~CT~") - 4));

				CreditAccount* Pointer = new CreditAccount(
					read_line.substr(read_line.find("~N~") + 3, read_line.find("~CN~") - read_line.find("~N~") - 3),
					read_line.substr(read_line.find("~BN~") + 4, read_line.find("~B~") - read_line.find("~BN~") - 4),
					stoll(read_line.substr(read_line.find("~B~") + 3, read_line.find("~L~") - read_line.find("~B~") - 3)),
					currency_type == "грн" ? Currency::UAH : currency_type == "дол" ? Currency::USD : Currency::EUR,
					stoi(read_line.substr(read_line.find("~L~") + 3, read_line.find("~BD~") - read_line.find("~L~") - 3)),
					stoi(read_line.substr(read_line.find("~P~") + 3)),
					string_to_date(read_line.substr(read_line.find("~BD~") + 4, read_line.find("~ED~") - read_line.find("~BD~") - 4)),
					string_to_date(read_line.substr(read_line.find("~ED~") + 4, read_line.find("~P~") - read_line.find("~ED~") - 4))
				);

				Pointer->set_card_number(read_line.substr(read_line.find("~CN~") + 4, read_line.find("~BN~") - read_line.find("~CN~") - 4));

				obj.add_account(std::move(Pointer));
			}
		}

		file.close();
	}

	std::string get_user_activities(const User& obj) const
	{
		std::ifstream file(file_direction);
		std::string read_line("");
		std::string result("");
		std::vector<std::string> temporary;

		while (getline(file, read_line))
		{
			if (read_line.find("$$$" + obj.get_login()) != std::string::npos)
			{
				result += read_line.substr(read_line.find("~T~") + 3, read_line.find("~N~") - read_line.find("~T~") - 3) + '\n';
				result += read_line.substr(read_line.find("~N~") + 3, read_line.find("~BN~") - read_line.find("~N~") - 3) + '\n';
				result += read_line.substr(read_line.find("~BN~") + 4, read_line.find("~CN~") - read_line.find("~BN~") - 4) + '\n';
				result += read_line.substr(read_line.find("~CN~") + 4, read_line.find("~I~") - read_line.find("~CN~") - 4) + '\n';
				result += read_line.substr(read_line.find("~I~") + 3, read_line.find("~B~") - read_line.find("~I~") - 3) + '\n';
				result += read_line.substr(read_line.find("~B~") + 3) + '\n';
				temporary.push_back(result);
				result.clear();
			}
		}

		reverse(temporary.begin(), temporary.end());
		result += "----------------------------------------\n";

		for (auto& i : temporary)
			result += i + "----------------------------------------\n";
		
		return result;
	}

	bool check_login_info(const std::string& login, const std::string& password) const
	{
		std::ifstream file(file_direction);
		std::string read_line("");

		while (getline(file, read_line))
		{
			if (read_line.substr(read_line.find("*") + 1, read_line.find("~N~") - read_line.find("*") - 1) == login
				&& read_line.substr(read_line.find("~P~") + 3) == password)
				return true;
		}

		return false;
	}

	bool check_login_uniqueness(const std::string& login) const
	{
		std::ifstream file(file_direction);
		std::string read_line("");

		while (getline(file, read_line))
		{
			if (read_line.substr(read_line.find("*") + 1, read_line.find("~N~") - read_line.find("*") - 1) == login)
				return false;
		}

		return true;
	}
};

#endif