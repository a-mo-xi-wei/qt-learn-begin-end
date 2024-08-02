#include "Util.h"

QString FitByteDisplayText(quint64 byteSize)
{
	qreal bytes = (qreal)byteSize;
	QString unit;	//显示的单位
	if(bytes< 1KB) {
		unit = "B";
	}
	else if(bytes< 1MB) {
		bytes /= 1KB;
		unit = "KB";
	}
	else if(bytes < 1GB){
		bytes /= 1MB;
		unit = "MB";
	}
	else{
		bytes /= 1GB;
		unit = "GB";
	}
	return QString("%1%2").arg(bytes,4,'f',2).arg(unit);
}

QString transferSpeed(quint64 byteSize, quint64 elapsedTime)
{
	//计算下载速度
	qreal speed = byteSize * 1000.0 / elapsedTime;
	//单位
	QString unit;
	
	if (speed < 1KB) {
		unit = "B/s";
	}
	else if (speed < 1MB) {
		speed /= 1KB;
		unit = "KB/s";
	}
	else if (speed < 1GB) {
		speed /= 1MB;
		unit = "MB/s";
	}
	else {
		speed /= 1GB;
		unit = "GB/s";
	}
	return QString("%1%2").arg(speed,4,'f',2).arg(unit);
}

QString TimeRemaining(quint64 totalBytes, quint64 byteSize, quint64 elapsedTime)
{
	//剩余多少没有传递完毕
	qreal reamBytes = static_cast<qreal>(totalBytes - byteSize);
	//计算当前的下载速度
	qreal speed = byteSize * 1000.0 / elapsedTime;
	//计算剩余字节下载需要的时长
	qreal duration =  reamBytes / speed;
	//单位
	QString unit;
	//小于一分钟
	if (duration < 60) {
		unit = "秒";
	}
	//小于一小时
	else if (duration < 60 * 60) {
		duration /= 60;
		unit = "分钟";
	}
	//小于一天
	else if (duration < 60 * 60 * 24) {
		duration /= 60 * 60;
		unit = "小时";
	}
	//小于一个月
	else if (duration < 60 * 60 * 24 * 30) {
		duration /= 60 * 60 * 24;
		unit = "天";
	}
	else {
		duration /= 60 * 60 * 24 * 30;
		unit = "月";
	}
	return QString("%1%2").arg(duration,2,'f',1).arg(unit);
}
