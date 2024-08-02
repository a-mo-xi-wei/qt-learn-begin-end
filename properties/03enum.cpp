#include<QApplication>
#include<QWidget>
#include<QMetaEnum>
class Test
{
public:
	Test() {
		QWidget w;
		qDebug() << w.inherits("QObject");
		auto metaObject = w.metaObject();
		qDebug() << metaObject->className() ;
	}
};
//1.在命名空间中使用注册枚举
namespace func {
	Q_NAMESPACE
	enum Type {
		Player,
		Enemy,
		Bullet
	};
	Q_ENUM_NS(Type) //把枚举类型注册到元对象系统
}

//在类中注册枚举
class Person : public QObject
{
	Q_OBJECT
public:
	enum Identity
	{
		Student ,
		Doctor,
		Teacher
	};
	Q_ENUM(Identity);
};
int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	Test t;
	using namespace func;
	Type type = Player;
	qDebug() << type;
	Person::Identity id = Person::Identity::Doctor;
	qDebug() << id;

	//获取枚举类型的信息
	QMetaEnum me = QMetaEnum::fromType<Person::Identity>();
	qDebug() << me.name() << me.keyCount();
	qDebug() << me.keyToValue("Teacher");
	qDebug() << me.valueToKey(1);
	return a.exec();
}
#include "main.moc"