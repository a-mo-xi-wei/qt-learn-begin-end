#include<QApplication>
#include<QWidget>
#include<QMouseEvent>

class CustomerEvvent : public QEvent
{
public:

};


class MWidget : public QWidget
{
public:
	MWidget(QWidget* parent = nullptr)
		:QWidget(parent)
	{

	}
	//所有的事件处理函数都是从event()事件派发函数调用的
	bool event(QEvent* ev)override
	{
		switch (ev->type())
		{
		case QEvent::MouseButtonPress:
			//mousePressEvent(dynamic_cast<QMouseEvent*>(ev));
			break;
		default:
			break;
		}

		return QWidget::event(ev);
	}
	void mousePressEvent(QMouseEvent* ev)override
	{
		qDebug() << ev->button();
	}
};

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	MWidget w;
	w.show();

	return a.exec();
}