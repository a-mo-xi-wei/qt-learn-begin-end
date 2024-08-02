#include "LoginRegisterDlg.h"
#include"EventFilterObject.h"
#include<QPainter>
#include<QGraphicsDropShadowEffect>
#include<QStaticText>
#include<QPushButton>
#include<QFile>
#include<QStackedWidget>
#include<QBoxLayout>
#include<QLineEdit>
#include<QCheckBox>
#include<QComboBox>	
#include<QRadioButton>
#include<QLabel>
#include<QSettings>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonParseError>
LoginRegisterDlg::LoginRegisterDlg(QWidget* parent)
	:QDialog(parent)
{
	Q_INIT_RESOURCE(Resource);
	initUi();
	readInfo();
}

void LoginRegisterDlg::initUi()
{
	//去掉标题栏
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
	//让无边框窗口能够被拖动
	installEventFilter(new EventFilterObject(this));
	//让窗口背景透明
	this->setAttribute(Qt::WA_TranslucentBackground);
	//调整窗口大小
	this->resize(740, 470);

	//右边矩形
	this->m_rightRect = { 5,40,width() - 5 * 2,height() - 40 * 2 };
	//左边矩形
	this->m_leftRect = { 5,5,185,440 };
	//中间弦矩形
	this->m_middleRect = { 4,5,370,440 };

	//给窗口加上阴影特效
	auto effect = new QGraphicsDropShadowEffect(this);
	effect->setOffset(0);
	effect->setBlurRadius(10);
	this->setGraphicsEffect(effect);
	
	this->m_tipBox = new QLabel(this);
	this->m_tipBox->setGeometry(434, 390, 200, 30);
	this->m_tipBox->setAlignment(Qt::AlignCenter);
	setTip("请登录QAQ!");

	//关闭按钮
	auto closeBtn = new QPushButton(this);
	closeBtn->setGeometry(width() - 45, 45, 32, 32);
	closeBtn->setObjectName("closeBtn");
	connect(closeBtn, &QPushButton::clicked, this, &QDialog::close);

	//堆栈窗口
	this->m_StkWidget = new QStackedWidget(this);
	QRect r = { 434,150,200,240 };
	this->m_StkWidget->setGeometry(r);
	this->m_StkWidget->setContentsMargins(0, 0, 0, 0);
	this->m_LoginWidget = CreateLoginWidget();
	this->m_RegisterWidget = CreateRegisterWidget();
	this->m_StkWidget->addWidget(this->m_LoginWidget);
	this->m_StkWidget->addWidget(this->m_RegisterWidget);

	//加载样式表
	QFile file(":/Resource/style.css");
	if (file.open(QIODevice::ReadOnly))
	{
		setStyleSheet(file.readAll());
	}
}

QWidget* LoginRegisterDlg::CreateLoginWidget()
{
	QWidget* w = new QWidget;
	m_AccountEdit = new QLineEdit;
	m_PwdEdit = new QLineEdit;
	m_RemPwdChx = new QCheckBox("记住密码");
	auto registerBtn = new QPushButton("注册账号");
	auto loginBtn = new QPushButton("登录");
	
	m_AccountEdit->setPlaceholderText("输入手机/邮箱");
	m_PwdEdit->setPlaceholderText("输入密码");
	m_RemPwdChx->setObjectName("remPwdChx");
	registerBtn->setObjectName("registerBtn");
	loginBtn->setObjectName("loginBtn");

	connect(registerBtn, &QPushButton::clicked, [=] {
		this->m_StkWidget->setCurrentWidget(this->m_RegisterWidget);
		});
	connect(loginBtn, &QPushButton::clicked, this, &LoginRegisterDlg::onLogin);
	

	auto vlayout = new QVBoxLayout;
	vlayout->setContentsMargins(0, 0, 0, 0);
	vlayout->addWidget(m_AccountEdit);
	vlayout->addWidget(m_PwdEdit);
	auto hlayout = new QHBoxLayout;
	hlayout->addWidget(m_RemPwdChx);
	hlayout->addWidget(registerBtn);
	vlayout->addLayout(hlayout);
	vlayout->addWidget(loginBtn);
	w->setLayout(vlayout);
	return w;
}

QWidget* LoginRegisterDlg::CreateRegisterWidget()
{
	QWidget* w = new QWidget;
	m_RegAccountEdit = new QLineEdit;
	m_RegPwdEdit = new QLineEdit;
	m_RegRePwdEdit = new QLineEdit;
	m_RegManBtn = new QRadioButton("男");
	m_RegWomenBtn = new QRadioButton("女");
	auto registeringBtn = new QPushButton("注册");
	auto backLoginBtn = new QPushButton("返回登录");
	m_RegAccountEdit->setPlaceholderText("输入用户名");
	m_RegPwdEdit->setPlaceholderText("输入密码");
	m_RegRePwdEdit->setPlaceholderText("重新输入密码");
	registeringBtn->setObjectName("registeringBtn");
	backLoginBtn->setObjectName("backLoginBtn");

	connect(backLoginBtn, &QPushButton::clicked, [this] {
		this->m_StkWidget->setCurrentWidget(this->m_LoginWidget);
		this->m_tipBox->setText("请登录QAQ!");
		});
	connect(registeringBtn, &QPushButton::clicked, this, &LoginRegisterDlg::onRegister);

	

	auto hLayout1 = new QHBoxLayout;
	hLayout1->addWidget(m_RegManBtn);
	hLayout1->addWidget(m_RegWomenBtn);
	auto hLayout2 = new QHBoxLayout;
	hLayout2->setSpacing(0);
	hLayout2->addWidget(registeringBtn);
	hLayout2->addWidget(backLoginBtn);
	auto vLayout = new QVBoxLayout(w);
	vLayout->addWidget(m_RegAccountEdit);
	vLayout->addWidget(m_RegPwdEdit);
	vLayout->addWidget(m_RegRePwdEdit);
	vLayout->addLayout(hLayout1);
	vLayout->addLayout(hLayout2);

	return w;
}

void LoginRegisterDlg::saveInfo()
{

	QSettings settings("user.ini", QSettings::IniFormat);
	settings.setValue("account", m_AccountEdit->text());
	if(!m_RemPwdChx->isChecked())
		settings.setValue("passwd", "");
	else 
		settings.setValue("passwd", m_PwdEdit->text());
	settings.setValue("remPwd", m_RemPwdChx->isChecked());
}

void LoginRegisterDlg::readInfo()
{
	QSettings settings("user.ini", QSettings::IniFormat);
	m_AccountEdit->setText(settings.value("account").toString());
	auto isRem = settings.value("remPwd").toBool();
	//如果之前是保存密码，那么这次默认也是
	m_RemPwdChx->setChecked(isRem);
	if (isRem)
	{
		m_PwdEdit->setText(settings.value("passwd").toString());
	}
}

void LoginRegisterDlg::onLogin()
{
	auto account = m_AccountEdit->text();
	auto pwd = m_PwdEdit->text();
	auto isRemPwd = m_RemPwdChx->isChecked();
	if (account.isEmpty())
	{
		setTip("请输入账号");
	}
	else if (pwd.isEmpty())
	{
		setTip("请输入密码");
	}
	else setTip("登录中...");
	//向服务器发送请求，验证账号和密码
	QJsonObject jobj;
	jobj["account"] = account;
	jobj["passwd"] = pwd;
	auto json = QJsonDocument(jobj).toJson(QJsonDocument::Compact);
	qDebug() << json;
	auto data = verifyLogin(json);
	QJsonParseError err;
	QJsonDocument jd = QJsonDocument::fromJson(data.toLocal8Bit(), &err);
	if (err.error != QJsonParseError::NoError)
	{
		qDebug() << R"({"code":200,"message":"json 解析失败"})";
		return;
	}
	else
	{
		//进入主界面
		qDebug() << data;
	}
	//验证成功就保存
	saveInfo();
}

void LoginRegisterDlg::onRegister()
{
	auto account = m_RegAccountEdit->text();
	auto pwd = m_RegPwdEdit->text();
	auto rePwd = m_RegRePwdEdit->text();
	auto gender = m_RegManBtn->isChecked() ? 1 : (m_RegWomenBtn->isChecked()? 2 : 0);
	if (account.isEmpty())setTip("请输入账号");
	else if (pwd.isEmpty())setTip("请输入密码");
	else if (rePwd.isEmpty())setTip("请再次输入密码");
	else if (pwd != rePwd)setTip("两次输入的密码不一致");
	else setTip("注册中...");

	QJsonObject jobj;
	jobj["account"] = account;
	jobj["passwd"] = pwd;
	jobj["gender"] = gender;
	auto json = QJsonDocument(jobj).toJson(QJsonDocument::Compact);
	qDebug() << json;
}

void LoginRegisterDlg::paintEvent(QPaintEvent* ev)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);//反锯齿
	//设置右边白色矩形
	painter.setPen(Qt::NoPen);
	painter.fillRect(this->m_rightRect, Qt::white);
	//绘制左边矩形
	painter.fillRect(this->m_leftRect, qRgb(27, 45, 67));
	//绘制中间的弦
	painter.setBrush(QColor(27, 45, 67));
	int startAngle = -90 * 16;
	int spanAngle = 180 * 16;
	painter.drawChord(this->m_middleRect, startAngle, spanAngle);
	//绘制welcome
	QPoint p = { 48,60 };
	painter.setPen(Qt::white);
	painter.setFont(QFont("微软雅黑", 30));
	painter.drawStaticText(p, QStaticText("Welcome!"));
	painter.setFont(QFont("微软雅黑", 10, QFont::Bold));
	p = { 48 ,120 };
	painter.drawStaticText(p, QStaticText("登录注册界面---威桑"));
	//梦想
	QRect r = { 40,200,250,250 };
	painter.drawPixmap(r, QPixmap(":/Resource/cai.png"));
	painter.drawPixmap(r, QPixmap(":/Resource/dream.png"));
	//logo
	p = { 423,70 };
	painter.drawPixmap(p, QPixmap(":/Resource/dunkai.png"));
}

void LoginRegisterDlg::setTip(const QString& msg)
{
	this->m_tipBox->setText("<font color = red>" + msg + "</font>");
	
}

QString verifyLogin(const QByteArray& json)
{
	QJsonParseError err;
	QJsonDocument jd = QJsonDocument::fromJson(json, &err);
	if (err.error != QJsonParseError::NoError)
	{
		return R"({"code":200,"message":"json 解析失败"})";
	}
	return R"({"code":200,"message":"success"})";
}
QString verifyRegister(const QByteArray& json)
{
	return "";
}
