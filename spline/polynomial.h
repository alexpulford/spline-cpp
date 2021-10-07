#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <cmath>
#include <iostream>
#include "linalg.h"
using namespace linalg;
using namespace linalg::aliases;

using namespace std;

class Polynomial
{
private:
    vector<double> coeff;

    void trim();
public:
    Polynomial(vector<double> coefficients) : coeff(coefficients) { };

    bool empty();

    int degree();

    double& operator[](std::size_t idx) {
        return coeff[idx];
    }

    double getCoeff(int i) {
        if(i < coeff.size())
            return coeff[i];
        else
            return 0.0;
    }
    double operator()(double x);

    friend ostream& operator<<(ostream& os, const Polynomial& p);

    Polynomial derivative();
};

bool operator<(Polynomial lhs, Polynomial rhs);
Polynomial operator+(Polynomial a, Polynomial b);
Polynomial operator*(Polynomial a, Polynomial b);

double curvature(Polynomial first, Polynomial second, double x);

#endif // POLYNOMIAL_H
