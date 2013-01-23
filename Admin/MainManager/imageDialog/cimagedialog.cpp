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
}

CImageDialog::CImageDialog(const QString &connectionName, const int imageId, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CImageDialog)
{
	ui->setupUi(this);
	mWasChanged = false;
	mConnectionName = connectionName;

	mId = imageId;

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
	mWasChanged = true;
}

void CImageDialog::on_pbnApply_clicked()
{
	if(mWasChanged)
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
			else
			{
				CImages::instance()->update(mId, mImage);
			}
			accept();
		}
	}
	close();
}
