#ifndef CONTACTDELEGATE
#define CONTACTDELEGATE

#include<QStyledItemDelegate>
class QPoint;
class ContactDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ContactDelegate(QObject* parent = nullptr);
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;
signals:
	void typeClicked();
private:
	QPoint m_mousePos;
	mutable QRect m_typeRect;
};








#endif // !CONTACTDELEGATE
