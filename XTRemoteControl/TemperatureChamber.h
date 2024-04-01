#pragma once

#include <QObject>

/**
 * @brief 温箱设备类
 *
 * 该类表示一个温箱设备,包含了温箱的IP地址、温度、湿度、运行状态和错误代码等属性,
 * 以及设置温度、湿度、启动和停止等控制函数。当温箱状态发生变化时,会发出statusChanged信号。
 */
class TemperatureChamber : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param ip 温箱设备的IP地址
     * @param parent 父对象指针
     */
    TemperatureChamber(const QString& ip, QObject* parent = nullptr);

    /**
     * @brief 获取温箱设备的IP地址
     * @return 温箱设备的IP地址
     */
    QString ip() const;


    /**
	* @brief 获取温箱设备的连接状态
	* @return 温箱设备的连接状态
	*/
    bool isConnected() const;

    /**
     * @brief 获取温箱设备的当前温度
     * @return 温箱设备的当前温度
     */
    double temperature() const;

    /**
     * @brief 获取温箱设备的当前湿度
     * @return 温箱设备的当前湿度
     */
    double humidity() const;

    /**
     * @brief 获取温箱设备的当前运行状态
     * @return 温箱设备的当前运行状态
     */
    QString runningStatus() const;

    /**
     * @brief 获取温箱设备的当前错误代码
     * @return 温箱设备的当前错误代码
     */
    QString errorCode() const;

    /**
     * @brief 设置温箱设备的目标温度
     * @param temperature 目标温度
     */
    void setTemperature(double temperature);

    /**
     * @brief 设置温箱设备的目标湿度
     * @param humidity 目标湿度
     */
    void setHumidity(double humidity);

    /**
     * @brief 启动温箱设备
     */
    void start();

    /**
     * @brief 停止温箱设备
     */
    void stop();

    /**
     * @brief 设置温箱设备的运行状态
     * @param status 运行状态
     */
    void setRunningStatus(const QString& status);

    /**
     * @brief 设置温箱设备的错误代码
     * @param errorCode 错误代码
     */
    void setErrorCode(const QString& errorCode);

signals:
    /**
     * @brief 温箱状态变化信号
     *
     * 当温箱的温度、湿度、运行状态或错误代码发生变化时,会发出此信号。
     */
    void statusChanged();

private:
    QString m_ip;           ///< 温箱设备的IP地址
    double m_temperature;   ///< 温箱设备的当前温度
    double m_humidity;      ///< 温箱设备的当前湿度
    QString m_runningStatus;///< 温箱设备的当前运行状态
    QString m_errorCode;    ///< 温箱设备的当前错误代码
    bool m_connected;       ///< 温箱设备的连接状态
};