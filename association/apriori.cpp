#include <deque>
#include <iostream>
#include <utility>
#include <cmath>
#include <vector>
#include <string>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>

using namespace std;


struct Item {
    string name;
    int count;
    Item(string name): name(name) { count = 0; }
    Item(string name, int count): name(name), count(count) {};
    bool operator<(const Item rhs) const {
        return rhs.name > this->name;
    }
    bool operator==(const Item rhs) const {
        return rhs.name == this->name;
    }
};


struct Transaction {
    string id;
    vector<Item> items;
    Transaction(string id, vector<Item> items): id(id), items(items) {

    }
};

struct Itemset {
    vector<set<Item>> itemset;
    int iteration;
    enum Type { CANDIDATE, LARGE };
    Type type;
    Itemset( Type type, int iteration, vector<set<Item>> itemset): type(type), iteration(iteration), itemset(itemset) {};
    Itemset( Type type, int iteration): type(type), iteration(iteration) {};
    void print() {
        cout<<"{ ";
        for( int i = 0 ; i < itemset.size() ; i++ ) {
            if( i != 0 ) cout<<",";
            cout<<" { ";
            for( auto it : itemset[i] ) {
                if( it.name != itemset[i].begin()->name ) {
                    cout<<" , ";
                }
                cout<<it.name;
            }
            cout<<" } ";
        }
        cout<<" }\n";
    }
};


struct Problem {
    int threshold;
    vector<Transaction> transactions;
    vector<Item> items;
    vector<Itemset> candidate_itemsets;
    vector<Itemset> large_itemsets;
    Problem(int threshold, vector<Transaction> transactions, vector<Item> items): threshold(threshold), transactions(transactions), items(items) {};

    bool checkItemset(set<Item> itemset) {
        int count = 0;
        for(int i = 0 ; i < transactions.size() ; i++ ) {
            bool present = true;
            for( auto it : itemset ) {
                for( auto iit : transactions[i].items ) {
                    if( it.name == iit.name && iit.count <= 0 ) {
                        present = false;
                    }
                }
            }
            if ( present ) count++;
        }
        //cout<<"\n itemset ";
        //for( auto it :itemset ) cout<<it.name;
        //cout<<" support "<<count<<"\n";
        return count >= threshold;
    }

    void solve() {
        for(int i = 0 ; i < items.size() ; i++ ) {
            cout<<items[i].name<<" ";
        }
        cout<<"\n";
        for(int i = 0 ; i < transactions.size() ; i++ ) {
            for(int j = 0 ; j < transactions[i].items.size() ; j++ ) {
                cout<<transactions[i].items[j].count<<" ";
            }
            cout<<"\n";
        }
        // tally items
        for( int i = 0 ; i < transactions.size() ; i++ ) {
            for( int j = 0 ; j < transactions[i].items.size() ; j++ ) {
                items[j].count += transactions[i].items[j].count;
            }
        }
        for(int i = 0 ; i < items.size() ; i++ ) {
            cout<<items[i].name<<" "<<items[i].count<<"\n";
        }
        candidate_itemsets.push_back(Itemset( Itemset::CANDIDATE, 0));    // dummy 0-itemset
        large_itemsets.push_back(Itemset( Itemset::LARGE, 0));        // dummy 0-itemset
        //large 1
        Itemset candidate_1( Itemset::CANDIDATE , 1 );
        Itemset large_1( Itemset::LARGE , 1 );

        for(int i = 0 ; i < items.size() ; i++ ) {
            set<Item> itemset_1_i;
            itemset_1_i.insert(items[i]);
            candidate_1.itemset.push_back(itemset_1_i);
            if( items[i].count >= threshold ) {
                large_1.itemset.push_back(itemset_1_i);
            }
        }
        candidate_itemsets.push_back(candidate_1);
        large_itemsets.push_back(large_1);

        cout<<"\nCandidate 1-itemsets: ";   candidate_1.print();
        cout<<"Large 1-itemsets: ";         large_1.print(); 

        //large 2
        Itemset candidate_2( Itemset::CANDIDATE , 2 );
        Itemset large_2( Itemset::LARGE , 2 );

        for( int i = 0 ; i < large_1.itemset.size() ; i++ ) {
            for( int j = i + 1 ; j < large_1.itemset.size() ; j++ ) {
                set<Item> itemset_2_i;

                itemset_2_i.insert(large_1.itemset[j].begin(), large_1.itemset[j].end());
                itemset_2_i.insert(large_1.itemset[i].begin(), large_1.itemset[i].end());

                candidate_2.itemset.push_back(itemset_2_i);

                if( checkItemset(itemset_2_i) ) {
                    //if( large_2.itemset.size() > 0 ) cout<<",";
                    large_2.itemset.push_back(itemset_2_i);
                    //cout<<" { "<<large_1.itemset[i].begin()->name<<" , "<<large_1.itemset[j].begin()->name<<" } ";
                }
            }
        }

        candidate_itemsets.push_back(candidate_2);
        large_itemsets.push_back(large_2);
        //print 2-itemsets
        cout<<"\nCandidate 2-itemsets: ";   candidate_2.print();
        cout<<"Large 2-itemsets: ";         large_2.print();

        // 3 and up

        bool finished = false;
        int iteration = 3;

        while( !finished ) {
            Itemset candidate_i( Itemset::CANDIDATE , iteration );
            Itemset large_i( Itemset::LARGE , iteration );
            auto its = large_itemsets[iteration-1].itemset.begin();
            auto it = large_itemsets[iteration-1].itemset.begin();
            candidate_itemsets.push_back(Itemset( Itemset::CANDIDATE , iteration));
            large_itemsets.push_back(Itemset( Itemset::LARGE , iteration));
            set<Item> prefix;
            while ( it != large_itemsets[iteration-1].itemset.end() ) {
                set<Item> current_set_prefix;
                for( auto itt = it->begin() ; itt != --( it->end() ) ; itt++ ) {
                    current_set_prefix.insert(*itt);
                }
                //dbg
                //for( auto ipp = current_set_prefix.begin() ; ipp != current_set_prefix.end() ; ipp++ ) {
                //    cout<<ipp->name<<" ";
                //}
                //dbg
                //cout<<"\n";
                if ( current_set_prefix != prefix || it == --( large_itemsets[iteration-1].itemset.end()) ) {
                    prefix = current_set_prefix;
                    if ( its != it ) {

                        for( auto itn = its ; itn != (it-1) ; itn++ ) {
                            for( auto itp = itn + 1; itp != it ; itp++ ) {
                                set<Item> new_set;
                                set_union(itn->begin(), itn->end(), itp->begin(), itp->end(), inserter(new_set, new_set.begin()) );
                                candidate_itemsets[iteration].itemset.push_back(new_set);
                                //cout<<"\nCandidate "<<iteration<<"-itemsets: ";     candidate_itemsets[iteration].print();
                            }
                        }
                    }
                    its = it;
                } //else cout<<"skip\n";
                it++;
            }
            cout<<"\nCandidate "<<iteration<<"-itemsets: ";     candidate_itemsets[iteration].print();
            /*
            vector<vector<set<Item>>::iterator> to_delete;

            for( auto ip = candidate_itemsets[iteration].itemset.begin() ; ip != candidate_itemsets[iteration].itemset.end() ; ip++ ) {
                vector<set<Item>> subsets;
                for ( int i = 0 ; i != ip->size() ; i++ ) {
                    set<Item> subset = *ip;
                    auto ipp = subset.begin();
                    for ( int j = 0 ; j < i ; j++ ) ipp++;
                    subset.erase(ipp);
                    subsets.push_back(subset);
                }
                //for( int i = 0 ; i < subsets.size() ; i++ ) {
                //    for( auto ij : subsets[i] ) {
                //        cout<< ij.name;
                //    }
                //}
                for( auto ik = subsets.begin() ; ik != subsets.end() ; ik++ ) {
                    
                    if ( !checkItemset(*ik) ) {
                        to_delete.push_back(ip);
                    }
                    
                    if ( find( large_itemsets[iteration-1].itemset.begin() ,  large_itemsets[iteration-1].itemset.end() , *ik ) == large_itemsets[iteration-1].itemset.end()  ) {
                        to_delete.push_back(ip);
                        cout<<" X ";
                        //candidate_itemsets[iteration].itemset.erase(ip);
                        break;
                    } else cout<<" o ";
                }
            }
            //for( auto io = to_delete.begin() ; io != to_delete.end() ; io++ ) {
            //    for( auto ix = (*io)-> )

            //}
            for( auto iu = to_delete.begin() ; iu != to_delete.end() ; iu++ ) {
                candidate_itemsets[iteration].itemset.erase(*iu);
            }
            */
            bool finished_pruning = true;
            auto ip = candidate_itemsets[iteration].itemset.begin();

            if ( ip != candidate_itemsets[iteration].itemset.end() ) {
                do {
                    vector<set<Item>> subsets;
                    for ( int i = 0 ; i != ip->size() ; i++ ) {
                        set<Item> subset = *ip;
                        auto ipp = subset.begin();
                        for ( int j = 0 ; j < i ; j++ ) ipp++;
                        subset.erase(ipp);
                        subsets.push_back(subset);
                    }
                    cout<<subsets.size();
                    bool removed = false;
                    for( auto ik = subsets.begin() ; ik != subsets.end() ; ik++ ) {
            
                        if ( find( large_itemsets[iteration-1].itemset.begin() ,  large_itemsets[iteration-1].itemset.end() , *ik ) == large_itemsets[iteration-1].itemset.end()  ) {
                            finished_pruning = false;
                            candidate_itemsets[iteration].itemset.erase(ip);
                            ip = candidate_itemsets[iteration].itemset.begin();
                            removed = true;
                            break;
                        }
                    }
                    if ( removed ) {
                        continue;
                    }
                    ip++;
                    if ( ip == candidate_itemsets[iteration].itemset.end() ) {
                        break;
                    }            

                } while ( !finished_pruning );
            }

            cout<<"Prune step:\n";
            cout<<"Candidate "<<iteration<<"-itemsets: ";     candidate_itemsets[iteration].print();

            cout<<"Counting step:\n";
            for( auto ip = candidate_itemsets[iteration].itemset.begin() ; ip != candidate_itemsets[iteration].itemset.end() ; ip++ ) {
                if ( checkItemset(*ip) ) {
                    large_itemsets[iteration].itemset.push_back(*ip);
                }
            }

            cout<<"Large "<<iteration<<"-itemsets: ";           large_itemsets[iteration].print();
            iteration++;
            if ( large_itemsets[iteration].itemset.size() == 0 ) {
                finished = true;
            }
        };

        //candidate_itemsets.push_back
        // large 2
        //for( int i = 0 ; i < )
    }
};

Problem initFromMatrix(string filename) {

    int cols = 0, rows = 0;
    int threshold = 0;

    fstream file;
    file.open("matrix.txt");
    file>>threshold>>rows>>cols;

    vector<Transaction> transactions;
    vector<Item> items;

    for(int i = 0 ; i < rows ; i++ ) {
        if ( i == 0 ) {
            for(int j = 0 ; j < cols ; j++ ) {
                string item_name;
                file>>item_name;
                items.push_back(Item(item_name));
                //cout<<items[j].name;
                //cout<<endl;
            }
        } else {
            vector<Item> transaction_items;
            for(int j = 0 ; j < cols ; j++ ) {
                int count;
                file>>count;
                transaction_items.push_back(Item(items[j].name,count));
                //cout<<items[j].name<<count;
            }
            for(int k = 0; k < transaction_items.size() ; k++ ) {
                //cout<<transaction_items[k].first<<transaction_items[k].second<<endl;
            }

            transactions.push_back(Transaction(to_string(i),transaction_items));
        }
    }

    Problem problem(threshold, transactions, items);
    return problem;
};

int main() {

    Problem problem = initFromMatrix("matrix.txt");

    problem.solve();

}