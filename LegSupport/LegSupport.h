#pragma once

#include <QtWidgets/QDialog>
#include <unordered_map>
#include "ui_LegSupport.h"
#include <QSqlDatabase>



struct MyHashFunction
{
	mutable std::map<std::string, size_t> indexs;
	std::size_t operator()(const std::string& key) const 
	{
		std::map<std::string, size_t>::iterator iterator = indexs.find(key);
		if (iterator == indexs.end())
		{
			indexs[key] = indexs.size();
		}
		return indexs[key];
	}
};


class LegSupport : public QDialog
{
    Q_OBJECT

public:
    LegSupport(std::unordered_map<std::string, std::string, MyHashFunction>& contrast,QWidget *parent = nullptr);
    ~LegSupport();

	static bool DLG_data(bool UsesHashData, std::unordered_map<std::string, std::string, MyHashFunction>& contrast);
public slots:
	//ѡ��֧������
	void whileTypeA(bool status);
	void whileTypeB(bool status);
	void whileTypeC(bool status);
	//ѡ��֧�ȹ��
	void choiceDN(QString &DN);
	//���ȷ��
	void invoke();
private:
    Ui::LegSupportClass ui;

	QSqlDatabase db_;//���ݿ�

	QString legType_;//֧������

	std::unordered_map<std::string, std::string, MyHashFunction>& contrast;
};


