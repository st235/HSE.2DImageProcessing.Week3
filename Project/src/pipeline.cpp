#include "pipeline.h"

#include<opencv2/opencv.hpp>

namespace {

double mean(const std::vector<cv::Vec3f>& arr) {
    double overall = 0;

    for (const cv::Vec3f& val: arr) {
        overall += (double) val[2];
    }

    return overall / arr.size();
}

double standart_deviation(const std::vector<cv::Vec3f>& arr) {
    double mv = mean(arr);

    double overall = 0;

    for (const cv::Vec3f& val: arr) {
        overall += std::pow((double) val[2] - mv, 2);
    }

    return std::sqrt(overall / arr.size());
}

} // namespace

namespace detector {

Pipeline::Pipeline(bool is_debug):
    Pipeline({ 
        new GreyScaleOperation(),
        new SmoothOperation(),
        new DetectEdgesOperation(),
        new ThresholdOperation(),
        new MorphologyOperation()
    }, is_debug) {
    // empty on purpose
}

Pipeline::Pipeline(std::vector<Operation*> operations, bool is_debug): _operations(operations), _is_debug(is_debug) {
    // empty on purpose
}

std::vector<Circle> Pipeline::detect(const std::string& name,
                                     const cv::Mat& image) const {
    // preprocessing steps
    cv::Mat copy(image);

    for (const auto* operation: _operations) {
        operation->process(copy, copy);

        if (_is_debug) {
            cv::imshow(name, copy);
            cv::waitKey(0);
        }
    }

    // circle detection algorithm
    std::vector<cv::Vec3f> raw_circles;
    cv::HoughCircles(copy, raw_circles, cv::HoughModes::HOUGH_GRADIENT, 1 /* dp */, 50 /* min distance */, 50, 60, 15, 0);

    // postprocessing: filter circles by 3 standart deviations rule
    double radius_mean = mean(raw_circles);
    double radius_sd = standart_deviation(raw_circles);

    double radius_min = radius_mean - 3 * radius_sd;
    double radius_max = radius_mean + 3 * radius_sd;

    std::vector<Circle> circles;
    for (const auto& circle: raw_circles) {
        if (circle[2] < radius_min || circle[2] > radius_max) {
            continue;
        }

        circles.push_back(Circle::fromVec3f(circle));
    }

    return circles;
}

} // namespace detecto 


