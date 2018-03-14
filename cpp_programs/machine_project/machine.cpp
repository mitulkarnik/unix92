
#include "machine.h"
#include <iostream>

// Singletin instance 
std::shared_ptr<MachineSerializer> MachineSerializer::getMachineSerializerInstance()
{
	if(privateInstance == NULL)
	{
		privateInstance = std::make_shared<MachineSerializer>();		
	}
	return privateInstance;
} 

int MachineSerializer::insertMachine(std::shared_ptr<Machine> machine)
{
	// insert the mahchine into the list
	std::lock_guard<std::mutex> guard(listLock);
	list.push_back(machine);
}

int MachineSerializer::getMachine(std::shared_ptr<Machine> machine, std::shared_ptr<Machine> &result, int *index)
{
	MachineFields field;
	std::string fieldValue;

	// Query the machine based on the details provided 
	if(machine->m_machineName != "")
	{
		field  = machineName;
		fieldValue = machine->m_machineName;
	}
	else if(machine->m_ip != "")
	{
		field = machineIp;
		fieldValue = machine->m_ip;
	}
	else if(machine->m_mac != "")
	{
		field = machineMAC;
		fieldValue = machine->m_mac;
	}	
	else if(machine->hostname)
	{
		field = machineHostName;
		fieldValue = machine->m_hostname;
	}
	
	result = queryMachine(field, fieldValue, index);
	return 0;			
}

std::shared_ptr<Machine> MachineSerializer::queryMachine(MachineFields field, std::string fieldValue, int *outIndex)
{
	std::vector<std::shared_ptr<Machine> >::iterator it = machineList.begin();
	std::shared_ptr<Machine> result = NULL;

	unsigned int index = 0;

	while(it != machineList.end())
	{
		switch(field)
		{
			case machineName:
				if(it->m_machineName == fieldValue)
				{
					result = it;
					goto end;
				}
			break; 
			case machineIp:
				if(it->m_ip == fieldValue)
				{
					result = it;
					goto end;
				}
			case machineMAC:
				if(it->m_mac == fieldValue)
				{		
					result = it;
					goto end;
				}
			break;
			case maachineHostName:
				if(it->m_hostname == fieldValue)
				{
					result = it;
					goto end;
				}
		}
		index++;
	}

end:
	*outIndex = index;
	std::cout << "Terminating from the querying condition" << std::endl;
	return result;
}

int MachineSerializer::deleteMachine(std::shared_ptr<Machine> machine)
{
	int index = 0;
	getMachine(machine, &index);

	std::lock_guard<std::mutex> guard(listLock);

	machineList.erase(index);
}

// update the Machine record 
// Find the machine copy the id to the target and then do the repace 
// operation

