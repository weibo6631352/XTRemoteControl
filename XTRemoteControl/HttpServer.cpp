#include "HttpServer.h"
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "TemperatureChamberManager.h"

HttpServer::HttpServer(quint16 port, QObject* parent)
	: QObject(parent)
{
	if (m_server.listen(QHostAddress::Any, port)) {
		qDebug() << "HTTP server started on port" << port;
		connect(&m_server, &QTcpServer::newConnection, this, &HttpServer::onNewConnection);
	}
	else {
		qDebug() << "Failed to start HTTP server on port" << port;
	}
}

HttpServer::~HttpServer()
{
	m_server.close();
}

void HttpServer::setTemperature(const QString& ip, double temperature)
{
	QJsonObject data;
	data["temperature"] = temperature;
	sendPostRequest(ip, "/temperature-chamber/temperature", data);
}

void HttpServer::setHumidity(const QString& ip, double humidity)
{
	QJsonObject data;
	data["humidity"] = humidity;
	sendPostRequest(ip, "/temperature-chamber/humidity", data);
}

void HttpServer::startTemperatureChamber(const QString& ip)
{
	sendPostRequest(ip, "/temperature-chamber/start");
}

void HttpServer::stopTemperatureChamber(const QString& ip)
{
	sendPostRequest(ip, "/temperature-chamber/stop");
}

void HttpServer::getTemperatureChamberStatus(const QString& ip)
{
	sendPostRequest(ip, "/temperature-chamber/status");
}

void HttpServer::getTemperatureChamberError(const QString& ip)
{
	sendPostRequest(ip, "/temperature-chamber/error");
}

void HttpServer::onNewConnection()
{
	QTcpSocket* socket = m_server.nextPendingConnection();
	QString ip = socket->peerAddress().toString();
	m_ipSocketMap[ip] = socket;
	connect(socket, &QTcpSocket::disconnected, this, &HttpServer::onSocketDisconnected);
}

void HttpServer::onSocketDisconnected()
{
	QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
	if (socket) {
		QString ip = m_ipSocketMap.key(socket);
		m_ipSocketMap.remove(ip);
		socket->deleteLater();
	}
}

void HttpServer::sendPostRequest(const QString& ip, const QString& path, const QJsonObject& data)
{
	QNetworkAccessManager* manager = new QNetworkAccessManager(this);
	QNetworkRequest request;
	request.setUrl(QUrl(QString("http://%1:8080%2").arg(ip).arg(path)));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	QJsonDocument jsonDoc(data);
	QByteArray requestData = jsonDoc.toJson();

	QNetworkReply* reply = manager->post(request, requestData);
	connect(reply, &QNetworkReply::finished, this, [=]() {
		handleResponse(reply);
		manager->deleteLater();
		});
}

void HttpServer::handleResponse(QNetworkReply* reply)
{
	if (reply->error() == QNetworkReply::NoError) {
		// 判断响应的URL是否为温箱设备
		if (reply->url().path().startsWith("/temperature-chamber")) {
			TemperatureChamber* temperatureChamber = TemperatureChamberManager::instance().getTemperatureChamber(reply->url().host());  // 获取温箱设备
			// 判断响应URL的命令类型
			if (reply->url().path() == "/temperature-chamber/status") {
				// 读取响应数据
				QByteArray responseData = reply->readAll();
				QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
				if (jsonDoc.isObject()) {
					QJsonObject responseObj = jsonDoc.object();
					if (responseObj.contains("temperature")) {
						double temperature = responseObj["temperature"].toDouble();
						temperatureChamber->setTemperature(temperature);
					}
					if (responseObj.contains("humidity")) {
						double humidity = responseObj["humidity"].toDouble();
						temperatureChamber->setHumidity(humidity);
					}
					if (responseObj.contains("running_status")) {
						QString runningStatus = responseObj["running_status"].toString();
						temperatureChamber->setRunningStatus(runningStatus);
					}
					if (responseObj.contains("error_code")) {
						QString errorCode = responseObj["error_code"].toString();
						temperatureChamber->setErrorCode(errorCode);
					}
				}
			}
			else if (reply->url().path() == "/temperature-chamber/error") {
				// 读取响应数据
				QByteArray responseData = reply->readAll();
				QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
				if (jsonDoc.isObject()) {
					QJsonObject responseObj = jsonDoc.object();
					if (responseObj.contains("error_code")) {
						QString errorCode = responseObj["error_code"].toString();
						temperatureChamber->setErrorCode(errorCode);
					}
				}
			}
			else if (reply->url().path() == "/temperature-chamber/start") {

				temperatureChamber->start();
			}
			else if (reply->url().path() == "/temperature-chamber/stop") {
				temperatureChamber->stop();
			}
		}		
	}
	else {
		qDebug() << "Request failed:" << reply->errorString();
	}
	reply->deleteLater();
}