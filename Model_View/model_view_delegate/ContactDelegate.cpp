#include "ContactDelegate.h"
#include "Contact.h"
#include<QStyleOptionViewItem>
#include<QPainter>
#include<QStaticText>
#include<QPoint>
#include<QEvent>
#include<QMouseEvent>
ContactDelegate::ContactDelegate(QObject* parent)
	:QStyledItemDelegate(parent)
{

}

void ContactDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	auto c = index.data(Qt::ItemDataRole::UserRole).value<Contact*>();
	if (!c) {
		QStyledItemDelegate::paint(painter, option, index);
		return;
	}
	auto rect = option.rect;
	QColor color = Qt::white;
	//鼠标悬停item背景颜色
	if (option.state & QStyle::State_MouseOver)
	{
		color = QColor(242, 242, 242, 242);
	}
	if (option.state & QStyle::State_Selected)
	{
		color = QColor(235,235,235,235);
	}
	painter->fillRect(rect, color);
	//绘制头像
	QRect profileRect = { rect.x() + 14, rect.y() + 8, 40, 40 };
	painter->drawPixmap(profileRect, QPixmap(c->m_profilePath));
	//昵称
	QRect nrect = { profileRect.right() + 10,rect.y()+15,0,0};
	auto fm = painter->fontMetrics();
	nrect.setWidth(fm.horizontalAdvance(c->m_userName));
	nrect.setHeight(fm.height());
	painter->drawStaticText(nrect.topLeft(), QStaticText(c->m_userName));
	//用户名
	QRect urect = { nrect.right() + 1,nrect.y() ,0,0};
	urect.setWidth(fm.horizontalAdvance(c->m_nickName));
	urect.setHeight(fm.height());
	painter->drawStaticText(urect.topLeft(), QStaticText("("+c->m_nickName+")"));

	//type
	QPixmap typePixmap;
	if (c->type == Contact::VIP)
		typePixmap.load(":/Resource/images/vip.png");
	else if (c->type == Contact::SVIP)
		typePixmap.load(":/Resource/images/svip.png");
	m_typeRect = { urect.topRight() + QPoint(10, 5),typePixmap.size()};
	painter->drawPixmap(m_typeRect,typePixmap);

	//检测鼠标是否在type上面
	if (m_typeRect.contains(m_mousePos)){
		painter->save();
		painter->setPen(Qt::red);
		painter->drawRect(m_typeRect);
		painter->setPen(Qt::black);
		painter->restore();
	}

	//签名
	QPoint p = { nrect.x(),nrect.bottom()+2};
	painter->drawStaticText(p, QStaticText(c->m_signature));
}

QSize ContactDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QSize(option.rect.width(),60);
}

bool ContactDelegate::editorEvent(QEvent* ev, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
	if (ev->type() == QEvent::MouseMove)
	{
		auto mev = static_cast<QMouseEvent*>(ev);
		this->m_mousePos = mev->pos();
		return true;
	}
	else if (ev->type() == QEvent::MouseButtonPress)
	{
		auto mev = static_cast<QMouseEvent*>(ev);
		if (mev->button() == Qt::LeftButton && m_typeRect.contains(m_mousePos)) {
			qDebug() << "typeClicked";
			emit ContactDelegate::typeClicked();
		}
	}
	return false;
}
