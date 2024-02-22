#include "svg_dumper.h"
#include "commons.h"
// #include <cmath>
#include <algorithm>
#include <iostream>
#include <cmath>

namespace core
{

static std::string header_top = "<svg version=\"1.1\" width=\"1200\" height=\"1000\" xmlns=\"http://www.w3.org/2000/svg\">\n";
static std::string header_bot = "</svg>";
static std::string line_top = "<line vector-effect=\"non-scaling-size\" ";
static std::string line_bot_red = " style=\"stroke:red;stroke-width:1\" />\n";
static std::string line_bot_blue = " style=\"stroke:blue;stroke-width:1\" />\n";
static std::string circle_top = "<circle ";
static std::string circle_bot = " r=\"2\" fill=\"green\" vector-effect=\"non-scaling-size\" />\n";
static std::string lable_top = "<g>\n<title> ";
static std::string lable_mid = " </title>\n";
static std::string lable_bot = " </g>\n";

static constexpr Latitude   lat0 = M_PI / 2;
static constexpr Longitude  long0 = 0;

Point::Point(double x, double y) : x_(x), y_(y) {}
double Point::getX() const { return x_; }
double Point::getY() const { return y_; }

Point SVG::getCleanProjection(const Location& location) const
{
    const Latitude    lat1 = location.getLatitude() * (M_PI / 180);
    const Longitude   long1 = location.getLongitude() * (M_PI / 180);

    const double cos_c = std::sin(lat0) * std::sin(lat1) + std::cos(lat0) * std::cos(long1 - long0);
    const double x = (std::cos(lat0) * std::sin(lat1) - std::sin(lat0) * std::cos(lat1) * std::cos(long1 - long0)) / cos_c;
    const double y = (std::cos(lat1) * std::sin(long1 - long0)) / cos_c;
    return {x, y};
}

Point SVG::getProjection(const Location& location) const
{
    const Latitude    lat1 = location.getLatitude() * (M_PI / 180);
    const Longitude   long1 = location.getLongitude() * (M_PI / 180);

    const double cos_c = std::sin(lat0) * std::sin(lat1) + std::cos(lat0) * std::cos(long1 - long0);
    // std::cout << "cos_c = " << cos_c << "\n";
    const double x = (std::cos(lat0) * std::sin(lat1) - std::sin(lat0) * std::cos(lat1) * std::cos(long1 - long0)) / cos_c;
    // std::cout << "x = " << x << "\n";
    const double y = (std::cos(lat1) * std::sin(long1 - long0)) / cos_c;
    // std::cout << "y = " << y << "\n";
    return {    600 * (x - bbox_.first.getX()) + offset_.getX()
            ,   600 * (y - bbox_.first.getY()) + offset_.getY()};
}

const Locations SVG::getLoactions(const Arc& arc, size_t desity) const
{
    Locations result;
    result.push_back(arc.getBegin());
    result.push_back(arc.getEnd());
    return result;
}

inline void SVG::initBBox(const Solution& solution)
{
    bool found = false;
    BBox tmpBbox = bbox_;
    for (const auto& [_, region]: *solution.getRegionsByName())
    {
        for (const auto& arc: region.getArcs())
        {
            Point p1 = getCleanProjection(arc.getBegin());
            Point p2 = getCleanProjection(arc.getEnd());
            if (!found)
            {
                tmpBbox = {p1, p2};
                found = true;
            }
            Point minPoint = {std::min(p1.getX(), p2.getX()), std::min(p1.getY(), p2.getY())};
            Point maxPoint = {std::max(p1.getX(), p2.getX()), std::max(p1.getY(), p2.getY())};
            
            tmpBbox.first = {std::min(minPoint.getX(), tmpBbox.first.getX()), std::min(minPoint.getY(), tmpBbox.first.getY())};
            tmpBbox.second = {std::max(maxPoint.getX(), tmpBbox.second.getX()), std::max(maxPoint.getY(), tmpBbox.second.getY())};
            
        }
    }
    bbox_ = tmpBbox;
}

void SVG::renderLocation(OStream& ostream, const Location& city) const
{
    Point p = getProjection(city);
    ostream << circle_top;
    ostream << "cx=\"" << p.getX() << "\" cy=\"" << p.getY() << "\"";
    ostream << circle_bot;
}

void SVG::renderCity(OStream& ostream, const City& city) const
{
    ostream << lable_top << city.getName() << lable_mid;
    renderLocation(ostream, city);
    ostream << lable_bot;
}

void SVG::renderArc(OStream& ostream, const Arc& arc, bool isRed) const
{
    Point p1 = getProjection(arc.getBegin());
    Point p2 = getProjection(arc.getEnd());
    ostream << line_top;
    ostream << "x1=\""<< p1.getX() << "\" y1=\"" << p1.getY();
    ostream  << "\" x2=\"" << p2.getX() << "\" y2=\"" << p2.getY() << "\"";
    if (isRed)
        ostream << line_bot_red;
    else
        ostream << line_bot_blue;
}

void SVG::renderRegion(OStream& ostream, const Region& region) const
{
    Arcs arcs = region.getArcs();
    for (const auto& arc: arcs)
    {
        renderArc(ostream, arc, true);
    }
}

void SVG::renderWay(OStream& ostream, const Way& way) const
{
    auto cities = way.getCities();
    for (size_t i = 0; i < cities.size(); ++i)
    {
        renderArc(ostream, {cities[i], cities[(i + 1) % cities.size()]}, false);
    }
    for (size_t i = 0; i < cities.size(); i++)
    {
        renderCity(ostream, cities[i]);
    }
}

void SVG::render(OStream& ostream, const Solution& solution)
{
    initBBox(solution);
    ostream << header_top;
    for (const auto& [_, region] : *solution.getRegionsByName())
    {
        renderRegion(ostream, region);
    }
    renderWay(ostream, *solution.getWay());
    std::cout << solution.getWay()->getCities().size() << "\n";
    ostream << header_bot;
}


}
