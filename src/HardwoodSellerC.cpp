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

	WoodItem WI_cherry("Cherry", 2.5, 6.00);
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
	int num_orders = 0;

	for (int i = 0; i < 6; ++i) 
	{
		delivery_times[i] = 0;
		delivery_amounts[i] = 0;
		totalcost[i] = 0;
	}

	cout << "Please enter file for processing: "; cin >> file_string; cout << endl;
	file_buffer = readInputFile(file_string);
	//parse out the basic customer info from the text with cstr holders
	char customer_name[50];
	char customer_address[75];
	char customer_date[50];
	size_t custinfo = 0;
	//gather values
	sscanf(file_buffer.c_str(), "%[^;];%[^;];%[^\n]\n", customer_name, customer_address, customer_date);
	//convert from cstr to string and copy values
	string nm(customer_name);
	customer = nm;
	string addr(customer_address);
	address = addr;
	string dte(customer_date);
	date = dte;
	custinfo = customer.length() + address.length() + date.length() + 3;
	// set buffer to order info now that we gathered customer info
	file_buffer = file_buffer.substr(custinfo, string::npos - custinfo);
	// customer info parsing complete

    cout << "Name: " << customer << endl << "Address: " << address << endl
         << "Date Order Was Created: " << date << endl
         << "Items Ordered:  Wood \t     Cost" << endl
         << "****************************************" << endl;
    bool cont = 1;
    int num_requests = 0;
    int num_wood_ordered = 0;
    while(cont)
    {
    cont = seperate_values(delivery_amounts[num_requests], file_buffer, woodorder[num_requests]);
    cout << "\t\t" << woodorder[num_requests] << "\t";
 for(int i = 0; i < 6; ++i)
 {
    if(WI_list[i].type == woodorder[num_requests])
    {
    cout << "\t" << WI_list[i].price;
    totalcost[num_wood_ordered] = WI_list[i].price * delivery_amounts[num_requests];
    delivery_times[num_wood_ordered] = deliveryTime(WI_list[i].baseDeliveryTime, delivery_amounts[num_requests]);
    num_wood_ordered++;
    break;
    }
 }	// price calculations
    cout << "*" << delivery_amounts[num_requests] << "= $" << totalcost[num_wood_ordered-1];
    cout << endl << endl;
    num_requests++;
    }
    // sort for greatest delivery time
    for (int i = 0; i < 6; ++i) if(delivery_times[i] > delivery_time) delivery_time = delivery_times[i];
    // sum total cost
    for (int i = 0; i < 6; ++i) cost_sum += totalcost[i];
	cout << "Est. Delivery Time:\t" << delivery_time << " hours\t" << "Total Cost: $" << cost_sum << endl;
	cout << "Order Processing Complete! Have a nice day!" << endl;
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
	streampos length;
	holder.seekg(0,holder.end);
	length = holder.tellg();
	holder.seekg(0, holder.beg);
	char * buffer = new char[length];
	holder.read(buffer, length);
	string save(buffer);
	delete [] buffer;
	holder.close();
	return save;
}

int seperate_values(double &delivery_amounts, string &file_buffer, string &woodorder)
{
    char * wood_label = new char[30];
    // used to skip customer info...
    size_t custinfo;
    //pick up desired values about wood order... 
    sscanf(file_buffer.c_str(), "%[^:]:%lf", wood_label, &delivery_amounts);
    woodorder.assign(wood_label);
    custinfo = file_buffer.find_first_of(";");
    file_buffer = file_buffer.substr(custinfo+1, string::npos - custinfo);
    // check if end of orders
    if(file_buffer.length() <= 2)
        return 0;
	return 1;
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
