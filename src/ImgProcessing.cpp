//
// Created by jakub on 12.5.2018.
//

#include "ImageFrame/ImgProcessing.h"

using namespace std;
using namespace cv;

void ImgProcessing::addFrames(std::vector<std::string> filenames) {
    vector<std::thread> threads;

    for (const auto &file : filenames) {
        if (threads.size() < NUM_OF_THREADS) {
            threads.emplace_back(
                    thread(ImgProcessing::addFrame, file, percentage, keepOldFiles, R, G, B, fileExtension));
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

void ImgProcessing::addFrame(std::string filename,
                             const float &percentage, const bool &keepOldFiles,
                             const int &R, const int &G, const int &B,
                             const std::string &fileExtension) {
    Magick::Image image;

    // Read a file into image object
    image.read(filename);

    auto size = image.size();

    int w_additional_px = static_cast<int>( size.width() * percentage / 100);
    int h_additional_px = static_cast<int>( size.height() * percentage / 100);

    stringstream ss;
    ss << w_additional_px << "x" << h_additional_px << "+0+0";

    Magick::ColorRGB color(R, G, B);
    image.borderColor(color);
    image.border(ss.str());

    // edit file extension
    int extensionSize = static_cast<int>(filename.length() - filename.rfind('.'));
    filename.replace(filename.rfind('.'), extensionSize, fileExtension);
    cout << filename << endl;

    if (keepOldFiles) {
        //create new filename
        size_t found = filename.rfind('.');
        string new_filename = filename;
        new_filename.insert(found, ADD_TO_FILENAME);
        image.write(new_filename);
    } else {
        image.write(filename);
    }
    cout << filename << " DONE " << endl;
}

QImage ImgProcessing::viewImage(const std::string &filename) {
    cv::Mat image = cv::imread(filename);

    int w_additional_px = static_cast<int>((int) image.cols * percentage / 100);
    int h_additional_px = static_cast<int>((int) image.rows * percentage / 100);

    Mat bkg(image.rows + h_additional_px * 2, image.cols + w_additional_px * 2, image.type(), Scalar(R, G, B));

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

            //qImage.
            return qImage;
        }

        default:
            throw std::runtime_error("This type of image is not supported. Type: " + bkg.type());
    }
}

void ImgProcessing::setFileExtension(const std::string &extension) {
    this->fileExtension = extension;
}

QImage *ImgProcessing::toQImage(const std::string &filename) {
    Magick::Image image;

    // Read a file into image object
    image.read(filename);

    auto size = image.size();

    int w_additional_px = static_cast<int>( size.width() * percentage / 100);
    int h_additional_px = static_cast<int>( size.height() * percentage / 100);

    stringstream ss;
    ss << w_additional_px << "x" << h_additional_px << "+0+0";

    Magick::ColorRGB color(R, G, B);
    image.borderColor(color);
    image.border(ss.str());

    using namespace Magick;

    //cout << "toQImage:" << image.columns() << image.rows();

    QImage *newQImage = new QImage(image.columns(), image.rows(), QImage::Format_RGB888);

    Magick::ImageType type = image.type();

    const Magick::PixelPacket *pixels;
    Magick::ColorRGB rgb;
    for (int y = 0; y < newQImage->height(); y++) {
        pixels = image.getConstPixels(0, y, newQImage->width(), 1);
        for (int x = 0; x < newQImage->width(); x++) {
            rgb = (*(pixels + x));
            newQImage->setPixel(x, y, QColor((int) (255 * rgb.red()), (int) (255 * rgb.green()),
                                             (int) (255 * rgb.blue())).rgb());
        }
    }
    return newQImage;
}

