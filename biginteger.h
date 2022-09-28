#include <vector>
#include <iostream>
#include <string>

class BigInteger{
public:
    BigInteger(const int x = 0);
    BigInteger(const std::string &s);
    BigInteger &operator=(const BigInteger &x);
    BigInteger &operator+=(const BigInteger &x);
    BigInteger &operator-=(const BigInteger &b);
    BigInteger &operator*=(const BigInteger &b);
    BigInteger &operator/=(const BigInteger &b);
    BigInteger &operator%=(const BigInteger &b);
    BigInteger &operator++();
    BigInteger operator++(int);
    BigInteger &operator--();
    BigInteger operator--(int);
    BigInteger operator-();
    std::string toString() const;
    explicit operator bool() const;

    friend bool operator<(const BigInteger &a, const BigInteger &b);
    friend bool mod_comparison_less(const BigInteger &a, const BigInteger &b);
    friend std::istream &operator>>(std::istream &in, BigInteger &x);
    friend void swap(BigInteger &a,  BigInteger &b);
private:
    std::vector <int> digits;
    bool isnegative = false;
    void erase_leading_zeros();
    void sign_multiplication_division(const BigInteger &x);
    void shift();
    void zero_check();
    void addition(const BigInteger &x);
    void subtraction(const BigInteger &x);
};

bool operator<(const BigInteger &a, const BigInteger &b);
bool operator>(const BigInteger &a, const BigInteger &b);
bool operator==(const BigInteger &a, const BigInteger &b);
bool operator!=(const BigInteger &a, const BigInteger &b);
bool operator<=(const BigInteger &a, const BigInteger &b);
bool operator>=(const BigInteger &a, const BigInteger &b);
BigInteger operator+(const BigInteger &a, const BigInteger &b);
BigInteger operator-(const BigInteger &a, const BigInteger &b);
BigInteger operator*(const BigInteger &a, const BigInteger &b);
BigInteger operator/(const BigInteger &a, const BigInteger &b);
BigInteger operator%(const BigInteger &a, const BigInteger &b);
std::ostream &operator<<(std::ostream &out, const BigInteger &x);


void swap(BigInteger &a, BigInteger &b){
    std::swap(a.digits, b.digits);
    std::swap(a.isnegative, b.isnegative);
}

BigInteger::BigInteger(const int x){
    BigInteger copy = std::to_string(x);
    swap(copy, *this);
}

BigInteger::BigInteger(const std::string &str){
    std::string s = str;
    if (s.size() == 0){
        digits.push_back(0);
        isnegative = false;
    }
    int first_element = 0;
    if (s[0] == '-'){
        isnegative = true;
        first_element = 1;
    }
    int index = s.size() - 1;
    while (index >= first_element){
        if ((index - first_element + 1) <= 9)
            digits.push_back(std::stoi(str.substr(first_element, index - first_element + 1)));
        else{
            digits.push_back(std::stoi(str.substr(index - 8, 9)));
        }
        index -= 9;
    }

    erase_leading_zeros();
}

BigInteger &BigInteger::operator=(const BigInteger &x) {
    BigInteger new_x = x;
    swap(new_x, *this);
    return *this;
}

void BigInteger::addition(const BigInteger &x){
    for (size_t i = 0; i <= (digits.size() - 1); i++){
        if (i <= (x.digits.size() - 1)){
            if ((digits[i] / 100000000 + x.digits[i] / 100000000) < 10){
                digits[i] += x.digits[i];
            }
            else{
                digits[i] = static_cast<long long> (digits[i] + x.digits[i]) - 1000000000;
                if (i != digits.size()){
                    digits[i + 1] += 1;
                }
                else{
                    digits.push_back(1);
                }
            }
        }
    }
    if (digits.size() < x.digits.size()){
        for (size_t i = digits.size(); i <= x.digits.size() - 1; i++){
            digits.push_back(i);
        }
    }
}

void BigInteger::subtraction(const BigInteger &x){
    if (mod_comparison_less(x, *this) || x == *this) {
        int mind = 0;
        for (size_t i = 0; i <= (digits.size() - 1); i++) {
            if (i <= (x.digits.size() - 1)) {
                if ((digits[i] - mind) >= x.digits[i]) {
                    digits[i] = digits[i] - x.digits[i] - mind;
                    mind = 0;
                } else {
                    digits[i] = digits[i] + 1000000000 - mind - x.digits[i];
                    mind = 1;
                }
            } else {
                if (i == digits.size() - 1)
                    digits[i] = digits[i] - mind;
                else if ((digits[i] - mind < 0)) {
                    digits[i] = digits[i] + 1000000000 - mind;
                    mind = 1;
                } else
                    mind = 0;
            }
        }
    }
    else {
        int mind = 0;
        digits.resize(x.digits.size());
        for (size_t i = 0; i <= (x.digits.size() - 1); i++) {
            if (i <= (digits.size() - 1)) {
                if ((x.digits[i] - mind) > digits[i]) {
                    digits[i] = x.digits[i] - digits[i] - mind;
                    mind = 0;
                } else {
                    digits[i] = x.digits[i] + 1000000000 - mind - digits[i];
                    mind = 1;
                }
            } else {
                if (i == x.digits.size() - 1)
                    digits[i] = x.digits[i] - mind;
                else if ((x.digits[i] - mind < 0)) {
                    digits[i] = x.digits[i] + 1000000000 - mind;
                    mind = 1;
                } else
                    mind = 0;
            }
        }
    }
}

BigInteger &BigInteger::operator+=(const BigInteger &x){
    if (isnegative == x.isnegative){
        if (x.digits.size() > digits.size())
            digits.resize(x.digits.size());
        addition(x);
    }
    else{
        if ((mod_comparison_less(*this, x) && !isnegative) || (!mod_comparison_less(*this, x) && isnegative))
            isnegative = true;
        else
            isnegative = false;
        subtraction(x);
    }
    zero_check();
    return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &x){
    if (isnegative == x.isnegative){
        if ((mod_comparison_less(*this, x) && !isnegative) || (!mod_comparison_less(*this, x) && isnegative))
            isnegative = true;
        else
            isnegative = false;
        subtraction(x);
    }
    else{
        if (x.digits.size() > digits.size())
            digits.resize(x.digits.size());
        addition(x);
    }
    zero_check();
    return *this;
}

BigInteger &BigInteger::operator*=(const BigInteger &x){
    if ((digits.size() == 1 && digits[0] == 0) || (x.digits.size() == 1 && x.digits[0] == 0) || digits.size() == 0 || x.digits.size() == 0) {
        isnegative = false;
        return *this = 0;
    }
    BigInteger ans;
    int n = static_cast<int>(digits.size());
    int m = static_cast<int>(x.digits.size());

    ans.digits.resize(n + m);
    for (int i = 0; i <= n - 1; i++) {
        long long  mind = 0;
        for (int j = 0; j <= m - 1; j++) {
            long long current = 0;
            if (j <= m - 1)
                current = static_cast<long long>(ans.digits[i + j]) + static_cast<long long>(digits[i]) * static_cast<long long>(x.digits[j]) + mind;
            else
                current = mind;
            ans.digits[i + j] = (current % 1000000000);
            mind = (current / 1000000000);
        }
        if (mind != 0)
            ans.digits[i + m] = (mind % 1000000000);

    }
    ans.erase_leading_zeros();
    sign_multiplication_division(x);
    std::swap(digits, ans.digits);
    return *this;
}

BigInteger &BigInteger::operator/=(const BigInteger &x){
    if (*this == 0 or x == 0) {
        return *this = 0;
    }
    BigInteger ans;
    BigInteger dividend;

    ans.digits.resize(digits.size());
    int result;
    for (size_t i = digits.size(); i >= 1; i--) {
        dividend.shift();
        dividend.digits[0] = digits[i - 1];
        dividend.erase_leading_zeros();
        result = 0;
        int l = 0, r = 999999999;
        while (l <= r) {
            int m = (l + r) / 2;
            BigInteger current;
            if (x > 0)
                current = x * m;
            else
                current = x * m * -1;
            if (current <= dividend) {
                result = m;
                l = m + 1;
            }
            else r = m - 1;
        }

        ans.digits[i - 1] = result;
        if (x > 0)
            dividend = dividend - x * result;
        else
            dividend = dividend - x * result * -1;
    }

    sign_multiplication_division(x);
    std::swap(ans.digits, digits);
    erase_leading_zeros();
    return *this;
}

BigInteger &BigInteger::operator%=(const BigInteger &x){
    BigInteger t = *this;
    *this = t - ((t / x) * x);
    erase_leading_zeros();
    zero_check();
    return *this;
}

BigInteger &BigInteger::operator++(){
    *this += 1;
    zero_check();
    return *this;
}

BigInteger BigInteger::operator++(int){
    BigInteger &copy = *this;
    *this += 1;
    zero_check();
    return copy;
}

BigInteger &BigInteger::operator--(){
    *this -= 1;
    zero_check();
    return *this;
}

BigInteger BigInteger::operator--(int){
    BigInteger &copy = *this;
    *this -= 1;
    zero_check();
    return copy;
}

BigInteger BigInteger::operator-(){
    BigInteger copy = *this;
    copy.isnegative = !isnegative;
    if (digits.size() == 0 || (digits.size() == 1 && digits[0] == 0))
        copy.isnegative = false;
    return copy;
}

std::string BigInteger::toString() const {
    std::string s;
    if (digits.size() == 0){
        s = "0";
    }
    if (isnegative){
        s += "-";
    }
    int digits_sz = static_cast<int>(digits.size());
    for (int i = digits_sz - 1; i >= 0; i--){
        std::string add = std::to_string(digits[i]);
        while ((add.size() < 9) && (i != digits_sz - 1)){
            add = "0" + add;
        }
        s += add;
    }
    return s;
}

bool operator<(const BigInteger &a, const BigInteger &b){
    if (a.isnegative && !b.isnegative)
        return true;
    else if (!a.isnegative && b.isnegative)
        return false;
    else if (!a.isnegative && a.digits.size() > b.digits.size())
        return false;
    else if (a.isnegative && a.digits.size() > b.digits.size())
        return true;
    else if (a.isnegative && a.digits.size() < b.digits.size())
        return false;
    else if (!a.isnegative && a.digits.size() < b.digits.size())
        return true;
    else{
        int i = a.digits.size() - 1;
        while (i >= 0){
            if (a.digits[i] < b.digits[i]){
                return true;
            } else if (a.digits[i] > b.digits[i]){
                return false;
            } else{
                i--;
            }
        }
        return false;
    }
}

bool operator>(const BigInteger &a, const BigInteger &b){
    if (b < a)
        return true;
    return false;
}

bool operator==(const BigInteger &a, const BigInteger &b){
    if (!(a < b) && !(a > b))
        return true;
    return false;
}

bool operator!=(const BigInteger &a, const BigInteger &b){
    if (!(a == b))
        return true;
    return false;
}

bool operator<=(const BigInteger &a, const BigInteger &b){
    if ((a < b) || (a == b))
        return true;
    return false;
}

bool operator>=(const BigInteger &a, const BigInteger &b){
    if ((a > b) || (a == b))
        return true;
    return false;
}

BigInteger operator+(const BigInteger &a, const BigInteger &b) {
    BigInteger copy = a;
    copy += b;
    return copy;
}

BigInteger operator-(const BigInteger &a, const BigInteger &b) {
    BigInteger copy = a;
    copy -= b;
    return copy;
}

BigInteger operator*(const BigInteger &a, const BigInteger &b) {
    BigInteger copy = a;
    copy *= b;
    return copy;
}

BigInteger operator/(const BigInteger &a, const BigInteger &b) {
    BigInteger copy = a;
    copy /= b;
    return copy;
}

BigInteger operator%(const BigInteger &a, const BigInteger &b) {
    BigInteger copy = a;
    copy %= b;
    return copy;
}

bool mod_comparison_less(const BigInteger &a, const BigInteger &b){
    if (a.digits.size() > b.digits.size())
        return false;
    else if (a.digits.size() < b.digits.size())
        return true;
    else{
        int i = a.digits.size() - 1;
        while (i != -1){
            if (a.digits[i] < b.digits[i]){
                return true;
            } else if (a.digits[i] > b.digits[i]){
                return false;
            } else{
                i--;
            }
        }
        return false;
    }

}

void BigInteger::zero_check(){
    if (digits.size() == 0 || (digits.size() == 1 && digits[0] == 0))
        isnegative = false;
}

void BigInteger::erase_leading_zeros(){
    while ((digits.size() > 1) && (digits[digits.size() - 1] == 0)){
        digits.pop_back();
    }
    if ((digits.size() == 1) && (digits[0] == 0)){
        isnegative = false;
    }
}

void BigInteger::sign_multiplication_division(const BigInteger &x){
    if (isnegative == x.isnegative)
        isnegative = false;
    else
        isnegative = true;
}

void BigInteger::shift() {
    if (digits.empty())
        digits.push_back(0);
    else {
        digits.push_back(digits[digits.size() - 1]);
        for (size_t i = digits.size() - 2; i > 0; i--){
            digits[i] = digits[i - 1];
        }
        digits[0] = 0;
    }
}

std::ostream &operator<<(std::ostream &out, const BigInteger &bi) {
    out << bi.toString();
    return out;
}

std::istream &operator>>(std::istream &in, BigInteger &x) {
    std::string s;
    in >> s;
    x.digits.clear();
    x = BigInteger(s);
    return in;
}

BigInteger::operator bool() const {
    if (*this == 0 || digits.size() == 0)
        return false;
    return true;
}


class Rational{
public:
    Rational(const BigInteger x, const BigInteger y);
    Rational(const int x = 0, const int y = 1);
    Rational(const BigInteger &x);
    Rational &operator=(const Rational &x);
    Rational &operator+=(const Rational &x);
    Rational &operator-=(const Rational &x);
    Rational &operator*=(const Rational &x);
    Rational &operator/=(const Rational &x);
    Rational operator-();
    std::string toString() const;
    std::string asDecimal(size_t precision = 0) const;
    explicit operator double() const;
    friend bool operator<(const Rational &a, const Rational &b);
private:
    BigInteger numerator;
    BigInteger denominator;
    bool isnegative = false;
    static BigInteger nod(BigInteger a, BigInteger b);
};

Rational operator+(const Rational &a, const Rational &b);
Rational operator-(const Rational &a, const Rational &b);
Rational operator*(const Rational &a, const Rational &b);
Rational operator/(const Rational &a, const Rational &b);
std::ostream &operator<<(std::ostream &out, const Rational &x);
bool operator>(const Rational &a, const Rational &b);
bool operator<=(const Rational &a, const Rational &b);
bool operator>=(const Rational &a, const Rational &b);
bool operator==(const Rational &a, const Rational &b);
bool operator!=(const Rational &a, const Rational &b);


Rational::Rational(const BigInteger x, const BigInteger y){
    numerator = x;
    denominator = y;
    if (!(x < 0 && y < 0) && ((x < 0) || (y < 0))) {
        isnegative = true;
        if (x < 0)
            numerator = -1 * x;
        if (y < 0)
            denominator = -1 * y;
    }
    if (x < 0 && y < 0){
        numerator = -1 * x;
        denominator = -1 * y;
    }
}

Rational::Rational(const int x, const int y){
    numerator = x;
    denominator = y;
    if (!(x < 0 && y < 0) && ((x < 0) || (y < 0))) {
        isnegative = true;
        if (x < 0)
            numerator = -x;
        if (y < 0)
            denominator = -y;
    }
    if (x < 0 && y < 0){
        numerator = -x;
        denominator = -y;
    }
}

Rational::Rational(const BigInteger &x) {
    numerator = x;
    denominator = 1;
    if (numerator < 0) {
        isnegative = true;
        numerator = -numerator;
    }
}

Rational &Rational::operator=(const Rational &x){
    Rational new_x = x;
    std::swap(numerator, new_x.numerator);
    std::swap(denominator, new_x.denominator);
    std::swap(isnegative, new_x.isnegative);
    return *this;
}

Rational &Rational::operator+=(const Rational &x){
    if (isnegative != x.isnegative){
        if (numerator * x.denominator < x.numerator * denominator)
            isnegative = x.isnegative;
    }
    BigInteger add = x.numerator * denominator;
    denominator *= x.denominator;
    numerator *= x.denominator;
    if (isnegative == x.isnegative)
        numerator += add;
    else {
        numerator -= add;
        if (numerator < 0)
            numerator *= -1;
    }
    return *this;
}

Rational &Rational::operator-=(const Rational &x){

    BigInteger add = x.numerator * denominator;
    denominator *= x.denominator;
    numerator *= x.denominator;
    if (isnegative == x.isnegative) {
        numerator -= add;
        if (numerator < 0)
            numerator *= -1;
    }
    else
        numerator += add;

    if (isnegative == x.isnegative){
        if (numerator * x.denominator < x.numerator * denominator)
            isnegative = !isnegative;
    }
    return *this;
}

Rational &Rational::operator*=(const Rational &x){
    numerator *= x.numerator;
    denominator *= x.denominator;
    BigInteger gcd = nod(numerator, denominator);
    numerator /= gcd;
    denominator /= gcd;
    if (isnegative == x.isnegative)
        isnegative = false;
    else
        isnegative = true;
    return *this;
}

Rational &Rational::operator/=(const Rational &x){
    numerator *= x.denominator;
    denominator *= x.numerator;
    BigInteger gcd = nod(numerator, denominator);
    numerator /= gcd;
    denominator /= gcd;
    if (isnegative == x.isnegative)
        isnegative = false;
    else
        isnegative = true;
    return *this;
}

Rational Rational::operator-(){
    Rational copy = *this;
    copy.isnegative = !isnegative;
    return copy;
}

Rational operator+(const Rational &a, const Rational &b){
    Rational copy = a;
    copy += b;
    return copy;
}

Rational operator-(const Rational &a, const Rational &b){
    Rational copy = a;
    copy -= b;
    return copy;
}

Rational operator*(const Rational &a, const Rational &b){
    Rational copy = a;
    copy *= b;
    return copy;
}

Rational operator/(const Rational &a, const Rational &b){
    Rational copy = a;
    copy /= b;
    return copy;
}

bool operator<(const Rational &a, const Rational &b){
    Rational difference = a;
    difference -= b;
    if (difference.isnegative && difference.numerator != 0 && difference.denominator != 0){
        difference = 0;
        return true;
    }
    else {
        difference = 0;
        return false;
    }
}

bool operator>(const Rational &a, const Rational &b){
    if (b < a)
        return true;
    return false;
}

bool operator==(const Rational &a, const Rational &b){
    if (!(a < b) && !(a > b))
        return true;
    return false;
}

bool operator!=(const Rational &a, const Rational &b){
    if (!(a == b))
        return true;
    return false;
}

bool operator<=(const Rational &a, const Rational &b){
    if ((a < b) || (a == b))
        return true;
    return false;
}

bool operator>=(const Rational &a, const Rational &b){
    if ((a > b) || (a == b))
        return true;
    return false;
}

std::string Rational::toString() const{
    std::string s = "";
    BigInteger nod_ = nod(numerator, denominator);
    BigInteger numerator_ = numerator / nod_;
    BigInteger denominator_ = denominator / nod_;
    if (isnegative)
        s += "-";
    if (denominator_ == 1) {
        s += numerator_.toString();
        return s;
    }
    s += numerator_.toString();
    s += "/";
    s += denominator_.toString();
    return s;
}

std::string Rational::asDecimal(size_t precision) const{
    std::string s = "";
    if (isnegative)
        s += "-";
    s += (numerator / denominator).toString();
    if (numerator % denominator == 0)
        return s;
    s += ".";
    BigInteger ten_degree(1);
    for(size_t i = 1; i <= precision; i++){
        ten_degree *= 10;
    }
    BigInteger add = ((numerator * ten_degree) / denominator) % ten_degree;
    if (add < 0)
        add = -add;

    BigInteger zeros_amount = precision - add.toString().size();
    while (zeros_amount > 0){
        s += "0";
        zeros_amount--;
    }
    s += add.toString();
    return s;
}

Rational::operator double() const {
    std::string result = asDecimal(100);
    return std::stod(result);
}

BigInteger Rational::nod(BigInteger a,  BigInteger b) {
    while (a != 0 && b != 0) {
        if (b > a)
            b %= a;
        else
            a %= b;
    }
    return a + b;
}

std::ostream & operator << (std::ostream & out, const Rational &a) {
    Rational x = a;
    out << x.asDecimal(10);
    return out;
}
