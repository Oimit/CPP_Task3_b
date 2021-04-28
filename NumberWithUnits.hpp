#include <iostream>
#include <string>
#include <map>
using namespace std;
namespace ariel
{

    class NumberWithUnits
    {

    private:
        double amount;
        string unit;

    public:
        NumberWithUnits(double amount, const string &unit);

        NumberWithUnits()
        {
            amount = 0;
            unit = " ";
        }
        static double getNumberToConvertSrcToDest(const string &src, const string &dest);

        static void read_units(ifstream &units_file);

        //Operators +, (+), +=, -, (-), -= .
        NumberWithUnits operator+(const NumberWithUnits &other) const;
        NumberWithUnits operator+() const;
        NumberWithUnits &operator+=(const NumberWithUnits &other);
        NumberWithUnits operator-() const;
        NumberWithUnits operator-(const NumberWithUnits &other) const;
        NumberWithUnits &operator-=(const NumberWithUnits &other);

        //Operators ++, -- .
        NumberWithUnits &operator++();   //prefix
        NumberWithUnits operator++(int); //postfix
        NumberWithUnits &operator--();   //prefix
        NumberWithUnits operator--(int); //postfix

        //Operators >, <, >=, <=, ==, != .
        friend bool operator>(const NumberWithUnits &left, const NumberWithUnits &right);
        friend bool operator<(const NumberWithUnits &left, const NumberWithUnits &right);
        friend bool operator<=(const NumberWithUnits &left, const NumberWithUnits &right);
        friend bool operator>=(const NumberWithUnits &left, const NumberWithUnits &right);
        friend bool operator==(const NumberWithUnits &left, const NumberWithUnits &right);
        friend bool operator!=(const NumberWithUnits &left, const NumberWithUnits &right);

        //Operators *, *= --> We write twice this operators so we can multiple from each side (because of the double).
        friend NumberWithUnits operator*(double num, const NumberWithUnits &num1);
        friend NumberWithUnits operator*(const NumberWithUnits &num1, double num);
        friend NumberWithUnits operator*=(double num, const NumberWithUnits &num1);
        friend NumberWithUnits operator*=(const NumberWithUnits &num1, double num);

        //Input & Output operations.
        friend ostream &operator<<(ostream &os, const NumberWithUnits &num);
        friend istream &operator>>(istream &is, NumberWithUnits &num);
    };
}