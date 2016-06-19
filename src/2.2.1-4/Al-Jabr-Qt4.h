
#ifndef QGRAPH_H
#define QGRAPH_H


#include <QMainWindow>
#include <QApplication>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QToolBar>
#include <QSplashScreen>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QStatusBar>
#include <QInputDialog>
#include <QColorDialog>



class FGraph2D;
class FGraph3D;
struct Data;

 class Aljabr : public QMainWindow
 {
     Q_OBJECT

 public:
     Aljabr(Data * d = 0);

 protected:
     

 private slots:
     void save();
     void about();
     void addPoint();
     void clearContents();
     void enableAddButton(const QString &);
     void enableClearButton();
     void funcPenColor();
     void interPenColor();
     void errorPenColor();
     void penWidth();				

 private:
     void createActions();
     void createMenus();
     void createToolBar();	
     void createStatusBar();
     
     FGraph2D * graph2d;
     FGraph3D * graph3d;		
     QTableWidget * point_list;
     QTabWidget *tab;
     QLineEdit *text2dX;
     QLabel *label;
     QPushButton *flush_list;
     QPushButton *add;	
     QLabel *scale;
     int row;		

     QMenu *fileMenu;
     QMenu *optionMenu;
     QMenu *viewMenu;
     QMenu *goMenu;
     QMenu *helpMenu;

     QAction *openAct;
     QAction *exitAct;
     QAction *aboutAct;
     QAction *aboutQtAct;
     QAction *zoominAct;
     QAction *zoomoutAct;	
     QAction *showGridAct;
     QAction *showErrorAct;
     QAction *showFuncAct;
     QAction *showPointsAct;	

     QAction *upAct;
     QAction *downAct;	
     QAction *leftAct;
     QAction *rightAct;
     QAction *funcPenColorAct;
     QAction *interPenColorAct;
     QAction *errorPenColorAct;
     QAction *penWidthAct;
     
     QToolBar * toolbar;
     
 };




extern float	XQuadrat(float);
extern float	CosX(float);
extern float	SinX(float);
extern float	LogX(float);


#endif


