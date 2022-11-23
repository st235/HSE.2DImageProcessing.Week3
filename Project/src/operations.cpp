#include "operations.h"

#include<opencv2/opencv.hpp>

namespace detector {

void GreyScaleOperation::process(const cv::Mat& in, cv::Mat& out) const {
    cv::cvtColor(in, out, cv::COLOR_BGR2GRAY);
    out.convertTo(out, CV_8U);
}

void SmoothOperation::process(const cv::Mat& in, cv::Mat& out) const {
    cv::GaussianBlur(in, out, cv::Size(5, 5) /* kernel */, 0 /* sigma */);
}

void DetectEdgesOperation::process(const cv::Mat& in, cv::Mat& out) const {
    cv::Canny(in, out, 200 /* threshold 1 */, 225 /* threshold 2 */, 5 /* kernel */, true /* L2gradient */);
}

void ThresholdOperation::process(const cv::Mat& in, cv::Mat& out) const {
    cv::threshold(in, out, 225 /* threshold */, 255 /* max_value */, cv::THRESH_BINARY | cv::THRESH_OTSU);
}

void DilateOperation::process(const cv::Mat& in, cv::Mat& out) const {
    cv::morphologyEx(in, out, cv::MorphTypes::MORPH_DILATE, cv::getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, cv::Size(15, 15)));
}

void ErodeOperation::process(const cv::Mat& in, cv::Mat& out) const {
    cv::morphologyEx(in, out, cv::MorphTypes::MORPH_ERODE, cv::getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, cv::Size(10, 10)));
}

void OpenOperation::process(const cv::Mat& in, cv::Mat& out) const {
    cv::morphologyEx(in, out, cv::MorphTypes::MORPH_OPEN, cv::getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, cv::Size(50, 50)));
}

void CloseOperation::process(const cv::Mat& in, cv::Mat& out) const {
    cv::morphologyEx(in, out, cv::MorphTypes::MORPH_CLOSE, cv::getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, cv::Size(20, 20)));
}

} // namespace detector
