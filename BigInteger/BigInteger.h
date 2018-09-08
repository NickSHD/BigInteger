#pragma once
#include <functional>
#include <string>
#include <vector>

typedef unsigned int ui;
typedef unsigned long long ull;

class BigInteger {

private:
	void make_right();
	//void shiftCells(int rhs);
	std::string BASE_to_string(ui, bool) const;

public:
	BigInteger();
	BigInteger(BigInteger const &other);
	BigInteger(std::vector<ui> const &obj, bool _sign);
	BigInteger(int a);
	//BigInteger(ui a);
	explicit BigInteger(std::string const &str);
	//~BigInteger();


	BigInteger &operator=(BigInteger const &other);
	//BigInteger &operator*=(ui const &rhs);
	//BigInteger &operator-=(BigInteger const &rhs);
	//BigInteger &operator*=(BigInteger const &rhs);
	//BigInteger &operator/=(BigInteger const &rhs);
	//BigInteger &operator%=(BigInteger const &rhs);
	//BigInteger &apply_bit_operation(BigInteger const &rhs, const std::function<ui(ui, ui)> func);
	//BigInteger &operator&=(BigInteger const &rhs);
	//BigInteger &operator|=(BigInteger const &rhs);
	//BigInteger &operator^=(BigInteger const &rhs);
	//BigInteger &operator<<=(int rhs);
	//BigInteger &operator>>=(int rhs);
	BigInteger operator+() const;
	BigInteger operator-() const;
	//BigInteger operator~() const;
	//BigInteger &operator++();
	//const BigInteger operator++(int);
	//BigInteger &operator--();
	//const BigInteger operator--(int);

	bool operator==(BigInteger const &right) const;
	bool operator!=(BigInteger const &right) const;
	bool operator<(BigInteger const &right) const;
	bool operator>(BigInteger const &right) const;
	bool operator<=(BigInteger const &right) const;
	bool operator>=(BigInteger const &right) const;

	std::string to_string() const;
	//friend std::pair<BigInteger, ui> sDiv(BigInteger const &a, ui const &b);
	//friend BigInteger operator*(BigInteger const &a, ui const &b);
	//friend ui getTrial(ui const &a, ui const &b, ui const &c);

private:
	const int BASE_Degree[10] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };
	std::vector<ui> data;
	bool sign;

	static const ui kLogBase;
	static const ull kBase;
	static const ui maxNumber;
};

//BigInteger operator+(BigInteger a, BigInteger const &b);
//BigInteger operator-(BigInteger a, BigInteger const &b);
//BigInteger operator*(BigInteger a, BigInteger const &b);
//BigInteger operator/(BigInteger a, BigInteger const &b);
//BigInteger operator%(BigInteger a, BigInteger const &b);
//BigInteger operator&(BigInteger a, BigInteger const &b);
//BigInteger operator|(BigInteger a, BigInteger const &b);
//BigInteger operator^(BigInteger a, BigInteger const &b);
//BigInteger operator<<(BigInteger a, int b);
//BigInteger operator>>(BigInteger a, int b);

//bool operator==(BigInteger const &a, BigInteger const &b);
//bool operator!=(BigInteger const &a, BigInteger const &b);
//bool operator<(BigInteger const &a, BigInteger const &b);
//bool operator>(BigInteger const &a, BigInteger const &b);
//bool operator<=(BigInteger const &a, BigInteger const &b);
//bool operator>=(BigInteger const &a, BigInteger const &b);

int check(BigInteger const &a, BigInteger const &b);
int check_modules(BigInteger const &a, BigInteger const &b);
std::string to_string(BigInteger const &a);
std::ostream &operator<<(std::ostream &s, BigInteger const &a);
std::istream &operator>>(std::istream &s, BigInteger &a);
//std::pair<BigInteger, ui> myDiv(BigInteger const &a, ui const &b);
//BigInteger operator*(BigInteger const &a, ui const &b);
//ui getTrial(ui const &a, ui const &b, ui const &c);


