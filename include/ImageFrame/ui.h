//
// Created by jakub on 12.5.2018.
//

#ifndef IMAGEFRAME_WIN_H
#define IMAGEFRAME_WIN_H

#include <iostream>
#include <Magick++.h>
#include <sstream>
#include <future>

#include <QtWidgets>
#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QThread>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void on_browse_btn_clicked();
    void on_color_btn_clicked();
    void on_render_btn_clicked();
    void on_quit_btn_clicked();
    void refresh();

private:
    Ui::MainWindow *ui;

    bool check_before_render();
    void addFrame(std::string filepath);
    void processImages(std::vector<std::string> filepaths);

    QTimer timer;
    QFuture<void> processingDone;
    QStringList inputFiles;
    std::atomic_uint_fast16_t progress;
    std::atomic<bool> cancelProcessing;
    std::string output_directory;

    QMutex hdd_IO_mutex;            // only one process can access IO HDD
    QProgressBar * progressBar;
    QPushButton *cancelBtn;
    float framePercentage;
    std::string fileExtension;
    QColor frameColor = QColor("white");

    const unsigned short num_of_threads = QThread::idealThreadCount();
};

#endif //IMAGEFRAME_WIN_H
