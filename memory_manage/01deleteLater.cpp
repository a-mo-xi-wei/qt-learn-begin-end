#include<QApplication>
#include<QWidget>
#include<QPushButton>
#include<QRadioButton>
int main(int argc,char* argv[]) {
	QApplication a(argc,argv);
	QWidget w;
	w.resize(640, 480);
	//{
	//	QPushButton btn("touch me",&w);
	//	btn.show();
	//}
	//	auto btn = QPushButton("touch me", &w);
	{
		auto rbtn = new QRadioButton("男", &w);
		rbtn->setObjectName("man_rbtn");
		auto btn = new QPushButton("touch me", &w);
		btn->setObjectName("touch_btn");
		btn->move(150, 0);
		//btn->show();
		//btn->setParent(&w);
		//如果指定了父对象，则不需要显示执行show(),，而是随父对象show而show
		QObject::connect(btn, &QPushButton::destroyed, [] {
			qDebug() << "btn destoryed ";
			});
		//获取btn 的父对象
		
		auto parentw = dynamic_cast<QWidget*>(btn->parent());
		if (parentw)
		{
			qDebug() << parentw;
		}
		//或者直接
		//auto parentw = btn->parentWidget();
		
		//获取子对象
		const QObjectList& list = w.children();
		for (auto val : list) {
			qDebug() << val;
		}

		//查找指定类型的子对象
		qDebug() << "sub object" << w.findChild<QPushButton*>();
		qDebug() << w.findChild<QPushButton*>("touch_btn");
		qDebug() << w.findChild<QWidget*>("touch_btn");

	}
	//把所有子对象添加到窗口之后，再显示，即w.show()放在后面，
	//原因是由于之前没有显示指定子对象的show函数，导致窗口show过早，没来得及show子对象
	//可用子对象的show解决，但建议把w.show放后面省事

	w.show();

	auto b = new QWidget;

	QObject::connect(b, &QPushButton::destroyed, [] {
		qDebug() << "b destoryed...";
		});
	
	//1.new 完不 delete 会内存泄漏 （对于直接或间接继承QObject的类对象，
	// 不推荐使用delete来释放
	//2.使用QObject提供的安全释放的函数来释放对象（下一次运行到循环的时候，才去释放对象
	//delete b;
		
	b->deleteLater();	//此处不能使用，因为事件循环已经结束了，要将a.exec()放在此处后面
	int ret = a.exec();
	return ret;
}