#pragma once
#include "commons.h"
#include <istream>
#include <cassert>
#include <algorithm>
#include <iostream>
namespace core
{

class Location
{
public:
    Location() = default;
    Location(Latitude, Longitude);
    friend IStream& operator>>(IStream&, Location&);
    
    Latitude getLatitude() const;
    Longitude getLongitude() const;

protected:
    Latitude latitude_;
    Longitude longitude_;
};

class City : public Location
{
public:
    City() = default;
    Name getName() const { return name_; }
    friend IStream& operator>>(IStream&, City&);
    friend std::ostream& operator<<(std::ostream&, const City&);
private:
    Name name_;
};

class Point
{
public:
    Point(double, double);
    double y() const { return x_; }
    double x() const { return y_; }
    double getX() const;
    double getY() const;
    void setX(double x) { x_ = x; }
    void setY(double y) { y_ = y; }
private:
    double x_, y_;
};

class Way
{
public:
    Way() = default;
    void add(City);
    Arcs getArcs() const;
    const Cities& getCities() const;
    void clear();
    Distance getTotalLength() const;
    friend std::ostream& operator<<(std::ostream&, const Way&);
    size_t size() const;
    void swapCities(size_t i, size_t j);
private:
    Cities cities_;
    Distance total_;
};

class Region
{
public:
    Region(Name&&);
    void addLocation(Location&&);
    Arcs getArcs() const;
    Name getName() const;
protected:
    Name name_;
    Locations locations_;
};

class Arc
{
public:
    Arc(const Location&, const Location&, size_t, size_t);
    Arc(Location&&, Location&&, size_t, size_t);
    const Location& getBegin() const;
    const Location& getEnd() const;
    size_t getBeginCity() const { return beginCity_; }
    size_t getEndCity() const { return endCity_; }

protected:
    Location begin_;
    Location end_;
    size_t beginCity_, endCity_;
};

class Solution
{
public:
    Solution() = default;
    Solution(const Way&, const RegionsByName&);
    const Way* getWay() const;
    const RegionsByName* getRegionsByName() const;
private:
    Way way_;
    RegionsByName regions_;
};


inline Location::Location(Latitude latitude, Longitude longitude) : latitude_(latitude), longitude_(longitude) {}

inline IStream& operator>>(IStream& istream, Location& location)
{
    istream >> location.latitude_ >> location.longitude_;
    return istream;
}

inline std::ostream& operator<<(std::ostream& out, const City& city)
{
    out << city.name_;
    return out;
}

inline Latitude Location::getLatitude() const
{
    return latitude_;
}

inline Longitude Location::getLongitude() const
{
    return longitude_;
}

inline IStream& operator>>(IStream& istream, City& city)
{
    istream >> city.name_ >> city.latitude_ >> city.longitude_;
    return istream;
}

inline Point::Point(double x, double y) : x_(x), y_(y) {}

inline double Point::getX() const { return x_; }

inline double Point::getY() const { return y_; }

inline static void to_spherical(const Location& location, double& thetta, double& phi)
{
    Distance radian = 0.0174533;
    thetta = (90 - location.getLatitude()) * radian;
    phi = (location.getLongitude()) * radian;
}

inline double distance(const Location& first, const Location& second)
{
    double thetta1, thetta2, phi1, phi2;
    Distance earth_radius = 6356.752;
    to_spherical(first, thetta1, phi1);
    to_spherical(second, thetta2, phi2);
    Distance twoRadSq = 2 * earth_radius * earth_radius;
    return std::sqrt(twoRadSq - twoRadSq * (std::sin(thetta1) * std::sin(thetta2) * std::cos(phi1 - phi2) + std::cos(thetta1) * std::cos(thetta2)));
}

inline void Way::add(City city)
{
    cities_.push_back(city);
}

inline Arcs Way::getArcs() const
{
    Arcs result;
    for (size_t i = 0; i != cities_.size(); ++i)
    {
        result.emplace_back(cities_[i], cities_[(i + 1) % cities_.size()], i, (i + 1) % cities_.size());
    }
    return result;
}

inline const Cities& Way::getCities() const
{
    return cities_;
}

inline void Way::clear()
{
    cities_.clear();
}

inline Distance Way::getTotalLength() const
{
    Distance result = 0;
    for (size_t i = 0; i < cities_.size(); ++i) {
        result += distance(cities_[i], cities_[(i + 1) % cities_.size()]);
    }
    return result;
}

inline void Way::swapCities(size_t i, size_t j)
{
    j = (j + 1) % cities_.size();

    if (i > j)
        std::swap(i, j);
    std::cout << i << " " << j << "\n";
    std::reverse(cities_.begin() + i, cities_.begin() + j);
}

inline std::ostream& operator<<(std::ostream& out, const Way& way)
{
    for (const auto& city: way.cities_)
    {
        out << city << "\n";
    }
    return out;
}

inline size_t Way::size() const
{
    return cities_.size();
}

inline Region::Region(Name&& name) : name_(std::move(name)) {}

inline void Region::addLocation(Location&& location)
{
    locations_.push_back(std::move(location));
}

inline Arcs Region::getArcs() const
{
    Arcs result;
    for (size_t i = 0; i != locations_.size(); ++i)
    {
        result.emplace_back(locations_[i], locations_[(i + 1) % locations_.size()], -1, -1);
    }
    return result;
}

inline Name Region::getName() const
{
    return name_;
}

inline Arc::Arc(const Location& begin, const Location& end, size_t bcity, size_t ecity) : begin_(begin), end_(end), beginCity_(bcity), endCity_(ecity) {}

inline Arc::Arc(Location&& begin, Location&& end, size_t bcity, size_t ecity) : begin_(std::move(begin)), end_(std::move(end)), beginCity_(bcity), endCity_(ecity) {}

inline const Location& Arc::getBegin() const
{
    return begin_;
}

inline const Location& Arc::getEnd() const
{
    return end_;
}

inline Solution::Solution(const Way& way, const RegionsByName& regByName) : way_(way), regions_(regByName) {}

inline const Way* Solution::getWay() const
{
    return &way_;
}

inline const RegionsByName* Solution::getRegionsByName() const
{
    return &regions_;
}


}
