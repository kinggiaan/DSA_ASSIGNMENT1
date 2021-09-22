#include "SymbolTable.h"


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
bool SymbolTable::checkSameBlockLevelDec(SymbolNode* node) {
    SymbolNode* temp = this->head;
    while (temp != NULL) {
        if (temp->data.indentify == node->data.indentify) {
            if (temp->data.level == node->data.level) return true;
        }

        temp = temp->next;
    }
    return false;
}
void SymbolTable::PRINT(int globallv) {
    SymbolNode* temp = this->head;
    while (temp != NULL) {
        if (temp->redeclared == 0) {
            if (temp->next == NULL) {
                cout << temp->data.indentify << "//" << temp->data.level << endl;
            }
            else cout << temp->data.indentify << "//" << temp->data.level << " ";
        }
        else if (temp->data.level == globallv) {
            if (temp->next == NULL) {
                cout << temp->data.indentify << "//" << this->LOOKUPLargest(temp->data.indentify) << endl;
            }
            else cout << temp->data.indentify << "//" << this->LOOKUPLargest(temp->data.indentify) << " ";
        }

        temp = temp->next;
    }
}
int SymbolTable::LOOKUPLargest(string iden) {
    int largestLevel = 0, iniLevel = 0;
    SymbolNode* temp = this->head;
    while (temp != NULL) {
        if (temp->data.indentify == iden) {

            iniLevel = temp->data.level;

            if (iniLevel > largestLevel) largestLevel = iniLevel;
        }
        temp = temp->next;
    }
    return largestLevel;

}
void SymbolTable::RPRINT() {}
SymbolNode* SymbolTable::REVERSE() {
    SymbolNode* head_ref = this->head;
    SymbolNode* curr = this->head;
    SymbolNode* pre = NULL;
    SymbolNode* next = NULL;
    while (curr != NULL) {
        next = curr->next;
        curr->next = pre;
        pre = curr;
        curr = next;
    }
    head_ref = pre;
    return head_ref;
}
void SymbolTable::DeleteNode(SymbolNode* Node) {
    
    
}
void SymbolTable::run(string filename)
{
    int global_level = 0;
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
                    if (count_space >= 2) break;
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
                    bool again = false;//Kiem tra iden xuat hien lan nao trong MainTable chua
                    SymbolNode* temp = MainTable.head;
                    while (temp != NULL) {
                        if (temp->data.indentify == key2) {
                            if (global_level == temp->data.level) throw Redeclared(str);
                            else {
                                again = true;
                                temp->redeclared = true;
                            }
                        }
                        temp = temp->next;
                    }
                    SymbolNode* tmp = new SymbolNode;
                    tmp->data.indentify = key2;
                    tmp->data.level = global_level;
                    tmp->data.type = key3;
                    tmp->next = NULL;
                    
                    if(again) tmp->redeclared=true;// Neu co xuat hien roi thi tang len theo block
                    
                    temp = MainTable.head;
                    while (temp->next != NULL) {
                        temp = temp->next;
                        //if (temp->data.indentify == tmp->data.indentify) throw Redeclared(str);
                        //if (checkSameBlockLevelDec(tmp)) throw Redeclared(str);
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
                                        if (temp->data.type == tmp->data.type)
                                        {
                                            cout << "success" << endl;
                                            exits2 = true;
                                            // delete tmp;
                                            break;
                                        }
                                        else throw TypeMismatch(str);
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
            ///END
            if (key1 == "END") {
                SymbolNode* temp = MainTable.head;
                while (temp != NULL) {
                    
                    bool found = false;
                    if (temp->data.level == global_level) {
                        SymbolNode* Node = temp;
                        found = true;
                        temp = temp->next;
                        if (MainTable.head == Node) {//Node in the head
                            MainTable.head = Node->next;
                            Node->next = NULL;
                            delete Node;
                        }
                        else {
                           
                            SymbolNode* tmp = MainTable.head;
                            while (tmp != NULL) {
                                if (tmp->next == Node) {
                                    tmp->next = Node->next;
                                    Node->next = NULL;
                                    delete Node;
                                    break;
                                }

                                tmp = tmp->next;
                            }
                            
                        }

                    }
                    if (found) found = false;
                    else temp = temp->next;
                }
                global_level--;
                if (global_level >= 0) {
                    //cout << "success" << endl;
                }
                else throw UnknownBlock();
            }
            ///LOOKUP
            if (key1 == "LOOKUP") {
                
                if (MainTable.head == NULL) throw Undeclared(str);//SymbolNode chua khoi tao
                else {
                    int largestLevel = 0,iniLevel=0;
                    bool found = false;
                    SymbolNode* temp = MainTable.head;
                    while (temp != NULL) {
                        if (temp->data.indentify == key2) {
                            found = true;
                            iniLevel = temp->data.level;
                            if (iniLevel == global_level) {
                                largestLevel = iniLevel;
                                break;
                            }
                            if (iniLevel > largestLevel) largestLevel = iniLevel;
                            //if (largestLevel == 0) break;
                        }
                        temp = temp->next;
                    }
                    if (found) cout << largestLevel << endl;
                    else throw Undeclared(str);
                }
            }
            ///PRINT
            if (key1 == "PRINT") {
              MainTable.PRINT(global_level);
            }
            ///RPRINT
            if (key1 == "RPRINT") {
                MainTable.head=MainTable.REVERSE();
                MainTable.PRINT(global_level);
                MainTable.head = MainTable.REVERSE();
            }
           // cout << "success"<<endl;


        }
        
    }
    if (global_level > 0) throw UnclosedBlock(global_level);
    //else if (global_level < 0) throw UnknownBlock;
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
