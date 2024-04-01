#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QJsonObject>
#include <QNetworkReply>

/**
 * @brief HTTP��������
 *
 * �����ʵ����һ���򵥵�HTTP������,��������Ի�����HTTP����,
 * ���ò��Ի���������¶ȡ�ʪ��,������ֹͣ����,�Լ���ȡ�����״̬�ʹ�����Ϣ��
 * ����������ָ���Ķ˿�,���ղ��Ի��������,������Ի�����HTTP����
 * ������֧�ַ�����������:
 * - ���������¶�
 * - ��������ʪ��
 * - ��������
 * - ֹͣ����
 * - ��ȡ����״̬
 * - ��ȡ���������Ϣ
 */
class HttpServer : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief ���캯��
     * @param port �����������Ķ˿ں�
     * @param parent ������ָ��
     */
    explicit HttpServer(quint16 port, QObject* parent = nullptr);

    /**
     * @brief ��������
     */
    ~HttpServer();

    /**
     * @brief ���������¶�
     * @param ip ���Ի����IP��ַ
     * @param temperature Ҫ���õ��¶�ֵ
     */
    void setTemperature(const QString& ip, double temperature);

    /**
     * @brief ��������ʪ��
     * @param ip ���Ի����IP��ַ
     * @param humidity Ҫ���õ�ʪ��ֵ
     */
    void setHumidity(const QString& ip, double humidity);

    /**
     * @brief ��������
     * @param ip ���Ի����IP��ַ
     */
    void startTemperatureChamber(const QString& ip);

    /**
     * @brief ֹͣ����
     * @param ip ���Ի����IP��ַ
     */
    void stopTemperatureChamber(const QString& ip);

    /**
     * @brief ��ȡ����״̬
     * @param ip ���Ի����IP��ַ
     */
    void getTemperatureChamberStatus(const QString& ip);

    /**
     * @brief ��ȡ���������Ϣ
     * @param ip ���Ի����IP��ַ
     */
    void getTemperatureChamberError(const QString& ip);

private slots:
    /**
     * @brief �����ӽ���ʱ�Ĳۺ���
     *
     * �����µĲ��Ի������ӽ���ʱ,�ᴥ������ۺ�����
     * �����л�Ϊ�µ����Ӵ���һ��QTcpSocket����,��������Ӧ���źźͲۡ�
     */
    void onNewConnection();

    /**
     * @brief ���ӶϿ�ʱ�Ĳۺ���
     *
     * �����Ի���Ͽ�����ʱ,�ᴥ������ۺ�����
     * �����л���б�Ҫ��������,���Ƴ����ӵĲ��Ի���socket��
     */
    void onSocketDisconnected();

private:
    /**
     * @brief ����HTTP POST����
     * @param ip ���Ի����IP��ַ
     * @param path �������Դ·��
     * @param data ���������
     */
    void sendPostRequest(const QString& ip, const QString& path, const QJsonObject& data = QJsonObject());

    /**
     * @brief ������Ի����HTTP��Ӧ
     * @param reply ���Ի����HTTP��Ӧ
     */
    void handleResponse(QNetworkReply* reply);

    QTcpServer m_server;                      ///< TCP����������,���ڼ����˿ںͽ���������
    QMap<QString, QTcpSocket*> m_ipSocketMap; ///< �洢���Ի����IP��ַ�Ͷ�Ӧ��socket��ӳ���ϵ
};

#endif // HTTPSERVER_H