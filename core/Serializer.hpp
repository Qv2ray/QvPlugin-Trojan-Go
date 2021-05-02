#pragma once
#include "Common.hpp"
#include "QvPluginProcessor.hpp"

#include <QObject>
#include <QUrl>
#include <QUrlQuery>
#include <QtDebug>
#include <QtGlobal>

using namespace Qv2rayPlugin;

class TrojanGoSerializer : public PluginOutboundHandler
{
  public:
    explicit TrojanGoSerializer() : PluginOutboundHandler(){};
    const QString SerializeOutbound(const QString &protocol,   //
                                    const QString &alias,      //
                                    const QString &groupName,  //
                                    const QJsonObject &object, //
                                    const QJsonObject &) const override
    {
        Q_UNUSED(groupName)
        const auto obj = TrojanGoShareLinkObject::fromJson(object);
        QUrl url;
        QUrlQuery query;
        url.setScheme(protocol);
        url.setHost(obj.server);
        url.setPort(obj.port);
        url.setUserInfo(obj.password);
        url.setFragment(alias);
        url.setPath("/");
        //
        query.addQueryItem("type", TRANSPORT_TYPE_STRING_MAP[obj.type]);
        if (!obj.sni.isEmpty())
            query.addQueryItem("sni", obj.sni);
        if (obj.type == TRANSPORT_WEBSOCKET)
        {
            if (!obj.host.isEmpty())
            {
                query.addQueryItem("host", QUrl::toPercentEncoding(obj.host));
            }
            else
            {
                qWarning() << "empty host is deprecated when websocket is used";
            }
            query.addQueryItem("path", QUrl::toPercentEncoding(obj.path));
            if (!obj.encryption.isEmpty())
                query.addQueryItem("encryption", QUrl::toPercentEncoding(obj.encryption));
            if (!obj.plugin.isEmpty())
                query.addQueryItem("plugin", QUrl::toPercentEncoding(obj.plugin));
        }
        url.setQuery(query);
        return url.toString(QUrl::FullyEncoded);
    }
    const QPair<QString, QJsonObject> DeserializeOutbound(const QString &url, QString *alias, QString *errorMessage) const override
    {
        QUrl link{ url };
        QUrlQuery query{ link };
        if (!link.isValid())
        {
            *errorMessage = "Not a valid Url";
            return {};
        }
        if (link.scheme() != "trojan-go")
        {
            *errorMessage = "Invalid scheme";
            return {};
        }

        const auto extractQueryField = [&query](const QString &key) -> QString {
            return query.queryItemValue(key, QUrl::FullyDecoded);
        };

        TrojanGoShareLinkObject info;
        info.password = link.userName(QUrl::FullyDecoded);
        if (info.password.isEmpty())
        {
            *errorMessage = "Invalid password";
            return {};
        }

        info.server = link.host(QUrl::FullyDecoded);
        if (info.server.isEmpty())
        {
            *errorMessage = "Invalid server address";
            return {};
        }

        info.port = link.port(443);
        if (info.port < 1 || info.port > 65535)
        {
            *errorMessage = "Invalid port";
            return {};
        }
        {
            bool hasSNI = query.hasQueryItem("sni");
            if (hasSNI)
            {
                info.sni = extractQueryField("sni");
                if (info.sni.isEmpty())
                {
                    *errorMessage = "Invalid SNI";
                    return {};
                }
            }
            else
            {
                info.sni = info.server;
            }
        }
        {
            const auto hasType = query.hasQueryItem("type");
            if (hasType)
            {
                const auto _type = extractQueryField("type");
                if (!TRANSPORT_TYPE_STRING_MAP.values().contains(_type))
                {
                    *errorMessage = "Invalid Type";
                    return {};
                }
                info.type = TRANSPORT_TYPE_STRING_MAP.key(_type);
            }
            else
            {
                info.type = TRANSPORT_ORIGINAL;
            }
        }
        {
            bool hasHost = query.hasQueryItem("host");
            if (!hasHost)
            {
                info.host = info.server;
            }
            else
            {
                info.host = extractQueryField("host");
                if (info.host.isEmpty())
                {
                    *errorMessage = "Invalid Host";
                    return {};
                }
            }
        }
        {
            info.path = extractQueryField("path");
            if (info.type == TRANSPORT_WEBSOCKET)
            {
                if (info.path.isEmpty())
                {
                    *errorMessage = "Invalid Path";
                    return {};
                }
            }
        }
        bool hasEncryption = query.hasQueryItem("encryption");
        if (hasEncryption)
        {
            info.encryption = extractQueryField("encryption");
            if (info.encryption.isEmpty())
            {
                *errorMessage = "Encryption should not be empty";
                return {};
            }
            if (info.encryption.startsWith("ss;"))
            {
                const auto ssParms = info.encryption.split(";");
                if (ssParms.length() != 2)
                {
                    *errorMessage = "Shadowsocks argument length invalid.";
                    return {};
                    const auto ssParms2 = ssParms[1].split(":");
                    if (!VALID_SHADOWSOCKS_ENCRYPTION_LIST.contains(ssParms2[0]))
                    {
                        *errorMessage = "Shadowsocks encryption not supported.";
                        return {};
                    }
                }
            }
            else
            {
                *errorMessage = "Encryption not supported.";
                return {};
            }
        }

        bool hasPlugin = query.hasQueryItem("plugin");
        if (hasPlugin)
        {
            info.plugin = extractQueryField("plugin");
            if (info.plugin.isEmpty())
            {
                *errorMessage = "Plugin Settings invalid";
                return {};
            }
        }

        *alias = link.fragment(QUrl::FullyDecoded);
        return { "trojan-go", info.toJson() };
    }

    const Qv2rayPlugin::OutboundInfoObject GetOutboundInfo(const QString &protocol, const QJsonObject &outbound) const override
    {
        if (protocol == "trojan-go")
            return {
                { INFO_SERVER, outbound["server"].toString() }, //
                { INFO_PORT, outbound["port"].toInt() },        //
                { INFO_PROTOCOL, protocol },
                { INFO_SNI, outbound["sni"] } //
            };
        return {};
    }

    const void SetOutboundInfo(const QString &protocol, const OutboundInfoObject &info, QJsonObject &outbound) const override
    {
        if (protocol != "trojan-go")
            return;
        if (info.contains(INFO_PORT))
            outbound["port"] = info[INFO_PORT].toInt();
        if (info.contains(INFO_SERVER))
            outbound["server"] = info[INFO_SERVER].toInt();
        if (info.contains(INFO_SNI))
            outbound["sni"] = info[INFO_SNI].toString();
    }

    const QList<QString> SupportedLinkPrefixes() const override
    {
        return { "trojan-go://" };
    }

    const QList<QString> SupportedProtocols() const override
    {
        return { "trojan-go" };
    }
};
