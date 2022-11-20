#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string>
#include <unordered_map>

namespace cv {

class Mat;

} // namespace cv

namespace detector {

class Operation {
public:
  virtual void process(const cv::Mat& in, cv::Mat& out) const = 0;
  virtual ~Operation() {}
};

class GreyScaleOperation: public Operation {
public:
  void process(const cv::Mat& in, cv::Mat& out) const override;
  ~GreyScaleOperation() override = default;
};

class SmoothOperation: public Operation {
public:
  void process(const cv::Mat& in, cv::Mat& out) const override;
  ~SmoothOperation() override = default;
};

class DetectEdgesOperation: public Operation {
public:
  void process(const cv::Mat& in, cv::Mat& out) const override;
  ~DetectEdgesOperation() override = default;
};

class ThresholdOperation: public Operation {
public:
  void process(const cv::Mat& in, cv::Mat& out) const override;
  ~ThresholdOperation() override = default;
};

class MorphologyOperation: public Operation {
public:
  void process(const cv::Mat& in, cv::Mat& out) const override;
  ~MorphologyOperation() override = default;
};

} // namespace detector

#endif // OPERATIONS_H
