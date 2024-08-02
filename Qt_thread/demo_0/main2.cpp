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

class Widget : public QWidget , public QThread
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
	void run()override {
		recvData();
		//qDebug() << exec();	//子线程的事件循环
		qDebug() << "over";
	}
	void recvData() const{
		for (size_t i = 0; i < 100; i++)
		{
			if (!isInterruptionRequested())break;//!isRunning
			//m_edit->insertPlainText(randText() + '\n');
			QMetaObject::invokeMethod(m_edit, "insertPlainText", Q_ARG(QString, randText()+'\n'));
			//QThread::msleep(1);
		}
	}
	void closeEvent(QCloseEvent* ev)override {
		//这两个函数是退出线程的事件循环（但是如果是死循环，就没用）
		this->exit(8);
		quit();

		//不一定强制杀死进程		不安全
		//if(!isFinished())
		//	QThread::terminate();

		isRunning = false;
		requestInterruption();//等同于isRunning的作用
		QThread::wait(1000);
	}
private:
	bool isRunning = true;
	QPlainTextEdit* m_edit{};
};

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	Widget w;
	QObject::connect(&w, &QThread::finished, [] {qDebug() << "finished"; });
	w.start();
	w.show();
	
	//QThread::create([&w] {
	//	w.recvData();
	//	})->start();
	//w.recvData(); 
	return a.exec();
}
#include "main.moc"