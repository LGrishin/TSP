#pragma once
#include <filesystem>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>

namespace core
{
using Name = std::string;
using Latitude = double;
using Longitude = double;
using IStream = std::istream;
using OStream = std::ostream;
using SizeType = size_t;
using Filename = std::filesystem::path;
using Distance = double;

class Location;
class City;
class Region;
class Arc;

using Locations = std::vector<Location>;
using Cities = std::vector<City>;
using RegionsByName = std::unordered_map<Name, Region>;
using Arcs = std::vector<Arc>;
using Filenames = std::vector<Filename>;
using Distances = std::vector<Distance>;

using Graph = std::vector<Distances>;


}
