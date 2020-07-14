#pragma once
#include <QList>
#include <QMap>
#include <QString>
#include <libs/QJsonStruct/QJsonStruct.hpp>

enum TrojanGoTransportType
{
    TRANSPORT_ORIGINAL,
    TRANSPORT_WEBSOCKET
};

const static QMap<TrojanGoTransportType, QString> TRANSPORT_TYPE_STRING_MAP{ { TRANSPORT_ORIGINAL, "original" }, { TRANSPORT_WEBSOCKET, "ws" } };
const static QList<QString> VALID_SHADOWSOCKS_ENCRYPTION_LIST{ "aes-128-gcm", "aes-256-gcm", "chacha20-ietf-poly1305" };

struct TrojanGoShareLinkObject
{
    QString server;
    int port;
    QString password;
    QString sni;
    TrojanGoTransportType type;
    QString host;
    QString path;
    QString encryption;
    QString plugin;
    JSONSTRUCT_REGISTER(TrojanGoShareLinkObject, F(server, port, password, sni, type, host, path, encryption, plugin))
};
