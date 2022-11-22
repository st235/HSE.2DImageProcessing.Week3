#include "report.h"

namespace report {


Report::Report(const std::string title): _title(title), _file() {
    _file.open(title, std::ios::out | std::ios::trunc);
}

void Report::appendCircles(const std::string& image_name,
                           const std::vector<detector::Circle>& circles) {
    _file << std::setw(50) << std::left << image_name << std::endl;
    _file << "Detected: " << std::setw(10) << std::left << circles.size() << " circles." << std::endl;

    int index = 1;
    for (const auto& circle: circles) {
        _file << "#" << std::setfill('0') << std::setw(3) << std::right << index << ". ";
        _file << std::setfill(' ');
        _file << "Center at {" << circle.x() << ", " << circle.y() << "}. ";
        _file << "Radius is " << circle.radius() << "." << std::endl;

        index++;
    }

    _file << std::endl;
}

Report::~Report() {
    _file.close();
}

} // namespace report
