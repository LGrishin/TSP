#pragma once

#include "commons.h"
#include "data_model.h"
#include "parser.h"
#include "svg_dumper.h"
#include <fstream>
#include <algorithm>
#include <glpk.h>

namespace core
{

Distance distance(const Location& first, const Location& second);

class Solver
{
public:
    Solver() = default;

    Solution getSolution() const;
    void read(const Filename&, const Filenames&, size_t);
    virtual void solve() = 0;
    void dump(const Filename&);
protected:
    RegionsByName regions_;
    Cities cities_;
    Way way_;
    Graph matrix_;
};

}