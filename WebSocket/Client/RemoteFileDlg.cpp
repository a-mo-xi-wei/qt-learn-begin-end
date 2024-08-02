#include "RemoteFileDlg.h"
#include "ui_RemoteFileDlg.h"
#include"Util.h"
#include<QDirIterator>
#include<QStandardPaths>
#include<QLineEdit>
#include<QBuffer>
#include<QDesktopServices>
/*请求对方文件列表*/
#define REQUEST_FILE_LIST "request_file_list"
#define RESPONSE_FILE_LIST "response_file_list"

#define Request(type) "request_"##type
#define Response(type) "response_"##type
QString FileTypeString(const QFileInfo& info);

RemoteFileDlg::RemoteFileDlg(QWidget* parent)
	:QWidget(parent)
	, ui(new Ui::RemoteFileDlg)
{
	ui->setupUi(this);
	init();
	start(QUrl("ws://127.0.0.1:8888"));
}

RemoteFileDlg::~RemoteFileDlg() {
	m_client->close();
	delete m_client;
	delete ui;
}

void RemoteFileDlg::start(const QUrl& url)
{
	if (!m_client) {
		m_client = new QWebSocket("威桑");
		connect(m_client, &QWebSocket::errorOccurred, [=] {qDebug() << "连接失败"; });
		connect(m_client, &QWebSocket::connected, this, &RemoteFileDlg::onConnected);
		connect(m_client, &QWebSocket::disconnected, this, &RemoteFileDlg::onDisConnected);
		connect(m_client, &QWebSocket::binaryMessageReceived, this, &RemoteFileDlg::onBinaryMessageReceived);
		connect(m_client, &QWebSocket::textMessageReceived, this, &RemoteFileDlg::onTextMessageReceived);
	}
	m_client->open(url);
}

void RemoteFileDlg::init()
{
	//本地文件
	m_localFileModel = new QStandardItemModel(this);
	ui->localFileTableView->setModel(m_localFileModel);
	/*设置不可编辑*/
	ui->localFileTableView->setEditTriggers(QTableView::NoEditTriggers);
	/*设置选中状态:选中一行*/
	ui->localFileTableView->setSelectionMode(QTableView::SelectionMode::ContiguousSelection);
	ui->localFileTableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
	ui->localFileTableView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

	m_localFileDir.setPath(QStandardPaths::writableLocation(QStandardPaths::PublicShareLocation));
	updateLocalFile();
	connect(ui->localFile_backBtn, &QPushButton::clicked, this, &RemoteFileDlg::localFileBack);
	connect(ui->localFile_refreshBtn, &QPushButton::clicked, this, &RemoteFileDlg::updateLocalFile);
	connect(ui->localFile_newFolderBtn, &QPushButton::clicked, this, [=] {
		QString folder = "新建文件夹";
		if (!m_localFileDir.mkdir(folder))
		{
			qDebug() << "创建文件夹失败";
		}
		QList<QStandardItem*> items;
		auto item = new QStandardItem(folder);
		item->setIcon(m_fileIconProvider.icon(QFileIconProvider::IconType::Folder));

		items.append(item);
		items.append(new QStandardItem(""));
		items.append(new QStandardItem());
		items.append(new QStandardItem("文件夹"));

		m_localFileModel->appendRow(items);

		ui->localFileTableView->scrollToBottom();
		ui->localFileTableView->edit(item->index());

		});
	connect(ui->localFile_curPathCmb, &QComboBox::currentIndexChanged, [=] {
		//qDebug() << "QComboBox::currentIndexChanged : " << ui->localFile_curPathCmb->currentText();
		setLocalFilePath(ui->localFile_curPathCmb->currentText());
		});
	connect(ui->localFile_curPathCmb->lineEdit(), &QLineEdit::returnPressed, [=] {
		qDebug() << "编辑结束";
		setLocalFilePath(ui->localFile_curPathCmb->currentText());
		});
	connect(m_localFileModel, &QStandardItemModel::itemChanged, [=](QStandardItem* item) {
		m_localFileDir.rename("新建文件夹", item->text());
		});
	connect(ui->localFileTableView, &QTableView::doubleClicked, this, &RemoteFileDlg::onLocalFileTableViewDoubleClicked);

	//远程文件
	m_remoteFileDir.setPath("");
	m_remoteFileModel = new QStandardItemModel(this);
	ui->remoteFileTableView->setModel(m_remoteFileModel);
	ui->remoteFileTableView->setEditTriggers(QTableView::NoEditTriggers);
	ui->remoteFileTableView->setSelectionMode(QTableView::SelectionMode::ContiguousSelection);
	ui->remoteFileTableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
	ui->remoteFileTableView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

	connect(ui->remoteFile_backBtn, &QPushButton::clicked, this, &RemoteFileDlg::remoteFileBack);
	connect(ui->remoteFile_refreshBtn, &QPushButton::clicked, this, &RemoteFileDlg::getRemoteFile);
	connect(ui->remoteFile_newFolderBtn, &QPushButton::clicked, this, [=] {
		sendJson({
			{"type","request_mkdir"},
			{"name","新建文件夹"},
			{"path",m_remoteFileDir.canonicalPath()}
			});

		});
	connect(ui->remoteFile_curPathCmb, &QComboBox::currentIndexChanged, [=] {
		//qDebug() << "QComboBox::currentIndexChanged : " << ui->remoteFile_curPathCmb->currentText();
		//setRemoteFilePath(ui->remoteFile_curPathCmb->currentText());//会造成递归
		m_remoteFileDir.setPath(ui->remoteFile_curPathCmb->currentText());
		getRemoteFile();
		});
	connect(ui->remoteFile_curPathCmb->lineEdit(), &QLineEdit::returnPressed, this, [=] {
		//qDebug() << "编辑结束";
		setRemoteFilePath(ui->remoteFile_curPathCmb->currentText());
		//m_remoteFileDir.setPath(ui->remoteFile_curPathCmb->currentText());
		//getRemoteFile();
		});
	connect(m_remoteFileModel, &QStandardItemModel::itemChanged, [=](QStandardItem* item) {
		//m_remoteFileDir.rename("新建文件夹", item->text());
		sendJson({
			{"type","request_rename"},
			{"oldName",item->data(Qt::UserRole).toString()},
			{"newName",item->text()},
			{"path",m_remoteFileDir.canonicalPath()}
			});
		});
	connect(ui->remoteFileTableView, &QTableView::doubleClicked, this, &RemoteFileDlg::onRemoteFileTableViewDoubleClicked);

	//两个 tabelview选择互斥
	connect(ui->localFileTableView, &QTableView::clicked, [=] {ui->remoteFileTableView->clearSelection(); });
	connect(ui->remoteFileTableView, &QTableView::clicked, [=] {ui->localFileTableView->clearSelection(); });

	m_contextMenu = new QMenu(this);
	m_contextMenu->addAction("打开", [=] {
		if (m_menuType == LocalMenu) {
			auto index = ui->localFileTableView->currentIndex();
			auto fileName = m_localFileModel->data(m_localFileModel->index(index.row(), 0), Qt::DisplayRole).toString();
			QDesktopServices::openUrl(QUrl::fromLocalFile(m_localFileDir.canonicalPath() + '/' + fileName));
		}
		});
	m_contextMenu->addAction("传输", this, &RemoteFileDlg::transfer);
	m_contextMenu->addAction("复制", [=] {
		if (m_menuType == LocalMenu) {
			auto index = ui->localFileTableView->currentIndex();
			auto fileName = m_localFileModel->data(m_localFileModel->index(index.row(), 0), Qt::DisplayRole).toString();
			m_optionFile = m_localFileDir.canonicalPath() + '/' + fileName;
		}
		else {
			auto index = ui->remoteFileTableView->currentIndex();
			auto fileName = m_remoteFileModel->data(m_remoteFileModel->index(index.row(), 0), Qt::DisplayRole).toString();
			m_optionFile = m_remoteFileDir.canonicalPath() + '/' + fileName;
		}
		m_optionType = m_menuType;
		});
	auto pasteAct = m_contextMenu->addAction("粘贴", [=] {
		sendFile(m_optionFile, m_remoteFileDir.canonicalPath());
		getRemoteFile();
		});
	pasteAct->setEnabled(false);
	m_contextMenu->addAction("删除", [=] {
		auto index = ui->remoteFileTableView->currentIndex();
		auto fileName = m_remoteFileModel->data(m_remoteFileModel->index(index.row(), 0), Qt::DisplayRole).toString();
		if (m_menuType == RemoteMenu) {
			sendJson({
				{"type","request_delete"},
				{"fileNamePath",m_remoteFileDir.canonicalPath() + '/' + fileName}
				});
		}
		else {
			auto index = ui->localFileTableView->currentIndex();
			auto fileName = m_localFileModel->data(m_localFileModel->index(index.row(), 0), Qt::DisplayRole).toString();
			if (QFile::remove(m_localFileDir.canonicalPath() + '/' + fileName)) {
				m_localFileModel->removeRow(index.row());
			}
		}

		});
	m_contextMenu->addAction("重命名");
	m_contextMenu->addAction("新建文件夹");
	m_contextMenu->addAction("全选");
	connect(ui->localFileTableView, &QTableView::customContextMenuRequested, [=](const QPoint& pos) {
		if (m_optionType == LocalMenu)pasteAct->setEnabled(false);
		else if (!m_optionFile.isEmpty()) pasteAct->setEnabled(true);
		m_menuType = LocalMenu;
		m_contextMenu->popup(ui->localFileTableView->mapToGlobal(pos) + QPoint(20, 25));
		});
	connect(ui->remoteFileTableView, &QTableView::customContextMenuRequested, [=] {
		if (m_optionType == RemoteMenu)pasteAct->setEnabled(false);
		else if (!m_optionFile.isEmpty()) pasteAct->setEnabled(true);
		m_menuType = RemoteMenu;
		m_contextMenu->popup(QCursor::pos());
		});

	//传输文件
	connect(ui->transferBtn, &QPushButton::clicked, this, &RemoteFileDlg::transfer);

}
QJsonObject RemoteFileDlg::getFiles(const QString& path)
{
	QJsonArray jfiles;
	QDirIterator iter(path, QDir::NoDotAndDotDot | QDir::AllEntries);
	while (iter.hasNext()) {
		auto info = iter.nextFileInfo();
		QJsonObject jobj;
		jobj.insert("fileName", info.fileName());
		jobj.insert("fileSize", info.size());
		jobj.insert("modifyTime", info.lastModified().toString("yyyy/MM/dd hh:mm"));
		jobj.insert("fileType", FileTypeString(info));
		//图片是二进制数据	因此需转成base64编码
		auto icon = m_fileIconProvider.icon(info);
		if (!icon.isNull()) {
			auto img = icon.pixmap(16).toImage();
			QByteArray bytes;
			QBuffer buf(&bytes);
			img.save(&buf, "ico");
			buf.close();
			jobj.insert("iconData", QString::fromLatin1(bytes.toBase64()));
		}
		jfiles.append(jobj);
	}
	QJsonObject jobj;
	jobj.insert("type", RESPONSE_FILE_LIST);
	jobj.insert("data", jfiles);
	jobj.insert("path", path);
	return jobj;
}

void RemoteFileDlg::localFileBack()
{
	m_localFileDir.cdUp();
	updateLocalFile();
	//qDebug() << "返回已刷新";
}

void RemoteFileDlg::setLocalFilePath(const QString& path)
{
	m_localFileDir.setPath(path);
	updateLocalFile();
	//qDebug() << "设置路径已刷新";
}

void RemoteFileDlg::updateLocalFile()
{
	//qDebug() << "调用updateLocalFile";
	m_localFileModel->clear();
	//qDebug() << "清理";
	//把当前路径展示出来
	auto idx = ui->localFile_curPathCmb->findText(m_localFileDir.absolutePath());
	if (idx != -1) {	//下拉框里已经有了这个路径
		ui->localFile_curPathCmb->setCurrentIndex(idx);
		//qDebug() << "下拉框里已经有了这个路径，无需添加";
	}
	else {
		ui->localFile_curPathCmb->addItem(m_localFileDir.canonicalPath());
		ui->localFile_curPathCmb->setCurrentIndex(ui->localFile_curPathCmb->count() - 1);
		/*注意setCurrentIndex会触发：currentIndexChanged 信号，导致触发槽函数，从而导致出现重复文件*/
		//qDebug() << "向下拉框中添加数据";
	}
	//最多展示10条路径
	if (ui->localFile_curPathCmb->count() > 10)ui->localFile_curPathCmb->removeItem(0);

	QDirIterator iter(m_localFileDir.absolutePath(), QDir::NoDotAndDotDot | QDir::AllEntries);
	//int i = 0;
	m_localFileModel->clear();//稍微有点取巧
	while (iter.hasNext()) {
		//qDebug() << "开始插入数据";
		auto info = iter.nextFileInfo();
		auto type = FileTypeString(info);
		QList<QStandardItem*>items;
		auto item = new QStandardItem(info.fileName());
		item->setIcon(m_fileIconProvider.icon(info));
		items.append(item);
		items.append(new QStandardItem(type == "文件夹" ? "" : FitByteDisplayText(info.size())));
		items.append(new QStandardItem(info.fileTime(QFile::FileTime::FileModificationTime).toString("yyyy/MM/dd hh:mm")));
		//items.append(new QStandardItem(info.lastModified().toString("yyyy/MM/dd hh:mm")));
		items.append(new QStandardItem(type));
		m_localFileModel->appendRow(items);
		//qDebug() << "现在有" << ++i << "条数据";
	}
}

void RemoteFileDlg::remoteFileBack()
{
	m_remoteFileDir.cdUp();
	getRemoteFile();
}

void RemoteFileDlg::setRemoteFilePath(const QString& path)
{
	m_remoteFileDir.setPath(path);
	getRemoteFile();
}

void RemoteFileDlg::getRemoteFile()
{
	if (!m_client || !m_client->isValid())return;
	//QJsonObject jobj;
	//jobj.insert("type", "request_file_list");
	//jobj.insert("path", m_remoteFileDir.canonicalPath());
	//sendJson(jobj);
	sendJson({
		{"type",REQUEST_FILE_LIST},
		{"path",m_remoteFileDir.canonicalPath()}
		});

}

void RemoteFileDlg::updateRemoteFile(const QJsonObject& jobj)
{
	m_remoteFileModel->clear();
	auto path = jobj.value("path").toString();
	//把当前路径展示出来
	if (ui->remoteFile_curPathCmb->currentIndex() == -1) {
		ui->remoteFile_curPathCmb->lineEdit()->setText(path);
		m_remoteFileDir.setPath(path);
	}
	//最多展示10条路径
	if (ui->remoteFile_curPathCmb->count() > 10)ui->remoteFile_curPathCmb->removeItem(0);

	auto jarr = jobj.value("data").toArray();
	for (size_t i = 0; i < jarr.size(); i++)
	{
		auto jfile = jarr.at(i).toObject();
		auto type = jfile.value("fileType").toString();
		QList<QStandardItem*>items;
		auto item = new QStandardItem(jfile.value("fileName").toString());
		QImage img;
		img.loadFromData(QByteArray::fromBase64(jfile.value("iconData").toString().toLatin1()), "ico");
		item->setIcon(QPixmap::fromImage(img));
		items.append(item);
		//items.append(new QStandardItem(jfile.value("fileSize").toInteger()));
		items.append(new QStandardItem(type == "文件夹" ? "" : FitByteDisplayText(jfile.value("fileSize").toInteger())));
		items.append(new QStandardItem(jfile.value("modifyTime").toString()));
		items.append(new QStandardItem(type));
		//qDebug() << "测试" << jfile["fileName"].toString()<<" : " << jfile.value("fileSize").toInteger();
		m_remoteFileModel->appendRow(items);
	}
}

void RemoteFileDlg::transfer()
{
	//判断是谁发给谁
	/*QModelIndexList selectedRows(int column = 0) const;*/
	QModelIndexList localRows = ui->localFileTableView->selectionModel()->selectedRows();
	QModelIndexList remoteRows = ui->remoteFileTableView->selectionModel()->selectedRows();

	//qDebug() << localRows;
	//qDebug() << remoteRows;
	//本地传给远端
	if (!localRows.isEmpty()) {
		for (const auto& index : localRows) {
			/*Qt::DisplayRole是一个预定义的角色，
				通常用于获取应该显示给用户的项目数据，
				也就是对应表格单元格的显示内容。*/
			auto fileName = index.data(Qt::DisplayRole).toString();
			sendFileOrFolder(m_localFileDir.canonicalPath() + '/' + fileName, m_remoteFileDir.canonicalPath());
		}
		getRemoteFile();
	}
	//远端传给本地
	else if (!remoteRows.isEmpty()) {
		QJsonArray jarray;
		for (const auto& idx : remoteRows) {
			auto fileName = m_remoteFileDir.canonicalPath() + '/' + idx.data(Qt::DisplayRole).toString();
			jarray.append(fileName);
		}
		sendJson(
			{
				{"type",Request("get_file_list")},
				{"filePath",m_localFileDir.canonicalPath()},
				{"data",jarray}
			}
		);
	}//没有选择文件
	else qDebug() << "请先选择文件";

}

void RemoteFileDlg::sendFile(const QString& fileName, const QString& perrSavePath)
{
	QFileInfo info(fileName);
	if (info.isFile()) {
		//发送文件信息
		sendJson({
			{"type","put"},
			{"filePath",perrSavePath},
			{"fileName",info.fileName()},
			{"fileSize",info.size()}
			});
		//发送文件数据
		QFile file(info.absoluteFilePath());
		if (!file.open(QIODevice::ReadOnly)) {
			qWarning() << file.fileName() << "打开失败" << file.errorString();
		}
		else {
			//如果文件太大，就分块发送
			if (file.size() > 4MB) {
				while (!file.atEnd()) {
					m_client->sendBinaryMessage(file.read(4MB));
				}
			}
			else m_client->sendBinaryMessage(file.readAll());
		}
	}

}

void RemoteFileDlg::sendFolder(const QString& foloder, const QString& perrSavePath)
{
	QFileInfo info(foloder);
	if (!info.exists() || !info.isDir()) return;
	QDirIterator iter(foloder, QDir::NoDotAndDotDot | QDir::AllEntries, QDirIterator::Subdirectories);
	while (iter.hasNext()) {
		auto nextInfo = iter.nextFileInfo();
		if (nextInfo.isFile()) {
			//qDebug() << nextInfo.canonicalFilePath();
			//qDebug() << perrSavePath + '/' + nextInfo.canonicalPath().remove(info.canonicalPath()).remove(0,1) + '/' + nextInfo.fileName();
			sendFile(nextInfo.canonicalFilePath(), perrSavePath + '/' + nextInfo.canonicalPath().remove(info.canonicalPath()).remove(0, 1));
		}
	}

}

void RemoteFileDlg::sendFileOrFolder(const QString& fileName, const QString& perrSavePath)
{
	QFileInfo info(fileName);
	if (info.isFile())sendFile(info.absoluteFilePath(), perrSavePath);
	else if (info.isDir())sendFolder(fileName, perrSavePath);
}

void RemoteFileDlg::onLocalFileTableViewDoubleClicked(const QModelIndex& index)
{
	auto filename = m_localFileModel->data(m_localFileModel->index(index.row(), 0)).toString();
	qDebug() << filename;
	QFileInfo info(m_localFileDir.absolutePath() + "/" + filename);
	if (info.isDir()) {
		m_localFileDir.cd(filename);
		updateLocalFile();
	}
}

void RemoteFileDlg::onRemoteFileTableViewDoubleClicked(const QModelIndex& index)
{
	auto filename = m_remoteFileModel->data(m_remoteFileModel->index(index.row(), 0)).toString();
	auto fileType = m_remoteFileModel->data(m_remoteFileModel->index(index.row(), 3)).toString();
	qDebug() << filename;
	//QFileInfo info(m_remoteFileDir.absolutePath() + '/' + filename);
	//if (info.isDir()) {
	//	m_remoteFileDir.cd(filename);
	//	updateRemoteFile();
	//}
	if (fileType == "文件夹") {
		m_remoteFileDir.cd(filename);
		getRemoteFile();
	}
}

void RemoteFileDlg::onConnected()
{
	qDebug() << __FUNCTION__;
	//m_client->sendBinaryMessage("Hello web");
	//m_client->sendTextMessage("Hello web");
	getRemoteFile();
}

void RemoteFileDlg::onDisConnected()
{
	qDebug() << __FUNCTION__;
}

void RemoteFileDlg::onBinaryMessageReceived(const QByteArray& data)
{
	//qDebug() << __FUNCTION__;
	if (m_recvFile) {
		m_recvFile->write(data);
		//显示下载进度
		qDebug() << "传输速度 : " << transferSpeed(m_recvFile->size(), m_elapsedtimer.elapsed())
			+ " 已传输 : " + FitByteDisplayText(m_recvFile->size()) + ",共" + FitByteDisplayText(m_totalSize)
			+ " 预计还剩 : " << TimeRemaining(m_totalSize, m_recvFile->size(), m_elapsedtimer.elapsed());
		//接收完成
		if (m_recvFile->size() == m_totalSize) {
			qDebug() << "接收完成~~" << m_recvFile->fileName();
			m_recvFile->close();
			m_recvFile->deleteLater();
			m_recvFile = nullptr;
			m_totalSize = 0;
			updateLocalFile();
		}
	}

}

void RemoteFileDlg::onTextMessageReceived(const QString& data)
{
	//qDebug() << __FUNCTION__;
	QJsonParseError err;
	auto jdoc = QJsonDocument::fromJson(data.toUtf8(), &err);
	if (err.error != QJsonParseError::NoError) {
		qWarning() << "json parse error," << err.errorString();
		return;
	}
	//获取消息类型 
	auto type = jdoc["type"].toString();
	qDebug() << type;
	if (type == REQUEST_FILE_LIST) {
		auto path = jdoc["path"].toString();
		path = path.isEmpty() ? QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) : path;
		sendJson(getFiles(path));
	}
	else if (type == RESPONSE_FILE_LIST) {	//收到消息就会更新
		updateRemoteFile(jdoc.object());
	}
	else if (type == "put") {
		m_recvFileName = jdoc["fileName"].toString();
		m_totalSize = jdoc["fileSize"].toInteger();
		auto recvFilePath = jdoc["filePath"].toString();

		QDir dir(recvFilePath);
		if (!dir.exists()) {
			if (!dir.mkpath(recvFilePath)) {
				qWarning() << "创建路径失败" << recvFilePath;
				return;
			}
		}

		m_recvFile = new QFile(recvFilePath + "/" + m_recvFileName);
		if (!m_recvFile->open(QIODevice::WriteOnly | QIODevice::Append)) {
			qWarning() << m_recvFileName << " 打开失败" << m_recvFile->errorString();
		}
		else {
			//qDebug() << m_recvFileName << " 打开成功";
			m_elapsedtimer.restart();
		}
	}
	else if (type == "request_mkdir") {
		QDir dir(jdoc["path"].toString());
		dir.mkdir(jdoc["name"].toString());
		sendJson({
			{"type","response_mkdir"},
			{"name",jdoc["name"].toString()}
			});
	}
	else if (type == "response_mkdir") {
		QList<QStandardItem*> items;
		auto item = new QStandardItem(jdoc["name"].toString());
		item->setIcon(m_fileIconProvider.icon(QFileIconProvider::IconType::Folder));
		item->setData(jdoc["name"].toString(), Qt::UserRole);
		items.append(item);
		items.append(new QStandardItem(""));
		items.append(new QStandardItem());
		items.append(new QStandardItem("文件夹"));

		m_remoteFileModel->appendRow(items);
		ui->remoteFileTableView->scrollToBottom();
		ui->remoteFileTableView->edit(item->index());
	}
	else if (type == Request("rename")) {
		QDir dir(jdoc["path"].toString());
		dir.rename(jdoc["oldName"].toString(), jdoc["newName"].toString());
	}
	else if (type == Request("delete")) {
		if (QFile::remove(jdoc["fileNamePath"].toString())) {
			sendJson({
				{"type",Response("delete")},
				{"message","success"}
				});
		}
		else {
			sendJson({
				{"type",Response("delete")},
				{"message","failed"}
				});
		}
	}
	else if (type == Response("delete")) {
		if (jdoc["messsage"] == "success") {
			auto index = ui->remoteFileTableView->currentIndex();
			m_remoteFileModel->removeRow(index.row());
			getRemoteFile();
		}
	}
	else if (type == Request("get_file_list")) {
		auto jarray = jdoc["data"].toArray();
		for (size_t i = 0; i < jarray.size(); i++)
		{
			auto fileName = jarray.at(i).toString();
			auto peersavePath = jdoc["filePath"].toString();
			sendFileOrFolder(fileName, peersavePath);
		}
	}
}

static QString FileTypeString(const QFileInfo& info)
{
	if (info.isAlias())return "别名";				//mac专用
	else if (info.isBundle())return "bundle";	//mac专用
	else if (info.isDir())return "文件夹";
	else if (info.isExecutable())return "exe";
	else if (info.isFile())return info.suffix();
	else if (info.isHidden())return "隐藏文件";
	else if (info.isJunction())return "lnk";
	else if (info.isShortcut())return "lnk";
	else if (info.isSymLink())return "lnk";
	else if (info.isSymbolicLink())return "lnk";
	else return "unknown";
}