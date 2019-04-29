//
// Created by jakub on 12.5.2018.
//

#include "ImageFrame/ui.h"
#include "../ui/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
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
    QString output_directory = QFileDialog::getExistingDirectory(this, "Output directory", QDir::currentPath(), QFileDialog::ShowDirsOnly);
    ui->textOutput->append("Output directory: " + output_directory);
    if (output_directory == "") {
        return;
    }

    // set output directory
    imgProcessing.setOutputDir(output_directory.toStdString());

    // check if any file is added
    if (inputFiles.empty()) {
        QMessageBox msgBox;
        msgBox.setText("No input files for rendering");
        msgBox.exec();
        return;
    }

    ui->textOutput->clear();
    ui->textOutput->append(">>> Processing <<<");

    // set image size
    imgProcessing.setImageSize(ui->imageWidth->text().toFloat(), ui->imageHeight->text().toFloat());

    // set percentage or absolute width of frame for processing
    if (ui->percentage_rbtn->isChecked()) {
        imgProcessing.setPercentage(ui->percentageInput->text().toFloat());
    }else if (ui->absolute_rbtn->isChecked()) {
        imgProcessing.setFrameWidth(ui->frameWidth->text().toFloat());
    }

    // set file extension
    imgProcessing.setFileExtension(ui->fileExtension->text().toStdString());


    // prepare filenames and process them
    std::vector<std::string> files;
    for (const auto &file : inputFiles) {
        files.push_back(file.toStdString());
    }
    imgProcessing.addFrames(files);

    // successful processed
    ui->textOutput->clear();
    ui->textOutput->append(">>> Successful processed <<<");
}

void MainWindow::on_color_btn_clicked() {
    auto *c = new QColorDialog();
    c->exec();
    imgProcessing.setFrameColor(c->selectedColor().red(), c->selectedColor().green(), c->selectedColor().blue());
    std::cout<<c->selectedColor().red()<<std::endl;
    std::cout<<c->selectedColor().green()<<std::endl;
    std::cout<<c->selectedColor().blue()<<std::endl;
}

void MainWindow::on_quit_btn_clicked() {
    QCoreApplication::quit();
}