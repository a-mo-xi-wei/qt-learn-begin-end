#ifndef MYWORKER_H_
#define MYWORKER_H_

#include<QObject>
class MyWorker : public QObject
{
	Q_OBJECT
public:
	void drawImage();
signals:
	void newImage(const QImage& img);

};


#endif // !MYWORKER_H_
