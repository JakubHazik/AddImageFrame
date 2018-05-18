//
// Created by jakub on 12.5.2018.
//

#include "ImageFrame/Gui.h"

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
            "/home/",
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

    textOutput->clear();
    textOutput->append(">>> Processing <<<");

    // set image size
    imgProcessing.setImageSize(imageWidth->text().toFloat(), imageHeight->text().toFloat());

    // rewriting original files
    if (rewriteFiles_cbox->checkState()) {
        imgProcessing.setKeepOldFiles(false);
    } else {
        imgProcessing.setKeepOldFiles(true);
    }

    // set percentage of frame for processing
    //imgProcessing.setPercentage(percentageInput->text().toFloat());
    imgProcessing.setFrameWidth(frameWidth->text().toFloat());

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
    quit_btn = new QPushButton("Quit");
    // connect buttons
    connect(browse_btn, SIGNAL(released()), this, SLOT(handleBrowseFiles()));
    connect(render_btn, SIGNAL(released()), this, SLOT(handelRenderImages()));
    connect(color_btn, SIGNAL(released()), this, SLOT(handleColorSettings()));
    connect(quit_btn, SIGNAL(released()), this, SLOT(handleQuitApp()));

    // crete percentage input
    fileExtension = new QLineEdit(DEFAULT_FILE_EXTENSION);

    // crete checkbox
    rewriteFiles_cbox = new QCheckBox("Rewrite original inputFiles");
    rewriteFiles_cbox->setChecked(DEFAULT_REWRITE_ORIGINAL_FILES);

    // create layout with creted widgets
    controllLayout = new QVBoxLayout;
    controllLayout->addWidget(browse_btn);
    controllLayout->addWidget(createPercentageWidget());
    //controllLayout->addWidget(createFrameSizeWidget());
    controllLayout->addWidget(new QLabel("File extension:"));
    controllLayout->addWidget(fileExtension);
    controllLayout->addWidget(color_btn);
    controllLayout->addWidget(rewriteFiles_cbox);
    controllLayout->addWidget(render_btn);
    controllLayout->addWidget(quit_btn);

    // create widget with layouted buttons
    controllPanel = new QWidget();
    controllPanel->setLayout(controllLayout);
}

QWidget *Gui::createPercentageWidget() {
    percentage_rbtn= new QRadioButton("Percentage");
    percentageInput = new QLineEdit(DEFAULT_FRAME_PERCENTAGE);
    absolute_rbtn= new QRadioButton("Absolute size");
    absolute_rbtn->setChecked(true);
    imageHeight = new QLineEdit(DEFAULT_IMAGE_HEIGHT);
    imageWidth = new QLineEdit(DEFAULT_IMAGE_WIDTH);
    frameWidth = new QLineEdit(DEFAULT_FRAME_WIDTH);

    QGroupBox *radioBtnBox = new QGroupBox("Select type",this);

    QVBoxLayout *layoutV;
    QHBoxLayout *layoutH;
    QWidget *widget;

    // vertical layout
    {
        layoutV = new QVBoxLayout;

        //percentage radiobutton
        layoutV->addWidget(percentage_rbtn);

        // horizontal percentage input
        {
            layoutH = new QHBoxLayout;
            layoutH->addWidget(new QLabel("Percentage of frame: [%]"));
            layoutH->addWidget(percentageInput);
            widget = new QWidget();
            widget->setLayout(layoutH);
        }
        layoutV->addWidget(widget);

        //absolute radiobutton
        layoutV->addWidget(absolute_rbtn);

        // horizontal Image Size
        {
            layoutH = new QHBoxLayout;
            layoutH->addWidget(new QLabel("Image size:"));
            layoutH->addWidget(new QLabel("W[cm]"));
            layoutH->addWidget(imageWidth);
            layoutH->addWidget(new QLabel("H[cm]"));
            layoutH->addWidget(imageHeight);
            widget = new QWidget();
            widget->setLayout(layoutH);
            layoutV->addWidget(widget);
        }
        layoutV->addWidget(widget);

        // horizontal Frame Width
        {
            layoutH = new QHBoxLayout;
            layoutH->addWidget(new QLabel("Frame width[mm]"));
            layoutH->addWidget(frameWidth);
            widget = new QWidget();
            widget->setLayout(layoutH);
            layoutV->addWidget(widget);
        }
        layoutV->addWidget(widget);
    }

    radioBtnBox->setLayout(layoutV);
    radioBtnBox->setFlat(true);
    return radioBtnBox;
}

void Gui::createLogPanel() {
    // crete text area
    textOutput = new QTextEdit();
    textOutput->setMinimumSize(20, 100);
    textOutput->setReadOnly(true);
}

void Gui::handleColorSettings() {
    auto *c = new QColorDialog();
    c->exec();
    imgProcessing.setFrameColor(c->selectedColor().red(), c->selectedColor().green(), c->selectedColor().blue());
}

void Gui::handleQuitApp() {
    QCoreApplication::quit();
}