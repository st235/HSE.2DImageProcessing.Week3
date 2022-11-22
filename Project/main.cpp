#include <iostream>
#include <vector>
#include <cmath>
#include<opencv2/opencv.hpp>

#include "pipeline.h"
#include "circle.h"

#include "args_parser.h"

void OnHelp() {
    std::cout << "On Help" << std::endl;
}

void OnImages(const std::vector<std::string>& images,
             const std::string& output) {
    std::cout << "On Images" << std::endl;
}

void OnDebug(const std::string& image,
            const std::string& output) {
    std::cout << "On Debug";
}

void OnFolder(const std::string& folder,
             const std::string& output) {
    std::cout << "On Folder";
}

int main(int argc, char* argv[]) {
    try {
        args::ArgsDict args = args::ParseArgs(argc, argv);

        if (args::DetectArgs(args, { "-h" })) {
            OnHelp();
        } else if (args::DetectArgs(args, { "-d", "-o" })) {
            const auto& image = args::GetString(args, "-d");
            const auto& output = args::GetString(args, "-o");
            OnDebug(image, output);
        } else if (args::DetectArgs(args, { "-f", "-o" })) {
            const auto& folder = args::GetString(args, "-f");
            const auto& output = args::GetString(args, "-o");
            OnFolder(folder, output);
        } else if (args::DetectArgs(args, { args::FLAG_TITLE_UNSPECIFIED, "-o" })) {
            const auto& images = args::GetStringList(args, args::FLAG_TITLE_UNSPECIFIED);
            const auto& output = args::GetString(args, "-o");
            OnImages(images, output);
        } else {
            std::cout << "Cannot find suitable command for the given flags." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
