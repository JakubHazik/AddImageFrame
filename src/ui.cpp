//
// Created by jakub on 12.5.2018.
//


#include "ImageFrame/ui.h"
#include "../ui/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

void MainWindow::refresh() {
    progressBar->setValue(progress);

    if (processingDone.isFinished()) {
        // successful processed
        ui->centralwidget->setDisabled(false);
        delete progressBar;
        delete cancelBtn;
        ui->textOutput->clear();
        if (cancelProcessing) {
            ui->textOutput->append(">>> Processing aborted <<<");
        } else {
            ui->textOutput->append(">>> Successful processed <<<");
        }
        timer.stop();
    }
}

void MainWindow::on_browse_btn_clicked() {
    inputFiles = QFileDialog::getOpenFileNames(
            this,
            "Select one or more input files to open",
            QDir::currentPath(),
            "Images (*.png *.jpg)");

    ui->textOutput->clear();
    ui->textOutput->append("Inputed images:");
    for (const auto &s:inputFiles) {
        QStringList path = s.split("/");
        ui->textOutput->append(path.back());
    }
}

void MainWindow::on_render_btn_clicked() {
    // check if any file is added
    if (inputFiles.empty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Icon::Warning);
        msgBox.setText("No input files for rendering");
        msgBox.exec();
        return;
    }

    // set percentage or absolute width of frame for processing
    if (ui->percentage_rbtn->isChecked()) {
        framePercentage = ui->percentageInput->text().toFloat();
        if (framePercentage < 0) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Icon::Critical);
            msgBox.setText("Wrong percentage value");
            msgBox.exec();
            return;
        }

    }else if (ui->absolute_rbtn->isChecked()) {
        float imgWidth = ui->imageWidth->text().toFloat();
        if (imgWidth < 1) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Icon::Critical);
            msgBox.setText("Wrong image width");
            msgBox.exec();
            return;
        }

        float frameWidth = ui->frameWidth->text().toFloat();
        if (frameWidth < 0) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Icon::Critical);
            msgBox.setText("Wrong frame width");
            msgBox.exec();
            return;
        }

        framePercentage = 100*frameWidth/imgWidth;
    }

    fileExtension = '.' + ui->fileExtension->currentText().toStdString();

    output_directory = QFileDialog::getExistingDirectory(this, "Output directory", QDir::currentPath(), QFileDialog::ShowDirsOnly).toStdString();
    if (output_directory.empty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Icon::Warning);
        msgBox.setText("No output directory for rendering");
        msgBox.exec();
        return;
    }

    // prepare filenames and process them
    std::vector<std::string> files;
    for (const auto &file : inputFiles) {
        files.push_back(file.toStdString());
    }

    ui->textOutput->clear();
    ui->textOutput->append(">>> Processing <<<");

    processingDone = QtConcurrent::run(this, &MainWindow::processImages, files);

    // disable UI
    ui->centralwidget->setDisabled(true);
    ui->quit_btn->setDisabled(false);

    // start waiting timer
    connect(&timer, SIGNAL(timeout()), this, SLOT(refresh()));
    timer.start(100);

    // create progress bar
    progressBar = new QProgressBar();
    progressBar->setRange(0, files.size());
    progressBar->setTextVisible(true);
    progressBar->setFormat("Processing");
    ui->statusBar->addPermanentWidget(progressBar, 2);

    // create abort processing button
    cancelBtn = new QPushButton();
    cancelBtn->setText("Abort");
    connect(cancelBtn, &QPushButton::clicked, this, [&](){
        cancelProcessing = true;
    });
    ui->statusBar->addPermanentWidget(cancelBtn, 0);
}

void MainWindow::on_color_btn_clicked() {
    auto *c = new QColorDialog();
    c->exec();
    frameColor = c->selectedColor();
}

void MainWindow::on_quit_btn_clicked() {
    QCoreApplication::quit();
}

void MainWindow::addFrame(std::string filepath) {
    Magick::Image image;
    // Read a file into image object
    image.read(filepath);

    auto size = image.size();

    int w_additional_px = static_cast<int>( size.width() * framePercentage / 100);
    int h_additional_px = static_cast<int>( size.height() * framePercentage / 100);

    std::stringstream ss;
    ss << w_additional_px << "x" << h_additional_px << "+0+0";

    Magick::ColorRGB color(frameColor.red(), frameColor.green(), frameColor.blue());
    image.borderColor(color);
    image.border(ss.str());

    // edit file extension
    int extensionSize = static_cast<int>(filepath.length() - filepath.rfind('.'));
    filepath.replace(filepath.rfind('.'), extensionSize, fileExtension);
    filepath = filepath.substr(filepath.rfind('/') + 1);

    image.write(output_directory + '/' + filepath);

    std::cout << filepath << " DONE " << std::endl;
}

void MainWindow::processImages(std::vector<std::string> filepaths) {
    std::list<QFuture<void>> threads;

    progress = 0;
    cancelProcessing = false;

    while (!filepaths.empty()) {
        if (cancelProcessing) {
            break;
        }

        if (threads.size() < NUM_OF_THREADS) {
            threads.push_back(QtConcurrent::run(this, &MainWindow::addFrame,  filepaths.back()));
            filepaths.pop_back();
            progress++;
        } else {
            for (auto it = threads.begin(); it != threads.end(); it++) {
                if(it->isFinished()) {
                    it = threads.erase(it);
                }
            }
            QThread::msleep(50);
        }
    }

    while (!threads.empty()) {
        for (auto it = threads.begin(); it != threads.end(); it++) {
            if (it->isFinished()) {
                it = threads.erase(it);
            }
        }
        QThread::msleep(50);
    }
}
