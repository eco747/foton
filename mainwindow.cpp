#include "mainwindow.h"
#include <QDebug>
#include <QScrollArea>

#include "qhtmlrenderer.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	QHtmlRenderer*	renderer = new QHtmlRenderer();
	renderer->loadFile( "c:/qt/projects/photonboard3/tests/wiki.html" );

	QScrollArea*	scroller = new QScrollArea( this );
	scroller->setWidget( renderer );
	setCentralWidget( scroller );
}

MainWindow::~MainWindow()
{

}

