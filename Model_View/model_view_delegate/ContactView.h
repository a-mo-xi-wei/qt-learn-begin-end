#ifndef CONTACTVIEW_H_
#define CONTACTVIEW_H_

#include<QListView>
#include<memory.h>
#include<QStandardItemModel>
#include"Contact.h"
class ContactView : public QListView 
{
public:
	ContactView(QWidget* parent = nullptr);
	void onClicked(const QModelIndex& index);
private:
	void initUi();
	std::vector<std::unique_ptr<Contact>>m_contacts;
	QStandardItemModel* m_model;
};

#endif // !CONTACTVIEW_H_
