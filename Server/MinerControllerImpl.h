#pragma once

#include "Common.h"

/* Disabling -Wshadow warning on grpc and its generated code */
DISABLE_WARNING()
#include "grpcpp/grpcpp.h"
#include "coinswitcher.grpc.pb.h"
ENABLE_WARNING()

#include <yaml-cpp/yaml.h> // Include YAML library

class MinerControllerImpl final : public MinerController::Service
{
public:
    explicit MinerControllerImpl(const std::string& configPath);

    grpc::Status SwitchCoin(grpc::ServerContext* context, const SwitchCoinRequest* request,
							SwitchCoinResponse* response) override;

    std::string GetListenAddress() const { return ListenAddress; }
    int GetListenPort() const { return ListenPort; }

private:
    void LoadYamlConfig(const std::string& path);

    const std::string& ConfigPath;
    YAML::Node Config;
    const std::string ListenAddress;
    const int ListenPort;
    const std::string BatchPath;
	const std::string KillScript;
    const YAML::Node CoinMinerScripts;

};