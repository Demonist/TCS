#include "cplaceschemedialog.h"
#include "ui_cplaceschemedialog.h"

//private:

bool CPlaceSchemeDialog::eventFilter(QObject *obj, QEvent *event)
{
	static QGraphicsItem *movingItem = 0;

	if(event->type() == QEvent::MouseButtonRelease)
	{
		qDebug("MouseButtonRelease");
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

		if(obj == ui->gvScheme->viewport() && mouseEvent->button() == Qt::LeftButton)
		{
			if(mEditType == Add)
			{
				qDebug("add");	////

				QGraphicsPixmapItem *item = mScene.addPixmap(QPixmap(":/images/Zoom_in.png"));
				item->setPos(ui->gvScheme->mapToScene(mouseEvent->pos()));

				obj->event(event);
				event->accept();
				return true;
			}
			else if(mEditType == Delete)
			{
				qDebug("delete");	////

				QGraphicsItem *item = mScene.itemAt(ui->gvScheme->mapToScene(mouseEvent->pos()));
				if(item)
				{
					mScene.removeItem(item);
				}
			}
			else if(mEditType == Move && movingItem)
			{
				qDebug("moving stop");	////

				movingItem = 0;
			}
		}
	}
	else if(event->type() == QEvent::MouseButtonPress)
	{
		qDebug("MouseButtonPress");
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

		if(obj == ui->gvScheme->viewport() && mouseEvent->button() == Qt::LeftButton)
		{
			QGraphicsItem *item = mScene.itemAt(ui->gvScheme->mapToScene(mouseEvent->pos()));
			if(item)
			{
				qDebug("moving started");	////

				movingItem = item;
			}
		}
	}
	else if(event->type() == QEvent::MouseMove)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

		if(movingItem && mEditType == Move)
		{
			movingItem->setPos(ui->gvScheme->mapToScene(mouseEvent->pos()));
		}
	}

	return false;
}

//public:

CPlaceSchemeDialog::CPlaceSchemeDialog(const QString &connectionName, const int placeId, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CPlaceSchemeDialog)
{
	ui->setupUi(this);
	ui->hblMain->setStretch(0, 100);
	ui->hblMain->setStretch(1, 1);

////	setWindowState(windowState() | Qt::WindowMaximized);
	mEditType = None;
	mConnectionName = connectionName;
	mId = placeId;

	mScene.setSceneRect(0.0, 0.0, 1020.0, 730.0);
	ui->gvScheme->setScene(&mScene);
	ui->gvScheme->viewport()->installEventFilter(this);

	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	query.prepare("SELECT title FROM Places WHERE id = :id");
	query.bindValue(":id", mId);
	if(query.exec() && query.first())
		ui->lPlaceName->setText(query.value(0).toString());

	query.prepare("SELECT COUNT(id) FROM PlaceSchemes WHERE id_place = :id");
	query.bindValue(":id", mId);
	if(query.exec() && query.first())
		ui->lTotalSeatCount->setText(tr("Максимальное число мест в зале: %1").arg(query.value(0).toString()));
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
	qDebug("%d %d", mScene.items().size(), ui->gvScheme->items().size());
}

void CPlaceSchemeDialog::on_tbnZoomIn_clicked()
{
	ui->gvScheme->scale(1.2, 1.2);
}

void CPlaceSchemeDialog::on_tbnZoomOut_clicked()
{
	ui->gvScheme->scale(0.8, 0.8);
}

void CPlaceSchemeDialog::on_tbnZoomDefault_clicked()
{
	ui->gvScheme->resetMatrix();
}

void CPlaceSchemeDialog::on_tbnAdd_clicked(bool checked)
{
	if(checked)
	{
		ui->tbnMove->setChecked(false);
		ui->tbnDel->setChecked(false);
		ui->tbnDrag->setChecked(false);
		mEditType = Add;
	}
	else
		mEditType = None;
}

void CPlaceSchemeDialog::on_tbnMove_clicked(bool checked)
{
	if(checked)
	{
		ui->tbnAdd->setChecked(false);
		ui->tbnDel->setChecked(false);
		ui->tbnDrag->setChecked(false);
		mEditType = Move;
	}
	else
		mEditType = None;
}

void CPlaceSchemeDialog::on_tbnDel_clicked(bool checked)
{
	if(checked)
	{
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
		ui->tbnAdd->setChecked(false);
		ui->tbnMove->setChecked(false);
		ui->tbnDel->setChecked(false);
		mEditType = Drag;
	}
	else
		mEditType = None;
}
