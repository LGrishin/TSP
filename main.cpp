#include <filesystem>

#include "commons.h"
#include "approximation.h"
#include "milp_solver.h"

static const std::filesystem::path cities = "data/cities.txt";
static const std::filesystem::path region1 = "data/region_1_bolshevik.txt";
static const std::filesystem::path region2 = "data/region_2_october_revolution.txt";
static const std::filesystem::path region3 = "data/region_3_mainland.txt";
static const std::filesystem::path region4 = "data/region_4_komsomolets.txt";
static const std::filesystem::path region5 = "data/region_5_kaliningrad.txt";
static const std::filesystem::path region6 = "data/region_6_severny_novaya_zemlya.txt";
static const std::filesystem::path region7 = "data/region_7_sakhaline.txt";
static const std::filesystem::path region8 = "data/region_8_wrangel.txt";
static const std::filesystem::path region9 = "data/region_9_yuzhny_novaya_zemlya.txt";
static const std::filesystem::path test_svg = "data/svg_test.txt";
static const core::Filenames regions =
{
    region1,
    region2,
    region3,
    region4,
    region5,
    region6,
    region7,
    region8,
    region9,
};


int main()
{
    core::ApproximationSolver solver1;
    solver1.read(cities, regions, 94);
    solver1.solve();
    solver1.dump("result.svg");
    
    auto solution = solver1.getSolution();
    std::cout << *solution.getWay() << "\n";
    std::cout << "Way length = " << solution.getWay()->getTotalLength() << "\n";
}
