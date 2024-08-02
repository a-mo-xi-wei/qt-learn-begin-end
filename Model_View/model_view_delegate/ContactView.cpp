#include "ContactView.h"
#include"ContactDelegate.h"

ContactView::ContactView(QWidget* parent)
	:QListView(parent),
	m_model(new QStandardItemModel(this))
{
	setFixedSize(340, 480);
	setModel(m_model);
	//给每一项都设置代理
	setItemDelegate(new ContactDelegate(this));
	//设置编辑策略
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	//设置鼠标追踪
	setMouseTracking(true);


	initUi(); 
	connect(this, &ContactView::clicked, this, &ContactView::onClicked);
}

void ContactView::onClicked(const QModelIndex& index)
{
	Contact* c = index.data(Qt::ItemDataRole::UserRole).value<Contact*>();
	if (!c) {
		qWarning() << "联系人信息获取失败";
		return;
	}
	qDebug() << c->m_userName << c->m_nickName << c->m_signature;
}

void ContactView::initUi()
{
	//准备联系人
	m_contacts.emplace_back(new Contact("十一期 益达","龟桑"));
	m_contacts.emplace_back(new Contact("十二期 农夫山泉","周桑"));
	m_contacts.emplace_back(new Contact("十九期 快递","吴桑"));
	m_contacts.emplace_back(new Contact("十三期 手机","魏桑"));
	m_contacts.emplace_back(new Contact("十三期 球拍","魏桑"));

	//for (size_t i = 0; i < 20000; i++)	//非常慢
	//{
	//	m_contacts.emplace_back(new Contact(QString("十三期 球拍%1").arg(i), "魏桑"));
	//}

	for (auto& val : m_contacts)
	{
		auto item = new QStandardItem(val->m_userName);
		item->setData(QVariant::fromValue(val.get()), Qt::ItemDataRole::UserRole);
		m_model->appendRow(item);
	}

}
