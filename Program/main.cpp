#include <algorithm>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <locale>
#include <sstream>
#include <string>
#include <regex>
#include "symbolTable.cpp"
using namespace std;
void start(string newLine, symbolTable &TB, int lineCount);
bool isSemicolon(string *line){
	if ( regex_match(*line, regex(";"))){
		return true;
	}
	return false;
}
//function to find the statement in a string and remove it from the original string

void removeSpaces(string *input){
	while((regex_match(*input, regex(" .*(\n*.*)*"))||regex_match(*input, regex("\t.*(\n*.*)*")))&&input->size()>1){
		//remove the space at the front of the text.
		
		input->erase(0,1);
		
	}

}
//function to match a non-constant identifier given by a vector of strings
string findIdentifier(string *line,symbolTable *TB, bool findConst){
	//get the identifiers
	vector<string> NormalKeys = TB->listNormKeys();
	vector<string> ConstKeys = TB->listConstKeys();
	removeSpaces(line);
	for (string item : NormalKeys){
		if (regex_match (*line, regex(item+".*(\n*.*)*") )){
			if(!findConst){
				throw ("Error on line, Cannot use the Non-Constant: "+item);
			}
			line->erase(0,item.size());
			
			return item;
		}
	}
	for (string item : ConstKeys){
		
		if (regex_match (*line, regex(item+".*(\n*.*)*") )){
			if(findConst){
				throw ("Error on line, Cannot use the Constant: "+item);
			}
			line->erase(0,item.size());
			
			
			return item;
		}
	}
	removeSpaces(line);
	return "";
}

void findExperession(string *line, symbolTable *TB, string *str){
	removeSpaces(line);
	string output = "";
	try{
		output = findIdentifier(line,TB, true);
		//cout<<"Item: \""<<output<<"\"\n";
	} catch (string msg){
		output = findIdentifier(line,TB,false);
		//cout<<"Found const: \""<<output<<"\"\n";
		
	} 
	removeSpaces(line);
	if(output == ""){
		//use the literal for the string
		if(regex_match (*line, regex("^\".*(\n*.*)*"))){
			*line =  line->substr(line->find("\"")+1, line->size()-1);
			output = line->substr(0,line->find("\""));
			*line =  line->substr(line->find("\"")+1, line->size()-1);
			removeSpaces(line);
		} else {
			
			throw "No Such variable or literal \n";
		}
	} else {
		
		//find the literal from the symbol table
		output = TB->getvalue(output);
	}
	*str += output;
	if(line->find("+")==0&& line->size()>1){
		*line =  line->substr(1, line->size()-1);
		removeSpaces(line);

		findExperession( line,  TB, str);
	}
}
void repeat(string *newLine, symbolTable &TB, int lineCount){
	removeSpaces(newLine);

	if(newLine->find(";") == 0){
		*newLine = newLine->substr(1, newLine->size());
		
		start(*newLine, TB, lineCount);

	}
	else {
		cerr<<"Expected ';'\n";
		cerr<<"Line: "<<*newLine<<"\n";
		cerr<<"Error on line: "<<lineCount<<"\n";
		
	}
}
int findStatement(string *line){
	removeSpaces(line);
	string statements[9];
	statements[0] = "append";
	statements[1] = "list";
	statements[2] = "exit";
	statements[3] = "printlength";
	statements[4] = "printwords";
	statements[5] = "printwordcount";
	statements[6] = "print";
	statements[7] = "set";
	statements[8] = "reverse";
	int i = 0;
	
	for (string item : statements){
		
		if (regex_match (*line, regex(item+".*(\n*.*)*") )) {
			removeSpaces(line);
			break;
		} else {
			++i;
		}
		
	}
	line->erase(0, statements[i].size());
	removeSpaces(line);
	return i;
}

void start(string newLine, symbolTable &TB, int lineCount){
	string letter = "[_a-zA-Z]";
	string digit = "[0-9]";
	string punctuation = "[.,:;?!@#$%^&*()]\"\\";
	vector<string> *cont;
	int statement = findStatement(&newLine);
	string key;
	string expression;
	switch (statement){
		//append
		case(0):
			try{
				key = findIdentifier(&newLine, &TB, true);
			} catch (string msg){
				cerr<<"Line: "<<newLine<<"\n";
				cerr<<"Error on line: "<<lineCount<<"\n"<<msg;
				break;
			}
			if (key == ""){
				int ident = newLine.find(" ");
				key = newLine.substr(0,ident);
				if(regex_match(key, regex(digit+".*(\n*.*)*"))) {cout<<"Cannot start a variable name with a number!\n" ;break;};
				TB.add(newLine.substr(0,ident), "", false);
				newLine.erase(0,ident);
				
			}
			expression = "";
			//get the expression out of the string.
			try{
				findExperession(&newLine, &TB, &expression);
				if(!TB.setValue(key, TB.getvalue(key)+expression)){
					cout<<"Bad entry\n";
				}
			} catch (const char * msg){
				cerr<<"Error on line: "<<lineCount<<"\n"<<msg;
			}
			repeat(&newLine, TB, lineCount);
			break;
		//list
		case(1):
			TB.printEntries();
			cout<<"\n";
			repeat(&newLine, TB, lineCount);
			break;
		//exit
		case(2):
			if(isSemicolon(&newLine)){
				cout<<"Exiting!\n";
				exit(10);
			} else {
				cout<<"Expecting \";\", got Nothing!";
			}
			repeat(&newLine, TB, lineCount);
			break;

		//print
		case(6):
			try{
				expression = "";
				findExperession(&newLine, &TB, &expression);
				cout<<expression<<"\n";
			} catch (const char * msg){
				cerr<<"Error trying to print on line: "<<lineCount<<"\n"<<msg;
			}
			repeat(&newLine, TB, lineCount);
			break;
		//printlength

		case(3):
			try{
				expression = "";
				findExperession(&newLine, &TB, &expression);
				cout<<expression.size()<<"\n";
			} catch (const char * msg){
				cerr<<"Error on line: "<<lineCount<<"\n"<<msg;
			}
			repeat(&newLine, TB, lineCount);
			break;
		//printwords

		case(4):
			try{
				cont = new vector<string>();
				expression = "";
				findExperession(&newLine, &TB, &expression);
				split(expression, cont, ' ');
				for ( string item: *cont){
					cout<<item<<"\n";
				}
				delete cont;

				
				
			} catch (const char * msg){
				cerr<<"Error on line: "<<lineCount<<"\n"<<msg;
			}
			repeat(&newLine, TB, lineCount);
			break;

		//printwordcount
		case(5):
			try{
				cont = new vector<string>();
				expression = "";
				findExperession(&newLine, &TB, &expression);
				split(expression, cont, ' ');
				cout<<"Size is: "<<cont->size()<<"\n";
				delete cont;
			} catch (const char * msg){
				cerr<<"Error on line: "<<lineCount<<"\n"<<msg;
			}
			repeat(&newLine, TB, lineCount);
			break;
			
		//set
		case(7):
			try{
				key = findIdentifier(&newLine, &TB, true);
			} catch (string msg){
				cerr<<"Couldnt find identifier\n";
				cerr<<"Line: "<<newLine<<"\n";
				cerr<<"Error on line: "<<lineCount<<"\n"<<msg;
				
				break;
			}
			if (key == ""){
				removeSpaces(&newLine);
				int ident = newLine.find(" ");

				key = newLine.substr(0,ident);
				if(regex_match(key, regex((digit+".*$")))) 
				{
					cout<<"Cannot start variables with a number!\n";break;
				}
				if(TB.add(newLine.substr(0,ident), "", false)){
					newLine.erase(0,ident);
				} else {
					cout << "Cannot add to symbol table! is the Key Null?\n";
					break;
				}
				
				
			}
			expression = "";
			//get the expression out of the string.
			try{
				findExperession(&newLine, &TB, &expression);
				if(!TB.setValue(key, expression)){
					cout<<"Bad entry\n";
				}
			} catch (const char * msg){
				cerr<<"Error trying to set on line: "<<lineCount<<"\n"<<msg;
			}
			repeat(&newLine, TB, lineCount);
			break;
		//reverse
		case(8):
			try{
				key = findIdentifier(&newLine, &TB, true);
			} catch (string msg){
				cerr<<"Line: "<<newLine<<"\n";
				cerr<<"Error on line: "<<lineCount<<"\n"<<msg;
				break;
			}
			if(!isSemicolon(&newLine)){
				cout<<"Semicolon expected!\n";
			} else {
				TB.reverse(key);
			}
			repeat(&newLine, TB, lineCount);
			
			
			break;
		//error
		case(-1):
			cout<<"There was an error - this statement is not recognized";
			break;
		
		
	};

}
int find(string str, char target){
	int index = -1;
	unsigned int i = 0;
	for(i = 0; i < str.size(); ++i){
		if(str[i] == target){
			index = i;
			break;
		}
	}
	return index;
	
	
}
int countOccurances(string str, char target){
	
	int occurance = 0;
	while (str.size() > 0){
		int index = find(str, target);
		if (index >= 0){
			occurance +=1;
			str = str.substr(index+1,str.size());
		} else {
			str = "";
		}
	}
	
	return occurance;
}
	
	
	


		

int main(){
	string in;
	int quoteCounter = 0;
	int opCounter = 1;
	string query = "";
	int lineCount = 0;
	vector<string> TakenWords = *(new vector< string>());
	generateTaken(&TakenWords);
	symbolTable Table = symbolTable(TakenWords);
	
	
	while(true){
		getline(cin, in, '\n');
		query += in;
		quoteCounter += countOccurances(in,'"');
		bool match = regex_match(in, regex(".*\\+ *\t*$"));
		if (quoteCounter %2 == 0 && !match){
			string str = query;
			str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
			if(str.find_last_of(';') == str.size()-1 ){	
				
				start(query, Table, lineCount);
				query = "";
			} else if(str.find_last_of('\n') == str.size()-1 ){	
				query = query.substr(0,query.size()-1);

			}
		} else if (quoteCounter %2 != 0) {
			quoteCounter = quoteCounter % 2;
			query += "\n";
		} 
			
	
		
		++lineCount;
	}
}