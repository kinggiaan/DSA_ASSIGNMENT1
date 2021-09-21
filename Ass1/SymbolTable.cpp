#include "SymbolTable.h"
int global_level = 0;

SymbolTable::SymbolTable() :head(NULL) {};
SymbolTable::SymbolTable(SymbolNode* _head) : head(_head)  {};
SymbolTable::~SymbolTable() {
    SymbolNode* current = this->head;
    while (current != NULL) {
        SymbolNode* next = current->next;

        delete current;
        current = next;
    }
    head = NULL;


}
void SymbolTable::run(string filename)
{
    ifstream File(filename);
    string str="";
    if (File.is_open()) {
        SymbolTable MainTable;
        while (getline(File, str,'\n'))
        {
            string key1 = "";
            string key2 = "";
            string key3 = "";
            string temp = "";
            int count_key = 0, count_space = 0;
            ///Kiem tra _ cuoi dong neu co dung lai
            if (str[str.length() - 1] == ' ')
                throw InvalidInstruction(str);
            else {
               
                string quote = "'";
                
                size_t k,stop;

                for (k = 0; k < str.length(); k++) {
                    if (str[k] == ' ') count_space++;
                }
                if (!count_space) {
                    key1 = str.substr(0);
                    count_key++;
                    //cout << key1<<endl;
                }
                else {
                    if (count_space == 2) {//Neu 2 space la 3 gia tri 
                        for (k = 0; k < str.length(); k++) {
                            if (str[k] == ' ' && count_key == 0) {//key 1
                                key1 = str.substr(0, k);
                                count_key++;
                                k++;
                                stop = k;
                                //cout << key1 ;
                            }
                            if (str[k] == ' ' && count_key == 1) {//key 2 va 3
                                key2 = str.substr(stop, k-stop);
                                count_key++;
                                k++;
                                stop = k ;
                                //cout << key2;
                            
                            //if (str[k] == ' ' && count_key == 2) {//key 2
                                key3 = str.substr(stop, str.length());
                                
                                count_key++;
                                //cout << key3 << endl;
                            }
                        }
                    }
                    else if (count_space == 1) {//Neu 1 space la 2 gia tri 
                        for (k = 0; k < str.length(); k++) {
                            if (str[k] == ' ' && count_key == 0) {//key 1 va 2
                                key1 = str.substr(0, k);
                                count_key++;
                                k++;
                                stop = k;
                               // cout << key1;

                               

                                //if (str[k] == ' ' && count_key == 2) {//key 2
                                key2 = str.substr(stop, str.length());
                                count_key++;

                                //cout << key2 << endl;
                            }
                        }
                    }
                }

            }
        
            if (checkCode(key1) == true) {//Instruction Valid
                if (key1 == "INSERT" || key1 == "ASSIGN")
                    if (count_key != 3) throw InvalidInstruction(str);
                if (key1 == "LOOKUP")
                    if (count_key != 2) throw InvalidInstruction(str);
                if (key1 == "PRINT" || key1 == "RPRINT" || key1 == "BEGIN" || key1 == "END")
                    if (count_key != 1) throw InvalidInstruction(str);
            }
            else throw InvalidInstruction(str);
        ///// COMPLETE INVALID INSTRUCTION
            
            
         /////////INSERT////   
            if (key1 == "INSERT") {
                if (MainTable.head == NULL) {
                    MainTable.head = new SymbolNode();
                    MainTable.head->data.indentify = key2;
                    MainTable.head->data.level = global_level;
                    MainTable.head->data.type = key3;
                    MainTable.head->next = NULL;
                }
                else {
                    SymbolNode* temp = MainTable.head;
                    if (temp->data.indentify==key2&&global_level==temp->data.level) throw Redeclared(str);
                    SymbolNode* tmp = new SymbolNode;
                    tmp->data.indentify = key2;
                    tmp->data.level = global_level;
                    tmp->data.type = key3;
                    tmp->next = NULL;
                    while (temp->next != NULL) {
                        temp = temp->next;
                        //if (temp->data.indentify == tmp->data.indentify) throw Redeclared(str);
                        if (checkSameBlockLevelDec(tmp)) throw Redeclared(str);
                    }
                    temp->next = tmp;
                    

                }

                cout << "success" << endl;


            }


            ////////ASSIGN 

            if (key1 == "ASSIGN") {
                if (MainTable.head == NULL) throw Undeclared(str);//SymbolNode chua khoi tao
                else {
                    SymbolNode* temp = MainTable.head;
                    bool exist = false;
                    while (temp != NULL) {
                        
                        if (temp->data.indentify == key2) {//Neu tim thay
                            exist = true;
                            break;
                        }
                        
                    
                        temp = temp->next;
                    }
                    if (exist==false) throw Undeclared(str);//Neu khong tim thay de assign
                    else {
                        ////Kiem tra key3 thuoc dang nao COnstdigi,Constring hay Iden
                        if (checkQuote(key3)) {
                            if (temp->data.type == "string") {
                                if (checkConstString(deleteQuote(key3))) {
                                    cout << "success" << endl;
                                    //delete temp;
                                }
                                else throw TypeMismatch(str);
                            }
                            else throw TypeMismatch(str);
                        }
                        else if (checkConstDigi(key3)) {

                            if (temp->data.type == "number") {
                                if (checkConstDigi(key3)){
                                    cout << "success" << endl;
                                //delete temp;
                                }
                                else throw TypeMismatch(str);;
                            }
                            else throw TypeMismatch(str);
                      
                        }
                        else {
                            if(checkConstDigi(key3.substr(0,1))) throw TypeMismatch(str);
                            else {
                                SymbolNode* tmp = MainTable.head;
                                bool exits2 = false;
                                while (tmp != NULL)
                                {
                                    if (tmp->data.indentify == key3) {
                                        cout << "success" << endl;
                                        exits2 = true;
                                        // delete tmp;
                                        break;
                                    }
                                    tmp = tmp->next;
                                }
                                if (exits2 == false) throw Undeclared(str);
                            }
                        }
                
                    }


                }
            }

            ///////////BEGIN/END
            if (key1 == "BEGIN") {
                global_level++;
                //cout << "success" << endl;

            }

            if (key1 == "END") {
                global_level--;
                if (global_level >= 0) {
                    //cout << "success" << endl;
                }
                else throw UnknownBlock();
            }
           // cout << "success"<<endl;
        }
        
    }
    if (global_level != 0) throw UnclosedBlock(global_level);
}
/////////////Function Addition/////
bool checkCode(string str) {
    if (str == "INSERT" ||
        str == "ASSIGN" ||
        str == "BEGIN" ||
        str == "END" ||
        str == "LOOKUP" ||
        str == "PRINT" ||
        str == "RPRINT") return true;
    return false;
}
bool checkConstDigi(string str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (!(str[i] <= 57 && str[i] >= 48)) return false;
    }
    return true;
}
bool checkConstString(string str) {

    for (size_t i = 0; i < str.length(); i++) {
        if (!((str[i] <= 90 && str[i] >= 65) || (str[i] >= 97 && str[i] <= 122) || (str[i] <= 57 && str[i] >= 48) || str[i] == 32)) return false;
    }
    return true;
}
bool checkQuote(string str) {
    //for (size_t i = 0; i < str.length(); i++) {
        if (str[0]==39) return true;
    //}
    return false;
}
string deleteQuote(string str) {
    return str.substr(1, str.length() - 2);
}
bool SymbolTable::checkSameBlockLevelDec( SymbolNode* node) {
    SymbolNode* temp = this->head;
    while (temp != NULL) {
        if (temp->data.indentify == node->data.indentify) {
            if (temp->data.level == node->data.level) return true;
        }

        temp = temp->next;
    }
    return false;
}