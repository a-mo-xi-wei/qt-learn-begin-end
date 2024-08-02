#include "Server.h"

Server::Server(QObject* parent)
	:QObject(parent)
{
	init();
}

void Server::init()
{
}

bool Server::start(const QHostAddress& address, quint16 port)
{
	if (!m_server) {
		m_server = new QWebSocketServer("weisang fileTransfer", QWebSocketServer::SslMode::NonSecureMode, this);
		connect(m_server, &QWebSocketServer::newConnection, this, &Server::onNewConnection);
	}
	return m_server->listen(address,port);
}

void Server::onNewConnection()
{
	while (m_server->hasPendingConnections()) {
		auto client = m_server->nextPendingConnection();
		connect(client, &QWebSocket::disconnected, this, &Server::onDisConnection);
		connect(client, &QWebSocket::binaryMessageReceived, this, &Server::onBinaryMessageReceived);
		connect(client, &QWebSocket::textMessageReceived, this, &Server::onTextMessageReceived);
		auto key = QString::number((quint64)client);
		m_clients.insert(key, client);
		qDebug() << "after new connections : " << m_clients.size();
	}
}

void Server::onDisConnection()
{
	auto client = dynamic_cast<QWebSocket*>(sender());
	if (client) {
		m_clients.remove(QString::number((quint64)client));
		qDebug() << "after dis connections : " << m_clients.size();
	}
}

void Server::onBinaryMessageReceived(const QByteArray& data)
{
	auto client = dynamic_cast<QWebSocket*>(sender());
	if (client) {
		for (auto c : m_clients) {
			//不要自己发给自己
			//if (c != client) {
				c->sendBinaryMessage(data);
			//}
		}
	}
}

void Server::onTextMessageReceived(const QString& data)
{
	auto client = dynamic_cast<QWebSocket*>(sender());
	if (client) {
		for (auto c : m_clients) {
			//不要自己发给自己
			//if (c != client) {
				c->sendTextMessage(data);
			//}
		}
	}
}
