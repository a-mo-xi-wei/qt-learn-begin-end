#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include<QWidget>
#include<QListWidgetItem>
namespace Ui { class MainWindow; }

class MainWindow : public QWidget
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();
private slots:
    void on_msgSendBtn_clicked();

    void on_friendListWidget_itemClicked(QListWidgetItem *item);

private:
	Ui::MainWindow* ui{};
};

#endif // !MAINWINDOW_H_
