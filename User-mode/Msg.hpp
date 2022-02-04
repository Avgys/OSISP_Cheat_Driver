struct Msg {
	void* pFound;
	unsigned long long pid;
	void* value;
	size_t valueLength;
	size_t* addrs;
};