/*
 * QemuMonitor.cpp
 *
 *  Created on: Jun 10, 2010
 *      Author: kittel
 */

#include "QemuMonitor.h"


#define DEBUG

#ifdef DEBUG
#define VERBOSE "QemuMonitor"
#endif /* DEBUG */
#include "Debug.h"


namespace libVMI {

#define MONITORDEVICE "/dev/ttyS1"
#define MONITORSHELL  "(qemu)"

QemuMonitor::QemuMonitor()  throw(QemuMonitorException):
		QemuMonitorParser(MONITORDEVICE, MONITORSHELL){}

QemuMonitor::~QemuMonitor() {}


bool QemuMonitor::isRunning() throw(QemuMonitorException){
	LIBVMI_DEBUG_MSG("isRunning called");
	std::string string;
	this->infoStatus(string);
	if(string.rfind("running") != std::string::npos) return true;
	else if(string.rfind("paused") != std::string::npos) return false;
	throw QemuMonitorParserException();
}

void QemuMonitor::pauseVM() throw(QemuMonitorException){
	LIBVMI_DEBUG_MSG("pauseVM called");
	std::string string;
	this->cmdStop(string);
}

void QemuMonitor::resumeVM() throw(QemuMonitorException){
	LIBVMI_DEBUG_MSG("resumeVM called");
	std::string string;
	this->cmdCont(string);
}


}