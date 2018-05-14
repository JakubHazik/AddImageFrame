//
// Created by jakub on 12.5.2018.
//

#include <Magick++.h>
#include "ImageFrame/Gui.h"


using namespace std;
using namespace Magick;

int main(int argc, char *argv[]) {





    //return 0;

//
//    Magick::Image img();

//    Magick::Blob blobICC();
//    blobICC = image.iccColorProfile();
//    if ( blobICC.length() > 0 )
//    {
//        Magick::Image ICCProfile( blobICC );
//        ICCProfile.magick( "ICC" );
//        ICCProfile.write( (LPCTSTR) csICMFileName );
//    }


    QApplication app(argc, argv);
    Gui w;
    w.show();
    return QApplication::exec();

}