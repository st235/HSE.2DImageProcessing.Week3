#include <iostream>
#include <vector>
#include<opencv2/opencv.hpp>

#include "args_parser.h"
#include "circle.h"
#include "circles_tracker.h"
#include "file_utils.h"
#include "pipeline.h"
#include "metrics_config.h"

namespace {

typedef config::MetricsConfig::Metric Metric;

void OnPrintMetrics(const Metric& metric) {
        std::cout << std::endl;

        std::cout << "The test set has " << metric.tp() + metric.fn() << " coins." << std::endl;

        std::cout << std::endl;

        std::cout << "Confusion matrix values:" << std::endl;
        std::cout << std::setw(20) << std::left << "true positive"
            << std::setw(20) << std::left << metric.tp() << std::endl;
        std::cout << std::setw(20) << std::left << "true negative"
            << std::setw(20) << std::left << metric.tn() << std::endl;
        std::cout << std::setw(20) << std::left << "false positive"
            << std::setw(20) << std::left << metric.fp() << std::endl;
        std::cout << std::setw(20) << std::left << "false negative"
            << std::setw(20) << std::left << metric.fn() << std::endl;

        std::cout << std::endl;

        std::cout << "Prediction metrics:" << std::endl;
        std::cout << std::setw(20) << std::left << "precision"
            << std::setw(20) << std::left << metric.precision() << std::endl;
        std::cout << std::setw(20) << std::left << "recall"
            << std::setw(20) << std::left << metric.recall() << std::endl;
        std::cout << std::setw(20) << std::left << "f1"
            << std::setw(20) << std::left << metric.f1Score() << std::endl;
}

void OnPrintDetectedCircles(const std::string& image_id,
                            const std::string& file_path,
                            const std::vector<detector::Circle>& detected_circles) {
    std::cout << std::setw(50) << std::left << image_id << std::endl;

    std::cout << "File path: " 
        << std::setw(50) << std::left << file_path << std::endl;

    std::cout << "Detected: " 
        << std::setw(5) << std::left << detected_circles.size() 
        << " circles." << std::endl;

    auto circle_id = 1;

    for (const auto& circle: detected_circles) {
        std::cout << "#" << std::setfill('0')
            << std::setw(3) << std::right
            << circle_id << ". ";

        // needed to reset setfill('0')
        std::cout << std::setfill(' ');

        std::cout << "Center{x=" << circle.x() << ", y=" << circle.y() << "}, ";
        std::cout << "Radius{" << circle.radius() << "}." << std::endl;

        circle_id++;
    }

    std::cout << std::endl;
}

void OnSaveReportImage(const std::string& file_path,
                       const std::vector<detector::Circle>& detected_circles,
                       const std::string& override_output_prefix) {
    cv::Mat report_image = cv::imread(file_path);
    detector::Circle::drawAll(report_image, detected_circles);

    const auto& report_image_name = 
        utils::GetFileName(file_path) + "_report" 
        + utils::GetFileExtension(file_path);

    std::string output_image_path = report_image_name;

    // if output prefix 
    if (!override_output_prefix.empty()) {
        output_image_path = utils::Join({ 
            utils::GetAbsolutePath(override_output_prefix), report_image_name });
    }

    cv::imwrite(output_image_path, report_image);
    cv::imshow(file_path, report_image);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void OnProcessFiles(const std::vector<std::string>& raw_files,
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

    // always process images in the same order,
    // it would be easier to visually debug them
    std::sort(files.begin(), files.end());

    for (const auto& file_path: files) {
        cv::Mat image = cv::imread(file_path, cv::IMREAD_COLOR);

        if (image.empty()) {
            // not an image, skipping
            continue;
        }

        const auto& image_id = utils::GetFileNameWithExtension(file_path);

        const auto& detected_circles = pipeline.detect(file_path, image);
        tracker.addDetectedCircles(image_id, detected_circles);

        OnPrintDetectedCircles(image_id, file_path, detected_circles);
        OnSaveReportImage(file_path, detected_circles, output_directory);
    }

    if (!config_file.empty()) {
        std::cout << "P.S.: Calculating metrics may take a while." << std::endl;

        const auto& config = config::MetricsConfig::fromFile(config_file);
        const auto& metric = config.getConfusionMetric(tracker.imagesLut());

        OnPrintMetrics(metric);
    }
}

} // namespace

int main(int argc, char* argv[]) {
    try {
        args::ArgsDict args = args::ParseArgs(argc, argv);

        if (args::DetectArgs(args, 
                { args::FLAG_TITLE_UNSPECIFIED } /* mandatory flags */,
                { "-d", "-o", "-c" } /* optional flags */)) {
            const auto& files = args::GetStringList(args, args::FLAG_TITLE_UNSPECIFIED);
            const auto& output_directory = args::GetString(args, "-o", "" /* default */);
            const auto& config_file = args::GetString(args, "-c", "" /* default */);
            const auto& is_debug = args::HasFlag(args, "-d");

            OnProcessFiles(files, output_directory, config_file, is_debug);
        } else {
            std::cout << "Cannot find suitable command for the given flags." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
