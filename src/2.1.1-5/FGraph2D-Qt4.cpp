#include "FGraph2D-Qt4.h"
#include "aljabr.h"
#include "Data.h"

#include <cmath>
#include <stdio.h>



FGraph2D::FGraph2D(Data * data, QWidget *parent): QWidget(parent)
{
	setCursor(QCursor(Qt::CrossCursor));
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	this->data=data;
	initData();
	error = false;
	effects = true;
	func = true;
	points = false;
	funcPenColor = QColor(148,24,24);
	interPenColor = QColor(80,119,101);
	errorPenColor = QColor(Qt::red);
	penWidth = 2;
	
};

void FGraph2D::initData()
{ 
	float up,down,add,oldX,oldY;
	int i, spanX, spanY, top, left,x,y;
	float _left,_top,_spanX,_spanY;
	
	up=data->f[0];
	down=up;
	
	for( i = 0 ; i < data->N ; i++)
	{ 
		if (data->f[i] > up)up=(data->f[i]);
		else if (data->f[i] < down)down=(data->f[i]);	
	}
	
	_left = data->begin;
	_top  = up;
	
	_spanX = FJ::abs(data->end - data->begin);
	_spanY = up - down;
	
	oldX = _spanX;
	oldY = _spanY;

	add =ceil((_spanX + _spanY) / (2 *10));
	
	if (add<2)add=2;
	margin = (int) floor(add/2);

	_spanX+=2*add;
	_spanY+=2*add;

	down = _spanX / 4;
	up   = _spanY / 3;

	_spanX = qMax(down , up) * 4;
	_spanY = qMax(down , up) * 3;
	
	down = FJ::abs(oldX - _spanX);
	up   = FJ::abs(oldY - _spanY);	

	_left -= down/2;
	_top  += up/2;

	spanX = (int) ceil (_spanX);
	spanY = (int)  ceil (_spanY);
	top   = (int)  ceil (_top);
	left  = (int)  floor (_left);

	viewport = new 	QRect(left, top, spanX, -spanY);
	window = new QRect(left, top, spanX, -spanY);
	Axis=axisNo;

	printf("margin = %i",margin);
	printf("left = %i\ntop = %i\nspanX = %i\nspanY = %i\n",left,top,spanX,spanY);
	
	if(  (left+margin <= 0) && (left+spanX-margin >= 0)  &&  (top-margin >= 0) && (top-spanY+margin <= 0)  ) Axis|=axisXY;
	else if( (top-margin >= 0) && (top-spanY+margin <= 0) ) Axis|=axisX;
	else if( (left+margin <= 0) && (left+spanX-margin >= 0) ) Axis|=axisY; 	


	if(Axis & axisXY)
	{
		centre.setX(0);
		centre.setY(0);			
	}
	else if(Axis & axisX)
	{
		if(viewport->left()<0)
		x = viewport->right() + 1 - margin; else x = viewport->left() + margin;
		centre.setX(x);
		centre.setY(0);	
	}
	else if(Axis & axisY)
	{
		if(viewport->top()<=0)
		y=viewport->top() - margin; else y=viewport->bottom() + 1 + margin;
		centre.setX(0);
		centre.setY(y);	
	}
	else
	{	if(viewport->left()<0)
		x = viewport->right() + 1 - margin; else x = viewport->left() + margin;
		if(viewport->top()<=0)
		y=viewport->top() - margin; else y=viewport->bottom() + 1 + margin;
		centre.setX(x);
		centre.setY(y);	
	}
	
	

}


QSize FGraph2D::minimumSizeHint() const
{
    return QSize( 6 *100, 4 *100);
}


QSize FGraph2D::sizeHint() const
{
    return QSize(6* 100, 4* 100);
}


void FGraph2D::paintEvent(QPaintEvent *)
{
	
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.drawPixmap(0, 0, pixmap);



	if(this->hasMouseTracking() && this->cursor().shape() != Qt::PointingHandCursor && effects) 
	{
		QRect v=rect();
		QPen ccursor;
		QPointF r=toWidget(mark);
		
		ccursor.setColor(QColor(0xc0,0,0));
		ccursor.setStyle(Qt::SolidLine);
		painter.setPen(ccursor);

		painter.drawLine(QPointF((float)v.left(),r.y()), QPointF((float)v.right()+1, r.y()));
		painter.drawLine(QPointF(r.x(),(float)v.top()),QPointF(r.x(),v.bottom()+1));
		
		int w=90, h=13;
			
		if(r.x()+w+10 > this->width())
		r.setX(this->width()-w-10);
		if(r.y()+h+10 > this->height())
		r.setY(this->height()-h-10);
		if(r.y() < 0)
		r.setY(-10);
		
		painter.setPen(QPen(QColor(0,0,0)));
		
painter.drawText(r.x()+15,r.y()+15,QString("x=%1 y=%2").arg(mark.x(),4,'f',2,QLatin1Char(' ')).arg(mark.y(),4,'f',2,QLatin1Char(' ')));

	}	
};


void FGraph2D::mouseMoveEvent ( QMouseEvent * event ) 
{
	mark=inFunc(fromWidget(event->posF()));
	this->repaint();
	
}


void FGraph2D::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Left:
        left();
        break;
    case Qt::Key_Right:
        right();
        break;
    case Qt::Key_Down:
        down();
        break;
    case Qt::Key_Up:
        up();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}


void FGraph2D::resizeEvent(QResizeEvent * /* event */)
{
	refreshPixmap();
}


void FGraph2D::zoomIn()
{
	int zoom = FJ::abs(window->height()) - margin *2;
	if (zoom > 0)
	{
	window->adjust(margin,-margin,-margin,margin);
	refreshPixmap();
	}	
};


void FGraph2D::zoomOut()
{
	window->adjust(-margin,margin,margin,-margin);
	refreshPixmap();
};


void FGraph2D::left()
{
	window->moveLeft(window->left()-margin);
	refreshPixmap();
};


void FGraph2D::right()
{
	window->moveRight(window->right()+1+margin);
	refreshPixmap();
};

void FGraph2D::up()
{
	window->moveTop(window->top()+margin);
	refreshPixmap();
};


void FGraph2D::down()
{
	//window->moveCenter(QPoint(window->center().x(),window->center().y()-margin));
	//window->moveBottom(window->bottom()+1-margin);	
	window->setY(window->y()-margin);
	window->setBottom(window->bottom()-margin);		
	refreshPixmap();
};


void FGraph2D::ShowGrid(bool b)
{

effects = b;
setMouseTracking(b);
refreshPixmap();

};


void FGraph2D::ShowFunc(bool b)
{

func = b;
refreshPixmap();

};


void FGraph2D::ShowError(bool b)
{
error = b;
refreshPixmap();
}


void FGraph2D::ShowPoints(bool b)
{
points = b;
refreshPixmap();
};


void FGraph2D::refreshPixmap()
{
	pixmap = QPixmap(size());
	QPainter painter(&pixmap);
	initPainter(&painter);	
	drawGrids(&painter);
	drawFunctions(&painter);
	update();
}


void FGraph2D::initPainter(QPainter *painter)
{
	painter->initFrom(this);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->fillRect(rect(), QBrush(Qt::white));
	painter->setWindow(*window);
};



void FGraph2D::drawGrids(QPainter *painter)
{
	
	QPen pen,dotpen;
	int x,y,CellScale = (int) ceil((float) viewport->width() / 40) ;
	
	dotpen.setStyle(Qt::DashLine);
	dotpen.setColor(QColor(100,100,255));
	pen.setStyle(Qt::SolidLine);
	
		
	pen.setColor(QColor(225,225,255));
	painter->setPen(pen);
if(effects)
{
/////////////////vertical lines/////////////////////////////	
	for(x=centre.x();x<=window->right()+1; x+=CellScale) 
	painter->drawLine(x,window->top(),x ,window->bottom()+1);
	
	for(x=centre.x();x>=window->left(); x-=CellScale) 
	painter->drawLine(x,window->top(),x ,window->bottom()+1);	

//////////////////horizontal lines///////////////////////////
	for(y=centre.y(); y<=window->top(); y+=CellScale) 
	painter->drawLine(window->left(),y,window->right()+1, y);
	
	for(y=centre.y(); y>=window->bottom()+1; y-=CellScale) 
	painter->drawLine(window->left(),y,window->right()+1, y);
}	

	pen.setColor(QColor(100,100,255));
	painter->setPen(pen);
	

	if(Axis & axisXY)
	{	
		painter->drawLine(window->left(),0,window->right()+1,0);			// X axis
		painter->drawLine(0,window->top(),0,window->bottom()+1);			// Y axis
		
	}
	else if(Axis & axisX)
	{
		painter->drawLine(window->left(),0,window->right()+1,0);			// X axis	
		painter->setPen(dotpen);
		painter->drawLine(centre.x(),window->top(),centre.x(),window->bottom()+1);	// Y axis
	}
	else if(Axis & axisY)
	{
		painter->setPen(dotpen);
		painter->drawLine(window->left(),centre.y(),window->right()+1,centre.y());	// X axis	
		painter->setPen(pen);		
		painter->drawLine(0,window->top(),0,window->bottom()+1);			// Y axis
	}
	else
	{
		painter->setPen(dotpen);
		painter->drawLine(window->left(),centre.y(),window->right()+1,centre.y());	// X axis	
		painter->drawLine(centre.x(),window->top(),centre.x(),window->bottom()+1);	// Y axis
	}


};

void FGraph2D::drawFunctions(QPainter * painter)
{

	QPen pen;
	QPointF p1,p2;
	float begin = data->begin,**a =data->a,*x = data->virtx , end = data->end,step,x0,x1;
	int pad,n = data->N; 

	pen.setColor(interPenColor);//7b499a  507765 941818
	pen.setJoinStyle(Qt::RoundJoin);
	pen.setStyle(Qt::SolidLine);
	step =(float) viewport->width() / (float)width();	
	pen.setWidthF(step * penWidth);
	painter->setPen(pen);


	// Drawing interpolating function
	x0 = begin;
	p1.setX(x0);
	data->interpolate(x0,begin,end,n,a,x);
	p1.setY(x0);

	while(p1.x()<=end-step)
		{
		x0 = p1.x()+step;
		p2.setX(x0);
		data->interpolate(x0,begin,end,n,a,x);
		p2.setY(x0);
		painter->drawLine(p1,p2);
		p1 = p2;
		}
	
	


		
	//// Drawing default function if no input file is given
	if((!(data->bitmap & FJ::InputFile)) && func)
	{
	pen.setColor(funcPenColor);
	painter->setPen(pen);
	
	p1.setX(begin);
	p1.setY(data->func(begin));

	while(p1.x()<end)
		{
		p2.setX(p1.x()+step);
		p2.setY(data->func(p2.x()));
		painter->drawLine(p1,p2);
		p1 = p2;
		}
	}
	

	//// Drawing error function if no input file is given and Show error is triggered
	if((!(data->bitmap & FJ::InputFile)) && error)
	{
	pen.setColor(errorPenColor);
	painter->setPen(pen);
	painter->drawLine((int)floor(begin),viewport->bottom()+1+margin,(int)ceil(end),viewport->bottom()+1+margin);
	pad = viewport->bottom()+1+margin;
	
	x1 = x0 = begin+step;	
	p1.setX(x0);
	
	x0 = data->func(x0);
	data->interpolate(x1,begin,end,n,a,x);
	
	x0 = FJ::abs(x0 - x1);
	x0+=(float) pad;
	p1.setY(x0);


	while(p1.x()<end-step)
		{
		x1 = x0 = p1.x()+step;
		p2.setX(x0);
		x0 = data->func(x0);
		data->interpolate(x1,begin,end,n,a,x);		
		
		x0 = FJ::abs(x1 - x0);
		x0+=(float) pad;
		p2.setY(x0);
		painter->drawLine(p1,p2);
		p1 = p2;
		}
	
	
	}

	if(points)
	{
	painter->setBrush(QBrush(Qt::black,Qt::SolidPattern));
	for(pad=0;pad<n;pad++)painter->drawEllipse(QPointF(data->x[pad],data->f[pad]),step * penWidth,step * penWidth);
	}	



};


QPointF  FGraph2D::fromWidget(QPointF p)
{
	float w = (float) width() / (float) window->width();
	float h = (float )height() / (float)window->height();
	float x = window->left();
	float y = window->top();
	QPointF r;
	r.setX(p.x()/w + x);
	r.setY(p.y()/h + y);

	return r;
};


QPointF FGraph2D::toWidget(QPointF p)
{
	float w = (float) width() / (float) window->width();
	float h = (float )height() / (float)window->height();
	float x = window->left();
	float y = window->top();
		
	QPointF orig((p.x() - x)*w,(p.y() - y)*h);
	return orig;
}


QPointF FGraph2D::inFunc(QPointF p)
{
	if((data->begin<p.x()) && (data->end>p.x()))
	{
	float y;
	y = p.x();
	data->interpolate(y,data->begin,data->end,data->N,data->a,data->virtx);
	p.setY(y);
	}
	return p;
}


int FGraph2D::calc(float x,float &y,float &diff)
{
	int r;
	y=x;
	r=data->interpolate(y,data->begin,data->end,data->N,data->a,data->virtx);
	if(r<0)return -1;
	printf("inter %f\n",y);
	if(data->bitmap & FJ::InputFile)
	diff=0;
	else
	{
	diff = data->func(x);
	printf("func %f\n",diff);
	diff = FJ::abs(diff - y);
	}

	return 1;
};


void FGraph2D::savePixmap(QString file)
{
pixmap.save(file,"PNG");
};


