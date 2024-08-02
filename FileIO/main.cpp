#include<QCoreApplication>
#include<QFile>
#include<iostream>
#include<QString>
#include<QTextStream>
#include<QDataStream>
#include<QSettings>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QDir>
#include<QDirIterator>
void test_file() {
	{
		//打开文件，如果不存在则创建
		QFile file("file.txt");
		if (!file.open(QIODevice::WriteOnly)) {
			qDebug() << "open" << file.fileName() << "failed: " << file.errorString();
			return;
		}
		//写入数据
		file.write("Hello World\n");
		file.write("我是你爹");
	}
	{
		QFile file("file.txt");
		if (!file.open(QIODevice::ReadOnly)) {
			qDebug() << "open" << file.fileName() << "failed: " << file.errorString();
			return;
		}
		//读取数据
		auto val = file.read(30);
		qDebug() << QString::fromUtf8(val);
		
		//调整文件位置指针
		file.seek(0);

		//读取一行
		qDebug()<<file.readLine();

		//读取所有内容
		qDebug() << QString::fromUtf8(file.readAll()); 

	}

}

void test_stream() {
	QFile file("file.txt");
	if (file.open(QIODevice::ReadOnly))
	{
		//file.readAll();
		QTextStream stream(&file);	//把文件和文本流建立了关联，通过流去操作文件
		//qDebug()<<stream.readAll();
		QString str;
		stream >> str;
		qDebug() << str;
	}
	//文本流
	{
		QString str;
		QTextStream stream(&str);	//建立关联
		stream << "大家好" << "我是你爹";
		qDebug() << str;
	}
	//二进制流
	{
		//序列化数据
		int age = 12;
		QString name = "weisang";
		
		QByteArray datas;
		QDataStream stream(&datas, QIODevice::ReadWrite);
		stream << age << name;
		{
			QDataStream rstream(&datas, QIODevice::ReadOnly);
			int age1;
			QString name1;
			rstream >> age1 >> name1;
			qDebug() << age1 <<" "<< name1;
		}
	}

	QByteArray array;
	QDataStream stream(&array, QIODevice::WriteOnly);
	stream << "wokao";
	qDebug() << array;
}

void test_QSettings() {
	//读取配置
	QSettings set("dunkai", qApp->applicationName());
	auto home = set.value("home").toString();
	auto name = set.value("name").toString();
	qDebug() << home << " " << name;
	//写入配置(注册表)
	{
		QSettings set("dunkai", qApp->applicationName());
		set.setValue("home", "jiangxi");
		set.setValue("name", "weisang");
	}
	//写入ini文件
	{
		QSettings se("fileini", QSettings::Format::IniFormat);
		se.setValue("host", "192.168.56.128");
		se.setValue("port", 8080);

		// 设置组
		se.beginGroup("admin");
		se.setValue("user_name", "admin");
		se.setValue("passwd", "123456");
		se.endGroup();

		//设置数组
		se.beginWriteArray("ages");
		for (int i = 0;i < 5; i++)
		{
			se.setArrayIndex(i);
			se.setValue("age",18+i);
		}
		se.endArray();
	}
	//读取ini文件
	{
		QSettings se("fileini", QSettings::Format::IniFormat);
		qDebug() << se.value("host").toString();
		se.beginGroup("admin");
		qDebug() << se.value("passwd").toString();
		se.endGroup();

		//读取数组
		qsizetype size = se.beginReadArray("ages");
		for (size_t i = 0; i < size; i++)
		{
			se.setArrayIndex(i);
			qDebug()<<se.value("age").toInt();
		}
	}
}

QJsonDocument parseJson() {
	QFile file("./test.json");
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "Open Failed " << file.errorString();
		return QJsonDocument();
	}
	QJsonParseError err;
	QJsonDocument jdoc = QJsonDocument::fromJson(file.readAll(),&err);
	if (err.error != QJsonParseError::NoError) {
		qDebug() << "Json Parsed Failed" << err.errorString();
		return QJsonDocument();
	}
	return jdoc;
}

void test_QJsonDocument() {
	QJsonDocument jdoc = parseJson();

	//解析简单的对象
	{
		//	{"name": "weisang","age": 18}
		//qDebug() << jdoc["name"].toString() << " " << jdoc["age"].toInt();
	}
	//解析简单数组
	{
		QJsonArray arr = jdoc.array();
		for (size_t i = 0; i < arr.size(); i++)
		{
			QJsonValue val = jdoc[i];
			if (val.isDouble())// json 中只要是数字就是double
				qDebug() << val.toInt();
			else if (val.isString())
				qDebug() << val.toString();
			else if (val.isObject()) {
				QJsonObject obj = val.toObject();
				qDebug() << obj.value("one").toInt() << " " << obj.value("three").toInt();
			}
			else qDebug() << val;
		}
	}
}

void test_dir() {
	//创建目录对象
	QDir dir("../bin");
	if (!dir.exists()) {
		qDebug() << "目录不存在";
		return;
	}
	//获取目录项列表
	QStringList dir_entry = dir.entryList();
	for (auto& val : dir_entry) {
		qDebug() << val;
	}
	//获取项目FileInfo列表
	QFileInfoList info_list = dir.entryInfoList();
	for (auto val : info_list) {
		qDebug() <<"大小: "<<(qreal)val.size()/1024/1024<<"MB" << val.fileName() << val.absoluteFilePath();
	}
	//创建目录
	dir.mkdir("aa");
	if (dir.cd("aa")) {
		qDebug() << "切换目录";
	}
	dir.mkpath("bb/cc");
	if (dir.rmpath("bb/cc")) {
		qDebug() << "删除路径";
	}
	qDebug() << QDir::drives();
}

void test_diriterator() {
	QDirIterator it("E:", {"*.mp3","*.mp4"},QDir::Filter::Files , QDirIterator::IteratorFlag::Subdirectories);
	while (it.hasNext()) {
		QString str = it.next();
		//if (str.endsWith(".mp4"))
			qDebug() << str;
	}
}



int main(int argc, char* argv[]) {
	QCoreApplication a(argc,argv);

	//test_file();
	
	//test_stream();
	
	//test_QSettings();
	
	//test_QJsonDocument();
	
	//test_dir();

	test_diriterator();

	return 0;
}