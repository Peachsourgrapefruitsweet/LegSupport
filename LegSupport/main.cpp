#include "LegSupport.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	std::unordered_map<std::string, std::string, MyHashFunction> contrast;
	contrast["shell_id"] = "99";
	contrast["shell_thk"] = "99";
	contrast["LegType"] = "A";
	contrast["leg_kn"] = "99";
	contrast["leg_dn"] = "300";
	contrast["leg_toweldline"] = "99";
	contrast["leg_startangle"] = "99";
	contrast["leg_num"] = "4";
	contrast["leg_shetltotl"] = "99";
	contrast["leg_H0max"] = "999";
	contrast["leg_H0"] = "99";
	contrast["leg_bb"] = "99";
	contrast["leg_dd"] = "99";
	contrast["leg_lh"] = "99";
	contrast["leg_hf"] = "99";
	contrast["leg_bd"] = "99";
	contrast["leg_detb"] = "99";
	contrast["leg_l1"] = "99";
	contrast["leg_l2"] = "99";
	contrast["leg_detc"] = "99";
	contrast["leg_aq"] = "99";
	contrast["leg_ax"] = "99";
	contrast["leg_deta"] = "99";
	contrast["leg_l0"] = "99";
	contrast["leg_boltdiameter"] = "99";
	contrast["leg_boltm"] = "M99";
	contrast["leg_bcd"] = "99";
	contrast["leg_kg1"] = "99";
	contrast["leg_kg2"] = "99";
	contrast["leg_kg3"] = "99";
	contrast["leg_kg4"] = "99";
	contrast["leg_w"] = "99";
	contrast["leg_t1"] = "99";
	contrast["leg_t2"] = "99";
	contrast["leg_padmt"] = "Q99R";
	contrast["leg_beammt"] = "Q99B";
	contrast["leg_paddey"] = "99";
	contrast["leg_beamdey"] = "99";
	contrast["leg_standard"] = "NB/T 47065-2018";
	contrast["leg_bomsize"] = "A4-900-12A编号-支承高度H-垫板厚度deta";
    LegSupport::DLG_data(true, contrast);
    //w.show();
    return a.exec();
}
