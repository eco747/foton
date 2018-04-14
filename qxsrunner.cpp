#include <QDebug>
#include "qxsrunner.h"

#include "xsall.h"
#include "xs.h"
#include "prebuild_js.xs.h"

#include "litehtml/include/litehtml.h"


extern "C" {

	void fxRunModule(txMachine* the, const char* path)
	{
		mxPushStringC((txString)path);
		fxRequireModule(the, XS_NO_ID, the->stack);
		the->stack++;
	}

	int modTimersExecute(void);

	void fxRunLoop(txMachine* /*the*/)
	{
		while( modTimersExecute() ) {

		}
	}

	txPreparation* xsPreparation();

	void xsDebugLog( txString message )
	{
		qDebug( ) << message;
	}
}


QXsRunner::QXsRunner( litehtml::document* doc )
{
	m_document = doc;
}

void QXsRunner::run()
{
	fxInitializeSharedCluster();

	txPreparation* preparation = xsPreparation();

	txMachine	root;
	c_memset(&root, 0, sizeof(txMachine));

	root.machine_data	= m_document;
	root.preparation	= preparation;
	root.archive		= NULL;
	root.keyArray		= preparation->keys;
	root.keyCount		= (txID)preparation->keyCount + (txID)preparation->creation.keyCount;
	root.keyIndex		= (txID)preparation->keyCount;
	root.nameModulo		= preparation->nameModulo;
	root.nameTable		= preparation->names;
	root.symbolModulo	= preparation->symbolModulo;
	root.symbolTable	= preparation->symbols;

	root.stack			= &preparation->stack[0];
	root.stackBottom	= &preparation->stack[0];
	root.stackTop		= &preparation->stack[preparation->stackCount];

	root.firstHeap		= &preparation->heap[0];
	root.freeHeap		= &preparation->heap[preparation->heapCount - 1];
	root.aliasCount		= (txID)preparation->aliasCount;

	xsMachine* machine = fxCloneMachine( &preparation->creation, &root, (char*)"xs", NULL );

	xsBeginHost(machine);
	{
		xsTry {
			//xsVars(1);
			//xsVar(0) = xsNewHostObject( NULL );
			//xsSetHostData(xsVar(0), m_document );
			//xsVar(1) = xsNewHostFunction(dom_document_create_element, 0);
			//xsDefine( xsVar(0), xsID_createElement, xsVar(1), xsDefault );
			//xsSet(xsGlobal, fxID(the,"document"), xsVar(0));

//xsStartProfiling();
			fxRunModule(the, "C:\\Qt\\Projects\\PhotonBoard3\\test.js");
//xsStopProfiling();

			fxRunLoop(the);
		}
		xsCatch {
			xsStringValue message = xsToString(xsException);
			qDebug() << message;
		}
	}

	xsEndHost(the);
	xsDeleteMachine(machine);
	fxTerminateSharedCluster();
}





