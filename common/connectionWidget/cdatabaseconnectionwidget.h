#ifndef CDATABASECONNECTIONWIDGET_H
#define CDATABASECONNECTIONWIDGET_H

#include <QtGui>
#include <QtSql>
#include "global.h"

namespace Ui {
class CDataBaseConnectionWidget;
}

/**
Виджет полдключения к базе данных.
Данные класс @b НЕ является менеджером подключений. Т.е. он не отвечает за закрытие подключения.

@author Demonist
*/
class CDataBaseConnectionWidget : public QWidget
{
	Q_OBJECT

	//types:
public:
	enum ConnectionType{ConnectionServer, ConnectionFile};

	//members:
protected:
	Ui::CDataBaseConnectionWidget *ui;
	QSettings *mCache;

	mutable bool mConnected;
	ConnectionType mConnectionType;
	QString mConnectionName;    //! Имя соединения с базой данных.
	QStringList mCheckTables;	//! Имена таблиц, которые должны присутствовать в базе данных.

	//methods:
protected:
	virtual void processConnecting();

public:
	explicit CDataBaseConnectionWidget(QWidget *parent = 0);
	~CDataBaseConnectionWidget();

	inline QString connectionName() const;
	bool isConnected() const;
	bool isCloseButtonVisible() const;
	bool isConnectionChoiceEnable() const;
	inline ConnectionType connectionType() const;
	inline QStringList checkTables() const;

private slots:
	void on_pbnDbExit_clicked();
	void on_pbnDbConnect_clicked();
	void on_rbnDbServer_toggled(bool checked);
	void on_rbnDbFile_toggled(bool checked);
	void on_tbnDbFile_clicked();
	void on_cbxDbHost_currentIndexChanged(const QString &arg1);

public slots:
	void setConnectionName(const QString &connectionName);
	void setCloseButtonVisible(bool show);
	void setConnectionChoiceEnable(bool enable);
	void setConnectionType(const ConnectionType &connectionType);
	inline void setCheckTables(const QStringList &checkTablesList);

signals:
	void closed();
	void connectedToDatabase(QString connectionName);
};

//inline implementation:

inline QString CDataBaseConnectionWidget::connectionName() const
{
	return mConnectionName;
}

inline CDataBaseConnectionWidget::ConnectionType CDataBaseConnectionWidget::connectionType() const
{
	return mConnectionType;
}

/**
  Задает список таблиц, которые обязательно должны присутсвовать в базе данных.
Если \a checkTablesList пустой список, то проверка не производится.
*/
inline void CDataBaseConnectionWidget::setCheckTables(const QStringList &checkTablesList)
{
	mCheckTables = checkTablesList;
}
inline QStringList CDataBaseConnectionWidget::checkTables() const
{
	return mCheckTables;
}

#endif // CDATABASECONNECTIONWIDGET_H
