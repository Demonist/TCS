#include "cplaceschemedialog.h"
#include "ui_cplaceschemedialog.h"

//private:

QPoint CPlaceSchemeDialog::toGrid(const QPoint &pos)
{
	const int gridStep = 10;
	QPoint newPos;
	newPos.setX(pos.x() - pos.x() % gridStep);
	newPos.setY(pos.y() - pos.y() % gridStep);
	return newPos;
}

void CPlaceSchemeDialog::drawDistances(const QGraphicsItem &item)
{
	const int maxDistance = 500;
	const int minDistance = 30;

	QGraphicsItem *left = 0,
			*up = 0,
			*right = 0,
			*down = 0;
	QGraphicsItem *leftLeft = 0,
			*upUp = 0,
			*rightRight = 0,
			*downDown = 0;
	QPoint pos;

	//Demonist: Приношу свои извинения за следующую неводомую адовую хвень...

	pos.setY(item.y());
	for(int dx = 1; dx < maxDistance*2; dx++)
	{
		if(!left)
		{
			if(dx < maxDistance)
			{
				pos.setX(item.x() - dx);
				left = mScene.itemAt(pos);
				if(left)
					if(left->data(0).toString() != CSeatItem::itemName()
					   || left == &item
					   || pos != left->pos())
						left = 0;
			}
			else
				if(!right)
					break;
		}
		else if(!leftLeft)
		{
			pos.setX(item.x() - dx);
			leftLeft = mScene.itemAt(pos);
			if(leftLeft)
				if(leftLeft->data(0).toString() != CSeatItem::itemName()
				   || leftLeft == left
				   || pos != leftLeft->pos())
					leftLeft = 0;
		}
		else if(rightRight)
			break;

		if(!right)
		{
			if(dx < maxDistance)
			{
				pos.setX(item.x() + dx);
				right = mScene.itemAt(pos);
				if(right)
					if(right->data(0).toString() != CSeatItem::itemName()
					   || right == &item
					   || pos != right->pos())
						right = 0;
			}
		}
		else if(!rightRight)
		{
			pos.setX(item.x() + dx);
			rightRight = mScene.itemAt(pos);
			if(rightRight)
				if(rightRight->data(0).toString() != CSeatItem::itemName()
				   || rightRight == right
				   || pos != rightRight->pos())
					rightRight = 0;
		}
	}

	pos.setX(item.x());
	for(int dy = 1; dy < maxDistance*2; dy++)
	{
		if(!up)
		{
			if(dy < maxDistance)
			{
				pos.setY(item.y() - dy);
				up = mScene.itemAt(pos);
				if(up)
					if(up->data(0).toString() != CSeatItem::itemName()
					   || up == &item
					   || pos != up->pos())
						up = 0;
			}
			else if(!down)
				break;
		}
		else if(!upUp)
		{
			pos.setY(item.y() - dy);
			upUp = mScene.itemAt(pos);
			if(upUp)
				if(upUp->data(0).toString() != CSeatItem::itemName()
				   || upUp == up
				   || pos != upUp->pos())
					upUp = 0;
		}
		else if(downDown)
			break;

		if(!down)
		{
			if(dy < maxDistance)
			{
				pos.setY(item.y() + dy);
				down = mScene.itemAt(pos);
				if(down)
					if(down->data(0).toString() != CSeatItem::itemName()
					   || down == &item
					   || pos != down->pos())
						down = 0;
			}
		}
		else if(!downDown)
		{
			pos.setY(item.y() + dy);
			downDown = mScene.itemAt(pos);
			if(downDown)
				if(downDown->data(0).toString() != CSeatItem::itemName()
				   || downDown == down
				   || pos != downDown->pos())
					downDown = 0;
		}
	}

	if(left && item.x() - left->x() >= minDistance)
	{
		mDistanceLeftItem->setPos(item.pos());
		mDistanceLeftItem->setTargetPos(left->pos() - item.pos());
		mDistanceLeftItem->show();

		if(leftLeft && left->x() - leftLeft->x() >= minDistance)
		{
			mDistanceLeftLeftItem->setPos(left->pos());
			mDistanceLeftLeftItem->setTargetPos(leftLeft->pos() - left->pos());
			mDistanceLeftLeftItem->show();
		}
		else
			mDistanceLeftLeftItem->hide();
	}
	else
	{
		mDistanceLeftItem->hide();
		mDistanceLeftLeftItem->hide();
	}

	if(up && item.y() - up->y() >= minDistance)
	{
		mDistanceUpItem->setPos(item.pos());
		mDistanceUpItem->setTargetPos(up->pos() - item.pos());
		mDistanceUpItem->show();

		if(upUp && up->y() - upUp->y() >= minDistance)
		{
			mDistanceUpUpItem->setPos(up->pos());
			mDistanceUpUpItem->setTargetPos(upUp->pos() - up->pos());
			mDistanceUpUpItem->show();
		}
		else
			mDistanceUpUpItem->hide();
	}
	else
	{
		mDistanceUpItem->hide();
		mDistanceUpUpItem->hide();
	}

	if(right && right->x() - item.x() >= minDistance)
	{
		mDistanceRightItem->setPos(item.pos());
		mDistanceRightItem->setTargetPos(right->pos() - item.pos());
		mDistanceRightItem->show();

		if(rightRight && rightRight->x() - right->x() >= minDistance)
		{
			mDistanceRightRightItem->setPos(right->pos());
			mDistanceRightRightItem->setTargetPos(rightRight->pos() - right->pos());
			mDistanceRightRightItem->show();
		}
		else
			mDistanceRightRightItem->hide();
	}
	else
	{
		mDistanceRightItem->hide();
		mDistanceRightRightItem->hide();
	}

	if(down && down->y() - item.y() >= minDistance)
	{
		mDistanceDownItem->setPos(item.pos());
		mDistanceDownItem->setTargetPos(down->pos() - item.pos());
		mDistanceDownItem->show();

		if(downDown && downDown->y() - down->y() >= minDistance)
		{
			mDistanceDownDownItem->setPos(down->pos());
			mDistanceDownDownItem->setTargetPos(downDown->pos() - down->pos());
			mDistanceDownDownItem->show();
		}
		else
			mDistanceDownDownItem->hide();
	}
	else
	{
		mDistanceDownItem->hide();
		mDistanceDownDownItem->hide();
	}
}

void CPlaceSchemeDialog::hideDistances()
{
	mDistanceLeftItem->hide();
	mDistanceUpItem->hide();
	mDistanceRightItem->hide();
	mDistanceDownItem->hide();
	mDistanceLeftLeftItem->hide();
	mDistanceUpUpItem->hide();
	mDistanceRightRightItem->hide();
	mDistanceDownDownItem->hide();
}

bool CPlaceSchemeDialog::eventFilter(QObject *obj, QEvent *event)
{
	if(obj != ui->gvScheme->viewport())
		return false;

	static QGraphicsItem *movingItem = 0;
	static QPointF offset(0, 0);

	if(event->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

		if(mouseEvent->button() == Qt::LeftButton)
		{
			if(mEditType == Add)
			{
				CSeatItem *item = new CSeatItem();
				item->setPos(toGrid(ui->gvScheme->mapToScene(mouseEvent->pos())));

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

				hideDistances();
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
						ui->leRow->setText(mSelectedItem->row());
						ui->leSeatNumber->setFocus();
					}
					else if(mSelectedItem != item)
					{
						mSelectedItem->setSelectedAnimated(false);
						mSelectedItem = item;
						mSelectedItem->setSelectedAnimated(true);
						ui->leSeatNumber->setText(mSelectedItem->text());
						ui->leRow->setText(mSelectedItem->row());
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

		if(mouseEvent->button() == Qt::LeftButton)
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

					drawDistances(*movingItem);
				}
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
				movingItem->setPos(toGrid(ui->gvScheme->mapToScene(mouseEvent->pos()) + offset));
				drawDistances(*movingItem);
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
							itemAtCursor->setPenColorAnimated(QColor(70, 70, 70));
							itemAtCursor = 0;
						}
						if(itemAtCursor == 0)
						{
							itemAtCursor = item;
							itemAtCursor->setPenColorAnimated(QColor(100, 200, 50));
						}
					}
				}
				else if(itemAtCursor)
				{
					itemAtCursor->setPenColorAnimated(QColor(70, 70, 70));
					itemAtCursor = 0;
				}
			}
		}
		else if(mAddItem && mEditType == Add)
		{
			mAddItem->setPos(toGrid(ui->gvScheme->mapToScene(mouseEvent->pos())));
			QList<QGraphicsItem*> collidingItems = mScene.collidingItems(mAddItem, Qt::IntersectsItemBoundingRect);
			for(int i = 0; i < collidingItems.count(); i++)
				if(collidingItems[i]->data(0).toString() != CSeatItem::itemName())
					collidingItems.removeAt(i--);
			if(collidingItems.isEmpty())
			{
				mAddItem->show();
				drawDistances(*mAddItem);
			}
			else
			{
				mAddItem->hide();
				hideDistances();
			}
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
						itemAtCursor->setPenColorAnimated(QColor(70, 70, 70));
						itemAtCursor = 0;
					}
					if(itemAtCursor == 0)
					{
						itemAtCursor = item;
						itemAtCursor->setPenColorAnimated(QColor(200, 100, 100));
					}
				}
			}
			else if(itemAtCursor)
			{
				itemAtCursor->setPenColorAnimated(QColor(70, 70, 70));
				itemAtCursor = 0;
			}
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
		hideDistances();
	}

	return false;
}

void CPlaceSchemeDialog::updateSeatsCountText()
{
	ui->lTotalSeatCount->setText(tr("Максимальное число мест в зале: %1").arg(mSeatsCount));
}

void CPlaceSchemeDialog::updateScaleText()
{
	ui->gbxScale->setTitle(tr("Вид [%1%]").arg(mScale*100));
}

//public:

CPlaceSchemeDialog::CPlaceSchemeDialog(const QString &connectionName, const int placeId, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CPlaceSchemeDialog)
{
	ui->setupUi(this);
	ui->gvScheme->setMouseTracking(true);
	ui->splitter->setStretchFactor(0, 100);
	ui->splitter->setStretchFactor(1, 1);
	ui->splitter->setCollapsible(1, true);

	setWindowState(windowState() | Qt::WindowMaximized);

	mEditType = None;
	mScale = 1.0f;
	mSeatsCount = 0;
	mSelectedItem = 0;

	mConnectionName = connectionName;
	mId = placeId;

	mScene.setSceneRect(0.0, 0.0, 1020.0f, 730.0f);
	ui->gvScheme->setScene(&mScene);
	ui->gvScheme->viewport()->installEventFilter(this);
	connect(ui->gvScheme, SIGNAL(wheelUp()), this, SLOT(on_tbnZoomIn_clicked()));
	connect(ui->gvScheme, SIGNAL(wheelDown()), this, SLOT(on_tbnZoomOut_clicked()));

	ui->chbxShowAxis->setChecked(mScene.isDrawAxis());
	ui->chbxShowBackground->setChecked(mScene.isDrawBackground());

	//addItem:
	mAddItem = new CSeatItem();
	mAddItem->setData(0, "addItem");
	mAddItem->hide();
	mAddItem->setOpacity(0.4);
	mScene.addItem(mAddItem);

	//Distances ->
	mDistanceLeftItem = new CDistanceItem();
	mDistanceUpItem = new CDistanceItem();
	mDistanceRightItem = new CDistanceItem();
	mDistanceDownItem = new CDistanceItem();
	mDistanceLeftLeftItem = new CDistanceItem();
	mDistanceUpUpItem = new CDistanceItem();
	mDistanceRightRightItem = new CDistanceItem();
	mDistanceDownDownItem = new CDistanceItem();

	hideDistances();

	mDistanceLeftItem->setBold(true);
	mDistanceUpItem->setBold(true);
	mDistanceRightItem->setBold(true);
	mDistanceDownItem->setBold(true);

	mScene.addItem(mDistanceLeftItem);
	mScene.addItem(mDistanceUpItem);
	mScene.addItem(mDistanceRightItem);
	mScene.addItem(mDistanceDownItem);
	mScene.addItem(mDistanceLeftLeftItem);
	mScene.addItem(mDistanceUpUpItem);
	mScene.addItem(mDistanceRightRightItem);
	mScene.addItem(mDistanceDownDownItem);
	//Distances <-

	//load:
	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	query.prepare("SELECT title, id_background, backgroundWidth, backgroundHeight FROM Places WHERE id = :id;");
	query.bindValue(":id", mId);
	if(query.exec() && query.first())
	{
		ui->lPlaceName->setText(query.value(0).toString());
		if(query.isNull(1) == false)
		{
			mScene.setBackgroundImage(CImages::instance()->image(query.value(1).toInt()));
		}
		ui->sbxWidth->setValue(query.value(2).toInt());
		ui->sbxHeight->setValue(query.value(3).toInt());
		mScene.setSceneRect(0.0f, 0.0f, query.value(2).toReal(), query.value(3).toReal());
	}

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

	query.prepare("UPDATE Places SET backgroundWidth = :width, backgroundHeight = :height WHERE id = :id;");
	query.bindValue(":id", mId);
	query.bindValue(":width", ui->sbxWidth->value());
	query.bindValue(":height", ui->sbxHeight->value());
	if(!query.exec())
		qDebug(qPrintable(query.lastError().text()));

	QList<QGraphicsItem*> items = mScene.items();

	QProgressDialog progressDialog(this);
	progressDialog.setWindowTitle(tr("Сохранение"));
	progressDialog.setCancelButton(0);
	progressDialog.setMinimumDuration(500);
	progressDialog.setMaximum(items.count());

	for(int i = 0; i < items.count(); i++)
	{
		if(items[i]->data(0).toString() == CSeatItem::itemName())
		{
			CSeatItem *item = static_cast<CSeatItem*>(items[i]);
			if(item->needSave())
			{
				if(item->id() == 0)
				{
					query.prepare("INSERT INTO PlaceSchemes VALUES(NULL, :number, :row, :x, :y, :placeId);");
					query.bindValue(":number", item->text());
					query.bindValue(":row", item->row());
					query.bindValue(":x", item->x());
					query.bindValue(":y", item->y());
					query.bindValue(":placeId", mId);
					query.exec();
				}
				else
				{
					query.prepare("UPDATE PlaceSchemes SET seatNumber = :number, row = :row, x = :x, y = :y WHERE id = :id;");
					query.bindValue(":number", item->text());
					query.bindValue(":row", item->row());
					query.bindValue(":x", item->x());
					query.bindValue(":y", item->y());
					query.bindValue(":id", item->id());
					query.exec();
				}
			}
		}
		progressDialog.setValue(i);
	}
	progressDialog.close();

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
	if(mScale < 10)
	{
		if(mScale >= 1.0f)
			mScale += qRound(mScale + 1.0f) * 0.2f;
		else
			mScale += 0.2f;
		ui->gvScheme->setScaleAnimated(mScale);
		updateScaleText();
	}
}

void CPlaceSchemeDialog::on_tbnZoomOut_clicked()
{
	if(mScale > 0.2000001f)
	{
		if(mScale >= 1.0f)
			mScale -= qRound(mScale + 1.0f) * 0.2f;
		else
			mScale -= 0.2f;
		ui->gvScheme->setScaleAnimated(mScale);
		updateScaleText();
	}
}

void CPlaceSchemeDialog::on_tbnZoomDefault_clicked()
{
	mScale = 1.0f;
	ui->gvScheme->setScaleAnimated(mScale);
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
		ui->gbxRow->setEnabled(true);
		mEditType = Select;
	}
	else
		mEditType = None;
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

void CPlaceSchemeDialog::on_leRow_textEdited(const QString &arg1)
{
	if(mSelectedItem)
		mSelectedItem->setRow(arg1);
}

void CPlaceSchemeDialog::on_tbnSelect_toggled(bool checked)
{
	if(!checked)
	{
		ui->gbxNumber->setEnabled(false);
		ui->gbxRow->setEnabled(false);
		if(mSelectedItem)
		{
			mSelectedItem->setSelectedAnimated(false);
			mSelectedItem = 0;
		}
	}
}

void CPlaceSchemeDialog::on_tbnDrag_toggled(bool checked)
{
	ui->gvScheme->setDragEnabled(checked);
}

void CPlaceSchemeDialog::on_pbnBackground_clicked()
{
	CImageDialog imageDialog(mConnectionName, "Places", "id_background", tr("id = %1").arg(mId), this);
	imageDialog.setWindowTitle(tr("Подложка концертной площадки"));
	if(QDialog::Accepted == imageDialog.exec())
		mScene.setBackgroundImage(imageDialog.image());
}

void CPlaceSchemeDialog::on_chbxShowBackground_toggled(bool checked)
{
	mScene.setDrawBackground(checked);
}

void CPlaceSchemeDialog::on_chbxShowAxis_toggled(bool checked)
{
	mScene.setDrawAxis(checked);
}

void CPlaceSchemeDialog::on_sbxWidth_valueChanged(int arg1)
{
	mScene.setSceneRect(0, 0, arg1, mScene.height());
}

void CPlaceSchemeDialog::on_sbxHeight_valueChanged(int arg1)
{
	mScene.setSceneRect(0, 0, mScene.width(), arg1);
}
