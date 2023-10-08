#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <string> 
#include <map>

using namespace std;

class Tag {
    public:
        string label;
        vector<pair<string,string>> attributes;
        vector<Tag *> subtags;
        bool open;
    
    void addAttribute(string attribute, string value){
        pair<string, string> av;
        av.first = attribute;
        av.second = value;
        attributes.push_back(av);
    }
    void addSubtag(Tag *t){
        subtags.push_back(t);
    }
    
    string getVal(string query){
        for(int i=0; i< (int)attributes.size(); i++){
            if(attributes[i].first == query)
                return attributes[i].second;
        }
        return "Not Found!";
    }
};


int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */ 
    vector<Tag *> tags;
    string tagname;
    Tag *parent = new Tag;
    tags.push_back(parent);
    bool open;
    int N, Q;
    cin >> N >> Q;
    string line;
    getline(cin, line);
    size_t eq = 0;
    for(int i = 0; i<N; i++){
        getline(cin, line);
        // if open, add subtag to current tag (top of stack) and add new tag to stack
        (line[1] != '/') ? open = true : open = false;
        if(open){
            
            size_t endtag = line.find(" ");
            if(endtag == string::npos){
                endtag = line.size()-1;
            }
            Tag *curr = new Tag;
            curr->label = line.substr(1,endtag-1);
            size_t start = line.find((curr->label))+ endtag-1;
            while((eq = line.find('=', eq+1)) != string::npos){
                string attribute = line.substr(start+1,eq-2-start);
                size_t startq = line.find('"', eq);
                size_t endq = line.find('"', startq+1);
                string value = line.substr(startq+1, endq-startq-1);
                start = endq +1;
                curr->addAttribute(attribute, value);
            }
            tags.push_back(curr);
            parent->addSubtag(curr);
            parent = curr;
        }
        else{
            tags.pop_back();
            parent = tags.back();
        }
        // if closed, pop stack (reset current tag);
    }
    // start processing Q queries
    for(int j = 0; j<Q; j++){
        getline(cin, line);
        size_t lastdot = 0;
        size_t second_to_lastdot;
        Tag *curr;
        parent = tags[0];
        vector<Tag *>::iterator itr;
        bool first = 1;
        while((lastdot = line.find('.',lastdot+1)) != string::npos){
            if(first ==1){
                second_to_lastdot = 0;
                first = 0;
            }
            else{
                second_to_lastdot = line.find_last_of('.',lastdot-1)+1;
            }
            string currtag = line.substr(second_to_lastdot,lastdot-second_to_lastdot);
            for(itr = (parent->subtags).begin(); itr<(parent->subtags).end(); itr++){
                if(currtag == (*itr)->label){
                    parent = *itr;
                    break;
                }
            }
        }
        size_t squiggle = line.find('~');
        lastdot = line.find_last_of('.');
        string currtag = line.substr(lastdot+1, squiggle-lastdot-1);
        bool found =0;
        for(itr = (parent->subtags).begin(); itr<(parent->subtags).end(); itr++){
            if(currtag == (*itr)->label){
                parent = *itr;
                found = 1;
                break;
            }
        }
        if(found){
            string query = line.substr(squiggle+1, line.size());
            string val = parent->getVal(query);
            cout<< val<< endl;
        }
        else{
            cout << "Not Found!"<< endl;
        }
    }
    
      
    return 0;
}
