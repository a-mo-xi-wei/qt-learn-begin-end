#include<QApplication>
#include<QWidget>
#include<QThread>
#include<QLabel>
#include<QRunnable>
#include<QPlainTextEdit>
#include<qrandom.h>
#include<QTextStream>
#include<QLayout>
#include<QMetaObject>
QString randText() {
	int length = QRandomGenerator::global()->bounded(50, 100);
	QString str;
	QTextStream stream(&str);
	for (size_t i = 0; i < length; i++)
	{
		if (i % 2 == 0) {
			stream << static_cast<char>(QRandomGenerator::global()->bounded(0, 26) + 'a');
		}
		else {
			stream << QRandomGenerator::global()->bounded(9);
		}
	}
	return str;
}

class Widget : public QWidget
{
	Q_OBJECT
public:
	Widget(QWidget* parent = nullptr)
		:QWidget(parent),
		m_edit(new QPlainTextEdit(this))
	{
		auto layout = new QHBoxLayout(this);
		layout->addWidget(m_edit);
	}
	~Widget() { qDebug() << __FUNCTION__; }
	
	void recvData() const {
		for (size_t i = 0; i < 100; i++)
		{
			QMetaObject::invokeMethod(m_edit, "insertPlainText", Q_ARG(QString, randText() + '\n'));
			QThread::msleep(1);
		}
	}
private:

	QPlainTextEdit* m_edit{};
};

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	Widget w;
	w.show();

	//把需要在线程中工作的的对象移到线程中
	QThread th;
	w.moveToThread(&th);//不能把widget移动到子线程中
	//关联需要在线程中调用的函数
	QObject::connect(&th, &QThread::started, &w, &Widget::recvData);
	th.start();
	return a.exec();
}
#include "main.moc"