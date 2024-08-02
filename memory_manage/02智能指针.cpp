#include<QApplication>
#include<QPushButton>
#include<QWidget>
#include<QPointer>

class SString
{
public:
	SString() = default;
	SString(const QString& str);
	SString(const std::string& str);
	SString(const char* str)
	{
		m_size = strlen(str);
		m_capacity = m_size + 1;
		m_str = new char[m_capacity];
		strncpy(m_str, str, m_size);
		m_str[m_size] = '\0';
	}


	const char* data()const { return m_str; }
	char* data() { return m_str; }

	char& operator[](size_t idx)
	{
		return m_str[idx];
	}
	char operator[](size_t idx)const 
	{
		return m_str[idx];
	}
	~SString() { 
		//delete[] m_str;
	}
private:
	char* m_str = nullptr;
	size_t m_size = 0;
	size_t m_capacity = 0;
};

class Test : public QWidget
{
	Q_OBJECT
public:
	Test(QWidget* parent = nullptr)
		:QWidget(parent)
	{
		test_QPointer(); 
		test_QSharedDataPointer();
	}
	void test_QPointer()
	{
		//QPointer 不会释放保存的对象
		QPointer btn = new QPushButton("^_^", this);
		if (btn) {
			qDebug() << "在在在";
		}
		//btn->deleteLater();//没用，要等到下一次事件循环，应该直接delete
		delete(btn);
		if(!btn) {
			qDebug() << "不在不在";
		}
	}

	void test_QSharedDataPointer()
	{
		SString name = "god";
		SString you = name;	//使用了默认拷贝构造，会造成浅拷贝
		//读时共享，写时复制
		qDebug() <<Qt::hex<< (int*)name.data() << (int *)you.data();
		qDebug() << name.data()[0] << you.data()[0];

		name.data()[0] = 'b';
		qDebug() << name.data() << you.data();

	}
};


int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	Test test;
	test.show();
	return a.exec();
}
#include "main.moc"