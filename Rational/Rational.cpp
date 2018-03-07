// Garrett McDonnell
// ID: 3021287

#include <iostream>

using std::ostream;
using std::istream;
using std::cout;
using std::cin;
using std::endl;

class Rational
{
	int num;
	int denom;
	
	public:
		Rational() : num(1),denom(1) {};
		Rational(int a,int b);
		int getNum() {return num;}
		int getDenom() {return denom;}
		void reduce();
		bool operator == (Rational r);
		bool operator != (Rational r);
		bool operator < (Rational r);
		bool operator <= (Rational r);
		bool operator > (Rational r);
		bool operator >= (Rational r);
		void operator + (int n);
		void operator - (int n);
		void operator * (int n);
		void operator / (int n);
		friend ostream& operator << (ostream& obj, Rational r);
		friend istream& operator >> (istream& obj, Rational& r);
		
};

int main ()
{
	
}

Rational::Rational(int a, int b) : num(a),denom(b)
{
	if (denom==0)
		throw;
	reduce();
}

bool Rational::operator == (Rational r)
{
	if (r.getNum()==num && r.getDenom()==denom)
		return true;
	return false;
}

bool Rational::operator != (Rational r)
{
	if (*this==r)
		return false;
	return true;
}

bool Rational::operator < (Rational r)
{
	double a=(static_cast <double> (num))/denom;
	double b=(static_cast <double> (r.num))/r.denom;
	if (a<b)
		return true;
	else
		return false;
}

bool Rational::operator <= (Rational r)
{
	double a=(static_cast <double> (num))/denom;
	double b=(static_cast <double> (r.num))/r.denom;
	if (a<=b)
		return true;
	else
		return false;
}

bool Rational::operator > (Rational r)
{
	double a=(static_cast <double> (num))/denom;
	double b=(static_cast <double> (r.num))/r.denom;
	if (a>b)
		return true;
	else
		return false;
}

bool Rational::operator >= (Rational r)
{
	double a=(static_cast <double> (num))/denom;
	double b=(static_cast <double> (r.num))/r.denom;
	if (a>=b)
		return true;
	else
		return false;
}

void Rational::operator + (int n)
{
	num+=n*denom;
	reduce();
}

void Rational::operator - (int n)
{
	num-=n*denom;
	reduce();
}

void Rational::operator * (int n)
{
	num*=n;
	reduce();
}

void Rational::operator / (int n)
{
	denom*=n;
	reduce();
}

ostream& operator << (ostream& obj, Rational r)
{
	if (r.getDenom()==1)
		obj << r.getNum();
	else
		obj << r.getNum() << '/' << r.getDenom();
	return obj;
}

istream& operator >> (istream& obj, Rational& r)
{
	obj >> r.num >> r.denom;
	return obj;
}

void Rational::reduce()
{
	int i=2;
	while (i<num || i<denom)
	{
		while (num%i==0 && denom%i==0)
			{
				num/=i;
				denom/=i;
			}
		++i;
	}
}
