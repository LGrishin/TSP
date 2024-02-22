#include "core.h"
#include "commons.h"
#include <cmath>

namespace core
{

using Angle = double;

static constexpr core::Distance earth_radius = 6356.752;
static constexpr core::Distance radian = 0.0174533;

static void to_spherical(const Location& location, double& thetta, double& phi)
{
    thetta = (90 - location.getLatitude()) * radian;
    phi = (location.getLongitude()) * radian;
}

double distance(const Location& first, const Location& second)
{
    double thetta1, thetta2, phi1, phi2;
    to_spherical(first, thetta1, phi1);
    to_spherical(second, thetta2, phi2);
    Distance twoRadSq = 2 * earth_radius * earth_radius;
    return std::sqrt(twoRadSq - twoRadSq * (std::sin(thetta1) * std::sin(thetta2) * std::cos(phi1 - phi2) + std::cos(thetta1) * std::cos(thetta2)));
}

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
