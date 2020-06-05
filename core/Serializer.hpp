#pragma once
#include "Common.hpp"
#include "QvPluginProcessor.hpp"

#include <QObject>

class SimpleSerializer : public Qv2rayPlugin::QvPluginSerializer
{
    Q_OBJECT
  public:
    explicit SimpleSerializer(QObject *parent = nullptr) : QvPluginSerializer(parent){};
    const QString SerializeOutbound(const QString &protocol,  //
                                    const QString &alias,     //
                                    const QString &groupName, //
                                    const QJsonObject &object) const override
    {
        if (protocol == "trojan")
        {
            Q_UNUSED(groupName)
            TrojanGoSettings o = TrojanGoSettings::fromJson(object);
            QString trojanUri = o.password.first().toUtf8().toPercentEncoding() +                                     //
                                "@" + o.remote_addr +                                                                 //
                                ":" + QString::number(o.remote_port) +                                                //
                                "?allowInsecure=" + QString::number((int) !(o.ssl.verify || o.ssl.verify_hostname)) + //
                                "&tfo=" + QString::number(o.tcp.fast_open) +                                          //
                                "#" + QUrl::toPercentEncoding(alias.toUtf8());
            return "trojan://" + trojanUri;
        }
        else
        {
            return "";
        }
    }
    const QPair<QString, QJsonObject> DeserializeOutbound(const QString &link, QString *alias, QString *errorMessage) const override
    {
        const QString prefix = "trojan://";
        if (!link.startsWith(prefix))
        {
            *errorMessage = ("Invalid Trojan URI");
            return {};
        }
        //
        const auto trueList = QStringList{ "true", "1", "yes", "y" };
        const QUrl trojanUrl(link);
        const QUrlQuery query(trojanUrl.query());
        *alias = trojanUrl.fragment();
        //
        TrojanGoSettings result;
        result.remote_addr = trojanUrl.host();
        result.remote_port = trojanUrl.port();
        result.password.push_back(trojanUrl.userName());
        result.ssl.sni = query.queryItemValue("sni");
        //
        result.tcp.fast_open = trueList.contains(query.queryItemValue("tfo").toLower());
        //
        bool allowAllInsecure = trueList.contains(query.queryItemValue("allowInsecure").toLower());
        result.ssl.verify = !allowAllInsecure;
        result.ssl.verify_hostname = !(allowAllInsecure || trueList.contains(query.queryItemValue("allowInsecureHostname").toLower()));
        //
        result.websocket.path = query.queryItemValue("wsPath");
        result.websocket.hostname = query.queryItemValue("wsHost");
        result.websocket.enabled = trueList.contains(query.queryItemValue("websocket").toLower());
        //
        return { "trojan", result.toJson() };
    }
    const Qv2rayPlugin::QvPluginOutboundInfoObject GetOutboundInfo(const QString &protocol, const QJsonObject &outbound) const override
    {
        if (protocol == "trojan")
        {
            auto o = TrojanGoSettings::fromJson(outbound);
            return Qv2rayPlugin::QvPluginOutboundInfoObject{ o.remote_addr, "trojan", o.remote_port };
        }
        else
        {
            return {};
        }
    }
    const QList<QString> ShareLinkPrefixes() const override
    {
        return { "trojan://" };
    }
    const QList<QString> OutboundProtocols() const override
    {
        return { "trojan" };
    }
};
