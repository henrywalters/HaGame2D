#include "stdafx.h"
#include "Inventory.h"
#include <iostream>

Inventory::Inventory()
{
}


Inventory::~Inventory()
{
}

bool Inventory::inventoryItemExists(std::string itemName)
{
	bool exists = (inventory.find(itemName) != inventory.end());
	return exists;
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
		std::cout << "initializing item: " + item.toString() + "\n";
		inventory[item.name] = item;
	}
}

void Inventory::addToInventory(std::string itemName, int quantity)
{
	if (inventoryItemExists(itemName)) {
		inventory[itemName].quantity += quantity;
	}
	else {
		addItem(InventoryItem{ itemName, quantity });
	}
}

void Inventory::takeFromInventory(std::string itemName, int quantity)
{
	if (inventoryItemExists(itemName)) {
		inventory[itemName].quantity -= quantity;
	}
}
