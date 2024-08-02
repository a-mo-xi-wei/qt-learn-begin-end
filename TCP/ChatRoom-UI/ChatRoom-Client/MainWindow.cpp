#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Util/SEventfilterObject.h"
#include "Util/SChatBubble.h"

MainWindow::MainWindow(QWidget* parent)
	:QWidget(parent)
	,ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setWindowFlag(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	installEventFilter(new SEventFilterObject(this));

	SChatBubble* b = new SChatBubble(QPixmap(), "hello wrold");
	ui->chatMsgListWidget->addItem(b);
	ui->chatMsgListWidget->setItemWidget(b, b);

	 b = new SChatBubble(QPixmap(), "hello wroldsdfsfjsklfjkdjfklj",SChatBubble::BubbleRight);
	ui->chatMsgListWidget->addItem(b);
	ui->chatMsgListWidget->setItemWidget(b, b);
}

MainWindow::~MainWindow()
{
	delete ui;
}

/* 发送消息 */
void MainWindow::on_msgSendBtn_clicked()
{
  }

/* 联系人选择改变 */
void MainWindow::on_friendListWidget_itemClicked(QListWidgetItem *item)
{

}

