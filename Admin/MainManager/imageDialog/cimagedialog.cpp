#include "cimagedialog.h"
#include "ui_cimagedialog.h"

//private:

void CImageDialog::updateImage()
{
	if(mImage.isNull() == false)
		ui->lImage->setPixmap(mImage.scaled(ui->lImage->size(), Qt::KeepAspectRatio));
}

void CImageDialog::resizeEvent(QResizeEvent *event)
{
	updateImage();
}

//public:

CImageDialog::CImageDialog(const QString &connectionName, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CImageDialog)
{
	ui->setupUi(this);
	mWasChanged = false;
	mConnectionName = connectionName;

	mId = 0;
	mAlternativeConstructor = false;
}

CImageDialog::CImageDialog(const QString &connectionName, const int imageId, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CImageDialog)
{
	ui->setupUi(this);
	mWasChanged = false;
	mConnectionName = connectionName;

	mId = imageId;
	mAlternativeConstructor = false;

	if(mId)
	{
		mImage = CImages::instance()->image(mId);
		if(mImage.isNull() == false)
		{
			mImageFileName = CImages::imageFileName(mId);
			ui->lImage->resize(400, 300);
			updateImage();
			ui->pbnRemove->setEnabled(true);
		}
	}
}

/**
Конструктор позволяющий автоматизировать работу с базой данных.
Выполняет все необходимые операции с полем \param imageIdFieldName (в котором хранится идентификатор изображения) в таблице \param tableName для условия \param condition.
Пример использования:
\code
CImageDialog imageDialog(mConnectionName, "Places", "id_background", "id = 17", this);
\endcode

При удалении изображения в поле \param imageIdFieldName запишется NULL.
*/
CImageDialog::CImageDialog(const QString &connectionName, const QString &tableName, const QString &imageIdFieldName, const QString &condition, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CImageDialog)
{
	ui->setupUi(this);
	mWasChanged = false;
	mConnectionName = connectionName;

	mId = 0;
	mAlternativeConstructor = true;
	mTable = tableName;
	mField = imageIdFieldName;
	mCondition = condition;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	QString queryText = tr("SELECT %1 FROM %2 WHERE %3").arg(mField).arg(mTable).arg(mCondition);
	if(query.exec(queryText) && query.first())
	{
		if(query.isNull(0) == false)
		{
			mId = query.value(0).toInt();
			if(mId)
			{
				mImage = CImages::instance()->image(mId);
				mImageFileName = CImages::imageFileName(mId);
				ui->lImage->resize(400, 300);
				updateImage();
				ui->pbnRemove->setEnabled(true);
			}
		}
	}
	else
		qDebug(qPrintable("CImageDialog::CImageDialog, alternativeConstructor. Sql error: " + query.lastError().text()));
}

CImageDialog::~CImageDialog()
{
	delete ui;
}

void CImageDialog::on_pbnCancel_clicked()
{
	mWasChanged = false;
	close();
}

void CImageDialog::on_pbnSelectNew_clicked()
{
	QString newFile = QFileDialog::getOpenFileName(this, tr("Выберите изображение"), "", tr("Изображения (*.png *.jpg *.jpeg)"));
	if(newFile.isEmpty() == false)
	{
		if(mImage.load(newFile))
		{
			mImageFileName = newFile;
			mWasChanged = true;
			updateImage();
			ui->pbnRemove->setEnabled(true);
		}
	}
}

void CImageDialog::on_pbnRemove_clicked()
{
	ui->lImage->clear();
	ui->lImage->setText(tr("Нет"));
	ui->pbnRemove->setEnabled(false);
	mImageFileName.clear();
	mImage = QPixmap();
	mWasChanged = true;
}

void CImageDialog::on_pbnApply_clicked()
{
	if(mWasChanged)
	{
		if(mAlternativeConstructor == false)
		{
			if(mId == 0 && mImageFileName.isEmpty() == false)	//insert
			{
				mId = CImages::instance()->insert(mImageFileName);
				accept();
			}
			else if(mId)
			{
				if(mImageFileName.isEmpty())	//delete
				{
					CImages::instance()->remove(mId);
					mId = 0;
				}
				else	//update
					CImages::instance()->update(mId, mImage);
				accept();
			}
		}
		else	//mAlternativeConstructor == true
		{
			QSqlQuery query(QSqlDatabase::database(mConnectionName));
			QString queryText;

			if(mId == 0 && mImageFileName.isEmpty() == false)	//insert
			{
				mId = CImages::instance()->insert(mImageFileName);
				queryText = tr("UPDATE %1 SET %2 = :imageId WHERE %3").arg(mTable).arg(mField).arg(mCondition);
				query.prepare(queryText);
				query.bindValue(":imageId", mId);
				if(!query.exec())
					qDebug(qPrintable("CImageDialog::on_pbnApply_clicked, alternativeConstructor. Insert sql error: " + query.lastError().text()));
				accept();
			}
			else if(mId)
			{
				if(mImageFileName.isEmpty())	//delete
				{
					CImages::instance()->remove(mId);
					queryText = tr("UPDATE %1 SET %2 = NULL WHERE %3").arg(mTable).arg(mField).arg(mCondition);
					if(!query.exec(queryText))
						qDebug(qPrintable("CImageDialog::on_pbnApply_clicked, alternativeConstructor. Update sql error: " + query.lastError().text()));
					mId = 0;
				}
				else	//update
					CImages::instance()->update(mId, mImage);
				accept();
			}
		}
	}
	close();
}
