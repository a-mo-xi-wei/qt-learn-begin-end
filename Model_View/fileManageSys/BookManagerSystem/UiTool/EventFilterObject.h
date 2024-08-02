#ifndef _EVENTFILETEROBJECT_H_
#define  _EVENTFILETEROBJECT_H_

#include<QObject>
#include<QPoint>

class EventFilterObject : public QObject
{
public:
	EventFilterObject(QObject*parent = nullptr);
protected:
	bool eventFilter(QObject* watch, QEvent* ev)override;

	QPoint m_pressPos;
};

#endif // !_EVENTFILETEROBJECT_H_
