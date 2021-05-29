#include <iostream>
#include <fstream>

using namespace std;

unsigned int lenght = 0; //rozmiar

struct Node{ //pojedynczy element listy
    int val; //wartość
    Node* next; //wskaźnik na następny element
};

bool isEmpty(Node*head) {
    if(head == nullptr)
        return true;
    else
        return false;
}

void insert_first(Node*&head, Node*&last, int number) {
    Node* temp = new Node; //Tworzenie nowego elementu, nadanie wartości
    temp->val = number;
    temp->next = nullptr;
    head = temp; //dla jednego elementu, początek i koniec będą sobie równe
    last = temp;
    lenght++;
}

void insert_head(Node*&head, Node*&last, int number) {
    if(isEmpty(head)) {
        insert_first(head,last,number);//Dla nowej listy
    }
    else { //Dla listy już istniejącej
        Node* temp = new Node;
        temp->val = number;
        temp->next = nullptr;
        head->next = temp; // poprzednia "głowa" listy wskazuje teraz na nowy element, który staje się nową "głową"
        head = temp;
        lenght++;
    }

}

void insert_last(Node*&head, Node*&last, int number) { //analogicznie do funkcji wyżej
    if(isEmpty(head)) {
        insert_first(head,last,number);
    }
    else {
        Node* temp = new Node;
        temp->val = number;
        temp->next = last;
        last = temp;
        lenght++;
    }

}

void remove_last(Node*&head, Node*&last) {
    if(isEmpty(head))
        cout<<"Lista juz jest pusta\n";
    else if(head == last) { // wyzerowanie obu wskaźników, przy wyzerowaniu jednego następuje desynchronizacja danych
        head = nullptr;
        last = nullptr;
        lenght--;
    }
    else {
        Node* temp = last;
        last = last->next;
        delete temp; // elementy są tworzone dynamicznie, tak więc dobrze jest zwolnić po nich niepotrzebną już pamięć
        lenght--;
    }

}

void remove_head(Node*&head, Node*&last) {
    if(isEmpty(head))
        cout<<"Lista juz jest pusta\n";
    else if(head == last) {
        head = nullptr;
        last = nullptr;
        lenght--;
    }
    else {
        Node* temp = last;
        while(temp->next != head) { //Brak wskaźników do poprzednich elementów(1-way list), więc trzeba przejść od
            temp = temp->next; // początku do 2 elementu, aby usunąć "głowę"
        }
        temp->next = nullptr;
        temp = head;
        delete temp;
        lenght--;
    }

}

void print(Node*current){
     if(isEmpty(current)) {
        cout<<"Lista jest pusta :(";
     }
     else {
         cout<<"Oto kolejne elementy listy: ";
         while(current != nullptr) {
             cout<<current->val<<" ";
             current = current->next;
         }
     }
     cout<<endl<<"Dlugosc listy wynosi: "<<lenght<<endl;
 }

unsigned int menu() {
    unsigned int choice;
    cout<<"Wybierz, co chcesz zrobic z lista:\n 1. Dodaj element na koniec\n 2. Dodaj element na poczatek\n "
          "3. Umiesc element w wybranym miejscu\n 4. Usun pierwszy element\n 5. Usun ostatni \n 6. Wypisz\n "
          "7. Posortuj liste\n 8. Zapisz do pliku w trybie tekstowym\n 9. Zapisz w trybie binarnym\n "
          "10. Odczyt tekstowy z pliku\n 11. Odczyt binarny z pliku\n 12. Koniec programu\n";

    cin>>choice;
    return choice; //zwrot wyboru, przydatny dla instrukcji switch
 }

void splitList(Node*& head, Node*& front, Node*& back) //podział jednej większej listy na 2 mniejsze o możliwie równych
{ //rozmiarach
    Node* fast; //niedoszły wskaźnik na "głowę"
    Node* slow; //niedoszły wskaźnik na środkowy element
    slow = head;

    fast = head->next;

    while (fast)
    {
        fast = fast->next;
        if (fast)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    front = head;
    back = slow->next;
    //back->prev = NULL;
    slow->next = nullptr; //rozdzielenie obu części
}

Node* merge(Node*& a, Node*& b)
{
    Node* result = nullptr;
    if (!a) return (b); //dla list 1-elementowych zwracamy dany element
    else if (!b) return (a);

    if (a->val <= b->val) //dla 2-elementowych, porównujemy wartości obu i na ich podstawie decydujemy który jest najpierw
    {
        result = a;
        result->next = merge(a->next, b);
    }
    else
    {
        result = b;
        result->next = merge(a, b->next);
    }
    return result; // zwracamy rezultat, czyli naszą nową "głowę" po wykonaniu wszystkich wywołań
}

 void sorting(Node*&head) {
     Node* a;
     Node* b;

     if (!head || !head->next)
         return;
     splitList(head, a, b); //podział na pół listy o co najmniej 2 elementach

     sorting(a); //wywołania rekurencyjne do momentu, gdy zostaniemy z mozliwie mało liczebnymi listami
     sorting(b);

     head = merge(a, b); //sklejanie poszczególnych mniejszych list z powrotem
 }

 void place_at_given_pos(Node*&head, Node*&last, unsigned int position, int number) {
    if(position >= lenght) //przypadki skrajne
        insert_last(head, last, number);
    else if(position == 0)
        insert_head(head, last, number);
    else {
        Node* temp = new Node;
        Node* temp2 = new Node;
        Node* temp3 = new Node;
        temp->next = last;
        temp2->next = last;

        for(int q=0; q<position-1; q++) {
            temp = temp->next;
            temp2 = temp2->next;
        }
        temp2 = temp2->next;
        temp->next = temp3;
        temp3->val = number;
        temp3->next = temp2;
        lenght++;
    }
 }

void fail() { //prosta funkcja obsługująca potencjalne złe dane wprowadzone przez użytkownika
    cin.clear();
    cin.ignore(1000, '\n');
    cout<<"Blad strumienia wejscia\n";
}

int is_correct(const string &s) {
    cout<<s;
    int number;
    cin>>number;
    if(cin.fail()) {
        fail();
        return 0;
    }
    return number;
}

int save_text(Node*&head) { // zapis listy do pliku tekstowego w trybie tekstowym
    string path_name;
    cout<<"Podaj sciezke do pliku:\n";
    cin>>path_name;
    if(cin.fail()) {
        fail();
        return 0;
    }

    ofstream plik(path_name, ios::out | ios::trunc); //otworzenie pustego pliku; dla zapisu przy kolejnych uruchomieniach
    //wystarczy dodać ios::app zamiast ios::trunc
    if(!plik.good()) { //sprawdzenie poprawności ścieżki
        cout << "Bledna sciezka do pliku\n";
        return 0;
    }
    unsigned int counting = 1;
    while(head) {//wpisanie kolejnych wartości
        plik << "Wartosc " << counting << " : " << head->val << endl;
        head = head->next;
        counting++;
    }

    return 1;
}

int save_binary(Node*&head) { // zapis listy do pliku tekstowego w trybie binarnym
    string path_name;
    cout<<"Podaj sciezke do pliku:\n";
    cin>>path_name;
    if(cin.fail()) {
        fail();
        return 0;
    }

    ofstream plik(path_name, ios::out | ios::trunc | ios::binary); //widoczny dzięki zapisowi ios::binary
    if(!plik.good()) {
        cout << "Bledna sciezka do pliku\n";
        return 0;
    }

    string txt0 = "Wartosc ", txt1 = " : ", txt2 = "\n"; // zmienne potrzebne do korzystania z funkcji write
    unsigned int counting = 1, s0 = txt0.size(), s1 = txt1.size(), s2 = txt2.size();
    int val_to_save;
    while(head) {
        val_to_save = head->val;

        plik.write((char*)&s0, sizeof(int)); //pozyskanie długości tekstu + zapis na dysku
        plik.write((char*)txt0.data(), sizeof(char)*s0); //zapis samego tekstu o danej długości

        plik.write((char*)&counting, sizeof(counting));//zapis dla liczb

        plik.write((char*)&s1, sizeof(int));
        plik.write((char*)txt1.data(), sizeof(char)*s1);

        plik.write((char*)&val_to_save, sizeof(val_to_save));

        plik.write((char*)&s2, sizeof(int));
        plik.write((char*)txt2.data(), sizeof(char)*s2);

        head = head->next;
        counting++;
    }

    return 1;
}

int read_text() { //wypisywanie na ekran zawartości pliku w trybie tekstowym
    string path_name;
    cout<<"Podaj sciezke do pliku:\n";
    cin>>path_name;
    if(cin.fail()) {
        fail();
        return 0;
    }

    ifstream plik(path_name);
    if(!plik.good()) {
        cout << "Bledna sciezka do pliku\n";
        return 0;
    }
    char c;
    while(plik.get(c)) { //pobieranie znaków z pliku dopóki jest co pobierać
        cout<<c;
    }

    return 1;
}

string read_b(ifstream &plik) {
    int i;
    string s;
    plik.read((char*)&i, sizeof(int));
    for(int q=0; q<i; q++) {
        s += plik.get();
    }
    return s;
}

int read_binary() { //wypisywanie na ekran zawartości pliku w trybie binarnym
    string path_name;
    cout<<"Podaj sciezke do pliku:\n";
    cin>>path_name;
    if(cin.fail()) {
        fail();
        return 0;
    }

    ifstream plik(path_name, ios::binary | ios::in | ios::ate);
    if(!plik.good()) {
        cout << "Bledna sciezka do pliku\n";
        return 0;
    }
    ifstream::pos_type position = plik.tellg(); //przechowanie wskaźnika czytania, znajdującego się na końcu pliku dzięki ios::ate
    plik.seekg(0); // zmiana jego pozycji na sam początek
    string s1 {}, s2 {}, s3 {};
    int d, n1,n2;
    while(plik.tellg() != position) { //warunek: czytaj dopóki wskaźnik nie będzie na samym końcu pliku
        s1 = read_b(plik); //funkcja pomocnicza dla czytania, zwiekszająca przejrzystość

        plik.read((char*)&n1, sizeof(n1));

        s2 = read_b(plik);

        plik.read((char*)&n2, sizeof(n2));

        s3 = read_b(plik);
        cout<<s1<<n1<<s2<<n2<<s3;
    }

    return 1;
}

int main()
{
    Node *head = nullptr;
    Node *last = nullptr;

    unsigned int choice, pos;
    int number;
    do{ //instrukcja decydująca o tym, jaka akcja będzie wykonywana w danej chwili na podstawie wyboru użytkownika
        choice = menu();
        switch(choice) {
            case 1:
                number = is_correct("Podaj liczbe\n");
                if(number)
                    insert_head(head, last, number);
                break;
            case 2:
                number = is_correct("Podaj liczbe\n");
                if(number)
                    insert_last(head, last, number);
                break;
            case 3:
                number = is_correct("Podaj liczbe\n");
                pos = is_correct("Podaj pozycje\n");
                if(number && pos)
                    place_at_given_pos(head, last, pos, number);
                break;
            case 4:
                remove_head(head,last);
                break;
            case 5:
                remove_last(head, last);
                break;
            case 6:
                print(last);
                break;
            case 7:
                sorting(last);
                break;
            case 8:
                save_text(last);
                break;
            case 9:
                save_binary(last);
                break;
            case 10:
                read_text();
                break;
            case 11:
                read_binary();
                break;
            case 12:
                cout<<"Teraz program zostanie zakonczony";
                return 0;
            default:
                if(cin.fail())
                    fail();
                else
                    cout<<"Nie ma takiej wartosci do wyboru\n";
                break;
        }
    } while(choice != 12);
}