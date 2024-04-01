#pragma once

#include <QObject>

/**
 * @brief �����豸��
 *
 * �����ʾһ�������豸,�����������IP��ַ���¶ȡ�ʪ�ȡ�����״̬�ʹ�����������,
 * �Լ������¶ȡ�ʪ�ȡ�������ֹͣ�ȿ��ƺ�����������״̬�����仯ʱ,�ᷢ��statusChanged�źš�
 */
class TemperatureChamber : public QObject {
    Q_OBJECT

public:
    /**
     * @brief ���캯��
     * @param ip �����豸��IP��ַ
     * @param parent ������ָ��
     */
    TemperatureChamber(const QString& ip, QObject* parent = nullptr);

    /**
     * @brief ��ȡ�����豸��IP��ַ
     * @return �����豸��IP��ַ
     */
    QString ip() const;


    /**
	* @brief ��ȡ�����豸������״̬
	* @return �����豸������״̬
	*/
    bool isConnected() const;

    /**
     * @brief ��ȡ�����豸�ĵ�ǰ�¶�
     * @return �����豸�ĵ�ǰ�¶�
     */
    double temperature() const;

    /**
     * @brief ��ȡ�����豸�ĵ�ǰʪ��
     * @return �����豸�ĵ�ǰʪ��
     */
    double humidity() const;

    /**
     * @brief ��ȡ�����豸�ĵ�ǰ����״̬
     * @return �����豸�ĵ�ǰ����״̬
     */
    QString runningStatus() const;

    /**
     * @brief ��ȡ�����豸�ĵ�ǰ�������
     * @return �����豸�ĵ�ǰ�������
     */
    QString errorCode() const;

    /**
     * @brief ���������豸��Ŀ���¶�
     * @param temperature Ŀ���¶�
     */
    void setTemperature(double temperature);

    /**
     * @brief ���������豸��Ŀ��ʪ��
     * @param humidity Ŀ��ʪ��
     */
    void setHumidity(double humidity);

    /**
     * @brief ���������豸
     */
    void start();

    /**
     * @brief ֹͣ�����豸
     */
    void stop();

    /**
     * @brief ���������豸������״̬
     * @param status ����״̬
     */
    void setRunningStatus(const QString& status);

    /**
     * @brief ���������豸�Ĵ������
     * @param errorCode �������
     */
    void setErrorCode(const QString& errorCode);

signals:
    /**
     * @brief ����״̬�仯�ź�
     *
     * ��������¶ȡ�ʪ�ȡ�����״̬�������뷢���仯ʱ,�ᷢ�����źš�
     */
    void statusChanged();

private:
    QString m_ip;           ///< �����豸��IP��ַ
    double m_temperature;   ///< �����豸�ĵ�ǰ�¶�
    double m_humidity;      ///< �����豸�ĵ�ǰʪ��
    QString m_runningStatus;///< �����豸�ĵ�ǰ����״̬
    QString m_errorCode;    ///< �����豸�ĵ�ǰ�������
    bool m_connected;       ///< �����豸������״̬
};