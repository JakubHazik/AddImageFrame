//
// Created by jakub on 12.5.2018.
//

#include "ImageFrame/ImgProcessing.h"

using namespace std;
using namespace cv;

void ImgProcessing::addFrames(std::vector<std::string> filenames) {
    vector<std::thread> threads;

    for (const auto &file : filenames) {
        if (threads.size() < 10) {
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
                             float percentage, bool keepOldFiles,
                             int R, int G, int B,
                             const std::string &fileExtension) {
    Magick::Image image;

    // Read a file into image object
    image.read(filename);

    auto size = image.size();
    //cout<<size.width()<<endl;
    //cout<<size.height();

    int w_additional_px = static_cast<int>( size.width() * percentage / 100);
    int h_additional_px = static_cast<int>( size.height() * percentage / 100);

    stringstream ss;
    ss << w_additional_px << "x" << h_additional_px << "+0+0";

    Magick::ColorRGB color(R, G, B);
    //olor.redQuantum(R);
    image.borderColor(color);
    image.border(ss.str());


//
//    cv::Mat image = cv::imread(filename);
//
//    int w_additional_px = static_cast<int>((int) image.cols * percentage / 100);
//    int h_additional_px = static_cast<int>((int) image.rows * percentage / 100);
//
//    Mat bkg(image.rows + h_additional_px * 2, image.cols + w_additional_px * 2, image.type(), Scalar(R, G, B));
//
//    image.copyTo(bkg(cv::Rect(w_additional_px, h_additional_px, image.cols, image.rows)));
//

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
        //imwrite(new_filename, bkg);
    } else {
        image.write(filename);
        // imwrite(filename, bkg);
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

void ImgProcessing::create(std::string filename) {
    int WIDTH = 4763, HEIGHT = 3175;

//    CvSize size;
//    IplImage *rgb_img;
//    int i = 0;
//
//    size.height = HEIGHT;
//    size.width = WIDTH;
//
//    rgb_img = cvCreateImageHeader(size, IPL_DEPTH_8U, 3);
//    rgb_img->imageData = my_device.ColorBuffer;
//    rgb_img->imageDataOrigin = rgb_img->imageData;





//
//    CvSize mSize;
//    mSize.height = 960;
//    mSize.width = 1280;
//

//    cvNamedWindow( "corners1", 1 );
//    cvShowImage( "corners1", image1 );


    int IMAGE_WIDTH = 4763, IMAGE_HEIGHT = 3175;
    Mat img;

    FILE *fp = NULL;
    char *imagedata = NULL;
    int framesize = IMAGE_WIDTH * IMAGE_HEIGHT;

//Open raw Bayer image.
    fp = fopen("/home/jakub/a/IMG_7395.CR2", "rb");

//Memory allocation for bayer image data buffer.
    imagedata = (char *) malloc(sizeof(char) * framesize);

//Read image data and store in buffer.
    fread(imagedata, sizeof(char), framesize, fp);

//Image dimension.
    //imageSize.height = IMAGE_WIDTH;
    //imageSize.width = IMAGE_HEIGHT;

//Create Opencv mat structure for image dimension. For 8 bit bayer, type should be CV_8UC1.
    img.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);

    memcpy(img.data, imagedata, framesize);
    imwrite("Image.jpg", img);


    CvSize mSize = CvSize();
    mSize.height = IMAGE_WIDTH;
    mSize.width = IMAGE_HEIGHT;
    IplImage *image1 = cvCreateImage(mSize, 8, 1);
    memcpy(image1->imageData, imagedata, framesize);

    cvShowImage("Image", image1);
    //imwrite("Image.jpg", img);
    //cvWaitKey();

//    fclose(fp);
//   imwrite("/home/jakub/a/ahoj.JPG", img);
//    free(imagedata);
    return;

//Perform demosaicing process
//    _OutputArray &RGBImage = img;
//
//    cvtColor(img, RGBImage, CV_BayerBG2BGR);
//    imwrite("/home/jakub/a/ahoj.JPG", RGBImage);
//    return;









    QString file("/home/jakub/a/180506_Jakuub&Natalia_07.jpg");

    QFile f("/home/jakub/a/180506_Jakuub&Natalia_07.jpg");
    if (!f.open(QFile::ReadOnly)) return;
    QByteArray array = f.readAll();
    QImage i((const unsigned char *) (array.data()), 4763, 3175, QImage::Format_RGB16);

//    auto rawData = array.data();
//    QImage* img = new QImage(640, 480, QImage::Format_RGB16);
//    for (int y = 0; y < img->height(); y++)
//    {
//        memcpy(img->scanLine(y), array[y], img->bytesPerLine());
//    }


    i.save(QString("/home/jakub/a/ahoj.JPG"));

//
//    QImage image(file);
//
//    QPixmap pixmap(file);
//
//    pixmap.save(QString("/home/jakub/a/b.png"));
//    image.save(QString("/home/jakub/a/c.bmp"));

}


QImage *ImgProcessing::toQImage(const std::string &filename) {
    Magick::Image image;

    // Read a file into image object
    image.read(filename);

    auto size = image.size();
    //cout<<size.width()<<endl;
    //cout<<size.height();

    int w_additional_px = static_cast<int>( size.width() * percentage / 100);
    int h_additional_px = static_cast<int>( size.height() * percentage / 100);

    stringstream ss;
    ss << w_additional_px << "x" << h_additional_px << "+0+0";

    Magick::ColorRGB color(R, G, B);
    image.borderColor(color);
    image.border(ss.str());

    using namespace Magick;
    

    cout << "toQImage:" << image.columns() << image.rows();

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

