//
// Created by jakub on 12.5.2018.
//

#include <Magick++.h>
#include "ImageFrame/Gui.h"

using namespace std;
using namespace Magick;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

#ifdef __linux__
    try{
        QApplication::setWindowIcon(QIcon("/usr/local/bin/icons/icon.png"));
    } catch (...){
        std::cout<<"Icon not found"<<endl;
    }
#endif

    Gui w;
    w.show();
    return QApplication::exec();
}