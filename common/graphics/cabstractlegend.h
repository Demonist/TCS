#ifndef CABSTRACTLEGEND_H
#define CABSTRACTLEGEND_H

#include <QtGui>

/**
Абстрактный класс для рисования легенды на \class CGraphicsView.

\author Demonist
*/
class CAbstractLegend
{
public:
	explicit CAbstractLegend();
	virtual ~CAbstractLegend() = 0;

	/**
	Данный методы вызывается классом \class CGraphicsView для заданного в нем CAbstractLegend объекта.
	*/
	virtual void paintLenend(QPainter *painter, const QSize &viewSize) = 0;
};

#endif // CABSTRACTLEGEND_H
