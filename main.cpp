#include <QCoreApplication>
#include <QCommandLineParser>
#include "p2tparser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("p2t-ricochet-solver");
    QCoreApplication::setApplicationVersion("1.0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("P2t-ricochet solver");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption spoilOption(QStringList() << "s" << "spoil-me", "Shows the best score for each robot.");
    parser.addOption(spoilOption);
    QCommandLineOption spoilMoreOption(QStringList() << "m" << "more", "Shows the best solution for each robot. Use it with -s or --spoil-me.");
    parser.addOption(spoilMoreOption);
    QCommandLineOption dateOption(QStringList() << "d" << "date", "Solves the grid of the given date. The format of <date> must be yyyymmdd. Default value is today date.", "date");
    parser.addOption(dateOption);

    parser.process(a);

    bool spoilEnabled = parser.isSet(spoilOption);
    bool spoilMoreEnabled = parser.isSet(spoilMoreOption);

    if (spoilMoreEnabled && !spoilEnabled)
    {
        qWarning("You need to use --spoil-me with --more !");
        return EXIT_FAILURE;
    }

    QString date = parser.value(dateOption);
    if (parser.isSet(dateOption))
    {
        if (!date.contains(QRegExp("^[0-9]{8}$")))
        {
            qWarning("The given date is malformed !");
            return EXIT_FAILURE;
        }
    }

    P2tParser p(spoilEnabled, spoilMoreEnabled, date);

    return a.exec();
}
