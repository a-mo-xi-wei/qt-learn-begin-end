#ifndef REMOTEFILEDLG_H_
#define REMOTEFILEDLG_H_

#include<QWidget>
#include<QDir>
#include<QStandardItemModel>
#include<QFileIconProvider>
#include<QWebSocket>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QJsonValue>
#include<QMenu>
#include<QCursor>
#include<QElapsedTimer>
namespace Ui { class RemoteFileDlg; }

class RemoteFileDlg : public QWidget
{
	Q_OBJECT
public:
	explicit RemoteFileDlg(QWidget* parent = nullptr);
	~RemoteFileDlg();
	void start(const QUrl& url);
private:
	void init();
	void sendJson(const QJsonObject& jobj) {
		m_client->sendTextMessage(QJsonDocument(jobj).toJson(QJsonDocument::Compact));
	};
	QJsonObject getFiles(const QString& path);

	void localFileBack();
	void setLocalFilePath(const QString& path);
	void updateLocalFile();

	void remoteFileBack();
	void setRemoteFilePath(const QString& path);
	void getRemoteFile();
	void updateRemoteFile(const QJsonObject& jobj);

	void transfer();

	void sendFile(const QString& fileName, const QString& perrSavePath);
	void sendFolder(const QString& fileName, const QString& perrSavePath);
	void sendFileOrFolder(const QString& fileName, const QString& perrSavePath);
private://slots
	void onLocalFileTableViewDoubleClicked(const QModelIndex& index);
	void onRemoteFileTableViewDoubleClicked(const QModelIndex& index);

	void onConnected();
	void onDisConnected();
	void onBinaryMessageReceived(const QByteArray& data);
	void onTextMessageReceived(const QString& data);
private:
	Ui::RemoteFileDlg* ui;
	QDir m_localFileDir;
	QStandardItemModel* m_localFileModel{};

	QDir m_remoteFileDir;
	QStandardItemModel* m_remoteFileModel{};

	QFileIconProvider m_fileIconProvider;

	QMenu* m_contextMenu{};
	enum MenuType{LocalMenu,RemoteMenu};
	int m_menuType{};
	//保存复制、删除、改名的文件
	QString m_optionFile;
	//当前操作的是本地还是远程的文件
	int m_optionType{ -1 };

	QWebSocket* m_client{};

	QString m_recvFileName;
	quint64 m_recvSize{};
	quint64 m_totalSize{};
	QFile* m_recvFile{};
	
	QElapsedTimer m_elapsedtimer;
};


#endif // !REMOTEFILEDLG_H_
