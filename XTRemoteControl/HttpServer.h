#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QJsonObject>
#include <QNetworkReply>

/**
 * @brief HTTP服务器类
 *
 * 这个类实现了一个简单的HTTP服务器,用于向测试机柜发送HTTP请求,
 * 设置测试机柜温箱的温度、湿度,启动和停止温箱,以及获取温箱的状态和错误信息。
 * 服务器监听指定的端口,接收测试机柜的连接,并向测试机柜发送HTTP请求。
 * 服务器支持发送以下请求:
 * - 设置温箱温度
 * - 设置温箱湿度
 * - 启动温箱
 * - 停止温箱
 * - 获取温箱状态
 * - 获取温箱错误信息
 */
class HttpServer : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param port 服务器监听的端口号
     * @param parent 父对象指针
     */
    explicit HttpServer(quint16 port, QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~HttpServer();

    /**
     * @brief 设置温箱温度
     * @param ip 测试机柜的IP地址
     * @param temperature 要设置的温度值
     */
    void setTemperature(const QString& ip, double temperature);

    /**
     * @brief 设置温箱湿度
     * @param ip 测试机柜的IP地址
     * @param humidity 要设置的湿度值
     */
    void setHumidity(const QString& ip, double humidity);

    /**
     * @brief 启动温箱
     * @param ip 测试机柜的IP地址
     */
    void startTemperatureChamber(const QString& ip);

    /**
     * @brief 停止温箱
     * @param ip 测试机柜的IP地址
     */
    void stopTemperatureChamber(const QString& ip);

    /**
     * @brief 获取温箱状态
     * @param ip 测试机柜的IP地址
     */
    void getTemperatureChamberStatus(const QString& ip);

    /**
     * @brief 获取温箱错误信息
     * @param ip 测试机柜的IP地址
     */
    void getTemperatureChamberError(const QString& ip);

private slots:
    /**
     * @brief 新连接建立时的槽函数
     *
     * 当有新的测试机柜连接建立时,会触发这个槽函数。
     * 函数中会为新的连接创建一个QTcpSocket对象,并连接相应的信号和槽。
     */
    void onNewConnection();

    /**
     * @brief 连接断开时的槽函数
     *
     * 当测试机柜断开连接时,会触发这个槽函数。
     * 函数中会进行必要的清理工作,如移除连接的测试机柜socket。
     */
    void onSocketDisconnected();

private:
    /**
     * @brief 发送HTTP POST请求
     * @param ip 测试机柜的IP地址
     * @param path 请求的资源路径
     * @param data 请求的数据
     */
    void sendPostRequest(const QString& ip, const QString& path, const QJsonObject& data = QJsonObject());

    /**
     * @brief 处理测试机柜的HTTP响应
     * @param reply 测试机柜的HTTP响应
     */
    void handleResponse(QNetworkReply* reply);

    QTcpServer m_server;                      ///< TCP服务器对象,用于监听端口和接受新连接
    QMap<QString, QTcpSocket*> m_ipSocketMap; ///< 存储测试机柜的IP地址和对应的socket的映射关系
};

#endif // HTTPSERVER_H