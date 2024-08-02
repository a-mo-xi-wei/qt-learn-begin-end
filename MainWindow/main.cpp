#include<QApplication>
#include<QWidget>
#include<QMainWindow>
#include<QMenuBar>
#include<QToolBar>
#include<QStyle>
#include <QPushButton>
#include<QToolButton>
#include<QStatusBar>
#include<QLabel>
#include<QTimer>
#include<QDockWidget>
#include<QTextEdit>
#include<QSystemTrayIcon>
#include<QContextMenuEvent>
class MainWindow : public QMainWindow
{
public:
	MainWindow(QWidget* parent = nullptr)
		:QMainWindow(parent)
	{
		this->resize(640, 480);
		test_menuBar();
		test_toolBar();
		test_statusBar();
		test_dockWidget();
		initContextMenu();
		initTrayIconMenu();
	}
	//菜单栏
	void test_menuBar()
	{
		//添加菜单栏
		auto mbar = menuBar();
		//给菜单栏添加菜单
		QMenu* fileMenu = new QMenu("菜单");
		//菜单栏（QMenuBar）中标题和图片只能选一个，会覆盖，但子菜单（QMenu）不会
		//fileMenu->setIcon(QIcon(":/Resource/Icon/file_icon.png"));

		QMenu* editMenu = new QMenu("编辑");
		//添加菜单项
		fileMenu->addAction("文件");
		QMenu* openFileMenu = new QMenu("打开");
		openFileMenu->setIcon(QIcon(":/Resource/Icon/file_icon.png"));
		openFileMenu->addAction("项目");
		openFileMenu->addAction("文件夹");
		openFileMenu->addAction("网站");
		fileMenu->addMenu(openFileMenu);

		QAction* selectAct = new QAction("选择");
		selectAct->setCheckable(true);

		connect(selectAct, &QAction::toggled, [](bool check) {
			qDebug() << check;
			});

		editMenu->addAction(selectAct);

		mbar->addMenu(fileMenu);
		mbar->addMenu(editMenu);

		//快捷键	取地址符 + 一个大写字母，然后在使用的时候按 alt + 字母 即可造成点击效果
		QMenu* view = mbar->addMenu("视图(&V)");
		view->addAction("解决方案管理器");



	}
	//工具栏
	void test_toolBar() {
		//添加工具栏
		auto toolBar_1 = addToolBar("toolbar_1");
		toolBar_1->addAction("wei");
		toolBar_1->addAction(style()->standardIcon(QStyle::StandardPixmap::SP_TitleBarMenuButton), "文件");
		auto toolBar_2 = addToolBar("toolbar_2");
		toolBar_2->addWidget(new QPushButton("touch me"));

		auto toolBtn = new QToolButton;
		toolBtn->setText("hahah");
		toolBtn->setIcon(style()->standardPixmap(QStyle::SP_ArrowLeft));
		toolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);//使文本显示在图标旁
		//设置菜单
		QMenu* menu = new QMenu("回退");
		menu->addAction("上海");
		menu->addAction("广州");
		menu->addAction("海南");
		toolBtn->setMenu(menu);
		//toolBtn->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);
		toolBtn->setPopupMode(QToolButton::ToolButtonPopupMode::MenuButtonPopup);
		toolBar_2->addWidget(toolBtn);
		
		
	}
	//状态栏
	void test_statusBar() {
		//获取状态栏
		QStatusBar* sBar = statusBar();
		//给状态栏添加控件
		sBar->addWidget(new QPushButton(style()->standardPixmap(QStyle::StandardPixmap::SP_DesktopIcon), "desktop"));
		sBar->addWidget(new QLabel("就绪"));

		auto btn = new QPushButton("press  me");
		connect(btn, &QPushButton::clicked, [=] {
			sBar->showMessage("正在加载中...");
			QTimer::singleShot(2000, [=] {
				sBar->showMessage("就绪");
				QTimer::singleShot(2000, [=] {
					sBar->clearMessage();
					});
				});
			});
		connect(sBar, &QStatusBar::messageChanged, [](const QString& str) {
			qDebug() << str;
			});
		//sBar->addWidget(btn);
		sBar->addPermanentWidget(btn);

	}
	//浮动窗口
	void test_dockWidget() {
		//添加中心窗口
		setCentralWidget(new QTextEdit);


		//添加浮动窗口
		QDockWidget* gitDock = new QDockWidget("Git");
		QDockWidget* SloveDock = new QDockWidget("解决方案管理器");
		QDockWidget* teamDock = new QDockWidget("团队资源管理器");
		QDockWidget* resourceDock = new QDockWidget("资源视图");
		 
		auto btn = new QPushButton(gitDock);
		//connect(gitDock, &QDockWidget::topLevelChanged, [=](bool dock) {
		//	if (!dock) {
		//		btn->move(0, gitDock->titleBarWidget()->height());
		//	}
		//	});
		connect(gitDock, &QDockWidget::dockLocationChanged, [](Qt::DockWidgetArea area) {
			qDebug() << area;
			});

		addDockWidget(Qt::LeftDockWidgetArea, gitDock);
		addDockWidget(Qt::RightDockWidgetArea, SloveDock);
		addDockWidget(Qt::TopDockWidgetArea, teamDock);
		addDockWidget(Qt::BottomDockWidgetArea, resourceDock);

		//分割浮动窗口
		splitDockWidget(gitDock, resourceDock, Qt::Orientation::Vertical);

		//选项卡（tab）//注意先后顺序和逻辑顺序
		tabifyDockWidget(gitDock, SloveDock);
		//tabifyDockWidget(SloveDock,teamDock); 
		//tabifyDockWidget(teamDock,resourceDock);

	}
	//初始化右键菜单
	void initContextMenu() {
		this->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);//不调用事件处理函数发出信号
		m_contextMenu = new QMenu(this);
		m_contextMenu->addAction("复制");
		m_contextMenu->addAction("粘贴");
		m_contextMenu->addAction("剪切");
		connect(this, &QWidget::customContextMenuRequested, [this](const QPoint& pos) {
			auto p = this->mapToGlobal(pos);
			m_contextMenu->popup(p);
			});
	}

	//重写右键菜单弹出请求事件处理函数
	void contextMenuEvent(QContextMenuEvent* ev)override {
		qDebug() << "context requested";
		m_contextMenu->popup(ev->globalPos());
	}

	//托盘菜单
	void initTrayIconMenu() {
		auto m_TrayMenu = new QMenu(this);
		m_TrayMenu->addAction("打开主面板",[=] {
			this->showNormal();
			this->raise();
			});
		m_TrayMenu->addSeparator();
		m_TrayMenu->addAction("退出", [] {qApp->quit();});
		//创建托盘（一定要设置图标，没有图标，托盘不可见）
		this->m_TrayIcon = new QSystemTrayIcon(QIcon(":/Resource/Icon/trayIcon.png"),this);
		this->m_TrayIcon->setContextMenu(m_TrayMenu);
		//显示托盘
		this->m_TrayIcon->show();
		connect(this->m_TrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayMenuActive);
		
	}
	void onTrayMenuActive(QSystemTrayIcon::ActivationReason reason) {
		switch (reason)
		{
		case QSystemTrayIcon::Unknown:
			qDebug() << "UnKnown";
			break;
		case QSystemTrayIcon::Context:		//右键
			qDebug() << "Context";
			break;
		case QSystemTrayIcon::DoubleClick:	//双击
			qDebug() << "DoubleClick";
			break;
		case QSystemTrayIcon::Trigger:		//左键
		{
			qDebug() << "Trigger";
			if (this->isMinimized())
			{
				this->showNormal();
				this->raise();
			}
			else
			{
				this->showMinimized();
			}
		}
			break;
		case QSystemTrayIcon::MiddleClick:	//中击
			qDebug() << "MiddleClick";
			break;
		default:
			break;
		}
	}

private:
	QMenu* m_contextMenu;
	QSystemTrayIcon* m_TrayIcon;
};

int main(int argc,char* argv[]){
	QApplication a(argc,argv);
	MainWindow w;
	w.show();
	return a.exec();	
}