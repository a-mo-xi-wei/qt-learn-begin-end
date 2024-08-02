#ifndef SERVER_H_
#define SERVER_H_

#include<QObject>
#include<QWebSocket>
#include<QWebSocketServer>
#include<QHash>
class Server : public QObject
{
	Q_OBJECT
public:
	Server(QObject* parent = nullptr);
	void init();
	bool start(const QHostAddress& address,quint16 port);
protected: //slots
	void onNewConnection();
	void onDisConnection();
	void onBinaryMessageReceived(const QByteArray& data);
	void onTextMessageReceived(const QString& data);

private:
	QWebSocketServer* m_server{};
	QHash<QString, QWebSocket*>m_clients{};
};

#endif // !SERVER_H_
