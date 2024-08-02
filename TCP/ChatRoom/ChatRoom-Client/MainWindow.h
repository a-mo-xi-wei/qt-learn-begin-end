#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include<QWidget>
#include<QListWidgetItem>
#include<QTcpSocket>
namespace Ui { class MainWindow; }

class MainWindow : public QWidget
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	void initUi();
private slots:
    void on_msgSendBtn_clicked();

    void on_friendListWidget_itemClicked(QListWidgetItem *item);

	void onErrorOccurred(QAbstractSocket::SocketError socketError);
	void onConnected();
	void onDisConnected();
	void onReadyRead();
private:
	Ui::MainWindow* ui{};
	QTcpSocket m_client;
	quint64 m_userid;
};

#endif // !MAINWINDOW_H_
