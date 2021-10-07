#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <set>
#include "linalg.h"

struct Controller {
    int basis;
    int span;
    std::set<linalg::aliases::int2> enabled;
    Controller(): basis(0), span(0), enabled() {}
};

#endif // CONTROLLER_H
