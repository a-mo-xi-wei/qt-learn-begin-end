#include<QApplication>
#include<QWidget>
#include<QLabel>
#include<QLineEdit>
#include<QBoxLayout>
#include<QPushButton>
#include<QCheckBox>
#include<QGridLayout>
#include<QFormLayout>
#include<QComboBox>
#include<QPlainTextEdit>
#include<QStackedLayout>
#include<QButtonGroup>
#include<QSplitter>

class Widget : public QWidget
{
public:
	Widget(QWidget* parent = nullptr)
		:QWidget(parent)
	{
		//boxLayout_1();
		//gridLayout(this);
		//boxLayout();
		//formLayout();
		//stackedLayout();
		//setFixedWidth(500);
		splitter();
	}

	static void boxLayout(QWidget* parent)
	{
		//创建控件
		auto tipLab = new QLabel("用户名");
		auto usernameEdit = new QLineEdit;

		auto passwordLab = new QLabel("密 码");
		auto passwrodEdit = new QLineEdit;

		//创建布局
		auto hlayout = new QHBoxLayout;
		//把控件添加到布局里面
		hlayout->addWidget(tipLab);
		hlayout->addWidget(usernameEdit);

		auto hlayout1 = new QHBoxLayout;
		//把控件添加到布局里面
		hlayout1->addWidget(passwordLab);
		hlayout1->addWidget(passwrodEdit);

		//垂直布局
		auto vlayout = new QVBoxLayout(parent);
		//布局嵌套，在布局里面添加布局
		vlayout->addLayout(hlayout);
		vlayout->addLayout(hlayout1);


		//把布局交给窗口(布局应用在谁上面)
		//setXXX 一般就只能设置一个,setLayout 也就是说只能设置一个布局
		//addXXX 可以添加很多个,addWidget 
		//setLayout(vlayout);

	}
	void boxLayout_1()
	{
		auto* btn1 = new QPushButton("One");
		auto* btn2 = new QPushButton("Two");
		auto* btn3 = new QPushButton("Three");
		auto* btn4 = new QPushButton("Four");
		auto* btn5 = new QPushButton("Five");

		btn3->setFixedHeight(50);

		auto hlayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight, this);

		//设置内容边距 
		hlayout->setContentsMargins(0, 0, 0, 0);
		//设置控件间距 
		hlayout->setSpacing(0);

		hlayout->addStretch(1);

		hlayout->addWidget(btn1);
		hlayout->addWidget(btn2, 1, Qt::AlignmentFlag::AlignTop);
		hlayout->addWidget(btn3);
		hlayout->addWidget(btn4, 2, Qt::AlignmentFlag::AlignBottom);
		hlayout->addWidget(btn5);

		hlayout->addStretch(2);			//添加弹簧(拉伸空间)

		//hlayout->setDirection(QBoxLayout::Direction::TopToBottom);
		hlayout->setStretchFactor(btn5, 3);//设置按钮的弹性
		hlayout->setStretchFactor(btn4, 2);
	}
	static void gridLayout(QWidget* parent)
	{
		auto profileLab = new QLabel;
		auto accountEdit = new QLineEdit;
		auto passwordEdit = new QLineEdit;
		auto rememberPasswordChx = new QCheckBox("记住密码");
		auto autoLognChx = new QCheckBox("自动登录");
		auto loginBtn = new QPushButton("登录");
		auto registerAccountBtn = new QPushButton("注册账号");
		auto retrievePasswordBtn = new QPushButton("找回密码");

		profileLab->setPixmap(QPixmap(":/Resource/zay.png"));
		profileLab->setFixedSize(90, 90);
		profileLab->setScaledContents(true);

		accountEdit->setPlaceholderText("QQ号码/手机/邮箱");
		passwordEdit->setPlaceholderText("输入密码");

		rememberPasswordChx->setChecked(true);

		auto glayout = new QGridLayout(parent);
		glayout->addWidget(profileLab, 0, 0, 3, 1);
		glayout->addWidget(accountEdit, 0, 1, 1, 2);
		glayout->addWidget(passwordEdit, 1, 1, 1, 2);
		glayout->addWidget(rememberPasswordChx, 2, 1);
		glayout->addWidget(autoLognChx, 2, 2);
		glayout->addWidget(loginBtn, 3, 1, 1, 2);
		glayout->addWidget(registerAccountBtn, 0, 3);
		glayout->addWidget(retrievePasswordBtn, 1, 3);

	}
	void formLayout()
	{
		//auto tipLab = new QLabel("用户名");
		auto usernameEdit = new QLineEdit;
		usernameEdit->setPlaceholderText("中文、英文");
		//auto passwordLab = new QLabel("密 码");
		auto passwrodEdit = new QLineEdit;
		auto mobileEdit = new QLineEdit;
		auto emailEdit = new QLineEdit;

		auto manChx = new QCheckBox("男");
		auto womanChx = new QCheckBox("女");
		manChx->setChecked(true);
		auto provinceCmb = new QComboBox;
		for (size_t i = 0; i < 23; i++)
		{
			provinceCmb->addItem("中国" + QString::number(i), i);
		}

		connect(provinceCmb, &QComboBox::currentTextChanged, [=](const QString& text)
			{
				qDebug() << text << provinceCmb->currentData().toInt();
			});

		//GridLayout
		{
			//auto glayout = new QGridLayout(this);
			//glayout->addWidget(tipLab, 0, 0);
			//glayout->addWidget(usernameEdit, 0, 1);
			//glayout->addWidget(passwordLab, 1, 0);
			//glayout->addWidget(passwrodEdit, 1, 1);
		}

		//FormLayout
		{
			auto flayout = new QFormLayout(this);
			flayout->setRowWrapPolicy(QFormLayout::RowWrapPolicy::WrapAllRows);

			flayout->addRow("<font color=red size=3>*</font>用户名", usernameEdit);
			flayout->addWidget(new QLabel("<font color=gray size=2>中文、英文、特殊字符</font>"));

			flayout->addRow("<font color=red size=4>*</font>密  码", passwrodEdit);

			flayout->addRow("<font color=red size=5>*</font>手机号码", mobileEdit);

			flayout->addRow("电子邮箱", emailEdit);

			auto hlayout = new QHBoxLayout;
			hlayout->addWidget(manChx);
			hlayout->addWidget(womanChx);
			hlayout->addStretch();
			flayout->addRow("性别", hlayout);

			flayout->addRow("住址", provinceCmb);
			flayout->addRow("详细地址", new QPlainTextEdit);
		}

	}
	void stackedLayout()
	{
		auto btnGroup = new QButtonGroup(this);
		btnGroup->addButton(new QPushButton("page1"), 0);	
		btnGroup->addButton(new QPushButton("page2"), 1);
		auto hlayout = new QHBoxLayout;
		hlayout->addWidget(btnGroup->button(0));
		hlayout->addWidget(btnGroup->button(1));
		hlayout->addStretch();

		//用来管理多个页面的
		auto slayout = new QStackedLayout;
		slayout->addWidget(CreatePage(0));	//0
		slayout->addWidget(CreatePage(1));	//1

		auto vlayout = new QVBoxLayout;
		vlayout->addLayout(hlayout);
		vlayout->addLayout(slayout);

		setLayout(vlayout);

		//切换页面
		qDebug() << slayout->count();

		connect(btnGroup, &QButtonGroup::idClicked, [=](int id)
			{
				slayout->setCurrentIndex(id);
			});

	}

	void splitter()
	{
		auto sp = new QSplitter(this);
		sp->addWidget(new QPlainTextEdit);
		sp->addWidget(CreatePage(1));

		sp->setCollapsible(0, false);
		sp->setCollapsible(3, false);

		auto vsp = new QSplitter(Qt::Orientation::Vertical);
		vsp->addWidget(sp);
		vsp->addWidget(new QPlainTextEdit);

		auto hlayou = new QHBoxLayout(this);
		hlayou->addWidget(vsp);

	}

	static QWidget* CreatePage(int id)
	{
		if (id == 0)
		{
			QWidget* page = new QWidget;
			boxLayout(page);
			return page;
		}
		else if (id == 1)
		{
			QWidget* page = new QWidget;
			gridLayout(page);
			return page;
		}
		return nullptr;
	}
};

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	Widget w;
	w.show();

	return a.exec();
}

#include "main.moc"
