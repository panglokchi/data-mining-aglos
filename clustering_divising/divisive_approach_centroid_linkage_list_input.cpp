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

double dist(pair<double,double> a, pair<double,double> b) {
    return sqrt((a.first-b.first)*(a.first-b.first)+(a.second-b.second)*(a.second-b.second));
}

struct Node {
    double centroid;
    bool leaf = false;
    pair<double,double> value;
    Node* lhs = nullptr;
    Node* rhs = nullptr;
};

struct Point {
    string name;
    pair<double,double> coords;
    Point(string name, pair<double,double> coords ): name(name), coords(coords) {};
};

pair<double,double> midpoint ( pair<double,double> a, pair<double,double> b ) {
    return pair<double,double>((a.first+b.first)/2,(a.second+b.second)/2);
}

void printMatrix(double** matrix, int x, int y) {
    for(int i = 0 ; i < x ; i++ ) {
        for(int j = 0 ; j < y ; j++ ) {
            if ( j > i ) continue;
            cout<<matrix[i][j]<<"\t";
        }
        cout<<"\n";
    }
}

pair<double,double> getCentroid(vector<Point> cluster) {
    if (cluster.size() == 0) return pair<double,double>(0,0);
    double sumX = 0, sumY = 0, count = 0;
    for( auto it : cluster ) {
        sumX += it.coords.first;
        sumY += it.coords.second;
        count++;
    }
    return pair<double,double>(sumX/count,sumY/count);
}

void printCluster( vector<Point> cluster ) {
    cout<<"{";
    bool first = true;
    for(auto it: cluster) {
        if (!first) cout<<","; else first = false;
        cout<<" "<<it.name<<"";
    }
    cout<<" }";
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

   file.open("in.txt");

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
    

    Node* root = nullptr;

    std::cout << std::fixed;
    std::cout << std::setprecision(2);

    //ofstream outfile;
    //outfile.open("median_out.csv");

    double** dist_matrix = new double*[num_data];
    for(int i = 0 ; i < num_data; i++ ) dist_matrix[i] = new double[num_data];

    for(int i = 0 ; i < num_data; i++ ) {
        for(int j = 0 ; j < num_data; j++ ) {
            dist_matrix[i][j] = dist(data[i].coords,data[j].coords);
        }
    }

    printMatrix(dist_matrix, num_data, num_data);

    vector<Point> clusterA;
    vector<Point> clusterB;
    
    
    for(int i = 0 ; i < num_data ; i++ ) {
        clusterB.push_back(data[i]);
    }

    int counter = 0;
    int highest_delta = 0;
    auto ip = clusterB.begin();
    for( auto it = clusterB.begin() ; it != clusterB.end() ; it++ ) {
        vector<Point> tempB = clusterB;
        vector<Point> tempA;
        tempA.push_back(*it);
        tempB.erase(tempB.begin()+counter);
        counter++;

        double delta = dist( getCentroid( tempA ) , getCentroid( tempB ) );
        cout<<"D("<<it->name<<",*) = "<<delta<<"\n";
        if ( delta > highest_delta ) {
            ip = it;
            highest_delta = delta;
        }
    }
    cout<<"Put in Cluster A: "<<ip->name<<"\n";
    clusterA.push_back(*ip);
    clusterB.erase(ip);
    cout<<"A = "; printCluster(clusterA);   cout<<"\n";
    cout<<"B = "; printCluster(clusterB);   cout<<"\n";

    while ( highest_delta > 0 ) {
        counter = 0;
        highest_delta = 0;
        auto ip = clusterB.begin();
        for( auto it = clusterB.begin() ; it != clusterB.end() ; it++ ) {
            vector<Point> tempB = clusterB;
            vector<Point> temp;
            temp.push_back(*it);
            tempB.erase(tempB.begin()+counter);
            counter++;

            double distA = dist( getCentroid( temp ) , getCentroid( clusterA ) );
            double distB = dist( getCentroid( temp ) , getCentroid( tempB ) );
            cout<<"D("<<it->name<<",A) = "<<distA<<"\t";
            cout<<"D("<<it->name<<",B) = "<<distB<<"\t";
            double delta = distB - distA;
            cout<<"Delta("<<it->name<<") = "<<delta<<"\n";
            if ( delta > highest_delta ) {
                ip = it;
                highest_delta = delta;
            }
        }
        cout<<"Put in Cluster A: "<<ip->name<<"\n";
        clusterA.push_back(*ip);
        clusterB.erase(ip);
        cout<<"A = "; printCluster(clusterA);   cout<<"\n";
        cout<<"B = "; printCluster(clusterB);   cout<<"\n";
    }
    cout<<"All Delta negative, stop!\n";

}