#ifndef CIRCLE_H
#define CIRCLE_H

#include <cstdint>
#include<opencv2/opencv.hpp>

namespace detector {

class Circle {
private:
  uint32_t _x;
  uint32_t _y;
  uint32_t _radius;

public:
  static Circle fromVec3f(const cv::Vec3f& vector) {
    return Circle(static_cast<uint32_t>(cvRound(vector[0])), 
                  static_cast<uint32_t>(cvRound(vector[1])),
                  static_cast<uint32_t>(cvRound(vector[2])));
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

  inline uint32_t x() const { return _x; }
  inline uint32_t y() const { return _y; }

  inline cv::Point center() const { return cv::Point(_x, _y); }

  inline uint32_t radius() const { return _radius; }

  ~Circle() = default;
};

}

#endif // CIRCLE_H
