#ifndef METRICS_CONFIG_H
#define METRICS_CONFIG_H

#include "circle.h"

#include <unordered_map>
#include <vector>
#include <string>

namespace config {

typedef std::unordered_map<std::string, std::vector<detector::Circle>> ImagesLUT;

class MetricsConfig {
private:
  ImagesLUT _images_lookup;

public:
  struct Metric {
  private:
    uint32_t _true_positive;
    uint32_t _true_negative;

    uint32_t _false_positive;
    uint32_t _false_negative;

  public:
    Metric(uint32_t true_positive, 
           uint32_t true_negative,
           uint32_t false_positive,
           uint32_t false_negative);

    Metric(const Metric& that);
    Metric& operator=(const Metric& that);

    double precision() const;
    double recall() const;
    double f1Score() const;

    Metric operator+(const Metric& that) const;

    inline uint32_t tp() const { return _true_positive; }
    inline uint32_t tn() const { return _true_negative; }
    inline uint32_t fp() const { return _false_positive; }
    inline uint32_t fn() const { return _false_negative; }
  };

  static MetricsConfig fromFile(const std::string& file);

  MetricsConfig();
  MetricsConfig(const MetricsConfig& that);
  MetricsConfig& operator=(const MetricsConfig& that);

  inline const std::vector<detector::Circle>& getCirclesForImage(const std::string& image_id) const {
    return _images_lookup.at(image_id);
  }

  void addImage(const std::string& image, 
                const std::vector<detector::Circle>& circles);

  MetricsConfig::Metric getConfusionMetric(const std::string& image,
                                           const std::vector<detector::Circle>& circles) const;

  MetricsConfig::Metric getConfusionMetric(const ImagesLUT& lut) const;
};

} // namespace config

#endif // METRICS_CONFIG_H
