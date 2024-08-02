#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_
#include<QWidget>
#include"MyWorker.h"
#include<QThread>
#include<memory>
#include<qtimer.h>
class MainWindow : public QWidget
{
public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
protected:
	void paintEvent(QPaintEvent* ev)override;

private:
	MyWorker worker;
	QThread* th;
	//std::shared_ptr<QThread>th;
	QImage m_img;
	QTimer timer;
};


#endif // !MAINWINDOW_H_
