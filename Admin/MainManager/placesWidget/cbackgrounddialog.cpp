#include "cbackgrounddialog.h"
#include "ui_cbackgrounddialog.h"

//private:

void CBackgroundDialog::updateImage()
{
	if(mImage.isNull() == false)
		ui->lImage->setPixmap(mImage.scaled(ui->lImage->size(), Qt::KeepAspectRatio));
}

void CBackgroundDialog::resizeEvent(QResizeEvent *event)
{
	updateImage();
}

//public:

CBackgroundDialog::CBackgroundDialog(const QString &connectionName, const int placeId, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CBackgroundDialog)
{
	ui->setupUi(this);

	mWasChanged = false;

	mConnectionName = connectionName;
	mId = placeId;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("SELECT id_background FROM Places WHERE id = :id;");
	query.bindValue(":id", mId);
	if(query.exec() && query.first())
	{
		if(query.isNull(0) == false)
		{
			mImage = CImages::instance()->image(query.value(0).toInt());
			mImageFileName = CImages::imageFileName(query.value(0).toInt());
			ui->lImage->resize(400, 300);
			updateImage();
			ui->pbnRemove->setEnabled(true);
		}
	}
}

CBackgroundDialog::~CBackgroundDialog()
{
	delete ui;
}

void CBackgroundDialog::on_pbnCancel_clicked()
{
	mWasChanged = false;
	close();
}

void CBackgroundDialog::on_pbnSelectNew_clicked()
{
	QString newFile = QFileDialog::getOpenFileName(this, tr("Выберите изображение для подложки"), "", tr("Изображения (*.png *.jpg *.jpeg)"));
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

void CBackgroundDialog::on_pbnRemove_clicked()
{
	ui->lImage->clear();
	ui->lImage->setText(tr("Нет"));
	ui->pbnRemove->setEnabled(false);
	mImageFileName.clear();
	mWasChanged = true;
}

void CBackgroundDialog::on_pbnApply_clicked()
{
	if(mWasChanged)
	{
		QSqlQuery query(QSqlDatabase::database(mConnectionName));
		query.prepare("SELECT id_background FROM Places WHERE id = :id;");
		query.bindValue(":id", mId);
		if(query.exec() && query.first())
		{
			if(query.isNull(0))
			{
				if(mImageFileName.isEmpty() == false)	//insert
				{
					int imageId = CImages::instance()->insert(mImageFileName);
					if(imageId)
					{
						query.prepare("UPDATE Places SET id_background = :imageId WHERE id = :id;");
						query.bindValue(":imageId", imageId);
						query.bindValue(":id", mId);
						query.exec();
					}
				}
				else
					mWasChanged = false;
			}
			else
			{
				int imageId = query.value(0).toInt();

				if(mImageFileName.isEmpty())	//delete
				{
					CImages::instance()->remove(imageId);
					query.prepare("UPDATE Places SET id_background = NULL WHERE id = :id;");
					query.bindValue(":id", mId);
					query.exec();
				}
				else	//update
				{
					CImages::instance()->update(imageId, mImage);
				}
			}
		}
	}
	close();
}
