#include<QApplication>
#include<QWidget>
#include<QSoundEffect>
#include<QMediaPlayer>
#include<QAudioOutPut>
#include<QVideoWidget>
class Test : public QObject
{
public:
	Test() {
		//audioCall();
		mediaPlayer();
	}
	void audioCall() {
		if (!m_sound) {
			m_sound = new QSoundEffect(this);
			m_sound->setSource(QUrl::fromLocalFile("F:\\code_review\\Qt-WorkSpace\\learn-begin-end\\audiovideo\\audio\\Resource\\sound\\message.wav"));
			//m_sound->setSource(QUrl::fromLocalFile("F:\\code_review\\Qt-WorkSpace\\learn-begin-end\\audiovideo\\audio\\Resource\\sound\\output.mp4"));
			//mp4报错
			m_sound->setLoopCount(QSoundEffect::Infinite);
		}
		m_sound->play();
	}
	void mediaPlayer() {
		if (!m_player) {
			//创建播放器
			m_player = new QMediaPlayer(this);
			//创建音频输出设备
			auto audioOutput = new QAudioOutput(this);
			//创建视频输出
			auto videoOutput = new QVideoWidget;
			//设置给播放器
			m_player->setAudioOutput(audioOutput);
			m_player->setVideoOutput(videoOutput);
			
			//设置媒体源
			//m_player->setSource(QUrl::fromLocalFile("F:\\code_review\\Qt-WorkSpace\\learn-begin-end\\audiovideo\\audio\\Resource\\sound\\message.wav"));
			m_player->setSource(QUrl::fromLocalFile("F:\\code_review\\Qt-WorkSpace\\learn-begin-end\\audiovideo\\audio\\Resource\\sound\\output.mp4"));
			//播放
			videoOutput->show();
			m_player->play();
			
		}
	}
private:
	QSoundEffect* m_sound{};
	QMediaPlayer* m_player{};
};

int main(int argc,char* argv[]){
	QApplication a(argc,argv);
	Test t;
	return a.exec();	
}