#include "metrics_config.h"

#include <fstream>
#include <unordered_set>

namespace config {

MetricsConfig::Metric::Metric(uint32_t true_positive, 
                              uint32_t true_negative,
                              uint32_t false_positive,
                              uint32_t false_negative):
                              _true_positive(true_positive),
                              _true_negative(true_negative),
                              _false_positive(false_positive),
                              _false_negative(false_negative) {
    // empty on purpose
}

MetricsConfig::Metric::Metric(const Metric& that):
    _true_positive(that._true_positive),
    _true_negative(that._true_negative),
    _false_positive(that._false_positive),
    _false_negative(that._false_negative) {
    // empty on purpose
}

MetricsConfig::Metric& MetricsConfig::Metric::operator=(const Metric& that) {
  if (this != &that) {
    this->_true_positive = that._true_positive;
    this->_true_negative = that._true_negative;
    this->_false_positive = that._false_positive;
    this->_false_negative = that._false_negative;
  }

  return *this;
}

double MetricsConfig::Metric::precision() const {
    return static_cast<double>(_true_positive) / (_true_positive + _false_positive);
}

double MetricsConfig::Metric::recall() const {
    return static_cast<double>(_true_positive) / (_true_positive + _false_negative);
}

double MetricsConfig::Metric::f1Score() const {
    double precision = this->precision();
    double recall = this->recall();
    return 2.0 * precision * recall / (precision + recall);
}

MetricsConfig::Metric MetricsConfig::Metric::operator+(const Metric& that) const {
    return Metric(_true_positive + that._true_positive /* _true_positive */,
                  _true_negative + that._true_negative /* _true_negative */,
                  _false_positive + that._false_positive /* _false_positive */,
                  _false_negative + that._false_negative /* _false_negative */);
}

MetricsConfig MetricsConfig::fromFile(const std::string& file) {
    MetricsConfig config;

    std::ifstream stream;
    stream.open(file, std::ios::in);

    std::string file_name;
    std::vector<detector::Circle> circles;

    std::string line;
    while (std::getline(stream, line)) {
        if (line.empty()) {
            config.addImage(file_name, circles);

            file_name = "";
            circles.clear();
            continue;
        }

        if (file_name.empty()) {
            file_name = line;
        } else {
            circles.push_back(detector::Circle::fromSerialisation(line));
        }
    }

    config.addImage(file_name, circles);
    return config;
}

MetricsConfig::MetricsConfig(): _images_lookup() {
    // empty on purpose
}

MetricsConfig::MetricsConfig(const MetricsConfig& that) {
    this->_images_lookup = that._images_lookup;
}

MetricsConfig& MetricsConfig::operator=(const MetricsConfig& that) {
    if (this != &that) {
        this->_images_lookup = that._images_lookup;
    }

    return *this;
}

void MetricsConfig::addImage(const std::string& image, 
                             const std::vector<detector::Circle>& circles) {
    _images_lookup[image] = circles;
}

MetricsConfig::Metric MetricsConfig::getConfusionMetric(
    const std::string& image,
    const std::vector<detector::Circle>& circles) const {
    if (_images_lookup.find(image) == _images_lookup.end()) {
        throw std::runtime_error(image + " was not found on the config.");
    }

    std::unordered_set<detector::Circle, detector::Circle::Hash> expected_circles;

    for (const auto& circle: _images_lookup.at(image)) {
        expected_circles.insert(circle);
    }

    uint32_t true_positive = 0;
    uint32_t false_positive = 0;
    uint32_t false_negative = 0;

    for (const auto& circle: circles) {
        double max_iou = 0;
        detector::Circle max_iou_circle(0, 0, 0);

        for (const auto& expected_circle: expected_circles) {
            double iou = circle.iou(expected_circle);

            if (iou > max_iou) {
                max_iou = iou;
                max_iou_circle = expected_circle;
            }
        }

        if (max_iou >= 0.5) {
            true_positive += 1;
            expected_circles.erase(max_iou_circle);
        } else {
            false_positive += 1;
        }
    }

    false_negative += expected_circles.size();

    return Metric(true_positive,
                  0 /* true_negative */,
                  false_positive,
                  false_negative);
}

MetricsConfig::Metric MetricsConfig::getConfusionMetric(const ImagesLUT& lut) const {
    MetricsConfig::Metric final_metric(0, 0, 0, 0);

    for (const auto& item: lut) {
        const auto& image = item.first;
        const auto& circles = item.second;

        const auto& metric = getConfusionMetric(image, circles);
        final_metric = final_metric + metric;
    }

    return final_metric;
}

} // namespace config
