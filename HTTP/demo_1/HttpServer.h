#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_
#include<QObject>
#include<QHostAddress>
#include<QHash>
class QTcpServer;
class QTcpSocket;

/*http 请求*/
class HttpServerRequest
{
	friend class HttpServer;
	HttpServerRequest() {}
	/*此处构造函数作为私有的目的：这意味着除了该类自身以及它的友元之外，
	其他任何类都无法直接实例化 HttpServerRequest 类的对象，
	是为了限制对 HttpServerRequest 对象创建的控制，
	确保其只能通过 HttpServer 类的相关逻辑正确初始化。*/
public:
	enum class Method {
		Unknown = 0,
		Get		= 1 << 1,
		Post	= 1 << 2
	};
	Method method() { return m_method; }
	QByteArray path()const { return m_path; }
	QByteArray query()const { return m_query; }
	QByteArray value(const QByteArray& key)const {
		auto it = std::find_if(m_headers.cbegin(), m_headers.cend(), [=](QPair<QByteArray, QByteArray>header) {
			return header.first == key;
		});
		if (it != m_headers.end()) {
			return it->second;
		}
		return QByteArray();
	}
	QList<QPair<QByteArray, QByteArray>>headers()const { return m_headers; }
	QByteArray body()const { return m_body; }
private:
	Method m_method{ Method::Unknown };
	QByteArray m_path{};
	QByteArray m_query{};
	QList<QPair<QByteArray, QByteArray>>m_headers{};
	QByteArray m_body{};
};

class HttpServerResponse 
{
	friend class HttpServer;
public:
	enum class Status {
		Ok = 200,
		NotFound = 404
	};
	HttpServerResponse(Status status = Status::Ok) :m_status(status) {}
	void write(const QByteArray& data) {
		addHeader("Content-Length", QByteArray::number(data.size()));
		addHeader("Content-Type", "application/json");
		m_data = data;
	}
	void addHeader(const QByteArray& key, const QByteArray& value) {
		m_headers.append({ key,value });
	}
	void addHeaders(QList<QPair<QByteArray, QByteArray>>& headers) {
		for (auto it = headers.cbegin(); it != headers.cend(); ++it) {
			m_headers.append({ it->first, it->second });
		}
	}
	QByteArray data()const { return m_data; }
	Status status()const { return m_status; }
private:
	Status m_status{ Status::Ok };
	QByteArray m_data{};
	QList<QPair<QByteArray, QByteArray>>m_headers{};
};


class HttpServer : public QObject
{
	Q_OBJECT
public:
	HttpServer(QObject* parent = nullptr);
	bool listen(const QHostAddress& address, quint16 port);
	void router(const QByteArray& path, const std::function<QString(const HttpServerRequest&)>& func) {
		m_routers.insert(path, func);
	}
private:
	void onReadyRead();
protected:
	bool handleRequest(const HttpServerRequest& request,HttpServerResponse& resp);
	void missingHandler(const HttpServerRequest& request, HttpServerResponse& resp);
private:
	QTcpServer* m_server{};
	QHash<QByteArray, std::function<QString(const HttpServerRequest&)>>m_routers;
};

#endif // !HTTPSERVER_H_
