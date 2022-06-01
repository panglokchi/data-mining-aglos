#include <deque>
#include <iostream>
#include <utility>
#include <cmath>
#include <vector>
#include <string>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <set>
#include <vector>

using namespace std;

double dist(pair<double,double> a, pair<double,double> b) {
    return sqrt((a.first-b.first)*(a.first-b.first)+(a.second-b.second)*(a.second-b.second));
}

struct Point {
    string name;
    pair<double,double> coords;
    Point(string name, pair<double,double> coords ): name(name), coords(coords) {};
    bool operator<(const Point rhs) const {
        return this->name<rhs.name;
    }
    bool operator==(const Point rhs) const {
        return this->name==rhs.name;
    }
};

pair<double,double> midpoint ( pair<double,double> a, pair<double,double> b ) {
    return pair<double,double>((a.first+b.first)/2,(a.second+b.second)/2);
}

int main() {

    vector<Point> data;

    /*
    data.push_back( Point("x1", pair<double,double>(4,5) ) );
    data.push_back( Point("x2", pair<double,double>(5,3) ) );
    data.push_back( Point("x3", pair<double,double>(-7,-2) ) );
    data.push_back( Point("x4", pair<double,double>(-2,1) ) );
    data.push_back( Point("x5", pair<double,double>(13,15) ) );
    data.push_back( Point("x6", pair<double,double>(11,9) ) );
    data.push_back( Point("x7", pair<double,double>(-5,-3) ) );
    data.push_back( Point("x8", pair<double,double>(5,4) ) );
    */

   fstream file;

   file.open("input_list.txt");

   int num_data = 0;

   file>>num_data;

   for ( int i = 0 ; i < num_data ; i++ ) {
       int x, y;
       string name;
       file>>name>>x>>y;
       data.push_back( Point(name, pair<double,double>(x, y) ) );
   }

    //double num_data = data.size();
    double num_data_left = num_data;
    
    double table[50][50];

    std::cout << std::fixed;
    std::cout << std::setprecision(2);

    ofstream outfile;
    outfile.open("median_out.csv");



    while( num_data_left > 0 ) {

        int offset = 0;
        
        for(int i = 0; i < data.size() ; i++ ) {
                if ( data[i].name.length()/8 > offset )
                    offset = data[i].name.length()/8;
        }


        cout<<"\n";

    
        double min = 999;
        pair<double,double> to_combine;
        for(int i = 0 ; i < data.size()  ; i++ ) {

                if ( i == 0 ) {
                    for( int j = 0 ; j < offset + 1 ; j++ ) {
                        cout<<"\t";
                    }
                    outfile<<",";
                    for( int j = 0 ; j < data.size(); j++ ) {
                        cout<<data[j].name<<"\t";
                        outfile<<data[j].name<<",";
                    }
                    outfile<<"\n";
                    cout<<"\n";
                }

                for( int j = 0 ; data.size(); j++ ) {

                    if ( j == 0 ) {
                        cout<<data[i].name;
                        outfile<<data[i].name<<",";
                        for ( int p = 0 ; p < offset + 1 - int(data[i].name.length()/8) ; p++ )
                            cout<<"\t";
                    }

                    for( int k = 0 ; k < offset - int(data[j].name.length()/8) ; k++ ) {
                        //cout<<"\t";
                    }

                    table[i][j] = dist(data[i].coords,data[j].coords);
                    if ( table[i][j] < min && table[i][j] != 0 ) {
                        min = table[i][j];
                        to_combine = pair<double,double>(i,j);
                    }
                    cout<<setw(4)<<table[i][j];
                    outfile<<table[i][j]<<",";
                    
                    for( int j = 0 ; j < offset + 1; j++ ) {
                        cout<<"\t";
                    }
                    if (j>=i) break;
                }
                cout<<"\n";
                outfile<<"\n";
        }
        cout<<"\n";
        cout<<"min: "<<min<<"\t merge "<<data[to_combine.second].name<<" and "<<data[to_combine.first].name<<"\n\n";
        outfile<<"\nmin:"<<min<<",merge,"<<data[to_combine.second].name<<",and,"<<data[to_combine.first].name<<"\n\n";

        //update offset
        for(int i = 0; i < data.size() ; i++ ) {
                if ( data[i].name.length() /8 > offset )
                    offset = data[i].name.length()/8;
        }

        //print centers
        cout<<"Cluster";
        for ( int p = 0 ; p < offset + 1 ; p++ ) { cout<<"\t"; } cout<<"Center\n";
        outfile<<"\n"<<"Cluster"<<","<<"Center"<<"\n";;
        for(int i = 0; i < data.size() ; i++ ) {
            cout<<data[i].name;
            outfile<<data[i].name<<",";
            for ( int p = 0 ; p < offset + 1 - int(data[i].name.length()/8) ; p++ ) {
                cout<<"\t";
            }
            cout<<"("<<data[i].coords.first<<", "<<data[i].coords.second<<")\n";
            outfile<<data[i].coords.first<<","<<data[i].coords.second<<"\n";
        }

        cout<<"\n";

        data[to_combine.second].coords = midpoint(data[to_combine.second].coords,data[to_combine.first].coords);
        data[to_combine.second].name = data[to_combine.second].name + data[to_combine.first].name;

        data.erase(data.begin() + to_combine.first);

        //for(int i = 0; i < data.size() ; i++ ) {
        //    cout<<data[i].name<<"\t\t";
        //    cout<<data[i].coords.first<<"\t"<<data[i].coords.second<<"\n";
        //}

        cout<<"\n";
        num_data_left--;


    }
}