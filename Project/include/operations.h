#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string>
#include<opencv2/opencv.hpp>
#include <unordered_map>

namespace detector {

class Operation {
public:
  virtual void process(const cv::Mat& in, cv::Mat& out) const = 0;
  virtual ~Operation() {}
};

class GreyScaleOperation: public Operation {
public:
  void process(const cv::Mat& in, cv::Mat& out) const override {
    cv::cvtColor(in, out, cv::COLOR_BGR2GRAY);
    out.convertTo(out, CV_8U);
  }

  ~GreyScaleOperation() override = default;
};

class SmoothOperation: public Operation {
private:
  uint32_t _kernel;
  uint32_t _sigma;

public:
  SmoothOperation(uint32_t kernel, uint32_t sigma):
    _kernel(kernel),
    _sigma(sigma) {
    // empty on purpose
  }

  SmoothOperation(const SmoothOperation& that) = delete;
  SmoothOperation& operator=(const SmoothOperation& that) = delete;

  void process(const cv::Mat& in, cv::Mat& out) const override {
    cv::GaussianBlur(in, out, cv::Size(_kernel, _kernel), _sigma);
  }

  ~SmoothOperation() override = default;
};

class DetectEdgesOperation: public Operation {
private:
  uint32_t _threshold1;
  uint32_t _threshold2;
  uint32_t _apperture_size;
  bool _l2gradient;

public:
  DetectEdgesOperation(uint32_t threshold1,
                       uint32_t threshold2,
                       uint32_t apperture_size,
                       bool l2gradient): 
    _threshold1(threshold1),
    _threshold2(threshold2),
    _apperture_size(apperture_size),
    _l2gradient(l2gradient) {
    // empty on purpose
  }

  DetectEdgesOperation(const DetectEdgesOperation& that) = delete;
  DetectEdgesOperation& operator=(const DetectEdgesOperation& that) = delete;

  void process(const cv::Mat& in, cv::Mat& out) const override {
    cv::Canny(in, out, _threshold1, _threshold2, _apperture_size, _l2gradient);
  }

  ~DetectEdgesOperation() override = default;
};

class ThresholdOperation: public Operation {
private:
  uint32_t _threshold;
  uint32_t _max_value;

public:
  ThresholdOperation(uint32_t threshold,
                     uint32_t max_value):
    _threshold(threshold),
    _max_value(max_value) {
    // empty on purpose
  }

  ThresholdOperation(const ThresholdOperation& that) = delete;
  ThresholdOperation& operator=(const ThresholdOperation& that) = delete;

  void process(const cv::Mat& in, cv::Mat& out) const override {
    cv::threshold(in, out, _threshold, _max_value, cv::THRESH_BINARY | cv::THRESH_OTSU);
  }

  ~ThresholdOperation() override = default;
};

class DilateOperation: public Operation {
private:
  uint32_t _structuring_element_size;

public:
  DilateOperation(uint32_t structuring_element_size):
    _structuring_element_size(structuring_element_size) {
    // empty on purpose
  }

  DilateOperation(const DilateOperation& that) = delete;
  DilateOperation& operator=(const DilateOperation& that) = delete;

  void process(const cv::Mat& in, cv::Mat& out) const override {
    cv::morphologyEx(in, out, cv::MorphTypes::MORPH_DILATE, cv::getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, cv::Size(_structuring_element_size, _structuring_element_size)));
  }

  ~DilateOperation() override = default;
};

class ErodeOperation: public Operation {
private:
  uint32_t _structuring_element_size;

public:
  ErodeOperation(uint32_t structuring_element_size):
    _structuring_element_size(structuring_element_size) {
    // empty on purpose
  }

  ErodeOperation(const ErodeOperation& that) = delete;
  ErodeOperation& operator=(const ErodeOperation& that) = delete;

  void process(const cv::Mat& in, cv::Mat& out) const override {
    cv::morphologyEx(in, out, cv::MorphTypes::MORPH_ERODE, cv::getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, cv::Size(_structuring_element_size, _structuring_element_size)));
  }

  ~ErodeOperation() override = default;
};

class OpenOperation: public Operation {
private:
  uint32_t _structuring_element_size;

public:
  OpenOperation(uint32_t structuring_element_size):
    _structuring_element_size(structuring_element_size) {
    // empty on purpose
  }

  OpenOperation(const OpenOperation& that) = delete;
  OpenOperation& operator=(const OpenOperation& that) = delete;

  void process(const cv::Mat& in, cv::Mat& out) const override {
    cv::morphologyEx(in, out, cv::MorphTypes::MORPH_OPEN, cv::getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, cv::Size(_structuring_element_size, _structuring_element_size)));
  }

  ~OpenOperation() override = default;
};

class CloseOperation: public Operation {
private:
  uint32_t _structuring_element_size;

public:
  CloseOperation(uint32_t structuring_element_size):
    _structuring_element_size(structuring_element_size) {
    // empty on purpose
  }

  CloseOperation(const CloseOperation& that) = delete;
  CloseOperation& operator=(const CloseOperation& that) = delete;

  void process(const cv::Mat& in, cv::Mat& out) const override {
    cv::morphologyEx(in, out, cv::MorphTypes::MORPH_CLOSE, cv::getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, cv::Size(_structuring_element_size, _structuring_element_size)));
  }

  ~CloseOperation() override = default;
};

} // namespace detector

#endif // OPERATIONS_H
