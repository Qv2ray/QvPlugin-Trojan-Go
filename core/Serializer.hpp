#pragma once
#include "Common.hpp"
#include "QvPluginProcessor.hpp"

#include <QObject>
#include <QUrl>
#include <QUrlQuery>

class TrojanGoSerializer : public Qv2rayPlugin::QvPluginSerializer
{
    Q_OBJECT
  public:
    explicit TrojanGoSerializer(QObject *parent = nullptr) : QvPluginSerializer(parent){};
    const QString SerializeOutbound(const QString &protocol,  //
                                    const QString &alias,     //
                                    const QString &groupName, //
                                    const QJsonObject &object) const override
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
        query.addQueryItem("sni", obj.sni);
        query.addQueryItem("type", TRANSPORT_TYPE_STRING_MAP[obj.type]);
        if (obj.type == TRANSPORT_WEBSOCKET)
        {
            query.addQueryItem("host", QUrl::toPercentEncoding(obj.host));
            query.addQueryItem("path", QUrl::toPercentEncoding(obj.path));
            if (!obj.encryption.isEmpty())
                query.addQueryItem("encryption", QUrl::toPercentEncoding(obj.encryption));
            if (!obj.plugin.isEmpty())
                query.addQueryItem("plugin", QUrl::toPercentEncoding(obj.plugin));
        }
        url.setQuery(query);
        return url.toString(QUrl::FullyDecoded);
    }
    const QPair<QString, QJsonObject> DeserializeOutbound(const QString &url, QString *alias, QString *errorMessage) const override
    {
#define default                                                                                                                                 \
    {                                                                                                                                           \
    }
        QUrl link{ url };
        QUrlQuery query{ link };
        if (!link.isValid())
        {
            *errorMessage = "Not a valid Url";
            return default;
        }
        if (link.scheme() != "trojan-go")
        {
            *errorMessage = "Invalid scheme";
            return default;
        }

        const auto extractQueryField = [&query](const QString &key) -> QString {
            return query.queryItemValue(key, QUrl::FullyDecoded);
        };

        TrojanGoShareLinkObject info;
        info.password = link.userName(QUrl::FullyDecoded);
        if (info.password.isEmpty())
        {
            *errorMessage = "Invalid password";
            return default;
        }

        info.server = link.host(QUrl::FullyDecoded);
        if (info.server.isEmpty())
        {
            *errorMessage = "Invalid server address";
            return default;
        }

        info.port = link.port(443);
        if (info.port < 1 || info.port > 65535)
        {
            *errorMessage = "Invalid port";
            return default;
        }
        {
            bool hasSNI = query.hasQueryItem("sni");
            if (hasSNI)
            {
                info.sni = extractQueryField("sni");
                if (info.sni.isEmpty())
                {
                    *errorMessage = "Invalid SNI";
                    return default;
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
                    return default;
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
                    return default;
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
                    return default;
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
                return default;
            }
            if (info.encryption.startsWith("ss;"))
            {
                const auto ssParms = info.encryption.split(";");
                if (ssParms.length() != 2)
                {
                    *errorMessage = "Shadowsocks argument length invalid.";
                    return default;
                    const auto ssParms2 = ssParms[1].split(":");
                    if (!VALID_SHADOWSOCKS_ENCRYPTION_LIST.contains(ssParms2[0]))
                    {
                        *errorMessage = "Shadowsocks encryption not supported.";
                        return default;
                    }
                }
            }
            else
            {
                *errorMessage = "Encryption not supported.";
                return default;
            }
        }

        bool hasPlugin = query.hasQueryItem("plugin");
        if (hasPlugin)
        {
            info.plugin = extractQueryField("plugin");
            if (info.plugin.isEmpty())
            {
                *errorMessage = "Plugin Settings invalid";
                return default;
            }
        }

        *alias = link.fragment(QUrl::FullyDecoded);
        return { "trojan-go", info.toJson() };
#undef default
    }
    const Qv2rayPlugin::QvPluginOutboundInfoObject GetOutboundInfo(const QString &protocol, const QJsonObject &outbound) const override
    {
        if (protocol == "trojan-go")
            return { outbound["server"].toString(), protocol, outbound["port"].toInt() };
        return {};
    }
    const QList<QString> ShareLinkPrefixes() const override
    {
        return { "trojan-go://" };
    }
    const QList<QString> OutboundProtocols() const override
    {
        return { "trojan-go" };
    }
};
