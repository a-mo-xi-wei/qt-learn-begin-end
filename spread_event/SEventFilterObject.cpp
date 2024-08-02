#include"SEventFilterObject.h"
#include<QWidget>
#include<QEvent>
#include<QMouseEvent>

SEventFilterObject::SEventFilterObject(QObject* parent)
	:QObject(parent)
{

}

bool SEventFilterObject::eventFilter(QObject* watched, QEvent* event)
{
	QWidget* w = dynamic_cast<QWidget*>(watched);
	//如果窗口没有边框，我才能让他点击窗口客户区移动窗口
	if (w->windowFlags() & Qt::FramelessWindowHint)
	{
		QMouseEvent* ev = dynamic_cast<QMouseEvent*>(event);
		if (event->type() == QEvent::MouseButtonPress && ev->button() == Qt::MouseButton::LeftButton)
		{
			m_pos = ev->pos();
		}
		else if (event->type() == QEvent::MouseButtonRelease && ev->button() == Qt::MouseButton::LeftButton)
		{
			m_pos = { 0,0 };
		}
		else if (event->type() == QEvent::MouseMove && ev->buttons() & Qt::MouseButton::LeftButton)
		{
			w->move(ev->globalPos() - m_pos);
		}
	}
	return false;
}
