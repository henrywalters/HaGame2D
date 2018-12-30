#pragma once
#include <string>

const std::string DEFAULT_FAIL_MESSAGE = "Requirement was not met";
const std::string DEFAULT_PASS_MESSAGE = "";

class InteractionRequirement
{
protected:
	virtual bool evaluate() { return false; };

    std::string failMessage = DEFAULT_FAIL_MESSAGE;
	std::string passMessage = DEFAULT_PASS_MESSAGE;

public:
	InteractionRequirement();
	~InteractionRequirement();

	 bool requirementMet() {
		 return evaluate();
	};

	std::string getRequirementLog() {
		return requirementMet() ? passMessage : failMessage;
	}

	void setPassMessage(std::string msg) {
		passMessage = msg;
	}

	void setFailMessage(std::string msg) {
		failMessage = msg;
	}
};

