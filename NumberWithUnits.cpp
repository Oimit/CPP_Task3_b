#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "NumberWithUnits.hpp"
using namespace std;
namespace ariel
{
    const double EPSILON = 0.001;
    static map<string,map<string, double>> mConvertorMapUnits;

    NumberWithUnits::NumberWithUnits(double amount, const string &unit)
    {
        auto search = mConvertorMapUnits.find(unit);
        if (search == mConvertorMapUnits.end())
        {
			throw invalid_argument("This unit doesnt exist.");
		}
            this->amount = amount;
            this->unit = unit;
    }

	void NumberWithUnits::read_units(ifstream &units_file)
	{
		string line, leftUnit, signEqual, rightUnit;
		double leftNumber = 0, rightNumber = 0;
		while (getline(units_file, line))
		{
			stringstream lineStremHelperExtractor(line);
			lineStremHelperExtractor >> skipws >> leftNumber;
			lineStremHelperExtractor >> skipws >> leftUnit;
			lineStremHelperExtractor >> skipws >> signEqual;
			lineStremHelperExtractor >> skipws >> rightNumber;
			if (rightNumber == 0)
			{
				throw invalid_argument("Unit Number cann'ot be 0");
			}
			lineStremHelperExtractor >> skipws >> rightUnit;
			if (leftNumber == 0 || rightNumber == 0)
			{
				throw invalid_argument("Unit Number cann'ot be 0");
			}
			if (signEqual != "=")
			{
				throw invalid_argument("Sign should be {=} and");
			}
			mConvertorMapUnits[leftUnit][rightUnit] = rightNumber;
			mConvertorMapUnits[rightUnit][leftUnit] = 1 / rightNumber;
			for (auto &itr : mConvertorMapUnits.at(rightUnit))
			{
				double itr_d = itr.second;
				double unitMul = mConvertorMapUnits.at(leftUnit).at(rightUnit) * itr_d;
	
				mConvertorMapUnits[leftUnit][itr.first] = unitMul;
				mConvertorMapUnits[itr.first][leftUnit] = 1 / unitMul;
			}

			for (auto &itr : mConvertorMapUnits.at(leftUnit))
			{
				double itr_d = itr.second;
				double unitMul = mConvertorMapUnits.at(rightUnit).at(leftUnit) * itr_d;

				mConvertorMapUnits[rightUnit][itr.first] = unitMul;
				mConvertorMapUnits[itr.first][rightUnit] = 1 / unitMul;
			}
		}
	}

    double NumberWithUnits::getNumberToConvertSrcToDest(const string &src, const string &dest)
    {
        if (src == dest)
        {
            return 1;
        }
        try
        {
            return mConvertorMapUnits.at(src).at(dest);
        }
        catch (...)
        {
            throw invalid_argument("Units do not match - [" + src + "] cannot be converted to [" + dest + "]");
        }
    }

    NumberWithUnits NumberWithUnits::operator+() const
    {
        return *this;
    }

    NumberWithUnits operator*=(double num, const NumberWithUnits &num1)
    {
        return num1;
    }

    NumberWithUnits operator*=(const NumberWithUnits &num1, double num)
    {
        NumberWithUnits res = num1;
        res.amount = num * res.amount;
        res.unit = num1.unit;
        return res;
    }

	string trim(const string& str)
	{
		size_t first = str.find_first_not_of(' ');
		if (string::npos == first)
		{
			return str;
		}
		size_t last = str.find_last_not_of(' ');
		return str.substr(first, (last - first + 1));
	}
 
    istream &operator>>(istream &is, NumberWithUnits &num)
    {
		string temp;
		getline(is, temp, '[');
		num.amount = stod(trim(temp));
		getline(is, temp, ']');
		string actualUnit = trim(temp);
		auto search = mConvertorMapUnits.find(actualUnit);
		if (search == mConvertorMapUnits.end())
		{
			throw invalid_argument("This unit doesnt exist.");
		}	
		num.unit = actualUnit;
		return is;
    }

	NumberWithUnits NumberWithUnits::operator-() const
	{
		return NumberWithUnits{ -this->amount,this->unit };
	}

	ostream &operator<<(ostream &os, const NumberWithUnits &num)
	{
		return os << num.amount << "[" << num.unit << "]";
	}

	NumberWithUnits operator*(double num, const NumberWithUnits &num1)
	{
		return NumberWithUnits{ num* num1.amount,num1.unit };
	}

	NumberWithUnits operator*(const NumberWithUnits &num1, double num)
	{
		return NumberWithUnits{ num* num1.amount,num1.unit };
	}

	NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits &other) const
	{
		double num = getNumberToConvertSrcToDest(other.unit, this->unit);
		return NumberWithUnits{ (num*other.amount) + this->amount , this->unit };
	}

	NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits &other) const
	{

		double num = getNumberToConvertSrcToDest(other.unit, this->unit);
		return NumberWithUnits{ this->amount-(num*other.amount)  , this->unit };
	}

	bool operator>(const NumberWithUnits& left, const NumberWithUnits& right) {
		double resultRight = right.amount * NumberWithUnits::getNumberToConvertSrcToDest(right.unit, left.unit);
		return left.amount > resultRight;
	}

	bool operator<(const NumberWithUnits& left, const NumberWithUnits& right) {
		double resultRight = right.amount * NumberWithUnits::getNumberToConvertSrcToDest(right.unit, left.unit);
		return left.amount < resultRight;
	}

	bool operator>=(const NumberWithUnits& left, const NumberWithUnits& right) {
		return left > right || left == right;
	}

	bool operator<=(const NumberWithUnits& left, const NumberWithUnits& right) {
		return left < right || left == right;
	}

	bool operator==(const NumberWithUnits& left, const NumberWithUnits& right) {
		double resultRight = right.amount * NumberWithUnits::getNumberToConvertSrcToDest(right.unit, left.unit);
		return (abs(left.amount - resultRight) < EPSILON);
	
	}

	bool operator!=(const NumberWithUnits& left, const NumberWithUnits& right) {
		return !(left == right);
	}

	NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits& other) {
		double num = getNumberToConvertSrcToDest(other.unit, this->unit);
		this->amount += other.amount*num;
		return *this;
	}

	NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits &other)
	{
		double num = getNumberToConvertSrcToDest(other.unit, this->unit);
		this->amount -= other.amount*num;
		return *this;
	}

	NumberWithUnits &NumberWithUnits::operator++()
	{
		this->amount += 1;
		return *this;
	}

	NumberWithUnits NumberWithUnits::operator++(int)
	{
		return NumberWithUnits(this->amount++, this->unit);
	}

	NumberWithUnits &NumberWithUnits::operator--()
	{
		this->amount -= 1;
		return *this;
	}

	NumberWithUnits NumberWithUnits::operator--(int)
	{

		return NumberWithUnits(this->amount--, this->unit);
	}
}