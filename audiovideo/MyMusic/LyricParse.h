#ifndef LYRICPRASE_H_
#define LYRICPRASE_H_

#include<QString>
#include<QHash>

class LyricParse
{
public:
	static LyricParse* instance();
	void addLyric(const QString& music, const QString& lyric);
	QList<QPair<QString, QString>> lyric(const QString& music);
	static qint64 timeStrToIntger(const QString& time);
private:
	QHash<QString, QString>m_lyricFile;
};

#endif // !LYRICPRASE_H_
