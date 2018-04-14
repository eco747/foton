#ifndef QXSRUNNER_H
#define QXSRUNNER_H

#include <qthread.h>
#include "litehtml/include/litehtml.h"

class QXsRunner
	: public QThread
{
	Q_OBJECT

	litehtml::document*	m_document;

public:
	QXsRunner( litehtml::document* doc );

	void	run();
};

#endif // QXSRUNNER_H
