#include "ContactItem.h"
#include<QLabel>
#include<QPushButton>
#include<QLayout>
ContactItem::ContactItem(Contact* contact,QWidget* parent)
	:m_contact(contact),
	QWidget(parent)
{
	this->setData(QVariant::fromValue(m_contact), Qt::ItemDataRole::UserRole);
	initUi();
	updateContactDisplay();
}


void ContactItem::initUi()
{
	setSizeHint(QSize(width(), 60));	//设置每一项的高度

	this->m_profilePathLab = new QLabel;
	this->m_profilePathLab->setFixedSize(40, 40);
	this->m_profilePathLab->setScaledContents(true);
	this->m_nickNameLab = new QLabel;
	this->m_userNameLab = new QLabel;
	this->m_typeLab = new QLabel;
	this->m_signatureLab = new QLabel;

	auto hlayout = new QHBoxLayout;
	hlayout->addWidget(m_userNameLab);
	hlayout->addWidget(m_nickNameLab);
	hlayout->addWidget(m_typeLab);
	hlayout->addStretch();
	auto glayout = new QGridLayout(this);
	glayout->addWidget(m_profilePathLab,0,0,2,1);
	glayout->addLayout(hlayout,0,1);
	glayout->addWidget(m_signatureLab,1,1);
}

void ContactItem::updateContactDisplay()
{
	if (!m_contact)return;
	m_profilePathLab->setPixmap(QPixmap(m_contact->m_profilePath));
	m_nickNameLab->setText(m_contact->m_nickName);
	m_userNameLab->setText(m_contact->m_userName);
	switch (m_contact->type)
	{
	case Contact::NONE:
		break;
	case Contact::VIP:
		m_typeLab->setPixmap(QPixmap(":/Resource/images/vip.png"));
		break;
	case Contact::SVIP:
		m_typeLab->setPixmap(QPixmap(":/Resource/images/svip.png"));
		break;
	
	default:
		break;
	}
	m_signatureLab->setText(m_contact->m_signature);
}
