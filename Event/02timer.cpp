#include<QApplication>
#include<QWidget>
#include<QPushButton>
#include<QTimer>
class SWidget : public QWidget
{
	Q_OBJECT
public:
	SWidget(QWidget* parent = nullptr)
		:QWidget(parent)
	{
		//两种定时器，周期性地处理
		//1.QTimer
		auto timer = new QTimer(this);
		timer->start(500); 
		//timer->callOnTimeout([] {
		//	qDebug() << "get data";
		//	});
		
		timer->callOnTimeout(this, &SWidget::game_update);

		QTimer::singleShot(1000, [] {qDebug() << "once"; });


		//2.定时器处理
		//开启定时器
		timer_id1 = startTimer(500);
		timer_id2 = startTimer(1000);

	}
	void timerEvent(QTimerEvent* ev)override
	{
		static int i = 0;
		if (i >= 5)killTimer(ev->timerId());
		if (ev->timerId() == timer_id1)
		{
			qDebug() << ev->timerId() << __FUNCTION__;
		}
		else if (ev->timerId() == timer_id2)
		{
			qDebug() << ev->timerId() << __FUNCTION__;
		}
		i++;
	}
	void game_update() { qDebug() << __FUNCTION__; }
	
	int timer_id1;
	int timer_id2;
};

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	SWidget w;
	w.show();
	return a.exec();
}
#include "main.moc"