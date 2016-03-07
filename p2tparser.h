#ifndef P2TPARSER_H
#define P2TPARSER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QLibrary>
#include <QCoreApplication>

#define NORTH 0x01
#define EAST  0x02
#define SOUTH 0x04
#define WEST  0x08
#define ROBOT 0x10

#define ROBOT_WHITE 0
#define ROBOT_RED   1
#define ROBOT_GREEN 2
#define ROBOT_BLUE  3

typedef struct {
    uint grid[256];
    uint moves[256];
    uint robots[4];
    uint token;
    uint last;
} Game;

typedef uint (*SearchFunction)(Game*, uchar*, void*);

class P2tParser : public QObject
{
    Q_OBJECT
    QNetworkAccessManager *m_pNetworkAccessManager;
    SearchFunction search;
    uint solveGame(Game *game, uint robot);
    bool m_spoil, m_spoilMore;
public:
    explicit P2tParser(bool spoil, bool spoilMore, QObject *parent = 0);

signals:

private slots:
    void parseResponse(QNetworkReply *httpReply);
};

#endif // P2TPARSER_H
