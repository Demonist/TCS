#ifndef CGRAPHICSVIEW_H
#define CGRAPHICSVIEW_H

#include <QtGui>
#include "cabstractlegend.h"

class CGraphicsView : public QGraphicsView
{
	Q_OBJECT

	Q_PROPERTY(qreal scale READ scale WRITE setScale)

private:
	bool mDragging;
	QPoint mDragPos;

protected:
	bool mDrag;
	bool mWheelScalling;	//! Флаг, разрешающий изменение масштаба роликом мыши. По умолчанию равен \a true. Если равен \a false, то сингналы \a wheelUp и \a wheelDown не посылаются.
	qreal mScale;
	QPropertyAnimation mScaleAnimation;
	CAbstractLegend *mLegend;

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

public:
	explicit CGraphicsView(QWidget *parent = 0);
	virtual ~CGraphicsView();
	
	inline bool dragEnabled() const;
	inline bool wheelScalling() const;
	inline qreal scale() const;
	inline CAbstractLegend* legend() const;
	inline bool isLegend() const;
	
signals:
	void wheelUp();
	void wheelDown();

public slots:
	void setDragEnabled(const bool enableDrag);
	inline void setWheelScalling(const bool enalbeWheelScaling);
	void setScale(const qreal scale);
	void setScaleAnimated(const qreal scale, const int durationMs = 750);
	inline void setLegend(CAbstractLegend *legend);
};

//inline implementation:

inline bool CGraphicsView::dragEnabled() const
{
	return mDrag;
}

inline qreal CGraphicsView::scale() const
{
	return mScale;
}

inline CAbstractLegend* CGraphicsView::legend() const
{
	return mLegend;
}
/**
Устанавливает класс для рисования легенды.
Если \param legend равен 0, то легенда не отображается.
*/
inline void CGraphicsView::setLegend(CAbstractLegend *legend)
{
	mLegend = legend;
	viewport()->repaint();
}
inline bool CGraphicsView::isLegend() const
{
	return mLegend != 0;
}

inline bool CGraphicsView::wheelScalling() const
{
	return mWheelScalling;
}
inline void CGraphicsView::setWheelScalling(const bool enalbeWheelScaling)
{
	mWheelScalling = enalbeWheelScaling;
}

#endif // CGRAPHICSVIEW_H
