# genpoly_fpg

A Python wrapper for the [genpoly-fpg](https://github.com/cgalab/genpoly-fpg) library for generating simple polygons.

## Prerequisites

- Python 3.7 or higher
- CMake 3.10 or higher
- A C++ compiler with C++14 support
- pip package manager

## Installation

1. Clone this repository with submodules:
```bash
git clone --recursive https://github.com/lukszi/python-fpg.git
cd python-fpg
```

2. Create and activate a virtual environment (optional but recommended):
```bash
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate
```

3. Install the package:
```bash
pip install .
```

## Usage

The library provides functionality to generate simple polygons:

```python
import genpoly_fpg

# Generate multiple polygons
# Parameters:
#   num_polygons: Number of polygons to generate
#   num_vertices: Final number of vertices in each polygon
#   initial_vertices: Number of vertices in the initial regular polygon
#   radius: Radius of the initial regular polygon
polygons = genpoly_fpg.generate_multiple_polygons(
    num_polygons=5,
    num_vertices=100,
    initial_vertices=20,
    radius=1.0
)

# The returned array has shape (num_polygons, num_vertices+1, 2)
# Each polygon is represented by its vertex coordinates
# The first vertex is repeated at the end to close the polygon
print(f"Generated {len(polygons)} polygons")
print(f"Each polygon has {len(polygons[0])} vertices")
```

## Development

To contribute or modify the code:

1. Clone the repository with submodules:
```bash
git clone --recursive https://github.com/yourusername/python-fpg.git
cd python-fpg
```

2. Create a development environment:
```bash
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate
pip install -e .
```

## Acknowledgments

This project is based on [genpoly-fpg](https://github.com/cgalab/genpoly-fpg) by Philipp Mayer.