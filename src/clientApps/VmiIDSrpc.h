/*
 * VmiIDSrpc.h
 *
 *  Created on: Jul 8, 2010
 *      Author: kittel
 */

#ifndef VMIIDSRPC_H_
#define VMIIDSRPC_H_

#include "vmiids/VmiIDS.h"

#include <rpc/rpc.h>
#include <string>

class VmiIDSrpc {
private:
	CLIENT *clnt;

	static VmiIDSrpc *instance;

public:
	VmiIDSrpc();
	virtual ~VmiIDSrpc();

	static VmiIDSrpc *getInstance();

	bool enqueueDetectionModule(std::string detectionModuleName);
	bool dequeueDetectionModule(std::string detectionModuleName);
	std::string runSingleDetectionModule(std::string module);
	bool stopIDS(int signum = 0);
	bool loadSharedObject(std::string path);
};

#endif /* VMIIDSRPC_H_ */
