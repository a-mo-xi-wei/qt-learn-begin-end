#ifndef CONTACTITEM_H_
#define CONTACTITEM_H_

#include"Contact.h"
#include<QWidget>
#include<QStandardItem>
class QLabel;
class QPushButton;
class ContactItem : public QWidget,public QStandardItem
{
public:
	ContactItem(Contact* contact,QWidget* parent = nullptr);

private:
	void initUi();
	void updateContactDisplay();
	QLabel* m_profilePathLab{};
	QLabel* m_nickNameLab{};
	QLabel* m_userNameLab{};
	QLabel* m_typeLab{};
	QLabel* m_signatureLab{};
	Contact* m_contact{};
};


#endif // !CONTACTITEM_H_
