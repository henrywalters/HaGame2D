#pragma once

template <class C, class A>
class Strategy {
public:

	Strategy() {};
	~Strategy() {};

	//Returns the action that should be returned given that context
	virtual A evaluate(C context) = 0;
};