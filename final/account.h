#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <ostream>

#include "date.h"
#include "system_work.h"

long long calculate_income(const long long& start_sum, short percents, const Date& begin_data, const Date& end_data)
{
	long long income = 0;

	if (begin_data > end_data)
		return income;

	for (int i = get_days_difference(begin_data, end_data); i > 0; i--)
		income += start_sum / 100 * percents;

	return income;
}

enum class Currency
{
	UAH = 980,
	USD = 840,
	EUR = 978
};

enum class DepositType
{
	Accumulative,
	Saving,
	Universal
};

std::ostream& operator << (std::ostream& os, const Currency& obj)
{
	switch (obj)
	{
	case(Currency::UAH):
		return os << "грн";

	case(Currency::USD):
		return os << "дол";

	case(Currency::EUR):
		return os << "Ївр";

	default:
		return os << '?';
	}
}

std::ostream& operator << (std::ostream& os, const DepositType& obj)
{
	switch (obj)
	{
	case(DepositType::Accumulative):
		return os << "накопичувальний";

	case(DepositType::Saving):
		return os << "ощадний";

	case(DepositType::Universal):
		return os << "ун≥версальний";

	default:
		return os << '?';
	}
}

class AccountException
{
	std::string error_message_;

public:

	AccountException(const std::string& error_message)
		:error_message_(error_message)
	{}

	friend std::ostream& operator << (std::ostream& os, const AccountException& ex)
	{ return os << ex.error_message_; }
};

class Account
{
protected:

	std::string type_;
	Currency currency_type_;

	std::string number_;
	std::string bank_name_;

	std::string card_number_;

	long long balance_;

public:

	Account()
		:type_(),
		 number_(), bank_name_(),
		 balance_(), currency_type_()
	{}

	Account(const std::string& type,
		    const std::string& number, const std::string& bank_name,
		    const long long& balance, const Currency& currency_type)
		   :type_(type),
		    number_(number), bank_name_(bank_name),
		    balance_(balance), currency_type_(currency_type)
	{}

	std::string get_type()          const { return type_; }
	std::string get_number()        const { return number_; }
	std::string get_bank_name()     const { return bank_name_; }
	std::string get_card_number()   const { return card_number_; }
	long long   get_balance()       const { return balance_; }
	Currency    get_currency_type() const { return currency_type_; }

	void set_type(const std::string& type)                { type_ = type; }
	void set_number(const std::string& type)              { number_ = type; }
	void set_bank_name(const std::string& bank_name)      { bank_name_ = bank_name; }
	void set_card_number(const std::string card_number)   { card_number_ = card_number; }
	void set_balance(const long long& balance)            { balance_ = balance; }
	void set_currency_type(const Currency& currency_type) { currency_type_ = currency_type; }

	void print_balance() const 
	{ std::cout << balance_ << currency_type_; } 

	virtual void income(const long long& amount)
	{ balance_ += amount; }

	virtual void expense(const long long& amount)
	{
		if (balance_ >= amount)
			balance_ -= amount;
		else
			throw(AccountException("недостатьно кошт≥в"));
	}
};

class DepositAccount :public Account
{
	long long start_sum_;

	DepositType deposit_type_;

	short percent_;

	Date begin_date_;
	Date end_date_;

public:

	DepositAccount()
		:Account(),
		start_sum_(),
		deposit_type_(), percent_(),
		begin_date_(), end_date_()
	{}

	DepositAccount(const std::string& number, const std::string& bank_name,
		           const long long& balance, const Currency& currency_type, 
				   const long long& start_sum,
		           const DepositType& type, short percent,
		           const Date& begin_date, const Date& end_date)
		          :Account(deposit_type_ == DepositType::Accumulative ? "депозитний накопичувальний" :
					       deposit_type_ == DepositType::Saving ? "депозитний ощадний" : "депозитний ун≥версальний",
					       number, bank_name, balance, currency_type),
		           start_sum_(start_sum),
		           deposit_type_(type), percent_(percent),
		           begin_date_(begin_date), end_date_(end_date)
	{
		date_synchronize_percents();
	}

	long long   get_start_sum()    const { return start_sum_; }
	short       get_percent()      const { return percent_; }
	Date        get_begin_date()   const { return begin_date_; }
	Date        get_end_date()     const { return end_date_; }
	DepositType get_deposit_type() const { return deposit_type_; }

	void set_start_sum(const long long& start_sum) { start_sum_ = start_sum; }
	void set_percent(short percent)                { percent_ = percent; }
	void set_begin_date(const Date& date)          { begin_date_ = date; }
	void set_end_date(const Date& date)            { end_date_ = date; }
	void set_deposit_type(const DepositType& type) { deposit_type_ = type; }

	void income(const long long& amount) override
	{
		if (deposit_type_ == DepositType::Saving)
			throw(AccountException("тип депозиту не п≥дл€г€Ї надходженн€м"));

		else if (current_utc_date() > end_date_)
			throw(AccountException("терм≥н д≥њ рахунку зак≥нчивс€"));

		balance_ += amount;
	}

	void expense(const long long& amount) override
	{
		if (deposit_type_ != DepositType::Universal && end_date_ > current_utc_date())
			throw(AccountException("тип депозиту не п≥дл€г€Ї витратам до зак≥нченн€ терм≥ну"));

		else if (balance_ < amount)
			throw(AccountException("недостатьно кошт≥в"));

		else
			balance_ -= amount;
	}

	void get_percents()
	{ balance_ += start_sum_ / 100 * percent_; }

	void date_synchronize_percents()
	{
		int days_difference = 0;

		if (current_utc_date() > end_date_)
			days_difference = get_days_difference(begin_date_, end_date_);
		else
			days_difference = get_days_difference(begin_date_, current_utc_date());

		for (days_difference; days_difference > 0; days_difference--)
			get_percents();
	}
};

class CreditAccount :public Account
{
	long int limit_;

	short percent_;

	Date begin_date_;
	Date end_date_;

public:

	CreditAccount()
		:Account(),
		 limit_(), percent_(),
		 begin_date_(), end_date_()
	{}

	CreditAccount(const std::string& number, const std::string& bank_name,
		          const long long& balance, const Currency& currency_type,
		          const long int& limit, short percent,
		          const Date& begin_date, const Date& end_date)
		         :Account("кредитний", number, bank_name, balance, currency_type),
		          limit_(limit), percent_(percent),
		          begin_date_(begin_date), end_date_(end_date)
	{}

	short    get_percent()    const { return percent_; }
	long int get_limit()      const { return limit_; }
	Date     get_begin_date() const { return begin_date_; }
	Date     get_end_date()   const { return end_date_; }

	void set_percent(short percent)       { percent_ = percent; }
	void set_limit(const long int& limit) { limit_ = limit; }
	void set_begin_date(const Date& date) { begin_date_ = date; }
	void set_end_date(const Date& date)   { end_date_ = date; }


	void expense(const long long& amount) override
	{
		bool has_credit = balance_ < 0;

		if (balance_ + limit_ < amount)
			throw(AccountException("поточний л≥м≥т не дозвол€Ї виконати операц≥ю"));

		if (balance_ < amount && balance_ + limit_ >= amount && current_utc_date() > end_date_)
			throw(AccountException("терм≥н д≥њ рахунку зак≥нчивс€"));

		else
			balance_ -= amount;

		if (!has_credit)
			balance_ -= amount / 100 * percent_;
	}
};

#endif