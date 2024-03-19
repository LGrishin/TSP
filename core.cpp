#include "core.h"
#include "commons.h"

namespace core
{

using Angle = double;

void Solver::read(const Filename& citiesFilename, const Filenames& regionsFileNames, size_t citiesCount)
{
    Parser parser;
    std::ifstream citiesFs(citiesFilename);
    parser.parseCities(citiesFs, &cities_, citiesCount);
    for (const auto& fn: regionsFileNames)
    {
        std::ifstream regionsFsCount(fn);
        std::ifstream regionsFs(fn);
        size_t linesCount = std::count(std::istreambuf_iterator<char>(regionsFsCount), std::istreambuf_iterator<char>(), '\n');
        Region region = parser.parseRegion(regionsFs, linesCount);
        regions_.insert({region.getName(), region});
    }
    matrix_.resize(cities_.size());
    for (size_t i = 0; i < matrix_.size(); ++i)
    {
        matrix_[i].resize(cities_.size());
    }

    for (size_t i = 0; i < matrix_.size(); ++i)
    {
        for (size_t j = 0; j < matrix_[i].size(); ++j)
        {
            matrix_[i][j] = distance(cities_[i], cities_[j]);
        }
    }
}

Solution Solver::getSolution() const
{
    return {way_, regions_};
}

void Solver::dump(const Filename& filename)
{
    SVG dumper;
    std::ofstream fs(filename);
    dumper.render(fs, getSolution());
}

}
