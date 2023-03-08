#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

#include "account.h"

class User
{
protected:

	std::string name_;
	std::string surname_;

	std::string login_;
	std::string password_;

	std::string gmail_;
	std::string phone_number_;

	std::vector<Account*> accounts_;

public:

	User()
	   :name_(), surname_(),
		login_(), password_(),
		gmail_(), phone_number_()
	{}

	explicit User(const std::string& name, const std::string& surname,
				  const std::string& login, const std::string password,
				  const std::string& gmail, const std::string& phone_number)
				 :name_(name), surname_(surname),
				  login_(login), password_(password),
				  gmail_(gmail), phone_number_(phone_number)
	{}

	~User()
	{
		for (auto& i : accounts_)
			delete i;
	}

	User(const User& other)
		:name_(other.name_), surname_(other.surname_),
		login_(other.login_), password_(other.password_),
		gmail_(other.gmail_), phone_number_(other.phone_number_)
	{
		for (auto& i : other.accounts_)
			accounts_.push_back(new Account(*i));
	}

	User(User&& other) noexcept
		:name_(other.name_), surname_(other.surname_),
		login_(other.login_), password_(other.password_),
		gmail_(other.gmail_), phone_number_(other.phone_number_)
	{
		other.name_.clear();
		other.surname_.clear();
		other.login_.clear();
		other.password_.clear();
		other.gmail_.clear();
		other.phone_number_.clear();

		for (auto& i : other.accounts_)
		{
			accounts_.push_back(i);
			i = nullptr;
		}
	}

	User& operator = (const User& other)
	{
		if (this == &other)
			return *this;

		name_ = other.name_;
		surname_ = other.surname_;
		login_ = other.login_;
		password_ = other.password_;
		gmail_ = other.gmail_;
		phone_number_ = other.phone_number_;

		for (auto& i : other.accounts_)
			accounts_.push_back(new Account(*i));

		return *this;
	}

	User& operator = (User&& other) noexcept
	{
		if (this == &other)
			return *this;

		name_ = other.name_;
		surname_ = other.surname_;
		login_ = other.login_;
		password_ = other.password_;
		gmail_ = other.gmail_;
		phone_number_ = other.phone_number_;

		other.name_.clear();
		other.surname_.clear();
		other.login_.clear();
		other.password_.clear();
		other.gmail_.clear();
		other.phone_number_.clear();

		for (auto& i : other.accounts_)
		{
			accounts_.push_back(i);
			i = nullptr;
		}

		return *this;
	}

	Account* operator[](int id)
	{
		if (id < 0 || id > accounts_.size() - 1)
			return new Account();
		else
			return accounts_.at(id);
	}

	std::string get_name()          const { return name_; }
	std::string get_surname()       const { return surname_; }
	std::string get_login()         const { return login_; }
	std::string get_password()      const { return password_; }
	std::string get_gmail()         const { return gmail_; }
	std::string get_phone_number()  const { return phone_number_; }
	size_t      get_accounts_size() const { return accounts_.size(); }
	
	void set_name(const std::string& name)                 { name_ = name; }
	void set_surname(const std::string& surname)           { surname_ = surname; }
	void set_login(const std::string& login)               { login_ = login; }
	void set_password(const std::string& password)         { password_ = password; }
	void set_gmail(const std::string& gmail)               { gmail_ = gmail; }
	void set_phone_number(const std::string& phone_number) { phone_number_ = phone_number; }

	void add_account(Account*&& account)
	{
		accounts_.push_back(account);
		account = nullptr;
	}

	bool income(int id, long long& amount)
	{
		DepositAccount* DpPointer;
		CreditAccount* CrPointer;
		bool no_errors = 1;

		if (id < 0 || id > accounts_.size() - 1)
		{
			std::cout << "невірний ідентифікатор";
			no_errors = 0;
		}
		else
		{
			if ((DpPointer = dynamic_cast<DepositAccount*>(accounts_.at(id))))
			{
				try
				{
					DpPointer->income(amount);
					std::cout << "операцію виконано успішно";
				}
				catch (const AccountException& ex)
				{
					no_errors = 0;
					std::cout << ex;
				}
			}
			else
			{
				accounts_.at(id)->income(amount);
				std::cout << "операцію виконано успішно";
			}
		}

		DpPointer = nullptr;
		CrPointer = nullptr;
		delete DpPointer;
		delete CrPointer;
		return no_errors;
	}

	bool expense(int id, long long& amount)
	{
		DepositAccount* DpPointer;
		CreditAccount* CrPointer;
		bool no_errors = 1;

		if (id < 0 || id > accounts_.size() - 1)
		{
			std::cout << "невірний ідентифікатор";
			no_errors = 0;
		}
		else
		{
			if ((DpPointer = dynamic_cast<DepositAccount*>(accounts_.at(id))))
			{
				try
				{
					DpPointer->expense(amount);
					std::cout << "операцію виконано успішно";
				}
				catch (const AccountException& ex)
				{
					std::cout << ex;
					no_errors = 0;
				}
			}
			else if ((CrPointer = dynamic_cast<CreditAccount*>(accounts_.at(id))))
			{
				try
				{
					CrPointer->expense(amount);
					std::cout << "операцію виконано успішно";
				}
				catch (const AccountException& ex)
				{
					std::cout << ex;
					no_errors = 0;
				}
			}
			else
			{
				try
				{
					accounts_.at(id)->expense(amount);
					std::cout << "операцію виконано успішно";
				}
				catch (const AccountException& ex)
				{
					std::cout << ex;
					no_errors = 0;
				}
			}
		}

		DpPointer = nullptr;
		CrPointer = nullptr;
		delete DpPointer;
		delete CrPointer;
		return no_errors;
	}

	bool extend_end_date(int id, const Date& date)
	{
		DepositAccount* DpPointer;
		CreditAccount* CrPointer;
		bool no_errors = 1;

		if (id < 0 || id > accounts_.size() - 1)
			std::cout << "невірний ідентифікатор";
		else
		{
			if ((DpPointer = dynamic_cast<DepositAccount*>(accounts_.at(id))))
			{
				if (DpPointer->get_end_date() >= date)
				{
					std::cout << "дата повинна бути пізнішою за поточну";
					no_errors = 0;
				}
				else
				{
					DpPointer->set_end_date(date);
					std::cout << "операцію виконано успішно";
				}
			}
			else if ((CrPointer = dynamic_cast<CreditAccount*>(accounts_.at(id))))
			{
				if (CrPointer->get_end_date() >= date)
				{
					std::cout << "дата повинна бути пізнішою за поточну";
					no_errors = 0;
				}
				else
				{
					CrPointer->set_end_date(date);
					std::cout << "операцію виконано успішно";
				}
			}
			else
			{
				std::cout << "невірний ідентифікатор";
				no_errors = 0;
			}
			
		}

		DpPointer = nullptr;
		CrPointer = nullptr;
		delete DpPointer;
		delete CrPointer;
		return no_errors;
	}

	void print_accounts()
	{
		int it = 0;
		for (auto& i : accounts_)
		{
			std::cout << ++it << " : "
				<< i->get_type() << " || номер рахунку: "
				<< i->get_number() << " || карта: "
				<< (i->get_card_number().empty() ? "немає" : i->get_card_number()) << " || залишок: ";
			i->print_balance();
			std::cout << '\n';
		}
	}
};

#endif