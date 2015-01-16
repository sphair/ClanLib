
#pragma once

class NetworkListView;

class ConnectionsViewController : public clan::ViewController
{
public:
	ConnectionsViewController();

private:
	std::shared_ptr<NetworkListView> networks;
	clan::SlotContainer slots;
};
