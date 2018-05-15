//
// Created by jakub on 12.5.2018.
//

#ifndef IMAGEFRAME_IMGPROCESSING_H
#define IMAGEFRAME_IMGPROCESSING_H

#include <iostream>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <thread>
#include <QtGui/QImage>
#include <QImage>
#include <QFile>
//#include <opencv2/imgproc/types_c.h>
//#include <opencv2/imgproc/imgproc.hpp>
#include <Magick++.h>
#include <QtGui/QPixmap>
#include <sstream>

#define ADD_TO_FILENAME "_ram"
#define NUM_OF_THREADS 8

class ImgProcessing {
public:

    void addFrames(std::vector<std::string> filenames);

    void setPercentage(const float &percentage);

    void setKeepOldFiles(const bool &keep);

    void setFrameColor(const int &R, const int &G, const int &B);

    void setFileExtension(const std::string &extension);

    //QImage viewImage(const std::string &filename);

    QImage *toQImage(const std::string &filename);

private:
    static void addFrame(std::string filename,
                         const float &percentage,
                         const bool &keepOldFiles,
                         const int &R, const int &G, const int &B,
                          const std::string &fileExtension);

    int R = 255, G = 255, B = 255;
    float percentage = 3;
    bool keepOldFiles = true;
    std::string fileExtension = ".jpg";
};


#endif //IMAGEFRAME_IMGPROCESSING_H
