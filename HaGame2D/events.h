#pragma once
#include <functional>
#include <vector>
#include <unordered_map>

template <class T>
using EventHandler = std::function<void(T)>;

template <class T>
struct EventListener {
	long id;
	EventHandler<T> handler;
};

template <class T>
struct EventChannel {
private:
	std::vector<EventListener<T>> listeners;

public:

	EventChannel() {
		listeners = std::vector<EventListener<T>>();
	}

	EventListener<T> subscribe(EventHandler<T> handler) {
		EventListener<T> listener;
		listener.id = listeners.size();
		listener.handler = handler;

		listeners.push_back(listener);

		return listener;
	}

	void trigger(T payload) {
		for (auto listener : listeners) {
			listener.handler(payload);
		}
	}

	void unsubscribeAll() {
		listeners.clear();
	}

	void unsubscribe(long id) {
		listeners.erase(id);
	}

	void unsubscribe(EventListener<T> listener) {
		unsubscribe(listener.id);
	}
};

template <class T>
class EventManager {
private:
	std::unordered_map<std::string, EventChannel<T>> channels;

	void checkChannel(std::string channel) {
		if (channels.find(channel) == channels.end()) {
			channels[channel] = EventChannel<T>();
		}
	}

public:

	EventManager() {
		channels = std::unordered_map<std::string, EventChannel<T>>();
	}

	EventListener<T> on(std::string channel, EventHandler<T> handler) {
		checkChannel(channel);
		return channels[channel].subscribe(handler);
	}

	void emit(std::string channel, T payload) {
		checkChannel(channel);
		channels[channel].trigger(payload);
	}
};