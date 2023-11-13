
#include "LegSupport.h"

#include <QtSql>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QPixmap>
#include <QTextCodec>

LegSupport::LegSupport(bool UsesHashData,std::unordered_map<std::string, std::string, MyHashFunction>& contrast, QWidget *parent)
	: QDialog(parent),
	contrast_(contrast)
{
	ui_.setupUi(this);
	//�������ݿ�
	db_ = QSqlDatabase::addDatabase("QSQLITE");
	db_.setDatabaseName("LegSupport.db");
	db_.setUserName("root");
	db_.setPassword("1238");


	if (!db_.open()) {
		QMessageBox::critical(this, QStringLiteral("����"), QStringLiteral("���ݿ��ʧ�ܣ�"));
		qCritical() << QStringLiteral("���ݿ��ʧ�ܣ�");
		return;
	}


	//�ؼ�������ʾ��ʵ��
	connect(ui_.radioButton_A, SIGNAL(toggled(bool)), this, SLOT(whileTypeA(bool)));
	connect(ui_.radioButton_B, SIGNAL(toggled(bool)), this, SLOT(whileTypeB(bool)));
	connect(ui_.radioButton_C, SIGNAL(toggled(bool)), this, SLOT(whileTypeC(bool)));
	ui_.radioButton_C->setChecked(true);
	ui_.radioButton_A->setChecked(true);

	//ѡ��֧�ȹ���Զ���ʾ
	connect(ui_.comboBox_leg_dn, SIGNAL(currentTextChanged(const QString &)), this, SLOT(choiceDN(const QString &)));

	//��������Բ
	connect(ui_.pushButton_ComputingCenterCircle,SIGNAL(clicked()),this,SLOT(coputingCenterCircle()));

	//ȷ��ȡ��
	QObject::connect(ui_.buttonBox, SIGNAL(accepted()), this, SLOT(invoke()));
	QObject::connect(ui_.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	if (UsesHashData)
	{
		useHashDate();
	}
	else
	{
		choiceDN(ui_.comboBox_leg_dn->currentText());
	}
	// ��ȡ�����ӿؼ�
	QList<QWidget*> allWidgets = findChildren<QWidget*>();
	QList<QWidget*> inputWidget;
	for (auto& widget : allWidgets)
	{
		if (QWidget* childWidget = qobject_cast<QWidget*>(widget)) 
		{
			QString objectName = childWidget->objectName();
			if (objectName.startsWith("lineEdit_")||objectName.startsWith("comboBox_")|| objectName == "groupBox_LegType")
			{
				inputWidget.push_back(childWidget);
				qDebug() << QStringLiteral("�ؼ����ƣ�") << objectName;
			}
		}
	}

	// ���ļ�
	QFile file("LegSupport_contrast.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
	{
		qDebug() << QStringLiteral("�޷����ļ�");
		return;
	}

	//��ȡ�ļ�����
	QTextStream in(&file);
	in.setCodec("UTF-8");
	QList<QStringList> readResult;
	while (!in.atEnd()) 
	{
		QString line = in.readLine().trimmed();
		// ʹ��tab�����зָ�
		QStringList fields = line.split("\t");
		if (fields.size() >= 4) {
			fields.removeAll("");
			readResult.push_back(fields);
		}
		else {
			qDebug() << QStringLiteral("��Ч���У�") << line;
		}
	}
	file.close();

	QStringList key1s, key2s;
	for (auto & it : inputWidget)
	{
		key1s.push_back(it->objectName().section('_', 1));
	}
	for (auto & it : readResult)
	{
		key2s.push_back(it.at(3));
	}
	QStringList key1NotInKey2 = key1s.toSet().subtract(key2s.toSet()).toList();
	QStringList key2NotInKey1 = key2s.toSet().subtract(key1s.toSet()).toList();

	qDebug() << QStringLiteral("��Ŀؼ�key��") << key1NotInKey2;
	qDebug() << QStringLiteral("�ٵĿؼ�key��") << key2NotInKey1;
	
}

LegSupport::~LegSupport()
{
	if (db_.isOpen()) {
		db_.close();
	}
}

bool LegSupport::DLG_data(bool UsesHashData, std::unordered_map<std::string, std::string, MyHashFunction>& contrast)
{
	LegSupport *legSupportDiolag = new LegSupport(UsesHashData,contrast);
	if (legSupportDiolag->exec() == QDialog::Accepted)
	{
		delete legSupportDiolag;
		return true;
	}
	return false;
}

void LegSupport::useHashDate()
{
	legType_ = QString::fromLocal8Bit(contrast_["LegType"].c_str());
	if (legType_ == "A")
	{
		//only A_LegSupport
		ui_.lineEdit_leg_bb->setText(QString::fromLocal8Bit(contrast_["leg_bb"].c_str()));
		ui_.lineEdit_leg_dd->setText(QString::fromLocal8Bit(contrast_["leg_dd"].c_str()));
		ui_.lineEdit_leg_lh->setText(QString::fromLocal8Bit(contrast_["leg_lh"].c_str()));
		ui_.lineEdit_leg_l1->setText(QString::fromLocal8Bit(contrast_["leg_l1"].c_str()));//�ǰ�߳�
		ui_.textBrowser->setHtml(
			QStringLiteral("<p>֧�����ͣ�%1</p>").arg(QString::fromLocal8Bit(contrast_["LegType"].c_str())) +
			QStringLiteral("<p>KN: %1</p>").arg(QString::fromLocal8Bit(contrast_["leg_kn"].c_str())) +
			QStringLiteral("<p>����������߾�: %1</p>").arg(QString::fromLocal8Bit(contrast_["leg_shetltotl"].c_str())) +
			QStringLiteral("<p>����Բ-����(D): %1</p>").arg(QString::fromLocal8Bit(contrast_["leg_bcd"].c_str())) +
			//QStringLiteral("<p>H<sub>1</sub><sup>b</sup>��%1</p>").arg(QString::fromLocal8Bit(contrast_["leg_lh"].c_str())) +
			QStringLiteral("<p>֧������: %1</p>").arg(QString::fromLocal8Bit(contrast_["leg_kg1"].c_str())) +
			QStringLiteral("<p>�װ�����: %1</p>").arg(QString::fromLocal8Bit(contrast_["leg_kg2"].c_str())) +
			QStringLiteral("<p>�ǰ�����: %1</p>").arg(QString::fromLocal8Bit(contrast_["leg_kg3"].c_str())) +
			QStringLiteral("<p>��������������壩: %1</p>").arg(QString::fromLocal8Bit(contrast_["leg_kg4"].c_str()))
		);
	}
	else if (legType_ == "C")
	{
		//only C_LegSupport
		ui_.lineEdit_leg_w->setText(QString::fromLocal8Bit(contrast_["leg_w"].c_str()));//���ָֹ��W
		ui_.lineEdit_leg_t1->setText(QString::fromLocal8Bit(contrast_["leg_t1"].c_str()));//���ָֺ��t1
		ui_.lineEdit_leg_t2->setText(QString::fromLocal8Bit(contrast_["leg_t2"].c_str()));//���ָֺ��t2
		ui_.lineEdit_leg_l0->setText(QString::fromLocal8Bit(contrast_["leg_l0"].c_str()));//�ؽ���˨�׼��L0
		ui_.lineEdit_leg_l0->setText(QString::fromLocal8Bit(contrast_["leg_l0"].c_str()));//�ǰ�߳�
		ui_.lineEdit_leg_l2->setText(QString::fromLocal8Bit(contrast_["leg_l2"].c_str()));//�ǰ���


		ui_.textBrowser->setHtml(
			QStringLiteral("<p>֧�����ͣ�%1</p>").arg(QString::fromLocal8Bit(contrast_["LegType"].c_str())) +
			QStringLiteral("<p>KN: %1</p>").arg(QString::fromLocal8Bit(contrast_["leg_kn"].c_str())) +
			QStringLiteral("<p>����������߾�: %1</p>").arg(QString::fromLocal8Bit(contrast_["leg_shetltotl"].c_str())) +
			//QStringLiteral("<p>����Բ-����(D): %1</p>").arg(query.value(QStringLiteral("D")).toString()) +
			//QStringLiteral("<p>H<sub>1</sub><sup>b</sup>��%1</p>").arg(QString::fromLocal8Bit(contrast_["leg_lh"].c_str())) +
			QStringLiteral("<p>֧������: %1</p>").arg(QString::fromLocal8Bit(contrast_["leg_kg1"].c_str())) +
			QStringLiteral("<p>�װ�����: %1</p>").arg(QString::fromLocal8Bit(contrast_["leg_kg2"].c_str())) +
			QStringLiteral("<p>�ǰ�����: %1</p>").arg(QString::fromLocal8Bit(contrast_["leg_kg3"].c_str())) +
			QStringLiteral("<p>������: %1</p>").arg(QString::fromLocal8Bit(contrast_["leg_kg4"].c_str()))
		);
	}

	// �Ҳ��� todo------------------------------
	ui_.lineEdit_leg_detc->setText(QString::fromLocal8Bit(contrast_["leg_detc"].c_str()));//�ǰ���

	ui_.lineEdit_leg_bd->setText(QString::fromLocal8Bit(contrast_["leg_bd"].c_str()));//�װ�߳�
	ui_.lineEdit_leg_detb->setText(QString::fromLocal8Bit(contrast_["leg_detb"].c_str()));//�װ���
	ui_.lineEdit_leg_boltdiameter->setText(QString::fromLocal8Bit(contrast_["leg_boltdiameter"].c_str()));//�ؽſ׾�
	ui_.lineEdit_leg_boltm->setText(QString::fromLocal8Bit(contrast_["leg_boltm"].c_str()));
	ui_.lineEdit_leg_deta->setText(QString::fromLocal8Bit(contrast_["leg_deta"].c_str()));//��Ͳ�������-������
	ui_.lineEdit_leg_aq->setText(QString::fromLocal8Bit(contrast_["leg_aq"].c_str()));//�����
	ui_.lineEdit_leg_ax->setText(QString::fromLocal8Bit(contrast_["leg_ax"].c_str()));//��峤��
	ui_.lineEdit_leg_hf->setText(QString::fromLocal8Bit(contrast_["leg_hf"].c_str()));//���쳤��-��庸��
	ui_.lineEdit_leg_padmt->setText(QString::fromLocal8Bit(contrast_["leg_padmt"].c_str()));//������-������
	ui_.lineEdit_leg_paddey->setText(QString::fromLocal8Bit(contrast_["leg_paddey"].c_str()));//����ܶ�
	ui_.lineEdit_leg_toweldline->setText(QString::fromLocal8Bit(contrast_["leg_toweldline"].c_str()));//֧�ȶ�λ�߶�
	ui_.lineEdit_leg_startangle->setText(QString::fromLocal8Bit(contrast_["leg_startangle"].c_str()));//֧����ʼ�Ƕ�
	ui_.lineEdit_leg_H0->setText(QString::fromLocal8Bit(contrast_["leg_H0"].c_str()));//֧�ȸ߶ȣ�Ĭ��Ϊ���֧�Ÿ߶�
	ui_.lineEdit_shell_id->setText(QString::fromLocal8Bit(contrast_["shell_id"].c_str()));//Ͳ���ھ�-������
	ui_.lineEdit_leg_standard->setText(QString::fromLocal8Bit(contrast_["leg_standard"].c_str()));//֧�Ȳο��淶
	ui_.lineEdit_leg_num->setText(QString::fromLocal8Bit(contrast_["leg_num"].c_str()));//֧������
	ui_.lineEdit_shell_thk->setText(QString::fromLocal8Bit(contrast_["shell_thk"].c_str()));//Ͳ��ں�-������
	ui_.lineEdit_leg_bomsize->setText(QString::fromLocal8Bit(contrast_["leg_bomsize"].c_str()));//֧����ϸ������
}

void LegSupport::whileTypeA(bool status)
{
	if (status == true)
	{

		legType_ = "A";
		// ����ͼƬ
		QPixmap pixmap(":/LegSupport/res/image/A_LegSupport.png");
		ui_.label->setPixmap(pixmap);
		ui_.label->setScaledContents(true);

		QSqlQuery query(db_);
		if (!query.exec("SELECT DN FROM A_LegSupport"))
		{
			qCritical(query.lastError().text().toLocal8Bit());
			QMessageBox::critical(this, QStringLiteral("����"), query.lastError().text());
			return;
		}
		QStringList DNs;
		while (query.next())
		{
			QString str = query.value(0).toString();
			DNs.append(str);
		}
		ui_.comboBox_leg_dn->clear();
		ui_.comboBox_leg_dn->addItems(DNs);
		{
			ui_.lineEdit_leg_bb->show();
			ui_.lineEdit_leg_dd->show();
			ui_.lineEdit_leg_lh->show();
			ui_.label_leg_bb->show();
			ui_.label_leg_dd->show();
			ui_.label_leg_lh->show();
		}
	}
	else
	{
		{
			ui_.lineEdit_leg_bb->hide();
			ui_.lineEdit_leg_dd->hide();
			ui_.lineEdit_leg_lh->hide();
			ui_.label_leg_bb->hide();
			ui_.label_leg_dd->hide();
			ui_.label_leg_lh->hide();
		}
	}
}
void LegSupport::whileTypeB(bool status)
{

	if (status == true)
	{
		legType_ = "B";
	}
	else
	{

	}
}
void LegSupport::whileTypeC(bool status)
{
	if (status == true)
	{
		legType_ = "C";
		// ����ͼƬ
		QPixmap pixmap(":/LegSupport/res/image/C_LegSupport.png");
		ui_.label->setPixmap(pixmap);
		ui_.label->setScaledContents(true);

		QSqlQuery query(db_);
		if (!query.exec("SELECT DN FROM C_LegSupport"))
		{
			qCritical(query.lastError().text().toLocal8Bit());
			QMessageBox::critical(this, QStringLiteral("����"), query.lastError().text());
			return;
		}
		QStringList DNs;
		while (query.next())
		{
			QString str = query.value(0).toString();
			DNs.append(str);
		}
		ui_.comboBox_leg_dn->clear();
		ui_.comboBox_leg_dn->addItems(DNs);

		{
			ui_.lineEdit_leg_t1->show();
			ui_.lineEdit_leg_t2->show();
			ui_.lineEdit_leg_w->show();
			ui_.lineEdit_leg_l2->show();
			ui_.lineEdit_leg_l0->show();
			ui_.label_leg_t1->show();
			ui_.label_leg_t2->show();
			ui_.label_leg_w->show();
			ui_.label_leg_l2->show();
			ui_.label_leg_l0->show();
		}
	}
	else
	{
		{			
			ui_.lineEdit_leg_t1->hide();
			ui_.lineEdit_leg_t2->hide();
			ui_.lineEdit_leg_w->hide();
			ui_.lineEdit_leg_l2->hide();
			ui_.lineEdit_leg_l0->hide();
			ui_.label_leg_t1->hide();
			ui_.label_leg_t2->hide();
			ui_.label_leg_w->hide();
			ui_.label_leg_l2->hide();
			ui_.label_leg_l0->hide();
		}
	}
}

void LegSupport::choiceDN(const QString & DN)
{
	double dn = DN.toDouble();
	QSqlQuery query(db_);
	query.prepare("SELECT * FROM " + legType_ + "_LegSupport WHERE DN = :dn");
	query.bindValue(":dn", dn);
	if (query.exec()) {
		while (query.next()) 
		{
			if (legType_ == "A")
			{
				//only A_LegSupport
				ui_.lineEdit_leg_bb->setText(query.value(QStringLiteral("�Ǹֳ�")).toString());
				ui_.lineEdit_leg_dd->setText(query.value(QStringLiteral("�Ǹֺ�")).toString());
				ui_.lineEdit_leg_lh->setText(query.value(QStringLiteral("�Ǹֳ���")).toString());
				ui_.lineEdit_leg_l1->setText(query.value("l").toString());//�ǰ�߳�
				ui_.textBrowser->setHtml(
					QStringLiteral("<p>֧�����ͣ�%1</p>").arg(query.value(QStringLiteral("֧������")).toString()) +
					QStringLiteral("<p>KN: %1</p>").arg(query.value(QStringLiteral("KN")).toString()) +
					QStringLiteral("<p>����������߾�: %1</p>").arg(query.value(QStringLiteral("����������߾�")).toString()) +
					QStringLiteral("<p>����Բ-����(D): %1</p>").arg(query.value(QStringLiteral("D")).toString()) +
					QStringLiteral("<p>H<sub>1</sub><sup>b</sup>��%1</p>").arg(query.value(QStringLiteral("H2")).toString()) +
					QStringLiteral("<p>֧������: %1</p>").arg(query.value(QStringLiteral("֧������")).toString()) +
					QStringLiteral("<p>�װ�����: %1</p>").arg(query.value(QStringLiteral("�װ�����")).toString()) +
					QStringLiteral("<p>�ǰ�����: %1</p>").arg(query.value(QStringLiteral("�ǰ�����")).toString()) +
					QStringLiteral("<p>��������������壩: %1</p>").arg(query.value(QStringLiteral("��������������壩")).toString())
				);
			}
			else if (legType_ == "C")
			{
				//only C_LegSupport
				ui_.lineEdit_leg_w->setText(query.value("W").toString());//���ָֹ��W
				ui_.lineEdit_leg_t1->setText(query.value("t1").toString());//���ָֺ��t1
				ui_.lineEdit_leg_t2->setText(query.value("t2").toString());//���ָֺ��t2
				ui_.lineEdit_leg_l0->setText(query.value("l0").toString());//�ؽ���˨�׼��L0
				ui_.lineEdit_leg_l0->setText(query.value("l1").toString());//�ǰ�߳�
				ui_.lineEdit_leg_l2->setText(query.value("l2").toString());//�ǰ���


				ui_.textBrowser->setHtml(
					QStringLiteral("<p>֧�����ͣ�%1</p>").arg(query.value(QStringLiteral("֧������")).toString()) +
					QStringLiteral("<p>KN: %1</p>").arg(query.value(QStringLiteral("KN")).toString()) +
					QStringLiteral("<p>����������߾�: %1</p>").arg(query.value(QStringLiteral("����������߾�")).toString()) +
					//QStringLiteral("<p>����Բ-����(D): %1</p>").arg(query.value(QStringLiteral("D")).toString()) +
					QStringLiteral("<p>H<sub>1</sub><sup>b</sup>��%1</p>").arg(query.value(QStringLiteral("H2")).toString()) +
					QStringLiteral("<p>֧������: %1</p>").arg(query.value(QStringLiteral("֧������")).toString()) +
					QStringLiteral("<p>�װ�����: %1</p>").arg(query.value(QStringLiteral("�װ�����")).toString()) +
					QStringLiteral("<p>�ǰ�����: %1</p>").arg(query.value(QStringLiteral("�ǰ�����")).toString()) +
					QStringLiteral("<p>������: %1</p>").arg(query.value(QStringLiteral("������")).toString())
				);
			}

			// �Ҳ��� todo------------------------------
			//ui_.lineEdit_leg_detc->setText(query.value(QStringLiteral("֧������")).toString());//�ǰ���

			ui_.lineEdit_leg_bd->setText(query.value("B").toString());//�װ�߳�
			ui_.lineEdit_leg_detb->setText(query.value(QStringLiteral("��b")).toString());//�װ���
			ui_.lineEdit_leg_boltdiameter->setText(query.value("db").toString());//�ؽſ׾�
			ui_.lineEdit_leg_boltm->setText(query.value(QStringLiteral("�ؽ���˨���")).toString());
			ui_.lineEdit_leg_aq->setText(query.value(QStringLiteral("A��")).toString());//�����
			ui_.lineEdit_leg_ax->setText(query.value("Ax").toString());//��峤��
			ui_.lineEdit_leg_hf->setText(query.value("hf").toString());//���쳤��-��庸��
			ui_.lineEdit_leg_padmt->setText("Q345R");//������-������
			ui_.lineEdit_leg_paddey->setText("7.85");//����ܶ�
			ui_.lineEdit_leg_toweldline->setText("50");//֧�ȶ�λ�߶�
			ui_.lineEdit_leg_startangle->setText("0");//֧����ʼ�Ƕ�
			ui_.lineEdit_leg_H0->setText(query.value(QStringLiteral("���֧�и߶�")).toString());//֧�ȸ߶ȣ�Ĭ��Ϊ���֧�Ÿ߶�
			ui_.lineEdit_leg_standard->setText("NB/T 47065-2018");//֧�Ȳο��淶
			ui_.lineEdit_leg_num->setText(query.value(QStringLiteral("����")).toString());//֧������
			ui_.lineEdit_leg_bomsize->setText(QStringLiteral("A4-900-12A���-֧�и߶�H-�����deta"));//֧����ϸ������

			//ui_.lineEdit_leg_deta->setText(QStringLiteral("������"));//��Ͳ�������-������
			//ui_.lineEdit_shell_id->setText(QStringLiteral("������"));//Ͳ���ھ�-������
			//ui_.lineEdit_shell_thk->setText(QStringLiteral("������"));//Ͳ��ں�-������

		}
	}
	else {
		qCritical(query.lastError().text().toLocal8Bit());
	}
}

void LegSupport::coputingCenterCircle()
{
	double leg_bcd_ang1 = atan(1.0 / 2.0);
	double leg_bcd_ang2 = 0.25*M_PI - leg_bcd_ang1;
	double leg_bcd_c1 = ui_.lineEdit_leg_dd->text().toDouble() / 3.0 / sin(leg_bcd_ang1);
	double leg_bcd_c2 = leg_bcd_c1 * cos(leg_bcd_ang2);

	double leg_bcd_y1 = 0.5*ui_.lineEdit_leg_bb->text().toDouble()*sqrt(2) - leg_bcd_c2;
	double leg_bcd_c3 = ui_.lineEdit_shell_id->text().toDouble() + ui_.lineEdit_shell_thk->text().toDouble() + ui_.lineEdit_leg_deta->text().toDouble() + ui_.lineEdit_leg_dd->text().toDouble() / 3.0;
	double leg_bcd_x1 = sqrt(leg_bcd_c3*leg_bcd_c3-leg_bcd_y1*leg_bcd_y1)+ leg_bcd_c1*sin(leg_bcd_ang2);

	double leg_bcd = 2 * leg_bcd_x1 - 40;

	QString currentHtml = ui_.textBrowser->toHtml(); //���ڵ�����
	QString additionalContent = QStringLiteral("<p>����Բ��%1</p>").arg(QString::number(leg_bcd));//�µ�����
	ui_.textBrowser->setHtml(currentHtml + additionalContent);
}

void LegSupport::invoke()
{

	QTextCodec *code = QTextCodec::codecForName("GB2312");//�������·������
	contrast_["LegType"] = code->fromUnicode(legType_).data();
	if (legType_ == "A")
	{
		//only A_LegSupport
		contrast_["leg_bb"] = code->fromUnicode(ui_.lineEdit_leg_bb->text()).data();
		contrast_["leg_dd"] = code->fromUnicode(ui_.lineEdit_leg_dd->text()).data();
		contrast_["leg_lh"] = code->fromUnicode(ui_.lineEdit_leg_lh->text()).data();
		contrast_["leg_l1"] = code->fromUnicode(ui_.lineEdit_leg_l1->text()).data();
	}
	else if (legType_ == "C")
	{
		//only C_LegSupport
		contrast_["leg_w"] = code->fromUnicode(ui_.lineEdit_leg_w->text()).data();
		contrast_["leg_t1"] = code->fromUnicode(ui_.lineEdit_leg_t1->text()).data();
		contrast_["leg_t2"] = code->fromUnicode(ui_.lineEdit_leg_t2->text()).data();
		contrast_["leg_l0"] = code->fromUnicode(ui_.lineEdit_leg_l0->text()).data();
		contrast_["leg_l0"] = code->fromUnicode(ui_.lineEdit_leg_l0->text()).data();
		contrast_["leg_l2"] = code->fromUnicode(ui_.lineEdit_leg_l2->text()).data();
	}

	contrast_["leg_bd"] = code->fromUnicode(ui_.lineEdit_leg_bd->text()).data();					  //�װ�߳�
	contrast_["leg_detb"] = code->fromUnicode(ui_.lineEdit_leg_detb->text()).data();				  //�װ���
	contrast_["leg_boltdiameter"] = code->fromUnicode(ui_.lineEdit_leg_boltdiameter->text()).data();  //�ؽſ׾�
	contrast_["leg_boltm"] = code->fromUnicode(ui_.lineEdit_leg_boltm->text()).data();
	contrast_["leg_deta"] = code->fromUnicode(ui_.lineEdit_leg_deta->text()).data();				  //��Ͳ�������
	contrast_["leg_aq"] = code->fromUnicode(ui_.lineEdit_leg_aq->text()).data();					  //�����
	contrast_["leg_ax"] = code->fromUnicode(ui_.lineEdit_leg_ax->text()).data();					  //��峤��
	contrast_["leg_hf"] = code->fromUnicode(ui_.lineEdit_leg_hf->text()).data();					  //���쳤��-��庸��
	contrast_["leg_padmt"] = code->fromUnicode(ui_.lineEdit_leg_padmt->text()).data();				  //������
	contrast_["leg_paddey"] = code->fromUnicode(ui_.lineEdit_leg_paddey->text()).data();			  //����ܶ�
	contrast_["leg_toweldline"] = code->fromUnicode(ui_.lineEdit_leg_toweldline->text()).data();	  //֧�ȶ�λ�߶�
	contrast_["leg_startangle"] = code->fromUnicode(ui_.lineEdit_leg_startangle->text()).data();	  //֧����ʼ�Ƕ�
	contrast_["leg_H0"] = code->fromUnicode(ui_.lineEdit_leg_H0->text()).data();					  //֧�ȸ߶ȣ�Ĭ��Ϊ���֧�Ÿ߶�
	contrast_["shell_id"] = code->fromUnicode(ui_.lineEdit_shell_id->text()).data();				  //Ͳ���ھ�
	contrast_["leg_standard"] = code->fromUnicode(ui_.lineEdit_leg_standard->text()).data();		  //֧�Ȳο��淶
	contrast_["leg_num"] = code->fromUnicode(ui_.lineEdit_leg_num->text()).data();					  //֧������
	contrast_["shell_thk"] = code->fromUnicode(ui_.lineEdit_shell_thk->text()).data();				  //Ͳ��ں�
	contrast_["leg_bomsize"] = code->fromUnicode(ui_.lineEdit_leg_bomsize->text()).data();			  //֧����ϸ������
	contrast_["leg_detc"] = code->fromUnicode(ui_.lineEdit_leg_detc->text()).data();			  //�ǰ���
	accept();
}
