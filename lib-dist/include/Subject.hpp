#pragma once
/*
*	   Program: Subject.hpp
*	    Author: Tongil Kim
*	      Date: Apr 19, 2019
*/
#include <Observer.hpp>
#include <set>

class Subject
{

private:
	std::set<Observer*> obSet;
public:
    void Attach(Observer* o) 
	{
		obSet.insert(o);
	}
	void Detach(Observer* o)
	{
		obSet.erase(o);
	}
	void Notify()
	{
		for (auto o : obSet)
		{
			o->update();
		}
	}
};