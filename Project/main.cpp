#include <iostream>
#include <vector>
#include<opencv2/opencv.hpp>

#include "args_parser.h"
#include "circle.h"
#include "file_utils.h"
#include "pipeline.h"
#include "reporter.h"

void DrawResult(const std::string& file_name,
                const std::vector<detector::Circle>& circles,
                const std::string& output_directory) {
    cv::Mat result = cv::imread(file_name);
    detector::Circle::drawAll(result, circles);

    std::string image_name;

    if (!output_directory.empty()) {
        const auto& final_file_name = files::GetFileName(file_name) + "_report" + files::GetFileExtension(file_name);

        image_name = files::Join({ files::GetAbsolutePath(output_directory),
                                   final_file_name });
    } else {
        image_name = files::ReplaceFilename(file_name, 
                                            files::GetFileName(file_name) + "_report");
    }

    cv::imwrite(image_name, result);
    cv::imshow(file_name, result);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void OnFiles(const std::vector<std::string>& raw_files,
             const std::string& output_directory,
             bool is_debug) {
    if (!output_directory.empty() && !files::IsDirectory(output_directory)) {
        throw std::runtime_error(output_directory + " is not a directory.");
    }

    report::Reporter reporter;
    detector::Pipeline pipeline(is_debug /* is_debug */);

    std::vector<std::string> files;

    for (const auto& raw_file: raw_files) {
        if (files::IsDirectory(raw_file)) {
            files::ListFiles(raw_file, files);
        } else {
            files.push_back(raw_file);
        }
    }

    for (const auto& file_name: files) {
        cv::Mat image = cv::imread(file_name, cv::IMREAD_COLOR);

        if (image.empty()) {
            // not image, skipping
            continue;
        }

        const auto& circles = pipeline.detect(file_name, image);
        reporter.reportDetectedCircles(file_name, circles);
        DrawResult(file_name, circles, output_directory);
    }
}

int main(int argc, char* argv[]) {
    try {
        args::ArgsDict args = args::ParseArgs(argc, argv);

        if (args::DetectArgs(args, { args::FLAG_TITLE_UNSPECIFIED }, { "-d", "-o" })) {
            const auto& files = args::GetStringList(args, args::FLAG_TITLE_UNSPECIFIED);
            const auto& output_directory = args::GetString(args, "-o", "");
            const auto& is_debug = args::HasFlag(args, "-d");

            OnFiles(files, output_directory, is_debug);
        } else {
            std::cout << "Cannot find suitable command for the given flags." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
