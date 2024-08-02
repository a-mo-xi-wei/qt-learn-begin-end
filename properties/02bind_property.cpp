#include<QApplication>
#include<QProperty>
//1. 在QObject子类中使用绑定属性
class Circle : public QObject
{
	Q_OBJECT
public:
	//int radius()const { return m_radius; }
	//qreal area()const { return M_PI * m_radius * m_radius; }
	//int m_radius = 0;
	Circle() {
		//添加绑定
		area.setBinding([this] {
			return M_PI * radius * radius;
			});
	}
	QProperty<int>radius;
	QProperty<qreal>area;
};
//2.如果类没有继承自QObject
class Rect {
public:
	Rect() {
		area.setBinding([this] {
			return w * h;
			});
	}
	QProperty<int>w;
	QProperty<int>h;
	QProperty<int>area;
};

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	Circle c;
	//c.m_radius = 1;
	//qDebug() << c.area();
	c.radius = 2;
	qDebug() << c.area;
	Rect re;
	re.w = 2;
	re.h = 3;
	qDebug() << re.area;

	return a.exec();
}
#include "main.moc"