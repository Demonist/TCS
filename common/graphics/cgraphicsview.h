#ifndef CGRAPHICSVIEW_H
#define CGRAPHICSVIEW_H

#include <QtGui>
#include "cabstractlegend.h"

class CGraphicsView : public QGraphicsView
{
	Q_OBJECT

	Q_PROPERTY(qreal scale READ scale WRITE setScale)

	//members:
protected:
	bool mDragging;
	QPoint mDragPos;

	bool mDrag;
	bool mDragByRightButton;
	bool mWheelScalling;	//! Флаг, разрешающий изменение масштаба роликом мыши. По умолчанию равен \a true. Если равен \a false, то сингналы \a wheelUp и \a wheelDown не посылаются.
	bool mWheelScallingAnimated;
	qreal mScale;
	QPropertyAnimation mScaleAnimation;
	CAbstractLegend *mLegend;

	//methods:
protected:
	virtual qreal roundScale(const qreal &scale) const;

	virtual void paintEvent(QPaintEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);

public:
	explicit CGraphicsView(QWidget *parent = 0);
	virtual ~CGraphicsView();

	void scaleUp();
	void scaleDown();
	
	inline bool dragEnabled() const;
	inline bool dragByRightButtonEnabled() const;
	inline bool wheelScalling() const;
	inline bool isWheelScallingAnimated() const;
	inline qreal scale() const;
	inline CAbstractLegend* legend() const;
	inline bool isLegend() const;
	
protected slots:
	virtual void scrollBarValueChanged(const int newValue);

public slots:
	void setDragEnabled(const bool enableDrag);
	void setDragByRightButtonEnabled(const bool enableDrag);
	inline void setWheelScalling(const bool enalbeWheelScaling);
	inline void setWheelScallingAnimated(const bool animatedScalling = true);
	virtual void setScale(const qreal scale);
	void setScaleAnimated(const qreal scale, const int durationMs = 750);
	inline void setLegend(CAbstractLegend *legend);
	inline void fitScene();
};

//inline implementation:

inline bool CGraphicsView::dragEnabled() const
{
	return mDrag;
}

inline bool CGraphicsView::dragByRightButtonEnabled() const
{
	return mDragByRightButton;
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

inline bool CGraphicsView::isWheelScallingAnimated() const
{
	return mWheelScallingAnimated;
}
inline void CGraphicsView::setWheelScallingAnimated(const bool animatedScalling)
{
	mWheelScallingAnimated = animatedScalling;
}

inline void CGraphicsView::fitScene()
{
	if(scene())
		fitInView(scene()->sceneRect(), Qt::KeepAspectRatioByExpanding);
}
#endif // CGRAPHICSVIEW_H
