//
// Created by jakub on 12.5.2018.
//

#include <Magick++.h>
#include "ImageFrame/Gui.h"

using namespace std;
using namespace Magick;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Gui w;
    w.show();
    return QApplication::exec();
}