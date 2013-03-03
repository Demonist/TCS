#include "cclientactionswidget.h"
#include "ui_cclientactionswidget.h"

#define ID 0
#define TITLE 1
#define PERFORMER 2
#define DATETIME 3
#define PLACE 4
#define CATEGORY 5

CClientActionsWidget::CClientActionsWidget(QWidget *parent) :
	CAbstractCommonWidget(parent),
	CAbstractLegend(),
	ui(new Ui::CClientActionsWidget)
{
	ui->setupUi(this);
	ui->stackedWidget->setCurrentIndex(0);
	ui->gvScene->setScene(&mScene);
	ui->gvScene->setLegend(this);

	mCanUpdateFilter = true;

	ui->twActions->hideColumn(ID);
	ui->twActions->setColumnWidth(ID, 0);

	connect(ui->cbxCategory, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilter()));
	connect(ui->cbxPerformer, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilter()));
	connect(ui->cbxPlace, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilter()));
	connect(ui->leDate, SIGNAL(textChanged(QString)), this, SLOT(updateFilter()));
}

CClientActionsWidget::~CClientActionsWidget()
{
	delete ui;
}

CSourceGraphicsView* CClientActionsWidget::view()
{
	return ui->gvScene;
}

void CClientActionsWidget::paintLenend(QPainter *painter, const QSize &viewSize)
{
	const int h = 90;

	painter->translate(0, viewSize.height() - h);

	QPen pen;
	pen.setColor(qRgb(30, 30, 30));
	pen.setWidth(5);
	painter->setPen(pen);
	painter->setBrush(QColor(70, 70, 70));
	painter->drawRoundedRect(-2, 0, viewSize.width()-2, h+5, 20, 10);	//2 и 5 - отступы, чтобы была видна только верхняя граница от pen.

	//:
	int selectedTicketCount = mSelectedSeats.count() + ui->sbxFanSellCount->value();
	int selectedTicketTotalPrice = mFanPriceForCurrentAction * ui->sbxFanSellCount->value();
	for(int i = 0; i < mSelectedSeats.count(); i++)
		selectedTicketTotalPrice += mSelectedSeats[i]->price();
	ui->lTotalPrice->setText(tr("Общая стоимость: <b>%1 руб</b>").arg(selectedTicketTotalPrice));

	//Выбор подходящего окончания:
	QString tickets = tr("ов");
	switch(selectedTicketCount % 10)
	{
		case 1:
			if(selectedTicketCount % 100 != 11)
				tickets.clear();
			break;
		case 2:
		case 3:
		case 4:
			if(selectedTicketCount < 10 || selectedTicketCount > 20)
				tickets = tr("а");
			break;
	}

	painter->save();
	pen.setColor(QColor(255, 255, 255));
	painter->setPen(pen);
	QFont font = painter->font();
	font.setPointSize(font.pointSize() + 4);
	font.setFamily("Helvetica [Cronyx]");
	painter->setFont(font);
	painter->drawText(QRect(0, 2, viewSize.width() - 12, 50), Qt::AlignRight | Qt::AlignTop, tr("Выбрано: %1 билет%2 на сумму %3 руб").arg(selectedTicketCount).arg(tickets).arg(selectedTicketTotalPrice));
	painter->restore();

	//:
	painter->translate(50, 30);
	pen.setColor(QColor(255, 255, 255));
	painter->setPen(pen);
	CSeatItem seatItem;
	seatItem.setBrushColor(QColor(200, 200, 200));
	seatItem.setPenWidth(5);

	//Фан зона:
	if(mFanCountForCurrentAction > 0)
	{
		seatItem.setPenColor(QColor(100, 100, 255));
		seatItem.setText(QString::number(mFanPriceForCurrentAction));
		seatItem.paint(painter, 0, 0);

		font = painter->font();
		font.setItalic(true);
		painter->save();
		painter->setFont(font);
		painter->drawText(-50, 20, 100, 50, Qt::AlignHCenter | Qt::AlignTop, tr("Фан зона"));
		painter->restore();
		painter->translate(100, 0);
	}

	for(int i = 0; i < mPriceGroupsForCurrentAction.count(); i++)
	{
		seatItem.setPenColor(mPriceGroupsForCurrentAction[i].color);
		seatItem.setText(mPriceGroupsForCurrentAction[i].price);
		seatItem.paint(painter, 0, 0);

		painter->drawText(-50, 20, 100, 50, Qt::AlignHCenter | Qt::AlignTop, mPriceGroupsForCurrentAction[i].name);
		painter->translate(100, 0);
	}
}

void CClientActionsWidget::updateFilter()
{
	if(mCanUpdateFilter)
	{
		QVariant category = ui->cbxCategory->itemData(ui->cbxCategory->currentIndex());
		QVariant place = ui->cbxPlace->itemData(ui->cbxPlace->currentIndex());
		QString performer;
		QString date = ui->leDate->text();

		bool categoryFilter = category.toInt() != 0;
		bool placeFilter = place.toInt() != 0;
		bool performerFilter;
		bool dateFilter;

		if(ui->cbxPerformer->currentIndex())
		{
			performer = ui->cbxPerformer->currentText();
			performerFilter = true;
		}
		else
			performerFilter = false;

		dateFilter = date.isEmpty() == false;

		bool hide;
		QTreeWidgetItem *item;
		for(int i = 0; i < ui->twActions->topLevelItemCount(); i++)
		{
			hide = false;
			item = ui->twActions->topLevelItem(i);

			if(categoryFilter && category != item->data(CATEGORY, Qt::UserRole))
				hide = true;
			else if(placeFilter && place != item->data(PLACE, Qt::UserRole))
				hide = true;
			else if(performerFilter && performer != item->text(PERFORMER))
				hide = true;
			else if(dateFilter && date != item->data(DATETIME, Qt::UserRole))
				hide = true;

			item->setHidden(hide);
		}
	}
}

void CClientActionsWidget::seatSelectionChanged(CClientActionSeatItem *item, const bool isSelected)
{
	if(item)
	{
		if(mSelectedSeats.contains(item))
		{
			if(isSelected == false)
				mSelectedSeats.removeOne(item);
		}
		else if(isSelected)
			mSelectedSeats.append(item);

		mScene.update();
	}
}

void CClientActionsWidget::on_tbnCategoryClear_clicked()
{
	ui->cbxCategory->setCurrentIndex(0);
}

void CClientActionsWidget::on_tbnPerformerCLear_clicked()
{
	ui->cbxPerformer->setCurrentIndex(0);
}

void CClientActionsWidget::on_tbnPlaceClear_clicked()
{
	ui->cbxPlace->setCurrentIndex(0);
}

void CClientActionsWidget::on_tbnDate_clicked()
{
	CDateDialog dateDialog(this);
	if(QDialog::Accepted == dateDialog.exec())
		ui->leDate->setText(dateDialog.selectedDate().toString("dd.MM.yyyy"));
}

void CClientActionsWidget::on_tbnDateClear_clicked()
{
	ui->leDate->clear();
}

void CClientActionsWidget::on_tbnFilterClear_clicked()
{
	mCanUpdateFilter = false;
	ui->cbxCategory->setCurrentIndex(0);
	ui->cbxPerformer->setCurrentIndex(0);
	ui->cbxPlace->setCurrentIndex(0);
	ui->leDate->clear();
	mCanUpdateFilter = true;
	updateFilter();
}

void CClientActionsWidget::on_pbnSoldTicket_clicked()
{
	QTreeWidgetItem *current = ui->twActions->currentItem();
	if(current)
	{
		QString actionTitle = tr("Мероприятие: %1 (%2). Площадка: %3. Дата: %4.")
							  .arg(current->text(TITLE)).arg(current->text(PERFORMER))
							  .arg(current->text(PLACE))
							  .arg(current->text(DATETIME));
		ui->lAction->setText(actionTitle);
		ui->lAction2->setText(actionTitle);
		ui->sbxFanSellCount->setValue(0);
		ui->gvScene->setScale(1.0f);

		mScene.clear();
		mPriceGroupsForCurrentAction.clear();
		mSelectedSeats.clear();

		mCurrentActionId = current->text(ID).toInt();
		mFanPriceForCurrentAction = 0;
		mFanCountForCurrentAction = 0;

		QSqlQuery query(QSqlDatabase::database(mConnectionName));
		query.prepare("SELECT"
					  " Places.backgroundWidth,"
					  " Places.backgroundHeight,"
					  " Places.id_background,"
					  " Actions.fanPrice,"
					  " Actions.fanCount "
					  "FROM"
					  " Places,"
					  " Actions "
					  "WHERE"
					  " Places.id = Actions.id_place AND"
					  " Actions.id = :id;"
					  );
		query.bindValue(":id", mCurrentActionId);
		if(query.exec() && query.first())
		{
			mScene.setSceneRect(0.0f, 0.0f, query.value(0).toReal(), query.value(1).toReal());
			if(query.value(2).isNull() == false)
				mScene.setBackgroundImage(CImages::instance()->image(query.value(2).toInt()));
			mFanPriceForCurrentAction = query.value(3).toInt();
			mFanCountForCurrentAction = query.value(4).toInt();
			ui->leFanPrice->setText(tr("%1 руб.").arg(mFanPriceForCurrentAction));
			ui->leFanAvaible->setText(tr("%1 шт.").arg(mFanCountForCurrentAction));
			ui->sbxFanSellCount->setMaximum(mFanCountForCurrentAction);
			ui->gbxFanZone->setVisible(mFanCountForCurrentAction > 0);
		}
		else
			qDebug(qPrintable(query.lastError().text()));

		query.prepare("SELECT name, price, color FROM ActionPriceGroups WHERE id_action = :id;");
		query.bindValue(":id", mCurrentActionId);
		if(query.exec())
		{
			PriceGroup priceGroup;
			while(query.next())
			{
				priceGroup.name = query.value(0).toString();
				priceGroup.price = query.value(1).toString();
				priceGroup.color = QColor(query.value(2).toString());
				mPriceGroupsForCurrentAction.append(priceGroup);
			}
		}
		else
			qDebug(qPrintable(query.lastError().text()));

		query.prepare("SELECT "
						"ActionScheme.id_placeScheme, "
						"ActionScheme.state, "
						"ActionPriceGroups.color, "
						"ActionPriceGroups.price, "
						"ActionPriceGroups.penalty, "
						"ActionPriceGroups.name, "
						"PlaceSchemes.x, "
						"PlaceSchemes.y, "
						"PlaceSchemes.seatNumber, "
						"PlaceSchemes.row "
					  "FROM "
						"ActionScheme, "
						"ActionPriceGroups, "
						"PlaceSchemes "
					  "WHERE "
						"ActionPriceGroups.id = ActionScheme.id_priceGroup AND "
						"PlaceSchemes.id = ActionScheme.id_placeScheme AND "
						"ActionScheme.state != :stateHided AND "
						"ActionScheme.id_action = :id;"
					  );
		query.bindValue(":stateHided", Global::SeatHided);
		query.bindValue(":id", mCurrentActionId);
		if(query.exec())
		{
			CClientActionSeatItem *item;
			while(query.next())
			{
				item = new CClientActionSeatItem(mConnectionName, query.value(0).toInt());
				if(item)
				{
					item->setPrice(query.value(3).toInt());
					item->setPenalty(query.value(4).toInt());
					item->setPriceGroupTitle(query.value(5).toString());
					item->setSeatState((Global::SeatState)query.value(1).toInt());
					item->setBrushColor(QColor(query.value(2).toString()));
					item->setPos(query.value(6).toInt(), query.value(7).toInt());
					item->setText(query.value(8).toString());
					item->setRow(query.value(9).toString());
					item->updateForSeat();
					if((Global::SeatState)query.value(1).toInt() == Global::SeatFree)
						connect(item, SIGNAL(selectionChanged(CClientActionSeatItem*,bool)), this, SLOT(seatSelectionChanged(CClientActionSeatItem*,bool)));
					mScene.addItem(item);
				}
			}
		}
		else
			qDebug(qPrintable(query.lastError().text()));

		ui->stackedWidget->slideHorizontalNext();
		emit hideLeftPanel();
	}
}

//public slots:

void CClientActionsWidget::updateData()
{
	mCanUpdateFilter = false;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	ui->cbxCategory->clear();
	ui->cbxCategory->addItem(tr("Все"), 0);
	if(query.exec("SELECT name, id FROM Categories;"))
		while(query.next())
			ui->cbxCategory->addItem(query.value(0).toString(), query.value(1).toInt());

	ui->cbxPerformer->clear();
	ui->cbxPerformer->addItem(tr("Все"), 0);
	query.prepare("SELECT DISTINCT performer FROM Actions WHERE state = :state OR state = :state2;");
	query.bindValue(":state", Global::ActionActive);
	query.bindValue(":state2", Global::ActionMoved);
	if(query.exec())
		while(query.next())
			ui->cbxPerformer->addItem(query.value(0).toString());

	ui->cbxPlace->clear();
	ui->cbxPlace->addItem(tr("Все"), 0);
	if(query.exec("SELECT title, id FROM Places;"))
		while(query.next())
			ui->cbxPlace->addItem(query.value(0).toString(), query.value(1).toInt());

	ui->leDate->clear();

	query.prepare("SELECT Actions.id, Actions.title, Actions.performer, Actions.dateTime, Places.title, Places.address, Places.id, Categories.name, Categories.id FROM Actions, Places, Categories WHERE Actions.id_place = Places.id AND Actions.id_category = Categories.id AND (Actions.state = :state OR Actions.state = :state2);");
	query.bindValue(":state", Global::ActionActive);
	query.bindValue(":state2", Global::ActionMoved);
	if(query.exec())
	{
		ui->twActions->clear();

		QTreeWidgetItem *item;
		while(query.next())
		{
			item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(ID, query.value(0).toString());
				item->setText(TITLE, query.value(1).toString());
				item->setText(PERFORMER, query.value(2).toString());
				QDateTime dateTime = query.value(3).toDateTime();
				item->setText(DATETIME, tr("%1 (%2)")
							  .arg(dateTime.toString("dd.MM.yyyy hh:mm"))
							  .arg(QDate::longDayName(dateTime.date().dayOfWeek()))
							  );
				item->setData(DATETIME, Qt::UserRole, dateTime.date().toString("dd.MM.yyyy"));
				item->setText(PLACE, query.value(4).toString() + " (" + query.value(5).toString() + ')');
				item->setData(PLACE, Qt::UserRole, query.value(6));
				item->setText(CATEGORY, query.value(7).toString());
				item->setData(CATEGORY, Qt::UserRole, query.value(8));

				ui->twActions->addTopLevelItem(item);
			}
			////TODO: Добавить обработчик ошибок.
		}
	}
	else
		qDebug(qPrintable(query.lastError().text()));

	mCanUpdateFilter = true;
}

void CClientActionsWidget::showActions()
{
	ui->stackedWidget->setCurrentIndex(0);
}

void CClientActionsWidget::on_pbnBackToActions_clicked()
{
	emit showLeftPanel();
	ui->stackedWidget->slideHorizontalPrev();	//index=0
}

void CClientActionsWidget::on_pbnStartSelling_clicked()
{
	if(mSelectedSeats.count() || ui->sbxFanSellCount->value())
	{
		if(ui->sbxFanSellCount->value())
		{
			ui->gbxFanZone2->setVisible(true);
			ui->lFanZone->setText(tr("Стоимость одного билета: %1 руб. Количество: %2 шт. Сумма: %3 руб.")
							  .arg(mFanPriceForCurrentAction)
							  .arg(ui->sbxFanSellCount->value())
							  .arg(mFanPriceForCurrentAction * ui->sbxFanSellCount->value()));
		}
		else
			ui->gbxFanZone2->setVisible(false);

		ui->twTickets->clear();
		for(int i = 0; i < mSelectedSeats.count(); i++)
		{
			QTreeWidgetItem *item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(0, mSelectedSeats[i]->row());
				item->setText(1, mSelectedSeats[i]->text());
				item->setText(2, QString::number(mSelectedSeats[i]->price()));
				ui->twTickets->addTopLevelItem(item);
			}
		}

		ui->stackedWidget->slideHorizontalNext();	//index=2
	}
}

void CClientActionsWidget::on_pbnBackToSeats_clicked()
{
	ui->stackedWidget->slideHorizontalPrev();	//index=1
}

void CClientActionsWidget::on_pbnPrintTickets_clicked()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	QPrinter printer;
	printer.setOrientation(QPrinter::Portrait);
	printer.setPaperSize(QPrinter::A4);
	printer.setFullPage(true);

	QPrintDialog printDialog(&printer, this);
	printDialog.setWindowTitle(tr("Печать билетов"));
	printDialog.setOptions(QAbstractPrintDialog::PrintToFile);
	printDialog.setPrintRange(QAbstractPrintDialog::AllPages);
	if(printDialog.exec() == QDialog::Accepted)
	{
		int fanPenalty = 0;
		int ticketSubstrateId = 0;
		query.prepare("SELECT fanCount, fanPenalty, id_ticketSubstrate FROM Actions WHERE id = :id;");
		query.bindValue(":id", mCurrentActionId);
		if(query.exec() && query.first())
		{
			QString error;
			fanPenalty = query.value(1).toInt();
			ticketSubstrateId = query.value(2).toInt();
			if(query.value(0).toInt() < ui->sbxFanSellCount->value())
			{
				mFanCountForCurrentAction = query.value(0).toInt();
				ui->sbxFanSellCount->setValue(mFanCountForCurrentAction);
				ui->leFanAvaible->setText(tr("%1 шт.").arg(mFanCountForCurrentAction));
				error = tr("В фан зоне доступно только %1 мест.\n").arg(mFanCountForCurrentAction);
			}

			int failedSeats = 0;
			query.prepare("SELECT state FROM ActionScheme WHERE id_action = :actId AND id_placeScheme = :seatId;");
			query.bindValue(":actId", mCurrentActionId);
			for(int i = 0; i < mSelectedSeats.count(); i++)
			{
				query.bindValue(":seatId", mSelectedSeats[i]->id());
				if(query.exec() && query.first())
				{
					if(query.value(0).toInt() != Global::SeatFree)
					{
						error += tr("Место %1 в ряду %2 занято.\n").arg(mSelectedSeats[i]->text()).arg(mSelectedSeats[i]->row());
						failedSeats++;
					}
				}
			}

			if(error.isEmpty() == false)
			{
				error.chop(1);	//remove \n from end.
				QMessageBox::warning(this, tr("Внимание! Некоторые места заняты"), error + tr("\n\nСейчас произойдет обновление"));

				//updating:
				qDebug("updating");////
				QList<int> seatsSelectedIds;
				for(int i = 0; i < mSelectedSeats.count(); i++)
					seatsSelectedIds.append(mSelectedSeats[i]->id());

				mSelectedSeats.clear();
				mScene.clear();

				query.prepare("SELECT "
								"ActionScheme.id_placeScheme, "
								"ActionScheme.state, "
								"ActionPriceGroups.color, "
								"ActionPriceGroups.price, "
								"ActionPriceGroups.penalty, "
								"ActionPriceGroups.name, "
								"PlaceSchemes.x, "
								"PlaceSchemes.y, "
								"PlaceSchemes.seatNumber, "
								"PlaceSchemes.row "
							  "FROM "
								"ActionScheme, "
								"ActionPriceGroups, "
								"PlaceSchemes "
							  "WHERE "
								"ActionPriceGroups.id = ActionScheme.id_priceGroup AND "
								"PlaceSchemes.id = ActionScheme.id_placeScheme AND "
								"ActionScheme.state != :stateHided AND "
								"ActionScheme.id_action = :id;"
							  );
				query.bindValue(":stateHided", Global::SeatHided);
				query.bindValue(":id", mCurrentActionId);
				if(query.exec())
				{
					CClientActionSeatItem *item;
					while(query.next())
					{
						item = new CClientActionSeatItem(mConnectionName, query.value(0).toInt());
						if(item)
						{
							item->setPrice(query.value(3).toInt());
							item->setPenalty(query.value(4).toInt());
							item->setSeatState((Global::SeatState)query.value(1).toInt());
							item->setBrushColor(QColor(query.value(2).toString()));
							item->setPos(query.value(6).toInt(), query.value(7).toInt());
							item->setText(query.value(8).toString());
							item->setRow(query.value(9).toString());
							item->updateForSeat();
							if((Global::SeatState)query.value(1).toInt() == Global::SeatFree)
							{
								connect(item, SIGNAL(selectionChanged(CClientActionSeatItem*,bool)), this, SLOT(seatSelectionChanged(CClientActionSeatItem*,bool)));
								int index = seatsSelectedIds.indexOf(item->id());
								if(index >= 0)
								{
									item->setSelected(true);
									mSelectedSeats.append(item);
									seatsSelectedIds.removeAt(index);
								}
							}
							mScene.addItem(item);
						}
					}
				}
				else
					qDebug(qPrintable(query.lastError().text()));

				ui->stackedWidget->slideHorizontalPrev();
			}
			else
			{
				QList<QString> fanTickets;
				QList<QString> seatTickets;

				for(int i = 0; i < ui->sbxFanSellCount->value(); i++)
					fanTickets.append(CTicketIdentifier::generate().data());

				for(int i = 0; i < mSelectedSeats.count(); i++)
					seatTickets.append(CTicketIdentifier::generate().data());

				//printing:

				bool queryError = false;

				if(query.exec("BEGIN TRANSACTION;"))
				{
					query.prepare("INSERT INTO Tickets VALUES(NULL, :actId, NULL, NULL, :identifier);");
					query.bindValue(":actId", mCurrentActionId);
					for(int i = 0; i < fanTickets.count() && !queryError; i++)
					{
						query.bindValue(":identifier", fanTickets[i]);
						if(!query.exec())
							queryError = true;
					}

					if(!queryError)
					{
						query.prepare("UPDATE Actions SET fanCount = fanCount - :fanSold WHERE id = :id;");
						query.bindValue(":id", mCurrentActionId);
						query.bindValue(":fanSold", ui->sbxFanSellCount->value());
						if(!query.exec())
							queryError = true;
					}

					if(!queryError)
					{
						query.prepare("INSERT INTO Tickets VALUES(NULL, :actId, :seatId, NULL, :identifier);");
						query.bindValue(":actId", mCurrentActionId);
						for(int i = 0; i < seatTickets.count() && !queryError; i++)
						{
							query.bindValue(":seatId", mSelectedSeats[i]->id());
							query.bindValue(":identifier", seatTickets[i]);
							if(!query.exec())
								queryError = true;
						}
					}

					if(!queryError)
					{
						query.prepare("UPDATE ActionScheme SET state = :state WHERE id_action = :actId AND id_placeScheme = :seatId;");
						query.bindValue(":state", Global::SeatSolded);
						query.bindValue(":actId", mCurrentActionId);
						for(int i = 0; i < mSelectedSeats.count() && !queryError; i++)
						{
							query.bindValue(":seatId", mSelectedSeats[i]->id());
							if(!query.exec())
								queryError = true;
						}
					}

					if(queryError)
					{
						query.exec("ROLLBACK TRANSACTION;");
						QMessageBox::critical(this, tr("Ошибка"), tr("Произошла ошибка при работе с сервером базы данных. Печать отменена.\nПроверьте подключение к интернету."));
					}
					else
					{
						query.exec("COMMIT TRANSACTION;");

						const static QPixmap printerBackground(":/clientImages/background.png");
						const static QPoint substratePoint(20, 375);
						const static QRect penaltyRect(635, 379, 135, 45);
						const static QRect priceRect(635, 660, 135, 40);
						const static QPoint translatePoint(553, 700);
						const static QRect titleRect(0, 0, 320, 20);
						const static QRect placeRect(0, 20, 320, 20);
						const static QRect dateTimeRect(0, 41, 320, 20);
						const static QRect seatRect(0, 61, 320, 20);
						const static QPoint barCodeTranslatePoint(45, 85);
						const static QSize barCodeRenderSize(230, 130);

						QPainter painter;
						painter.begin(&printer);
						QFont font = painter.font();
						font.setFamily("Arial");
						for(int i = 0; i < fanTickets.count(); i++)
						{
							painter.drawPixmap(0, 0, printerBackground);
							if(ticketSubstrateId)
								painter.drawPixmap(substratePoint, CImages::instance()->image(ticketSubstrateId));

							font.setBold(true);
							font.setPointSize(9);
							painter.setFont(font);

							painter.drawText(penaltyRect, Qt::AlignCenter, tr("При возврате билета\nудерживается сумма\nв размере %1 руб").arg(fanPenalty));
							painter.drawText(priceRect, Qt::AlignCenter, tr("Стоимость билета\n%1 руб").arg(mFanPriceForCurrentAction));

							font.setBold(false);
							painter.setFont(font);

							painter.save();
							painter.translate(translatePoint);
							painter.rotate(-90.0f);

							painter.drawText(titleRect, Qt::AlignCenter, ui->twActions->currentItem()->text(TITLE));
							painter.drawText(placeRect, Qt::AlignCenter, ui->twActions->currentItem()->text(PLACE));
							painter.drawText(dateTimeRect, Qt::AlignCenter, ui->twActions->currentItem()->text(DATETIME).left(16));	//16 - dd.MM.yyyy hh:mm
							painter.drawText(seatRect, Qt::AlignCenter, tr("Место: фан-зона"));

							painter.translate(barCodeTranslatePoint);
							CTicketIdentifier::generate(fanTickets[i]).render(barCodeRenderSize, &painter);
							painter.restore();

							if(i < fanTickets.count() - 1)	//if not last page
								printer.newPage();
						}
						if(seatTickets.count())
							printer.newPage();
						for(int i = 0; i < seatTickets.count(); i++)
						{
							painter.drawPixmap(0, 0, printerBackground);
							if(ticketSubstrateId)
								painter.drawPixmap(substratePoint, CImages::instance()->image(ticketSubstrateId));

							font.setBold(true);
							font.setPointSize(9);
							painter.setFont(font);

							painter.drawText(penaltyRect, Qt::AlignCenter, tr("При возврате билета\nудерживается сумма\nв размере %1 руб").arg(mSelectedSeats[i]->penalty()));
							painter.drawText(priceRect, Qt::AlignCenter, tr("Стоимость билета\n%1 руб").arg(mSelectedSeats[i]->price()));

							font.setBold(false);
							painter.setFont(font);

							painter.save();
							painter.translate(translatePoint);
							painter.rotate(-90.0f);

							painter.drawText(titleRect, Qt::AlignCenter, ui->twActions->currentItem()->text(TITLE));
							painter.drawText(placeRect, Qt::AlignCenter, ui->twActions->currentItem()->text(PLACE));
							painter.drawText(dateTimeRect, Qt::AlignCenter, ui->twActions->currentItem()->text(DATETIME).left(16));	//16 - dd.MM.yyyy hh:mm

							QString seat;
							if(mSelectedSeats[i]->text().isEmpty() == false)
								seat = tr("Место: ") + mSelectedSeats[i]->text();
							if(mSelectedSeats[i]->row().isEmpty() == false)
								seat += tr(" Ряд: ") + mSelectedSeats[i]->row();
							if(seat.isEmpty())
								seat = mSelectedSeats[i]->priceGroupTitle();
							else
								seat += " (" + mSelectedSeats[i]->priceGroupTitle() + ')';
							painter.drawText(seatRect, Qt::AlignCenter, seat);

							painter.translate(barCodeTranslatePoint);
							CTicketIdentifier::generate(seatTickets[i]).render(barCodeRenderSize, &painter);
							painter.restore();

							if(i < seatTickets.count() - 1)	//if not last page
								printer.newPage();
						}
						painter.end();

						emit showLeftPanel();
						ui->stackedWidget->setCurrentIndexAnimatedHorizontal(0);
					}
				}
			}
		}
	}
}

void CClientActionsWidget::on_sbxFanSellCount_valueChanged(int arg1)
{
	mScene.update();
}
