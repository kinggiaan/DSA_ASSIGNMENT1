#include "SymbolTable.h"


SymbolTable::SymbolTable() :head(NULL) {};//Constructor
SymbolTable::SymbolTable(SymbolNode* _head) : head(_head)  {};//Construct
SymbolTable::~SymbolTable() {//Destructor
    SymbolNode* current = this->head;
    while (current != NULL) {
        SymbolNode* next = current->next;

        delete current;
        current = next;
    }
    head = NULL;


}
bool SymbolTable::checkSameBlockLevelDec(SymbolNode* node) {///Kiem tra level lon nhat cua node
    SymbolNode* temp = this->head;
    while (temp != NULL) {
        if (temp->data.indentify == node->data.indentify) {
            if (temp->data.level == node->data.level) return true;
        }

        temp = temp->next;
    }
    return false;
}
string SymbolTable::PRINT(int globallv) {//PRINT Function 
    string res = "";
    SymbolNode* temp = this->head;
    while (temp != NULL) {
        if (temp->redeclared == 0) {
            if (temp->next == NULL) {
                res += temp->data.indentify;
                res += "//";
                res+= to_string(temp->data.level);
                    
            }
            else { 
                res += temp->data.indentify;
                res += "//";
                res += to_string(temp->data.level);
                res += " ";
            }
        }
        else if (temp->data.level == this->LOOKUPLargest(temp->data.indentify)) {
            if (temp->next == NULL) {
                res += temp->data.indentify;
                res += "//";
                res += to_string(temp->data.level);

            }
            else {
                res += temp->data.indentify;
                res += "//";
                res += to_string(temp->data.level);
                res += " ";
            }
        }

        temp = temp->next;
    }
    if (res[res.length() - 1] == ' ') res = res.substr(0, res.length() - 1);
    return res;
}
int SymbolTable::LOOKUPLargest(string iden) {//TIM Level lon nhat cua iden 
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
SymbolNode* SymbolTable::REVERSE() {//Reverse Linklist
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
                
                size_t k,stop;

                for (k = 0; k < str.length(); k++) {//Dem so _
                    if (str[k] == ' ') count_space++;
                    if (count_space >= 2) break;
                }
                if (!count_space) {//Neu khong co space
                    key1 = str.substr(0);
                    count_key++;
                }
                else {
                    if (count_space == 2) {//Neu 2 space la 3 gia tri 
                        for (k = 0; k < str.length(); k++) {
                            if (str[k] == ' ' && count_key == 0) {//key 1
                                key1 = str.substr(0, k);
                                count_key++;
                                k++;
                                stop = k;
                                
                            }
                            if (str[k] == ' ' && count_key == 1) {//key 2 va 3
                                key2 = str.substr(stop, k-stop);
                                count_key++;
                                k++;
                                stop = k ;
                                
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
                               
                                key2 = str.substr(stop, str.length());
                                count_key++;

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
                if (MainTable.head == NULL) {///INSERT Node dau tien
                    MainTable.head = new SymbolNode();
                    MainTable.head->data.indentify = key2;
                    MainTable.head->data.level = global_level;
                    MainTable.head->data.type = key3;
                    MainTable.head->next = NULL;
                }
                else {
                    bool again = false;//Kiem tra iden xuat hien lan nao trong MainTable chua
                    SymbolNode* temp = MainTable.head;
                    while (temp != NULL) {//Kiem tra iden da co xuat hien hay chua
                        if (temp->data.indentify == key2) {
                            if (global_level == temp->data.level) throw Redeclared(str);//Neu da xuat hien va nam trong level block 
                            else {
                                again = true;
                                temp->redeclared = true;
                            }
                        }
                        temp = temp->next;
                    }
                    SymbolNode* tmp = new SymbolNode;//Neu nhu chua xuat thi bat dau nhan data vao
                    tmp->data.indentify = key2;
                    tmp->data.level = global_level;
                    tmp->data.type = key3;
                    tmp->next = NULL;
                    
                    if(again) tmp->redeclared=true;// Neu co xuat hien roi thi tang len theo block
                    
                    temp = MainTable.head;//Do temp o phia tren da bi thay doi
                    while (temp->next != NULL) {////Duyet Linklist
                        temp = temp->next;
                        
                    }
                    temp->next = tmp;
                    

                }

                cout << "success" << endl;///Neu Insert duoc


            }


            ////////ASSIGN 

            if (key1 == "ASSIGN") {
                if (MainTable.head == NULL) throw Undeclared(str);//SymbolNode chua khoi tao
                else {///Neu Linklist da khoi tao
                    SymbolNode* temp = MainTable.head;
                    bool exist = false;
                    while (temp != NULL) {///Kiem tra trong block nay co iden can ASSIGN hay khong?
                        
                        if (temp->data.indentify == key2) {
                            if (temp->redeclared) {//Neu da khai bao > 1 lan
                                if (temp->data.level == MainTable.LOOKUPLargest(temp->data.indentify)) {//Neu tim thay o block co level lon nhat
                                    exist = true;
                                    break;
                                }
                            }
                            else {//Neu chua khai bao lan nao
                                    exist = true;
                                    break;
                                }
                        
                        }
                        
                    
                        temp = temp->next;
                    }
                    if (exist==false) throw Undeclared(str);//Neu khong tim thay de assign
                    else {
                        ////Kiem tra key3 thuoc dang nao COnstdigi,Constring hay Iden
                        if (checkQuote(key3)) {///Kiem tra co dau '' trong VALUE
                            if (temp->data.type == "string") {
                                if (checkConstString(deleteQuote(key3))) {
                                    cout << "success" << endl;
                                    //delete temp;
                                }
                                else throw TypeMismatch(str);
                            }
                            else throw TypeMismatch(str);
                        }
                        else if (checkConstDigi(key3)) {//Kiem tra VALUE co phai toan bo la digit

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
                            if(checkConstDigi(key3.substr(0,1))) throw TypeMismatch(str);///Neu Khong phai number ma co chua trong 
                            else {///Truong hop VALUE la identifer
                                SymbolNode* tmp = MainTable.head;
                                bool exits2 = false;
                                while (tmp != NULL)
                                {
                                    if (tmp->data.indentify == key3) {
                                        if (temp->data.type == tmp->data.type)
                                        {
                                            cout << "success" << endl;
                                            exits2 = true;
                                            
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
                global_level++;//level tang len;
                

            }
            ///END
            if (key1 == "END") {
                SymbolNode* temp = MainTable.head;
                while (temp != NULL) {///Xoa het nhung identifier cua Block bi dong
                    
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
                if (global_level >= 0) {///Neu global level <0 thi la block chua Open ma da CLOSE
                }
                else throw UnknownBlock();
            }
            ///LOOKUP
            if (key1 == "LOOKUP") {
                
                if (MainTable.head == NULL) throw Undeclared(str);//SymbolNode chua khoi tao
                else {
                    int largestLevel = 0,iniLevel=0;///Tim block co level lon nhat cua iden
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
                        }
                        temp = temp->next;
                    }
                    if (found) cout << largestLevel << endl;
                    else throw Undeclared(str);
                }
            }
            ///PRINT
            if (key1 == "PRINT") {
                if (MainTable.head!=NULL) {
                    cout << MainTable.PRINT(global_level);
                    cout << endl;
                }
            }
            ///RPRINT
            if (key1 == "RPRINT") {
                if (MainTable.head != NULL) {
                    MainTable.head = MainTable.REVERSE();
                    cout << MainTable.PRINT(global_level);
                    cout << endl;
                    MainTable.head = MainTable.REVERSE();
                }
            }
           


        }
        
    }
    if (global_level > 0) throw UnclosedBlock(global_level);
    
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
    
        if (str[0]==39) return true;
    
    return false;
}
string deleteQuote(string str) {
    return str.substr(1, str.length() - 2);
}
