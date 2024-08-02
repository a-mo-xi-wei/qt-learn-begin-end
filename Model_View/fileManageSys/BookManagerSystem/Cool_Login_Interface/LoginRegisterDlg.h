#ifndef _LOGINREGISTERDLG_H_
#define _LOGINREGISTERDLG_H_	

#include<QDialog>
class QStackedWidget;
class QLabel;
class QLineEdit;
class QCheckBox;
class QRadioButton;
class LoginRegisterDlg : public QDialog
{
public:
	LoginRegisterDlg(QWidget* parent = nullptr);

private:
	void initUi();
	//登录控件
	QLineEdit*m_AccountEdit;
	QLineEdit* m_PwdEdit;
	QCheckBox* m_RemPwdChx;
	//注册控件
	QLineEdit* m_RegAccountEdit;
	QLineEdit* m_RegPwdEdit;
	QLineEdit* m_RegRePwdEdit;
	QRadioButton* m_RegManBtn;
	QRadioButton* m_RegWomenBtn;
	QWidget* CreateLoginWidget();
	QWidget* CreateRegisterWidget();
	void saveInfo();
	void readInfo();
public://slot
	void onLogin();
	void onRegister();

protected:
	void paintEvent(QPaintEvent* ev)override;
private:
	QRect m_rightRect;
	QRect m_leftRect;
	QRect m_middleRect;


	QStackedWidget* m_StkWidget;
	QWidget* m_LoginWidget;
	QWidget* m_RegisterWidget;

	QLabel* m_tipBox;
	void setTip(const QString& msg);
};
//模拟服务器验证
QString verifyLogin(const QByteArray& json);
QString verifyRegister(const QByteArray& json);

#endif