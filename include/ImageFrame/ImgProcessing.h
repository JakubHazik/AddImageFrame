//
// Created by jakub on 12.5.2018.
//

#ifndef IMAGEFRAME_IMGPROCESSING_H
#define IMAGEFRAME_IMGPROCESSING_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <thread>
#include <QtGui/QImage>
#include <QImage>

#define ADD_TO_FILENAME "_ram"

class ImgProcessing {
public:

    void addFrames(std::vector<std::string> &filenames);

    void setPercentage(const float &percentage);

    void setKeepOldFiles(const bool &keep);

    void setFrameColor(const int &R, const int &G, const int &B);

    QImage viewImage(const std::string &filename);

private:
    static void addFrame(const std::string &filename, float percentage, bool keepOldFiles, int R, int G, int B);

    int R = 255, G = 255, B = 255;
    float percentage = 3;
    bool keepOldFiles = true;
};


#endif //IMAGEFRAME_IMGPROCESSING_H
