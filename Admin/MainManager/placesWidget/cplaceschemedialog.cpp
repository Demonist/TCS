#include "cplaceschemedialog.h"
#include "ui_cplaceschemedialog.h"

//private:

bool CPlaceSchemeDialog::eventFilter(QObject *obj, QEvent *event)
{
	if(obj != ui->gvScheme->viewport())
		return false;

	static QGraphicsItem *movingItem = 0;
	static QPointF offset(0, 0);

	static bool dragStarted = false;
	static QPoint lastDragPos(0, 0);

	if(event->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

		if(mouseEvent->button() == Qt::LeftButton)
		{
			if(mEditType == Add)
			{
				CSeatItem *item = new CSeatItem();
				item->setPos((QPointF)(ui->gvScheme->mapToScene(mouseEvent->pos())).toPoint());

				mScene.addItem(item);
				item->showAnimated(500);

				mSeatsCount++;
				updateSeatsCountText();
			}
			else if(mEditType == Delete)
			{
				QGraphicsItem *item = mScene.itemAt(ui->gvScheme->mapToScene(mouseEvent->pos()));
				if(item && item->data(0).toString() == CSeatItem::itemName())
				{
					CSeatItem *seat = (CSeatItem*)(item);
					mDeletedIds.append(seat->id());
					seat->hideAnimated(700);
					mScene.removeItem(item);
					mSeatsCount--;
					updateSeatsCountText();
				}
			}
			else if(mEditType == Move && movingItem)
			{
				movingItem->setSelected(false);
				movingItem = 0;
				ui->gvScheme->setCursor(Qt::ArrowCursor);
			}
			else if(mEditType == Drag)
			{
				lastDragPos = QPoint(0, 0);
				dragStarted = false;
				ui->gvScheme->setCursor(Qt::OpenHandCursor);
			}
			else if(mEditType == Select)
			{
				CSeatItem *item = (CSeatItem*)mScene.itemAt(ui->gvScheme->mapToScene(mouseEvent->pos()));
				if(item)
				{
					if(mSelectedItem == 0)
					{
						mSelectedItem = item;
						mSelectedItem->setSelectedAnimated(true);
						ui->leSeatNumber->setText(mSelectedItem->text());
						ui->leSeatNumber->setFocus();
					}
					else if(mSelectedItem != item)
					{
						mSelectedItem->setSelectedAnimated(false);
						mSelectedItem = item;
						mSelectedItem->setSelectedAnimated(true);
						ui->leSeatNumber->setText(mSelectedItem->text());
						ui->leSeatNumber->setFocus();
					}
				}
				else if(mSelectedItem)
				{
					mSelectedItem->setSelectedAnimated(false);
					ui->leSeatNumber->clear();
					mSelectedItem = 0;
				}
			}
		}
	}
	else if(event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

		if(obj == ui->gvScheme->viewport() && mouseEvent->button() == Qt::LeftButton)
		{
			QPointF pos = ui->gvScheme->mapToScene(mouseEvent->pos());
			if(mEditType == Move)
			{
				QGraphicsItem *item = mScene.itemAt(pos);
				if(item)
				{
					offset = item->pos() - pos;
					item->setSelected(true);

					movingItem = item;
					ui->gvScheme->setCursor(Qt::SizeAllCursor);
				}
			}
			else if(mEditType == Drag)
			{
				lastDragPos = mouseEvent->pos();
				dragStarted = true;
				ui->gvScheme->setCursor(Qt::ClosedHandCursor);
			}
		}
	}
	else if(event->type() == QEvent::MouseMove)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

		if(mEditType == Move)
		{
			if(movingItem)
			{
				movingItem->setPos((QPointF)(ui->gvScheme->mapToScene(mouseEvent->pos()) + offset).toPoint());
				QRectF rectLeft(movingItem->pos().toPoint(), QSize(-1020, 1));
				QList<QGraphicsItem*> leftItems = mScene.items(rectLeft, Qt::IntersectsItemBoundingRect);
				leftItems.removeOne(movingItem);
				for(int i = 0; i < leftItems.count(); i++)
					if((int)leftItems[i]->y() != (int)movingItem->y())
						leftItems.removeAt(i--);
//				setWindowTitle(tr("%1").arg(leftItems.count()));
			}
			else
			{
				static CSeatItem *itemAtCursor = 0;
				CSeatItem *item = (CSeatItem*)mScene.itemAt(ui->gvScheme->mapToScene(mouseEvent->pos()));
				if(item)
				{
					if(item->data(0).toString() == CSeatItem::itemName())
					{
						if(itemAtCursor && itemAtCursor != item)
						{
							itemAtCursor->setBorderColorAnimated(QColor(70, 70, 70));
							itemAtCursor = 0;
						}
						if(itemAtCursor == 0)
						{
							itemAtCursor = item;
							itemAtCursor->setBorderColorAnimated(QColor(100, 200, 50));
						}
					}
				}
				else if(itemAtCursor)
				{
					itemAtCursor->setBorderColorAnimated(QColor(70, 70, 70));
					itemAtCursor = 0;
				}
			}
		}
		else if(mAddItem && mEditType == Add)
		{
			mAddItem->setPos((QPointF)(ui->gvScheme->mapToScene(mouseEvent->pos())).toPoint());
			QList<QGraphicsItem*> collidingItems = mScene.collidingItems(mAddItem, Qt::IntersectsItemBoundingRect);
			if(collidingItems.isEmpty())
				mAddItem->show();
			else
				mAddItem->hide();
		}
		else if(mEditType == Delete)
		{
			static CSeatItem *itemAtCursor = 0;
			CSeatItem *item = (CSeatItem*)mScene.itemAt(ui->gvScheme->mapToScene(mouseEvent->pos()));
			if(item)
			{
				if(item->data(0).toString() == CSeatItem::itemName())
				{
					if(itemAtCursor && itemAtCursor != item)
					{
						itemAtCursor->setBorderColorAnimated(QColor(70, 70, 70));
						itemAtCursor = 0;
					}
					if(itemAtCursor == 0)
					{
						itemAtCursor = item;
						itemAtCursor->setBorderColorAnimated(QColor(200, 100, 100));
					}
				}
			}
			else if(itemAtCursor)
			{
				itemAtCursor->setBorderColorAnimated(QColor(70, 70, 70));
				itemAtCursor = 0;
			}
		}
		else if(mEditType == Drag && dragStarted == true)
		{
			QPoint pos = mouseEvent->pos();
			QPoint scroll = lastDragPos - pos;
			ui->gvScheme->horizontalScrollBar()->setValue(ui->gvScheme->horizontalScrollBar()->value() + scroll.x());
			ui->gvScheme->verticalScrollBar()->setValue(ui->gvScheme->verticalScrollBar()->value() + scroll.y());
			lastDragPos = pos;

		}
		else if(mEditType == Select)
		{
			static CSeatItem *hoveredItem = 0;
			CSeatItem *item = (CSeatItem*)mScene.itemAt(ui->gvScheme->mapToScene(mouseEvent->pos()));
			if(item)
			{
				if(hoveredItem == 0)
				{
					hoveredItem = item;
					hoveredItem->setHoveredAnimated(true);
				}
				else if(hoveredItem != item)
				{
					hoveredItem->setHoveredAnimated(false);
					hoveredItem = item;
					hoveredItem->setHoveredAnimated(true);
				}
			}
			else if(hoveredItem)
			{
				hoveredItem->setHoveredAnimated(false);
				hoveredItem = 0;
			}
		}
	}
	else if(event->type() == QEvent::Leave && mAddItem && mEditType == Add)
	{
		mAddItem->hide();
		dragStarted = false;
	}

	return false;
}

void CPlaceSchemeDialog::updateSeatsCountText()
{
	ui->lTotalSeatCount->setText(tr("Максимальное число мест в зале: %1").arg(mSeatsCount));
}

void CPlaceSchemeDialog::updateScaleText()
{
	ui->gbxScale->setTitle(tr("Вид [%1%]").arg(mScalePersent));
}

//public:

CPlaceSchemeDialog::CPlaceSchemeDialog(const QString &connectionName, const int placeId, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CPlaceSchemeDialog)
{
	ui->setupUi(this);
	ui->gvScheme->setMouseTracking(true);
	ui->hblMain->setStretch(0, 100);
	ui->hblMain->setStretch(1, 1);

	setWindowState(windowState() | Qt::WindowMaximized);

	mEditType = None;
	mScalePersent = 100;
	mSeatsCount = 0;
	mSelectedItem = 0;

	mConnectionName = connectionName;
	mId = placeId;

	mScene.setSceneRect(0.0, 0.0, 1020.0, 730.0);
	ui->gvScheme->setScene(&mScene);
	ui->gvScheme->viewport()->installEventFilter(this);

	mAddItem = new CSeatItem();
	mAddItem->setData(0, "addItem");
	mAddItem->hide();
	mAddItem->setOpacity(0.4);
	mScene.addItem(mAddItem);

	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	query.prepare("SELECT title FROM Places WHERE id = :id;");
	query.bindValue(":id", mId);
	if(query.exec() && query.first())
		ui->lPlaceName->setText(query.value(0).toString());

	query.prepare("SELECT id FROM PlaceSchemes WHERE id_place = :id;");
	query.bindValue(":id", mId);
	mSeatsCount = 0;
	if(query.exec())
		while(query.next())
		{
			CSeatItem *item = new CSeatItem(mConnectionName, query.value(0).toInt());
			mScene.addItem(item);
			item->showAnimated(2000);
			mSeatsCount++;
		}
	updateSeatsCountText();
}

CPlaceSchemeDialog::~CPlaceSchemeDialog()
{
	delete ui;
}

void CPlaceSchemeDialog::on_pbnCancel_clicked()
{
	close();
}

void CPlaceSchemeDialog::on_pbnApply_clicked()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	QList<QGraphicsItem*> items = mScene.items();
	for(int i = 0; i < items.count(); i++)
	{
		if(items[i]->data(0).toString() == CSeatItem::itemName())
		{
			CSeatItem *item = static_cast<CSeatItem*>(items[i]);
			if(item->needSave())
			{
				if(item->id() == 0)
				{
					query.prepare("INSERT INTO PlaceSchemes VALUES(NULL, :number, :x, :y, :placeId);");
					query.bindValue(":number", item->text());
					query.bindValue(":x", item->x());
					query.bindValue(":y", item->y());
					query.bindValue(":placeId", mId);
					query.exec();
				}
				else
				{
					query.prepare("UPDATE PlaceSchemes SET seatNumber = :number, x = :x, y = :y WHERE id = :id;");
					query.bindValue(":number", item->text());
					query.bindValue(":x", item->x());
					query.bindValue(":y", item->y());
					query.bindValue(":id", item->id());
					query.exec();
				}
			}
		}
	}

	for(int i = 0; i < mDeletedIds.count(); i++)
		if(mDeletedIds[i] > 0)
		{
			query.prepare("DELETE FROM PlaceSchemes WHERE id = :id;");
			query.bindValue(":id", mDeletedIds[i]);
			query.exec();
		}

	close();
}

void CPlaceSchemeDialog::on_tbnZoomIn_clicked()
{
	ui->gvScheme->scale(1.2, 1.2);
	mScalePersent += 20;
	updateScaleText();
}

void CPlaceSchemeDialog::on_tbnZoomOut_clicked()
{
	if(mScalePersent > 20)
	{
		ui->gvScheme->scale(0.8, 0.8);
		mScalePersent -= 20;
		updateScaleText();
	}
}

void CPlaceSchemeDialog::on_tbnZoomDefault_clicked()
{
	ui->gvScheme->resetMatrix();
	mScalePersent = 100;
	updateScaleText();
}

void CPlaceSchemeDialog::on_tbnSelect_clicked(bool checked)
{
	if(checked)
	{
		ui->tbnMove->setChecked(false);
		ui->tbnAdd->setChecked(false);
		ui->tbnDel->setChecked(false);
		ui->tbnDrag->setChecked(false);
		ui->gbxNumber->setEnabled(true);
		mEditType = Select;
	}
	else
	{
		mEditType = None;
		ui->gbxNumber->setEnabled(false);
	}
}

void CPlaceSchemeDialog::on_tbnMove_clicked(bool checked)
{
	if(checked)
	{
		ui->tbnSelect->setChecked(false);
		ui->tbnAdd->setChecked(false);
		ui->tbnDel->setChecked(false);
		ui->tbnDrag->setChecked(false);
		mEditType = Move;
	}
	else
		mEditType = None;
}

void CPlaceSchemeDialog::on_tbnAdd_clicked(bool checked)
{
	if(checked)
	{
		ui->tbnSelect->setChecked(false);
		ui->tbnMove->setChecked(false);
		ui->tbnDel->setChecked(false);
		ui->tbnDrag->setChecked(false);
		mEditType = Add;
	}
	else
		mEditType = None;
}

void CPlaceSchemeDialog::on_tbnDel_clicked(bool checked)
{
	if(checked)
	{
		ui->tbnSelect->setChecked(false);
		ui->tbnAdd->setChecked(false);
		ui->tbnMove->setChecked(false);
		ui->tbnDrag->setChecked(false);
		mEditType = Delete;
	}
	else
		mEditType = None;
}

void CPlaceSchemeDialog::on_tbnDrag_clicked(bool checked)
{
	if(checked)
	{
		ui->tbnSelect->setChecked(false);
		ui->tbnAdd->setChecked(false);
		ui->tbnMove->setChecked(false);
		ui->tbnDel->setChecked(false);
		mEditType = Drag;
	}
	else
		mEditType = None;
}

void CPlaceSchemeDialog::on_leSeatNumber_textEdited(const QString &arg1)
{
	if(mSelectedItem)
		mSelectedItem->setText(arg1);
}

void CPlaceSchemeDialog::on_tbnSelect_toggled(bool checked)
{
	if(!checked)
	{
		ui->gbxNumber->setEnabled(false);
		if(mSelectedItem)
		{
			mSelectedItem->setSelectedAnimated(false);
			mSelectedItem = 0;
		}
	}
}

void CPlaceSchemeDialog::on_tbnDrag_toggled(bool checked)
{
	if(checked)
		ui->gvScheme->setCursor(Qt::OpenHandCursor);
	else
		ui->gvScheme->setCursor(Qt::ArrowCursor);
}
