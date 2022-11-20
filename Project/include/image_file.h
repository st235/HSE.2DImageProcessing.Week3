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

public:
  ImageFile(std::string title, cv::Mat image);

  inline std::string title() const {
    std::vector<std::string> parts = Split(_path, std::filesystem::path::preferred_separator);
    return parts[parts.size() - 1];
  }

  inline cv::Mat image() const {
    return cv::imread("coins.jpg", cv::IMREAD_COLOR);
  }
};

} // namespace detector

#endif // IMAGE_FILE_H
