#ifndef CONTROLMANAGERMAINWINDOW_H
#define CONTROLMANAGERMAINWINDOW_H

#include <QtGui>
#include <QtSql>
#include "cticketidentifier.h"
#include "ccontrolupdatedbase.h"

namespace Ui {
class ControlManagerMainWindow;
}

class ControlManagerMainWindow : public QMainWindow
{
    Q_OBJECT

private:
	void addItem(QTreeWidgetItem *item, const QColor &color);

public:
    explicit ControlManagerMainWindow(QWidget *parent = 0);
    ~ControlManagerMainWindow();

	inline QColor labelColor() const;

public slots:
	void setLabelColor(const QColor &color);
	void setLabelColorAlbescent(const QColor &color, const int duration = 2000);

private slots:
    void on_leGetBarcode_returnPressed();
    void connected(QString connectionName);
	void importDataBase();
	void writeLogFile();

private:
    Ui::ControlManagerMainWindow *ui;
    QString mConnectionName;
	int itemsCount;
	QColor mLabelColor;
	QPropertyAnimation mLabelColorAnimation;

	Q_PROPERTY(QColor labelColor READ labelColor WRITE setLabelColor)
};

//inline implementation:

inline QColor ControlManagerMainWindow::labelColor() const
{
	return mLabelColor;
}

#endif // CONTROLMANAGERMAINWINDOW_H
