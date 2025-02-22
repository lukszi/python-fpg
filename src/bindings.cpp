// bindings.cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "initialPolygonGenerator.h"
#include "settings.h"
#include "triangulation.h"
#include "polygonTransformer.h"

namespace py = pybind11;

// Helper function to get polygon vertices as numpy array
py::array_t<double> get_polygon_vertices(const TPolygon* polygon) {
    if (!polygon) {
        throw std::runtime_error("Null polygon pointer");
    }
    
    std::vector<double> coords;
    const Vertex* start = polygon->getVertex(0);
    const Vertex* current = start;
    
    // Collect all vertices
    do {
        coords.push_back(current->getX());
        coords.push_back(current->getY());
        current = current->getNext();
    } while (current != start);
    
    // Add first vertex again to close the polygon
    coords.push_back(start->getX());
    coords.push_back(start->getY());
    
    // Create the numpy array
    const size_t num_vertices = coords.size() / 2;
    std::vector<ssize_t> shape = {static_cast<ssize_t>(num_vertices), 2};
    std::vector<ssize_t> strides = {2 * sizeof(double), sizeof(double)};
    
    auto result = py::array_t<double>(shape, strides);
    auto buf = result.request();
    double* ptr = static_cast<double*>(buf.ptr);
    
    std::copy(coords.begin(), coords.end(), ptr);
    
    return result;
}

// Helper function to generate multiple polygons and return as numpy array
py::array_t<double> generate_multiple_polygons(int num_polygons, int num_vertices, int initial_vertices, double radius) {

    // Update settings for polygon generation
    Settings::initialSize = initial_vertices;
    Settings::outerSize = num_vertices;
    Settings::radiusPolygon = radius;
    Settings::feedback = FeedbackMode::MUTE;
    Settings::checkAndApplySettings();

    // Container for all polygon vertices
    std::vector<std::vector<std::vector<double>>> all_polygons;
    
    // Generate specified number of polygons
    for (int i = 0; i < num_polygons; i++) {
        // Cleanup previous state
        Settings::cleanup();
        TPolygon::resetCounter();

        // Reset settings for new polygon
        Settings::initialSize = initial_vertices;
        Settings::outerSize = num_vertices;
        Settings::radiusPolygon = radius;
        Settings::feedback = FeedbackMode::MUTE;
        Settings::checkAndApplySettings();

        // Generate a new polygon
        std::unique_ptr<Triangulation> T(generateRegularPolygon());
        strategyNoHoles0(T.get());
        
        // Get vertices from the polygon
        std::vector<std::vector<double>> polygon_vertices;
        const Vertex* start = T->getVertex(0, 0);  // Get first vertex of outer polygon
        const Vertex* current = start;
        
        do {
            polygon_vertices.push_back({current->getX(), current->getY()});
            current = current->getNext();
        } while (current != start);
        
        // Add first vertex again to close the polygon
        polygon_vertices.push_back({start->getX(), start->getY()});
        
        all_polygons.push_back(polygon_vertices);
    }

    // Create numpy array from all polygons
    const size_t num_points = all_polygons[0].size();  // All polygons have same number of points
    std::vector<ssize_t> shape = {
        static_cast<ssize_t>(num_polygons),
        static_cast<ssize_t>(num_points),
        static_cast<ssize_t>(2)
    };
    
    auto result = py::array_t<double>(shape);
    auto buf = result.request();
    double* ptr = static_cast<double*>(buf.ptr);
    
    // Copy data to numpy array
    for (size_t i = 0; i < num_polygons; i++) {
        for (size_t j = 0; j < num_points; j++) {
            for (size_t k = 0; k < 2; k++) {
                ptr[i * num_points * 2 + j * 2 + k] = all_polygons[i][j][k];
            }
        }
    }
    
    return result;
}

PYBIND11_MODULE(genpoly_fpg, m) {
    m.doc() = "Python bindings for polygon generation library"; 

    m.def("generate_multiple_polygons", &generate_multiple_polygons,
        "Generate multiple regular polygons and return as numpy array",
        py::arg("num_polygons"),
        py::arg("num_vertices"),
        py::arg("initial_vertices"),
        py::arg("radius") = 1.0);
}