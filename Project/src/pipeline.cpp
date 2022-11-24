#include "pipeline.h"

#include<opencv2/opencv.hpp>

namespace {

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        // Mainly for demonstration purposes, i.e. works but is overly simple
        // In the real world, use sth. like boost.hash_combine
        return h1 ^ h2;  
    }
};

double mean(const std::vector<detector::Circle>& arr) {
    double overall = 0;

    for (const auto& val: arr) {
        overall += (double) val.radius();
    }

    return overall / arr.size();
}

double median(const std::vector<detector::Circle>& arr) {
    std::vector<double> radiuses;

    for (const auto& val: arr) {
        radiuses.push_back(val.radius());
    }

    std::sort(radiuses.begin(), radiuses.end());

    if (radiuses.size() == 0) {
        return 0;
    }

    if (radiuses.size() % 2 == 0) {
        return (radiuses[radiuses.size() / 2] + radiuses[radiuses.size() / 2 - 1]) / 2;
    } else {
        return radiuses[radiuses.size() / 2];
    }
}

double standart_deviation(const std::vector<detector::Circle>& arr) {
    double mv = mean(arr);

    double overall = 0;

    for (const auto& val: arr) {
        overall += std::pow((double) val.radius() - mv, 2);
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
        new CloseOperation(),
        new OpenOperation(),
        new DetectEdgesOperation(),
        new DilateOperation()
        // // new MorphologyOperation(),
        // // new ThresholdOperation()
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
            cv::destroyAllWindows();
        }
    }

    int max_radius = std::min(image.rows, image.cols) / 2;

    // circle detection algorithm
    std::vector<cv::Vec3f> raw_circles;
    cv::HoughCircles(copy, raw_circles, cv::HoughModes::HOUGH_GRADIENT_ALT, 2 /* dp */, 30 /* min distance */, 300 /* param 1 */, 0.6 /* param 2 */, 25 /* min_radius */, max_radius);

    std::unordered_map<std::pair<uint32_t, uint32_t>, std::vector<uint32_t>, pair_hash> circle_groups;

    for (const auto& item: raw_circles) {
        const auto& pair = std::make_pair<uint32_t, uint32_t>(item[0], item[1]);
        circle_groups[pair].push_back(item[2]);
    }

    std::vector<Circle> filtered_circles;

    for (const auto& item: circle_groups) {
        const auto& center = item.first;
        const auto& group = item.second;

        uint32_t overall_radius = 0;
        for (const auto& radius: group) {
            overall_radius += radius;
        }

        filtered_circles.push_back(Circle(center.first, center.second, overall_radius / group.size()));
    }

    // postprocessing: filter circles by 3 standart deviations rule
    double radius_mean = median(filtered_circles);
    double radius_sd = standart_deviation(filtered_circles);

    double radius_min = radius_mean - 3 * radius_sd;
    double radius_max = radius_mean + 3 * radius_sd;



    std::vector<Circle> circles;
    for (const auto& circle: filtered_circles) {
        if (circle.radius() < radius_min || circle.radius() > radius_max) {
            continue;
        }

        circles.push_back(circle);
    }

    return circles;
}

} // namespace detecto 


