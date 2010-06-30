/*
 * VmiIDS.h
 *
 *  Created on: Jun 26, 2010
 *      Author: kittel
 */

#ifndef VMIIDS_H_
#define VMIIDS_H_

#include "DetectionModule.h"
#include "NotificationModule.h"
#include "SensorModule.h"

#include <map>
#include <string>

class VmiIDS {
	private:
		std::map<std::string, DetectionModule *> detectionModules;
		pthread_mutex_t detectionModuleMutex;
		std::map<std::string, NotificationModule *> notificationModules;
		pthread_mutex_t notificationModuleMutex;
		std::map<std::string, SensorModule* > sensorModules;
		pthread_mutex_t sensorModuleMutex;

		static VmiIDS *instance;
		pthread_t rpcThread, vmiidsThread;

		bool vmiRunning;

		VmiIDS();

		void loadSharedObjectsInitial(std::string path);

	public:
		virtual ~VmiIDS();

		static VmiIDS *getInstance();
		int startIDS();
		void waitIDS();
		void stopIDS(int signum = 0);
		void loadSharedObject(std::string path);

		static void * run(void * this_pointer);

		void enqueueDetectionModule(DetectionModule *detectionModule);
		bool enqueueDetectionModule(std::string detectionModuleName);
		bool dequeueDetectionModule(std::string detectionModuleName);


		void enqueueNotificationModule(NotificationModule *notificationModule);
		bool enqueueNotificationModule(std::string notificationModuleName);
		bool dequeueNotificationModule(std::string notificationModuleName);

		void enqueueSensorModule(SensorModule *sensorModule);

		NotificationModule *getNotificationModule(std::string notificationModuleName);
		SensorModule *getSensorModule(std::string sensorModuleName);

		void collectThreadLevel();

};

#endif /* VMIIDS_H_ */