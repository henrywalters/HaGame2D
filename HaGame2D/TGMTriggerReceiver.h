#pragma once
#include "HaGame2D.h"
#include "events.h"

namespace TGM {

	// A Trigger-Receiver message
	struct TRMessage {
		long triggerId;
		int value;
	};

	class Receiver : public Component {
	public:
		EventHandler<int> handler;

		Receiver() {
			handler = [](int val) {};
		}

		void onReceive(EventHandler<int> _handler) {
			handler = _handler;
		}
	};

	class Trigger : public Component {
	public:
		long id;
		std::vector<Receiver*> receivers;
		int value;

		Trigger() {
			receivers = std::vector<Receiver*>();
		}

		Trigger(Receiver* receiver) {
			receivers = std::vector<Receiver*>();
			receivers.push_back(receiver);
		}

		Trigger(std::vector<Receiver*> _receivers) {
			receivers = _receivers;
		}

		void onInit() {
			id = transform->uid;
		}
	};

	class TriggerReceiverSystem : public System {
	public:
		std::vector<GameObject *> triggers;

		TriggerReceiverSystem() : System("Trigger_Receiver_System") {}

		void onInit() {
			refresh();
		}

		void refresh() {
			triggers = getScene()->getGameObjectsWhere<Trigger>();
		}

		void trigger(long id, int value) {
			for (auto trigger : triggers) {
				if (trigger->uid == id) {
					auto triggerComponent = trigger->getComponent<Trigger>();
					triggerComponent->value = value;
					for (auto receiver : triggerComponent->receivers) {
						receiver->handler(value);
					}
				}
			}
		}
	};
}