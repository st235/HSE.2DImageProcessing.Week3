#ifndef IMAGE_FILE_H
#define IMAGE_FILE_H

#include <string>
#include<opencv2/opencv.hpp>

namespace {

std::vector<std::string> Split(const std::string& origin, char c) {
    std::stringstream stream(origin);
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(stream, segment, '_'))
    {
    seglist.push_back(segment);
    }

    return seglist;
}

} // namespace

namespace cv {

class Mat;

} // namespace cv

namespace detector {

class ImageFile {
private:
  std::string _path;
  std::string _title;
  cv::Mat _image;

public:
  static ImageFile fromFile(const std::string& path) {
    return ImageFile(path, cv::imread("coins.jpg", cv::IMREAD_COLOR));
  }

  ImageFile(std::string path, cv::Mat image): _path(path), _image(image) {
    std::vector<std::string> parts = Split(_path, std::filesystem::path::preferred_separator);
    return parts[parts.size() - 1];
  }

  inline std::string title() const {
    return _title;
  }

  inline std::string path() const {
    return _path;
  }

  inline cv::Mat image() const {
    return _image;
  }
};

} // namespace detector

#endif // IMAGE_FILE_H
