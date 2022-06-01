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

struct Cluster {
    vector<Point> points;
    Cluster( Point point ) {
        points.push_back(point);
    }
    void merge( Cluster rhs ) {
        for( auto it = rhs.points.begin() ; it != rhs.points.end() ; it++ ) {
            this->points.push_back(*it);
        }
    }
};

double getAverage(vector<double> i) {
    double sum = 0; double count = 0;
    for( auto it: i) {
        sum += it;
        count++;
    }
    return (sum/count);
}


int main() {

    vector<Point> data;

    vector<Cluster> clusters;

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

    file.open("matrix_in.txt");

   int num_data = 0;

   file>>num_data;

    int num_data_left = num_data;

    



    ofstream outfile;
    outfile.open("median_out.csv");




    for(int i = 0; i < num_data ; i++ ) {
        data.push_back(Point(string("x"+to_string(i)),pair<double,double>(0,0)));
        clusters.push_back(Cluster(data[i]));
    }
    
    vector<Point> old_data = data;

    std::cout << std::fixed;
    std::cout << std::setprecision(2);



    double** table = new double*[num_data];
    for(int i = 0 ; i < num_data; i++ ) table[i] = new double[num_data];

    for(int i = 0 ; i < num_data; i++ ) {
        for(int j = 0 ; j <= i; j++ ) {
            file>>table[i][j];
        }
    }

    double** old_table = new double*[num_data];
    for(int i = 0 ; i < num_data; i++ ) old_table[i] = new double[num_data];

    for(int i = 0 ; i < num_data; i++ ) {
        for(int j = 0 ; j < num_data; j++ ) {
            old_table[i][j] = table[i][j];
        }
    }

    for(int i = 0 ; i < num_data; i++ ) {
        for(int j = i+1 ; j < num_data ; j++ ) {
            table[i][j]=table[j][i];
        }
    }

    while( num_data_left > 0 ) {

        int offset = 0;
        
        for(int i = 0; i < data.size() ; i++ ) {
                if ( data[i].name.length()/8 > offset )
                    offset = data[i].name.length()/8;
        }


        cout<<"\n";

    
        double min = 999;
        pair<int,int> to_combine;
        for(int i = 0 ; i < data.size() ; i++ ) {

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

                    //table[i][j] = dist(data[i].coords,data[j].coords);
                    if ( table[i][j] < min && table[i][j] != 0 ) {
                        min = table[i][j];
                        to_combine = pair<int,int>(i,j);
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

        for(int i = 0 ; i < data.size() ; i++ ) {
            for(int j = 0 ; j < data.size() ; j++ ) {
                //to_combine = pair<double,double>(i,j);
                if( i != to_combine.first && i != to_combine.second ) {
                    if( j != to_combine.first && j != to_combine.second ) {
                        continue;
                    }
                    if( j == to_combine.first ) {
                        if( table[i][to_combine.second] > table[i][j] ) {
                            table[i][j] = table[i][to_combine.second];
                        }
                    }
                    if( j == to_combine.second ) {
                        if( table[i][to_combine.first] > table[i][j] ) {
                            table[i][j] = table[i][to_combine.first];
                        }
                    }
                }
                if( i == to_combine.first || i == to_combine.second ) {
                    if( j == to_combine.first || j == to_combine.second ) {
                        continue;
                    }
                    if( i == to_combine.first ) {
                        if( table[to_combine.second][j] > table[i][j] ) {
                            table[i][j] = table[to_combine.second][j];
                        }
                    }
                    if( i == to_combine.second ) {
                        if( table[to_combine.first][j] > table[i][j] ) {
                            table[i][j] = table[to_combine.first][j];
                        }
                    }
                }
            }
        }



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