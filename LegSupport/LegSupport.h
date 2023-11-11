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
    LegSupport(bool UsesHashData,std::unordered_map<std::string, std::string, MyHashFunction>& contrast,QWidget *parent = nullptr);
    ~LegSupport();
	//��������
	static bool DLG_data(bool UsesHashData, std::unordered_map<std::string, std::string, MyHashFunction>& contrast);

	//����contrastӳ��
	void useHashDate();
public slots:
	//ѡ��֧������
	void whileTypeA(bool status);
	void whileTypeB(bool status);
	void whileTypeC(bool status);
	//ѡ��֧�ȹ��
	void choiceDN(const QString &DN);
	//���ȷ��
	void invoke();
private:
    Ui::LegSupportClass ui_;

	QSqlDatabase db_;//���ݿ�

	QString legType_ = "A";//֧������

	std::unordered_map<std::string, std::string, MyHashFunction>& contrast_;
};


