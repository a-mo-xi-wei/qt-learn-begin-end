#include<QApplication>
#include<QThread>

class Worker : public QObject
{
public:
	Worker(QObject* parent = nullptr)
		:QObject(parent)
	{

	}

	void randNumber()
	{
		for (size_t i = 0; i < 1000; i++)
		{
			m_number = i;
			qDebug() << i<<QThread::currentThread();
			//QThread::msleep(500);
		}
	}

	void hello()
	{
		int it = 0;
		while (it++ < 10)
		{
			qDebug() << "hello Thread" << QThread::currentThread();
			QThread::msleep(500);
		}

	}
	quint64 number()const { return m_number; }
private:
	quint64 m_number{};
};

class Controller : public QObject
{
	Q_OBJECT
public:
	Controller(QObject* parent = nullptr)
		:QObject(parent)
	{

		//ps:工作对象一定不能指定父对象，否则无法移动到子线程中
		//Worker* worker = new Worker(this);
		Worker* worker = new Worker;
		//worker->randNumber();

		QThread* thr = new QThread;

		//1,先把worker移动到子线程
		worker->moveToThread(thr);
		//2,当某个信号触发时，在子线程中调用worker的某个函数
		//connect(thr, &QThread::started, worker, &Worker::randNumber);
		//connect(this, &Controller::ffkk, worker, &Worker::randNumber);
		//下面这种lambda的方式，会在主线程中调用
		//如果要使用lambda表达式，那么必须是工作对象来调用lambda表达式
		connect(this, &Controller::ffkk,worker,[=]
			{
				worker->randNumber();
				thr->quit();
			});
		//connect(this, &Controller::playHlelo, worker, &Worker::hello);
		//启动线程
		thr->start();


		connect(thr, &QThread::finished, [=] 
			{
				worker->deleteLater();
				thr->deleteLater();
				qDebug() << "finished";
			});
		connect(thr, &QThread::destroyed, [] {qDebug() << "destroyed"; });



	}
signals:
	void ffkk();
	void playHlelo();
};

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	Controller c;
	emit c.ffkk();
	emit c.playHlelo();

	qDebug() << "main thread" << QThread::currentThread();

	return a.exec();
}

#include "main3.moc"
