#include "HttpServer.h"
#include<QTcpServer>
#include<QTcpSocket>
#include<QDataStream>
HttpServer::HttpServer(QObject* parent)
	:QObject(parent)
{
}

bool HttpServer::listen(const QHostAddress& address, quint16 port)
{
	if (!m_server)
	{
		m_server = new QTcpServer(this);
		connect(m_server, &QTcpServer::newConnection, [=]
			{
				while (m_server->hasPendingConnections()) {
					auto con = m_server->nextPendingConnection();
					if (con) {
						connect(con, &QTcpSocket::readyRead/*有数据可读*/, this, &HttpServer::onReadyRead);
						/*QTcpSocket只有在其缓冲区中有数据时才会触发readyRead信号。*/
					}
				}
			});
	}
	return m_server->listen(address, port);
}

static QByteArray readLine(QDataStream& stream) {
	QByteArray lineData;
	char ch;
	while (!stream.atEnd()) {
		stream.readRawData(&ch, 1);	/*从流中读取指定数量的原始字节数据，并将其存储在提供的缓冲区中*/
		if (ch == '\r') {
			stream.readRawData(&ch, 1);
			if (ch == '\n')break;
		}
		else lineData.append(ch);
	}
	return lineData;
}

void HttpServer::onReadyRead()
{
	auto con = dynamic_cast<QTcpSocket*>(sender());
	QDataStream stream(con);

	HttpServerRequest request;

	/*向服务器发送请求，服务器给出响应*/

	//服务器解析请求行
	auto requestLine = readLine(stream);
	auto rl_list = requestLine.split(' ');
	auto method = rl_list.first();
	auto uri = rl_list.at(1);
	qDebug() << "request line : " << requestLine;

	auto pos = uri.indexOf('?');
	request.m_path = pos != -1 ? uri.mid(0, pos) : uri;
	request.m_query = pos != -1 ? uri.mid(pos) : QByteArray();

	/*解析请求头header*/
	while (!stream.atEnd()) {
		auto line = readLine(stream);
		if (line.isEmpty()) {
			qDebug() << "header parse finished";
			break;
		}
		auto head = line.split(':');
		request.m_headers.append({ head.first(),head.last() });
	}
	//解析body
	auto len = request.value("Content-Length").toLongLong();
	if (len > 0) {
		request.m_body.resize(len + 1);
		request.m_body[len] = '\0';
		stream.readRawData(request.m_body.data(), len);
	}
	HttpServerResponse resp;
	if (!handleRequest(request, resp)) {
		missingHandler(request, resp);
	}

	//响应行
	QByteArray respLine = "HTTP/1.1 " + QByteArray::number((int)resp.status()) + " 没毛病\r\n";
	con->write(respLine);
	qDebug() << "响应行";
	//响应头
	QByteArray headers;
	for (auto it = resp.m_headers.cbegin(); it != resp.m_headers.cend(); ++it) {
		headers += it->first + ":" + it->second + "\r\n";
	}
	headers += "\r\n";
	con->write(headers);
	qDebug() << "响应头";

	//body 
	con->write(resp.m_data);
	qDebug() << "响应body";

	con->close();
}

bool HttpServer::handleRequest(const HttpServerRequest& request, HttpServerResponse& resp)
{
	for (auto it = m_routers.cbegin(); it != m_routers.cend(); ++it) {
		if (it.key() == request.path()) {
			resp.write(it.value()(request).toUtf8());
			return true;
		}

	}
	//resp.write(R"({"name":"weisang","age":18})");
	return false;
}

void HttpServer::missingHandler(const HttpServerRequest& request, HttpServerResponse& resp)
{
	resp.m_status = HttpServerResponse::Status::NotFound;
}

