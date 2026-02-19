ESP32-C3 多功能智能控制项目
https://_assets/cover.jpg <!-- 你可以放一张实物图 -->

本项目基于 ESP32-C3 Super Mini 开发板，打造了一个集 WiFi远程控制、BLE蓝牙近场控制、音乐播放、LED灯光视觉反馈 于一体的多功能智能设备。通过 MQTT 协议实现云端远程控制，通过 BLE 实现手机直接连接，同时利用无源蜂鸣器播放《两只老虎》等旋律，LED 亮度随音符频率变化，视觉听觉双重享受。

✨ 功能特性
双模控制：

WiFi + MQTT：通过公共 MQTT Broker 实现全球远程控制 。

BLE 蓝牙：手机直接连接 ESP32-C3，发送指令控制设备。

LED 视觉反馈：

开关控制（'1'开，'2'关）。

音乐播放时，LED 亮度随音符频率变化（频率越高越亮），采用指数映射让变化更明显。

音乐播放：

无源蜂鸣器播放《两只老虎》旋律，非阻塞播放，不影响其他功能。

时尚网页控制台：

部署在 GitHub Pages 上的现代化控制页面，支持毛玻璃效果、实时状态同步。

多通道通信：

WiFi 与 BLE 共存优化，通过调整广播参数减少射频冲突。

🛠️ 硬件清单
组件	说明	数量
ESP32-C3 Super Mini	主控芯片	1
无源蜂鸣器	播放音乐	1
LED	视觉反馈	1
100Ω 电阻	LED 限流	2
10µF 电解电容	电源滤波	1
面包板、杜邦线	连接	若干
🔌 硬件接线
组件	引脚	连接到 ESP32-C3	说明
LED	正极（长脚）	GPIO2 → 100Ω 电阻 → LED 正极	限流电阻串联
LED	负极（短脚）	GND	
蜂鸣器	正极（I/O）	GPIO4 → 100Ω 电阻 → 蜂鸣器正极	限流保护 GPIO
蜂鸣器	负极	GND	
10µF电容	正极（长脚）	3.3V	并联在电源和地之间，靠近 ESP32
10µF电容	负极（短脚）	GND	
重要：

所有元件必须共地（GND 连在一起）。

电容用于电源滤波，防止 LED 快速闪烁时电压波动导致复位 。

💻 软件环境配置
1. 安装 Arduino IDE
从 arduino.cc 下载安装。

2. 安装 ESP32 开发板支持
打开 Arduino IDE，进入 文件 → 首选项，在“附加开发板管理器网址”中添加：

text
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
进入 工具 → 开发板 → 开发板管理器，搜索 ESP32，安装 esp32 by Espressif Systems。

3. 安装所需库
在 项目 → 加载库 → 管理库 中搜索并安装：

PubSubClient by Nick O‘Leary (MQTT)

BLE 相关库（ESP32 内置）

WiFi（内置）

4. 下载项目代码
bash
git clone https://github.com/你的用户名/你的仓库名.git
📜 ESP32 代码说明
主程序文件：ESP32_MultiControl.ino

主要功能模块
模块	说明
WiFi 连接	连接到指定 WiFi，获取 IP 地址 
MQTT 客户端	连接到公共 Broker broker-cn.emqx.io，订阅控制主题，发布状态主题
BLE 服务器	创建 BLE 服务，广播设备名 ESP32-C3_LED，接收手机写入的指令 
音乐播放	非阻塞状态机播放《两只老虎》，使用 tone() 函数驱动蜂鸣器
LED 亮度映射	使用立方映射将音符频率转换为 PWM 占空比，让高频时 LED 更亮
指令集
指令	功能	来源
'1'	打开 LED	MQTT / BLE
'2'	关闭 LED	MQTT / BLE
'3'	播放《两只老虎》	MQTT / BLE
🌐 网页控制界面
项目包含一个现代化的 HTML 控制页面，部署在 GitHub Pages 上。

文件：index.html

功能特点
毛玻璃设计卡片，自适应移动端。

实时显示 LED 状态（开启/关闭）。

按钮控制：开灯、关灯、播放音乐。

通过 MQTT over WebSocket 与 ESP32 通信。

部署步骤
将 index.html 上传到你的 GitHub 仓库。

进入仓库 Settings → Pages，选择 main 分支，文件夹 / (root)，点击 Save。

等待几分钟，你的页面将在 https://你的用户名.github.io/仓库名/ 上线。

注意：页面中的 MQTT Broker 地址为 wss://broker-cn.emqx.io:8084/mqtt，与 ESP32 代码中的地址一致。

🚀 快速开始
硬件连接：按接线表连接所有元件。

修改 WiFi 信息：在代码中找到 ssid 和 password，替换为你的 WiFi 名称和密码。

上传代码：

选择开发板：ESP32C3 Dev Module

选择端口：你的 COM 口

点击上传

打开串口监视器（波特率 115200），查看 ESP32 的 IP 地址和运行日志。

网页控制：在浏览器中打开你的 GitHub Pages 页面，或直接输入 ESP32 的局域网 IP（端口 80）访问控制页面。

蓝牙控制：

手机下载 nRF Connect 或 LightBlue。

扫描并连接 ESP32-C3_LED。

找到可写特征值（UUID 6E400002...），在 Text 模式下发送 1、2、3。

📊 预期效果
网页点击按钮，LED 立即响应，页面状态同步更新。

蓝牙发送指令，效果相同。

点击“播放音乐”，蜂鸣器响起《两只老虎》，LED 亮度随旋律起伏（频率越高越亮）。

🔧 故障排查
问题	可能原因	解决方法
LED 不亮	接线错误、电阻过大	检查 GPIO2 连接，限流电阻建议 100Ω
蜂鸣器无声	接线错误、未串联电阻	检查 GPIO4 连接，确保串联 100Ω 电阻
网页显示“未连接”	MQTT Broker 地址错误、网络问题	检查 index.html 中的 brokerUrl 是否为 wss://broker-cn.emqx.io:8084/mqtt
网页状态不更新	ESP32 未发布状态、主题不匹配	打开浏览器 F12 控制台，查看是否有错误信息 
BLE 无法连接	设备未广播、手机不兼容	重启 ESP32，换用 nRF Connect 重试
WiFi 连接失败	密码错误、信号弱	检查串口输出，确认 WiFi 名称和密码
📁 文件结构
text
├── ESP32_MultiControl.ino   # 主程序
├── index.html                # 网页控制界面
├── README.md                 # 本文档
└── _assets/                  # 图片资源（可选）
    └── cover.jpg
📌 注意事项
电源稳定性：LED 快速闪烁时电流波动较大，10µF 电容务必接好，否则可能导致 ESP32 复位。

BLE 与 WiFi 共存：代码已优化 BLE 广播参数（setMinPreferred/setMaxPreferred），减少射频冲突。

安全提醒：如果修改代码，不要在发布状态时发送非预期字符串（如 "on-off"），否则网页无法解析。

🤝 贡献
欢迎提交 Issue 和 Pull Request！

📄 许可证
本项目采用 MIT 许可证。

📬 联系方式
项目作者：[你的名字]

GitHub：[你的 GitHub 主页链接]
