//
// Created by jakub on 12.5.2018.
//

#include "ImageFrame/ImgProcessing.h"


using namespace std;
using namespace cv;

void ImgProcessing::addFrames(std::vector<std::string> &filenames) {
    vector<std::thread> threads;
    for (const auto &file : filenames) {
        threads.emplace_back(thread(ImgProcessing::addFrame, file, percentage, keepOldFiles, R, G, B));
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

void ImgProcessing::addFrame(const std::string &filename, float percentage, bool keepOldFiles, int R, int G, int B) {
    cv::Mat image = cv::imread(filename);

    int w_additional_px = (int) image.cols * percentage / 100;
    int h_additional_px = (int) image.rows * percentage / 100;

    Mat bkg(image.rows + h_additional_px * 2, image.cols + w_additional_px * 2, CV_8UC3, Scalar(R, G, B));

    image.copyTo(bkg(cv::Rect(w_additional_px, h_additional_px, image.cols, image.rows)));

    if (keepOldFiles) {
        //create new filename
        size_t found = filename.rfind('.');
        string new_filename = filename;
        new_filename.insert(found, ADD_TO_FILENAME);

        imwrite(new_filename, bkg);
    } else {
        imwrite(filename, bkg);
    }
}

QImage ImgProcessing::viewImage(const std::string &filename) {
    cv::Mat image = cv::imread(filename);

    int w_additional_px = (int) image.cols * percentage / 100;
    int h_additional_px = (int) image.rows * percentage / 100;

    Mat bkg(image.rows + h_additional_px * 2, image.cols + w_additional_px * 2, CV_8UC3, Scalar(R, G, B));

    image.copyTo(bkg(cv::Rect(w_additional_px, h_additional_px, image.cols, image.rows)));

    switch (bkg.type()) {
        // 8-bit, 4 channel
        case CV_8UC4: {
            QImage qImage(bkg.data,
                          bkg.cols, bkg.rows,
                          static_cast<int>(bkg.step),
                          QImage::Format_ARGB32);

            return qImage;
        }

        // 8-bit, 3 channel
        case CV_8UC3: {
            QImage qImage(bkg.data,
                          bkg.cols, bkg.rows,
                          static_cast<int>(bkg.step),
                          QImage::Format_RGB888);

            return qImage.rgbSwapped();
        }

        // 8-bit, 1 channel
        case CV_8UC1: {
            QImage qImage(bkg.data,
                          bkg.cols, bkg.rows,
                          static_cast<int>(bkg.step),
                          QImage::Format_Grayscale8);


            return qImage;
        }

        default:
            throw std::runtime_error("This type of image is not supported. Type: " + bkg.type());
    }
}



