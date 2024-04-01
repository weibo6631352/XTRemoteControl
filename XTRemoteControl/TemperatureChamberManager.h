#ifndef TEMPERATURECHAMBERMANAGER_H
#define TEMPERATURECHAMBERMANAGER_H

#include <QObject>
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include "TemperatureChamber.h"

class TemperatureChamberManager : public QObject {
	Q_OBJECT

public:
	static TemperatureChamberManager& instance();

	void addTemperatureChamber(const QString& ip);
	void removeTemperatureChamber(const QString& ip);
	TemperatureChamber* getTemperatureChamber(const QString& ip);

private:
	TemperatureChamberManager(QObject* parent = nullptr);
	TemperatureChamberManager(const TemperatureChamberManager&) = delete;
	TemperatureChamberManager& operator=(const TemperatureChamberManager&) = delete;

	QMap<QString, TemperatureChamber*> m_temperatureChambers;
};

#endif // TEMPERATURECHAMBERMANAGER_H