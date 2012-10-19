/*=============================================================================
author        :Walter Schreppers
filename      :numbers.h
description   :This is actually a value class (stores numbers,strings from 
               results in expressions). 
=============================================================================*/



//typedef double number;
//number class to store variables ...

#ifndef NUMBER_H
#define NUMBER_H


#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class Number{
  public:
    
    //constructor/destructor
    //======================
    Number();
    Number(const Number&);
    ~Number(){}
    
    
    //public members
    //==============
    void toString(); //explicit conversion to string strVal
    void toDouble(); //explicit conversion to double val
    
    //operators
    //=========
    Number& operator=(const Number&);
    Number& operator=(const string&);
    Number& operator=(double);

    Number& operator+(const Number&);
    Number& operator-(const Number&);
    Number& operator*(const Number&);
    Number& operator/(const Number&);

    bool operator==(const Number&) const;
    bool operator!=(const Number&) const;
    bool operator<(const Number&) const;
    bool operator<=(const Number&) const;
    bool operator>(const Number&) const;
    bool operator>=(const Number&) const;

    friend ostream& operator<<(ostream&, const Number&); //outputs double or string
    friend istream& operator>>(istream&, Number&); //reads double
  

    //public members  
    bool bString; //set to true if Number represents a string...
    string strVal;
    double val;

};

#endif

