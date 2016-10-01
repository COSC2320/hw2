#include <string>
#include <iostream>
#include "emailType.h"

using namespace std;

class emailList
{
public:
	emailList();
	~emailList();
	void setFilename(std::string name);
	int length() const;
	void add(std::string name, std::string subject, std::string message);
	void print();
	void eliminateDuplicates();
	void deleteDuplicates(std::string name, std::string subject, std::string message, emailType * address);
	void eliminateCancelledReservations();
	void deleteCancelledReservations(std::string name, std::string subject, std::string message, emailType * address);
	void eliminatePurchaseOrderWithCancellation();
	void deletePurchaseOrderwithCancellation(std::string name, std::string subject, std::string message, emailType * address);
	void destroyList();
	void readFileContents();

private:
	std::string filename;
	emailType * first;
	emailType * last = NULL;
	int count;
};

emailList::emailList()
{
	first = NULL;
	last = NULL;
	count = 0;
}

emailList::~emailList()
{
}

void emailList::setFilename(std::string name)
{
	filename = name;
}

int emailList::length() const
{
	return count;
}


void emailList::add(std::string name, std::string subject, std::string message)
{
	if (NULL == first)
	{
		first = new emailType;
		first->name = name;
		first->subject = subject;
		first->message = message;
		first->link = NULL;
		first->previous = NULL;
		last = first;
	}
	else //adding node to the end of the list
	{
		last->link = new emailType;
		last->link->name = name;
		last->link->subject = subject;
		last->link->message = message;
		last->link->link = NULL;
		last->link->previous = last;
		last = last->link;
		count++;
	}
}

void emailList::print()
{
	emailType * current = first;
	emailType * end = last;
	while (current != NULL)
	{
		std::cout << current->name << "\t" << current->subject << "\t" << current->message << std::endl;
		current = current->link;
	}
}



void emailList::eliminateDuplicates()
{
	emailType * current = first;
	while (current != NULL)
	{
		std::string name = current->name;
		std::string subject = current->subject;
		std::string message = current->message;
		deleteDuplicates(name, subject, message, current);
		current = current->link;
	} //end while
	//std::cout << "Count after duplicates deleted: " << count << std::endl;
}

void emailList::deleteDuplicates(std::string name, std::string subject, std::string message, emailType * address)
{
	emailType * current = address; //points current to 1st node
	emailType * temp;
	while (current != NULL) //break when current is NULL
	{
		if (current->link) //if there is another node
		{
			if (name == current->link->name && subject == current->link->subject && message == current->link->message) //if all 3 fields match to the 2nd node
			{
				temp = current->link; //points temp to 2nd node
				current->link = current->link->link; //moves current to 3rd node
				delete temp; // deletes temp
				count--;
			}
			else
			{
				current = current->link; // points current to 2nd node
			}
		}
		else //there was only one node
		{
			current = NULL; //current is now NULL
		}
	}//end while
}

void emailList::eliminateCancelledReservations()
{
	emailType * current = first;
	while (current != NULL)
	{
		std::string name = current->name;
		std::string subject = current->subject;
		std::string message = current->message;
		deleteCancelledReservations(name, subject, message, current);
		current = current->link; //moves current to the next node
	}
	//std::cout << "Count after cancelled reservations deleted: " << count << std::endl;
}

void emailList::deleteCancelledReservations(std::string name, std::string subject, std::string message, emailType * address)
{
	emailType * current = address;
	emailType * trailCurrent;
	emailType * trailTemp;
	emailType * temp;
	bool found;
	if (current->subject == "Approve-reservation") //Case 2
	{
		found = false;
		trailCurrent = current;
		trailTemp = current;
		temp = current->link;
		current = current->link;
		while (current != NULL && !found)
		{
			if (current->subject == "Approve-reservation(Cancelled)" && trailCurrent->name == current->name && trailCurrent->message == current->message)
			{
				found = true;
			}
			else
			{
				trailCurrent = current;
				trailTemp = current;
				temp = current->link;
				current = current->link;
			}
		}
		if (found)
		{
			trailCurrent->link = current->link;
			trailTemp->link = current->link;
			count--;
			if (last == current) //node to be deleted was the last node
			{
				last = trailCurrent; //update the value of last
			}
			delete temp;
		}
	}//end if
}

void emailList::eliminatePurchaseOrderWithCancellation()
{
	emailType * current = first;
	while (current != NULL)
	{
		std::string name = current->name;
		std::string subject = current->subject;
		std::string message = current->message;
		deletePurchaseOrderwithCancellation(name, subject, message, current);
		current = current->link; //moves current to the next node
	}
	//std::cout << "Count after revised purchase orders deleted: " << count << std::endl;
}

void emailList::deletePurchaseOrderwithCancellation(std::string name, std::string subject, std::string message, emailType * address)
{
	emailType * trailCurrent;
	emailType * current = address;
	emailType * trailTemp1;
	emailType * temp1;
	emailType * trailTemp2;
	emailType * temp2;
	emailType * trailTemp3;
	emailType * temp3;
	while (current != NULL)
	{
		if (current->link)
		{
			trailTemp1 = current;
			temp1 = current->link;
			trailCurrent = current;
			current = current->link;
			if (current->subject == "Approve-purchase-order")
			{
				trailTemp1 = trailCurrent;
				temp1 = current;
				trailTemp2 = trailCurrent;
				temp2 = current;
				while (temp2 != NULL)
				{
					if (temp2->link)
					{
						trailTemp2 = temp2;
						temp2 = temp2->link;
						if (temp2->subject == "Purchase-order-cancellation" && temp2->message == current->message)
						{
							trailTemp3 = trailCurrent;
							temp3 = current;
							while (temp3 != NULL)
							{
								if (temp3->link && !(temp3 == temp2))
								{
									trailTemp3 = temp3;
									temp3 = temp3->link;
									if (temp3->subject == "Purchase-order-revise" && temp3->message == current->message)
									{
										trailCurrent->link = temp2;
										//delete temp1;
										//delete temp3;
									}
									else
									{
										trailCurrent->link = temp2;
										//delete temp1;
									}
								}
								else
								{
									temp3 = NULL;
								}
							}
						}
					}
					else
					{
						temp2 = NULL;
					}
				}
			}
			else
			{

			}
		}
		else
		{
			current = NULL;
		}
	}
}

void emailList::destroyList()
{
	emailType * temp;
	while (first != NULL)
	{
		temp = first;
		first = first->link;
		delete temp;
	}
	last = NULL;
	count = 0;
}

void emailList::readFileContents()
{
	std::ifstream olaf(filename);
	std::string str;
	std::vector<std::string> tokens;
	emailType *t;
	t = new emailType;
	std::string name;
	std::string subject;
	std::string message;

	while (getline(olaf, str))
	{
		if (str.find("#") == 0 || str.length() == 0)
		{
			// Skip this line when it starts with # or the line is empty
		}
		else
		{
			std::vector<std::string> tokens;

			for (auto i = strtok(&str[0], "\t\n\r"); i != nullptr; i = strtok(nullptr, "\t\n\r"))
				tokens.push_back(i);

			for (int unsigned i = 0; i < tokens.size(); i += 3)
			{
				name = tokens[i];
				subject = tokens[i + 1];
				message = tokens[i + 2];
				add(name, subject, message);
			}
		} //end else
	} //end while
	olaf.close();
}