#include "circles_tracker.h"

namespace detector {

CirclesTracker::CirclesTracker(): _images_lookup() {
    // empty on purpose
}

void CirclesTracker::addDetectedCircles(const std::string& file_name,
                                  const std::vector<detector::Circle>& circles) {
    _images_lookup[file_name] = circles;
}

} // namespace report
