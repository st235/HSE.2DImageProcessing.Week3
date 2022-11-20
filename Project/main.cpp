#include <iostream>
#include <vector>
#include <cmath>
#include<opencv2/opencv.hpp>

#include "pipeline.h"
#include "circle.h"

int main(int argc, char* argv[]) {
    cv::Mat img = cv::imread("coins.jpg", cv::IMREAD_COLOR);
    cv::Mat result(img);

    detector::Pipeline pipeline;
    auto circles = pipeline.detect(img);
    detector::Circle::drawAll(result, circles);

    cv::imshow("coins", result);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}
