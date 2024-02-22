#pragma once
#include "commons.h"
#include <istream>
#include <cassert>

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

class Way
{
public:
    Way() = default;
    void add(City);
    Arcs getArcs() const;
    const Cities& getCities() const;
    void clear();
    Distance getTotalLength() const;
    void setTotalLength(Distance);
    friend std::ostream& operator<<(std::ostream&, const Way&);
    size_t size() const;
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
    Arc(const Location&, const Location&);
    Arc(Location&&, Location&&);
    const Location& getBegin() const;
    const Location& getEnd() const;

protected:
    Location begin_;
    Location end_;
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

inline void Way::add(City city)
{
    cities_.push_back(city);
}

inline Arcs Way::getArcs() const
{
    Arcs result;
    for (size_t i = 0; i != cities_.size(); ++i)
    {
        result.emplace_back(cities_[i], cities_[(i + 1) % cities_.size()]);
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
    assert(total_ > 0 && "total length not set");
    return total_;
}

inline void Way::setTotalLength(Distance length)
{
    total_ = length;
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
        result.emplace_back(locations_[i], locations_[(i + 1) % locations_.size()]);
    }
    return result;
}

inline Name Region::getName() const
{
    return name_;
}

inline Arc::Arc(const Location& begin, const Location& end) : begin_(begin), end_(end) {}

inline Arc::Arc(Location&& begin, Location&& end) : begin_(std::move(begin)), end_(std::move(end)) {}

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
