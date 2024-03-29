#pragma once

#include "MinerClient.h"

#include "yaml-cpp/yaml.h"
#include "QtWidgets/QtWidgets"		// Include for basic UI elements
#include "QSignalMapper"
#include <iostream>
#include <fstream>

// Local function prototypes
static void UpdateUIFromConfig();
static void SwitchButtonOnClick();
static void UpdateCurrentCoinInList(const std::string& newCoin);
static void SwitchCoin(const std::string& coin);

// Structure and class declarations

class ConfigManager final : public QObject
{
	Q_OBJECT
		
private:
	static ConfigManager* s_SingleInstance;
	static std::string s_ConfigPath;
	static std::unique_ptr<YAML::Node> s_Config;
	static std::string s_CurrentCoin;
    static std::map<std::string, std::string> s_CoinList;
    static std::vector<std::string> s_MinerList;
	
	explicit ConfigManager() = default;

public:
	ConfigManager(ConfigManager &other) = delete;
    void operator=(const ConfigManager &) = delete;
	static ConfigManager* GetInstance();

	static void SetCurrentCoin(const std::string& currentCoin) { s_CurrentCoin = currentCoin; }

	static auto& GetCurrentCoin() { return s_CurrentCoin; }
	static auto& GetCoinList() { return s_CoinList; }
	static auto& GetMinerList() { return s_MinerList; }

	static void LoadYamlConfig(const std::string& configPath = s_ConfigPath);
	static void SaveCurrentCoinToYaml(const std::string& newCoin);

signals:
	void ConfigLoaded();
};

// ReSharper disable once CppInconsistentNaming
class GUI final : public QObject
{
	Q_OBJECT

private:
	int Argc;
	char** Argv;
	static GUI* s_SingleInstance;

	static std::unique_ptr<QApplication> s_App;
	static std::unique_ptr<QWidget> s_Window;
	static std::unique_ptr<QLabel> s_CoinListLabel;
	static std::unique_ptr<QListWidget> s_CoinListWidget;
	static std::unique_ptr<QPushButton> s_SwitchButton;
	static std::unique_ptr<QLabel> s_StatusLabel;
	static std::unique_ptr<QVBoxLayout> s_Layout;
	
	explicit GUI(int argc = 0, char* argv[] = nullptr);

public:
	GUI(GUI &other) = delete;
    void operator=(const GUI &) = delete;
	static GUI* GetInstance(int argc = 0, char* argv[] = nullptr);

	static auto GetApp() { return s_App.get(); }
	static auto GetWindow() { return s_Window.get(); }
	static auto GetCoinListLabel() { return s_CoinListLabel.get(); }
	static auto GetCoinListWidget() { return s_CoinListWidget.get(); }
	static auto GetSwitchButton() { return s_SwitchButton.get(); }
	static auto GetStatusLabel() { return s_StatusLabel.get(); }
	static auto GetLayout() { return s_Layout.get(); }
};
