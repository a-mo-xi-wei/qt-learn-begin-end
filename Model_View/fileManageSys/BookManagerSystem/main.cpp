#include<QApplication>
#include"Cool_Login_Interface/LoginRegisterDlg.h"

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	LoginRegisterDlg w;
	w.show();
	return a.exec();
}