//
// Created by jakub on 12.5.2018.
//

#ifndef IMAGEFRAME_IMG_PROCESSING_H
#define IMAGEFRAME_IMG_PROCESSING_H

#include <iostream>
#include <thread>
#include <QtGui/QImage>
#include <QImage>
#include <QFile>
#include <Magick++.h>
#include <QtGui/QPixmap>
#include <sstream>
#include <exception>

#define NUM_OF_THREADS 8

class ImgProcessing {
public:

    void addFrames(const std::vector<std::string>& filenames);

    void setPercentage(const float &percentage);

    void setKeepOldFiles(const bool &keep);

    void setFrameColor(const int &R, const int &G, const int &B);

    void setFileExtension(const std::string &extension);

    void setFrameWidth(const float &width);

    void setImageSize(const float &width, const float &height);

    void setOutputDir(std::string output_dir);

private:
    void addFrame(const std::string& filepath);

    int R = 255, G = 255, B = 255;
    float percentage = 3;
    float image_w = 0;
    float image_h = 0;
    bool keepOldFiles = true;
    std::string output_dir;
    std::string fileExtension = ".jpg";
};

#endif //IMAGEFRAME_IMG_PROCESSING_H
