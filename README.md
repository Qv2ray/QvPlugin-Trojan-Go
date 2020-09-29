# QvPlugin-Trojan-Go
> 适用于 Qv2ray v2.6+ 的 Trojan-Go 插件

感谢 [@p4gefault](https://github.com/p4gefau1t) 的 [Trojan-Go](https://github.com/p4gefau1t/trojan-go) 项目～

使用方法请参见 [Qv2ray 插件文档](https://qv2ray.github.io/plugins/usage.html)。

## 简明教程
1. 从 [Release](https://github.com/Qv2ray/QvPlugin-Trojan-Go/releases) 里下载对应 Qv2ray 本体版本的插件
2. 把插件放到 Qv2ray 插件目录，之后重开 Qv2ray 以加载插件
3. 进入插件管理器，选择 Trojan-Go 插件，切换到设定选项卡，把 Kernel Path 设定为 Trojan-Go 核心文件的路径。 Trojan-Go 核心文件可以在 [上游的 Release](https://github.com/p4gefau1t/trojan-go/releases) 中下载。Linux/macOS 用户要记得给 NaiveProxy 核心可执行权限。
4. 导入符合格式的分享链接，形如 `trojan-go://f@uck.me/?sni=microsoft.com&type=ws&path=%2Fgo&encryption=ss%3Baes-256-gcm%3Afuckgfw`，具体格式参见 [Trojan-Go 分享链接规范](https://github.com/p4gefau1t/trojan-go/issues/132)；或者点击添加连接，在出站类型选择 Trojan-Go，之后手动编辑设置。
5. 双击你导入/创建的节点，大功告成。
