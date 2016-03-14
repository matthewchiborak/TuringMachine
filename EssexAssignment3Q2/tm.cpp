#include <iostream>
#include <vector>
#include <string>

class Transition {
public:
	char inputSymbol;
	std::string nextState;
	char writeSymbol;
	char moveDirection;

	Transition(char i, std::string n, char w, char m)
	{
		inputSymbol = i;
		nextState = n;
		writeSymbol = w;
		moveDirection = m;
	}
};

class State {
private:
	std::vector<Transition*> stateTransitions;
	std::string id;
	bool isFinal;

public:
	State(std::string ident)
	{
		id = ident;
		isFinal = false;
		std::cout << "State " << id << " added\n";
	}

	~State()
	{
		for (int i = 0; i < stateTransitions.size(); i++)
		{
			delete stateTransitions.at(i);
		}
	}

	std::string getId()
	{
		return id;
	}

	void makeFinal()
	{
		isFinal = true;
	}

	bool getStateType()
	{
		return isFinal;
	}

	void addTransition(char inputSym, std::string nextState, char writeSymbol, char moveDir)
	{
		//Create the transition
		stateTransitions.push_back(new Transition(inputSym, nextState, writeSymbol, moveDir));
		std::cout << "Transition "<< inputSym << nextState << writeSymbol << moveDir << " added to state " << id << "\n";
	}
	
	//Returns the next state
	std::string getTransitionInfo(char inputSym, char* writeSym, char* moveDir)
	{
		for (int i = 0; i < stateTransitions.size(); i++)
		{
			if (inputSym == stateTransitions.at(i)->inputSymbol)
			{
				*writeSym = stateTransitions.at(i)->writeSymbol;
				*moveDir = stateTransitions.at(i)->moveDirection;
				return  stateTransitions.at(i)->nextState;
			}
		}

		return "none";
	}
};

int main()
{
	std::vector<State*> myStates;

	//Create the starting state
	State* startState = new State("0");
	myStates.push_back(startState);

	//Pointer to the current state of the machine
	State* focusState = startState;

	std::string userInput = "a";

	std::cout << "t - transition, f - final state, i - input, type 'end' to close and '_' is used for empty\n";

	//Get the number of lines specifying the behaviour of the Turing machine until the users wants to end
	while (userInput != "end")
	{
		std::cout << ">>";
		std::getline(std::cin, userInput);

		if (userInput == "")
		{
			std::cout << "INVALID INPUT\n";
		}
		else if (userInput.at(0) == 't')
		{
			std::string currentState = "";
			char inputSym;
			std::string nextState = "";
			char writeSymbol;
			char moveDir;

			int part = 0;

			//Parse the input
			//Ignore the t and the first space
			for (int i = 2; i < userInput.length(); i++)
			{
				if (userInput.at(i) == ' ')
				{
					part++;
				}
				else if (part == 0)
				{
					currentState += userInput.at(i);
				}
				else if (part == 1)
				{
					inputSym = userInput.at(i);
				}
				else if (part == 2)
				{
					nextState += userInput.at(i);
				}
				else if (part == 3)
				{
					writeSymbol = userInput.at(i);
				}
				else if (part == 4)
				{
					moveDir = userInput.at(i);
					break;
				}
			}

			//Find the wanted state and add the transition
			for (int i = 0; i < myStates.size(); i++)
			{
				if (currentState == myStates.at(i)->getId())
				{
					myStates.at(i)->addTransition(inputSym, nextState, writeSymbol, moveDir);
					break;
				}

				//Not found so create the state
				if ((i+1) == myStates.size())
				{
					myStates.push_back(new State(currentState));
					break;
				}
			}

			//Create the destination state if it doesn't exist
			for (int i = 0; i < myStates.size(); i++)
			{
				if (nextState == myStates.at(i)->getId())
				{
					break;
				}

				//Not found so create the state
				if ((i + 1) == myStates.size())
				{
					myStates.push_back(new State(nextState));
					break;
				}
			}
		}
		else if(userInput.at(0) == 'f')
		{
			std::string stateName = "";

			//Make all the given states final
			for (int i = 2; i < userInput.length(); i++)
			{
				if (userInput.at(i) == ' ')
				{
					for (int j = 0; j < myStates.size(); j++)
					{
						if (myStates.at(j)->getId() == stateName)
						{
							myStates.at(j)->makeFinal();
							std::cout << stateName << " became final state\n";
							stateName = "";
							break;
						}
					}

					stateName = "";
				}
				else
				{
					stateName += userInput.at(i);
				}
			}

			//Add the last final state
			for (int j = 0; j < myStates.size(); j++)
			{
				if (myStates.at(j)->getId() == stateName)
				{
					myStates.at(j)->makeFinal();
					std::cout << stateName << " became final state\n";
					stateName = "";
					break;
				}
			}

		}
		else if (userInput.at(0) == 'i')
		{
			//Reset the machine
			focusState = startState;
			int location = 2;
			userInput = userInput + '_';
			userInput.at(1) = '_';
			char writeSymbol = '_';
			char moveDirection = 'R';
			std::string wantedState = "";

			while (true)
			{
				//Get the new state that is moved to based on the input
				wantedState = focusState->getTransitionInfo(userInput.at(location), &writeSymbol, &moveDirection);

				//If no transition found reject
				if (wantedState == "none")
				{
					std::cout << userInput << "\nREJECTED\n";
					break;
				}

				//Move the head
				userInput.at(location) = writeSymbol;
				if (moveDirection == 'R')
				{
					location++;
				}
				if (moveDirection == 'L')
				{
					location--;
				}

				

				//Change the focus state
				for (int i = 0; i < myStates.size(); i++)
				{
					if (wantedState == myStates.at(i)->getId())
					{
						focusState = myStates.at(i);
					}
				}

				//Check if the machine is in an accept state
				if (focusState->getStateType())
				{
					std::cout << userInput << "\nACCEPT\n";
					break;
				}
			}
		}
		else if(userInput == "end")
		{
			std::cout << "GOODBYE!\n";
		}
		else
		{
			std::cout << "INVALID INPUT\n";
		}
	}
	
	//Free up the memory used to create the states
	for (int i = 0; i < myStates.size(); i++)
	{
		delete myStates.at(i);
	}


	return 0;
}