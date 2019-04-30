//
// Created by jakub on 12.5.2018.
//

#ifndef IMAGEFRAME_WIN_H
#define IMAGEFRAME_WIN_H


#include <QtWidgets>
#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include "img_processing.h"

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

private:
    Ui::MainWindow *ui;
    ImgProcessing imgProcessing;
    QStringList inputFiles;
};

#endif //IMAGEFRAME_WIN_H
