#pragma once
#include <QString>
#include <libs/QJsonStruct/QJsonStruct.hpp>

struct SSLObject
{
    bool verify;
    //
    // QString cert;
    // QString key;
    QString sni;
    QList<QString> alpn;
    bool prefer_server_cipher;
    QString cipher;
    QString cipher13;
    QString curves;
    JSONSTRUCT_REGISTER(SSLObject, F(verify, sni, alpn, prefer_server_cipher, cipher, cipher13, curves))
};

struct TCPObject
{
    bool no_delay;
    bool keep_alive;
    bool reuse_port;
    bool prefer_ipv4;
    bool fast_open;
    int fast_open_qlen;
    JSONSTRUCT_REGISTER(TCPObject, F(no_delay, keep_alive, reuse_port, prefer_ipv4, fast_open, fast_open_qlen))
};

struct MuxObject
{
    bool enabled;
    int concurrency;
    int idle_timeout;
    JSONSTRUCT_REGISTER(MuxObject, F(enabled, concurrency, idle_timeout))
};

struct WebsocketObject
{
    bool enabled;
    QString path;
    QString hostname;
    QString obfuscation_password;
    QString double_tls;
    SSLObject ssl;
    JSONSTRUCT_REGISTER(WebsocketObject, F(enabled, path, hostname, obfuscation_password, double_tls, ssl))
};

struct TransportPluginObject
{
    bool enabled;
    QString type;
    QString command;
    QString plugin_option;
    QList<QString> arg;
    QList<QString> env;
    JSONSTRUCT_REGISTER(TransportPluginObject, F(enabled, type, command, plugin_option, arg, env))
};

struct ForwardProxyObject
{
    bool enabled;
    QString proxy_addr;
    int proxy_port;
    QString username;
    QString password;
    JSONSTRUCT_REGISTER(ForwardProxyObject, F(enabled, proxy_addr, proxy_port, username, password))
};

struct TrojanGoSettings
{
    QString remote_addr;
    int remote_port;
    QString local_addr;
    int local_port;
    //
    int log_level;
    QString log_file;
    //
    QList<QString> password;
    int buffer_size;
    QList<QString> dns;
    bool disable_http_check;
    //
    SSLObject ssl;
    TCPObject tcp;
    MuxObject mux;
    WebsocketObject websocket;
    TransportPluginObject transport_plugin;
    ForwardProxyObject forward_proxy;
    //
    JSONSTRUCT_REGISTER(TrojanGoSettings,
                        F(remote_addr, remote_port, local_addr, local_port, log_level, log_file, password, buffer_size, dns, disable_http_check),
                        F(ssl, tcp, mux, websocket, transport_plugin, forward_proxy))
};
