#ifndef UTIL_H_
#define UTIL_H_

#include <QString>

inline quint64 operator""B(quint64 bytes) {	//重载后缀文本
	return bytes;
}
inline quint64 operator""KB(quint64 bytes) {
	return bytes * 1024B;
}
inline quint64 operator""MB(quint64 bytes) {
	return bytes * 1024KB;
}
inline quint64 operator""GB(quint64 bytes) {
	return bytes *1024MB;
}

/**
 * 传入字节大小，返回适合现实的文本.
 */
QString FitByteDisplayText(quint64 byteSize);

/**
 * 计算传输速度.
 */
QString transferSpeed(quint64 byteSize, quint64 elapsedTime);


/**  
* 计算剩余时间
*/
QString TimeRemaining(quint64 totalBytes, quint64 byteSize, quint64 elapsedTime);


#endif // !UTIL_H_
