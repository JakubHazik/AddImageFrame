//
// Created by jakub on 12.5.2018.
//

#include <Magick++.h>
#include "ImageFrame/ui.h"
#include <build_defines.h>

using namespace Magick;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

#ifdef __linux__
    try{
        QApplication::setWindowIcon(QIcon(install_dir + "icons/icon.png"));
    } catch (...){
        std::cout<<"Icon not found"<<std::endl;
    }
#elif __WIN__

#endif

    MainWindow w;
    w.show();
    return QApplication::exec();
}