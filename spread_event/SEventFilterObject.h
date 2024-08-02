#ifndef SEVENTFILTEROBJECT_H_
#define  SEVENTFILTEROBJECT_H_
#include <QObject>
#include <QPoint>
class SEventFilterObject : public QObject
{
public:
	SEventFilterObject(QObject* parent = nullptr);
	bool eventFilter(QObject* watched, QEvent* event)override;
private:
	QPoint m_pos;
};

#endif // !SEVENTFILTEROBJECT_H_
