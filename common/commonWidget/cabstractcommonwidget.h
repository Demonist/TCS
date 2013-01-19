#ifndef CABSTRACTCOMMONWIDGET_H
#define CABSTRACTCOMMONWIDGET_H

#include <QWidget>

class CAbstractCommonWidget : public QWidget
{
	Q_OBJECT

protected:
	QString mConnectionName;

public:
	explicit CAbstractCommonWidget(QWidget *parent = 0);
	virtual ~CAbstractCommonWidget() = 0;

	inline QString connectionName() const;

public slots:
	inline void setConnectionName(const QString &connectionName);
	virtual void updateData() = 0;
};

inline void CAbstractCommonWidget::setConnectionName(const QString &connectionName)
{
	mConnectionName = connectionName;
}

inline QString CAbstractCommonWidget::connectionName() const
{
	return mConnectionName;
}
#endif // CABSTRACTCOMMONWIDGET_H
