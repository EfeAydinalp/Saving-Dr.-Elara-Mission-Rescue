#include <valarray>
#include "Sector.h"

#include <sstream>



// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        // TODO: Calculate the distance to the Earth, and generate the sector code
    // Calculate the distance from the Earth using the Euclidean distance formula
    distance_from_earth = std::sqrt(x * x + y * y + z * z);

    // Generate the sector code based on coordinates and distance from the Earth
    sector_code = generateSectorCode();
}

std::string Sector::generateSectorCode() const {
    // Calculate distance component (floor of the distance)
    int distance_component = static_cast<int>(std::floor(distance_from_earth));

    // Initialize sector code with distance component
    std::string code = std::to_string(distance_component);

    // Append coordinate components based on X, Y, Z values
    code += getCodeForCoordinate(x,"x");
    code += getCodeForCoordinate(y,"y");
    code += getCodeForCoordinate(z,"z");

    return code;
}

// Function to generate code for a single coordinate
std::string Sector::getCodeForCoordinate(int value, std::string variable) const {
    if (value == 0) {
        return "S"; // Same as Earth's coordinate
    } else if (value > 0) {
        if (variable == "x") {
            return "R"; // Right (X)
        } else if (variable == "y") {
            return "U"; // Up (Y)
        } else if (variable == "z") {
            return "F"; // Forward (Z)
        }
        // Handle other positive values as needed
    } else {
        if (variable == "x") {
            return "L"; // Left (X)
        } else if (variable == "y") {
            return "D"; // Down (Y)
        } else if (variable == "z") {
            return "B"; // Backward (Z)
        }
        // Handle other negative values as needed
    }
    return ""; // Default case
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    // TODO: Overload the assignment operator
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}