#include<QApplication>
#include<QWidget>
int g_tel = 999;
class Person : public QObject
{
	Q_OBJECT
	//让成员变量暴露给属性
	Q_PROPERTY(int age MEMBER m_age NOTIFY ageChanged)
	Q_PROPERTY(QString name MEMBER m_name NOTIFY nameChanged)
	//直接定义属性
	Q_PROPERTY(quint64 tel READ getTel WRITE setTel RESET resetTel NOTIFY telChanged)
public:
	Person(){}

	int age()const { return m_age; }
	void setAge(int age) { m_age = age; } 
	QString name()const { return m_name; }
	void setName(QString name) { m_name = name; }
	quint64 getTel()const { return g_tel; }
	void setTel(quint64 tel) { 
		if (g_tel != tel)
		{
			g_tel = tel;
			emit(telChanged(tel));
		}
	}
	void resetTel() { setTel(-1); }
signals:
	void telChanged(quint64 tel);
	void ageChanged();
	void nameChanged();
private:
	int m_age = 0;
	QString m_name = "";
};

int main(int argc,char* argv[]){
	QApplication a(argc,argv);
	Person p;
	p.setAge(18);
	p.setName("zake");

	QObject::connect(&p, &Person::telChanged, [] {
		qDebug() << "tel Changed";
		});

	QObject::connect(&p, &Person::ageChanged, [] {
		qDebug() << "age Changed";
		});
	QObject::connect(&p, &Person::nameChanged, [] {
		qDebug() << "name Changed";
		});
	p.setProperty("tel", 99);
	p.setProperty("tel", QVariant());
	p.setAge(28);
	p.setProperty("name", "weisang");
	qDebug() << p.property("age") << p.property("name") << p.property("tel");

	qDebug() << "----------------------------------------";
	{
		//1.通过函数设置一些成员变量
		p.setObjectName("weisang");
		//2.通过属性来设置
		p.setProperty("objectName", "aaaaaa");
		qDebug() << p.objectName() << p.property("objectName");
		//3.如果设置类里面没有属性，那么则会添加临时的属性
		p.setProperty("_age", 18);
		p.setProperty("_name", "zake");
		qDebug() << p.property("_age") << p.property("_name");
	}
	

	return a.exec();	
}
#include "main.moc"