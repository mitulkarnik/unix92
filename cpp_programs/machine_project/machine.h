#ifndef _MACHINE_H
#define _MACHINE_H

#include <string>
#include <memory>
#include <mutex>

enum MachineFields
{
	machineName,
	machineIp,
	machineMAC,
	machineHostName
};

struct Machine
{

	std::string m_machineName;
	std::string m_ip;
	std::string m_mac;
	std::string m_hostname;
	// Timestamp for heartbeat if first request then create the entry 
	std::string m_keepalive;  
	
};


class MachineManager
{
public:
	// CRUD operations (Public interfaces)
	int createMachine(std::shared_ptr<Machine> machinePtr);
	int updateMachine(std::shared_ptr<Machine> machinePtr);
	int deleteMachine(std::shared_ptr<Machine> machinePtr);
	int queryMachine(std::shared_ptr<Machine> machinePtr);
	int createOrUpdateMachine(std::shared_ptr<Employee> machinePtr);

private:
	// Need to have Persistence handle 
	//
};

class MachineSerializer
{
	friend class MachineManager;
private:
	static std::shared_ptr<MachineSerializer> getMachineSerializerInstance();
	// Private Constructor
	MachineSerializer()
	{
		machineList.clear();
		privateInstance = NULL;
	}
	// Interface functions to access the internal list of machines
	int insertMachine(std::shared_ptr<Machine> machine);
	int getMachine(std::shared_ptr<Machine> machine, int *index);
	int deleteMachine(std::shared_ptr<Machine> machine);
	int updateMachine(std::shared_ptr<Machine> machine);
	// Private functions
	
	// Maintain the queue of Mahchine in Memory
	std::vector<std::shared_ptr<Machine> > machineList;
	std::mutex listLock;
	std::shared_ptr<Employee> privateInstance;
};

#endif 
