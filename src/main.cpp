#include "httpServer.h"


int main(int argc, char** argv)
{
	std::map<std::string, std::string> param2Value;
	
	HTTPServer httpServer;
	httpServer.run();

	return 0;
}

