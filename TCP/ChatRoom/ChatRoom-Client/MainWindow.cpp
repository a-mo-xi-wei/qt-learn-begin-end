#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Util/SEventfilterObject.h"
#include "Util/SChatBubble.h"
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDateTime>

MainWindow::MainWindow(QWidget* parent)
	:QWidget(parent)
	,ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	initUi();
	setWindowFlag(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	installEventFilter(new SEventFilterObject(this));


	m_client.connectToHost(QHostAddress("127.0.0.1"), 8888);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initUi()
{
	SChatBubble* b = new SChatBubble(QPixmap(), "hello wrold");
	ui->chatMsgListWidget->addItem(b);
	ui->chatMsgListWidget->setItemWidget(b, b);

	 b = new SChatBubble(QPixmap(), "hello wroldsdfsfjsklfjkdjfklj",SChatBubble::BubbleRight);
	ui->chatMsgListWidget->addItem(b);
	ui->chatMsgListWidget->setItemWidget(b, b);

	m_userid = QDateTime::currentDateTime().toMSecsSinceEpoch();

	connect(&m_client, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
	connect(&m_client, &QTcpSocket::connected, this, &MainWindow::onConnected);
	connect(&m_client, &QTcpSocket::disconnected, this, &MainWindow::onDisConnected);
	connect(&m_client, &QTcpSocket::errorOccurred, this, &MainWindow::onErrorOccurred);
}

/* 发送消息 */
void MainWindow::on_msgSendBtn_clicked()
{

}

/* 联系人选择改变 */
void MainWindow::on_friendListWidget_itemClicked(QListWidgetItem *item)
{

}

void MainWindow::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
}

void MainWindow::onConnected()
{
	//让用户输入用户名
	auto username = QInputDialog::getText(this, "欢迎使用", "请输入用户名");
	//登录
	QJsonObject obj;
	obj.insert("type", "login");
	obj.insert("username", username);
	obj.insert("id", (qreal)m_userid);

	m_client.write(QJsonDocument(obj).toJson(QJsonDocument::Compact));
}

void MainWindow::onDisConnected()
{
}

void MainWindow::onReadyRead()
{
}

