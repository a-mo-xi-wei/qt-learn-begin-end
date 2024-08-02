#include<QApplication>
#include<QWidget>
#include<QMouseEvent>
#include<QPushButton>
#include<QMessageBox>

class Button : public QPushButton
{
public:
	Button(QWidget* parent = nullptr)
		:QPushButton(parent)
	{

	}

protected:
	//重写了父类的虚函数，就不再使用父类的实现，而是自定义
	//按钮点击信号，是在mousePressEvent函数里面触发的
	void mousePressEvent(QMouseEvent* ev)override
	{
		if (ev->button() == Qt::LeftButton)
		{
			qDebug() << "Pressed";

		}
		//若是不想将父类的其他东西丢掉
		QPushButton::mousePressEvent(ev);
	}
	
	
};
class Widget : public QWidget
{
	Q_OBJECT
public:
	QPushButton* m_btn = nullptr;
	Widget(QWidget* parent = nullptr)
		:QWidget(parent),
		m_btn(new QPushButton("X",this))
	{
		this->m_btn->setFixedSize(30, 30);
		this->resize(640, 480);
		this->setWindowTitle("Test");
		auto btn = new Button(this);	
		btn->setText("hello");
		connect(btn, &QPushButton::clicked, [] {
			qDebug() << "clicked";
			});
		//setMouseTracking(true);//设置鼠标追踪
		for (size_t i = 0; i < 70; i++)
		{
			qDebug() << QKeySequence::StandardKey(i)<<" : "
				<<QKeySequence::keyBindings(QKeySequence::StandardKey(i));
		}
	}
	//处理鼠标点击事件
	//重写虚函数即可
	//鼠标点击处理函数
	void mousePressEvent(QMouseEvent* ev)
	{
		//判断一下是哪个键按下
		if (ev->button() == Qt::MouseButton::LeftButton)
		{
			qDebug() << "Left Button Pressed"<<ev->pos()<<ev->globalPos();
			isPress = true;
			setWindowTitle("volkao");
		}
	}
	void mouseReleaseEvent(QMouseEvent* ev)override
	{
		if (ev->button() == Qt::LeftButton)
		{
			isPress = false;
			qDebug() << "release";
		}
	}
	void mouseMoveEvent(QMouseEvent* ev)override
	{
		//如果鼠标左键按下，并且移动了鼠标
		if (isPress)
		{
			qDebug() << "Left Butto Press and Move";
		}
		if(ev ->buttons() & Qt::RightButton)//这样就用不到isPress了
			qDebug() << "Right Butto Press and Move";

		qDebug() << "move";
	}
	void mouseDoubleClickEvent(QMouseEvent* ev)override
	{
		qDebug() << "Double clicked";
	}
	//滚轮滚动事件
	void wheelEvent(QWheelEvent* ev)override
	{
		//判断滚轮的方向
		qDebug() << ev->angleDelta().y() << ev->angleDelta().x();
	}

	//按键事件
	void keyPressEvent(QKeyEvent* ev)override
	{
		//按下了什么键
		qDebug() << Qt::Key(ev->key());

		//描述键 ctrl shift alt ...
		if (ev->modifiers() & Qt::KeyboardModifier::ControlModifier &&
			ev->key() == Qt::Key::Key_A)
		{
			qDebug() << "全选"; 
		}
		if (ev->matches(QKeySequence::StandardKey::Save))
		{
			qDebug() << "保存";
		}
	}
	void keyReleaseEvent(QKeyEvent* ev)override
	{

	}
	//窗口关闭事件处理
	void closeEvent(QCloseEvent* ev)override
	{
		auto ret = QMessageBox::question(this, "关闭窗口", "有未保存的文件，是否保存并退出？");
		
		qDebug()<<ret;
		if (ret == QMessageBox::StandardButton::Yes)
		{
			//保存并退出
			ev->accept();
		}
		else
		{
			//忽略事件
			ev->ignore();
		}
	}
	//当窗口大小改变的时候，会调用
	void resizeEvent(QResizeEvent* ev)override
	{
		this->m_btn->move(ev->size().width() - this->m_btn->size().width(), 0);
	}

	void changeEvent(QEvent* ev)override
	{
		switch (ev->type())
		{
		case QEvent::Type::WindowTitleChange:
			qDebug() << "windowTitleChange" << this->windowTitle();
			break;

		default:
			break;
		}
	}

private:
	bool isPress = false;
}; 

int main(int argc,char* argv[]){
	QApplication a(argc,argv);
	Widget w;
	w.show();
	return a.exec();	
}
#include "main.moc"