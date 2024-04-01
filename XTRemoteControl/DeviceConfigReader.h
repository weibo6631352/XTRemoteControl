#pragma once
#include <QList>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QDir>
// 专用设备类型枚举
enum class DeviceType {
	TemperatureChamber,  // 温箱
	VibrationTable,      // 振动台
	// 可以在这里添加其他专用设备类型
};

// 测试机柜配置信息结构体
struct TestCabinetConfig {
	QString name;              // 机柜名称
	QString ip;                // 机柜IP地址
	DeviceType deviceType;     // 专用设备类型
	QString deviceName;        // 专用设备名称
	QString mysqlHost;         // MySQL主机地址
	int mysqlPort;             // MySQL端口号
	QString mysqlUser;         // MySQL用户名
	QString mysqlPassword;     // MySQL密码
	QString mysqlDatabase;     // MySQL数据库名称
	// 可以在这里添加其他配置信息,如通信协议、超时时间等
};

// 抽象配置读取器类
class DeviceConfigReader {
public:
	virtual ~DeviceConfigReader() {}
	// 纯虚函数,用于读取测试机柜配置信息
	virtual QList<TestCabinetConfig> readTestCabinetConfigs() = 0;
};

// JSON配置读取器类
class JsonDeviceConfigReader : public DeviceConfigReader {
public:
	// 实现从JSON文件读取测试机柜配置信息的功能
	QList<TestCabinetConfig> readTestCabinetConfigs() override {
		QList<TestCabinetConfig> configs;

		// 读取config目录下的所有JSON配置文件
		QDir configDir("config/CabinetConfig");
		QStringList jsonFiles = configDir.entryList(QStringList() << "cabinet_config_*.json", QDir::Files);
		for (const QString& fileName : jsonFiles) {
			QString filePath = configDir.filePath(fileName);
			// 读取JSON配置文件
			QFile configFile(filePath);
			if (!configFile.open(QIODevice::ReadOnly)) {
				qWarning() << "Failed to open config file:" << configFile.errorString();
				continue;
			}
			QByteArray configData = configFile.readAll();
			configFile.close();

			// 解析JSON配置
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
	// 获取设备配置管理器单例实例
	static DeviceConfigManager& instance() {
		static DeviceConfigManager instance;
		return instance;
	}

	// 设置配置读取器
	void setConfigReader(DeviceConfigReader* reader) {
		configReader = reader;
	}

	// 获取测试机柜配置信息
	QList<TestCabinetConfig> getTestCabinetConfigs() {
		if (testCabinetConfigs.isEmpty()) {
			if (configReader) {
				testCabinetConfigs = configReader->readTestCabinetConfigs();
			}
		}
		return testCabinetConfigs;
	}

	// 加载测试机柜配置信息
	void loadConfigs() {
		if (configReader) {
			testCabinetConfigs = configReader->readTestCabinetConfigs();
		}
	}

	// 根据ip获取测试机柜配置信息
	TestCabinetConfig getTestCabinetConfigByIp(const QString& ip) {
		for (const TestCabinetConfig& config : testCabinetConfigs) {
			if (config.ip == ip) {
				return config;
			}
		}
		return TestCabinetConfig();
	}

	// 根据设备名称获取测试机柜配置信息
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
//// 使用示例
//void useConfigExample() {
//	// 设置配置读取器为JSON配置读取器
//	DeviceConfigManager::instance().setConfigReader(new JsonDeviceConfigReader());
//	DeviceConfigManager::instance().loadConfigs();  // 程序加载时主动加载配置信息
//
//	// 获取测试机柜配置信息
//	QList<TestCabinetConfig> configs = DeviceConfigManager::instance().getTestCabinetConfigs();
//
//	// 使用配置信息进行相应的操作
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