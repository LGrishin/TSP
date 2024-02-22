#pragma once
#include "commons.h"
#include <istream>
#include "data_model.h"

namespace core
{

class Parser
{

public:
    Parser() = default;
    void parseCities(IStream&, Cities*, size_t) const;
    Region parseRegion(IStream&, size_t) const;
private:
    SizeType linesCount_;
};


}