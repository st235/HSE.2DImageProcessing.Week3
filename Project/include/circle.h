#ifndef CIRCLE_H
#define CIRCLE_H

#include <cstdint>
#include <opencv2/opencv.hpp>
#include <string>

namespace detector {

class Circle {
private:
  uint32_t _x;
  uint32_t _y;
  uint32_t _radius;

public:
  struct Hash {
    size_t operator()(const Circle& that) const {
      size_t hash = std::hash<int>()(that._x);
      hash ^= std::hash<int>()(that._y);
      hash ^= std::hash<int>()(that._radius);
      return hash;
    }
  };

  static Circle fromVec3f(const cv::Vec3f& vector) {
    return Circle(static_cast<uint32_t>(cvRound(vector[0])), 
                  static_cast<uint32_t>(cvRound(vector[1])),
                  static_cast<uint32_t>(cvRound(vector[2])));
  }

  static Circle fromSerialisation(const std::string& s) {
    cv::Vec3f vector;

    size_t index = 0;
    size_t last = 0;
    size_t next = 0;

    std::string delimiter = ",";
    
    while ((next = s.find(delimiter, last)) != std::string::npos) {
      vector[index] = std::stof(s.substr(last, last - next));
      last = next + 1;
      index += 1;
    } 
    
    vector[index] = std::stof(s.substr(last));
    return fromVec3f(vector);
  }

  static void draw(cv::Mat& canvas, const Circle& circle) {
    auto center = circle.center();
    auto radius = circle.radius();
    // draw the circle center
    cv::circle(canvas, center, 3 /* radius */, cv::Scalar(0, 255, 0) /* color */, -1 /* thickness */, 8 /* lineType */, 0 /* shift */);
    // draw the circle outline
    cv::circle(canvas, center, radius, cv::Scalar(0, 0, 255) /* color */, 3 /* thickness */, 8 /* lineType */, 0 /* shift */);
  }

  static void drawAll(cv::Mat& canvas, const std::vector<Circle>& circles) {
    for (const auto& circle: circles) {
        Circle::draw(canvas, circle);
    }
  }

  Circle(uint32_t x, uint32_t y, uint32_t radius): _x(x), _y(y), _radius(radius) {
    // empty on purpose
  }

  Circle(const Circle& that): _x(that._x), _y(that._y), _radius(that._radius) {
    // empty on purpose
  }

  Circle& operator=(const Circle& that) {
    if (this != &that) {
        this->_x = that._x;
        this->_y = that._y;
        this->_radius = that._radius;
    }

    return *this;
  }

  bool operator==(const Circle& that) const {
    return (_x == that._x
            && _y == that._y
            && _radius == that._radius);
  }

  bool isWithin(uint32_t x, uint32_t y) const {
    return std::sqrt(std::pow(_x - x, 2) + std::pow(_y - y, 2)) <= _radius;
  }

  uint32_t findIntersectionArea(const Circle& that) const {
    uint32_t intersectionArea = 0;

    for (uint32_t y = _y - _radius; y <= _y + _radius; y++) {
      for (uint32_t x = _x - _radius; x <= _x + _radius; x++) {
        if (!isWithin(x, y)) {
          continue;
        }

        if (that.isWithin(x, y)) {
          intersectionArea += 1;
        }
      }
    }

    return intersectionArea;
  }

  uint32_t findUnionArea(const Circle& that) const {
    uint32_t unionArea = 0;

    for (uint32_t y = _y - _radius; y <= _y + _radius; y++) {
      for (uint32_t x = _x - _radius; x <= _x + _radius; x++) {
        if (!isWithin(x, y)) {
          continue;
        }

        unionArea += 1;
      }
    }

    for (uint32_t y = that._y - that._radius; y <= that._y + that._radius; y++) {
      for (uint32_t x = that._x - that._radius; x <= that._x + that._radius; x++) {
        if (!that.isWithin(x, y)) {
          continue;
        }

        unionArea += 1;
      }
    }

    return unionArea - this->findIntersectionArea(that);
  }

  double iou(const Circle& that) const {
    return static_cast<double>(this->findIntersectionArea(that)) / this->findUnionArea(that);
  }

  inline uint32_t x() const { return _x; }
  inline uint32_t y() const { return _y; }

  inline cv::Point center() const { return cv::Point(_x, _y); }

  inline uint32_t radius() const { return _radius; }

  ~Circle() = default;
};

}

#endif // CIRCLE_H
