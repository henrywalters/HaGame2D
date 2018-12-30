#pragma once
#include <string>
#include <unordered_map>

struct InventoryItem {
	std::string name;
	int quantity;
};

class Inventory
{
	std::unordered_map<std::string, InventoryItem> inventory;
	std::vector<std::string> inventoryList;

public:
	Inventory();
	~Inventory();

	bool inventoryItemExists(std::string itemName);
	int getQuantity(std::string itemName);

	InventoryItem getItem(std::string itemName);
	void addItem(InventoryItem item);
	
	void addToInventory(std::string itemName, int quantity);
	void takeFromInventory(std::string itemName, int quantity);
};

