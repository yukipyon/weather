#include "plane.hpp"
#include <QPainter>
#include <iostream>
#include <QFile>

#include "weather_day.hpp"
#include "parser.hpp"
#include <vector>
#include <QVBoxLayout>
#include <sstream>
#include <QDesktopWidget>

plane::plane(QWidget *parent) : QWidget(parent) {

	// we probably wont need these...
	curly = new Curly();
	resizer = NULL;
	//
	
	setAttribute(Qt::WA_TranslucentBackground);
	//setAttribute(Qt::NoSystemBackground);
	setWindowFlags(Qt::FramelessWindowHint);
	//setWindowFlags(Qt::WindowStaysOnBottomHint);
	//setWindowFlags(Qt::X11BypassWindowManagerHint);
	setAttribute(Qt::WA_X11DoNotAcceptFocus);
	
	this->corner = 15;

	this->colour = QColor(11, 11, 44, 127);

	//QLabel *mi = new QLabel(this) ;
	//mi->setGeometry(0,0,100,100);
	//QPixmap pix(QString::fromStdString("test.png"));
	//mi->setPixmap(pix);

	get_data();

//mi->show();
 
}

void plane::paintEvent(QPaintEvent *e) {

	Q_UNUSED(e);
	QPainter qp(this);
	drawLines(&qp);
	QDesktopWidget *desktop = QApplication::desktop();
	int width = desktop->width();
	move((width - this->width())-1, 410);
	
}

void plane::drawLines(QPainter *qp) {

	qp->setOpacity(1.0);
	//QPen pen(Qt::red, 10, Qt::SolidLine);
	QPen pen(Qt::NoPen);
	qp->setPen(pen);

 	QBrush brush;
	brush.setStyle(Qt::SolidPattern); // Fix your problem !
      	brush.setColor(this->colour);
	qp->setBrush(brush);
/*

	QPainterPath p;
	p.lineTo( width-corner, 0);
	p.arcTo(width-(corner*2), 0, corner*2, corner*2, 0, corner*16*3);
	p.lineTo(width, height-corner);
	p.arcTo(width-(corner*2), height-(corner*2),  corner*2, corner*2, 0, -corner*16*3);
	p.lineTo(0+corner, height);
	p.arcTo(0, height-(corner*2),  corner*2, corner*2, 270*16, -corner*16*3);
	p.lineTo(0, 0+corner);
	p.arcTo(0, 0,  corner*2, corner*2, 90*16, corner*16*3);
	p.closeSubpath();

	RenderArea ra(p);
	r.setFilleRule(

*/

qp->drawRoundedRect(0,0, this->width(), this->height(), this->corner, this->corner);


}

void plane::get_data() {

        std::vector<Weather_day> items;
        //std::string filename("json_body_backup");
	std::string filename = curly->grab_to_file("http://api.worldweatheronline.com/free/v1/weather.ashx?q=DH1+3LE+&format=json&num_of_days=5&key=v2zhhdf5mtj3vbukytzudw3d");

        Parser p;

        items = p.parse_file(filename);

	QVBoxLayout *vbox = new QVBoxLayout(this);
	//std::vector<QLabel*> labels;

        for (int i = 0; i < items.size() ; i++) {

		QDate t;

		if (i == 0) {
			t = QDate::currentDate();
		} else {
			t = get_qdate(items[i].get_date());
		}
		
		QLabel *tmp_icon = new QLabel;
		tmp_icon->setPixmap(*get_icon(items[i].get_url()));
		QLabel *tmp_date = new QLabel(get_day(t));
		QLabel *tmp_temp = new QLabel(get_temp(items[i].get_temp()));
		QLabel *tmp_desc = new QLabel(QString::fromStdString(items[i].get_desc()));
		QLabel *tmp_label = new QLabel(QString::fromStdString(items[i].get_date()));
		QHBoxLayout *hbox = new QHBoxLayout();

		hbox->addWidget(tmp_date);
		hbox->addWidget(tmp_icon);
		hbox->addWidget(tmp_temp);
		hbox->addWidget(tmp_desc, Qt::AlignLeft);
		vbox->addLayout(hbox);
		

        }

	setLayout(vbox);
}

QPixmap * plane::get_icon(std::string url) {

	unsigned int pos = url.find_last_of('/');
	std::string path = url.substr(pos+1);

	QFile test(QString::fromStdString(path));

	if (!test.exists()) 
		grab_icon(url);
	
	return new QPixmap(QString::fromStdString(path));

}

void plane::grab_icon(std::string url) {

	if (resizer == NULL) 
		resizer = new Resize();
	
	if (curly == NULL)
		curly = new Curly();

	std::string filename = curly->grab_to_file(url);
	resizer->resize(filename);

}


QDate plane::get_qdate(std::string date) {

        std::stringstream ss(date);
	std::vector<int> parts;
        std::string tmp;
	std::istringstream iss;

	while(std::getline(ss, tmp, '-')) {

		std::cout << "tmp = " << tmp << std::endl;

		iss.str(tmp);
		int tmp_int=0;
		iss >> tmp_int;
		parts.push_back(tmp_int);
		iss.seekg(0);

        }

 
	return QDate(parts[0], parts[1], parts[2]);

}

QString plane::get_day(QDate date) {

	int day = date.dayOfWeek();

	switch(day) {
	
	case 1:
		return QStr         Q�.9Q�.:      �  �  �HTTP:https://www.facebook.com/ai.php?aed=AQL2G3Zmf2sBsInTChcgF--_0x1XnTqVk1ALEGpcT5PJUZzqpXV86ZqhY5yvmEga92Cv34YJZN-hr-IIzjYcfmDAYoGJo8SV3KfcJVd-Svq4Vm_N5YMqIs2BR_bfBLFfPc_U1zdMxb3fTdVo_RAZdDd9rziPqTPZgB8XGk9oOlpjrebhIS-momJkO5CVqJkHIkErlkYwtKLOcqW-VtQqBvTeg6qeCGwpcXL3RpIiVHbaEsq8ukvjYSMyVdOGQt1MIpXeMsstm1KZqtJ9BS0WjAopUCwqsMb-x3ztzSyAxuRpDDry4n7ZgKJKFwm2l3m-p32v9B-PlyXy24W7DT-Z1n5BtW1Lp9YqdU3abAgvfep3R3GwywK7bQ5J6NXwUcqQsTg9xP8H-3vAvvm03O8PpG8ECbnfhmNbK3JBWJeakBKkBoQMiZwy0UKFhjwf16IUuqqkCvgu4uaT2RxN0rmJc47LcWRhhSfICSLQBxotHtU7yjLPZK7uKBDo9SZEyStqsH8vgIeMLFgDgB90Ccs_UN_kQX4oH1WyVZb-fLIIUCNzfWNztQFx1WTDSbyg9xNtZGhkVnOKR9-T3Wch3Ikiz3WuEOC6MI7USi0U3GwahsVfOi_n5iLCwPJ0KGZKzsn4ciAS1f_nlyNg8R4CsyDY_OdyrTsHzlfpX1DGHnhsZzY7jjFDsSvoSRio6T90e75S8YG0e7kDHNJ3maa27IGmPSqNuxiKy2tEx82JNKfIcKlpxSHfCNRY94fEs6otqgTCAqE-_rkTF2GEtjQfpcVerj11YsR9RcZy3bHicRwrtZJ3CnXz7nVW3wm1TXxsksUL_qh4vfC7pYXu2z4hHgFc8VD_prQXB1iA_F2L6v5b_v9NOhnX-2ktrrsdhmYUjOIyXdJ_pm4MCe7rosQt4zF5yt6gzL6aXosfiXltYHw3ruyvfZf0FwvFpvJXik-VcV9NjxVb8iMWRrh-CdtNi6E9Kf9USX1VvXmCNzefJocu-HdxsM7ptyvzM-ZV06oqLXgp68RqLenP6OBFBHcjEREvt6j60GdPjzveKnTjg8CXqMz9GT_uN7i7030qELfNx77RFj0Zqua5O008hb77QyXk1KzC5D_hAKezcBFpwGVDDBDdys-9kImssPXlFRMMYA2WDMrhxzVsM79YMVMmrQK0IAu-SyY6mmpNo3iXCWasKvXLY2yib7ZNtO-Vm58NIsZq3MpKo9Hs2wsiAwxIpTbl446y5uDkUoa4og4h2N0uqlqRWfpHfUQWtFfwW4XCQYgH9X9nohVpzoPxbD2foycf8R1nC9GryNsUF7edvBKtqvlMuWDUgmyVz-VWMzos982XomC8iXBto41hFczbVXlbC9pn_erk8LXYy7sUfRHeBaPZRiTE5OWecD30sGsUcMHiHTMya9HfDq1msG2UOMsJAtmaRBeM-wAFZxe5L0kTIOVcRqachnRUqZYB2xJUNmYX2tZckVj2hbQEAXs_TeI0FnAIG7zPdTyhd51ZZpVTaUB1W1dVhQ-2X92KE3LeU1RIvrQNtvT3rXlpPOakP5phXtsPEY7Xold17ngR975WoqoOr5ZJRxe63IhlCoJpbOA4FLSpgSAMBbOOkronFkuVOp7AJ49wC4LyczeyiTyPSq9xJXeBLW4EOW0aL8zb-pIYilv_9bp8PRjUsJ-1weXOMwOYJWViLEaU_7o20CPt3u1RC_DsILb1XjWNcXtdKxUNnfd8N0NcBWIzVEXNyu6lyl0f47S2fSMHWaCvL7KYm37pVmFyvZjca-sMAVNkEKifLsA7oQxPYEXRHW80yXVbupNy1tbGrudv2A9-FxcqDyoY_ZMrw2b0tTD_Rvd4mFVlYUv-AASXy9UlojjNc6-40ymkVohNgUDIhGamNIYOfDcidht3PJmI4rWHr--KtWGi9jOrUq6FKZouoVStdKzfu-gFMc6ovqg3aGRGeJUSxKxQNh4qWi89nSwq0MI7jKQ9xYoWePIpzVdOkVaKDWaAuJU2lqcYsSdYZzLsfv2aP35W1cRdGHsbuY9JnqO6KKxTkGYHR5p8j_Xm0vvG7Q00Bpu2qhFHwAn-s3ZjxFx0b1xQhP2KbJUuCiarYUmCQ1vPBS9IK27nX0B7ME3IUTgb2XbUYiOF_CvSBFXgoa-LfjlE1XkSD8sJYLAjKHR8GdqEarJPjmznYThO4tbJQp-1Mt_hZ0DBFsgsHjWn4BWNQpNaNMra5FzF5jx4keOgnTJ1eYqPc2J11KCe8Vqzy_RaiH8mqYNn_p-Loiypp4vPaIkLr6qJcuyIyO_PXE8km6_yYw_sM7mEjH_zZca_JHHx7lfQvAEUtRwQwED4_9otn54KvefU0rgKUVL1frJFg-bvUhSImKX5JitJeiRVgIpgMxh-g0G4N0Nn0xj0GhJ0FykZht12KMqSXkYCQ0uf5TzTbKKtljb3iIETemrW6TI8rIMH0lrfJXZ-y2S6ML_VBMec0LH1iikSqhKOj-A2DLbeTO9C28GE0VdUx06hOPUTnLX0c0VMSdMCgflz7fRrkcdZKtGrJ-yGr9uPH2CaCL0W8glMCQtpz_DAdwWCcbn3DtQCEybfJ9FKJr-vrLCxlFDvCpHlRUAyW3Ydubk5Hc_8rgNJSRmekYAMA49YgkLtGPmKEs-Vu-xHXzo2bg753X_ly_k9MWLYX2_MCobGhkgLw49K7n5tlpUQh_KBQQukRXO6B67iIU4ospiaSxYeHzOwa2G09rorA5T9FDK7q4kHYUnYGdGTd-_wK6W3HtOrdkIafLAlw3NmoSnSYW836sHNYKxsAwXoYeqWGJoMm10EfQjjkGfN1NXdklo3zAaSHHMZTkUq0QogWcXAGxqO1uswzQg9-XOqQNx67jSKrcoM1rFjoSZYES8nIJbYrBHr3F2sRjIzhCtuAImH80-O4RM4P6ukyBlmgsSUoe0Zz8llharLgO0g6IoYlQC_rqDkgE3usyDSARhJB-zCg2nTGsVHRQyN5NJdqbBXxyTr5JndiAQ request-method GET response-head HTTP/1.1 200 OK
Content-Length: 411
Content-Type: text/html; charset=UTF-8
Date: Tue, 11 Jun 2013 14:06:21 GMT
x-fb-debug: 6gZZyrrtYaGWh/HNJW4Pk6AUxsV9wJh9uR8LXF/NgGc=
X-Firefox-Spdy: 3
 charset UTF-8 security-info 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                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     #include "plane.hpp"
#include <QPainter>
#include <iostream>
#include <QFile>

#include "weather_day.hpp"
#include "parser.hpp"
#include <vector>
#include <QVBoxLayout>
#include <sstream>
#include <QDesktopWidget>

plane::plane(QWidget *parent) : QWidget(parent) {

	// we probably wont need these...
	curly = new Curly();
	resizer = NULL;
	//
	
	setAttribute(Qt::WA_TranslucentBackground);
	//setAttribute(Qt::NoSystemBackground);
	setWindowFlags(Qt::FramelessWindowHint);
	//setWindowFlags(Qt::WindowStaysOnBottomHint);
	//setWindowFlags(Qt::X11BypassWindowManagerHint);
	setAttribute(Qt::WA_X11DoNotAcceptFocus);
	
	this->corner = 15;

	this->colour = QColor(11, 11, 44, 127);

	//QLabel *mi = new QLabel(this) ;
	//mi->setGeometry(0,0,100,100);
	//QPixmap pix(QString::fromStdString("test.png"));
	//mi->setPixmap(pix);

	get_data();

//mi->show();
 
}

void plane::paintEvent(QPaintEvent *e) {

	Q_UNUSED(e);
	QPainter qp(this);
	drawLines(&qp);
	QDesktopWidget *desktop = QApplication::desktop();
	int width = desktop->width();
	move((width - this->width())-1, 410);
	
}

void plane::drawLines(QPainter *qp) {

	qp->setOpacity(1.0);
	//QPen pen(Qt::red, 10, Qt::SolidLine);
	QPen pen(Qt::NoPen);
	qp->setPen(pen);

 	QBrush brush;
	brush.setStyle(Qt::SolidPattern); // Fix your problem !
      	brush.setColor(this->colour);
	qp->setBrush(brush);
/*

	QPainterPath p;
	p.lineTo( width-corner, 0);
	p.arcTo(width-(corner*2), 0, corner*2, corner*2, 0, corner*16*3);
	p.lineTo(width, height-corner);
	p.arcTo(width-(corner*2), height-(corner*2),  corner*2, corner*2, 0, -corner*16*3);
	p.lineTo(0+corner, height);
	p.arcTo(0, height-(corner*2),  corner*2, corner*2, 270*16, -corner*16*3);
	p.lineTo(0, 0+corner);
	p.arcTo(0, 0,  corner*2, corner*2, 90*16, corner*16*3);
	p.closeSubpath();

	RenderArea ra(p);
	r.setFilleRule(

*/

qp->drawRoundedRect(0,0, this->width(), this->height(), this->corner, this->corner);


}

void plane::get_data() {

        std::vector<Weather_day> items;
        //std::string filename("json_body_backup");
	std::string filename = curly->grab_to_file("http://api.worldweatheronline.com/free/v1/weather.ashx?q=DH1+3LE+&format=json&num_of_days=5&key=v2zhhdf5mtj3vbukytzudw3d");

        Parser p;

        items = p.parse_file(filename);

	QVBoxLayout *vbox = new QVBoxLayout(this);
	//std::vector<QLabel*> labels;

        for (int i = 1; i < items.size() ; i++) {

		QDate t;

		if (i == 0) {
			t = QDate::currentDate();
		} else {
			t = get_qdate(items[i].get_date());
		}
		
		QLabel *tmp_icon = new QLabel;
		tmp_icon->setPixmap(*get_icon(items[i].get_url()));
		QLabel *tmp_date = new QLabel(get_day(t));
		QLabel *tmp_temp = new QLabel(get_temp(items[i].get_temp()));
		QLabel *tmp_desc = new QLabel(QString::fromStdString(items[i].get_desc()));
		QLabel *tmp_label = new QLabel(QString::fromStdString(items[i].get_date()));
		QHBoxLayout *hbox = new QHBoxLayout();

		hbox->addWidget(tmp_date);
		hbox->addWidget(tmp_icon);
		hbox->addWidget(tmp_temp);
		hbox->addWidget(tmp_desc, Qt::AlignLeft);
		vbox->addLayout(hbox);
		

        }

	setLayout(vbox);
}

QPixmap * plane::get_icon(std::string url) {

	unsigned int pos = url.find_last_of('/');
	std::string path = url.substr(pos+1);

	QFile test(QString::fromStdString(path));

	if (!test.exists()) 
		grab_icon(url);
	
	return new QPixmap(QString::fromStdString(path));

}

void plane::grab_icon(std::string url) {

	if (resizer == NULL) 
		resizer = new Resize();
	
	if (curly == NULL)
		curly = new Curly();

	std::string filename = curly->grab_to_file(url);
	resizer->resize(filename);

}


QDate plane::get_qdate(std::string date) {

        std::stringstream ss(date);
	std::vector<int> parts;
        std::string tmp;
	std::istringstream iss;

	while(std::getline(ss, tmp, '-')) {

		std::cout << "tmp = " << tmp << std::endl;

		iss.str(tmp);
		int tmp_int=0;
		iss >> tmp_int;
		parts.push_back(tmp_int);
		iss.seekg(0);

        }

 
	return QDate(parts[0], parts[1], parts[2]);

}

QString plane::get_day(QDate date) {

	int day = date.dayOfWeek();

	switch(day) {
	
	case 1:
		return QString(QChar(0x6708));
		return QString("mon");
		break;

	case 2://6728
		return QString(QChar(0x706b));
		return QString("tue");
		break;

	case 3:
		return QString(QChar(0x6c34));
		return QString("wed");
		break;

	case 4:
		return QString(QChar(0x6728));
		return QString("thu");
		break;

	case 5:
		return QString(QChar(0x91d1));
		return QString("fri");
		break;

	case 6:
		return QString(QChar(0x571f));
		return QString("sat");
		break;

	case 7:
		return QString(QChar(0x65e5));
		return QString("sun");
		break;


	}

}

QString plane::get_temp(std::string temp) {

	if (temp.length() == 1) {

		temp = " " + temp;

	}

	return QString::fromStdString(temp) + QString(QChar(0xb0));
}
