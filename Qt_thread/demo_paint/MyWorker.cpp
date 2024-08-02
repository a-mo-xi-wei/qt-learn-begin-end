#include "MyWorker.h"
#include<QImage>
#include<QPainter>
#include<qrandom.h>
#include<QDebug>
#define mrand(min,max) QRandomGenerator::global()->bounded(min,max)
void MyWorker::drawImage()
{
	QImage img(640,480,QImage::Format_RGBA8888);	    
	QPainter painter(&img);
	painter.setPen(QPen(QColor(mrand(0,256),mrand(0,256),mrand(0,256),mrand(0,256)), 5));
	painter.setBrush(QBrush(QColor(mrand(0, 256), mrand(0, 256), mrand(0, 256), mrand(0, 256)),Qt::Dense2Pattern));

	//随机生成点
	QPoint pos[12];
	for (int i = 0; i < 12; i++) {
		pos[i] = QPoint(mrand(0, 640), mrand(0, 480));
	}
	painter.drawPolygon(pos, 12);
	qDebug() << "绘图函数已经完成";
	emit newImage(img);
}
