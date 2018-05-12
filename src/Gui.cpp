//
// Created by jakub on 12.5.2018.
//

#include "ImageFrame/Gui.h"
#include "ImageFrame/ImgProcessing.h"

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
}

void Gui::handleBrowseFiles() {
    inputFiles = QFileDialog::getOpenFileNames(
            this,
            "Select one or more inputFiles to open",
            "/home/jakub/",
            "Images (*.png *.jpg *.cr2)");

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

    if (rewriteFiles_cbox->checkState()) {
        imgProcessing.setKeepOldFiles(false);
    } else {
        imgProcessing.setKeepOldFiles(true);
    }

    // set percentage of frame for processing
    imgProcessing.setPercentage(percentageInput->text().toFloat());

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
    percentageInput = new QLineEdit("3");

    // crete checkbox
    rewriteFiles_cbox = new QCheckBox("Rewrite original inputFiles");

    // create layout with creted widgets
    controllLayout = new QVBoxLayout;
    controllLayout->addWidget(browse_btn);
    controllLayout->addWidget(percentageInput);
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
    QImage image = imgProcessing.viewImage(inputFiles.back().toStdString());

    QDialog *viewImage = new QDialog();
    QLabel *imageLabel = new QLabel(viewImage);
    imageLabel->setPixmap(QPixmap::fromImage(image));
    imageLabel->setMinimumSize(800, 600);
    imageLabel->setMaximumSize(1920, 1080);
    viewImage->exec();
}
