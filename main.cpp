#include <iostream>
#include <vector>

using namespace std;

// Open-ended question
// Question answered with response
class OpenQuestion {
public:
    OpenQuestion() = default;
    OpenQuestion(string statement) : statement(statement) {}

    // Prompts statement (writing it to output), then awaits input.
    // Stores answered response.
    virtual void prompt(ostream& output = cout, istream& input = cin) {
        output << statement << endl;
        std::getline(input, answer);
    }

    string getAnswer() {
        return answer;
    }

    bool isStrict() {
        return strict_answer;
    }

protected:
    string statement;
    string answer;
    // True when answer strict with correct answer.
    bool strict_answer = false;
};

// Closed-ended question
// Question answered with one of provided response
class ClosedQuestion : public OpenQuestion {
public:
    ClosedQuestion() = default;
    ClosedQuestion(string statement, vector<string> options) :
        OpenQuestion(statement), options(options)
    {
        strict_answer = true;
    }

    // Prompts statement (writing it to output), then awaits input.
    // Resets index of selected option to input.
    void prompt(ostream& output = cout, istream& input = cin) override {
        output << statement << endl;
        for (string option : options) {
            output << "\t" << option << endl;
        }
        output << "(Wpisz jedną z powyższych odpowiedzi)> ";
        std::getline(input, answer);

        unsigned i = 0;
        selected_option_i = -1; // default previous selected answer

        // find index of option thats was answered
        for (string option : options) {

            if (option == answer) {
                selected_option_i = i;
                break;
            }
            i++;
        }
    }

    // Returns chosen answer
    string getAnswer() {
        // Unhandled exceptions!
        // One may think... how to fix it... I wonder... ;3
        cout << selected_option_i << endl;
        return options[selected_option_i];
    }

protected:
//    string statement;
    vector<string> options;
    // -1 for not selected or invalid answer
    int selected_option_i = -1;
};


// Collection of questions and correct answers
class Quiz {
public:
    Quiz() = default;
    Quiz(vector<OpenQuestion*> questions, vector<string> correct_answers) :
        questions(questions), correct_answers(correct_answers), answers() {}
    ~Quiz() {
        // clear allocated pointers from vector
        for (OpenQuestion* question : questions) {
            delete question;
        }
        questions.clear();
    }

    // Prompt all questions sequently.
    // Collects answers from each question.
    void promptQuestions(ostream& output = cout, istream& input = cin) {
        output << "========= QUIZ! =========" << endl;
        unsigned i = 0;
        for (OpenQuestion* question : questions) { // it's copying object each time!
            output << (i+1) << ". ";
            question->prompt();
            answers.push_back(question->getAnswer());
            i++;
        }
        output << "=========================" << endl;
    }

    // Compare answers to correct ones.
    // Prints "Correct" on correct choice, otherwise reveals correct answers.
    // For non-strict questions show both answers.
    void showResults(ostream& output = cout) {
        output << "======== RESULTS ========" << endl;
        for (unsigned i=0; i<answers.size(); i++) {
            output << (i+1) << ". ";

            string answer = answers[i]; // it's copying object!
            bool is_strict = questions[i]->isStrict();

            if (!is_strict) {
                output << "Twoja odpowiedź: '" << answer << "'" << endl;
                if (correct_answers[i] != "")
                    output << " - Odpowiedź wzorowa: '" << correct_answers[i] << "'" << endl;
            }
            else if (answer == correct_answers[i]) {
                output << "'" << answer << "' jest poprawną odpowiedzią!" << endl;
            }
            else {
                output << "Niepoprawna odpowiedź ('" << answer << "'). Prawidłowa: "
                    << "'" << correct_answers[i] << "'" << endl;
            }
        }
        output << "=========================" << endl;
    }

private:
    vector<OpenQuestion*> questions;

    // Correct answers for closed and open-ended questions.
    // Empty string element ("") indicates that open-ended question
    // doesn't have suggested correct answer.
    vector<string> correct_answers;

    vector<string> answers;
};


int main() {
    //ClosedQuestion q1("Kto kocha wacki?", {"Johnny Bravo", "Juliusz Słowacki", "Julian Ochocki"});
    //q1.prompt();
    //cout << "Twoja odpowiedź to: " << q1.getAnswer();

    Quiz cpp_quiz(
        {
            new ClosedQuestion("Fragment \"int func();\" jest:",
                               {"definicją funkcji", "deklaracją funkcji"}
                ),
            new OpenQuestion("Definicja funkcji to..."),
            new ClosedQuestion("Fragment \"Point operator*(int rhs);\" wewnątrz klasy Point jest:",
                               {
                                   "wskaźnikiem do funkcji operator",
                                   "deklaracją przeciążenia operatora",
                                   "konstruktorem",
                                   "zmienną typu Point"
                               }
                ),
            new OpenQuestion("Nazwa \"rhs\", często pojawiająca się jako parametr przeciążeń operatorów, to skrót od..."),
            new ClosedQuestion("Stały wskaźnik o nazwie 'ptr' do typu bool ma zapis:",
                               {
                                   "const bool* ptr",
                                   "bool* ptr",
                                   "bool const* ptr",
                                   "bool* const ptr"
                               }
                ),
            new ClosedQuestion("Który symbol jest NIE dozwolonym operatorem przy przeciążaniu operatora",
                               {
                                   "+=",
                                   "!=",
                                   "<<",
                                   "#"
                               }
                ),
            new ClosedQuestion("Operator dereferencji ma symbol",
                               {
                                   "&",
                                   "operator&",
                                   "operator*",
                                   "*"
                               }
                ),
            new ClosedQuestion("Nawiązanie o nazwie 'x_ref' do typu zmiennej int ma zapis:",
                               {
                                   "int* x_ref",
                                   "int x_ref*",
                                   "int &x_ref",
                                   "int x_ref&"
                               }
                ),
            //https://en.cppreference.com/w/cpp/language/access
            new ClosedQuestion("Specyfikator dostępu private:",
                               {
                                   "uniemożliwia dostępu do pól obiektu poza deklaracjami metod owego w klasie",
                                   "umożliwia dostępu do pól obiektu wewnątrz klasy",
                                   "umożliwia dostępu do pól obiektu wewnątrz klas dziedziczących",
                                   "uniemożliwia zmiany pól obiektu, umożliwia wyłącznie ich odczytanie"
                               }
                ),
            //https://en.cppreference.com/w/cpp/string/basic_string/getline
            new ClosedQuestion("Odczytanie linii z otwartego pliku \"ifstream file\" i przypisanie do zmiennej \"string line\" dochodzi w instrukcji:",
                               {
                                   "string line = file.getline();",
                                   "string line; file.getline(line);",
                                   "string line; std::getline(file, line);",
                                   "string line; line.getline(file);"
                               }
                ),
            new ClosedQuestion("Wektor 'x' danego typu T zapisuje się jako:",
                               {
                                   "std::vector<T> x",
                                   "std::vector T x",
                                   "std::vector x(T)",
                                   "T[] x"
                               }
                ),
            new ClosedQuestion("Zmienna jako konstrukcja programistyczna składa się z:",
                               {
                                   "typu i wartości",
                                   "nazwy, adresu i wartości",
                                   "nazwy, typu i wartości",
                                   "nazwy, parametru i wartości zwracanej"
                               }
                ),
            new ClosedQuestion("Przypisanie wyniku referencji zmiennej \"int x\" do wskaźnika 'ptr' następuje w instrukcji:",
                               {
                                   "int& ptr = x;",
                                   "int* ptr &= x;",
                                   "int* ptr = *x;",
                                   "int* ptr = &x;",
                                   "int* ptr = x;"
                               }
                ),
            new ClosedQuestion("Dynamiczne zaalokowanie pamięci dla wskaźnika dochodzi w przypadku:",
                               {
                                   "przypisania wyniku referencji zmiennej",
                                   "zmianie wartości wskaźnika",
                                   "wykorzystania keyworda 'new' (lub malloc)",
                               }
               ),
            //https://www.geeksforgeeks.org/object-slicing-in-c/
            new OpenQuestion( //look at that C++11 raw string literal!!! pretty cool and new:
R"(W fragmencie kodu
```
class Base {
public:
	Base() = default;
	virtual void someMethod() {
		cout << "Hello world from Base class!" << endl;
	}
	int x = 5;
};
class Derived : public Base {
public:
	Derived() = default;
	void someMethod() {
		cout << "Hello world from Derived class!" << endl;
	}
    int x = 4;
    int y = 8;
};
int main() {
    Base b;
    Derived d;
    vector<Base> b_objs = {b, d};

    cout << "Base object size: " << sizeof(b) << endl;
    cout << "Derived object size: " << sizeof(d) << endl;

    for (Base& obj : b_objs) {
        obj.someMethod();
        cout << "x:" << obj.x << endl;
        //cout << "y:" << obj.y << endl; //inaccessible
        cout << "Object size: " << sizeof(obj) << endl;
    }
    return 0;
}
```
zawarty jest problem object slicing'u i dostępu do pól/metod po downcastowaniu. Po wykonaniu powyższego kodu otrzymujemy:
```
Base object size: 16
Derived object size: 24
Hello world from Base class!
x:5
Object size: 16
Hello world from Base class!
x:5
Object size: 16
```
Wielkość obiektu Derived po konwersji do typu Base (wewnątrz Vector'a) zostaje zredukowana z 24 do 16 bajtów - pole 'y' zostaje odcięte.
Dodatkowo wykorzystując typ Base mamy wyłącznie dostęp do zdeklarowanych wewnątrz klasy Base metod, stąd ten sam wynik metody 'someMethod()'.
Jakie jest możliwe rozwiązanie, by umożliwić poprawny dostęp do metod 'someMethod()' z poziomu elementów Vector'a?)")

            //new OpenQuestion("Wymień siedem typów prostych C++:")
        },
        {
        "deklaracją funkcji",
        "Opisanie działania (ciała) funkcji o danej nazwie, przyjmowanych parametrach oraz zwracanym typie.",
        "deklaracją przeciążenia operatora",
        "right hand side",
        "bool* const ptr",
        "#",
        "*",
        "int &x_ref",
        "umożliwia dostępu do pól obiektu wewnątrz klasy",
        "string line; std::getline(file, line);",
        "std::vector<T> x",
        "nazwy, adresu i wartości",
        "int* ptr = &x;",
        "wykorzystania keyworda 'new' (lub malloc)",

        "Wykorzystując wskaźnik typu w wektorze: Zastąpując typ Base wskaźnikiem do typu Base, każdy element będzie miał taką "
        "samą wielkość (wskaźnik ma stałą długość dla każdego typu), zarazem utworzone obiekty nie tracą informacji przez "
        "downcastowanie - jedynie podaje się ich adresy. Informacja o wykorzystywanej metodzie wirtualnej jest zachowywana w "
        "obiektach - odpowiednie metody zostaną wywołane."

        }
    );
    cpp_quiz.promptQuestions();
    cpp_quiz.showResults();

    return 0;
}
