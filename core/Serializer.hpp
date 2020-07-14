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
        query.addQueryItem("host", obj.host);
        query.addQueryItem("path", obj.path);
        if (!obj.encryption.isEmpty())
            query.addQueryItem("encryption", obj.encryption);
        if (!obj.plugin.isEmpty())
            query.addQueryItem("plugin", obj.plugin);

        url.setQuery(query);
        return url.toString();
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
            *errorMessage = "SHIT Url";
            return default;
        }
        if (link.scheme() != "trojan-go")
        {
            *errorMessage = "SHIT SCHEME";
            return default;
        }

        const auto extractQueryField = [&query](const QString &key) -> QString {
            return query.queryItemValue(key, QUrl::FullyDecoded);
        };

        TrojanGoShareLinkObject info;
        info.password = link.userName(QUrl::FullyDecoded);
        if (info.password.isEmpty())
        {
            *errorMessage = "SHIT PASSWORD";
            return default;
        }

        info.server = link.host(QUrl::FullyDecoded);
        if (info.server.isEmpty())
        {
            *errorMessage = "SHIT SERVER";
            return default;
        }

        info.port = link.port(443);
        if (info.port < 1 || info.port > 65535)
        {
            *errorMessage = "SHIT PORT";
            return default;
        }

        info.sni = extractQueryField("sni");
        if (info.sni.isEmpty())
        {
            *errorMessage = "SHIT SNI";
            return default;
        }

        const auto _type = extractQueryField("type");
        if (!TRANSPORT_TYPE_STRING_MAP.values().contains(_type))
        {
            *errorMessage = "SHIT TYPE";
            return default;
        }
        info.type = TRANSPORT_TYPE_STRING_MAP.key(_type);

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
                *errorMessage = "SHIT HOST";
                return default;
            }
        }

        info.path = extractQueryField("path");
        if (info.type == TRANSPORT_WEBSOCKET)
        {
            if (info.path.isEmpty())
            {
                *errorMessage = "SHIT PATH";
                return default;
            }
        }

        bool hasEncryption = query.hasQueryItem("encryption");
        if (hasEncryption)
        {
            info.encryption = extractQueryField("encryption");
            if (info.encryption.isEmpty())
            {
                *errorMessage = "SHIT ENCTYPRION EMPTY";
                return default;
            }
            if (info.encryption.startsWith("ss;"))
            {
                const auto ssParms = info.encryption.split(";");
                if (ssParms.length() != 3)
                {
                    *errorMessage = "SHIT SS ENC LENGTH";
                    return default;
                }
                if (!VALID_SHADOWSOCKS_ENCRYPTION_LIST.contains(ssParms[1]))
                {
                    *errorMessage = "SHIT SS ENC";
                    return default;
                }
            }
            else
            {
                *errorMessage = "SHIT ENC NOT SUPPORTED";
                return default;
            }
        }

        bool hasPlugin = query.hasQueryItem("plugin");
        if (hasPlugin)
        {
            info.plugin = extractQueryField("plugin");
            if (info.plugin.isEmpty())
            {
                *errorMessage = "SHIT PLUGIN";
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
