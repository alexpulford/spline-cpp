#ifndef SPLINE_H
#define SPLINE_H

#include <vector>
#include <assert.h>
#include <iostream>

#include "linalg.h"
#include "polynomial.h"

using namespace linalg;
using namespace linalg::aliases;
using namespace std;

class SplineType {
public:
    static const int none = 0;
    static const int closed = 1;
    static const int uniform = 2;
};

class Spline
{
public:
    Spline();
    void print();
    void recompute();
    void computeSpans();
    void constantBasis();
    void interpolateBasis();
    void computeBasis(int finalOrder);

    int knotCount() { return t.size(); }
    int basisCount() { return n.size(); }
    int spanCount() { return spans.size(); }
    bool isValid() { return knotCount()-1>2*order; }
    Polynomial functionAt(int basis, int span) { return n[basis][span];}
    bool functionExists(int basis, int span) { return !functionAt(basis,span).empty() and spanExists(span); }
    bool spanExists(int span) { return spans[span].x-spans[span].y != 0; }

    int getType();
    int2 getMultiplicity();
    int getContinuity();
    double2 getValidSpan();

    void makeUniform();
    void makeOpen();
    void makeClosed();

    int order;
    vector<double> t; //knots
    vector<vector<Polynomial>> n; //basis -> span -> polynomial
    vector<double2> spans;
    Polynomial lerp(int i, int p, bool inverse=false);
};

#endif // SPLINE_H
