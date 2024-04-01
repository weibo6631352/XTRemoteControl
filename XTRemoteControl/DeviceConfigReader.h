#pragma once
#include <QList>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QDir>
// ר���豸����ö��
enum class DeviceType {
	TemperatureChamber,  // ����
	VibrationTable,      // ��̨
	// �����������������ר���豸����
};

// ���Ի���������Ϣ�ṹ��
struct TestCabinetConfig {
	QString name;              // ��������
	QString ip;                // ����IP��ַ
	DeviceType deviceType;     // ר���豸����
	QString deviceName;        // ר���豸����
	QString mysqlHost;         // MySQL������ַ
	int mysqlPort;             // MySQL�˿ں�
	QString mysqlUser;         // MySQL�û���
	QString mysqlPassword;     // MySQL����
	QString mysqlDatabase;     // MySQL���ݿ�����
	// �����������������������Ϣ,��ͨ��Э�顢��ʱʱ���
};

// �������ö�ȡ����
class DeviceConfigReader {
public:
	virtual ~DeviceConfigReader() {}
	// ���麯��,���ڶ�ȡ���Ի���������Ϣ
	virtual QList<TestCabinetConfig> readTestCabinetConfigs() = 0;
};

// JSON���ö�ȡ����
class JsonDeviceConfigReader : public DeviceConfigReader {
public:
	// ʵ�ִ�JSON�ļ���ȡ���Ի���������Ϣ�Ĺ���
	QList<TestCabinetConfig> readTestCabinetConfigs() override {
		QList<TestCabinetConfig> configs;

		// ��ȡconfigĿ¼�µ�����JSON�����ļ�
		QDir configDir("config/CabinetConfig");
		QStringList jsonFiles = configDir.entryList(QStringList() << "cabinet_config_*.json", QDir::Files);
		for (const QString& fileName : jsonFiles) {
			QString filePath = configDir.filePath(fileName);
			// ��ȡJSON�����ļ�
			QFile configFile(filePath);
			if (!configFile.open(QIODevice::ReadOnly)) {
				qWarning() << "Failed to open config file:" << configFile.errorString();
				continue;
			}
			QByteArray configData = configFile.readAll();
			configFile.close();

			// ����JSON����
			QJsonDocument jsonDoc = QJsonDocument::fromJson(configData);
			if (jsonDoc.isNull() || !jsonDoc.isObject()) {
				qWarning() << "Failed to parse JSON config:" << fileName;
				continue;
			}
			QJsonObject jsonObj = jsonDoc.object();
			TestCabinetConfig config;
			config.name = jsonObj["name"].toString();
			config.ip = jsonObj["ip"].toString();
			QString deviceTypeStr = jsonObj["deviceType"].toString();
			if (deviceTypeStr == "TemperatureChamber") {
				config.deviceType = DeviceType::TemperatureChamber;
			}
			else if (deviceTypeStr == "VibrationTable") {
				config.deviceType = DeviceType::VibrationTable;
			}
			config.deviceName = jsonObj["deviceName"].toString();
			config.mysqlHost = jsonObj["mysqlHost"].toString();
			config.mysqlPort = jsonObj["mysqlPort"].toInt();
			config.mysqlUser = jsonObj["mysqlUser"].toString();
			config.mysqlPassword = jsonObj["mysqlPassword"].toString();
			config.mysqlDatabase = jsonObj["mysqlDatabase"].toString();
			configs.append(config);
		}

		return configs;
	}
};

class DeviceConfigManager {
public:
	// ��ȡ�豸���ù���������ʵ��
	static DeviceConfigManager& instance() {
		static DeviceConfigManager instance;
		return instance;
	}

	// �������ö�ȡ��
	void setConfigReader(DeviceConfigReader* reader) {
		configReader = reader;
	}

	// ��ȡ���Ի���������Ϣ
	QList<TestCabinetConfig> getTestCabinetConfigs() {
		if (testCabinetConfigs.isEmpty()) {
			if (configReader) {
				testCabinetConfigs = configReader->readTestCabinetConfigs();
			}
		}
		return testCabinetConfigs;
	}

	// ���ز��Ի���������Ϣ
	void loadConfigs() {
		if (configReader) {
			testCabinetConfigs = configReader->readTestCabinetConfigs();
		}
	}

	// ����ip��ȡ���Ի���������Ϣ
	TestCabinetConfig getTestCabinetConfigByIp(const QString& ip) {
		for (const TestCabinetConfig& config : testCabinetConfigs) {
			if (config.ip == ip) {
				return config;
			}
		}
		return TestCabinetConfig();
	}

	// �����豸���ƻ�ȡ���Ի���������Ϣ
	TestCabinetConfig getTestCabinetConfigByDeviceName(const QString& deviceName) {
		for (const TestCabinetConfig& config : testCabinetConfigs) {
			if (config.deviceName == deviceName) {
				return config;
			}
		}
		return TestCabinetConfig();
	}

private:
	DeviceConfigManager() {}
	DeviceConfigManager(const DeviceConfigManager&) = delete;
	DeviceConfigManager& operator=(const DeviceConfigManager&) = delete;

	DeviceConfigReader* configReader = nullptr;
	QList<TestCabinetConfig> testCabinetConfigs;
};
//
//// ʹ��ʾ��
//void useConfigExample() {
//	// �������ö�ȡ��ΪJSON���ö�ȡ��
//	DeviceConfigManager::instance().setConfigReader(new JsonDeviceConfigReader());
//	DeviceConfigManager::instance().loadConfigs();  // �������ʱ��������������Ϣ
//
//	// ��ȡ���Ի���������Ϣ
//	QList<TestCabinetConfig> configs = DeviceConfigManager::instance().getTestCabinetConfigs();
//
//	// ʹ��������Ϣ������Ӧ�Ĳ���
//	for (const TestCabinetConfig& config : configs) {
//		qDebug() << "Cabinet Name:" << config.name;
//		qDebug() << "Cabinet IP:" << config.ip;
//		qDebug() << "Device Type:" << (config.deviceType == DeviceType::TemperatureChamber ? "Temperature Chamber" : "Vibration Table");
//		qDebug() << "Device Name:" << config.deviceName;
//		qDebug() << "MySQL Host:" << config.mysqlHost;
//		qDebug() << "MySQL Port:" << config.mysqlPort;
//		qDebug() << "MySQL User:" << config.mysqlUser;
//		qDebug() << "MySQL Password:" << config.mysqlPassword;
//		qDebug() << "MySQL Database:" << config.mysqlDatabase;
//		qDebug() << "------------------------";
//	}
//}