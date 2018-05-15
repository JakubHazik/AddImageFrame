//
// Created by jakub on 12.5.2018.
//

#ifndef IMAGEFRAME_WIN_H
#define IMAGEFRAME_WIN_H


#include <QtWidgets>
#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include "ImgProcessing.h"
#include "ImageFrame/ImgProcessing.h"

#define DEFAULT_FILE_EXTENSION ".jpg"
#define DEFAULT_FRAME_PERCENTAGE "3"
#define DEFAULT_KEEP_ORIGINAL_FILES true

class Gui : public QMainWindow {
Q_OBJECT

public:
    explicit Gui(QWidget *parent = 0);

private slots:

    void handleBrowseFiles();

    void handelRenderImages();

    void handleColorSettings();

    void handleViewImage();

private:
    void createControllPanel();

    void createLogPanel();

    ImgProcessing imgProcessing;
    QPushButton *color_btn;
    QPushButton *view_image_btn;
    QStringList inputFiles;
    QLineEdit *percentageInput;
    QLineEdit *fileExtension;
    QCheckBox *rewriteFiles_cbox;
    QWidget *mainWidget;
    QHBoxLayout *mainLayout;
    QWidget *controllPanel;
    QPushButton *browse_btn;
    QPushButton *render_btn;
    QTextEdit *textOutput;
    QVBoxLayout *controllLayout;
};

#endif //IMAGEFRAME_WIN_H
