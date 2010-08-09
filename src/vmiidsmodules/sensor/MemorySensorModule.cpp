/*
 * MemorySensorModule.cpp
 *
 *  Created on: Jul 23, 2010
 *      Author: kittel
 */

#include "MemorySensorModule.h"

#include <sstream>

ADDDYNAMICSENSORMODULE(MemorySensorModule, __LINE__)
;

MemorySensorModule::MemorySensorModule() :
			SensorModule("MemorySensorModule") {

	//Get NotificationModule
	GETNOTIFICATIONMODULE(notify, ShellNotificationModule);

	GETOPTION(memtoolPath, this->memtoolPath);
	GETOPTION(memtoolScriptPath, this->memtoolScriptPath);
	GETOPTION(savedDebugingSymbols, this->savedDebugingSymbols);
	GETOPTION(memdumpFile, this->memdumpFile);
}

MemorySensorModule::~MemorySensorModule() {
	// TODO Auto-generated destructor stub
}


void MemorySensorModule::initSensorModule(){

}

void MemorySensorModule::runScript(std::string &scriptResult, std::string scriptName){
	std::stringstream command;

	std::string data;
	FILE *stream;
	char buffer[1024];

	command << "echo \"sc " << this->memtoolScriptPath << "/" << scriptName << "\" | "
			<< this->memtoolPath << " -l " << this->savedDebugingSymbols
			<< " -m " << this->memdumpFile << " 2>&1";

	stream = popen(command.str().c_str(), "r");
	while (fgets(buffer, 1024, stream) != NULL)
		data.append(buffer);
	pclose(stream);

	int firstChar;
	int lastChar;

	lastChar = data.rfind(">>>");

	firstChar = data.find(">>>");
	firstChar = data.find("\n", firstChar);
	firstChar++;

	scriptResult.append(data, firstChar, lastChar - firstChar);
}

void MemorySensorModule::getProcessList(std::map<uint32_t, MemtoolProcess> &memtoolProcessMap){
	std::string scriptResult;

	this->clearFSCache();

	this->runScript(scriptResult, "tasklist.js");

	size_t oldNewlineSeparator = 0;
	size_t newlineSeparator = 0;

	oldNewlineSeparator = scriptResult.find("\n");
	oldNewlineSeparator = scriptResult.find("\n", oldNewlineSeparator + 1); //Find the third line ...

	size_t position1 = 0;

	std::string currentLine;

	oldNewlineSeparator = scriptResult.find("\n", oldNewlineSeparator) + 1;
	while((newlineSeparator = scriptResult.find("\n", oldNewlineSeparator)) != std::string::npos){
		currentLine = scriptResult.substr(oldNewlineSeparator, newlineSeparator - oldNewlineSeparator);
		MemtoolProcess process;
		process.pid = atoi(currentLine.substr(5, 8).c_str());
		process.processName = currentLine.substr(23, 17);
		position1 = process.processName.find("\"");
		process.processName = process.processName.substr(0, position1);
		memtoolProcessMap.insert(std::pair<uint32_t, MemtoolProcess>(process.pid, process));
		oldNewlineSeparator = newlineSeparator+1;
	}
	return;
}


bool MemorySensorModule::clearFSCache() {
	bool result = true;

	// To clear the file system cache and get the latest version of the rootkitvms file system.
	result = (system("sync") == -1) ? false : true;
	result = (system(this->clearCacheCommand.c_str()) == -1) ? false : true;
	return result;
}
