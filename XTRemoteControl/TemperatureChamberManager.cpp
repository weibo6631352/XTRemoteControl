#include "TemperatureChamberManager.h"

TemperatureChamberManager::TemperatureChamberManager(QObject* parent)
	: QObject(parent)
{
}

TemperatureChamberManager& TemperatureChamberManager::instance() {
	static TemperatureChamberManager instance;
	return instance;
}

void TemperatureChamberManager::addTemperatureChamber(const QString& ip) {
	if (!m_temperatureChambers.contains(ip)) {
		TemperatureChamber* chamber = new TemperatureChamber(ip, this);
		m_temperatureChambers[ip] = chamber;
	}
}

void TemperatureChamberManager::removeTemperatureChamber(const QString& ip) {
	if (m_temperatureChambers.contains(ip)) {
		TemperatureChamber* chamber = m_temperatureChambers.take(ip);
		delete chamber;
	}
}

TemperatureChamber* TemperatureChamberManager::getTemperatureChamber(const QString& ip) {
	return m_temperatureChambers.value(ip, nullptr);
}