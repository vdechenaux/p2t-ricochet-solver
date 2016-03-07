#include <QCoreApplication>
#include "p2tparser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    P2tParser p;

    return a.exec();
}
