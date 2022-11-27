#ifndef REPORTER_H
#define REPORTER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "circle.h"

namespace detector {

class CirclesTracker {
private:
  std::unordered_map<std::string, std::vector<detector::Circle>> _images_lookup;

public:
  explicit CirclesTracker();

  CirclesTracker(const CirclesTracker& that) = delete;
  CirclesTracker& operator=(const CirclesTracker& that) = delete;

  inline std::unordered_map<std::string, std::vector<detector::Circle>> imagesLut() const {
    return _images_lookup;
  }

  void addDetectedCircles(const std::string& file_name,
                          const std::vector<detector::Circle>& circles);

  ~CirclesTracker() = default;
};

}

#endif // REPORTER_H
