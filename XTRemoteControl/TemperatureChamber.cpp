#include "TemperatureChamber.h"

TemperatureChamber::TemperatureChamber(const QString& ip, QObject* parent)
	: QObject(parent)
	, m_ip(ip)
	, m_temperature(0.0)
	, m_humidity(0.0)
	, m_runningStatus("Idle")
	, m_errorCode("")
	, m_connected(false)
{
}

QString TemperatureChamber::ip() const {
	return m_ip;
}

bool TemperatureChamber::isConnected() const{
	return m_connected;
}

double TemperatureChamber::temperature() const {
	return m_temperature;
}

double TemperatureChamber::humidity() const {
	return m_humidity;
}

QString TemperatureChamber::runningStatus() const {
	return m_runningStatus;
}

QString TemperatureChamber::errorCode() const {
	return m_errorCode;
}

void TemperatureChamber::setTemperature(double temperature) {
	if (m_temperature != temperature) {
		m_temperature = temperature;
		emit statusChanged();
	}
}

void TemperatureChamber::setHumidity(double humidity) {
	if (m_humidity != humidity) {
		m_humidity = humidity;
		emit statusChanged();
	}
}

void TemperatureChamber::start() {
	if (m_runningStatus != "Running") {
		m_runningStatus = "Running";
		emit statusChanged();
	}
}

void TemperatureChamber::stop() {
	if (m_runningStatus != "Idle") {
		m_runningStatus = "Idle";
		emit statusChanged();
	}
}

void TemperatureChamber::setRunningStatus(const QString& status) {
	if (m_runningStatus != status) {
		m_runningStatus = status;
		emit statusChanged();
	}
}

void TemperatureChamber::setErrorCode(const QString& errorCode) {
	if (m_errorCode != errorCode) {
		m_errorCode = errorCode;
		emit statusChanged();
	}
}