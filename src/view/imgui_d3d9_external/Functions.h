#include "proc/process_helper.h"
DWORD GetProcessId(LPCSTR ProcessName) {
    std::vector<uint32_t> pids = ProcessHelper::getProcessIdsByName(ProcessName);
    if (pids.size() <= CONF_PROCESS_INDEX) {
        return 0;
    }
    return pids[CONF_PROCESS_INDEX];
}

std::string RandomString(int len) {
	srand(time(NULL));
	std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string newstr;
	int pos;
	while (newstr.size() != len) {
		pos = ((rand() % (str.size() - 1)));
		newstr += str.substr(pos, 1);
	}
	return newstr;
}