#ifndef CABSTRACTGRAPHICSITEM_H
#define CABSTRACTGRAPHICSITEM_H

#include <QtGui>

/**
Абстрактный класс графического элемента сцены с эффектами анимации.
Анимированы: масштаб, прозрачность, цвет кисти, цвет пера, ширина пера, радиус размытия.

\author Demonist
*/
class CAbstractGraphicsItem : public QGraphicsObject
{
	Q_OBJECT

	//members:
protected:
	QColor mBrushColor;
	QColor mPenColor;
	int mPenWidth;
	QGraphicsBlurEffect mBlurEffect;

	Q_PROPERTY(qreal scale READ scale WRITE setScale)
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
	Q_PROPERTY(QColor brushColor READ brushColor WRITE setBrushColor)
	Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor)
	Q_PROPERTY(int penWidth READ penWidth WRITE setPenWidth)
	Q_PROPERTY(qreal blurRadius READ blurRadius WRITE setBlurRadius)

	QPropertyAnimation mScaleAnimation;
	QPropertyAnimation mBrushColorAnimation;
	QPropertyAnimation mPenColorAnimation;
	QPropertyAnimation mOpacityAnimation;
	QPropertyAnimation mPenWidthAnimation;
	QPropertyAnimation mBlurAnimation;

	//methods:
public:
	explicit CAbstractGraphicsItem();
	virtual ~CAbstractGraphicsItem() = 0;
	
	void setScaleAnimated(const qreal newScale, const int durationMs = 300);
	void setOpacityAnimated(const qreal newOpacity, const int durationMs = 300);
	void setBrushColorAnimated(const QColor &newBrushColor, const int durationMs = 300);
	void setPenColorAnimated(const QColor &newPenColor, const int durationMs = 300);
	void setPenWidthAnimated(const int newPenWidth, const int durationMs = 300);
	void setBlurAnimated(const qreal newBlurRadius, const int durationMs = 300);

	inline QColor brushColor() const;
	inline void setBrushColor(const QColor &newBrushColor);

	inline QColor penColor() const;
	inline void setPenColor(const QColor &newPenColor);

	inline int penWidth() const;
	inline void setPenWidth(const int newPenWidth);

	inline qreal blurRadius() const;
	inline void setBlurRadius(const qreal &newBlurRadius);
};

//inline implementation:

inline QColor CAbstractGraphicsItem::brushColor() const
{
	return mBrushColor;
}
inline void CAbstractGraphicsItem::setBrushColor(const QColor &newBrushColor)
{
	mBrushColor = newBrushColor;
	update();
}

inline QColor CAbstractGraphicsItem::penColor() const
{
	return mPenColor;
}
inline void CAbstractGraphicsItem::setPenColor(const QColor &newPenColor)
{
	mPenColor = newPenColor;
	update();
}

inline int CAbstractGraphicsItem::penWidth() const
{
	return mPenWidth;
}
inline void CAbstractGraphicsItem::setPenWidth(const int newPenWidth)
{
	mPenWidth = newPenWidth;
	update();
}

inline qreal CAbstractGraphicsItem::blurRadius() const
{
	return mBlurEffect.blurRadius();
}
inline void CAbstractGraphicsItem::setBlurRadius(const qreal &newBlurRadius)
{
	mBlurEffect.setBlurRadius(newBlurRadius);
}

#endif // CABSTRACTGRAPHICSITEM_H
