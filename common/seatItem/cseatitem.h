#ifndef CSEATITEM_H
#define CSEATITEM_H

#include <QtGui>
#include <QtSql>
#include "parallel.h"

class CSeatItem : public QGraphicsObject
{
    Q_OBJECT

	//members:
protected:
	QString mConnectionName;
	int mId;

	bool mHovered;
	bool mSelected;

	QString mText;
	QColor mBrush;
	QColor mBorderColor;
	int mPenWidth;
	QGraphicsBlurEffect mBlurEffect;

	Q_PROPERTY(qreal scale READ scale WRITE setScale)
	Q_PROPERTY(QColor brush READ brush WRITE setBrush)
	Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
	Q_PROPERTY(int penWidth READ penWidth WRITE setPenWidth)
	Q_PROPERTY(qreal blurRadius READ blurRadius WRITE setBlurRadius)

	QPropertyAnimation mScaleAnimation;
	QPropertyAnimation mBrushAnimation;
	QPropertyAnimation mBorderColorAnimation;
	QPropertyAnimation mOpacityAnimation;
	QPropertyAnimation mPenWidthAnimation;
	QPropertyAnimation mBlurAnimation;

	//save:
	QString mInitText;
	QPoint mInitPoint;

	//methods:
protected:
	void init();
	void animationInit();
	int fontSizeForText(const QString &text) const;

public:
	explicit CSeatItem();
	explicit CSeatItem(const QString &connectionName, const int seatId);
	virtual ~CSeatItem();

	virtual QRectF boundingRect() const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	virtual bool needSave() const;

	void setScaleAnimated(const qreal newScale, const int durationMs = 300);
	void setBrushAnimated(const QColor &newBrushColor, const int durationMs = 300);
	void setBorderColorAnimated(const QColor &newBorderColor, const int durationMs = 300);
	void setOpacityAnimated(const qreal newOpacity, const int durationMs = 300);
	void setPenWidthAnimated(const int newPenWidth, const int durationMs = 300);
	void setBlurAnimated(const qreal newBlurRadius, const int durationMs = 300);

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

	inline QColor brush() const;
	inline void setBrush(const QColor &brush);

	inline QColor borderColor() const;
	inline void setBorderColor(const QColor &newBorderColor);

	inline int penWidth() const;
	inline void setPenWidth(const int penWidth);

	inline qreal blurRadius() const;
	inline void setBlurRadius(const qreal &blurRadius);

protected slots:
	inline virtual void update_slot();
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

inline QColor CSeatItem::brush() const
{
	return mBrush;
}

inline void CSeatItem::setBrush(const QColor &brush)
{
	mBrush = brush;
}

inline QColor CSeatItem::borderColor() const
{
	return mBorderColor;
}

inline void CSeatItem::setBorderColor(const QColor &newBorderColor)
{
	mBorderColor = newBorderColor;
}

inline int CSeatItem::penWidth() const
{
	return mPenWidth;
}

inline void CSeatItem::setPenWidth(const int penWidth)
{
	mPenWidth = penWidth;
}

inline qreal CSeatItem::blurRadius() const
{
	return mBlurEffect.blurRadius();
}

inline void CSeatItem::setBlurRadius(const qreal &blurRadius)
{
	mBlurEffect.setBlurRadius(blurRadius);
}

//protected slots:

inline void CSeatItem::update_slot()
{
	update();
}

#endif // CSEATITEM_H
