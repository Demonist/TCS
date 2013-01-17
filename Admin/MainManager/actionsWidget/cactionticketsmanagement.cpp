#include "cactionticketsmanagement.h"
#include "ui_cactionticketsmanagement.h"

//protected:

bool CActionTicketsManagement::eventFilter(QObject *obj, QEvent *event)
{
	if(obj != ui->gvScene->viewport())
		return false;

	static bool pressed = false;
	static QPoint lastDragPos(0, 0);

	if(event->type() == QEvent::MouseButtonRelease)
	{
		pressed = false;
		if(mEditType == Drag)
			ui->gvScene->setCursor(Qt::OpenHandCursor);
	}
	else if(event->type() == QEvent::MouseButtonPress)
	{
		pressed = true;
		if(mEditType != None)
		{
			if(mEditType == Drag)
			{
				lastDragPos = static_cast<QMouseEvent*>(event)->pos();
				ui->gvScene->setCursor(Qt::ClosedHandCursor);
			}
			else
				mouseSceneEvent(static_cast<QMouseEvent*>(event));
		}
	}
	else if(event->type() == QEvent::MouseMove)
	{
		if(pressed && mEditType != None)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
			if(mEditType == Drag)
			{
				QPoint pos = mouseEvent->pos();
				QPoint scroll = lastDragPos - pos;
				ui->gvScene->horizontalScrollBar()->setValue(ui->gvScene->horizontalScrollBar()->value() + scroll.x());
				ui->gvScene->verticalScrollBar()->setValue(ui->gvScene->verticalScrollBar()->value() + scroll.y());
				lastDragPos = pos;
			}
			else
				mouseSceneEvent(mouseEvent);
		}
	}

	return false;
}

void CActionTicketsManagement::mouseSceneEvent(QMouseEvent *event)
{
	QGraphicsItem *item = mScene.itemAt(ui->gvScene->mapToScene(event->pos()));
	if(item && item->data(0).toString() == CActionSeatItem::itemName())
	{
		CActionSeatItem *seatItem = static_cast<CActionSeatItem*>(item);
		if(mEditType == Hide)
			seatItem->setSeatStateAnimated(Global::SeatHided, 1000);
		else if(mEditType == Show)
			seatItem->setSeatStateAnimated(Global::SeatNotAvaible, 1000);
		else if(mEditType == Avaible)
			seatItem->setSeatStateAnimated(Global::SeatFree, 1000);
		else if(mEditType == NotAvaible)
			seatItem->setSeatStateAnimated(Global::SeatNotAvaible, 1000);
	}
}

void CActionTicketsManagement::updateScaleText()
{
	ui->gbxScale->setTitle(tr("Вид [%1%]").arg(mScalePersent));
}

//public:

CActionTicketsManagement::CActionTicketsManagement(const QString &connectionName, const int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CActionTicketsManagement)
{
    ui->setupUi(this);
	setWindowState(windowState() | Qt::WindowMaximized);

	mConnectionName = connectionName;
	mId = id;

	mScene.setSceneRect(0.0, 0.0, 1020.0, 730.0);
	ui->gvScene->setScene(&mScene);
	ui->gvScene->viewport()->installEventFilter(this);
	mScalePersent = 100;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	query.prepare("SELECT Actions.title, Places.title, Actions.dateTime, Places.id, Actions.fanPrice, Actions.fanCount, Places.id_background, Actions.performer FROM Actions, Places WHERE Actions.id = :id AND Actions.id_place = Places.id;");
	query.bindValue(":id", mId);
	if(query.exec() && query.first())
	{
		ui->lPlace->setText(tr("%1 (%2) - %3 - %4")
							.arg(query.value(0).toString())
							.arg(query.value(1).toString())
							.arg(query.value(7).toString())
							.arg(query.value(2).toDateTime().toString("dd.MM.yyyy hh:mm"))
							);
		mPlaceId = query.value(3).toInt();
		ui->sbxPrice->setValue(query.value(4).toInt());
		ui->sbxFanTicketsCount->setValue(query.value(5).toInt());
		if(query.isNull(6) == false)
			mScene.setBackgroundBrush(CImages::instance()->image(query.value(6).toInt()));
	}

	query.prepare("SELECT id FROM PlaceSchemes WHERE id_place = :id;");
	query.bindValue(":id", mPlaceId);
	if(query.exec())
	{
		QSqlQuery query2(QSqlDatabase::database(mConnectionName));
		query2.prepare("SELECT state FROM ActionScheme WHERE id_action = :actionId AND id_placeScheme = :idPlace");
		while(query.next())
		{
			CActionSeatItem *item;

			query2.bindValue(":actionId", mId);
			query2.bindValue(":idPlace", query.value(0).toInt());
			if(query2.exec() && query2.first())
			{
				item = new CActionSeatItem(mConnectionName, query.value(0).toInt(), (Global::SeatState)query2.value(0).toInt());
				item->setData(1, true);
			}
			else
			{
				item = new CActionSeatItem(mConnectionName, query.value(0).toInt());
				item->setData(1, false);
			}

			mScene.addItem(item);

			item->showAnimated(2000);
		}
	}
}

CActionTicketsManagement::~CActionTicketsManagement()
{
    delete ui;
}

void CActionTicketsManagement::on_pbnCancel_clicked()
{
	close();
}

void CActionTicketsManagement::on_pbnApply_clicked()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	query.prepare("UPDATE Actions SET fanPrice = :price, fanCount = :count WHERE id = :id;");
	query.bindValue(":id", mId);
	query.bindValue(":price", ui->sbxPrice->value());
	query.bindValue(":count", ui->sbxFanTicketsCount->value());
	if(!query.exec())
		qDebug(qPrintable(query.lastError().text()));

	QList<QGraphicsItem*> items = mScene.items();

	QProgressDialog progressDialog(this);
	progressDialog.setWindowTitle(tr("Сохранение"));
	progressDialog.setCancelButton(0);
	progressDialog.setMinimumDuration(500);
	progressDialog.setMaximum(items.count());

	for(int i = 0; i < items.size(); i++)
	{
		if(items[i]->data(0) == CActionSeatItem::itemName())
		{
			CActionSeatItem *seatItem = static_cast<CActionSeatItem*>(items[i]);
			if(seatItem->needSave())
			{
				if(seatItem->seatState() == Global::SeatHided && seatItem->data(1).toBool() == true)
				{
					query.prepare("DELETE FROM ActionScheme WHERE id_action = :actId AND id_placeScheme = :seatId;");
					query.bindValue(":actId", mId);
					query.bindValue(":seatId", seatItem->id());
				}
				else
				{
					if(seatItem->data(1).toBool() == true)
					{
						query.prepare("UPDATE ActionScheme SET state = :state WHERE id_action = :actId AND id_placeScheme = :seatId;");
						query.bindValue(":state", (int)seatItem->seatState());
						query.bindValue(":actId", mId);
						query.bindValue(":seatId", seatItem->id());
					}
					else
					{
						query.prepare("INSERT INTO ActionScheme VALUES(:actId, :seatId, :state, NULL, NULL);");
						query.bindValue(":actId", mId);
						query.bindValue(":seatId", seatItem->id());
						query.bindValue(":state", (int)seatItem->seatState());
					}
				}
				query.exec();
			}
		}
		progressDialog.setValue(i);
	}
	progressDialog.close();
	close();
}

void CActionTicketsManagement::on_tbnAvaible_clicked(bool checked)
{
	if(checked)
	{
		ui->tbnNotAvaible->setChecked(false);
		ui->tbnShow->setChecked(false);
		ui->tbnHide->setChecked(false);
		ui->tbnDrag->setChecked(false);
		mEditType = Avaible;
	}
	else
		mEditType = None;
}

void CActionTicketsManagement::on_tbnNotAvaible_clicked(bool checked)
{
	if(checked)
	{
		ui->tbnAvaible->setChecked(false);
		ui->tbnShow->setChecked(false);
		ui->tbnHide->setChecked(false);
		ui->tbnDrag->setChecked(false);
		mEditType = NotAvaible;
	}
	else
		mEditType = None;
}

void CActionTicketsManagement::on_tbnHide_clicked(bool checked)
{
	if(checked)
	{
		ui->tbnAvaible->setChecked(false);
		ui->tbnNotAvaible->setChecked(false);
		ui->tbnShow->setChecked(false);
		ui->tbnDrag->setChecked(false);
		mEditType = Hide;
	}
	else
		mEditType = None;
}

void CActionTicketsManagement::on_tbnShow_clicked(bool checked)
{
	if(checked)
	{
		ui->tbnAvaible->setChecked(false);
		ui->tbnNotAvaible->setChecked(false);
		ui->tbnHide->setChecked(false);
		ui->tbnDrag->setChecked(false);
		mEditType = Show;
	}
	else
		mEditType = None;
}

void CActionTicketsManagement::on_tbnDrag_clicked(bool checked)
{
	if(checked)
	{
		ui->tbnAvaible->setChecked(false);
		ui->tbnNotAvaible->setChecked(false);
		ui->tbnShow->setChecked(false);
		ui->tbnHide->setChecked(false);
		mEditType = Drag;
	}
	else
		mEditType = None;
}

void CActionTicketsManagement::on_tbnDrag_toggled(bool checked)
{
	if(checked)
		ui->gvScene->setCursor(Qt::OpenHandCursor);
	else
		ui->gvScene->setCursor(Qt::ArrowCursor);
}

void CActionTicketsManagement::on_tbnZoomIn_clicked()
{
	ui->gvScene->scale(1.2, 1.2);
	mScalePersent += 20;
	updateScaleText();
}

void CActionTicketsManagement::on_tbnZoomOut_clicked()
{
	if(mScalePersent > 20)
	{
		ui->gvScene->scale(0.8, 0.8);
		mScalePersent -= 20;
		updateScaleText();
	}
}

void CActionTicketsManagement::on_tbnZoomDefault_clicked()
{
	ui->gvScene->resetMatrix();
	mScalePersent = 100;
	updateScaleText();
}
