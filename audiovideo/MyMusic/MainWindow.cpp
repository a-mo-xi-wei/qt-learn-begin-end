#include "MainWindow.h"
#include"LyricParse.h"
#include<QBoxLayout>
#include<QAudioOutput>
#include<QDirIterator>
MainWindow::MainWindow(QWidget* parent)
	:QWidget(parent)
{
	init();
	LyricParse lp;
}

void MainWindow::resizeEvent(QResizeEvent* ev)
{
	m_playlistWidget->setFixedWidth(280);
	m_playlistWidget->setFixedHeight(height() - 110);
	m_playlistWidget->move(width() - m_playlistWidget->width(), 50);
}

void MainWindow::onPlaylistWidgetClicked()
{
	setCurrentIndex(m_playlistWidget->currentRow());
	m_bottom_playBtn->setText("暂停");
}

void MainWindow::init()
{
	//整体布局
	auto vlayout = new QVBoxLayout(this);
	vlayout->addWidget(CreateTopWidget());
	vlayout->addWidget(CreateCentralWidget());
	vlayout->addWidget(CreateBottomWidget()); 
	//音乐播放
	m_playlistWidget = CreateMusicListWidget(this);
	m_volumeSlider = new QSlider(this);
	m_volumeSlider->setFixedSize(20, 100);
	m_volumeSlider->hide();
	//初始化播放器
	m_player = new QMediaPlayer(this);
	auto audioOutput = new QAudioOutput(this);
	m_player->setAudioOutput(audioOutput);
	

	connect(m_bottom_playListBtn, &QPushButton::clicked, [=]
		{
			if (m_playlistWidget->isHidden())m_playlistWidget->show();
			else m_playlistWidget->hide();
		});
	connect(m_bottom_volumeBtn, &QPushButton::clicked, [=]
		{
			if (m_volumeSlider->isHidden()) {
				auto pos = m_bottom_volumeBtn->mapTo(this,QPoint(0,0));
				m_volumeSlider->move(pos + QPoint(25, -110));
				m_volumeSlider->show();
			}
			else m_volumeSlider->hide();
		});
	connect(m_bottom_prevBtn, &QPushButton::clicked, [=]
		{
			int size = m_playlistWidget->count();
			setCurrentIndex((m_currentIndex + size - 1) % size);	
		});
	connect(m_bottom_playBtn, &QPushButton::clicked, [=]
		{
			if (m_player->isPlaying()) {
				m_bottom_playBtn->setText("播放");
				m_player->pause();
			}
			else {	//当前是暂停状态
				if (m_currentIndex == m_playlistWidget->currentRow()) {
					m_bottom_playBtn->setText("暂停");
					m_player->play();
				}
				else {
					setCurrentIndex(m_playlistWidget->currentRow());
					m_bottom_playBtn->setText("暂停");
				}
			}
		});
	connect(m_bottom_nextBtn, &QPushButton::clicked, [=]
		{
			int size = m_playlistWidget->count();
			setCurrentIndex((m_currentIndex + 1) % size);
			
		});

	connect(m_playlistWidget, &QListWidget::doubleClicked, this, &MainWindow::onPlaylistWidgetClicked);
	connect(this, &MainWindow::currentIndexChanged, [=](int index)
		{
			m_playlistWidget->setCurrentRow(index);
			m_central_lyricListWidget->clear();
			auto info = QFileInfo(m_playlist.at(m_currentIndex).fileName());
			auto lyricPair = LyricParse::instance()->lyric(info.baseName());
			for (const auto& lp : lyricPair) {
				auto item = new QListWidgetItem(lp.second);
				item->setData(Qt::UserRole, LyricParse::timeStrToIntger(lp.first));
				m_central_lyricListWidget->addItem(item);
			}
		});

	connect(m_player, &QMediaPlayer::durationChanged, [=](qint64 duration)
		{
			m_bottom_progressSlider->setRange(0, duration);
			m_bottom_totalDurationLab->setText
			(QString("%1:%2").arg(duration/1000/60,2,10,QChar('0')).arg(duration / 1000 % 60, 2, 10, QChar('0')));
		});
	connect(m_player, &QMediaPlayer::positionChanged, [=](qint64 position)
		{
			m_bottom_progressSlider->setValue(position);
			m_bottom_currentDurationLab->setText
			(QString("%1:%2").arg(position / 1000 / 60, 2, 10, QChar('0')).arg(position / 1000 % 60, 2, 10, QChar('0')));
		});
	connect(m_bottom_progressSlider, &QSlider::sliderMoved,[=] (int position)
	{
			m_player->setPosition(position);
	});
	//connect(m_playlistWidget, &QListWidget::currentRowChanged, [=](int row) {qDebug() << "当前是" << m_playlistWidget->currentRow(); });
	connect(m_player, &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus status)
		{
			if (status == QMediaPlayer::MediaStatus::EndOfMedia) {
				m_bottom_playBtn->setText("播放");  
			}
		});
	
	m_volumeSlider->setRange(0, 100);	 
	//qDebug() << audioOutput->volume();	//默认1
	m_volumeSlider->setValue(audioOutput->volume() * 100);
	connect(m_volumeSlider, &QSlider::valueChanged, [=](int value)
		{
			audioOutput->setVolume(value / 100.0f);
		});
	loadMedia();
	for (const QUrl& url : m_playlist) {
		m_playlistWidget->addItem(new QListWidgetItem(url.fileName()));
	}
	
}

QWidget* MainWindow::CreateTopWidget()
{
	auto w = new QLabel("音乐播放器");
	auto fnt = w->font();
	fnt.setPixelSize(20);
	w->setFont(fnt);
	w->setAlignment(Qt::AlignCenter);
	return w;
}

QWidget* MainWindow::CreateCentralWidget()
{
	auto w = new QWidget;
	m_central_pixLab = new QLabel;
	m_central_lyricListWidget = new QListWidget;
	auto hlayout = new QHBoxLayout(w);
	hlayout->addWidget(m_central_pixLab);
	hlayout->addWidget(m_central_lyricListWidget);

	m_central_pixLab->setPixmap(QPixmap(":/Resource/images/record.png"));
	m_central_pixLab->setFixedSize(450,500);
	return w;
}

QWidget* MainWindow::CreateBottomWidget()
{
	auto w = new QWidget;
	m_bottom_prevBtn = new QPushButton("上一曲");
	m_bottom_nextBtn = new QPushButton("下一曲");
	m_bottom_playBtn = new QPushButton("播放");
	m_bottom_currentDurationLab = new QLabel("00:00");
	m_bottom_progressSlider = new QSlider(Qt::Horizontal);
	m_bottom_totalDurationLab = new QLabel("00:00");
	m_bottom_volumeBtn = new QPushButton("音量");
	m_bottom_playListBtn = new QPushButton("播放列表");
	auto hlayout = new QHBoxLayout(w);
	hlayout->addWidget(m_bottom_prevBtn);
	hlayout->addWidget(m_bottom_playBtn);
	hlayout->addWidget(m_bottom_nextBtn);
	hlayout->addWidget(m_bottom_currentDurationLab);
	hlayout->addWidget(m_bottom_progressSlider);
	hlayout->addWidget(m_bottom_totalDurationLab);
	hlayout->addWidget(m_bottom_volumeBtn);
	hlayout->addWidget(m_bottom_playListBtn);

	return w;
}

QListWidget* MainWindow::CreateMusicListWidget(QWidget* parent) 
{
	auto w = new QListWidget(parent);
	w->hide();

	return w;
}

void MainWindow::loadMedia()
{
	QString path("F:\\code_review\\Qt-WorkSpace\\learn-begin-end\\audiovideo\\MyMusic\\Resource\\music");
	QDirIterator iter(path, { "*.mp3","*.ogg","*.mp4","*.lrc"});
	while (iter.hasNext()) {
		auto info = iter.nextFileInfo();
		if (info.suffix() == "lrc") {
			//qDebug() << info.baseName()<<info.absoluteFilePath();
			LyricParse::instance()->addLyric(info.baseName(), info.absoluteFilePath());
		}
		else {
		m_playlist.append(QUrl::fromLocalFile(info.absoluteFilePath()));

		}
	}
	
	
}

void MainWindow::setCurrentIndex(int index)
{
	if (m_playlist.isEmpty()) {
		qWarning() << "playlist is empty";
		return;
	}
	if (index < 0 || index >= m_playlist.count()) {
		qWarning() << "index out of range";
		return;
	}
	if (m_currentIndex != index) {
		m_currentIndex = index;
		emit currentIndexChanged(m_currentIndex);
	}
	m_player->setSource(m_playlist.at(m_currentIndex));
	m_player->play();
}

