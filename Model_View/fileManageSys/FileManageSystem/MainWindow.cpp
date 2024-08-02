#include "MainWindow.h"
#include "ui_MainWindow.h"
#include<QSplitter>
#include<QBoxLayout>
MainWindow::MainWindow(QWidget* parent)
	:QWidget(parent)
	,ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	initUi();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initUi()
{
	//窗口分裂器
	auto splitter = new QSplitter(this);
	splitter->addWidget(ui->navTreeView);
	splitter->addWidget(ui->stackedWidget);

	//布局
	auto hlayout = new QHBoxLayout(this);
	hlayout->addWidget(splitter);
	//文件系统模型
	m_treeModel = new QFileSystemModel(this);
	m_treeModel->setRootPath("C:");
	m_treeModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	//树视图(文件目录)
	ui->navTreeView->setModel(m_treeModel);

	//显示文件
	ui->tableView->setModel(m_treeModel);

	connect(ui->navTreeView, &QTreeView::clicked, this, &MainWindow::onNavTreeViewClicked);
	connect(ui->tableView, &QTableView::clicked, [=](const QModelIndex& index) {
		//qDebug() << index.row() << " " << index.column();
		ui->tableView->setRootIndex(m_treeModel->index(index.row(),0,ui->tableView->rootIndex()));
		});
}

void MainWindow::onNavTreeViewClicked(const QModelIndex& index)
{
	ui->tableView->setRootIndex(index);
}
