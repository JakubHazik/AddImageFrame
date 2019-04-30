//
// Created by jakub on 12.5.2018.
//

#ifndef IMAGEFRAME_IMG_PROCESSING_H
#define IMAGEFRAME_IMG_PROCESSING_H

#include <iostream>
#include <thread>
#include <Magick++.h>
#include <sstream>
#include <exception>
#include <future>
#include <unistd.h>

#define NUM_OF_THREADS 8

class ImgProcessing {
public:

    void addFrames(std::vector<std::string> filenames);

    void setPercentage(const float &percentage);

    void setFrameColor(const int &R, const int &G, const int &B);

    void setFileExtension(const std::string &extension);

    void setFrameWidth(const float &width);

    void setImageSize(const float &width, const float &height);

    void setOutputDir(std::string output_dir);

private:
    void addFrame(const std::string& filepath);
    std::atomic<int> running_threads;

    int R = 255, G = 255, B = 255;
    float percentage = 3;
    float image_w = 0;
    float image_h = 0;
    std::string output_dir;
    std::string fileExtension = ".jpg";
};

#endif //IMAGEFRAME_IMG_PROCESSING_H
