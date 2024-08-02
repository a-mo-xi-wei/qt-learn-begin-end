#include "LyricParse.h"
#include<QFile>	 
#include<QTextStream>
#include<QDebug>

LyricParse* LyricParse::instance()
{
	static LyricParse* ins = nullptr;
	if (ins == nullptr) {
		ins = new LyricParse;
	}
	return ins;
}

void LyricParse::addLyric(const QString& music, const QString& lyric)
{
	m_lyricFile.insert(music, lyric);
}

QList<QPair<QString, QString>> LyricParse::lyric(const QString& music)
{
	QList<QPair<QString, QString>> _pair;
	auto lyricFile = m_lyricFile.value(music);
	if (lyricFile.isEmpty()) {
		qWarning() << "lyricfile not found";
		return _pair;
	}
	QFile file(lyricFile);
	if (!file.open(QIODevice::ReadOnly)) {
		qWarning() << "parse lyricfile error : " << file.errorString();
		return _pair;
	}
	QTextStream stream(&file);
	while (!stream.atEnd()) {
		auto line = stream.readLine();
		auto data = line.split("]");
		_pair.append({ data.first().remove(0, 1), data.last() });
	}
	return _pair;
}

qint64 LyricParse::timeStrToIntger(const QString& time)
{
	auto arr = time.split(":");
	int second = arr.at(0).toInt() * 60 + arr.at(1).toInt();
	return second;
}
