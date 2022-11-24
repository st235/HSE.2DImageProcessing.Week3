#ifndef REPORTER_H
#define REPORTER_H

#include <string>
#include <vector>

#include "circle.h"

namespace report {

class Reporter {
public:
  explicit Reporter();

  Reporter(const Reporter& that) = delete;
  Reporter& operator=(const Reporter& that) = delete;

  void reportDetectedCircles(const std::string& file_name,
                             const std::vector<detector::Circle>& circles);

  ~Reporter() = default;
};

}

#endif // REPORTER_H
