//
// Created by jakub on 12.5.2018.
//

#ifdef WIN32
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")	// hide windows console
#endif

#include <Magick++.h>
#include "ImageFrame/ui.h"
#include <build_defines.h>

using namespace Magick;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

	// choose application icon
#ifdef __linux__
    try{
        QApplication::setWindowIcon(QIcon(install_dir + "icons/icon.png"));
    } catch (...){
        std::cout<<"Icon not found"<<std::endl;
    }
#elif WIN32
	try {
		QApplication::setWindowIcon(QIcon("icons/icon.png"));
	}
	catch (...) {
		std::cout << "Icon not found" << std::endl;
	}
#endif

    MainWindow w;
    w.show();
    return QApplication::exec();
}