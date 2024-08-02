#include "EventFilterObject.h"
#include <QMouseEvent>
#include <QWidget>
#include <QGraphicsDropShadowEffect>

EventFilterObject::EventFilterObject(QObject* parent)
	:QObject(parent)
{
	auto w = dynamic_cast<QWidget*>(parent);
	if (w)
	{
		//给窗口加上阴影特效
		auto effect = new QGraphicsDropShadowEffect(this);
		effect->setOffset(0);
		effect->setBlurRadius(10);
		effect->setColor(qRgb(60,60,60));
		w->setGraphicsEffect(effect);
	}
}

bool EventFilterObject::eventFilter(QObject* watch, QEvent* ev)
{
	QMouseEvent* mev = dynamic_cast<QMouseEvent*>(ev);
	if (ev->type() == QEvent::MouseButtonPress)
	{
		m_pressPos = mev->pos();
	}
	else if (ev->type() == QEvent::MouseMove && mev->buttons() & Qt::MouseButton::LeftButton)
	{
		QWidget* w = dynamic_cast<QWidget*>(watch);
		if (w)
			w->move(mev->globalPos() - m_pressPos);
		return true;
	}
	return false;
}
