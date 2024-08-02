#include<QCoreApplication>
#include<iostream>
#include<QString>
#include<QByteArray>
#include<QVariant>
void test_ByteArray() {
	QByteArray arr(10, 'a');
	qDebug() << arr;
	QByteArray name("Hello World");
	qDebug() << name;
	name.chop(6);
	qDebug() << name;
	qDebug() << name.back();
	qDebug() << *(name.end());
	//转整数
	QByteArray num("FF");
	qDebug() << num.toInt() << " " << num.toHex(0);
	QByteArray num2;
	num2.setNum(1234, 2);
	qDebug() << num2;
}
void test_QString_ByteArray() {
	QByteArray str1("Hello World"); 
	QString str2("Hello World");
	qDebug() << str1.size() << " " << str2.size();
	{
		QByteArray str1("你");	// 3 一个中文utf-8编码下，一个中文三个字节
		QString str2("你");		// 1 输出的是字符的个数
		qDebug() << str1.size() << " " << str2.size();

		//转成本地编码
		qDebug() << str1;
		qDebug() << str2.toLocal8Bit(); //gbk
	}
}
void test_QString_Format() {
	
	//C语言风格
	auto say = QString::asprintf("I'm %s ,%d years old...", "weisang", 18);
	qDebug() << say;
	
	//Qt风格
	auto str = QString("I'm %1 ,%2 years old ,%1 is a good name").arg("Jack").arg(25);
	qDebug() << str;

	//01:03:59
	auto time = QString("%1:%2:%3").arg(1, 2, 10, QChar('0')).arg(3, 2, 10, QChar('0')).arg(59, 2, 10, QChar('0'));
	qDebug() << time;
	QString log = "can i help you ? no thanks , please get out. ";
	qDebug() << log.indexOf("o");

}
struct MM
{
	QString name = "";
	int age = 0;
	inline friend QDebug operator<<(QDebug debug, const MM& m)
	{
		debug << "name : " << m.name << " age : " << m.age;
		return debug;
	}
};
//Qt5
//Q_DECLARE_METATYPE(MM);
void test_QVariant() {
	QVariant var(2);
	qDebug() << var;
	//拿到存储的值
	int n = var.toInt();
	qDebug() << n;
	//判断能够进行转换
	if (var.canConvert<double>())qDebug() << var.toDouble();
	//如何存储自定义的类型
	//Qt6是可以直接存储自定义类型的
	//Qt5必须先注册以下自定义的类型
	MM mm{ "mm", 18 };
	auto vmm = QVariant::fromValue(mm);
	qDebug() << vmm;
}
int main(int argc, char* argv[]) {
	QCoreApplication a(argc, argv);
	//test_ByteArray();
	//test_QString_ByteArray();
	//test_QString_Format();
	test_QVariant();
	return a.exec();
}
