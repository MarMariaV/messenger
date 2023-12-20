#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include <QMap>
#include <QString>

class QStandardItemModel;
class MyTextEdit;
class Client;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

private:
	QStandardItemModel* m_currentModel;
	MyTextEdit* m_textEdit;
	QThread* m_netThread;
	Client* m_client;

	Ui::MainWindowClass ui;

	quint16 m_fromID;
	quint16 m_toID;
	QString m_name;

	QMap<int, QString> contactMap;
	QMap<int, QStandardItemModel*> modelMap;

	QButtonGroup* bgContacts;

	void readAddressBook();
	void createContacts();

public slots:
	void slotIsConnected(bool bState);

private slots:
	void textFinished();
	void slotContactIsConnected(bool bState, quint16 id);
	void slotSetMessage(QString str, int fromID);
	void setChoseContact(int);
};
