
#include "main.h"

// Structure and class definitions

std::string ConfigManager::s_CurrentCoin;
std::map<std::string, std::string> ConfigManager::s_CoinList;
std::vector<std::string> ConfigManager::s_MinerList;

ConfigManager* ConfigManager::s_SingleInstance = nullptr;
std::string ConfigManager::s_ConfigPath;
std::unique_ptr<YAML::Node> ConfigManager::s_Config;

ConfigManager* ConfigManager::GetInstance()
{
	if (!s_SingleInstance)
		s_SingleInstance = new ConfigManager();
	return s_SingleInstance;
}

void ConfigManager::LoadYamlConfig(const std::string& configPath)
{
	if (configPath.empty())
	{
		std::cerr << "Error loading YAML config: config file path is empty." << std::endl;
		return;
	}
	
	s_ConfigPath = configPath;

	try
	{
		s_Config = std::make_unique<YAML::Node>(YAML::LoadFile(s_ConfigPath));
		if (s_Config)
		{
			s_CurrentCoin = (*s_Config)["CurrentCoin"].as<std::string>();
			s_CoinList = (*s_Config)["CoinList"].as<std::map<std::string, std::string>>();
			s_MinerList = (*s_Config)["MinerList"].as<std::vector<std::string>>();

			emit GetInstance()->ConfigLoaded();
		}
		else
		{
			std::cout << "Config file " << s_ConfigPath << " couldn't be loaded." << std::endl;
			return;
		}
	}
	catch (const std::exception& e)
	{
		// Handle YAML parsing errors
		std::cerr << "Error loading YAML config: " << e.what() << std::endl;
		return;
	}
}

void ConfigManager::SaveCurrentCoinToYaml(const std::string& newCoin)
{
	if (s_ConfigPath.empty())
	{
		std::cerr << "Error saving YAML config: config file path is empty." << std::endl;
		return;
	}

	if (!s_Config)
	{
		LoadYamlConfig();
		if (!s_Config)
		{
			std::cerr << "Error saving YAML config: invalid config in memory." << std::endl;
			return;
		}
	}

	// Save the updated config to the file
	try
	{
		// Update currentCoin in the config
		(*s_Config)["CurrentCoin"] = s_CurrentCoin = newCoin;

		std::ofstream configFile(s_ConfigPath);
		YAML::Emitter out(configFile);
		out << (*s_Config);
		std::cout << "Successfully saved current coin to " << s_ConfigPath << std::endl;
		configFile.close();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error saving current coin to YAML: " << e.what() << std::endl;
	}
}


GUI* GUI::s_SingleInstance = nullptr;
std::unique_ptr<QApplication> GUI::s_App;
std::unique_ptr<QWidget> GUI::s_Window;
std::unique_ptr<QLabel> GUI::s_CoinListLabel;
std::unique_ptr<QListWidget> GUI::s_CoinListWidget;
std::unique_ptr<QPushButton> GUI::s_SwitchButton;
std::unique_ptr<QLabel> GUI::s_StatusLabel;
std::unique_ptr<QVBoxLayout> GUI::s_Layout;

// ReSharper disable once CppParameterMayBeConst
GUI::GUI(int argc, char* argv[]) :
	Argc(argc),
	Argv(argv)
{
	// Qt Application
	s_App = std::make_unique<QApplication>(argc, argv);

	// Example UI with basic coin list and switch button (replace with your design)
	s_Window = std::make_unique<QWidget>();
	s_Window->setWindowTitle("Miner Control");
	
	// List label
	s_CoinListLabel = std::make_unique<QLabel>("Choose new coin to mine (Current coin in Green):");

	// Coin list widget
	s_CoinListWidget = std::make_unique<QListWidget>();

	// Switch button
	s_SwitchButton = std::make_unique<QPushButton>("Switch Coin");

	// Status label
	s_StatusLabel = std::make_unique<QLabel>();

	// Layout
	s_Layout = std::make_unique<QVBoxLayout>();
	s_Layout->addWidget(s_CoinListLabel.get());
	s_Layout->addWidget(s_CoinListWidget.get());
	s_Layout->addWidget(s_SwitchButton.get());
	s_Layout->addWidget(s_StatusLabel.get());
	s_Window->setLayout(s_Layout.get());
	
	s_Window->show();
}

GUI* GUI::GetInstance(int argc, char* argv[])
{
	if (!s_SingleInstance)
		s_SingleInstance = new GUI(argc, argv);
	return s_SingleInstance;
}


// Local function implementation

static void UpdateCurrentCoinInList(const std::string& newCoin)
{
	QListWidget* list = GUI::GetCoinListWidget();
	if (!list) return;

	// Find the List item that refers to the new coin
	for (int i = 0; i < list->count(); ++i)
	{
		// Update UI to reflect the new current coin (e.g., highlight selected item)
		if (list->item(i)->data(Qt::UserRole).toString().toStdString() == newCoin)
		{
			list->setCurrentItem(list->item(i));
			list->item(i)->setData(Qt::BackgroundRole, QColor("#44FF44"));
		}
		else
		{
			list->item(i)->setData(Qt::BackgroundRole, QColor("#FFFFFF"));
		}
	}
}

// ReSharper disable once CppInconsistentNaming
static void UpdateUIFromConfig()
{
	QListWidget* list = GUI::GetCoinListWidget();
	if (!list) return;

	list->clear();
	for (const auto& [coinName, displayText] : ConfigManager::GetCoinList() )
	{
		const auto item = new QListWidgetItem();
		item->setText(QString::fromStdString(displayText));
		item->setData(Qt::UserRole, QString::fromStdString(coinName));
		list->addItem(item);
	}
	if (!ConfigManager::GetCurrentCoin().empty())
		UpdateCurrentCoinInList(ConfigManager::GetCurrentCoin());
}

static void SwitchButtonOnClick()
{
	QLabel* statusLabel = GUI::GetStatusLabel();
	QPushButton* switchButton = GUI::GetSwitchButton();
	
	const QListWidget* list = GUI::GetCoinListWidget();
	if (!list) return;

	if (statusLabel)
		statusLabel->setText("");

	if (const auto item = list->currentItem())
	{
		// ReSharper disable once CppTooWideScopeInitStatement
		const std::string key = item->data(Qt::UserRole).toString().toStdString();
		if (key != ConfigManager::GetCurrentCoin())
		{
			if (switchButton)
				switchButton->setEnabled(false);
			SwitchCoin(key);
		}
		else if (statusLabel)
		{
			statusLabel->setText("Info: Selected coin is already the current one being mined.");
			statusLabel->setStyleSheet("QLabel { color : brown; }");
		}
	}
}

static void SwitchCoin(const std::string& coin)
{
	// Create MinerClient instance
	auto client = new MinerClient(ConfigManager::GetMinerList());
	client->SetNewCoin(coin);

	QObject::connect(client, &MinerClient::CurrentCoinChanged, GUI::GetInstance(),
		[](const std::string& newCoin)
		{
			ConfigManager::SaveCurrentCoinToYaml(newCoin);
			UpdateCurrentCoinInList(newCoin);
		});

	QObject::connect(client, &MinerClient::GrpcStatus, GUI::GetInstance(),
		[](const std::string& message)
		{
			QPushButton* switchButton = GUI::GetSwitchButton();

			QLabel* statusLabel = GUI::GetStatusLabel();
			if (!statusLabel) return;

			statusLabel->setText(QString::fromStdString(message)); // Update status label with error message

			if (MinerClient::WasLastRequestSuccess())
				statusLabel->setStyleSheet("QLabel { color : green; }");
			else
				statusLabel->setStyleSheet("QLabel { color : red; }");
			
			if (switchButton)
				switchButton->setEnabled(true);
		});

	const auto switchThread = new QThread();      // Thread for switching coin
	client->moveToThread(switchThread);
	QObject::connect(switchThread, &QThread::started, client, &MinerClient::SwitchCoinInThread);
	QObject::connect(client, &MinerClient::FinishedSwitching, switchThread, &QThread::quit);
	QObject::connect(client, &MinerClient::FinishedSwitching, client, &MinerClient::deleteLater);
	QObject::connect(switchThread, &QThread::finished, switchThread, &QThread::deleteLater);
	switchThread->start();
}

int main(int argc, char* argv[])
{
	// explicitly using the relative name of the current working directory
	const QDir dir(".");
	const std::string configFilename("client_config.yaml");
	const std::string configAbsPath = dir.absoluteFilePath(configFilename.c_str()).toStdString();

	if (!dir.exists(QString::fromStdString(configFilename)))
	{
		std::cout << "Config file " << configAbsPath << " doesn't exist." << std::endl;
		return 1;
	}

	// Initial call so args get passed on
	GUI::GetInstance(argc, argv);

	// Connect signals and slots
	QObject::connect(ConfigManager::GetInstance(), &ConfigManager::ConfigLoaded, &UpdateUIFromConfig);
	QObject::connect(GUI::GetSwitchButton(), &QPushButton::clicked, &SwitchButtonOnClick);

	ConfigManager::LoadYamlConfig(configAbsPath);
	
	return QApplication::exec();
}
