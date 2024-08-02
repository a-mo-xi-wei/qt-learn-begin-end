#ifndef CONTACT_H_
#define CONTACT_H_
#include<QString>
#include<qrandom.h>
struct Contact 
{
	enum type{NONE,VIP,SVIP};
	Contact(const QString& username, const QString& nickname)
		:m_userName(username),
		m_nickName(nickname),
		m_profilePath(":/Resource/images/defaultProfile.png"),
		m_signature("这人很懒，啥也没写QAQ"),
		type(QRandomGenerator::global()->bounded(3))
	{

	}
	QString m_profilePath;	//头像路径
	QString m_userName;		//用户名
	QString m_nickName;		//昵称
	QString m_signature;	//自我签名
	int type;				//账户类型
};

#endif // !CONTACT_H_
