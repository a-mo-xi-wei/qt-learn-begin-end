#include<QApplication>
#include<QWidget>
#include<QStandardItemModel>	//标准的数据模型
#include<QListView>				//列表视图
int main(int argc,char* argv[]){
	QApplication a(argc,argv);
	//1.创建模型
	auto model = new QStandardItemModel(qApp);
	
	//2.创建视图
	QListView view;

	//3.把模型交给视图进行显示
	view.setModel(model);
	view.show();

	//4.给模型添加数据
	model->appendRow(new QStandardItem("kali"));
	model->insertRow(0, new QStandardItem("威桑"));
	qDebug() << model->rowCount();
	model->insertRow(1, new QStandardItem("redhat"));
	qDebug() << model->rowCount();
	model->setItem(0, new QStandardItem("ubuntu"));//当前行有数据就自动删除
	qDebug() << model->rowCount();
	model->setItem(3, new QStandardItem("centos"));

	//5.删除item	//从模型中溢出，并释放内存
	//model->removeRow(0);
	//model->removeRows(0,1);

	qDebug() << model->rowCount();

	//6.从模型中溢出item,但是不释放item
	auto list = model->takeRow(0);
	for (auto val : list) {
		delete val;
	}
	qDebug() << list.size();

	//7.item索引
	auto item = new QStandardItem("windows");
	model->appendRow(item);
	QModelIndex idx = model->indexFromItem(item);
	qDebug() << idx;

	//8.角色
	model->setData(idx, "666", Qt::ItemDataRole::DisplayRole);
	item->setData(QColor(159, 0, 90), Qt::ItemDataRole::DecorationRole);
	item->setData(11111, Qt::ItemDataRole::UserRole);
	item->setCheckable(true);
	qDebug() << item->data(Qt::UserRole).toULongLong();
	QObject::connect(&view, &QListView::clicked, [=](const QModelIndex& idx) {
		qDebug() << idx.data(Qt::ItemDataRole::DisplayRole).toString() << '\n' <<
			model->item(idx.row(), idx.column())->text();
		});
	return a.exec();	
}