#ifndef PIPELINE_H
#define PIPELINE_H

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
  std::vector<Operation*> _operations;

public:
  explicit Pipeline(bool is_debug = false);
  Pipeline(std::vector<Operation*> operations, bool is_debug = false);
  std::vector<Circle> detect(const cv::Mat& image) const;

  ~Pipeline() = default;
};

} // namespace detector

#endif // PIPELINE_H
