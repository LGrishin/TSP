#pragma once
#include "core.h"

namespace core
{

class MILPSolver : public Solver
{
public:
    MILPSolver() : Solver() {}
    void solve() override;
};
   
}