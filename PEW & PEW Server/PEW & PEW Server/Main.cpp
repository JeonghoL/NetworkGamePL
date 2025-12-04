#include "pch.h"

int main()
{
	setlocale(LC_ALL, "korean");

	Logger::Init();
	Logger::SetLevel(LogLevel::Info);

	auto service = std::make_shared<Service>();
	
	if (not service->Init()) {
		LOG_ERR("Service Init Failed");
		return -1;
	}

	service->Start();

	std::cout << "종료하려면 Enter 키를 누르세요...\n";
	std::cin.get();

	service->Stop();
}