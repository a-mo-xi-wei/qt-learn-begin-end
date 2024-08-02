#include<QApplication>
#include<QWidget>
#include<QThread>
#include<QLabel>
#include<QRunnable>

class Test : public QWidget
{
	Q_OBJECT
public:
	Test()
		:m_label(new QLabel("<h1>hello</h1>",this))
	{
		resize(640, 480);
		//test_create();
		test_create1();
	}
	void test_create() {
		//通过QThread::create创建线程
		QThread* th = QThread::create([=]() {
			//widget 必须被创建在主线程（GUI）
			//QWidget w;
			//w.show();
			int i = 0;
			while (i<1000) {
				qDebug() << "sub thread" << QThread::currentThread();
				//1.不能在子线程中操作ui相关的东西
				//m_label->setText(QString("<h1>%1</h1>").arg(i++));
				//2.信号和槽(没有问题，这也是最好的方式)
				emit textChanged(QString("<h1>%1</h1>").arg(i++));
				//3.使用invoke函数来执行setText函数
				//不做解释
			}
			});
		th->start();

		connect(th, &QThread::destroyed, [] {qDebug() << "destroyed!!!"; });
		connect(th, &QThread::finished, [=] {
			th->deleteLater();
			qDebug() << "th finfished"; });
		connect(this, &Test::textChanged, m_label, &QLabel::setText);
	}

	void test_create1() {
		QThread::create(&Test::worker, this)->start();
	}
	void worker() {
		while (true) {
			qDebug() << __FUNCTION__;
		}
	}
	//启动线程
signals:
	void textChanged(const QString& text);

private:
	QLabel* m_label;
};
/*其他方式：必须继承QThread类，实现run接口（把你需要在线程中处理的逻辑放到run中）*/

class Worker :public QThread
{
	Q_OBJECT
public:
	void run() override
	{
		quint64 sum = 0;
		for (size_t i = 0; i < 100; i++)
		{
			sum += i;
			QThread::msleep(1);
		}
		emit calcFinished(sum);
	}
signals:
	void calcFinished(quint64 sum);
};

int main(int argc,char* argv[]){
	QApplication a(argc,argv);
	//Test t;
	//t.show();
	//在主循环中千万不要使用死循环

	Worker w;
	//不能手动调用run函数，否则就不是在线程里面处理
	//w.run();
	//2.必须调用
	w.start();
	QObject::connect(&w, &Worker::calcFinished, [](quint64 s) {
		qDebug() << "sum = " << s;
		});

	qDebug() << "main thread : " << QThread::currentThread();
	return a.exec();	
}
#include "main.moc"