#include <iostream>
#include <vector>
#include<opencv2/opencv.hpp>

#include "args_parser.h"
#include "circle.h"
#include "circles_tracker.h"
#include "file_utils.h"
#include "pipeline.h"
#include "metrics_config.h"

void DrawResult(const std::string& file_name,
                const std::vector<detector::Circle>& circles,
                const std::string& output_directory) {
    std::cout << std::setw(50) << std::left << file_name << std::endl;
    std::cout << "Detected: " << std::setw(10) << std::left << circles.size() << " circles." << std::endl;

    int index = 1;
    for (const auto& circle: circles) {
        std::cout << "#" << std::setfill('0') << std::setw(3) << std::right << index << ". ";
        std::cout << std::setfill(' ');
        std::cout << "Center at {" << circle.x() << ", " << circle.y() << "}. ";
        std::cout << "Radius is " << circle.radius() << "." << std::endl;

        index++;
    }

    std::cout << std::endl;

    cv::Mat result = cv::imread(file_name);
    detector::Circle::drawAll(result, circles);

    const auto& report_image_name = utils::GetFileName(file_name) + "_report" + utils::GetFileExtension(file_name);

    std::string output_image_path = report_image_name;


    if (!output_directory.empty()) {
        const auto& final_file_name = utils::GetFileName(file_name) + "_report" + utils::GetFileExtension(file_name);
        output_image_path = utils::Join({ utils::GetAbsolutePath(output_directory),
                                   report_image_name });
    }

    cv::imwrite(output_image_path, result);
    cv::imshow(file_name, result);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void OnFiles(const std::vector<std::string>& raw_files,
             const std::string& output_directory,
             const std::string& config_file,
             bool is_debug) {
    if (!output_directory.empty() && !utils::IsDirectory(output_directory)) {
        throw std::runtime_error(output_directory + " is not a directory.");
    }

    detector::CirclesTracker tracker;
    detector::Pipeline pipeline(is_debug /* is_debug */);

    std::vector<std::string> files;

    for (const auto& raw_file: raw_files) {
        if (utils::IsDirectory(raw_file)) {
            utils::ListFiles(raw_file, files);
        } else {
            files.push_back(raw_file);
        }
    }

    // always process images in the same order
    // it makes easier to visually debug them
    std::sort(files.begin(), files.end());

    for (const auto& file_name: files) {
        cv::Mat image = cv::imread(file_name, cv::IMREAD_COLOR);

        if (image.empty()) {
            // not image, skipping
            continue;
        }

        const auto& image_id = utils::GetFileNameWithExtension(file_name);

        const auto& circles = pipeline.detect(file_name, image);
        tracker.addDetectedCircles(image_id, circles);

        DrawResult(file_name, circles, output_directory);
    }

    if (!config_file.empty()) {
        const auto& config = config::MetricsConfig::fromFile(config_file);
        const auto& metric = config.getConfusionMetric(tracker.imagesLut());

        std::cout << metric.tp() << "/" << metric.tn() << "/" << metric.fp() << "/" << metric.fn() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    try {
        args::ArgsDict args = args::ParseArgs(argc, argv);

        if (args::DetectArgs(args, { args::FLAG_TITLE_UNSPECIFIED }, { "-d", "-o", "-c" })) {
            const auto& files = args::GetStringList(args, args::FLAG_TITLE_UNSPECIFIED);
            const auto& output_directory = args::GetString(args, "-o", "");
            const auto& config_file = args::GetString(args, "-c", "");
            const auto& is_debug = args::HasFlag(args, "-d");

            OnFiles(files, output_directory, config_file, is_debug);
        } else {
            std::cout << "Cannot find suitable command for the given flags." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
