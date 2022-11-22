#include <iostream>
#include <vector>
#include <cmath>
#include<opencv2/opencv.hpp>

#include "args_parser.h"
#include "circle.h"
#include "file_utils.h"
#include "pipeline.h"
#include "report.h"

void DrawResult(const std::string& image,
                const std::vector<detector::Circle>& circles) {
    cv::Mat result = cv::imread(image);
    detector::Circle::drawAll(result, circles);
    cv::imshow(image, result);
    cv::waitKey(0);
}

void OnHelp() {
    std::cout << "On Help" << std::endl;
}

void OnImages(const std::vector<std::string>& images,
              const std::string& output,
              bool is_debug,
              bool show_result) {
    report::Report report(output);
    detector::Pipeline pipeline(is_debug /* is_debug */);

    for (const auto& image: images) {
        cv::Mat img = cv::imread(image, cv::IMREAD_COLOR);

        const auto& circles = pipeline.detect(image, img);
        report.appendCircles(image, circles);

        if (show_result) {
            DrawResult(image, circles);
        }
    }

    cv::destroyAllWindows();
}

void OnDebug(const std::string& image,
            const std::string& output,
            bool show_result) {
    if (!files::IsFile(image)) {
         std::cout << '\"' << image << '\"' << " is not an image." << std::endl;
        return;
    }

    std::vector<std::string> images;
    images.push_back(image);

    OnImages(images, output, true /* is_debug */, show_result);
}

void OnFolder(const std::string& folder,
              const std::string& output,
              bool show_result) {
    if (!files::IsDirectory(folder)) {
        std::cout << '\"' << folder << '\"' << " is not a directory." << std::endl;
        return;
    }

    std::vector<std::string> files;
    files::ListFiles(folder, files);

    OnImages(files, output, false /* is_debug */, show_result);
}

int main(int argc, char* argv[]) {
    try {
        args::ArgsDict args = args::ParseArgs(argc, argv);

        if (args::DetectArgs(args, { "-h" })) {
            OnHelp();
        } else if (args::DetectArgs(args, { "-d", "-o" }, { "-r" })) {
            const auto& image = args::GetString(args, "-d");
            const auto& output = args::GetString(args, "-o");
            bool should_show_result = args::HasFlag(args, "-r");

            OnDebug(image, output, should_show_result);
        } else if (args::DetectArgs(args, { "-f", "-o" }, { "-r" })) {
            const auto& folder = args::GetString(args, "-f");
            const auto& output = args::GetString(args, "-o");
            bool should_show_result = args::HasFlag(args, "-r");

            OnFolder(folder, output, should_show_result);
        } else if (args::DetectArgs(args, { args::FLAG_TITLE_UNSPECIFIED, "-o" }, { "-r" })) {
            const auto& images = args::GetStringList(args, args::FLAG_TITLE_UNSPECIFIED);
            const auto& output = args::GetString(args, "-o");
            bool should_show_result = args::HasFlag(args, "-r");

            OnImages(images, output, false /* is_debug */, should_show_result);
        } else {
            std::cout << "Cannot find suitable command for the given flags." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
