#include "stdafx.h"
#include "Inventory.h"


Inventory::Inventory()
{
}


Inventory::~Inventory()
{
}

bool Inventory::inventoryItemExists(std::string itemName)
{
	return (inventory.find(itemName) == inventory.end());
}

int Inventory::getQuantity(std::string itemName)
{
	return (inventoryItemExists(itemName)) ? inventory[itemName].quantity : 0;
}

InventoryItem Inventory::getItem(std::string itemName)
{
	return (inventoryItemExists(itemName)) ? inventory[itemName] : InventoryItem {itemName, 0};
}

void Inventory::addItem(InventoryItem item)
{
	if (inventoryItemExists(item.name)) {
		inventory[item.name].quantity += item.quantity;
	}
	else {
		inventory[item.name] = item;
	}
}

void Inventory::addToInventory(std::string itemName, int quantity)
{
	if (inventoryItemExists(itemName)) {
		inventory[itemName].quantity += quantity;
	}
}

void Inventory::takeFromInventory(std::string itemName, int quantity)
{
	if (inventoryItemExists(itemName)) {
		inventory[itemName].quantity -= quantity;
	}
}
