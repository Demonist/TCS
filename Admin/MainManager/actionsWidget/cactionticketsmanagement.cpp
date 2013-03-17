#include "cactionticketsmanagement.h"
#include "ui_cactionticketsmanagement.h"

#define ID 0
#define COLOR 1
#define NAME 2
#define PRICE 3
#define PENALTY 4

//protected:

bool CActionTicketsManagement::eventFilter(QObject *obj, QEvent *event)
{
	if(obj != ui->gvScene->viewport())
		return false;

	static bool pressed = false;

	if(event->type() == QEvent::MouseButtonRelease)
	{
		if(static_cast<QMouseEvent*>(event)->button() == Qt::LeftButton)
		{
			pressed = false;

			//Дополнительная обработка для уже занятых мест:
			QGraphicsItem *item = mScene.itemAt(ui->gvScene->mapToScene(static_cast<QMouseEvent*>(event)->pos()));
			if(item && item->data(0).toString() == CActionSeatItem::itemName())
			{
				CActionSeatItem *seatItem = static_cast<CActionSeatItem*>(item);
				if( (seatItem->seatState() == Global::SeatReserved || seatItem->seatState() == Global::SeatSolded)
				   && (mEditType == Avaible || mEditType == NotAvaible || mEditType == Hide || mEditType == Show) )
				{
					QString seatStr = seatItem->seatState() == Global::SeatSolded ? tr("проданного") : tr("забронированного");
					if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите изменить состояние уже %1 места?").arg(seatStr), QMessageBox::Yes, QMessageBox::No))
					{
						if(mEditType == Hide)
							seatItem->setSeatStateAnimated(Global::SeatHided, 1000);
						else if(mEditType == Avaible)
							seatItem->setSeatStateAnimated(Global::SeatFree, 1000);
						else if(mEditType == NotAvaible)
							seatItem->setSeatStateAnimated(Global::SeatNotAvaible, 1000);
					}
				}
			}
		}
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
		if(seatItem->seatState() != Global::SeatReserved && seatItem->seatState() != Global::SeatSolded)
		{
			if(mEditType == Hide)
				seatItem->setSeatStateAnimated(Global::SeatHided, 1000);
			else if(mEditType == Avaible)
				seatItem->setSeatStateAnimated(Global::SeatFree, 1000);
			else if(mEditType == NotAvaible)
				seatItem->setSeatStateAnimated(Global::SeatNotAvaible, 1000);
		}

		if(mEditType == Paint)
		{
			QTreeWidgetItem *priceItem = ui->twPriceGroups->currentItem();
			if(priceItem && seatItem->priceGroupId() != priceItem->text(ID).toInt())
			{
				seatItem->setPriceGroupId(priceItem->text(ID).toInt());
				seatItem->setPenColorAnimated(priceItem->backgroundColor(COLOR), 500);
			}
		}
	}
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
	painter->drawRoundedRect(-2, 0, viewSize.width()-2, h+5, 20, 10);	//2 и 5 - отступы, чтобы была видна только верхняя граница от pen.

	pen.setColor(qRgb(200, 200, 200));
	painter->setPen(pen);
	painter->drawText(QRect(0, 2, viewSize.width() - 12, 20), Qt::AlignRight | Qt::AlignTop, tr("Легенда"));

	painter->translate(50, 30);
	pen.setColor(qRgb(255, 255, 255));
	painter->setPen(pen);
	CSeatItem seatItem;
	seatItem.setBrushColor(QColor(200, 200, 200));


	//Фан зона:
	if(mFanCount > 0)
	{
		seatItem.setPenColor(QColor(100, 100, 255));
		seatItem.setText(QString::number(mFanPrice));
		seatItem.paint(painter, 0, 0);

		QFont font = painter->font();
		font.setItalic(true);
		painter->setFont(font);
		painter->drawText(-50, 20, 100, 50, Qt::AlignHCenter | Qt::AlignTop, tr("Фан зона"));
		font.setItalic(false);
		painter->setFont(font);
		painter->translate(100, 0);
	}

	for(int i = 0; i < ui->twPriceGroups->topLevelItemCount(); i++)
	{
		QTreeWidgetItem *item = ui->twPriceGroups->topLevelItem(i);

		seatItem.setPenColor(item->backgroundColor(COLOR));
		seatItem.setText(item->text(PRICE));
		seatItem.paint(painter, 0, 0);

		painter->drawText(-50, 20, 100, 50, Qt::AlignHCenter | Qt::AlignTop, item->text(NAME));
		painter->translate(100, 0);
	}
}

//public:

CActionTicketsManagement::CActionTicketsManagement(const QString &connectionName, const int id, Type type, QWidget *parent) :
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
	ui->twPriceGroups->setColumnWidth(PENALTY, 80);

	mConnectionName = connectionName;
	mId = id;

	mScene.setSceneRect(0.0f, 0.0f, 1020.0f, 730.0f);
	mScene.setDrawBounds(true);

	ui->gvScene->setScene(&mScene);
	ui->gvScene->viewport()->installEventFilter(this);
	ui->gvScene->setLegend(this);

	ui->cbxShowBackground->setChecked(mScene.isDrawBackground());
	ui->cbxShowLegend->setChecked(ui->gvScene->isLegend());

	if(type == Show)
	{
		ui->gbxSeats->setEnabled(false);
		ui->pbnApply->setEnabled(false);
		ui->pbnFan->setEnabled(false);
		ui->tbnPriceAdd->setEnabled(false);
		ui->tbnPriceDel->setEnabled(false);
		ui->tbnPriceEdit->setEnabled(false);
		ui->tbnPricePaint->setEnabled(false);
	}

	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	query.prepare("SELECT"
				  " Actions.title,"
				  " Places.title,"
				  " Actions.performer,"
				  " Actions.dateTime,"
				  " Places.id,"
				  " Places.id_background,"
				  " Places.backgroundWidth,"
				  " Places.backgroundHeight,"
				  " Actions.fanPrice,"
				  " Actions.fanPenalty,"
				  " Actions.fanCount "
				  "FROM"
				  " Actions,"
				  " Places "
				  "WHERE"
				  " Actions.id = :id AND"
				  " Actions.id_place = Places.id;");
	query.bindValue(":id", mId);
	if(query.exec() && query.first())
	{
		ui->lPlace->setText(tr("%1 (%2) - %3 - %4")
							.arg(query.value(0).toString())
							.arg(query.value(1).toString())
							.arg(query.value(2).toString())
							.arg(query.value(3).toDateTime().toString("dd.MM.yyyy hh:mm"))
							);
		mPlaceId = query.value(4).toInt();
		if(query.isNull(5) == false)
			mScene.setBackgroundImage(CImages::instance()->image(query.value(5).toInt()));
		mScene.setSceneRect(0.0f, 0.0f, query.value(6).toReal(), query.value(7).toReal());
		mFanPrice = query.value(8).toInt();
		mFanCount = query.value(10).toInt();
		ui->lFanPrice->setText(tr("Цена: %1 р.").arg(mFanPrice));
		ui->lFanPenalty->setText(tr("Неустойка: %1 р.").arg(query.value(9).toInt()));
		ui->lFanCount->setText(tr("Количество: %1 шт.").arg(mFanCount));

	}
	else
		qDebug(qPrintable(query.lastError().text()));

	query.prepare("SELECT id, name, price, penalty, color FROM ActionPriceGroups WHERE id_action = :actId;");
	query.bindValue(":actId", mId);
	if(query.exec())
	{
		while(query.next())
		{
			QTreeWidgetItem *item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(ID, query.value(0).toString());
				item->setText(NAME, query.value(1).toString());
				item->setText(PRICE, query.value(2).toString());
				item->setBackgroundColor(COLOR, QColor(query.value(4).toString()));
				item->setText(PENALTY, query.value(3).toString());
				ui->twPriceGroups->addTopLevelItem(item);
			}
		}
	}
	else
		qDebug(qPrintable(query.lastError().text()));

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
	else
		qDebug(qPrintable(query.lastError().text()));
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
	QList<QGraphicsItem*> items = mScene.items();

	QProgressDialog progressDialog(this);
	progressDialog.setWindowTitle(tr("Сохранение"));
	progressDialog.setCancelButton(0);
	progressDialog.setMinimumDuration(500);
	progressDialog.setMaximum(items.count());

	//Проверка на наличие ценовой группы:
	for(int i = 0; i < items.size(); i++)
		if(items[i]->data(0) == CActionSeatItem::itemName())
		{
			CActionSeatItem *seatItem = static_cast<CActionSeatItem*>(items[i]);
			if(seatItem->priceGroupId() == 0 && seatItem->seatState() != Global::SeatHided)
				if(QMessageBox::Yes == QMessageBox::warning(this, tr("Внимание"), tr("У некоторых мест отсутствует ценовая группа, такие места НЕ будут отображаться.\nВсе равно желаеете продолжить?"), QMessageBox::Yes, QMessageBox::No))
					break;
				else
					return;
		}

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
						query.prepare("INSERT INTO ActionScheme VALUES(:actId, :seatId, :state, :priceGroup);");

						query.bindValue(":actId", mId);
						query.bindValue(":seatId", seatItem->id());
						query.bindValue(":state", (int)seatItem->seatState());
						query.bindValue(":priceGroup", seatItem->priceGroupId());
					}
				}
				if(!query.exec())
				{
					qDebug(qPrintable(query.lastError().text()));
					break;
				}
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
		ui->tbnDrag->setChecked(false);
		ui->tbnPricePaint->setChecked(false);
		mEditType = Hide;
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
	ui->gvScene->scaleUp();
}

void CActionTicketsManagement::on_tbnZoomOut_clicked()
{
	ui->gvScene->scaleDown();
}

void CActionTicketsManagement::on_tbnZoomDefault_clicked()
{
	ui->gvScene->setScaleAnimated(1.0f);
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
			item->setText(PENALTY, QString::number(priceDialog.penalty()));
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
			item->setText(PENALTY, QString::number(priceDialog.penalty()));

			QList<QGraphicsItem*> items = mScene.items();
			for(int i = 0; i < items.count(); i++)
			{
				if(items[i]->data(0).toString() == CActionSeatItem::itemName())
				{
					CActionSeatItem *seatItem = static_cast<CActionSeatItem*>(items[i]);
					if(seatItem->priceGroupId() == priceDialog.id())
						seatItem->setPenColorAnimated(priceDialog.color(), 1000);
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
						seatItem->setPenColorAnimated(qRgb(70, 70, 70), 1000);
					}
				}
			}
		}
	}
}

void CActionTicketsManagement::on_pbnFan_clicked()
{
	CFanDialog fanDialog(mConnectionName, mId, this);
	if(QDialog::Accepted == fanDialog.exec())
	{
		ui->lFanPrice->setText(tr("Цена: %1 р.").arg(fanDialog.price()));
		ui->lFanPenalty->setText(tr("Неустойка: %1 р.").arg(fanDialog.penalty()));
		ui->lFanCount->setText(tr("Количество: %1 шт.").arg(fanDialog.count()));
		mFanPrice = fanDialog.price();
		mFanCount = fanDialog.count();
	}
}
