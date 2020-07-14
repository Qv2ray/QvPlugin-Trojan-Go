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
        return "";
    }
    const QPair<QString, QJsonObject> DeserializeOutbound(const QString &link, QString *alias, QString *errorMessage) const override
    {
        return { "trojan", {} };
    }
    const Qv2rayPlugin::QvPluginOutboundInfoObject GetOutboundInfo(const QString &protocol, const QJsonObject &outbound) const override
    {
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
