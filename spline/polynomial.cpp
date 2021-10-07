#include "polynomial.h"

double Polynomial::operator()(double x) {
    double y = 0;
    double t = 1;
    for(int p = 0; p <= degree(); p++) {
        y += t*coeff[p];
        t *= x;
    }
    return y;
}

Polynomial operator+(Polynomial a, Polynomial b) {
    int z = max(a.degree(), b.degree());
    vector<double> o = vector<double>(z + 1);

    for(int i = 0; i <= z; i++) {
        o[i] = a.getCoeff(i) + b.getCoeff(i);
    }
    return Polynomial(o);
}

Polynomial operator*(Polynomial a, Polynomial b) {
    vector<double> o = vector<double>(a.degree() + b.degree() + 2);
    for(int x = 0; x <= a.degree(); x++) {
         for(int y = 0; y <= b.degree(); y++) {
            o[x+y] += a.getCoeff(x) * b.getCoeff(y);
         }
    }
    return Polynomial(o);
}

Polynomial Polynomial::derivative() {
    int z = degree();
    if(z == -1)
        return *this;
    auto o = vector<double>(z);
    for (int x = 1; x <= z; x++) {
        o[x-1] = x*getCoeff(x);
    }

    return Polynomial(o);
}

bool operator<(Polynomial lhs, Polynomial rhs) {
    return lhs.degree() < rhs.degree();
}

ostream& operator<<(ostream& os, const Polynomial& p) {
    for(double c : p.coeff)
        os << c << ',';
    return os;
}

int Polynomial::degree() {
    trim();
    return coeff.size()-1;
}

bool Polynomial::empty() {
    for(int i = 0; i <= degree(); i++) {
        if(coeff[i] != 0.0)
            return false;
    }
    return true;
}

void Polynomial::trim() {
    while(coeff.size() > 1 and coeff.back() == 0.0) {
        coeff.pop_back();
    }
}


double curvature(Polynomial first, Polynomial second, double x) {
    auto num = abs(second(x));
    auto denom = first(x);

    denom = 1 + denom*denom;
    denom = denom * denom * denom;
    denom = sqrt(denom);
    return num/denom;
}
