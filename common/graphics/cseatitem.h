#ifndef CSEATITEM_H
#define CSEATITEM_H

#include <QtGui>
#include <QtSql>
#include "cabstractgraphicsitem.h"
#include "parallel.h"

class CSeatItem : public CAbstractGraphicsItem
{
    Q_OBJECT

	//members:
protected:
	QString mConnectionName;
	int mId;

	bool mHovered;
	bool mSelected;

	QString mText;
	QString mRow;

	//save:
	QString mInitText;
	QString mInitRow;
	QPoint mInitPoint;

	//methods:
protected:
	void init();
	int fontSizeForText(const QString &text) const;

public:
	explicit CSeatItem();
	explicit CSeatItem(const QString &connectionName, const int seatId);
	virtual ~CSeatItem();

	virtual QRectF boundingRect() const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	virtual bool needSave() const;

	void setHovered(const bool hovered);
	void setHoveredAnimated(const bool hovered, const int durationMs = 300);

	void setSelected(const bool selected);
	void setSelectedAnimated(const bool selected, const int durationMs = 300);

	void hideAnimated(const int durationMs = 300);
	void showAnimated(const int durationMs = 300);

	static const char* itemName();

	inline bool isHovered() const;
	inline bool isSelected() const;

	inline int id() const;

	inline QString text() const;
	inline void setText(const QString &text);

	inline QString row() const;
	inline void setRow(const QString &row);
};

//inline implementation:

inline bool CSeatItem::isHovered() const
{
	return mHovered;
}

inline bool CSeatItem::isSelected() const
{
	return mSelected;
}

inline int CSeatItem::id() const
{
	return mId;
}

inline QString CSeatItem::text() const
{
	return mText;
}
inline void CSeatItem::setText(const QString &text)
{
	mText = text;
	update();
}

inline QString CSeatItem::row() const
{
	return mRow;
}
inline void CSeatItem::setRow(const QString &row)
{
	mRow = row;
}

#endif // CSEATITEM_H
