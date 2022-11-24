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

  std::vector<Operation*> _operations;

public:
  explicit Pipeline(bool is_debug);

  Pipeline(std::vector<Operation*> operations, 
           bool is_debug);

  std::vector<Circle> detect(const std::string& name,
                             const cv::Mat& image) const;

  ~Pipeline() = default;
};

} // namespace detector

#endif // PIPELINE_H
