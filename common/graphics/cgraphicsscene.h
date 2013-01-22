#ifndef CGRAPHICSSCENE_H
#define CGRAPHICSSCENE_H

#include <QtGui>

class CGraphicsScene : public QGraphicsScene
{
	Q_OBJECT

protected:
	QPixmap mImage;			//! Подложка.
	QPoint mImageShear;		//! Смещение подложки относительно начала координат.
	bool mDrawAxis;			//! Отображать ли оси?!
	bool mDrawBackground;	//! Отображать ли подложку?!

protected:
	void drawBackground(QPainter *painter, const QRectF &rect);

public:
	explicit CGraphicsScene(QObject *parent = 0);
	
	inline QPixmap backgroundImage() const;
	inline QPoint backgroundImageShear() const;
	inline bool isDrawAxis() const;
	inline bool isDrawBackground() const;

public slots:
	inline void setBackgroundImage(const QPixmap &backgroundImage);
	inline void setBackgroundImageShear(const QPoint &shear);
	inline void setDrawAxis(const bool isDrawAxis);
	inline void setDrawBackground(const bool draw);
};

//inline implementation:

inline QPixmap CGraphicsScene::backgroundImage() const
{
	return mImage;
}
inline void CGraphicsScene::setBackgroundImage(const QPixmap &backgroundImage)
{
	mImage = backgroundImage;
	update();
}

inline QPoint CGraphicsScene::backgroundImageShear() const
{
	return mImageShear;
}
inline void CGraphicsScene::setBackgroundImageShear(const QPoint &shear)
{
	mImageShear = shear;
	update();
}

inline bool CGraphicsScene::isDrawAxis() const
{
	return mDrawAxis;
}
inline void CGraphicsScene::setDrawAxis(const bool drawAxis)
{
	mDrawAxis = drawAxis;
	update();
}

inline bool CGraphicsScene::isDrawBackground() const
{
	return mDrawBackground;
}
inline void CGraphicsScene::setDrawBackground(const bool draw)
{
	mDrawBackground = draw;
	update();
}

#endif // CGRAPHICSSCENE_H
