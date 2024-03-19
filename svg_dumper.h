#pragma once
#include "commons.h"
#include "data_model.h"
#include <ostream>

namespace core
{

class SVG
{
    using BBox = std::pair<Point, Point>;   //minx miny, maxx maxy

protected:
    
    Point getCleanProjection(const Location&) const;
    Point getProjection(const Location&) const;
    const Locations getLoactions(const Arc&, size_t) const;
    void initBBox(const Solution&);
    void renderLocation(OStream&, const Location&) const;
    void renderCity(OStream&, const City&) const;
    void renderArc(OStream&, const Arc&, bool) const;
    void renderRegion(OStream&, const Region&) const;
    void renderWay(OStream&, const Way&) const;
    
public:
    SVG() = default;
    void render(OStream&, const Solution&);
protected:
    BBox bbox_ = {{0, 0}, {0, 0}};
    Point offset_ = {100, 100};
};

}
