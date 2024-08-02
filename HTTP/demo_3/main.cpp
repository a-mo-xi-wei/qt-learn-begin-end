#include<QCoreApplication>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>
#include<QJsonParseError>
#include<QJsonArray>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QUrlQuery>
//#include<iostream>
class Client : public QObject
{
public:
	Client(QObject* parent = nullptr)
		:QObject(parent) 
	{
		test1();
		test2();
		test3();
	}
	void test1() {
		QNetworkRequest request(QUrl("http://localhost:8888/login"));
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
		QNetworkReply* reply = manager()->post(request, QJsonDocument({ {"username","weisang"},{"passwd","123456"}}).toJson(QJsonDocument::Compact));
		connect(reply, &QNetworkReply::errorOccurred, [=](QNetworkReply::NetworkError err) {
			qDebug() << "errorOccurred " << err << " " << reply->errorString();
		});
		connect(reply, &QNetworkReply::finished, [=] {
			//判断有没有问题
			if (reply->error() == QNetworkReply::NoError) {
				QJsonParseError err;
				auto jdoc = QJsonDocument::fromJson(reply->readAll(), &err);
				if (err.error == QJsonParseError::NoError) {
					qDebug() << jdoc["code"].toString() << jdoc["message"].toString();
				}
			}

			reply->deleteLater();	
			});
	}
	void test2() {

		QUrlQuery uquery;
		uquery.addQueryItem("is_enable", "false");
		QUrl url("http://localhost:8888/user_list");
		url.setQuery(uquery);
		QNetworkRequest request(url);
		//request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

		QNetworkReply* reply = manager()->get(request);
		connect(reply, &QNetworkReply::errorOccurred, [=](QNetworkReply::NetworkError err) {
			qDebug() << "errorOccurred " << err << " " << reply->errorString();
			});
		connect(reply, &QNetworkReply::finished, [=] {
			//判断有没有问题
			if (reply->error() == QNetworkReply::NoError) {
				QJsonParseError err;
				auto jdoc = QJsonDocument::fromJson(reply->readAll(), &err);
				if (err.error == QJsonParseError::NoError) {
					qDebug() << jdoc["code"].toString() << jdoc["message"].toString();
					auto jarray = jdoc["data"].toObject().value("lists").toArray();
					for (size_t i = 0; i < jarray.size(); i++)
					{
						auto juser = jarray[i].toObject();
						qDebug() << juser.value("userid").toDouble()
							<< juser.value("username").toString()
							<< juser.value("passwd").toString()
							<< juser.value("tel").toString();
					}
				}
			}

			reply->deleteLater();
			});
	}
	void test3() {

		QNetworkRequest request(QUrl("https://api.seniverse.com/v3/weather/now.json?key=SvAISfOS9wnvZelpS&location=nanchang&language=zh-Hans&unit=c"));
		//request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

		QNetworkReply* reply = manager()->get(request);
		connect(reply, &QNetworkReply::errorOccurred, [=](QNetworkReply::NetworkError err) {
			qDebug() << "errorOccurred " << err << " " << reply->errorString();
			});
		connect(reply, &QNetworkReply::finished, [=] {
			//判断有没有问题
			if (reply->error() == QNetworkReply::NoError) {
				qDebug() << QString::fromUtf8(reply->readAll());
			}

			reply->deleteLater();
			});
	}
	static QNetworkAccessManager* manager() {
		static QNetworkAccessManager* ins = nullptr;
		if (!ins) {
			ins = new QNetworkAccessManager;
		}
		return ins;
	}
};

int main(int argc,char* argv[]){
	QCoreApplication a(argc,argv);
	Client client;
	//std::cout << "你好";
	return a.exec();	
}