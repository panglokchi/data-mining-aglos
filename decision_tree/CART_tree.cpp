#include <deque>
#include <iostream>
#include <utility>
#include <cmath>
#include <vector>
#include <string>
#include <iomanip>
#include <cstring>
#include <fstream>

using namespace std;

int main() {


    //std::cout << std::fixed;
    //std::cout << std::setprecision(5);

    fstream file;
    file.open("dataset.txt");

    int num_records;
    int num_attrs;

    int attr_values[12] = {0};
    int records[100][12];
    string attr_names[12];

    double stopping_condition_percentage;
    int stopping_condition_amount;

    file>>num_records>>num_attrs;

    for(int i = 0 ; i < num_attrs ; i++ ) {
        file>>attr_values[i];
    }

    file>>stopping_condition_percentage;
    stopping_condition_percentage = stopping_condition_percentage/double(100);

    file>>stopping_condition_amount;

    for(int j = 0 ; j < num_attrs; j++) {
        file>>attr_names[j];
    }

    for(int i = 0 ; i < num_records ; i++ ) {
        for(int j = 0 ; j < num_attrs; j++) {
            file>>records[i][j];
        }
    }

    int tgt = num_attrs - 1;

    do{
        double infoT = 1 ;
        
        int tdist[4] = { 0 };
        for(int i = 0 ; i < num_records ; i++ ) {
            for(int j = 0 ; j < attr_values[tgt] ; j++)
                if ( records[i][tgt] == j )
                    tdist[j]++;
        }

        cout<<"Info(T) = 1 ";


        for(int i = 0 ; i < attr_values[tgt] ; i++ ) {
            infoT -= (double(tdist[i])/num_records)*(double(tdist[i])/num_records);
            cout<<" - ("<<tdist[i]<<"/"<<num_records<<")^2";
        }
        
        cout<<" = "<<infoT<<"\n\n";

        double gain[12] = {0};

        for(int attr = 0 ; attr < num_attrs - 1 ; attr++ ) {

            cout<<"For attribute "<<attr_names[attr]<<": \n";
            int self_dist[4] = { 0 };
            int dist[4][4] = { 0 };
            for(int i = 0 ; i < num_records ; i++ ) {
                for(int j = 0 ; j < attr_values[attr] ; j++) {
                    for( int k = 0 ; k < attr_values[tgt] ; k++)
                        if ( records[i][attr] == j && records[i][tgt] == k )
                            dist[j][k]++;
                    if ( records[i][attr] == j ) {
                        self_dist[j]++;
                    }
                }
            }

            double info[4] = { 1, 1, 1, 1 };

            for(int i = 0 ; i < attr_values[attr] ; i++ ) {
                //double info[4] = { 0 };
                cout<<"Info(T_"<<i<<") = 1";
                for(int j = 0 ; j < attr_values[tgt] ; j++ ) {
                    if ( dist[i][j] != 0 )
                        info[i] -= (double(dist[i][j])/self_dist[i])*(double(dist[i][j])/self_dist[i]);                    
                    cout<<" - ("<<dist[i][j]<<"/"<<self_dist[i]<<")^2";
                }
                cout<<" = "<<info[i]<<"\n";
                
            }
            
            double total_info = 0;

            cout<<"Info("<<attr_names[attr]<<", T"<<") = ";

            for(int i = 0 ; i < attr_values[attr] ; i++ ) {
                total_info += (double(self_dist[i])/num_records)*info[i];                    
                cout<<"+("<<self_dist[i]<<"/"<<num_records<<")*Info(T_"<<i<<")";  
            }

            cout<<" = ";
            
            for(int i = 0 ; i < attr_values[attr] ; i++ ) {
                cout<<"+("<<self_dist[i]<<"/"<<num_records<<")*"<<info[i]<<"";    
            } 

            cout<<" = "<<total_info<<"\n";

            cout<<"Gain("<<attr_names[attr]<<", T"<<") = Info(T) - Info("<<attr_names[attr]<<" ,T) = ";

            cout<<infoT<<" - "<<total_info;

            gain[attr] = infoT-total_info;
           
            cout<<" = "<<gain[attr];

            cout<<"\n\n";
        }

    } while( false );
}