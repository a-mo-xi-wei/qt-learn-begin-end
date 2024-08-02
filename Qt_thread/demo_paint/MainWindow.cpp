#include "MainWindow.h"
#include<QPushButton>
#include<QPainter>
#include<QTimer>
MainWindow::MainWindow(QWidget* parent)
	:th(new QThread)
{
	setFixedSize(640, 480);
	auto btn = new QPushButton("绘图", this);

	//产生图片的工作类
	worker.moveToThread(th);
	
	timer.setInterval(100);
	connect(&timer, &QTimer::timeout, btn, &QPushButton::click);
	timer.start();
	connect(btn, &QPushButton::clicked, &worker, &MyWorker::drawImage);
	th->start();
	
	connect(&worker, &MyWorker::newImage, [=](const QImage& img) {
		m_img = img;
		update();
	});
}
MainWindow::~MainWindow() {
	//th.get()->wait();
	//th.get()->destroyed();
	th->wait();
	th->deleteLater();
}
void MainWindow::paintEvent(QPaintEvent* ev)
{
	QPainter painter(this);
	painter.drawImage(0,0,m_img);
	return QWidget::paintEvent(ev);
}
