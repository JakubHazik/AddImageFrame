//
// Created by jakub on 12.5.2018.
//

#ifndef IMAGEFRAME_WIN_H
#define IMAGEFRAME_WIN_H


#include <QtWidgets>
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <iostream>
#include <QtWidgets/QVBoxLayout>
#include <QDialog>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QCheckBox>
#include "ImgProcessing.h"

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
