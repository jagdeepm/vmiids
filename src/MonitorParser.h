#ifndef _MONITORPARSER_H_
#define _MONITORPARSER_H_

#include "ConsoleMonitor.h"

namespace libVMI {

/*!
 * \exception MonitorParserException MonitorParser.h
 * \brief Exception for MonitorParser.
 */
class MonitorParserException: public std::exception {
	virtual const char* what() const throw () {
		return "ConsoleMonitor abort";
	}
};

/*!
 * \class MonitorParser MonitorParser.h
 *
 * \brief Parser for the QEmu Monitor Console.
 *
 * The normal way to communicate with the QEmu Hypervisor is
 * the QEmu Monitor Console. \sa http://wiki.qemu.org/download/qemu-doc.html#pcsys_005fmonitor<p>
 *
 * At the time of implementation the following commands were available:
 *
 *
 * <pre>
 help|? [cmd] 					-- show the help
commit device|all 				-- commit changes to the disk images (if -snapshot is used) or backing files
info [subcommand] 				-- show various information about the system state
q|quit  					-- quit the emulator
eject [-f] device 				-- eject a removable medium (use -f to force it)
change device filename [format]		 	-- change a removable medium, optional format
screendump filename 				-- save screen into PPM image 'filename'
logfile filename 				-- output logs to 'filename'
log item1[,...] 				-- activate logging of the specified items to '/tmp/qemu.log'
savevm [tag|id] 				-- save a VM snapshot. If no tag or id are provided, a new snapshot is created
loadvm tag|id 					-- restore a VM snapshot from its tag or id
delvm tag|id 					-- delete a VM snapshot from its tag or id
singlestep [on|off] 				-- run emulation in singlestep mode or switch to normal mode
stop  						-- stop emulation
c|cont  					-- resume emulation
gdbserver [device] 				-- start gdbserver on given device (default 'tcp::1234'), stop with 'none'
x /fmt addr 					-- virtual memory dump starting at 'addr'
xp /fmt addr 					-- physical memory dump starting at 'addr'
p|print /fmt expr 				-- print expression value (use $reg for CPU register access)
i /fmt addr 					-- I/O port read
o /fmt addr value 				-- I/O port write
sendkey keys [hold_ms] 				-- send keys to the VM (e.g. 'sendkey ctrl-alt-f1', default hold time=100 ms)
system_reset  					-- reset the system
system_powerdown  				-- send system power down event
sum addr size 					-- compute the checksum of a memory region
usb_add device 					-- add USB device (e.g. 'host:bus.addr' or 'host:vendor_id:product_id')
usb_del device 					-- remove USB device 'bus.addr'
device_add device 				-- add device, like -device on the command line
device_del device 				-- remove device
cpu index 					-- set the default CPU
mouse_move dx dy [dz] 				-- send mouse move events
mouse_button state 				-- change mouse button state (1=L, 2=M, 4=R)
mouse_set index 				-- set which mouse device receives events
wavcapture path [frequency [bits [channels]]] 	-- capture audio to a wave file (default frequency=44100 bits=16 channels=2)
stopcapture capture index 			-- stop capture
memsave addr size file 				-- save to disk virtual memory dump starting at 'addr' of size 'size'
pmemsave addr size file 			-- save to disk physical memory dump starting at 'addr' of size 'size'
boot_set bootdevice 				-- define new values for the boot device list
nmi cpu 					-- inject an NMI on the given CPU
migrate [-d] [-b] [-i] uri 			-- migrate to URI (using -d to not wait for completion)
			 -b for migration without shared storage with full copy of disk
			 -i for migration without shared storage with incremental copy of disk (base image shared between src and destination)
migrate_cancel  				-- cancel the current VM migration
migrate_set_speed value 			-- set maximum speed (in bytes) for migrations
migrate_set_downtime value 			-- set maximum tolerated downtime (in seconds) for migrations
drive_add [[<domain>:]<bus>:]<slot>
	[file=file][,if=type][,bus=n]
	[,unit=m][,media=d][index=i]
	[,cyls=c,heads=h,secs=s[,trans=t]]
	[snapshot=on|off][,cache=on|off] 	-- add drive to PCI storage controller
pci_add auto|[[<domain>:]<bus>:]
	<slot> nic|storage|host
	[[vlan=n][,macaddr=addr][,model=type]]
	 [file=file][,if=type][,bus=nr]...
	[host=02:00.0[,name=string][,dma=none] 	-- hot-add PCI device
pci_del [[<domain>:]<bus>:]<slot> 		-- hot remove PCI device
host_net_add tap|user|socket|vde|dump [options] -- add host VLAN client
host_net_remove vlan_id name 			-- remove host VLAN client
hostfwd_add [vlan_id name]
	[tcp|udp]:[hostaddr]:hostport-
	[guestaddr]:guestport 			-- redirect TCP or UDP connections from host to guest (requires -net user)
hostfwd_remove [vlan_id name]
	[tcp|udp]:[hostaddr]:hostport 		-- remove host-to-guest TCP or UDP redirection
balloon target 					-- request VM to change it's memory allocation (in MB)
set_link name up|down 				-- change the link status of a network adapter
watchdog_action
[reset|shutdown|poweroff|pause|debug|none] 	-- change watchdog action
acl_show aclname 				-- list rules in the access control list
acl_policy aclname allow|deny 			-- set default access control list policy
acl_add aclname match allow|deny [index] 	-- add a match rule to the access control list
acl_remove aclname match 			-- remove a match rule from the access control list
acl_reset aclname 				-- reset the access control list
mce cpu bank status mcgstatus addr misc 	-- inject a MCE on the given CPU
getfd getfd name 				-- receive a file descriptor via SCM rights and assign it a name
closefd closefd name 				-- close a file descriptor previously passed via SCM rights
block_passwd block_passwd device password 	-- set the password of encrypted block devices
cpu_set cpu [online|offline] 			-- change cpu state
</pre>

Furthermore the following info commands were available:<br>
<pre>
info version           -- show the version of QEMU
info commands          -- list QMP available commands
info network           -- show the network state
info chardev           -- show the character devices
info block             -- show the block devices
info blockstats        -- show block device statistics
info registers         -- show the cpu registers
info cpus              -- show infos for each CPU
info history           -- show the command line history
info irq               -- show the interrupts statistics (if available)
info pic               -- show i8259 (PIC) state
info pci               -- show PCI info
info tlb               -- show virtual to physical memory mappings
info mem               -- show the active virtual memory mappings
info hpet              -- show state of HPET
info jit               -- show dynamic compiler info
info kvm               -- show KVM information
info numa              -- show NUMA information
info usb               -- show guest USB devices
info usbhost           -- show host USB devices
info profile           -- show profiling information
info capture           -- show capture information
info snapshots         -- show the currently saved VM snapshots
info status            -- show the current VM status (running|paused)
info pcmcia            -- show guest PCMCIA status
info mice              -- show which guest mouse is receiving events
info vnc               -- show the vnc server status
info name              -- show the current VM name
info uuid              -- show the current VM UUID
info usernet           -- show user network stack connection states
info migrate           -- show migration status
info balloon           -- show balloon information
info qtree             -- show device tree
info qdm               -- show qdev device model list
info roms              -- show roms
</pre>

 */
class MonitorParser : public ConsoleMonitor {
	public:
		MonitorParser() throw(MonitorParserException);
		virtual ~MonitorParser();

		/**
		 * \brief Function to get the current vm state.
		 * @return Returns true when the monitored VM is currently running.
		 */
		bool isRunning() throw(MonitorParserException);

		/**
		 * \brief Function to pause the execution of the monitored VM.
		 */
		void pauseVM() throw(MonitorParserException);

		/**
		 * \brief Function to resume the execution of the monitored VM.
		 */
		void resumeVM() throw(MonitorParserException);

		/**
		 * \brief show the help.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void help(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show the help of the info command.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoHelp(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show the version of QEMU.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoVersion(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief list QMP available commands.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoCommands(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show the network state.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoNetwork(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show the character devices.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoChardev(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show the block devices.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoBlock(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show block device statistics.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoBlockstats(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show the cpu registers.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoRegisters(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show infos for each CPU.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoCpus(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show the command line history.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoHistory(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show the interrupts statistics (if available).
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoIrq(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show i8259 (PIC) state.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoPic(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show PCI info.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoPci(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show virtual to physical memory mappings.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoTlb(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show the active virtual memory mappings.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoMem(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show state of HPET.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoHpet(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show dynamic compiler info.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoJit(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show KVM information.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoKvm(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show NUMA information.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoNuma(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show guest USB devices.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoUsb(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show host USB devices.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoUsbhost(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show profiling information.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoProfile(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show capture information.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoCapture(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show the currently saved VM snapshots.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoSnapshots(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show the current VM status (running|paused).
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoStatus(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show guest PCMCIA status.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoPcmcia(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show which guest mouse is receiving events.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoMice(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show the vnc server status.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoVnc(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show the current VM name.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoName(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show the current VM UUID.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoUuid(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show user network stack connection states.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoUsernet(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show migration status.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoMigrate(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show balloon information.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoBallon(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show device tree.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoQtree(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show qdev device model list.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoQdm(std::string &helptext) throw(MonitorParserException);

		/**
		 * \brief show roms.
		 * @param helptext String Buffer in which the result will be returned in.
		 */
		void infoRoms(std::string &helptext) throw(MonitorParserException);

};

}
#endif /* _MONITORPARSER_H_ */
