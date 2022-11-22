#ifndef REPORT_H
#define REPORT_H

#include <string>
#include <vector>
#include <fstream>

#include "circle.h"

namespace report {

class Report {
private:
  std::string _title;
  std::fstream _file;

public:
  explicit Report(const std::string title);

  Report(const Report& that) = delete;
  Report& operator=(const Report& that) = delete;

  void appendCircles(const std::string& image_name,
                     const std::vector<detector::Circle>& circles);

  ~Report();
};

}

#endif // REPORT_H
