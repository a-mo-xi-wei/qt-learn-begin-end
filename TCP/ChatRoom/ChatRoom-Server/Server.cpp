#include "Server.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>

ChatRoomServer::ChatRoomServer(QObject* parent)
	:QObject(parent)
{
	init();
	m_server.listen(QHostAddress::AnyIPv4, 8888);
}

ChatRoomServer::~ChatRoomServer()
{
	m_server.close();
}

void ChatRoomServer::init()
{
	connect(&m_server, &QTcpServer::newConnection,this, &ChatRoomServer::onNewConnection);
}

void ChatRoomServer::onNewConnection()
{
	while (m_server.hasPendingConnections())
	{
		QTcpSocket* clientScoket =  m_server.nextPendingConnection();
		if (!clientScoket)
		{
			qWarning() << clientScoket->errorString();
			continue;
		}
		connect(clientScoket, &QTcpSocket::readyRead, this, &ChatRoomServer::onReadyRead);
		connect(clientScoket, &QTcpSocket::disconnected, this, &ChatRoomServer::onDisConnected);

	}
}

void ChatRoomServer::onReadyRead()
{
	auto socket = dynamic_cast<QTcpSocket*>(sender());


	QJsonParseError err;
	auto jdoc =  QJsonDocument::fromJson(socket->readAll(),&err);
	if (err.error != QJsonParseError::NoError)
	{
		QJsonObject obj;
		obj.insert("type", "error");
		obj.insert("message", "你发送的jsond数据貌似有点点，毛病" + err.errorString());
		socket->write(QJsonDocument(obj).toJson(QJsonDocument::Compact));
		return;
	}

	//登录的时候保存你的信息
	if (jdoc["type"] == "login")
	{
		User::ptr user(new User);
		user->id = jdoc["id"].toDouble();
		user->username = jdoc["username"].toString();

		qDebug() << "登录成功" << user->id << user->username;
		m_userHash.insert(socket, std::move(user));
	}
}

void ChatRoomServer::onDisConnected()
{

}
