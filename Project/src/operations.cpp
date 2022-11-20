#include "operations.h"

#include<opencv2/opencv.hpp>

namespace detector {

void GreyScaleOperation::process(const cv::Mat& in, cv::Mat& out) const {
    cv::cvtColor(in, out, cv::COLOR_BGR2GRAY);
}

void SmoothOperation::process(const cv::Mat& in, cv::Mat& out) const {
    cv::GaussianBlur(in, out, cv::Size(3, 3) /* kernel */, 1 /* sigma */);
}

void DetectEdgesOperation::process(const cv::Mat& in, cv::Mat& out) const {
    cv::Canny(in, out, 150 /** threshold 1 */, 225 /* threshold 2 */, 3 /* kernel */);
}

void ThresholdOperation::process(const cv::Mat& in, cv::Mat& out) const {
    cv::threshold(in, out, 128, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}

void MorphologyOperation::process(const cv::Mat& in, cv::Mat& out) const {
    cv::morphologyEx(in, out, cv::MorphTypes::MORPH_DILATE, cv::getStructuringElement(cv::MorphShapes::MORPH_RECT, cv::Size(3, 3)));

    cv::morphologyEx(out, out, cv::MorphTypes::MORPH_OPEN, cv::getStructuringElement(cv::MorphShapes::MORPH_RECT, cv::Size(3, 3)));
}

} // namespace detector
