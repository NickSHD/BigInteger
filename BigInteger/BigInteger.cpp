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

BigInteger::BigInteger(ui a) {
	sign = false;
	if (a == 0) {
		data.push_back(0);
	}
	else if (a > 0) {
		data.push_back((unsigned)a);
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


std::string BigInteger::BASE_to_string(ui n, bool f) const {
	std::string s;
	while (n != 0) {
		s = char((n % 10) + '0') + s;
		n /= 10;
	}
	if (f)
		while (s.length() < 9) {
			s = "0" + s;
		}
	return s;
}

ui BigInteger::getEmptyCell() const
{
	if (sign) {
		return (ui)(kBase - 1);
	}
	return 0;
}

std::string BigInteger::to_string() const {
	std::string s = "";
	for (int i = 0; i < (int)data.size(); ++i) {
		if (i != (int)data.size() - 1)
			s = BASE_to_string(data[i], true) + s;
		else
			s = BASE_to_string(data[i], false) + s;
	}
	if (this->sign) {
		s = "-" + s;
	}
	if (s.empty()) {
		return "0";
	}
	else {
		return s;
	}
}

BigInteger::BigInteger(std::string const &str) {
	std::string s(str);
	if (s == "0") {
		return;
	}
	if (s[0] == '-') {
		sign = true;
		s.erase(s.begin());
	}
	else {
		sign = false;
	}
	while (s.length() % 9 != 0) {
		s = '0' + s;
	}
	int k = s.length() / 9;
	for (int i = 0; i < k; ++i) {
		data.push_back(0);
		for (int j = 8; j >= 0; --j) {
			data[i] += ((int((s[i * 9 + j])) - int('0')) * BASE_Degree[8 - j]);
		}
	}
	for (int i = 0; i < (int)data.size() / 2; ++i) {
		long long a = data[i];
		data[i] = data[(int)data.size() - i - 1];
		data[data.size() - i - 1] = a;
	}
}


BigInteger &BigInteger::operator=(BigInteger const &other) {
	data = other.data;
	sign = other.sign;
	return *this;
}

bool BigInteger::operator<(const BigInteger &right) const {
	if (this->sign) {
		if (right.sign) return ((-right) < (-*(this)));
		else return true;
	}
	else if (right.sign) return false;
	else {
		if ((int)this->data.size() != (int)right.data.size()) {
			return (int)this->data.size() < (int)right.data.size();
		}
		else {
			for (int i = (int)this->data.size() - 1; i >= 0; --i) {
				if (this->data[i] != right.data[i]) return this->data[i] < right.data[i];
			}
			return false;
		}
	}
}

bool BigInteger::operator>(const BigInteger &right) const {
	return (right < *this);
}

bool BigInteger::operator==(const BigInteger &right) const {
	return (!(*this < right) && !(*this > right));
}

bool BigInteger::operator<=(const BigInteger &right) const {
	return (*this > right);
}

bool BigInteger::operator>=(const BigInteger &right) const {
	return !(*this < right);
}

bool BigInteger::operator!=(const BigInteger &right) const {
	return !(*this == right);
}

BigInteger BigInteger::operator+() const {
	return *this;
}

BigInteger BigInteger::operator-() const {
	return BigInteger(data, !sign);
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

BigInteger& BigInteger::operator*=(ui const &b) {
	BigInteger res;
	res.sign = this->sign;
	unsigned c = 0;
	for (size_t i = 0; i < this->data.size(); ++i) {
		res.data[i] = this->data[i] * b + c;
		c = ((unsigned long long)this->data[i] * b + c) / BigInteger::kBase;
	}
	res.make_right();
	return *this;
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
	if (*this == rhs) {
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
	if (*this < rhs) {
		return *this = -(rhs - *this);
	}
	unsigned long long c = 0;
	for (size_t i = 0; i < len; ++i) {
		if (data[i] > rhs.data[i]) {
			data[i] -= rhs.data[i];
		}
		else {
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

ui getNumber(std::vector<ui> const &a, size_t const &ind) {
	return ind < a.size() ? a[ind] : (ui)0;
}

std::pair<BigInteger, ui> sDiv(BigInteger const & a, ui const & b)
//std::pair<BigInteger, ui> sDiv(BigInteger const &a, unsigned int const &b) 
{
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

void myDiv(BigInteger const &a, BigInteger &b, BigInteger &res, BigInteger &cur) {
	if (a.data.size() < b.data.size()) {
		res = a;
		return;
	}
	if (b.data.size() == 1) {
		std::pair<BigInteger, ui> p = sDiv(a, b.data[0]);
		res = p.first;
		cur.data[0] = p.second;
		return;
	}
	BigInteger u = a;
	ull n = a.data.size(), m = u.data.size() - b.data.size();
	ui scale = BigInteger::kBase / (b.data[n - 1] + 1); //нормализация
	if (scale > 1) {   //нормализация
		u *= scale;
		b *= scale;
	}
	ull uJ, vJ, i;            //vJ - тек сдвиг B относ U(при вычитании), инд очередной цифры частного
	ull temp1, temp2, temp = 0;   //uJ - тек цифра U
	ui qGuess, r;             //догадка для частного и остаток
	ui borrow, carry; //переносы
	for (vJ = m, uJ = n + vJ; vJ >= 0; --vJ, --uJ) {
		qGuess = (u.data[uJ] * BigInteger::kBase + u.data[uJ - 1]) / b.data[n - 1];
		r = (u.data[uJ] * BigInteger::kBase + u.data[uJ - 1]) % b.data[n - 1];
		while (r < BigInteger::kBase) {
			temp2 = b.data[n - 2] * qGuess;
			temp1 = r * BigInteger::kBase + u.data[uJ - 2];
			if ((temp2 > temp1) || (qGuess == BigInteger::kBase)) {
				--qGuess;                 //не работает, qGuess уменьшим
				r += b.data[n - 1];       //новый остаток
			}
			else break;     //qGuess правильное или на единицу больше
		}
		//U - B * qGuess начиная с vJ+i позиции
		carry = 0, borrow = 0;
		ui *uShift = &u.data[vJ];
		for (i = 0; i < n; ++i) {
			temp1 = b.data[i] * qGuess + carry;
			carry = temp1 / BigInteger::kBase;
			temp1 -= carry * BigInteger::kBase;
			temp2 = uShift[i] - temp1 + borrow;
			if (temp2 < 0) {
				uShift[i] = temp2 + BigInteger::kBase;
				borrow = -1;
			}
			else {
				uShift[i] = temp2;
				borrow = 0;
			}
		}
		temp2 = uShift[i] - carry + borrow;
		if (temp2 < 0) {
			uShift[i] = temp2 + BigInteger::kBase;
			borrow = -1;
		}
		else {
			uShift[i] = temp2;
			borrow = 0;
		}
		if (borrow == 0) res.data[vJ] = qGuess;
		else { //qGuess на 1 больше нужного
			res.data[vJ] = qGuess - 1;
			carry = 0;
			for (i = 0; i < n; ++i) {
				temp = uShift[i] + b.data[i] + carry;
				if (temp >= BigInteger::kBase) {
					uShift[i] = temp - BigInteger::kBase;
					carry = 1;
				}
				else {
					uShift[i] = temp;
					carry = 0;
				}
			}
			uShift[i] = uShift[i] + carry - BigInteger::kBase;
		}
		u.make_right();
	}
	res.make_right();
	if (scale > 1) {
		std::pair<BigInteger, ui> p = sDiv(b, scale);
		b = p.first;
		p = sDiv(u, scale);
		cur = p.first;
	}
	else cur = u;
}

BigInteger &BigInteger::operator/=(BigInteger const &rhs) {
	BigInteger b = rhs, res, cur;
	myDiv(*this, b, res, cur);
	return res;
}

BigInteger &BigInteger::operator%=(BigInteger const &rhs) {
	BigInteger b = rhs, res, cur;
	myDiv(*this, b, res, cur);
	return cur;
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
	make_right();
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
	}
	else {
		for (auto i = (size_t)-rhs; i < data.size(); ++i) {
			data[i + rhs] = data[i];
		}
		for (size_t i = data.size(); i > data.size() + rhs; --i) {
			data[i - 1] = getEmptyCell();
		}
		make_right();
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
	make_right();
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
	make_right();
	return *this;
}

BigInteger BigInteger::operator~() const {
	BigInteger r = *this;
	if (data.empty()) {
		r.data.push_back(0);
	}
	for (size_t i = 0; i < r.data.size(); ++i) {
		r.data[i] = ~r.data[i];
	}
	r.make_right();
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


std::string to_string(BigInteger const &a) {
	return a.to_string();
}

std::ostream &operator<<(std::ostream &s, BigInteger const &a) {
	return s << a.to_string();
}

std::istream &operator>>(std::istream &s, BigInteger &a) {
	std::string aux;
	s >> aux;
	a = BigInteger(aux);
	return s;
}

bool operator==(BigInteger& a, const BigInteger& b) {
	return (a == b);
}

bool operator<=(BigInteger& a, const BigInteger& b) {
	return (a <= b);
}
