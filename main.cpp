#include "mainwindow.h"
#include "qapplication.h"
#include <stdio.h>
#include <stdlib.h>


extern "C" {
    #include "extApi.h"
}

int main(int argc, char *argv[]){

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

}
