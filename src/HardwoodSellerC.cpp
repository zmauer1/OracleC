//============================================================================
// Name        : HardwoodSellerC.cpp
// Author      : Esteban Parra
// Version     : 1
// Copyright   : It's free
// Description : Main class for the Hardwood Seller
//============================================================================

#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include "WoodItem.cpp"
using namespace std;

string readInputFile(string);
int seperate_values(double&, string&, string&);
double deliveryTime(double, double);

int main() {

	// WoodObject(type, baseDeliveryTime, itemPrice);
	WoodItem WI_cherry("Cherry", 2.5, 6.00);					//Hardcode all my values into the program...
	WoodItem WI_maple("Curly Maple", 1.5, 6.00);
	WoodItem WI_mahogany("Genuine Mahogany", 3, 9.60);
	WoodItem WI_wenge("Wenge", 5, 22.35);
	WoodItem WI_oak("White Oak", 2.3, 6.70);
	WoodItem WI_sawdust("Sawdust", 1, 1.5);

	WoodItem WI_list[6];
	WI_list[0] = WI_cherry;
	WI_list[1] = WI_maple;
	WI_list[2] = WI_mahogany;
	WI_list[3] = WI_wenge;
	WI_list[4] = WI_oak;
	WI_list[5] = WI_sawdust; 

	string customer;
	string address;
	string date;
	string file_string;
	string file_buffer;
	string woodorder[6];

	double delivery_amounts[6];
	double cost_sum = 0.0;
	double delivery_time = 0.0;
	double delivery_times[6];
	double totalcost[6];
	int custinfo;

	for (int i = 0; i < 6; ++i) 
	{
		delivery_times[i] = 0;
		delivery_amounts[i] = 0;
		totalcost[i] = 0;
	}

	cout << "Please enter file for processing: "; cin >> file_string; cout << endl;
	file_buffer = readInputFile(file_string);


	char cust_hold[50];
	char addr_hold[75];
	char date_hold[50];

	//parse out and gather customer info with c_str conversion method with sscanf...
	sscanf(file_buffer.c_str(), "%[^;];%[^;];%[^\n]\n", cust_hold, addr_hold, date_hold);

	string scust_hold(cust_hold);
	customer = scust_hold;
	string saddr_hold(addr_hold);
	address = saddr_hold;
	string sdate_hold(date_hold);
	date = sdate_hold;


	custinfo = customer.length() + address.length() + date.length() + 3;


	// set buffer to order info now that we gathered customer info...
	file_buffer = file_buffer.substr(custinfo, string::npos - custinfo);
	//display back customer info...
    cout << "Name: " << customer << endl << "Address: " << address << endl
         << "Date Order Was Created: " << date << endl
         << "Order:\t  Wood \t     Cost" << endl
         << "****************************************" << endl;



    int incr = 0;
    int incr2 = 0;
    bool cont = 1;
    while(cont)
    {
 
    char * wood_requested = new char[30];													//make holder for requested wood...
    int wood_coupling;																		//once wood coupling is read, skip it for the next coupling...			 
    sscanf(file_buffer.c_str(), "%[^:]:%lf", wood_requested, &delivery_amounts[incr]);		//pick up desired values about wood order...
    woodorder[incr].assign(wood_requested);													//store the string name of wood requested in the wood order...
    wood_coupling = file_buffer.find_first_of(";");											//identify the end of grouping...
    file_buffer = file_buffer.substr(wood_coupling+1, string::npos - wood_coupling);		//jump to wood couplings past customer info...
    if(file_buffer.length() <= 2) cont = 0;													//check if at the end of the orders listed on the input file and mark last while iteration...

    cout << "\t" << woodorder[incr] << "\t";														//display the wood that read in by the request back to the customer...

		 for(int i = 0; i < 6; ++i)
		 {
		    if(WI_list[i].type == woodorder[incr])														//if this wood is ordered perfor the following...
		    {
		    cout << "   " << WI_list[i].price;															//read out that specific wood's price...
		    totalcost[incr2] = WI_list[i].price * delivery_amounts[incr];								//caculate the total cost of this wood's order amount...
		    delivery_times[incr2] = deliveryTime(WI_list[i].baseDeliveryTime, delivery_amounts[incr]);	//caculate to what multiplier this wood will undergoe depending on amount...
		    incr2++;																					//incr2++ for following cout...
		    break;
		    }
		 }

    cout << "*" << delivery_amounts[incr] << "= $" << totalcost[incr2-1];					//show the user the equation that the wood order forgone to calculate the total...
    cout << endl << endl;																	//format...
    incr++;																					//incr++ to next iteration of wood coupling in order...
    }


    for (int i = 0; i < 6; ++i)																//sort for greatest delivery time...
    	if(delivery_times[i] > delivery_time) 
     		delivery_time = delivery_times[i];
    
    for (int i = 0; i < 6; ++i)																//sum total cost...
    	cost_sum += totalcost[i];

	cout << "Est. Delivery Time:\t" << delivery_time << " hours\t" << endl							//display calculations back to customer... 
		 << "Total Cost: $" << cost_sum << endl;
	cout << "Order Processing Complete! Have a nice day!" << endl << endl;


	return 0;
}

/*
 * Method to read the input file
 */
string readInputFile(string inputFilePath) 
{
	ifstream holder;
	holder.open(inputFilePath.c_str());
	if (!holder)
	{
		cout << "input file cannot be opened! (fatal error)" << endl;
		exit(1);
	}
	streampos length;					//beginning of file...
	holder.seekg(0,holder.end);			//get me to the end of file...
	length = holder.tellg();			//tell me how far I went...
	holder.seekg(0, holder.beg);		//set me back to the beginning...
	char * buffer = new char[length];	//create my buffer with the found size...
	holder.read(buffer, length);		//read my file into the buffer...
	string saved_buffer(buffer);		//convert my c_str buffer into a string...
	delete [] buffer;					//clear the memory...
	holder.close();						//close out the file...
	return saved_buffer;				//return saved buffer...
}
/*
 * Method to compute the deliveryTime
 */
double deliveryTime(double base,double amount) 
{
	double deliveryETA = 0.0;
	if(amount == 0) deliveryETA = 0.0;
	else if((amount >= 1) && (amount < 101)) deliveryETA = 1 * base;
	else if((amount >= 101) && (amount < 201)) deliveryETA = 2 * base;
	else if((amount >= 201) && (amount < 301)) deliveryETA = 3 * base;
	else if((amount >= 301) && (amount < 401)) deliveryETA = 4 * base;
	else if((amount >= 401) && (amount < 501)) deliveryETA = 5 * base;
	else if((amount >= 501) && (amount < 1000)) deliveryETA = 5.5 * base;
	return deliveryETA;
}

