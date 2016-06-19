#include "Al-Jabr-Qt4.h"
#include "FGraph2D-Qt4.h"
#include "FGraph3D-Qt4.h"
#include "Data.h"


#include <stdio.h>


Aljabr::Aljabr(Data * data)
 {
	
	move(200,200);
	setMinimumSize(806,500);
	resize(850,520);
	tab = new QTabWidget(this);
	tab->setTabPosition(QTabWidget::North);
	tab->setFocusPolicy(Qt::NoFocus);
	setWindowIcon(QIcon(":/images/al-jabr-logo.png"));	

	////// 2D graph tab  /////////////////

	QSplitter * split = new QSplitter(Qt::Horizontal,this);
	
	graph2d = new FGraph2D(data,split);
	QWidget * left = new QWidget(split);

	QVBoxLayout *left_layout = new QVBoxLayout;
	QHBoxLayout *leftbottom = new QHBoxLayout;
	
	left->setMaximumWidth(260);
	
	point_list = new QTableWidget(0, 3,left);
	
	point_list->setHorizontalHeaderLabels(QStringList() << tr("x") << tr("F(x)") << tr("|F(x)-f(x)|"));
	point_list->setColumnWidth(0,60);
	point_list->setColumnWidth(1,70);
	point_list->setColumnWidth(2,70);
	point_list->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	row=0;
	point_list->insertRow(row);

	text2dX = new QLineEdit(left);
	text2dX->setMaxLength(10);
	
	label = new QLabel("&X = ",left);
	label->setBuddy(text2dX);
	add = new QPushButton("Add point",left);	
	flush_list = new QPushButton("Clear list",left);
	add->setDefault(true);
	add->setEnabled(false);
	flush_list->setEnabled(false);

	
	leftbottom->addWidget(label);
	leftbottom->addWidget(text2dX);
	leftbottom->addWidget(add);
	leftbottom->addStretch();	
	
	left_layout->addWidget(point_list);
	left_layout->addWidget(flush_list);
	left_layout->addLayout(leftbottom);
	left->setLayout(left_layout);
    
	connect(add,SIGNAL(clicked()),this, SLOT(addPoint()));
	connect(flush_list,SIGNAL(clicked()),this, SLOT(clearContents()));
	connect(text2dX,SIGNAL(textChanged(const QString &)),this,SLOT(enableAddButton(const QString &)));
	connect(add,SIGNAL(clicked()),this, SLOT(enableClearButton()));

	createActions();
	createMenus();
	createToolBar();
	createStatusBar();

	
	split->setStretchFactor(0, 1);
	tab->addTab(split, "2D Graph");
	////// End of 2D graph tab  //////////////
	

	////// 3D graph tab  /////////////////
	graph3d = new FGraph3D();
	tab->addTab(graph3d, "3D Graph"); // demo
	////// End of 2D graph tab  //////////////



	setCentralWidget(tab);
	setWindowTitle("Al-Jabr");
	

	text2dX->show();
	label->show();
	graph2d->show();
	point_list->show();
 
 }

void Aljabr::save()
 {
QString fileName = QFileDialog::getSaveFileName(this,
                           tr("Save profile"), ".",
                           tr("Image files (*.PNG)"));
if (!fileName.isEmpty())
    graph2d->savePixmap(fileName);

 }

void Aljabr::addPoint()
{
	float x,Fx,diffF;
	QString text = text2dX->text();
	//int row;

	x=text.toFloat();	
	if(graph2d->calc(x,Fx,diffF)<0)
	{
	QMessageBox::critical	(this,
				tr("Al-Jabr"),
				QString("Error : point %1 is out of range").arg(x,3,'g'),
				QMessageBox::Ok | QMessageBox::Default
				);
	text2dX->clear();
	return;
	}
	//row = point_list->rowCount();
	point_list->insertRow(row);

	QTableWidgetItem *item0 = new QTableWidgetItem;
	item0->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	point_list->setItem(row, 0, item0);

	QTableWidgetItem *item1 = new QTableWidgetItem;
	item1->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	point_list->setItem(row, 1, item1);

	QTableWidgetItem *item2 = new QTableWidgetItem;
	item2->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	point_list->setItem(row, 2, item2);

	point_list->setCurrentItem(item0);

        point_list->item(row, 0)->setText(QString::number(x));
	point_list->item(row, 1)->setText(QString::number(Fx));
	point_list->item(row, 2)->setText(QString::number(diffF));
	
	row++;
	text2dX->clear();
}


void Aljabr::clearContents()
{
row=0;
point_list->clearContents();
point_list->setRowCount(1);
flush_list->setEnabled(false);
}


void Aljabr::enableAddButton(const QString &text)
{
        add->setEnabled(!text.isEmpty());
}


void Aljabr::enableClearButton()
{
        flush_list->setEnabled(row>0);
}


void Aljabr::funcPenColor()
{
	QColor newColor = QColorDialog::getColor(graph2d->funcPenColor,this);
	if (newColor.isValid())
	graph2d->funcPenColor = newColor;
	graph2d->refreshPixmap();
}


void Aljabr::interPenColor()
{
	QColor newColor = QColorDialog::getColor(graph2d->interPenColor,this);
	if (newColor.isValid())
	graph2d->interPenColor = newColor;
	graph2d->refreshPixmap();
}


void Aljabr::errorPenColor()
{
	QColor newColor = QColorDialog::getColor(graph2d->errorPenColor,this);
	if (newColor.isValid())
	graph2d->errorPenColor = newColor;
	graph2d->refreshPixmap();
}


void Aljabr::penWidth()
 {
     bool ok;
     int newWidth = QInputDialog::getInteger(this, tr("Al-Jabr"),
                                             tr("Select pen width:"),
                                             graph2d->penWidth,
                                             1, 50, 1, &ok);
     if (ok){
        graph2d->penWidth = newWidth;
	graph2d->refreshPixmap();
	}
}

void Aljabr::about()
{
     QMessageBox::about(this, tr("About Al-Jabr"),
             tr("<p><b>Al-Jabr</b> is cross-platform sortware for graphical analyses"
                " of the function by piecewise linear interpolation and parabolic"
                " spline methods.</p>"
		"<p>Project\'s main purpose is to develop software that approximately"
		" computes certain functions values on given interval which is very"
		" expensive to evaluate by analytic manner, and draws 2D and 3D graphics"
		" of the function.</p>"
                "<p><h4><center><a href=\"http://oqila.uz/aljabr\" >http://oqila.uz/aljabr</a></center></h4></p>"
		"<h6>Copyright &copy; 2009-2010 Juraev Fazliddin.</h6>"));
 }
            

void Aljabr::createActions()
 {
     openAct = new QAction(tr("&Take photo..."), this);
     openAct->setShortcut(tr("Ctrl+S"));
     openAct->setIcon(QIcon(":/images/photo.png"));
     connect(openAct, SIGNAL(triggered()), this, SLOT(save()));

     exitAct = new QAction(tr("E&xit"), this);
     exitAct->setShortcut(tr("Ctrl+Q"));
     connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

     aboutAct = new QAction(tr("&About"), this);
     connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

     aboutQtAct = new QAction(tr("About &Qt"), this);
     connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
     
	showGridAct = new QAction(tr("Effe&cts"), this);
	showGridAct->setCheckable(true);
	showGridAct->setChecked(true);
	showGridAct->setStatusTip(tr("Enable/Disabel effects"));
	connect(showGridAct, SIGNAL(toggled(bool)),graph2d, SLOT(ShowGrid(bool)));
	
	showFuncAct = new QAction(tr("&Show Function"), this);
	showFuncAct->setCheckable(true);
	showFuncAct->setChecked(true);
	showFuncAct->setStatusTip(tr("Show or hide the function"));
	connect(showFuncAct, SIGNAL(toggled(bool)),graph2d, SLOT(ShowFunc(bool)));

	showErrorAct = new QAction(tr("S&how Error"), this);
	showErrorAct->setCheckable(true);
	showErrorAct->setChecked(false);
	showErrorAct->setStatusTip(tr("Show or hide the error function"));
	connect(showErrorAct, SIGNAL(toggled(bool)),graph2d, SLOT(ShowError(bool)));

	showPointsAct = new QAction(tr("Show p&oints"), this);;
	showPointsAct->setCheckable(true);
	showPointsAct->setChecked(false);
	showPointsAct->setStatusTip(tr("Show or hide the points"));
	connect(showPointsAct, SIGNAL(toggled(bool)),graph2d, SLOT(ShowPoints(bool)));
	
	zoominAct = new QAction(tr("&In"), this);
	zoominAct->setIcon(QIcon(":/images/zoomin.png"));
	zoominAct->setShortcut(tr("Ctrl+Plus"));
	connect(zoominAct, SIGNAL(triggered()), graph2d, SLOT(zoomIn()));
	
	zoomoutAct = new QAction(tr("&Out"), this);
	zoomoutAct->setIcon(QIcon(":/images/zoomout.png"));
	zoomoutAct->setShortcut(tr("Ctrl+Minus"));
	connect(zoomoutAct, SIGNAL(triggered()), graph2d, SLOT(zoomOut()));

	upAct = new QAction(tr("&Up"), this);
	upAct->setIcon(QIcon(":/images/gtk-go-up.png"));
	upAct->setShortcut(tr("Alt+Up"));
	connect(upAct, SIGNAL(triggered()), graph2d, SLOT(up()));

	downAct =  new QAction(tr("&Down"), this);
	downAct->setIcon(QIcon(":/images/gtk-go-down.png"));
	downAct->setShortcut(tr("Alt+Down"));
	connect(downAct, SIGNAL(triggered()), graph2d, SLOT(down()));
	
	leftAct =  new QAction(tr("&Left"), this);
	leftAct->setIcon(QIcon(":/images/gtk-go-left.png"));
	leftAct->setShortcut(tr("Alt+Left"));
	connect(leftAct, SIGNAL(triggered()), graph2d, SLOT(left()));

	rightAct =  new QAction(tr("&Right"), this);
	rightAct->setIcon(QIcon(":/images/gtk-go-right.png"));
	rightAct->setShortcut(tr("Alt+Right"));
	connect(rightAct, SIGNAL(triggered()), graph2d, SLOT(right()));

	funcPenColorAct = new QAction(tr("&Function Color"), this);
	funcPenColorAct->setIcon(QIcon(":/images/color.png"));
	connect(funcPenColorAct, SIGNAL(triggered()), this, SLOT(funcPenColor()));
	
	interPenColorAct = new QAction(tr("I&nterpolation Color"), this);
	interPenColorAct->setIcon(QIcon(":/images/color.png"));
	connect(interPenColorAct, SIGNAL(triggered()), this, SLOT(interPenColor()));
	
	errorPenColorAct = new QAction(tr("&Error Color"), this);
	errorPenColorAct->setIcon(QIcon(":/images/color.png"));
	connect(errorPenColorAct, SIGNAL(triggered()), this, SLOT(errorPenColor()));

	penWidthAct = new QAction(tr("Pen &Width"), this);
	penWidthAct->setIcon(QIcon(":/images/pen.png"));
	connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

}

void Aljabr::createMenus()
 {
     
     fileMenu = new QMenu(tr("&File"), this);
     fileMenu->addAction(openAct);
     fileMenu->addSeparator();
     fileMenu->addAction(exitAct);
     
     viewMenu = new QMenu(tr("&View"), this);
     viewMenu->addAction(zoominAct); 
     viewMenu->addAction(zoomoutAct);
     viewMenu->addSeparator();
     viewMenu->addAction(showFuncAct); 
     viewMenu->addAction(showErrorAct);
     viewMenu->addSeparator();
     viewMenu->addAction(showPointsAct);	
     viewMenu->addSeparator();
     viewMenu->addAction(showGridAct);

     goMenu = new QMenu(tr("&Go"), this);
     goMenu->addAction(upAct); 
     goMenu->addAction(downAct);
     goMenu->addAction(leftAct); 
     goMenu->addAction(rightAct);		
     
     optionMenu = new QMenu(tr("&Options"), this);
     optionMenu->addAction(funcPenColorAct); 
     optionMenu->addAction(interPenColorAct);
     optionMenu->addAction(errorPenColorAct);	
     optionMenu->addSeparator(); 
     optionMenu->addAction(penWidthAct);
     
     helpMenu = new QMenu(tr("&Help"), this);
     helpMenu->addAction(aboutAct);
     helpMenu->addAction(aboutQtAct);

     menuBar()->addMenu(fileMenu);
     menuBar()->addMenu(viewMenu);
     menuBar()->addMenu(goMenu);
     menuBar()->addMenu(optionMenu);
     menuBar()->addMenu(helpMenu);
     
 }

void Aljabr::createToolBar()
{
	toolbar= addToolBar("Zooming");
	toolbar->addAction(openAct);
	toolbar->addSeparator();
	toolbar->addAction(zoominAct);
	toolbar->addAction(zoomoutAct);
	toolbar->addSeparator();
	toolbar->addAction(upAct);
	toolbar->addAction(downAct);
	toolbar->addAction(leftAct);
	toolbar->addAction(rightAct);
	toolbar->addSeparator();
	toolbar->addAction(penWidthAct);
	
}


void Aljabr::createStatusBar()
{
	int cell =  graph2d->margin;
	scale = new QLabel(QString("Cell scaling: 1 x %1").arg(cell));
	statusBar()->addWidget(scale);
};



///////////////////////////////
///////////////////////////////
///////////////////////////////
///////////////////////////////


