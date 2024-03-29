#pragma once

#include "Common.h"
#include "QSignalMapper"
#include "QThread"

/* Disabling -Wshadow warning on grpc and its generated code */
DISABLE_WARNING()
#include "grpcpp/grpcpp.h"
#include "coinswitcher.grpc.pb.h"
ENABLE_WARNING()

class MinerClient final : public QObject
{
	Q_OBJECT

public:
    explicit MinerClient(const std::vector<std::string>& minerList);

	[[nodiscard]] static bool WasLastRequestSuccess() { return LastRequestSuccess; }

	// Should be used before starting Switching thread
	void SetNewCoin(const std::string& coin) { NewCoin = coin; }

public slots:
	void SwitchCoinInThread();

signals:
	void CurrentCoinChanged(const std::string& coin);
	void GrpcStatus(const std::string& message); // Signal for notifying UI about gRPC status
	void FinishedSwitching();
private:
    std::shared_ptr<std::vector<std::string>> MinerList;
	std::string NewCoin; // Member variable to store coin data for thread access

	static bool LastRequestSuccess;
};