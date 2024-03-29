#include "MinerClient.h"

#include <fstream>

using namespace grpc;

bool MinerClient::LastRequestSuccess = false;

MinerClient::MinerClient(const std::vector<std::string>& minerList) :
	MinerList(make_shared<std::vector<std::string>>(minerList))
{
}

void MinerClient::SwitchCoinInThread()
{
	const std::string& coin = NewCoin;

	std::stringstream ss;
	if (!coin.empty())
	{
		LastRequestSuccess = true;
		for (const std::string& serverAddress : *MinerList)
		{
			// Create gRPC channel (if not already created)
			std::shared_ptr<grpc::Channel> channel;
			channel = grpc::CreateChannel(serverAddress, grpc::InsecureChannelCredentials());

			// Create gRPC stub
			const auto stub = MinerController::NewStub(channel);

			// Prepare request message
			SwitchCoinRequest request;
			request.set_coin(coin);

			// Send gRPC call
			SwitchCoinResponse response;
			ClientContext context;
			Status status = stub->SwitchCoin(&context, request, &response);

			if (response.success())
			{
				string msg = stringFormat("Server %s: Coin switched successfully\n", serverAddress.c_str());
				std::cout << msg;
				ss << msg;
			}
			else
			{
				LastRequestSuccess = false;		// Any one server call fail marks the whole switch request as fail
				string msg = stringFormat("Server %s: %s\n", serverAddress.c_str(),
					response.message().empty() ? "Unknown error" : response.message().c_str());
				std::cerr << msg;
				ss << msg;
			}
		}
	}
	else
	{
		LastRequestSuccess = false;
		string msg = "Error: No coin provided for switching.\n";
		std::cerr << msg;
		ss << msg;
	}

	// Remove last \n
	// ReSharper disable once CppZeroConstantCanBeReplacedWithNullptr
	ss.seekp(-1,ss.cur); ss << '\0';

	// Emit a custom signal here to notify the UI about the gRPC status
	emit GrpcStatus(ss.str());

	if (LastRequestSuccess)
	{
		emit CurrentCoinChanged(coin);
	}
	
	emit FinishedSwitching();
}