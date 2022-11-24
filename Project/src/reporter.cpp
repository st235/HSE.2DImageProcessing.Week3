#include "reporter.h"

#include <iostream>

namespace report {

Reporter::Reporter() {
    // empty on purpose
}

void Reporter::reportDetectedCircles(const std::string& file_name,
                                     const std::vector<detector::Circle>& circles) {
    std::cout << std::setw(50) << std::left << file_name << std::endl;
    std::cout << "Detected: " << std::setw(10) << std::left << circles.size() << " circles." << std::endl;

    int index = 1;
    for (const auto& circle: circles) {
        std::cout << "#" << std::setfill('0') << std::setw(3) << std::right << index << ". ";
        std::cout << std::setfill(' ');
        std::cout << "Center at {" << circle.x() << ", " << circle.y() << "}. ";
        std::cout << "Radius is " << circle.radius() << "." << std::endl;

        index++;
    }

    std::cout << std::endl;
}

} // namespace report
