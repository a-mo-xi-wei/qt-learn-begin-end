#include<QCoreApplication>
#include<QHttpServer>
#include<QHostAddress>
#include<QHttpServerRequest>
#include<QHttpServerResponse>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>
#include<QJsonParseError>
#include<QJsonArray>
#include<QUrlQuery>
int main(int argc,char* argv[]){
	QCoreApplication a(argc,argv);
	QHttpServer server;

	//server.route("/version", [](const QHttpServerRequest& request) {
	//	return R"({"version":"1.0.1","name":"weisang server"})";
	//});
	//获取参数
	server.route("/version",QHttpServerRequest::Method::Get,[](const QHttpServerRequest& request) {
		return R"({"version":"1.0.1","name":"weisang server","method":"Get"})";
	});
	//登录
	server.route("/login", [] (const QHttpServerRequest& request){
		QJsonParseError err;
		auto jdoc = QJsonDocument::fromJson(request.body(), &err);
		if (err.error != QJsonParseError::NoError) {
			QJsonObject jobj;
			jobj.insert("code", "failed");
			jobj.insert("message", "参数错误");
			return QJsonDocument(jobj).toJson(QJsonDocument::Compact);
		}
		//jdoc["username"].toString();
		//jdoc["passwd"].toString();
		QJsonObject jobj;
		jobj.insert("code", "success");
		jobj.insert("message", "登陆成功~");
		return QJsonDocument(jobj).toJson(QJsonDocument::Compact);
	});

	//查询用户
	server.route("/user_list", [](const QHttpServerRequest& request) {

		auto uquery = request.query();
		if (uquery.hasQueryItem("is_enable")) {
			qDebug() << uquery.queryItemValue("is_enable");
		}
		QJsonArray jlists;
		for (size_t i = 0; i < 10; i++)
		{
			QJsonObject juser;
			juser.insert("userid", qint64(2000 + i));
			juser.insert("username", QString("weiwang_%1").arg(i));
			juser.insert("passwd",QString("abc_%1").arg(i));
			juser.insert("tel", QString("132456%1").arg(i));
			jlists.append(juser);
		}
		QJsonObject jdata;
		jdata.insert("page", 1);
		jdata.insert("pagesize", 10);
		jdata.insert("lists",jlists);

		QJsonObject jobj;
		jobj.insert("code", "success");
		jobj.insert("message", "登录成功~");
		jobj.insert("data", jdata);

		return QJsonDocument(jobj).toJson(QJsonDocument::Compact);
	});
	server.route("/xxxx/<arg>", [](const QString& year,const QHttpServerRequest& request) {
		qDebug() << year;
		return R"({"route":"xxxx/xxxx"})";
		});
	server.listen(QHostAddress::Any, 8888);

	return a.exec();	
}