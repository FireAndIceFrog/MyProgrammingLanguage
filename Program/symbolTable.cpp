#include <algorithm>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <locale>
#include <sstream>
#include <string>
 

using namespace std;

void split(const string& str, vector<string> *cont, char delim = ' ')
{
	stringstream ss(str);
	string token;
	while (getline(ss, token, delim)) {
		cont->push_back(token);
	}
}
string combine(vector<string> *cont, char delim = ' ')
{
	string finale = "";
	for (string item : *cont){
		finale+=item+delim;
	}
	return finale;
}

class symbolTable{
	private:
		//each row in the list is considered one element
		struct Table {
			string key;
			string value;
			bool constant;
		};
		//create a list of the table data
		vector <Table> List;
		//create a list of pre-defined used words
		vector <string> Taken;
		
		
	public:
		//add a value to the dictionary
		bool add(string key, string value, bool cons){
			//quick check to see if it is a already referenced word
			for (string item : Taken) {
				//compare each letter to the letter in the Taken list
				if(item.compare(key) == 0){
					return false;
				} else if (item.compare(key)>0){
					//stop looking, its not here. (vector assumed to be sorted)
					break;
				}
			}
			if (exists(key)) return false;
			//add the entry to the 
			Table tmp;
			tmp.key = key;
			tmp.value = value;
			tmp.constant = cons;
			vector<Table>::iterator pos = List.begin(); 

			//attempt to insert the items sorted
			for (Table item: List){
				if(item.key.compare(key)>0){
					List.insert(pos, tmp);
					return true;
				}
				++pos;
			}
			//if there are no matches, insert to end of list
			this->List.push_back(tmp);
			return true;
		}
		//function to print all the entries in this class.
		void printEntries(){
			vector<string> normKeys = this->listNormKeys();
			cout<<"Identifier list ("<<normKeys.size()<<"):\n";
			for (string item : normKeys){
				cout<<item<<": \"";
				cout<<this->getvalue(item);
				cout<<"\"\n";
			}
			
		}
		//function to return the key of a certain index
		string getKey(unsigned int i){
			if(i > this->List.size() || i < 0) return NULL;
			return this->List[i].key;
		}
		//return the list of all the keys which are not constants
		vector<string> listNormKeys(){
			vector<string> keys = vector<string>();
			for (Table item: List){
				if(!item.constant){
					keys.push_back(item.key);
				}
			}
			return keys;
		}
		//return the list of all the keys
		vector<string> listConstKeys(){
			vector<string> keys = vector<string>();
			for (Table item: List){
				if(item.constant){
					keys.push_back(item.key);
				}
			}
			return keys;
		}
		void reverse(string key){
			//function to reverse a variables string based on it's word count.
			vector<string> *words = new vector<string>();
			split(this->getvalue(key), words, ' ');
			std::reverse(words->begin(), words->end());
			this->setValue(key, combine(words,' '));
			
			
			
			
			
		}
		//get the value from the table of elements.
		string getvalue(string key){
			//loop until you find the right table
			for (Table item : List){
				if(item.key.compare(key) == 0){
					//return the first letter of the string
					
					return item.value;
				} else if (item.key.compare(key) > 0){
					//if it has gone over the sorted position, break becuase it isnt there.
					break;
				}
			}
			//nothing is found; return null
			return "Nothing";
		}
		bool exists(string key){
			//check to see that the key exists already, return true if it does.
			for (Table item : List) {
				//compare each letter to the letter in the Taken list
				if(item.key.compare(key) == 0){
					return true;
				}else if (item.key.compare(key) > 0){
					//if it has gone over the sorted position, break becuase it isnt there.
					break;
				}
			}
			return false;
		}
		bool setValue(string key, string value){
			//function to set the value of a variable that has already been created.
			for (unsigned int i = 0; i < List.size(); ++i){
				if(List[i].key.compare(key) == 0){
					//return the first letter of the string
					if(List[i].constant) return false;
					//set the table value
					List[i].value = value;
					return true;
				} else if (List[i].key.compare(key) > 0){
					//if it has gone over the sorted position, break becuase it isnt there.
					return false;
				}
			}
			return false;
		}
	
	
		//set up the symbol table. Add in all const keywords and initialize the values.
		symbolTable(vector <string> TakenWords){
			this->Taken = TakenWords;
			this->List = *(new vector<Table>());
			
			//base keywords are "SPACE" "TAB" and "NewLine"
			this->add("SPACE"," ", true);
			this->add("TAB","\t", true);
			this->add("NEWLINE","\n",true);
		}
};



void generateTaken(vector<string>* TakenWords ){
	//all words used to form program are Taken
	TakenWords->push_back("append");
	TakenWords->push_back("list");
	TakenWords->push_back("exit");
	TakenWords->push_back("print");
	TakenWords->push_back("printlength");
	TakenWords->push_back("printwords");
	TakenWords->push_back("printwordcount");
	TakenWords->push_back("set");
	TakenWords->push_back("reverse");
	
	
}




