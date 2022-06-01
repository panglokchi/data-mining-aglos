#include <deque>
#include <iostream>
#include <utility>
#include <cmath>
#include <vector>
#include <string>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <set>

using namespace std;

struct Node {
    int id;
    char item;
    int count;
    enum Type { ROOT, HEAD, NODE };
    Type type;
    int num_descendants;
    Node* next_node;
    Node* parent;
    int next_node_id;
    vector<Node*> descendants;
    vector<int> descendant_ids;
    //root constr
    Node(int id, vector<int> descendant_ids): id(id), descendant_ids(descendant_ids), num_descendants(descendant_ids.size()) {
        next_node_id = -1;
        type = Node::ROOT;
    }
    //head constr
    Node(int id, char item, int next_node_id): id(id), item(item), next_node_id(next_node_id), num_descendants(0) {
        type = Node::HEAD;
    }
    //node constr
    Node(int id, char item, int count, int next_node_id, vector<int> descendant_ids): id(id), item(item), count(count), descendant_ids(descendant_ids), next_node_id(next_node_id), num_descendants(descendant_ids.size()), type(Node::NODE) {
    }

    //dummy constrs
    Node(int id): id(id) {};
    Node(Type type): type(type) {};
    bool operator==(const Node& rhs) const {
        return this->id == rhs.id;
    }
};

struct Item {
    char name;
    int count;
    Item(char name, int count): name(name), count(count) {};
    bool operator<(const Item& rhs) const {
        if ( this->count == rhs.count )
            return this->name < rhs.name;
        else return this->count > rhs.count;
    }
    Item& operator+=(const Item& rhs) {
        if ( rhs.name != this->name ) {
            return *this;
        } else {
            this->count += rhs.count;
            return *this;
        }
    }
};

int main() {



    vector<Node> nodes;
    vector<int> head_node_ids;

    fstream file;
    file.open("tree.txt");

    int num_nodes;

    file>>num_nodes;

    for(int i = 0 ; i < num_nodes ; i++ ) {
        //cout<<i;
        int node_id;
        char item;
        file>>node_id;
        char type;
        int num_descendants;
        int next_node_id;
        vector<int> descendant_ids;
        Node* new_node;
        file>>type;
        if ( type == 'r' ) {
            file>>num_descendants;
            for(int i = 0 ; i < num_descendants ; i++ ) {
                int descendant;
                file>>descendant;
                descendant_ids.push_back(descendant);
            }
            new_node = new Node(node_id,descendant_ids);
            //nodes.push_back(Node(node_id,descendant_ids));
            //cout<<"root "<<descendant_ids.size()<<" "<<nodes.size()<<"\n";
        } else if ( type == 'h' ) {
            file>>item;
            file>>next_node_id;
            head_node_ids.push_back(node_id);
            //nodes.push_back(Node(node_id,item,next_node_id));
            new_node = new Node(node_id,item,next_node_id);
            //cout<<"head "<<item<<" "<<descendant_ids.size()<<" "<<nodes.size()<<"\n";
        } else if ( type == 'n' ) {
                int count;
                file>>item>>count>>next_node_id;
                file>>num_descendants;
                for(int i = 0 ; i < num_descendants ; i++ ) {

                    int descendant;
                    file>>descendant;
                    descendant_ids.push_back(descendant);

                }
                //cout<<" "<<descendant_ids.size();
                //cout<<"?";
                //cout<<"node "<<item<<" "<<count<<" "<<descendant_ids.size()<<" "<<nodes.size()<<"\n";
                //new_node = new Node(node_id,item,count,next_node_id/*,descendant_ids*/);
                new_node = new Node(node_id,item,count,next_node_id,descendant_ids);
                //nodes.push_back(  Node(node_id,item,count,next_node_id,descendant_ids)  );           
                //cout<<"?";
        } else {
            cout<<"INPUT ERROR 1";
            return 0;
        }

    nodes.push_back(*new_node);            
    }
    
    //link up nodes
    for(auto it = nodes.begin() ; it != nodes.end() ; it++ ) {
        //cout<<it->id<<" ";
        for( auto ip = it->descendant_ids.begin() ; ip != it->descendant_ids.end() ; ip++ ) {
            auto ik = find(nodes.begin(), nodes.end(), Node(*ip));
            //cout<<*ip<<" ";
            if ( ik == nodes.end() ) { cout<<"INPUT ERROR 2 at Node id "; cout<<it->id; cout<<"#childs: "<<it->descendant_ids.size(); return 0; }
            it->descendants.push_back(&(*ik));
            ik->parent = &(*it);
            //cout<<ik->id<<" ";
        }
        //cout<<"\n";
        if ( it->next_node_id != -1 ) {
            auto ik = find(nodes.begin(), nodes.end(), Node(it->next_node_id));
            if ( ik == nodes.end() ) { cout<<"INPUT ERROR 3 "; cout<<it->id; return 0; }
            it->next_node = &(*ik);
            //cout<<it.id<<" "<<ik->id<<"\n";
            //cout<<it.id<<" "<<it.next_node->id<<"\n";
            //cout<<it->id<<" "<<it->next_node<<" "<<(it->next_node)->id<<"\n";
            //cout<<&it<<"\n";
        } else { it->next_node = nullptr; }
    }

    vector<Node*> heads;

    for( auto it: head_node_ids ) {
        auto ik = find(nodes.begin(), nodes.end(), Node(it));
        heads.push_back(&(*ik));
        //cout<<ik->id<<" "<<&(*ik)<<"\n";
    }

    for (std::vector<Node*>::reverse_iterator it = heads.rbegin(); it != heads.rend(); ++it)
    {
        Node* itr = (*it)->next_node;
        //vector<pair<vector<char>,int>> itemsets;
        vector<vector<Item>> itemsets;
        vector<Item> sums;
        
        int sum_of_current_item = 0;

        do {

            //cout<<"itr: "<<itr->id<<"/"<<itr->count<<" ";
            Node* itu = itr;
            vector<Item> items;
            while( itu->type != Node::ROOT ) {
                //cout<<"itu: "<<itu->id<<" ";
                /*
                auto ik = itemsets.begin();
                for(  ; ik != itemsets.end() ; ik++ ) {
                    if ( ik->first == itu->item ) {
                        ik->second += itu->count;
                        break;
                    }
                }
                */
                
                /*
                if ( ik == itemsets.end() ) {
                    itemsets.push_back(pair<char,int>(itu->item,itu->count));
                }*/
                if ( itu->item == itr->item ) sum_of_current_item += itu->count;
                items.push_back(Item(itu->item, itr->count));
                //cout<<itu->id<<"/"<<items.size()<<" ";
                itu = itu->parent;
            }
            //cout<<"next \n"; 
            itemsets.push_back(items);
            
            itr = itr->next_node;
        } while( itr != nullptr );
        //cout<<" next head ";
        
        cout<<"Cond. FP-tree on {"<<(*it)->item<<"}:"<<sum_of_current_item<<"\n";
        for( auto ip = itemsets.begin() ; ip != itemsets.end() ; ip++) {
            cout<<"{";
            //for( auto ipp = ip->first.begin() ; ipp != ip->first.end() ; ipp++)
            //    cout<<*ipp;
            //cout<<":"<<ip->second<<"}\t\n";
            for( auto ipp = ip->begin() ; ipp != ip->end(); ipp++) {
                if ( ipp!= ip->begin()) cout<<",";
                cout<<ipp->name<<":"<<ipp->count;
            }
            cout<<"}\n";
        }


        auto ik = itemsets.begin();

        //tally items
        for(  ; ik != itemsets.end() ; ik++ ) {
            

            for( auto ij = ik->begin() ; ij != ik->end() ; ij++ ) {

                bool new_item = true;

                auto il = sums.begin();
                for(  ; il != sums.end() ; il++ ) {

                    
                    //cout<<">"<<il->first<<"/"<<*ij<<"<  ";
                    if ( il->name == ij->name ) {
                        il->count += ij->count;
                        new_item = false;
                    }
                    
                }

                if ( il == sums.end() && new_item == true ) {

                    //cout<<*ij<<" "<<ik->second;
                    sums.push_back(Item(*ij));
                }


            }
        }

        
        for(  auto il = sums.begin();  il != sums.end() ; il++ ) {

            //cout<<il->name<<" "<<il->count<<"\n";

        }


        set<Item> sorted_sums;
        int max = 0;
        for(  auto il = sums.begin();  il != sums.end() ; il++ ) {

           sorted_sums.insert(*il);

        }

        for(  auto il = sorted_sums.begin();  il != sorted_sums.end() ; il++ ) {

            cout<<il->name<<" "<<il->count<<"\n";

        }



        cout<<"===========================\n";
    }
}

