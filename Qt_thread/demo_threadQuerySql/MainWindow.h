#ifndef  _MAINDINDOW_H_
#define   _MAINDINDOW_H_

#include <QWidget>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QMap>

namespace Ui { class MainWindow; }
class MainWindow : public QWidget
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	void test_insertData();
public slots:
	void on_searchBtn_clicked();
	void onSearch();
private:
	Ui::MainWindow* ui{};
	QStandardItemModel* m_model{};
	QSortFilterProxyModel* m_proxyModel{};
	QMap<QString, QString> m_filedMapper;
	QString m_sqlFilter;
};

#endif // ! _MAINDINDOW_H_
