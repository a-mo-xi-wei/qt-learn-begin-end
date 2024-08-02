#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include<QWidget>
#include<QPushButton>
#include<QLabel>
#include<QSlider>
#include<QListWidget>
#include<QMediaPlayer>

class MainWindow : public QWidget
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);

protected:
	void resizeEvent(QResizeEvent* ev)override;

public:
	void onPlaylistWidgetClicked();

private:
	void init();
	//顶部
	QWidget* CreateTopWidget();
	//中间
	QLabel* m_central_pixLab{};
	QListWidget* m_central_lyricListWidget{};
	QWidget* CreateCentralWidget();
	//底部
	QPushButton* m_bottom_prevBtn{};
	QPushButton* m_bottom_nextBtn{};
	QPushButton* m_bottom_playBtn{};
	QLabel* m_bottom_currentDurationLab{};	//播放时长
	QSlider* m_bottom_progressSlider{};		//播放进度
	QLabel* m_bottom_totalDurationLab{};	//总时长
	QPushButton* m_bottom_volumeBtn{};		//控制音量
	QPushButton* m_bottom_playListBtn{};	//控制播放列表显示
	QWidget* CreateBottomWidget();
	//歌曲列表
	QListWidget* m_playlistWidget{};		//播放列表容器
	QListWidget* CreateMusicListWidget(QWidget* parent = nullptr);

	//音量调节slider
	QSlider* m_volumeSlider{};				//音量滑块

	QMediaPlayer* m_player{};
	QList<QUrl>m_playlist;					//播放列表
	int m_currentIndex{ -1 };				//当前下标
	void loadMedia();
	void setCurrentIndex(int index);
	int currentIndex() { return m_currentIndex; }

signals:
	void currentIndexChanged(int index);
};

#endif // !MAINWINDOW_H_
