#include "MinerControllerImpl.h"

#include <fstream>

using namespace grpc;

MinerControllerImpl::MinerControllerImpl(const std::string& configPath) :
	ConfigPath(configPath),
	// ReSharper disable CppClangTidyClangDiagnosticComma
	ListenAddress((LoadYamlConfig(configPath), Config["listen_address"].as<std::string>())), // Make sure we run the loading before the rest of initializers
	ListenPort(Config["listen_port"].as<int>()),
	BatchPath(Config["batch_path"].as<std::string>()),
	KillScript(Config["kill_miner_batch"].as<std::string>()),
	CoinMinerScripts(Config["CoinMinerScripts"])
{
}

Status MinerControllerImpl::SwitchCoin(ServerContext* context,
                                       const SwitchCoinRequest* request,
                                       SwitchCoinResponse* response)
{
	// Get miner script path for the requested coin
	if (!CoinMinerScripts.IsMap()) {
		response->set_success(false);
		response->set_message("Invalid CoinMinerScripts configuration");
		return {StatusCode::INTERNAL, ""};
	}

	// ReSharper disable once CppTooWideScopeInitStatement
	const auto minerScript = CoinMinerScripts[request->coin()].as<std::string>();
	if (minerScript.empty()) {
		response->set_success(false);
		response->set_message("Coin not found in CoinMinerScripts");
		return {StatusCode::INVALID_ARGUMENT, ""};
	}

	// Modify batch file content
	std::ofstream batchFile(BatchPath);
	batchFile << "@echo off\nstart \"\" /min " << minerScript << "\nexit /b";
	batchFile.close();

	// Execute scripts
	if (std::system(stringFormat("\"%s\"", KillScript.c_str()).c_str()))
	{
		response->set_success(false);
		response->set_message("Failed to kill previous miner");
		return {StatusCode::UNKNOWN, ""};
	}
	
	if (std::system(stringFormat("\"%s\"", BatchPath.c_str()).c_str()))
	{
		response->set_success(false);
		response->set_message("Failed to start new miner");
		return {StatusCode::UNKNOWN, ""};
	}

	// Set response and return success
	response->set_success(true);
	response->set_message("Coin switched successfully");
	return Status::OK;
}

void MinerControllerImpl::LoadYamlConfig(const std::string& path)
{
	if (ConfigPath.empty())
	{
		std::cerr << "Error loading YAML config: config file path is empty." << std::endl;
		return;
	}

	try
	{
		Config = YAML::LoadFile(ConfigPath);
	}
	catch (const std::exception& e) {
		// Handle YAML parsing errors
		std::cerr << "Error loading YAML config: " << e.what() << std::endl;
		throw;
	}
}