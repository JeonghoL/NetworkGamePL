#pragma 

#include <thread>
#include <atomic>
#include <WinSock2.h>

class IOThread {
public:
	IOThread();
	~IOThread() { Stop(); }

	void Start(SOCKET socket);
	void Stop();

	bool IsRunning() const { return m_running; }
		 
private:
	void Thread_Func();

private:
	std::thread m_thread;
	SOCKET m_clientSocket;

	std::atomic<bool> m_running;
};

