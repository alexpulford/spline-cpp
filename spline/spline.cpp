#include "spline.h"

double print(vector<double> p) {
    for( int i = 0; i < p.size(); i++) {
        std::cout << p[i] << ", ";
    }
    std::cout << "\n";
}

Spline::Spline() {
    t = vector<double>{0, 0, 0, 0.3, 0.5, 0.5, 0.6, 1, 1, 1};
    order = 2;

    recompute();
}

void Spline::recompute() {
    computeBasis(order);
    computeSpans();
}

void Spline::print() {
    for(size_t i = 0; i < n.size(); i++) {
        for(size_t j = 0; j < n[i].size(); j++) {
            cout << "<";
            auto l = n[i][j].degree();
            for(int p = 0; p <= l; p++) {
                cout << n[i][j][p] << " ";
            }
            cout << "> ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void Spline::constantBasis() {
    order = 0;
    size_t z = t.size()-1;
    n = vector<vector<Polynomial>>();
    if(z < 1)
        return;
    for(size_t i = 0; i < z; i++) {
        n.emplace_back(vector<Polynomial>());
        for(size_t k = 0; k < z; k++){
            n[i].emplace_back(Polynomial({i == k ? 1.0 : 0.0}));
        }
    }
}

void Spline::computeBasis(int finalOrder) {
    constantBasis();
    print();
    for(int k = 0; k < finalOrder; k++) {
        interpolateBasis();
    }
    print();
}

void Spline::interpolateBasis() {
    order++;
    int z = (int)t.size() - 1;
    if(z <= order)
        return;
    for(int r = 0; r < z; r++) {
        for(int i = 0; i < z-order; i++) {
            n[i][r] = (n[i][r] * lerp(i, order)) + (n[i+1][r] * lerp(i+1, order, true));
        }
    }
    n.pop_back();
}

void Spline::computeSpans() {
    spans = vector<double2>();
    if(t.size() == 0)
        return;
    for(size_t i = 0; i < t.size()-1; i++) {
        spans.emplace_back(double2{t[i], t[i+1]});
    }
}
int Spline::getType() {
    auto mul = getMultiplicity();
    int type = SplineType::none;
    if(mul.x == mul.y and mul.x == order+1)
        type |= SplineType::closed;

    int m = mul.x-1;
    double d = -1.0f;
    auto uniform = true;

    for(int i = 1; i < knotCount()-2*m; i++) {
        auto delta = t[i+m]-t[i+m-1];
        if(d == -1) {
            d = delta;
        }

        if(abs(d - delta) > 0.000001) {
            uniform = false;
            break;
        }
    }
    if(uniform)
        type |= SplineType::uniform;

    return type;
}

int2 Spline::getMultiplicity() {
    //get start multiplicity
    int start_m = 1;
    double start = t[0];
    for(int i = 1; i < knotCount(); i++) {
        if(t[i] == start)
            start_m++;
        else
            break;
    }
    int last = knotCount()-1;
    //get end multiplicity
    int end_m = 1;
    double end = t[last];
    for(int i = 1; i < knotCount(); i++) {
        if(t[last-i] == end)
            end_m++;
        else
            break;
    }

    return {start_m, end_m};
}

int Spline::getContinuity() {
    auto m = getMultiplicity().x;

    int max_c = 0;
    int c = 0;
    for(int i = m; i < knotCount()-m; i++) {
        if(abs(t[i] - t[i-1]) < 0.000001)
            c++;
        else
            c = 0;
        max_c = max(max_c, c);
    }

    c = order-max_c-1; //continuity is limited by multiplicity
    auto z = (order+2) - knotCount(); //continuity is limited by knot vector size
    if(z > 0)
        c = min(c, order-z);

    return max(c, 0);
}

double2 Spline::getValidSpan() {
    auto size = spanCount()-1;
    return {spans[order].x, spans[size-order].y};
}

void Spline::makeUniform() {

    auto m = getMultiplicity();

    int inner = knotCount()-2*(order+1);

    if(m.x == m.y and m.x == order+1) {
        auto d = 1.0/(inner+1);
        for(int i = 1; i <= inner; i++) {
            t[i+order] = d*i;
        }
    } else {
        auto d = 1.0/(knotCount()-1);
        for(int i = 0; i < knotCount(); i++) {
            t[i] = d*i;
        }
    }
}



void Spline::makeOpen() {
    auto m = getMultiplicity();

    int start_m = m.x;
    int end_m = m.y;

    //remove start multiplicity
    double delta = t[start_m]/start_m;
    for(int i = 0; i < start_m; i++) {
        t[i] = i*delta;
    }

    //remove end multiplicity
    int last = knotCount()-1;
    delta = (1-t[last-end_m])/end_m;
    for(int i = 0; i < end_m; i++) {
        t[last-i] = 1-(i*delta);
    }
}

void Spline::makeClosed() {
    int m = order + 1; //multiplicity
    int end = knotCount()-1;
    for(int i = 0; i < m; i++) {
        t[i] = 0;
        t[end-i] = 1;
    }
}

Polynomial Spline::lerp(int i, int p, bool inverse) {
    double a,b;
    double2 o = double2{0,0};
    if(t[i] != t[i+p]) {
        a = 1.0/(t[i+p] - t[i]);
        b = -t[i] * a;
        o = double2{a, b};
    }
    if(inverse) {
        o = double2{-o.x, 1-o.y};
    }
    return Polynomial(vector<double>{o.y, o.x});
}
