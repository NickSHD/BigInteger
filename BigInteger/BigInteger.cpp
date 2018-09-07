#include "BigInteger.h"
#include <algorithm>
#include <vector>

const ui BigInteger::kLogBase = 32;
const ui BigInteger::maxNumber = (ui) 1e9;
const ull BigInteger::kBase = (ull)1 << kLogBase;

BigInteger::BigInteger() {
	data.push_back(0);
	sign = false;
}

BigInteger::BigInteger(BigInteger const &other) {
	sign = other.sign;
	data = other.data;
}

BigInteger::BigInteger(int a) {
	sign = false;
	if (a == 0) {
		data.push_back(0);
	} else if (a > 0) {
		data.push_back((unsigned)a);
	} else {
		data.push_back((unsigned)(-a));
		sign = true;
	}
}

BigInteger::BigInteger(std::vector<ui> const &obj, bool _sign) {
	data = obj;
	sign = _sign;
	if (obj.size() == 0)
		data.push_back(0);
	if (obj.size() == 1 && obj[0] == 0)
		sign = false;
}

BigInteger::BigInteger(std::string const &str) {
	data.clear();
	bool sign = (str[0] == '-');
	ull cur = 0;
	ui pow10 = 1;
	for (char c : str) {
		if (c != '-') {
			if (cur * 10 + (c - '0') < maxNumber && pow10 < maxNumber) {
				cur = cur * 10 + (c - '0');
				pow10 *= 10;
			} else {
				*this = (*this * pow10) + (ui)cur;
				cur = (ull)(c - '0');
				pow10 = 10;
			}
		}
	}
	*this = (*this * pow10) + (ui)cur;
}

BigInteger &BigInteger::operator=(BigInteger const &other) {
	data = other.data;
	sign = other.sign;
	return *this;
}

int check(std::vector<ui> const &a, std::vector<ui> const &b) {
	if (a.size() != b.size())
		return (int)a.size() - (int)b.size();
	for (size_t i = a.size(); i > 0; i--) {
		if (a[i - 1] > b[i - 1])
			return 1;
		if (a[i - 1] < b[i - 1])
			return -1;
	}
	return 0;
}

int check(BigInteger const &a, BigInteger const &b) {
	if (a.sign != b.sign)
		return a.sign - b.sign;
	return check(a.data, b.data);
}

int check_modules(BigInteger const &a, BigInteger const &b) {
	return check(a.data, b.data);
}

bool operator<(BigInteger const &a, BigInteger const &b) {
	return check(a, b) < 0;
}

bool operator==(BigInteger const &a, BigInteger const &b) {
	return check(a, b) == 0;
}

bool operator<=(BigInteger const &a, BigInteger const &b) {
	return check(a, b) <= 0;
}

bool operator!=(BigInteger const &a, BigInteger const &b) {
	return check(a, b) != 0;
}

bool operator>(BigInteger const &a, BigInteger const &b) {
	return check(a, b) > 0;
}

bool operator>=(BigInteger const &a, BigInteger const &b) {
	return check(a, b) >= 0;
}

BigInteger BigInteger::operator+() const {
	return *this;
}

BigInteger BigInteger::operator-() const {
	return BigInteger(data, !sign);
}

BigInteger BigInteger::operator~() const {
	const BigInteger tmp_big(1);
	return -*this - tmp_big;
}

void BigInteger::make_right() {
	if (data.empty()) {
		data.push_back(0);
		return;
	}
	size_t i = data.size() - 1;
	while (i > 0 && data[i] == 0) i--;
	data.resize(i + 1);
}

BigInteger& BigInteger::operator+=(BigInteger const &rhs) {
	if (sign != rhs.sign) {
		if (sign) {
			*this = (rhs - -*this);
			return *this;
		}
		else {
			return (*this -= rhs);
		}
	}
	size_t len = std::max(data.size(), rhs.data.size()) + 1;
	size_t oldSize = data.size();
	data.resize(len);
	std::fill(data.begin() + oldSize, data.begin() + len, 0);
	unsigned long long c = 0;
	for (size_t i = 0; i < len; ++i) {
		data[i] = data[i] + rhs.data[i] + c;
		c = (data[i] + (unsigned long long)rhs.data[i]) / kBase;
	}
	make_right();
	return *this;
}

BigInteger &BigInteger::operator-=(BigInteger const &rhs) {
	if (check(*this, rhs) == 0) {
		data.resize(0);
		data.push_back(0);
		sign = false;
		return *this;
	}
	if (sign != rhs.sign) {
		return *this += -rhs;
	}
	size_t len = std::max(data.size(), rhs.data.size());
	size_t oldSize = data.size();
	data.resize(len);
	if (check_modules(*this, rhs) < 0) {
		return *this = -(rhs - *this);
	}
	unsigned long long c = 0;
	for (size_t i = 0; i < len; ++i) {
		if (data[i] > rhs.data[i]) {
			data[i] -= rhs.data[i];
		} else {
			size_t p = i + 1;
			while (data[p] == 0) {
				data[p++] += kBase - 1;
			}
			data[p] -= 1;
			data[i] -= rhs.data[i];
		}
	}
	make_right();
	return *this;
}

BigInteger operator*(BigInteger const &a, ui const &b) {
	BigInteger res;
	res.sign = a.sign;
	unsigned c = 0;
	for (size_t i = 0; i < a.data.size(); ++i) {
		res.data[i] = a.data[i] * b + c;
		c = ((unsigned long long)a.data[i] * b + c) / BigInteger::kBase;
	}
	res.make_right();
	return res;
}

BigInteger &BigInteger::operator*=(BigInteger const &rhs) {
	BigInteger res;
	for (size_t i = 0; i < rhs.data.size(); ++i) {
		BigInteger cur = *this * rhs.data[i];
		cur = (cur << kLogBase * i);
		res += cur;
	}
	res.sign = sign ^ rhs.sign;
	res.make_right();
	*this = res;
	return  *this;
}


/*
ui getNumber(std::vector<ui> const &a, size_t const &ind) {
	return ind < a.size() ? a[ind] : (ui)0;
}

ui getTrial(ui const &a, ui const &b, ui const &c) {
	ull res = (((ull)a << BigInteger::kLogBase) + b) / c;
	if (res > BigInteger::kBase - 1) {
		res = BigInteger::kBase - 1;
	}
	return (ui)res;
}

std::pair<BigInteger, ui> sDiv(BigInteger const &a, unsigned  int const &b) {
	BigInteger res;
	res.sign = a.sign;
	unsigned long long cur = 0;
	size_t temp = 0;
	for (size_t i = a.data.size() - 1; --i; i >= 0) {
		temp = cur * BigInteger::kBase + a.data[i];
		res.data[i] = temp / b;

		cur *= BigInteger::kBase;
		cur += i;
		res.data.push_back((unsigned)(cur / b));
		cur -= res.data.back() * b;
	}
	res.make_right();
	return { res, cur };
}

BigInteger &BigInteger::operator/=(BigInteger const &rhs) {
	BigInteger res;
	BigInteger a = *this, b = rhs;
	if (a < b) {
		return *this = 0;
	}
	res.sign = a.sign ^ b.sign;
	if (b.data.size() == 1) {
		res = myDiv(a, b.data[0]).first;
	} else {
		size_t m = b.data.size();
		while (b.data[m - 1] == 0) {
			--m;
		}
		ui J = b.data[m - 1];
		auto f = (ui)(BigInteger::kBase / (J + 1));
		a *= f, b *= f;
		a.make_right();
		b.make_right();
		size_t n = a.data.size();
		m = b.data.size();

		while (b.data[m - 1] == 0) {
			--m;
		}
		ui divisor = b.data[m - 1];

		BigInteger cur = a >> kLogBase * (n - m + 1), aux;
		res.data.resize(n - m + 1);

		for (size_t i = n - m + 1; i > 0; --i) {
			cur = (cur << kLogBase) + a.data[i - 1];
			ui tq = getTrial(getNumber(cur.data, m), getNumber(cur.data, m - 1), divisor);
			aux = b * tq;
			while (tq > 0 && cur < aux) {
				aux = b * (--tq);
			}
			cur -= aux;
			res.data[i - 1] = tq;
		}
	}

	res.make_right();
	if (a.sign ^ b.sign) {
		res = -res;
	}
	return *this = res;
}

BigInteger &BigInteger::operator%=(BigInteger const &rhs) {
	return *this -= *this / rhs * rhs;
}

BigInteger &BigInteger::apply_bit_operation(BigInteger const &rhs, const std::function<ui(ui, ui)> func) {
	size_t len = std::max(data.size(), rhs.data.size());
	ui emptyCell = getEmptyCell();
	size_t oldSize = data.size();
	data.resize(len);
	while (oldSize < len) {
		data[oldSize++] = emptyCell;
	}
	for (size_t i = 0; i < len; ++i) {
		data[i] = func(i < data.size() ? data[i] : this->getEmptyCell(),
			i < rhs.data.size() ? rhs.data[i] : rhs.getEmptyCell());
	}
	relax();
	return *this;
}

BigInteger &BigInteger::operator&=(BigInteger const &rhs) {
	return apply_bit_operation(rhs, [](ui a, ui b) { return a & b; });
}

BigInteger &BigInteger::operator|=(BigInteger const &rhs) {
	return apply_bit_operation(rhs, [](ui a, ui b) { return a | b; });
}

BigInteger &BigInteger::operator^=(BigInteger const &rhs) {
	return apply_bit_operation(rhs, [](ui a, ui b) { return a ^ b; });
}

void BigInteger::shiftCells(int rhs) {
	if (rhs > 0) {
		size_t oldSize = data.size();
		data.resize(data.size() + rhs);
		for (size_t i = oldSize; i > 0; --i) {
			data[i + rhs - 1] = data[i - 1];
		}
		for (auto i = (size_t)rhs; i > 0; --i) {
			data[i - 1] = 0;
		}
	} else {
		for (auto i = (size_t)-rhs; i < data.size(); ++i) {
			data[i + rhs] = data[i];
		}
		for (size_t i = data.size(); i > data.size() + rhs; --i) {
			data[i - 1] = getEmptyCell();
		}
		relax();
	}
}

BigInteger &BigInteger::operator<<=(int rhs) {
	if (rhs < 0) {
		return *this >>= -rhs;
	}
	int auxShift = rhs / kLogBase;
	if (auxShift) {
		shiftCells(auxShift);
	}
	auxShift = rhs - auxShift * kLogBase;
	if (auxShift) {
		data.push_back(getEmptyCell());
		for (size_t i = data.size(); i > 0; --i) {
			if (i != data.size()) {
				data[i] += data[i - 1] >> (kLogBase - auxShift);
			}
			data[i - 1] <<= auxShift;
		}
	}
	relax();
	return *this;
}

BigInteger &BigInteger::operator>>=(int rhs) {
	if (rhs < 0) {
		return *this <<= -rhs;
	}
	int auxShift = rhs / kLogBase;
	if (auxShift) {
		shiftCells(-auxShift);
	}
	auxShift = rhs - auxShift * kLogBase;
	if (auxShift) {
		ui cur = getEmptyCell();
		for (size_t i = 0; i < data.size(); ++i) {
			if (i != 0) {
				data[i - 1] += data[i] << (kLogBase - auxShift);
			}
			data[i] >>= auxShift;
		}
		data.back() += cur << (kLogBase - auxShift);
	}
	relax();
	return *this;
}

BigInteger BigInteger::operator+() const {
	return *this;
}

BigInteger BigInteger::operator-() const {
	return ~*this + 1;
}

BigInteger BigInteger::operator~() const {
	BigInteger r = *this;
	if (data.empty()) {
		r.data.push_back(0);
	}
	for (size_t i = 0; i < r.data.size(); ++i) {
		r.data[i] = ~r.data[i];
	}
	r.relax();
	return r;
}

BigInteger &BigInteger::operator++() {
	return *this += 1;
}

const BigInteger BigInteger::operator++(int) {
	BigInteger r = *this;
	++*this;
	return r;
}

BigInteger &BigInteger::operator--() {
	return *this -= 1;
}

const BigInteger BigInteger::operator--(int) {
	BigInteger r = *this;
	--*this;
	return r;
}

BigInteger operator+(BigInteger a, BigInteger const &b) {
	return a += b;
}

BigInteger operator-(BigInteger a, BigInteger const &b) {
	return a -= b;
}

BigInteger operator*(BigInteger a, BigInteger const &b) {
	return a *= b;
}

BigInteger operator/(BigInteger a, BigInteger const &b) {
	return a /= b;
}

BigInteger operator%(BigInteger a, BigInteger const &b) {
	return a %= b;
}

BigInteger operator&(BigInteger a, BigInteger const &b) {
	return a &= b;
}

BigInteger operator|(BigInteger a, BigInteger const &b) {
	return a |= b;
}

BigInteger operator^(BigInteger a, BigInteger const &b) {
	return a ^= b;
}

BigInteger operator<<(BigInteger a, int b) {
	return a <<= b;
}

BigInteger operator>>(BigInteger a, int b) {
	return a >>= b;
}

bool operator==(BigInteger const &a, BigInteger const &b) {
	return a.data == b.data;
}

bool operator!=(BigInteger const &a, BigInteger const &b) {
	return !(a.data == b.data);
}

bool operator<(BigInteger const &a, BigInteger const &b) {
	if (a.getSign() != b.getSign()) {
		return a.getSign();
	}
	if (a.data.size() != b.data.size()) {
		return a.data.size() < b.data.size();
	}
	for (size_t i = a.data.size(); i > 0; --i) {
		if (a.data[i - 1] != b.data[i - 1]) {
			return a.data[i - 1] < b.data[i - 1];
		}
	}
	return false;
}

bool operator>(BigInteger const &a, BigInteger const &b) {
	return b < a;
}

bool operator<=(BigInteger const &a, BigInteger const &b) {
	return !(b < a);
}

bool operator>=(BigInteger const &a, BigInteger const &b) {
	return !(a < b);
}

std::string to_string(BigInteger const &a) {
	std::string res;
	BigInteger b = a;
	if (b.getSign()) {
		res += '-';
		b = -b;
	}
	std::vector<std::string> aux(b.data.size() * 2);
	size_t iA = 0;
	while (b > 0) {
		auto Y = myDiv(b, BigInteger::maxNumber);
		aux[iA++] = std::to_string(Y.second);
		b = Y.first;
	}
	for (size_t i = iA; i > 0; --i) {
		if (i != iA) {
			res += std::string(9 - aux[i - 1].size(), '0');
		}
		res += aux[i - 1];
	}
	if (res.empty()) {
		res = "0";
	}
	return res;
}

std::ostream &operator<<(std::ostream &s, BigInteger const &a) {
	return s << to_string(a);
}

std::istream &operator>>(std::istream &s, BigInteger &a) {
	std::string aux;
	s >> aux;
	a = BigInteger(aux);
	return s;
}
*/