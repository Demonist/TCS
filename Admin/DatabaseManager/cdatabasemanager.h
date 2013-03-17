#ifndef CDATABASEMANAGER_H
#define CDATABASEMANAGER_H

#include <QtGui>
#include <QtSql>

namespace Ui {
	class CDatabaseManager;
}

class CDatabaseManager : public QMainWindow
{
	Q_OBJECT
	
protected:
	void showDatabases();
	void showTables();
	void connectToDatabase(const QString &databaseName);

public:
	explicit CDatabaseManager(QWidget *parent = 0);
	~CDatabaseManager();
	
private slots:
	void connectingToFile(const QString &fileName);
	void connectingToHost();
	void connectingToDatabase(const QString &databaseName);

	void on_pbnConnectToDB_clicked();
	void on_pbnDeleteDB_clicked();
	void on_pbnCreateDB_clicked();
	void on_pbnClear_clicked();
	void on_pbnDelete_clicked();
	void on_pbnClearAll_clicked();
	void on_pbnDeleteAll_clicked();
	void on_pbnCreateTables_clicked();

private:
	Ui::CDatabaseManager *ui;
	QString mConnectionName;
	bool mServer;
};

#endif // CDATABASEMANAGER_H
