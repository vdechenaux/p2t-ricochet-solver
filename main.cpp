#include <QCoreApplication>
#include <QCommandLineParser>
#include "p2tparser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("p2t-ricochet-solver");
    QCoreApplication::setApplicationVersion("1.0.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("P2t-ricochet solver");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption spoilOption(QStringList() << "s" << "spoil-me", "Shows the best score for each robot.");
    parser.addOption(spoilOption);
    QCommandLineOption spoilMoreOption(QStringList() << "m" << "more", "Shows the best solution for each robot. Use it with -s or --spoil-me.");
    parser.addOption(spoilMoreOption);

    parser.process(a);

    bool spoilEnabled = parser.isSet(spoilOption);
    bool spoilMoreEnabled = parser.isSet(spoilMoreOption);

    if (spoilMoreEnabled && !spoilEnabled)
    {
        qWarning("You need to use --spoil-me with --more !");
        return EXIT_FAILURE;
    }

    P2tParser p(spoilEnabled, spoilMoreEnabled);

    return a.exec();
}
