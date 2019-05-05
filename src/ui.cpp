//
// Created by jakub on 12.5.2018.
//


#include "ImageFrame/ui.h"
#include "../ui/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(this, SIGNAL(overwriteFilesQuestion_signal(QString)), this, SLOT(overwriteFilesQuestion(QString)),Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(printLog_signal(QString)), this, SLOT(printLog(QString)));
}

void MainWindow::refresh() {
    progressBar->setValue(progress);

    QString progressMsg;
    progressMsg.sprintf("Processing [%d/%d]", static_cast<int>(progress), imagesNum);
    progressBar->setFormat(progressMsg);

    if (processingDone.isFinished()) {
        // successful processed
        disableUI(false);
        progressBar->deleteLater();
        cancelBtn->deleteLater();
        if (cancelProcessing) {
            emit printLog_signal(">>> Processing aborted");
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
    emit printLog_signal(">>> Input images:");
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

    emit printLog_signal(">>> Processing");

    // run rendering process asynchonous
    processingDone = QtConcurrent::run(this, &MainWindow::processImages, files);

    // disable UI
    disableUI(true);

    // start waiting timer
    connect(&timer, SIGNAL(timeout()), this, SLOT(refresh()));
    timer.start(100);

    // create progress bar
    progressBar = new QProgressBar(ui->statusBar);
    progressBar->setRange(0, files.size());
    progressBar->setTextVisible(true);
    ui->statusBar->addPermanentWidget(progressBar, 2);

    // create abort processing button
    cancelBtn = new QPushButton(ui->statusBar);
    cancelBtn->setText("Abort");
    connect(cancelBtn, &QPushButton::clicked, this, [&](){
        emit printLog_signal(">>> Aborting ...");
        cancelBtn->setDisabled(true);
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

void MainWindow::addFrame(std::string inputFilepath, std::string outputFilepath) {
    Magick::Image image;

    // Read a file into image object
    hdd_IO_mutex.lock();
    image.read(inputFilepath);
    hdd_IO_mutex.unlock();

    auto size = image.size();

    int w_additional_px = static_cast<int>( size.width() * framePercentage / 100);
    int h_additional_px = static_cast<int>( size.height() * framePercentage / 100);

    std::stringstream ss;
    ss << w_additional_px << "x" << h_additional_px << "+0+0";

    Magick::ColorRGB color(frameColor.red(), frameColor.green(), frameColor.blue());
    image.borderColor(color);
    image.border(ss.str());

    hdd_IO_mutex.lock();
    image.write(outputFilepath);
    hdd_IO_mutex.unlock();

    // log to text output
    emit printLog_signal(getFileNameFromPath(outputFilepath) + " DONE");
}

void MainWindow::processImages(std::vector<std::string> filepaths) {
    std::list<QFuture<void>> threads;

    imagesNum = filepaths.size();
    progress = 0;
    cancelProcessing = false;

    while (!filepaths.empty()) {
        if (cancelProcessing) {
            break;
        }

        if (threads.size() < num_of_threads) {
            std::string outputFilepath = getOutputFilePath(filepaths.back());

            if (!overwriteAll && fileExists(outputFilepath)) {
                int userDecision = emit overwriteFilesQuestion_signal(getFileNameFromPath(outputFilepath));

                if (userDecision == QMessageBox::YesToAll) {
                    emit printLog_signal(">>> All files will be overwrote");
                    overwriteAll = true;
                } else if (userDecision == QMessageBox::Abort) {
                    emit printLog_signal(">>> Processing aborted");
                    return;
                } else if (userDecision == QMessageBox::Yes) {
                    // continue in processing
                }
            }

            threads.push_back(QtConcurrent::run(this, &MainWindow::addFrame,  filepaths.back(), outputFilepath));
            filepaths.pop_back();

        } else {
            for (auto it = threads.begin(); it != threads.end(); it++) {
                if(it->isFinished()) {
                    it = threads.erase(it);
                    progress++;
                }
            }
            QThread::msleep(50);
        }
    }

    while (!threads.empty()) {
        for (auto it = threads.begin(); it != threads.end(); it++) {
            if (it->isFinished()) {
                it = threads.erase(it);
                progress++;
            }
        }
        QThread::msleep(50);
    }

    emit printLog_signal(">>> Successful processed");
}

std::string MainWindow::getOutputFilePath(std::string inputFilepath) {
    // edit file extension
    int extensionSize = static_cast<int>(inputFilepath.length() - inputFilepath.rfind('.'));
    inputFilepath.replace(inputFilepath.rfind('.'), extensionSize, fileExtension);
    inputFilepath = inputFilepath.substr(inputFilepath.rfind('/') + 1);
    return output_directory + '/' + inputFilepath;
}

int MainWindow::overwriteFilesQuestion(QString filename) {
    QMessageBox msgBox;
    msgBox.setText("Do you wan to overwrite file: " + filename);
    msgBox.setIcon(QMessageBox::Icon::Question);
    msgBox.setStandardButtons(QMessageBox::YesAll | QMessageBox::Yes | QMessageBox::Abort);
    return msgBox.exec();
}

void MainWindow::printLog(QString msg) {
    ui->textOutput->append(msg);
}

void MainWindow::disableUI(bool disable) {
    ui->fileExtension->setDisabled(disable);
    ui->browse_btn->setDisabled(disable);
    ui->color_btn->setDisabled(disable);
    ui->groupBox->setDisabled(disable);
    ui->render_btn->setDisabled(disable);
    ui->quit_btn->setDisabled(disable);
}

bool MainWindow::fileExists(std::string filepath) {
    QFileInfo check_file(QString::fromStdString(filepath));
    return (check_file.exists() && check_file.isFile());
}

QString MainWindow::getFileNameFromPath(std::string filepath) {
    auto delimiter = filepath.rfind('/') + 1;
    std::string fileName = filepath.substr(delimiter, filepath.length() - delimiter);
    return QString::fromStdString(fileName);
}
