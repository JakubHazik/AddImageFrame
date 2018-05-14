//
// Created by jakub on 12.5.2018.
//

#include "ImageFrame/Gui.h"
#define DEFAULT_FILE_EXTENSION ".jpg"
#define DEFAULT_FRAME_PERCENTAGE "3"
#define DEFAULT_KEEP_ORIGINAL_FILES true

Gui::Gui(QWidget *parent) {

    setWindowTitle("Application for add frame");
    setMinimumSize(200, 200);

    createControllPanel();
    createLogPanel();

    // crete main layout
    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(controllPanel);
    mainLayout->addWidget(textOutput);

    // create main widget with created layout
    mainWidget = new QWidget();
    mainWidget->setLayout(mainLayout);

    // add widget to MainWindow
    this->setCentralWidget(mainWidget);

    // set implicit values for rendering images
    imgProcessing.setFileExtension(DEFAULT_FILE_EXTENSION);
    imgProcessing.setPercentage((const float &) DEFAULT_FRAME_PERCENTAGE);
    imgProcessing.setKeepOldFiles(DEFAULT_KEEP_ORIGINAL_FILES);

    //imgProcessing.create("nev");
}

void Gui::handleBrowseFiles() {
    inputFiles = QFileDialog::getOpenFileNames(
            this,
            "Select one or more inputFiles to open",
            "/home/jakub/",
            "Images (*.png *.jpg)");

    textOutput->clear();
    textOutput->append("Inputed images:");
    for (const auto &s:inputFiles) {
        QStringList path = s.split("/");
        textOutput->append(path.back());
    }
}

void Gui::handelRenderImages() {
    // check if any file is added
    if (inputFiles.empty()) {
        QMessageBox msgBox;
        msgBox.setText("No input files for rendering");
        msgBox.exec();
        return;
    }

    // rewriting original files
    if (rewriteFiles_cbox->checkState()) {
        imgProcessing.setKeepOldFiles(false);
    } else {
        imgProcessing.setKeepOldFiles(true);
    }

    // set percentage of frame for processing
    imgProcessing.setPercentage(percentageInput->text().toFloat());

    // set file extension
    imgProcessing.setFileExtension(fileExtension->text().toStdString());

    // prepare filenames and process them
    std::vector<std::string> files;
    for (const auto &file : inputFiles) {
        files.push_back(file.toStdString());
    }
    imgProcessing.addFrames(files);

    // successful processed
    textOutput->clear();
    textOutput->append(">>> Successful processed <<<");
}

void Gui::createControllPanel() {
    //create buttons
    browse_btn = new QPushButton("Add images");
    render_btn = new QPushButton("Render");
    color_btn = new QPushButton("Set color");
    view_image_btn = new QPushButton("View image");
    // connect buttons
    connect(browse_btn, SIGNAL(released()), this, SLOT(handleBrowseFiles()));
    connect(render_btn, SIGNAL(released()), this, SLOT(handelRenderImages()));
    connect(color_btn, SIGNAL(released()), this, SLOT(handleColorSettings()));
    connect(view_image_btn, SIGNAL(released()), this, SLOT(handleViewImage()));

    // crete percentage input
    percentageInput = new QLineEdit(DEFAULT_FRAME_PERCENTAGE);
    fileExtension = new QLineEdit(DEFAULT_FILE_EXTENSION);

    // crete checkbox
    rewriteFiles_cbox = new QCheckBox("Rewrite original inputFiles");

    // create layout with creted widgets
    controllLayout = new QVBoxLayout;
    controllLayout->addWidget(browse_btn);
    controllLayout->addWidget(new QLabel("Percentage of frame:"));
    controllLayout->addWidget(percentageInput);
    controllLayout->addWidget(new QLabel("File extension:"));
    controllLayout->addWidget(fileExtension);
    controllLayout->addWidget(color_btn);
    controllLayout->addWidget(rewriteFiles_cbox);
    controllLayout->addWidget(view_image_btn);
    controllLayout->addWidget(render_btn);

    // create widget with layouted buttons
    controllPanel = new QWidget();
    controllPanel->setLayout(controllLayout);
}

void Gui::createLogPanel() {
    // crete text area
    textOutput = new QTextEdit();
    textOutput->setMinimumSize(20, 100);
    textOutput->setReadOnly(true);
    //textOutput->append("askdasda");
}

void Gui::handleColorSettings() {
    QColorDialog *c = new QColorDialog();
    c->exec();
    imgProcessing.setFrameColor(c->selectedColor().red(), c->selectedColor().green(), c->selectedColor().blue());
}

void Gui::handleViewImage() {
    // check if any file is added
    if (inputFiles.empty()) {
        QMessageBox msgBox;
        msgBox.setText("No input files for rendering");
        msgBox.exec();
        return;
    }

    // prepare image for Qt
    //QImage image = imgProcessing.viewImage(inputFiles.back().toStdString());
    QImage *image = imgProcessing.toQImage(inputFiles.back().toStdString());

    QDialog *viewImage = new QDialog();
    QLabel *imageLabel = new QLabel(viewImage);
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    imageLabel->setMinimumSize(800, 600);
    imageLabel->setMaximumSize(1920, 1080);
    viewImage->exec();
}
