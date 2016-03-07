#include "p2tparser.h"

P2tParser::P2tParser(bool spoil, bool spoilMore, QObject *parent) : QObject(parent)
{
    search = (SearchFunction)QLibrary::resolve(QCoreApplication::applicationDirPath()+"/fogleman-ricochet-solver/_ricochet", "search");

    m_pNetworkAccessManager = new QNetworkAccessManager(this);
    connect(m_pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseResponse(QNetworkReply*)));
    m_pNetworkAccessManager->get(QNetworkRequest(QUrl("http://jeux.prise2tete.fr/p2t-ricochet/p2t-ricochet.php5")));
    m_spoil = spoil;
    m_spoilMore = spoilMore;
}

void P2tParser::parseResponse(QNetworkReply *httpReply)
{
    QString html = httpReply->readAll();
    httpReply->deleteLater();

    int gridPos = 0;
    Game game;
    game.last = 0;
    memset(game.moves, 0, sizeof(game.moves));

    for (int y=0; y<16; y++)
    {
        for (int x=0; x<16; x++)
        {
            QRegExp cellRegex(QString("<[^>]*id=\"%1_%2\"[^>]*>").arg((y*2)+1).arg((x*2)+1));
            QRegExp NcellRegex(QString("<[^>]*id=\"%1_%2\"[^>]*>").arg((y*2)).arg((x*2)+1));
            QRegExp EcellRegex(QString("<[^>]*id=\"%1_%2\"[^>]*>").arg((y*2)+1).arg((x*2)+2));
            QRegExp ScellRegex(QString("<[^>]*id=\"%1_%2\"[^>]*>").arg((y*2)+2).arg((x*2)+1));
            QRegExp WcellRegex(QString("<[^>]*id=\"%1_%2\"[^>]*>").arg((y*2)+1).arg((x*2)));
            QRegExp robotReg("r([1-4])");
            html.indexOf(cellRegex);
            html.indexOf(NcellRegex);
            html.indexOf(EcellRegex);
            html.indexOf(ScellRegex);
            html.indexOf(WcellRegex);
            QString cell = cellRegex.cap(0);
            QString Ncell = NcellRegex.cap(0);
            QString Ecell = EcellRegex.cap(0);
            QString Scell = ScellRegex.cap(0);
            QString Wcell = WcellRegex.cap(0);

            game.grid[gridPos] = 0;
            if (Ncell.contains("bord"))
                game.grid[gridPos] = game.grid[gridPos] | NORTH;
            if (Ecell.contains("bord"))
                game.grid[gridPos] = game.grid[gridPos] | EAST;
            if (Scell.contains("bord"))
                game.grid[gridPos] = game.grid[gridPos] | SOUTH;
            if (Wcell.contains("bord"))
                game.grid[gridPos] = game.grid[gridPos] | WEST;
            if (cell.contains(robotReg))
            {
                game.grid[gridPos] = game.grid[gridPos] | ROBOT;
                game.robots[robotReg.cap(1).toInt()-1] = gridPos;
            }
            if (cell.contains("but") && !((x == 7 || x == 8) && (y == 7 || y == 8)))
                game.token = gridPos;

            gridPos++;
        }
    }

    uint total = 0;
    total += solveGame(&game, ROBOT_WHITE);
    total += solveGame(&game, ROBOT_RED);
    total += solveGame(&game, ROBOT_GREEN);
    total += solveGame(&game, ROBOT_BLUE);

    QTextStream cout(stdout);
    cout << QString("Best score is %1.").arg(total) << endl;
    QCoreApplication::quit();
}

uint P2tParser::solveGame(Game *game, uint robot)
{
    QStringList colors = (QStringList() << "White" << "Red" << "Green" << "Blue");
    QStringList directions = (QStringList() << "" << "North" << "East" << "" << "South" << "" << "" << "" << "West");

    if (robot != 0)
    {
        uint oldSelectedRobot = game->robots[0];
        game->robots[0] = game->robots[robot];
        game->robots[robot] = oldSelectedRobot;

        colors.swap(0, robot);
    }

    uchar path[32];
    uint depth = search(game, path, NULL);

    if (m_spoil)
    {
        QTextStream cout(stdout);
        cout << QString("Optimal solution for %1 robot: %2 movements.").arg(colors.first()).arg(depth) << endl;
        for (uint i = 0; i < depth; i++)
        {
            uint value = path[i];
            uint color = (value >> 4) & 0x0f;
            uint direction = value & 0x0f;
            if (m_spoilMore)
                cout << colors.at(color) << ": " << directions.at(direction) << endl;
        }
    }

    return depth;
}
