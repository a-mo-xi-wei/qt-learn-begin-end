#include<QApplication>
#include<QWidget>
#include<QPushButton>
#include<QMouseEvent>
#include"SEventFilterObject.h"


class SWidget : public QWidget
{
public:
	SWidget(QWidget* parent = nullptr)
		:QWidget(parent)
	{
		//去掉窗口的边框
		setWindowFlag(Qt::FramelessWindowHint);

		//设置鼠标追踪
		setMouseTracking(true);

		//安装事件过滤器,不会获得事件过滤对象的所有权，
		// 因为一个事件过滤对象可以安装给很多的对象
		installEventFilter(new SEventFilterObject(this));

		//取消安装事件过滤器
		//installEventFilter(nullptr);
	}

};


class Widget : public QWidget
{
public:
	Widget(QWidget* parent = nullptr)
		:QWidget(parent)
	{
		//去掉窗口的边框
		setWindowFlag(Qt::FramelessWindowHint);

		//设置鼠标追踪
		setMouseTracking(true);

		//安装事件过滤器,不会获得事件过滤对象的所有权，
		// 因为一个事件过滤对象可以安装给很多的对象
		installEventFilter(new SEventFilterObject(this));

		//取消安装事件过滤器
		//installEventFilter(nullptr);
	}
};


int main(int argc, char* argv[])
{
	QApplication a(argc, argv);


	Widget w;
	w.show();

	SWidget sw;
	sw.show();

	return a.exec();
}
