#include<QApplication>
#include<QLabel>
#include<QPushButton>
#include<QLineEdit>
#include<QCheckBox>
#include<QGridLayout>
#include<QDialog>
#include<QSettings>
#include<QDesktopServices>

class LoginDlg : public QDialog
{
public:
	LoginDlg(QWidget* parent = nullptr)
		:QDialog(parent)
	{
		initUi();
		readConfig();
	}

	void initUi()
	{

		profileLab = new QLabel;
		accountEdit = new QLineEdit;
		passwordEdit = new QLineEdit;
		rememberPasswordChx = new QCheckBox("记住密码");
		autoLognChx = new QCheckBox("自动登录");
		loginBtn = new QPushButton("登录");
		registerAccountBtn = new QPushButton("注册账号");
		retrievePasswordBtn = new QPushButton("找回密码");

		passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);	//起遮盖效果

		profileLab->setPixmap(QPixmap(":/Resource/zay.png"));
		profileLab->setFixedSize(90, 90);
		profileLab->setScaledContents(true);

		accountEdit->setPlaceholderText("QQ号码/手机/邮箱");
		passwordEdit->setPlaceholderText("输入密码");

		auto glayout = new QGridLayout;
		glayout->addWidget(profileLab, 0, 0, 3, 1);
		glayout->addWidget(accountEdit, 0, 1, 1, 2);
		glayout->addWidget(passwordEdit, 1, 1, 1, 2);
		glayout->addWidget(rememberPasswordChx, 2, 1);
		glayout->addWidget(autoLognChx, 2, 2);
		glayout->addWidget(loginBtn, 3, 1, 1, 2);
		glayout->addWidget(registerAccountBtn, 0, 3);
		glayout->addWidget(retrievePasswordBtn, 1, 3);

		setLayout(glayout);


		connect(loginBtn, &QPushButton::clicked, this, &LoginDlg::onLogin);
		connect(registerAccountBtn, &QPushButton::clicked, this, []
			{
				QDesktopServices::openUrl(QUrl("www.bilibili.com"));
			});

	}

	void rememberPassword()
	{
		QSettings settings("config.ini", QSettings::IniFormat);
		settings.setValue("account", accountEdit->text());
		settings.setValue("rememberPassword", rememberPasswordChx->isChecked());
		settings.setValue("autoLogin", autoLognChx->isChecked());

		if (!rememberPasswordChx->isChecked())
			settings.setValue("password", QVariant());
		else
			settings.setValue("password", passwordEdit->text());
	}
	void readConfig()
	{
		QSettings settings("config.ini", QSettings::IniFormat);
		accountEdit->setText(settings.value("account").toString());

		bool isRem = settings.value("rememberPassword", false).toBool();
		if (isRem)
		{
			rememberPasswordChx->setChecked(true);
			passwordEdit->setText(settings.value("password").toString());
			if (settings.value("autoLogin", false).toBool())
			{
				loginBtn->click();
			}
		}
	}
public://slots:
	void onLogin()
	{
		auto account = accountEdit->text();
		auto passwrod = passwordEdit->text();

		if (true)
		{
			qDebug() << account << passwrod;
			rememberPassword();
		}
	}
private:
	QLabel* profileLab{};
	QLineEdit* accountEdit{};
	QLineEdit* passwordEdit{};
	QCheckBox* rememberPasswordChx{};
	QCheckBox* autoLognChx{};
	QPushButton* loginBtn{};
	QPushButton* registerAccountBtn{};
	QPushButton* retrievePasswordBtn{};

};

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);


	LoginDlg w;
	w.show();

	return a.exec();
}

#include "login.moc"
