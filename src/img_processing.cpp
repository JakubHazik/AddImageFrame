//
// Created by jakub on 12.5.2018.
//

#include "ImageFrame/img_processing.h"

using namespace std;

void ImgProcessing::addFrames(const std::vector<std::string>& filenames) {
    vector<std::thread> threads;

    for (const auto &file : filenames) {
        if (threads.size() < NUM_OF_THREADS) {
            threads.emplace_back(thread(&ImgProcessing::addFrame, this, file));
        } else {
            threads.front().join();
            threads.erase(threads.begin());
        }
    }
    for (auto &t : threads) {
        t.join();
    }
}

void ImgProcessing::setPercentage(const float &percentage) {
    this->percentage = percentage;
}

void ImgProcessing::setKeepOldFiles(const bool &keep) {
    this->keepOldFiles = keep;
}

void ImgProcessing::setFrameColor(const int &R, const int &G, const int &B) {
    this->R = R;
    this->G = G;
    this->B = B;
}

void ImgProcessing::addFrame(const std::string& filepath) {
    Magick::Image image;

    // Read a file into image object
    image.read(filepath);

    std::string filename = filepath;

    auto size = image.size();

    int w_additional_px = static_cast<int>( size.width() * percentage / 100);
    int h_additional_px = static_cast<int>( size.height() * percentage / 100);

    stringstream ss;
    ss << w_additional_px << "x" << h_additional_px << "+0+0";

    Magick::ColorRGB color(R, G, B);
    image.borderColor(color);
    image.border(ss.str());

    // edit file extension
    int extensionSize = static_cast<int>(filepath.length() - filepath.rfind('.'));
    filename.replace(filename.rfind('.'), extensionSize, fileExtension);
    filename = filename.substr(filename.rfind('/') + 1);

    cout << filename << " DONE " << endl;

    image.write(output_dir + '/' + filename);
}

void ImgProcessing::setFileExtension(const std::string &extension) {
    this->fileExtension = extension;
}

void ImgProcessing::setFrameWidth(const float &width) {
    if (this->image_w == 0 || this->image_h == 0) {
        throw std::logic_error("Size of Image was not sat");
    }
    percentage = 100*width/image_w;
}

void ImgProcessing::setImageSize(const float &width, const float &height) {
    this->image_w = width;
    this->image_h = height;
}

void ImgProcessing::setOutputDir(std::string output_dir) {
    this->output_dir = output_dir;
}