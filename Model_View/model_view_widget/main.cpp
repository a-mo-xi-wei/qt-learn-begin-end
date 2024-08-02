#include<QApplication>
#include<QWidget>
#include"ContactItem.h"
#include"ContactView.h"
int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	//ContactItem w(new Contact("weisang","威桑"));
	//w.show();
	ContactView v;
	v.show();
	return a.exec();
}