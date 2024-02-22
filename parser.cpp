#include "parser.h"
#include "commons.h"

namespace core
{

void Parser::parseCities(IStream& istream, Cities* cities, size_t linesCount) const
{
    for (SizeType count = 0; count < linesCount; ++count)
    {
        City city;
        istream >> city;
        cities->push_back(std::move(city));
    }
}

Region Parser::parseRegion(IStream& istream, size_t linesCount) const
{
    Name regionName;

    istream >> regionName;
    Region result(std::move(regionName));

    for (SizeType count = 0; count < linesCount; ++count)
    {
        Location location;
        istream >> location;
        result.addLocation(std::move(location));
    }
    return result;
}

}
