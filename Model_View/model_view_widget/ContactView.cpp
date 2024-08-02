#include "ContactView.h"

ContactView::ContactView(QWidget* parent)
	:QListView(parent),
	m_model(new QStandardItemModel(this))
{
	setModel(m_model);
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

	//for (size_t i = 0; i < 20000; i++)
	//{
	//	m_contacts.emplace_back(new Contact(QString("十三期 球拍%1").arg(i), "魏桑"));
	//}

	for (auto& val : m_contacts)
	{
		auto itemWidget = new ContactItem(val.get());

		m_model->appendRow(itemWidget);

		this->setIndexWidget(itemWidget->index(), itemWidget);
	}

}
