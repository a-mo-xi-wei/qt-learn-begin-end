#include "MainWindow.h"
#include "ui_MainWindow.h" 
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QApplication>
#include <QMessageBox>
#include<QThread>
#include<qrandom.h>
#include<QCompleter>
#define mrand(min,max) QRandomGenerator::global()->bounded(min,max)


class ConnectionPool
{
public:
	static QSqlDatabase database(const QString& name = "default") {
		//查找是否存在name
		auto db = QSqlDatabase::database(name);
		if (db.isValid()) {
			return db;
		}
		db = QSqlDatabase::addDatabase("QMYSQL",name);
		db.setHostName("localhost");
		db.setPort(3306);
		db.setUserName("root");
		db.setPassword("WWWl192837465");
		db.setDatabaseName("students");

		if (!db.open())
		{
			QMessageBox::critical(nullptr, "数据库打开", "数据库打开失败" + db.lastError().text());
			qApp->quit();
		}
		return db;
	}
};


MainWindow::MainWindow(QWidget* parent)
	 :QWidget(parent)
	, ui(new Ui::MainWindow)
	,m_model(new QStandardItemModel(this))
	,m_proxyModel(new QSortFilterProxyModel(this))
{
	ui->setupUi(this);
	m_proxyModel->setSourceModel(m_model);
	ui->tableView->setModel(m_proxyModel);
	//ui->tableView->setModel(m_model);
	ui->tableView->setEditTriggers(QTableView::NoEditTriggers);
	ConnectionPool::database();
	qDebug() << "数据库打开成功";
	m_filedMapper.insert("id", "ID");
	m_filedMapper.insert("name", "用户名");
	m_filedMapper.insert("age", "年龄");

	auto compteter = new QCompleter({ "顽石老师","玩蛇","玩水" }, this);
	ui->searchEdit->setCompleter(compteter);
	connect(ui->searchEdit, &QLineEdit::textChanged, this, &MainWindow::on_searchBtn_clicked);
	onSearch();

	test_insertData();
}
MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::test_insertData()
{
	auto th = QThread::create([] {
		//1.子线程和主线程不能共用连接
		QSqlQuery query(ConnectionPool::database("sud_thread_conn"));
		QString sql = "insert into student(id,name,age) values";
		for (size_t i = 0; i < 100; i++)
		{
			sql += QString("(%1,'weisang_%2',%3),").arg(100 + i).arg(i + 1).arg(mrand(18, 50));
		}
		sql.chop(1);
		if (!query.exec(sql)) {
			qDebug() << "failed " << query.lastError().text();
		}
		else {
			qDebug() << "insert success";
		}
		});
	th->start();
	connect(th, &QThread::finished, [=] { 
		th->deleteLater();
		});
}

void MainWindow::on_searchBtn_clicked()
{
	qDebug() << __FUNCTION__;
	auto filter = ui->searchEdit->text();
	//m_proxyModel->setFilterKeyColumn(-1);
	//m_proxyModel->setFilterWildcard(filter);

	m_sqlFilter = filter;
	onSearch();
}

void MainWindow::onSearch()
{
	m_model->clear();
	auto thr = QThread::create([=] {
		auto db = ConnectionPool::database("main_thread_conn");
		{
			QSqlQuery query(db);
			if (!query.exec("SELECT * FROM student WHERE name LIKE '%" + m_sqlFilter + "%' LIMIT 2"))
			{
				QMessageBox::warning(this, "查询用户", "查询用户失败" + query.lastError().text());
				return;
			}

			auto record = query.record();
			auto filedCount = record.count();
			//获取表头
			QStringList headerLabels;
			for (size_t i = 0; i < filedCount; i++)
			{
				headerLabels << m_filedMapper.value(record.fieldName(i), record.fieldName(i));
			}
			m_model->setHorizontalHeaderLabels(headerLabels);


			//获取数据
			while (query.next())
			{
				QList<QStandardItem*> items;
				for (size_t i = 0; i < filedCount; i++)
				{
					items << new QStandardItem(query.value(i).toString());
				}
				m_model->appendRow(items);
			}
		}
		QSqlDatabase::removeDatabase("main_thread_conn");
		});
	thr->start();
	connect(thr, &QThread::finished, this, [=]()
		{
			thr->deleteLater();
		});
}
