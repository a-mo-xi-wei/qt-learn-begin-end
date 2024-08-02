#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_
#include<QWidget>
#include<QFileSystemModel>

namespace Ui { class MainWindow; }

class MainWindow : public QWidget
{
public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	void initUi();
public:
	void onNavTreeViewClicked(const QModelIndex& index);
private:
	Ui::MainWindow* ui;	//使用Ui里面继承过Ui_MainWindow的MianWindow
	QFileSystemModel* m_treeModel{};
};

#endif // !MAINWINDOW_H_
