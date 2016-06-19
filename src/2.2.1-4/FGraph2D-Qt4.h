#ifndef FGRAPH_H
#define FGRAPH_H



#include <QPainter>
#include <QPixmap>
#include <QAction>
#include <QMouseEvent>
#include <QKeyEvent>



struct Data;


class FGraph2D :public QWidget
{ 
	Q_OBJECT

public:
	FGraph2D(Data * , QWidget *parent = 0);
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	int calc(float,float &,float&);
	void refreshPixmap();

	QColor funcPenColor;
	QColor interPenColor;
	QColor errorPenColor;
	
	int penWidth;
	int margin;
	
public slots:
	void zoomIn();
	void zoomOut();
	void left();
	void right();
	void up();
	void down();
	void ShowGrid(bool);
	void ShowError(bool);
	void ShowFunc(bool);
	void ShowPoints(bool);
	void savePixmap(QString);
		
protected:
	void paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);
	void keyPressEvent(QKeyEvent *event);

private:
	void drawGrids(QPainter *);
	void drawFunctions(QPainter *);
	void initData();
	void initPainter(QPainter *);

	QPointF fromWidget(QPointF);
	QPointF toWidget(QPointF);
	QPointF inFunc(QPointF);
	
	Data * data;
	QRect * viewport;
	QRect * window;
	QPixmap pixmap;
	QPointF mark;
	QPoint centre;

	int Axis;
	bool effects;
	bool error;
	bool func;
	bool points;
	enum { axisNo = 0x0 , axisX = 0x1 , axisY = 0x2 , axisXY = 0x4 };

};

#endif
