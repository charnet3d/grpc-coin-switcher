#include "MinerControllerImpl.h"

using namespace std;
using namespace grpc;

#include <QDir>

int main()
{
	// explicitly using the relative name of the current working directory
	const QDir dir(".");
	const std::string configFilename("server_config.yaml");
	const std::string configAbsPath = dir.absoluteFilePath(configFilename.c_str()).toStdString();

	if (!dir.exists(QString::fromStdString(configFilename)))
		cout << "Config file " << configAbsPath << " doesn't exist." << endl;

    ServerBuilder builder;
	
	MinerControllerImpl minerControllerService(configAbsPath);

	const string listenAddress = stringFormat("%s:%d",
		minerControllerService.GetListenAddress().c_str(),
		minerControllerService.GetListenPort());

    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(listenAddress, grpc::InsecureServerCredentials());
	
    // Register "service" as the instance through which we'll communicate with clients.
    builder.RegisterService(&minerControllerService);

	// Finally assemble the server.
    const std::unique_ptr<Server> server(builder.BuildAndStart());
	
	cout <<  stringFormat("Listening for Coin Switch requests on %s ...", listenAddress.c_str()) << endl;
	
	server->Wait();

	return 0;
}
