#include "LegSupport.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	std::unordered_map<std::string, std::string, MyHashFunction> contrast;
    LegSupport::DLG_data(true, contrast);
    //w.show();
    return a.exec();
}
