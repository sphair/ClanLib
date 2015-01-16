
#pragma once

class XMLSettings;
class NetworkListView;

class ConnectionsViewController : public clan::ViewController
{
public:
	ConnectionsViewController();

private:
	void add_clicked();
	void connect_clicked(XMLSettings connection);
	void edit_clicked(XMLSettings connection);
	void remove_clicked(XMLSettings connection);

	std::shared_ptr<NetworkListView> networks;
	clan::SlotContainer slots;
};
