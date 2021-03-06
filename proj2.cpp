#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vector>
using namespace std;

void print_matrix(vector<vector<int>> vect){
	for (int i=0;i<vect.size();i++){
		for (int j=0;j<vect[i].size();j++){
			cout<<vect[i][j]<<" ";
		}
		cout<<endl;
	}
}

//creates request matrix
vector<vector<int>> request_vector(vector<vector<int>> total_matrix, int i, int j){
	vector<vector<int>> vect;
	for (int x=0;x<i;x++){
		vector<int> temp;
		for (int y=i;y<total_matrix[x].size();y++){
			temp.push_back(total_matrix[x][y]);
		}
		vect.push_back(temp);
	}
	return vect;
}

//creates allocation matrix
vector<vector<int>> allocation_vector(vector<vector<int>> total_matrix, int i, int j)
{
	vector<vector<int>> vect;
	for (int x=i;x<total_matrix.size();x++){
		vector<int> temp;
		for (int y=0;y<i;y++){
			temp.push_back(total_matrix[x][y]);
		}
		vect.push_back(temp);
	}
	vector<vector<int>> vect1(vect[0].size(), vector<int>(vect.size()));
	for (int x=0; x<vect.size();++x){
		for (int y=0; y<vect[0].size(); ++y){
			vect1[y][x] = vect[x][y];
		}
	}
	return vect1;
}

//updates the request matrix if no deadlock is detected and reduces resources from allocation matrix
void change_request(vector<vector<int>> &request, vector<vector<int>> &allocation, int row){
	//cout<<row<<endl;
	for (int i=0;i<request.size();i++){
		for (int j=0;j<request[i].size();j++){
			if (request[i][j]==1 && allocation[row][j]==1){
				request[i][j]-=allocation[row][j];
				allocation[row][j]-=1;
				
			}
		}
	}
    /* DEBUG
	cout<<"In change request\n";
	cout<<"request\n";
	print_matrix(request);
	cout<<"---\n";
	cout<<"allocation\n";
	print_matrix(allocation);
    */

}

//checks for deadlock state
void check_deadlock(vector<vector<int>> &request, vector<vector<int>> &allocation)
{
	for (int i=0;i<request.size();i++){
		//row_sum represents the sum of an entire row
		//sum of zero indicates a process that has no outgoing requests
		int row_sum=0;
		for (int j=0;j<request[i].size();j++){
			row_sum+=request[i][j];
			//checks if at the end element of a row
			if (j==request[i].size()-1){
				//true==process has no outgoing requests==process reduction
				if (row_sum==0){
					change_request(request, allocation, i);
					//reduce row
					request.erase(request.begin() + i);
					allocation.erase(allocation.begin() + i);

                    /* DEBUG
			cout<<"In check_deadlock\n";
			print_matrix(request);
			cout<<"---\n";
			print_matrix(allocation);
                    */
					//start over
					check_deadlock(request, allocation);
				}
			}
		}
	}
}

int main()
{	int process=0;
	int resource=0;
	int Equal_count=0;	//Used to count process and resource number - is a temp variable
	int comma_count=0;
	vector <vector<int>> total_matrix;
	vector<int> avail_resource;
	vector <vector<int>> request;
	vector <vector<int>> allocation;
	//Getting input from user 
	string filename;
	cout<<"File name : ";
    //cin >> filename;
    getline(cin, filename);

    while(filename.empty())
    {
        cout<<"Please provide input file: \n";
        getline(cin, filename);
    }

	//Reading user file
	ifstream textfile;
	textfile.open(filename);
	if (textfile.is_open())
    {
		string line;
		while(getline(textfile, line))
		{
			if (line[0]=='%' || line==""){
				continue;
			}else if(line.find('=') != std::string::npos){
				string temp;
				int count=0;
				stringstream ss(line);
				while(getline(ss, temp, '='))
				{
					if (count>0){
						if (Equal_count==0)
						{
							stringstream foo(temp);
							foo >> process;
							// cout<<"Process : "<<process<<endl;
						}
						else
						{
							stringstream foo(temp);
							foo >> resource;
							// cout<<"Resources : "<<resource<<endl;
						}
					}
					count++;
				}
				Equal_count++;
			}else if(line.find(',') != std::string::npos){
				vector<int> v1;
				string temp;
				stringstream ss(line);
				while(getline(ss, temp, ','))
				{
					if (comma_count>0){
						int temp1;
						stringstream foo(temp);
						foo >> temp1;
						v1.push_back(temp1);
					}else{
						int temp1;
						stringstream foo(temp);
						foo >> temp1;
						avail_resource.push_back(temp1);
					}
				}
				if (comma_count>0){
					total_matrix.push_back(v1);
				}
				comma_count++;
			}
		}
	}
    else
    {
        cout<<"ERROR: Unable to locate " <<"'"<< filename<<"'"<< "\n";
        exit(0);
    }

	request=request_vector(total_matrix, process, resource);
	allocation=allocation_vector(total_matrix, process, resource);
	
	cout<<"request matrix\n";
	print_matrix(request);
	cout<<endl;
	cout<<"allocation matrix\n";
	print_matrix(allocation);
	cout<<endl;

	check_deadlock(request, allocation);
	print_matrix(request);

	if (request.empty()){
		cout<<"Since the vector could be completely reduced. NO DEADLOCK!!\n";
	}else{
		cout<<"Since the vector could not be completely reduced. YES DEADLOCK!!\n";
	}
	
	textfile.close();

}

