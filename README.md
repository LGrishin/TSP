# TSP SOLVER
## Гришин Лаврентий Викторович

To run and build, you will need to install `BOOST v1.71` or higher and `glpk` package.

### Intro
The data is stored in a modified format (without commas and parentheses) in the `data` directory. This directory is copied during the build to `BUILD_DIR`.

After the build, the executable file `run` is obtained.

The data is read using a parser (see `parser.h`).

In the files `core.h` and `core.cpp` the solver interface has been declared and implemented.

In the file `main.cpp ` the source code of the executable file assembly.

### Task 1.
Distance calculation: The distance calculation function `distance` between two points (cities) on the sphere is located at the beginning of the file `data_model.h:144` and `data_model.h:151`.

Rendering: all the rendering functionality is in the file `svg_dumper.cpp`. The image is created in `SVG` format.
Gnomic projection: calculated by the `getProjection` function (file `svg_dumper.cpp:36 `).

*When you hover the cursor over a city, its name is highlighted.*

After building the route, a list of cities of the route is displayed in the order of detour and its total length (currently shortest 45339.5 km).

### Task 3.
It is located in the files `approximation.h` and `Approximation.cpp ` - construction of an approximated route using a **spanning tree** and **dfs**.

### Task 4.
Heuristics for optimizing the route obtained after approximation: elimination of intersections in the route. Function `ApproximationSolver::optimize()` in `approximation.cpp:106`.

### Additionally:
The `images` folder contains a file with the route built using approximation. I also added an image with a minimal spanning tree there, however, now it is no longer drawn when running the executable file by default.

The shortest way at the moment is 45339.5 km.