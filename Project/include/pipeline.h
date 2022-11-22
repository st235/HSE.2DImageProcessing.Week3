#ifndef PIPELINE_H
#define PIPELINE_H

#include <string>
#include <vector>

#include "circle.h"
#include "operations.h"

namespace cv {

class Mat;

} // namespace cv

namespace detector {

class Pipeline {
private:
  bool _is_debug;
  int _debug_step;

  std::vector<Operation*> _operations;

public:
  static const int NO_SPECIFIC_DEBUG_STEP;

  explicit Pipeline(bool is_debug, 
                    int debug_step);

  Pipeline(std::vector<Operation*> operations, 
           bool is_debug, 
           int debug_step);

  std::vector<Circle> detect(const std::string& name,
                             const cv::Mat& image) const;

  ~Pipeline() = default;
};

} // namespace detector

#endif // PIPELINE_H
