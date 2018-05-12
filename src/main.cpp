//
// Created by jakub on 12.5.2018.
//

#include "ImageFrame/Gui.h"


using namespace std;

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    Gui w;
    w.show();
    return QApplication::exec();

}