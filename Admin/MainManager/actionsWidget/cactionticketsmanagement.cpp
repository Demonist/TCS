#include "cactionticketsmanagement.h"
#include "ui_cactionticketsmanagement.h"

#define ID 0
#define COLOR 1
#define NAME 2
#define PRICE 3

//protected:

bool CActionTicketsManagement::eventFilter(QObject *obj, QEvent *event)
{
	if(obj != ui->gvScene->viewport())
		return false;

	static bool pressed = false;

	if(event->type() == QEvent::MouseButtonRelease)
	{
		if(static_cast<QMouseEvent*>(event)->button() == Qt::LeftButton)
			pressed = false;
	}
	else if(event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
		if(mouseEvent->button() == Qt::LeftButton)
		{
			pressed = true;
			if(mEditType != None
				&& mEditType != Drag)
					mouseSceneEvent(mouseEvent);
		}
	}
	else if(event->type() == QEvent::MouseMove)
	{
		if(pressed
		   && mEditType != None
		   && mEditType != Drag)
		mouseSceneEvent(static_cast<QMouseEvent*>(event));
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
		else if(mEditType == Paint)
		{
			QTreeWidgetItem *priceItem = ui->twPriceGroups->currentItem();
			if(priceItem && seatItem->priceGroupId() != priceItem->text(ID).toInt())
			{
				seatItem->setPriceGroupId(priceItem->text(ID).toInt());
				seatItem->setBorderColorAnimated(priceItem->backgroundColor(COLOR), 500);
			}
		}
	}
}

void CActionTicketsManagement::updateScaleText()	//Данная функция не inline т.е. в заголовочном файле ui не виден.
{
	ui->gbxScale->setTitle(tr("Вид [%1%]").arg(mScale * 100));
}

void CActionTicketsManagement::paintLenend(QPainter *painter, const QSize &viewSize)
{
	const int h = 80;

	painter->translate(0, viewSize.height() - h);

	QPen pen;
	pen.setColor(qRgb(30, 30, 30));
	pen.setWidth(5);
	painter->setPen(pen);
	painter->setBrush(QColor(70, 70, 70));
	painter->drawRoundedRect(-2, 0, viewSize.width()-2, h+5, 20, 10);	//5 - отступы, чтобы была видна только верхняя граница от pen.

	pen.setColor(qRgb(200, 200, 200));
	painter->setPen(pen);
	painter->drawText(QRect(0, 2, viewSize.width() - 12, 20), Qt::AlignRight | Qt::AlignTop, tr("Легенда"));

	painter->translate(50, 30);
	pen.setColor(qRgb(255, 255, 255));
	painter->setPen(pen);
	CSeatItem seatItem;
	seatItem.setBrush(QColor(200, 200, 200));
	for(int i = 0; i < ui->twPriceGroups->topLevelItemCount(); i++)
	{
		QTreeWidgetItem *item = ui->twPriceGroups->topLevelItem(i);

		seatItem.setBorderColor(item->backgroundColor(COLOR));
		seatItem.setText(item->text(PRICE));
		seatItem.paint(painter, 0, 0);

		painter->drawText(-50, 20, 100, 50, Qt::AlignHCenter | Qt::AlignTop, item->text(NAME));
		painter->translate(100, 0);
	}
}

//public:

CActionTicketsManagement::CActionTicketsManagement(const QString &connectionName, const int id, QWidget *parent) :
    QDialog(parent),
	CAbstractLegend(),
    ui(new Ui::CActionTicketsManagement)
{
    ui->setupUi(this);
	setWindowState(windowState() | Qt::WindowMaximized);
	ui->splitter->setStretchFactor(0, 100);
	ui->splitter->setStretchFactor(1, 1);
	ui->splitter->setCollapsible(1, true);

	ui->twPriceGroups->hideColumn(ID);
	ui->twPriceGroups->headerItem()->setText(COLOR, "");
	ui->twPriceGroups->setColumnWidth(COLOR, 20);
	ui->twPriceGroups->setColumnWidth(NAME, 80);
	ui->twPriceGroups->setColumnWidth(PRICE, 50);

	mConnectionName = connectionName;
	mId = id;
	mScale = 1.0f;

	mScene.setSceneRect(0.0f, 0.0f, 1020.0f, 730.0f);
	ui->gvScene->setScene(&mScene);
	ui->gvScene->viewport()->installEventFilter(this);
	ui->gvScene->setLegend(this);
	connect(ui->gvScene, SIGNAL(wheelUp()), this, SLOT(on_tbnZoomIn_clicked()));
	connect(ui->gvScene, SIGNAL(wheelDown()), this, SLOT(on_tbnZoomOut_clicked()));
	ui->cbxShowBackground->setChecked(mScene.isDrawBackground());
	ui->cbxShowLegend->setChecked(ui->gvScene->isLegend());

	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	query.prepare("SELECT Actions.title, Places.title, Actions.dateTime, Places.id, Actions.fanPrice, Actions.fanCount, Places.id_background, Actions.performer, Places.backgroundWidth, Places.backgroundHeight FROM Actions, Places WHERE Actions.id = :id AND Actions.id_place = Places.id;");
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
			mScene.setBackgroundImage(CImages::instance()->image(query.value(6).toInt()));
		mScene.setSceneRect(0.0f, 0.0f, query.value(8).toReal(), query.value(9).toReal());
	}

	query.prepare("SELECT id, name, price, color FROM ActionPriceGroups WHERE id_action = :actId;");
	query.bindValue(":actId", mId);
	if(query.exec())
		while(query.next())
		{
			QTreeWidgetItem *item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(ID, query.value(0).toString());
				item->setText(NAME, query.value(1).toString());
				item->setText(PRICE, query.value(2).toString());
				item->setBackgroundColor(COLOR, QColor(query.value(3).toString()));
				ui->twPriceGroups->addTopLevelItem(item);
			}
		}

	query.prepare("SELECT id FROM PlaceSchemes WHERE id_place = :id;");
	query.bindValue(":id", mPlaceId);
	if(query.exec())
	{
		QSqlQuery query2(QSqlDatabase::database(mConnectionName));
		query2.prepare("SELECT state, id_priceGroup FROM ActionScheme WHERE id_action = :actionId AND id_placeScheme = :idSeat");
		while(query.next())
		{
			CActionSeatItem *item;

			query2.bindValue(":actionId", mId);
			query2.bindValue(":idSeat", query.value(0).toInt());
			if(query2.exec() && query2.first())
			{
				item = new CActionSeatItem(mConnectionName, query.value(0).toInt(), (Global::SeatState)query2.value(0).toInt(), query2.value(1).toInt());
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
						query.prepare("UPDATE ActionScheme SET state = :state, id_priceGroup = :priceGroup WHERE id_action = :actId AND id_placeScheme = :seatId;");
						query.bindValue(":state", (int)seatItem->seatState());
						query.bindValue(":actId", mId);
						query.bindValue(":seatId", seatItem->id());
						query.bindValue(":priceGroup", seatItem->priceGroupId());
					}
					else
					{
						query.prepare("INSERT INTO ActionScheme VALUES(:actId, :seatId, :state, :priceGroup, NULL);");
						query.bindValue(":actId", mId);
						query.bindValue(":seatId", seatItem->id());
						query.bindValue(":state", (int)seatItem->seatState());
						query.bindValue(":priceGroup", seatItem->priceGroupId());
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
		ui->tbnPricePaint->setChecked(false);
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
		ui->tbnPricePaint->setChecked(false);
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
		ui->tbnPricePaint->setChecked(false);
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
		ui->tbnPricePaint->setChecked(false);
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
		ui->tbnPricePaint->setChecked(false);
		mEditType = Drag;
	}
	else
		mEditType = None;
}

void CActionTicketsManagement::on_tbnPricePaint_clicked(bool checked)
{
	if(checked)
	{
		ui->tbnAvaible->setChecked(false);
		ui->tbnNotAvaible->setChecked(false);
		ui->tbnShow->setChecked(false);
		ui->tbnHide->setChecked(false);
		ui->tbnDrag->setChecked(false);
		mEditType = Paint;
	}
	else
		mEditType = None;
}

void CActionTicketsManagement::on_tbnDrag_toggled(bool checked)
{
	ui->gvScene->setDragEnabled(checked);
}

void CActionTicketsManagement::on_tbnPricePaint_toggled(bool checked)
{
	if(!checked)
		ui->twPriceGroups->clearSelection();
}

void CActionTicketsManagement::on_tbnZoomIn_clicked()
{
	if(mScale >= 1.0f)
		mScale += qRound(mScale + 1.0f) * 0.2f;
	else
		mScale += 0.2f;
	ui->gvScene->setScaleAnimated(mScale);
	updateScaleText();
}

void CActionTicketsManagement::on_tbnZoomOut_clicked()
{
	if(mScale > 0.2f)
	{
		if(mScale >= 1.0f)
			mScale -= qRound(mScale + 1.0f) * 0.2f;
		else
			mScale -= 0.2f;
		ui->gvScene->setScaleAnimated(mScale);
		updateScaleText();
	}
}

void CActionTicketsManagement::on_tbnZoomDefault_clicked()
{
	mScale = 1.0f;
	ui->gvScene->setScaleAnimated(mScale);
	updateScaleText();
}

void CActionTicketsManagement::on_cbxShowBackground_toggled(bool checked)
{
	mScene.setDrawBackground(checked);
}

void CActionTicketsManagement::on_cbxShowLegend_toggled(bool checked)
{
	if(checked)
		ui->gvScene->setLegend(this);
	else
		ui->gvScene->setLegend(0);
}

void CActionTicketsManagement::on_tbnPriceAdd_clicked()
{
	CPriceDialog priceDialog(mConnectionName, mId, this);
	if(QDialog::Accepted == priceDialog.exec())
	{
		QTreeWidgetItem *item = new QTreeWidgetItem();
		if(item)
		{
			item->setText(ID, QString::number(priceDialog.id()));
			item->setText(NAME, priceDialog.name());
			item->setText(PRICE, QString::number(priceDialog.price()));
			item->setBackgroundColor(COLOR, priceDialog.color());
			ui->twPriceGroups->addTopLevelItem(item);
		}
	}
}

void CActionTicketsManagement::on_tbnPriceEdit_clicked()
{
	QTreeWidgetItem *item = ui->twPriceGroups->currentItem();
	if(item)
	{
		CPriceDialog priceDialog(mConnectionName, mId, item->text(ID).toInt(), this);
		if(QDialog::Accepted == priceDialog.exec())
		{
			item->setText(NAME, priceDialog.name());
			item->setText(PRICE, QString::number(priceDialog.price()));
			item->setBackgroundColor(COLOR, priceDialog.color());

			QList<QGraphicsItem*> items = mScene.items();
			for(int i = 0; i < items.count(); i++)
			{
				if(items[i]->data(0).toString() == CActionSeatItem::itemName())
				{
					CActionSeatItem *seatItem = static_cast<CActionSeatItem*>(items[i]);
					if(seatItem->priceGroupId() == priceDialog.id())
						seatItem->setBorderColorAnimated(priceDialog.color(), 1000);
				}
			}
		}
	}
}

void CActionTicketsManagement::on_tbnPriceDel_clicked()
{
	QTreeWidgetItem *item = ui->twPriceGroups->currentItem();
	if(item)
	{
		if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите удалить ценовую группу '%1 (%2 р.)'?").arg(item->text(NAME)).arg(item->text(PRICE)), QMessageBox::Yes, QMessageBox::No))
		{
			int priceGrId = item->text(ID).toInt();
			QSqlQuery query(QSqlDatabase::database(mConnectionName));
			query.prepare("DELETE FROM ActionPriceGroups WHERE id = :id;");
			query.bindValue(":id", priceGrId);
			query.exec();
			ui->twPriceGroups->takeTopLevelItem(ui->twPriceGroups->indexOfTopLevelItem(item));

			QList<QGraphicsItem*> items = mScene.items();
			for(int i = 0; i < items.count(); i++)
			{
				if(items[i]->data(0).toString() == CActionSeatItem::itemName())
				{
					CActionSeatItem *seatItem = static_cast<CActionSeatItem*>(items[i]);
					if(seatItem->priceGroupId() == priceGrId)
					{
						seatItem->setPriceGroupId(0);
						seatItem->setBorderColorAnimated(qRgb(70, 70, 70), 1000);
					}
				}
			}
		}
	}
}
