#include<QApplication>
#include<QFlags>

enum Direct
{
	Up = 1,
	Down = 1<<2,
	Left = 1<<3,
	Right = 1<<4,
	Center = 1<<5
};

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	int flages = Up | Left;
	if (flages & Up)qDebug() << "has Up";
	if (flages & Left)qDebug() << "has Left";
	return a.exec();
}
#include "main.moc"