#include <iostream>
#include <vector>
#include <string>
#include <stack>

using namespace std;

enum type_of_lex {
	LEX_NULL,		// 0
	LEX_AND,		// 1
	LEX_BOOL,		// 2
	LEX_ELSE,		// 3
	LEX_FALSE,		// 4
	LEX_FOR,		// 5
	LEX_IF,			// 6
	LEX_INT,		// 7
	LEX_NOT,		// 8
	LEX_OR,			// 9
	LEX_PROGRAM,	// 10
	LEX_READ,		// 11
	LEX_STRING,		// 12
	LEX_TRUE,		// 13
	LEX_WHILE,		// 14
	LEX_WRITE,		// 15
	LEX_FIN,		// 16

	LEX_LBRACE,		// 17
	LEX_RBRACE,		// 18
	LEX_QUOTE,		// 19
	LEX_SEMICOLON,	// 20
	LEX_COMMA,		// 21
	LEX_COLON,		// 22
	LEX_ASSIGN,		// 23
	LEX_PLUS,		// 24
	LEX_MINUS,		// 25
	LEX_MULT,		// 26
	LEX_SLASH,		// 27
	LEX_LPAREN,		// 28
	LEX_RPAREN,		// 29
	LEX_EQ,			// 30
	LEX_GRT,		// 31
	LEX_LSS,		// 32
	LEX_GEQ,		// 33
	LEX_LEQ,		// 34
	LEX_NEQ,		// 35
	LEX_ID,			// 36
	LEX_NUM,		// 37
	LEX_STR_CONST,	// 38

	LEX_UN_MINUS,	// 39
	POLIZ_GO,		// 40
	POLIZ_FGO,		// 41
	POLIZ_LABEL,	// 42
	POLIZ_ADDRESS	// 43
};

class Lex {
	type_of_lex type; //объявление приватного члена данных type типа 
	int value; 
public:
	Lex(type_of_lex _type = LEX_NULL, int _value = 0): type(_type), value(_value) {} // конструктор класса Lex. Принимает два аргумента: _type (тип лексемы) и _value (значение лексемы). Если значения аргументов не указаны при создании объекта класса, то используются значения по умолчанию LEX_NULL и 0 соответственно. 
	//Конструктор инициализирует члены данных type и value значениями _type и _value соответственно.
	type_of_lex get_type() const { return type; } //метод класса get_type(), который возвращает тип лексемы. 
	//Он объявлен как константный метод (const), что означает, что он не изменяет состояние объекта.
	int get_value() const { return value; } //метод класса get_value(), который возвращает значение лексемы. Он также объявлен как константный метод (const).
	friend ostream& operator<<(ostream& s, Lex l) { //перегруженный оператор вывода (operator<<), объявлен как друг класса. Он принимает два аргумента: ссылку на объект ostream (s), который представляет поток вывода, и объект класса Lex (l), который будет выводиться.
		s << '(' << l.type << ',' << l.value << ");"; //выполняет последовательный вывод в поток s. Сначала выводится символ '(', затем значение l.type, затем символ ',', затем значение l.value и, наконец, символ ');'. Результат вывода передается обратно в поток s.
		return s; //возвращает поток s после завершения оператора вывода.
	}
};

class Ident {
	string name;
	bool declare; //объявление приватного булевого члена данных declare, который указывает, был ли идентификатор объявлен.
	type_of_lex type; 
	bool assign;
	int value;

	string str_value;
	bool label;
	int address;
public:
	Ident(const string str = "\0") { //конструктор класса Ident. Принимает один аргумент str, который представляет имя идентификатора. По умолчанию аргумент равен пустой строке "\0". В конструкторе инициализируются члены данных: declare, assign, label устанавливаются в false, идентификатору присваивается значение -1, адреc устанавливается в -1. Если переданное имя str не пустое, то оно присваивается члену данных name.
		declare = false;
		assign = false;
		label = false;
		if (!str.empty()) {
			name = str;
		}
			value = -1;
			address = -1;
	}

	string get_name() { return name; }
	bool get_declare() { return declare; } //метод класса get_declare(), который возвращает значение, указывающее, был ли идентификатор объявлен.
	type_of_lex get_type() { return type; }
	bool get_assign() { return assign; }
	int get_value() { return value; }
	string get_str_value() { return str_value; }
	bool get_label() { return label; }
	int get_address() { return address; }

	void put_name(const string _name) { name = _name; }
	void put_declare() { declare = true; }
	void put_type(type_of_lex _type) { type = _type; }
	void put_assign() { assign = true; }
	void put_value(int _value) { value = _value; }
	void put_value(string _str_value) { str_value = _str_value; }
	void put_label() { label = true; }
	void put_address(int _address) { address = _address; }
};

class Scanner {
	enum state { H, IDENT, NUM, STR, COM, COM_STR, DELIM, NEQ, FIN };
	state CS;
	static string TW[];
	static type_of_lex words[];
	static string TD[];
	static type_of_lex dlms[];
	FILE *fp;
	char c;
	string buf;												// буфер для строки
	int buf_size = 255;										// max размер строки
	int buf_top;											// последний !NULL символ в buf

	void program_file(const string str) {					// open file
		fp = fopen(str.c_str(), "r");
		return;
	}

	void clear() {											// clear buf
		buf_top = 0;
		buf.clear();
		return;
	}

	void add() {											// new elem -> buf
		buf.push_back(c);
		buf_top++;
		return;
	}

	int look(const string &buf, const string list[]) {		// str in buf in the list of lexems? - check
		int i = 0;
		while (!list[i].empty()) {
			if (buf == list[i]) {
				return i + 1;
			}
			i++;
		}
		return 0;
	}

	void gc() {												// new symb
		c = fgetc(fp);
		return;
	}

	void un_gc() {											// return one symb
		ungetc(c, fp);
		return;
	}

	void lex_error(string error) {							// lex_errors analyse
		try {
			throw error;
		}
		catch (string s) {
			string err;
			switch(s[0]) {
				case '\\':
					err = "UNKNOWN ESC SEQ: " + s;
					break;
				case '\"':
					err = "MISSING TERMINATING \" CHARACTER";
					break;
				case '!':
					err = "EXPECTED INITIALIZER BEFORE '" + s + "' token.";
					break;
				case '\'':
					err = "NO MATCH FOR DELIMINATOR " + s;
					break;
			}
			cerr << "LEXICALL ERROR: " << err << endl;
			exit(1);
		}
	}

public:
	Scanner(string program) {
		program_file(program);
		CS = H;
		gc();
	}
	~Scanner() { fclose(fp); }

	Lex get_lex();
};

/*
string Scanner::TW[] = {
	"",
	"program",
	"int",
	"string",
	"bool",
	"true",
	"false",
	"if",
	"else",
	"for",
	"while",
	"read",
	"write",
	"and",
	"not",
	"or",
};

type_of_lex Scanner::words[] = {
	LEX_PROGRAM,
	LEX_INT,
	LEX_STRING,
	LEX_BOOL,
	LEX_TRUE,
	LEX_FALSE,
	LEX_IF,
	LEX_ELSE,
	LEX_FOR,
	LEX_WHILE,
	LEX_READ,
	LEX_WRITE,
	LEX_AND,
	LEX_NOT,
	LEX_OR,
	LEX_FIN
};

string Scanner::TD[] = {
	"(",
	")",
	"{",
	"}",
	"\"",
	",",
	":",
	";",
	"=",
	"+",
	"-",
	"*",
	"/",
	"==",
	">",
	"<",
	">=",
	"<=",
	"!="
};

type_of_lex Scanner::dlms[] = {
	LEX_LPAREN,
	LEX_RPAREN,
	LEX_LBRACE,
	LEX_RBRACE,
	LEX_QUOTE,
	LEX_COMMA,
	LEX_COLON,
	LEX_SEMICOLON,
	LEX_ASSIGN,
	LEX_PLUS,
	LEX_MINUS,
	LEX_MULT,
	LEX_SLASH,
	LEX_EQ,
	LEX_GRT,
	LEX_LSS,
	LEX_GEQ,
	LEX_LEQ,
	LEX_NEQ
};
*/

string Scanner::TW[] = {
	"and",
	"bool",
	"else",
	"false",
	"for",
	"if",
	"int",
	"not",
	"or",
	"program",
	"read",
	"string",
	"true",
	"while",
	"write",
};

type_of_lex Scanner::words[] = {
	LEX_AND,		// 1
	LEX_BOOL,		// 2
	LEX_ELSE,		// 3
	LEX_FALSE,		// 4
	LEX_FOR,		// 5
	LEX_IF,			// 6
	LEX_INT,		// 7
	LEX_NOT,		// 8
	LEX_OR,			// 9
	LEX_PROGRAM,	// 10
	LEX_READ,		// 11
	LEX_STRING,		// 12
	LEX_TRUE,		// 13
	LEX_WHILE,		// 14
	LEX_WRITE,		// 15
	LEX_FIN,		// 16
};

string Scanner::TD[] = {
	"{",
	"}",
	"\"",
	";",
	",",
	":",
	"=",
	"+",
	"-",
	"*",
	"/",
	"(",
	")",
	"==",
	">",
	"<",
	">=",
	"<=",
	"!=",
};

type_of_lex Scanner::dlms[] = {
	LEX_LBRACE,			// 1
	LEX_RBRACE,			// 2
	LEX_QUOTE,			// 3
	LEX_SEMICOLON,		// 4
	LEX_COMMA,			// 5
	LEX_COLON,			// 6
	LEX_ASSIGN,			// 7
	LEX_PLUS,			// 8
	LEX_MINUS,			// 9
	LEX_MULT,			// 10
	LEX_SLASH,			// 11
	LEX_LPAREN,			// 12
	LEX_RPAREN,			// 13
	LEX_EQ,				// 14
	LEX_GRT,			// 15
	LEX_LSS,			// 16
	LEX_GEQ,			// 17
	LEX_LEQ,			// 18
	LEX_NEQ,			// 19
};

vector<Ident> TID;												// Table of Ident
vector<string> TSTRC;											// Table of STR CONST

int put(const string &buf) {									// Find in TID and fill
//	vector<Ident>::iterator iter;
	for (auto iter = TID.begin(); iter != TID.end(); iter++) {
		if (buf == iter->get_name()) {
			return iter - TID.begin();
		}
	}
	TID.push_back(Ident(buf));
	return TID.size() - 1;										// return num of Ident in TID
}

int put_string(const string &buf) {								// To zhe samoe, no s str_const
//	vector<string>::iterator iter;
	for (auto iter = TSTRC.begin(); iter != TSTRC.end(); iter++) {
		if (buf == *iter) {
			return iter - TSTRC.begin();
		}
	}
	TSTRC.push_back(buf);
	return TSTRC.size() - 1;
}

Lex Scanner::get_lex() {
	clear();
	int d;
	int tmp; 										// lexeme value
	do {
		switch(CS) {								// H, IDENT, NUM, STR, COM, COM_STR, DELIM, NEQ, FIN
			case H:
				if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
					gc();
				} else if (isalpha(c)) {
					clear();
					add();
					gc();
					CS = IDENT;
				} else if (isdigit(c)){
					d = c - '0';
					gc();
					CS = NUM;
				} else if (c == '\"') {
					add();
					gc();
					CS = STR;
					tmp = look(buf, TD);
					return Lex(LEX_QUOTE, tmp);
				} else if (c == '/') {
					clear();
					add();
					gc();
					if (c == '*') {
						clear();
						gc();
						CS = COM;
					} else if (c == '/') {
						clear();
						gc();
						CS = COM_STR;
					} else {
						un_gc();					// not comment -> return one symb to file
						c = '/';
						CS = DELIM;
					}
				} else if (c == '!') {
					clear();
					add();
					gc();
					CS = NEQ;
				} else if (c == EOF) {
					CS = FIN;
				} else {
					clear();
					add();
					CS = DELIM;
				}
				break;

			case IDENT:
				tmp = look(buf, TW);
				if (isalpha(c) || isdigit(c)) {
					add();
					gc();
				} else {
					CS = H;
					if (tmp) {												// str in buf == with elem in TW -> return Lex
						return Lex((type_of_lex)tmp, tmp);
					} else {
						tmp = put(buf);										// else put lexeme into TW
						return Lex(LEX_ID, tmp);
					}
				}
				break;

			case NUM:
				if (isdigit(c)) {
					d = d * 10 + c - '0';
					gc();
				} else {
					CS = H;
					return Lex(LEX_NUM, d);
				}
				break;

			case STR:
				clear();
				if (c != '\"') {
					while (c != '\"') {
						if (c == '\\') {
							gc();
							switch(c) {
								case 'n':
									c = '\n';
									add();
									break;

								case 'r':
									c = '\r';
									add();
									break;

								case 't':
									c = '\t';
									add();
									break;

								case '0':
									c = '\0';
									add();
									break;

								case '\\':
									add();
									break;

								case '\'':
									add();
									break;

								case '\"':
									add();
									break;

								case '\n':
									gc();
									while (c == '\t') {
										gc();
									}
									un_gc();
									break;

								default:
									string err = "\\" + string(1, c);
									lex_error(err);
									break;
							}
						} else if (c == '\n') {
							lex_error("\"");
						} else {
							add();
						}
						gc();
					}
					tmp = put_string(buf);
					return Lex(LEX_STR_CONST, tmp);
				} else {
					add();
					gc();
					CS = H;
					tmp = look(buf, TD);
					return Lex(LEX_QUOTE, tmp);
				}
				break;

			case COM:
				add();
				if (c == '*') {
					gc();
					if (c == '/') {
						add();
						gc();
						CS = H;
					} else {
						un_gc();
					}
				}
				if (c == EOF) {
					CS = FIN;
				} else {
					gc();
				}
				break;

			case COM_STR:
				while (c != '\n' && c != EOF) {
					gc();
				}
				if (c == EOF) {
					CS = FIN;
				} else {
					gc();
					CS = H;
				}
				break;

			case NEQ:
				if (c == '=') {
					add();
					gc();
					CS = H;
					tmp = look(buf, TD);
					return Lex(LEX_NEQ, tmp);
				} else {
					lex_error("!");
				}
				break;

			case DELIM:
				char c1;
				c1 = c;
				gc();
				if (c1 == '>' || c1 == '<' || c1 == '=') {
					switch(c) {
						case '=':
							add();
							gc();
							break;

						default:
							break;
					}
				}
				CS = H;
				tmp = look(buf, TD);
				if (tmp) {
					return Lex((type_of_lex)(tmp + (int)LEX_FIN), tmp);
				} else {
					lex_error("'" + buf + "'");
				}
				break;

			case FIN:
				return Lex(LEX_FIN);
				break;
		}
	} while (true);
};

class Parser {
	Lex curr_lex;
	type_of_lex c_type;
	int c_val;
	Scanner scan;
	stack<type_of_lex> st_lex;

	bool lvalue;

	stack<int> st_plus;
	stack<int> st_minus;
	stack<int> st_lvalue_uncertain;
	int num;
	bool pp_id;

/* Syntax */
	void P();						// <программа>			-> program { <операторы> }
	void DESCRS();					// <описания>			-> { <описание>; }
	void DESCR();					// <описание>			-> [int | string | bool] <переменная> { ,<переменная> }
	void VAR();						// <переменная>			-> LEX_ID | LEX_ID = <константа>
	void CONST();					// <константа>			-> INT | STRING | BOOL
	void OPERS();					// <операторы>			-> { <оператор> }
	void OPER();					// <оператор>			-> { <описания> | <оператор-выражение> | {<операторы>} |
									//							 if (<выражение>) <оператор> else <оператор> |
									//					 		 while (<выражение>) <оператор> |
									//							 read (<IDENT>); |
									//							 write (<выражение> { ,<выражение> }); }
	void OPER_EXPR();				// <оператор-выражение>	-> <выражение>;
	void EXPR(int v = 1);			// <выражение>			-> A | A = <выражение> |
									//						   A [== | < | > | <= | >= | !=] A
	void A();						// A					-> B | B [+ | - | or] B
	void B();						// B					-> C | C [* | / | and] C
	void C();						// C					-> IDENT | [+ | -] C | STR | not C | <выражение>

/* Semantic */
	void dec();
	void check_id(int val);
	void check_id_read();
	void check_oper();
	void check_un_oper();
	void check_not();
	void eq_type();
	void eq_bool();

/* into POLIZ */
	void to_poliz();

	void gl() {
		curr_lex = scan.get_lex();
		c_type = curr_lex.get_type();
		c_val = curr_lex.get_value();
	}

	void syn_error(string error) {
		try {
			throw error;
		}
		catch (string s) {
			cerr << "SYNT: " << s << ", LEX:" << curr_lex << ' ' << curr_lex.get_type() << endl;
			exit(1);
		}
	}

	void sem_error(string error) {
		try {
			throw error;
		}
		catch (string s) {
			cerr << "ERROR: " << s << endl;
			exit(1);
		}
	}

public:
	Parser(const string _program): scan(_program) {
		lvalue = 1;
		pp_id = 0;
	}

	vector<Lex> Poliz;

	void poliz_print() {
		cout << "{ ";
		for (auto iter = Poliz.begin(); iter != Poliz.end(); iter++) {
			cout << *iter << " ";
		}
		cout << "}\n";
	}

	void analyse();
};

template <class T1, class T2>
void extract(T1 &st, T2 &item) {
	item = st.top();
	st.pop();
}

void Parser::analyse() {
	gl();
	P();
	if (c_type != LEX_FIN) {
		syn_error("NO FINAL");
	cout << "good" << endl;
	}
}

/* SYNTAX */
void Parser::P() {
	if (c_type == LEX_PROGRAM) {
		gl();
	} else {
		syn_error("NO PROGRAM");
	}
	if (c_type == LEX_LBRACE) {
		gl();
	} else {
		syn_error("NO LBRACE");
	}
	OPERS();
}

void Parser::DESCRS() {
	DESCR();
	if (c_type != LEX_SEMICOLON) {
		syn_error("NO SEMICOLON");
	}
	gl();
}

void Parser::DESCR() {
	type_of_lex ident_type = c_type;
	st_lex.push(ident_type);
	gl();
	VAR();
	while (c_type == LEX_COMMA) {
		st_lex.push(ident_type);
		gl();
		VAR();
	}
}

void Parser::VAR() {
	if (c_type == LEX_ID) {
		dec();
		Poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
		gl();
		if (c_type == LEX_ASSIGN) {
			gl();
			CONST();
			Poliz.push_back(LEX_ASSIGN);
		} else {
			Poliz.pop_back();
		}
		if (c_type == LEX_COMMA || c_type == LEX_SEMICOLON) {
			st_lex.pop();
		} else {
			syn_error("ONLY '='");
		}
	} else {
		syn_error("NO IDENT");
	}
}

void Parser::CONST() {
	if (st_lex.top() == LEX_INT || st_lex.top() == LEX_STRING || st_lex.top() == LEX_BOOL) {
		EXPR(0);
		eq_type();
	} else {
		syn_error("NO CONST TYPE");
	}
}

void Parser::OPERS() {
	while (c_type != LEX_RBRACE) {
		OPER();
		if (c_type == LEX_FIN) {
			syn_error("NO RBRACE");
		}
	}
	gl();
}

void Parser::OPER() {
	int p0, p1, p2, p3, p4;
	switch(c_type) {
		case LEX_INT:
			DESCRS();
			break;

		case LEX_STRING:
			DESCRS();
			break;

		case LEX_BOOL:
			DESCRS();
			break;

		case LEX_IF:
			gl();
			if (c_type != LEX_LPAREN) {
				syn_error("NO LPAREN IN IF");
			}

			gl();
			EXPR();
			eq_bool();
			p2 = Poliz.size();
			Poliz.push_back(Lex());
			Poliz.push_back(Lex(POLIZ_FGO));

			if (c_type != LEX_RPAREN) {
				syn_error("NO RPAREN IN IF");
			}

			gl();
			OPER();
			Poliz[p2] = Lex(POLIZ_LABEL, Poliz.size());

			if (c_type == LEX_ELSE) {
				p3 = Poliz.size();
				Poliz.push_back(Lex());
				Poliz.push_back(Lex(POLIZ_GO));
				Poliz[p2] = Lex(POLIZ_LABEL, Poliz.size());
				gl();
				OPER();
				Poliz[p3] = Lex(POLIZ_LABEL, Poliz.size());
			}
			break;

		case LEX_WHILE:
			p0 = Poliz.size();
			gl();
			if (c_type != LEX_LPAREN) {
				syn_error("NO LPAREN IN WHILE");
			}

			gl();
			EXPR();
			eq_bool();
			p1 = Poliz.size();
			Poliz.push_back(Lex());
			Poliz.push_back(Lex(POLIZ_FGO));

			if (c_type != LEX_RPAREN) {
				syn_error("NO RPAREN IN WHILE");
			}

			gl();
			OPER();
			Poliz.push_back(Lex(POLIZ_LABEL, p0));
			Poliz.push_back(Lex(POLIZ_GO));
			Poliz[p1] = Lex(POLIZ_LABEL, Poliz.size());
			break;

		case LEX_FOR:
			gl();
			if (c_type != LEX_LPAREN) {
				syn_error("NO LPAREN IN FOR");
			}

			gl();
			if(c_type == LEX_SEMICOLON) {
				gl();
			} else if (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOL) {
				DESCRS();
			} else {
				OPER_EXPR();
			}

			p3 = Poliz.size();
			if (c_type == LEX_SEMICOLON) {
				Poliz.push_back(Lex(LEX_TRUE, 1));
				gl();
			} else {
				EXPR();
				eq_bool();
				if (c_type != LEX_SEMICOLON) {
					syn_error("NO SEMICOLON IN FOR");
				}
				gl();
			}

			p1 = Poliz.size();
			Poliz.push_back(Lex());
			Poliz.push_back(Lex(POLIZ_FGO));
			p2 = Poliz.size();
			Poliz.push_back(Lex());
			Poliz.push_back(Lex(POLIZ_GO));
			p4 = Poliz.size();

			if (c_type == LEX_RPAREN) {
				gl();
			} else {
				EXPR();
				Poliz.push_back(Lex(POLIZ_LABEL, p3));
				Poliz.push_back(Lex(POLIZ_GO));
				if (c_type != LEX_RPAREN) {
					syn_error("NO RPAREN IN FOR");
				}
				gl();
			}

			Poliz[p2] = Lex(POLIZ_LABEL, Poliz.size());
			OPER();
			Poliz.push_back(Lex(POLIZ_LABEL, p4));
			Poliz.push_back(Lex(POLIZ_GO));
			Poliz[p1] = Lex(POLIZ_LABEL, Poliz.size());
			break;

		case LEX_READ:
			gl();
			if (c_type != LEX_LPAREN) {
				syn_error("NO LPAREN IN READ");
			}

			gl();
			if (c_type != LEX_ID) {
				syn_error("NO IDENT IN READ");
			}
			check_id_read();
			Poliz.push_back(Lex(POLIZ_ADDRESS, c_val));

			gl();
			if (c_type != LEX_RPAREN) {
				syn_error("NO RPAREN IN READ");
			}

			gl();
			Poliz.push_back(Lex(LEX_READ));
			if (c_type != LEX_SEMICOLON) {
				syn_error("NO SEMICOLON IN READ");
			}
			gl();
			break;

		case LEX_WRITE:
			gl();
			if (c_type != LEX_LPAREN) {
				syn_error("NO LPAREN IN WRITE");
			}

			gl();
			if (c_type == LEX_RPAREN) {
				syn_error("NO IDENT IN WRITE");
			}

			EXPR(0);
			while (c_type == LEX_COMMA) {
				gl();
				EXPR(0);
			}

			if (c_type != LEX_RPAREN) {
				syn_error("NO RPAREN IN WRITE");
			}

			gl();
			Poliz.push_back(Lex(LEX_WRITE));
			if (c_type != LEX_SEMICOLON) {
				syn_error("NO SEMICOLON IN WRITE");
			}

			gl();
			break;

		case LEX_LBRACE:
			gl();
			OPERS();
			break;

		default:
			OPER_EXPR();
			break;
	}
}

void Parser::OPER_EXPR() {
	pp_id = 0;
	EXPR(1);
	if (c_type == LEX_SEMICOLON || c_type == LEX_COLON) {
		gl();
	} else {
		syn_error("NO SEMICOLON");
	}
}

void Parser::EXPR(int v) {
	lvalue = v;
	type_of_lex assign_type = c_type;
	A();

	if (c_type == LEX_ASSIGN) {
		if (assign_type == LEX_ID && lvalue) {
			pp_id = 1;
			int value;
			extract(st_lvalue_uncertain, value);
			Poliz[num] = Lex(POLIZ_ADDRESS, value);
			gl();
			EXPR();
			eq_type();
			Poliz.push_back(LEX_ASSIGN);
			to_poliz();
		} else {
			syn_error("LVALUE");
		}
	} else if (c_type >= LEX_EQ && c_type <= LEX_NEQ) {
		type_of_lex tmp_type = c_type;
		lvalue = 0;
		st_lex.push(c_type);
		gl();
		A();
		check_oper();
		Poliz.push_back(Lex(tmp_type));
	}

	if (!st_lvalue_uncertain.empty()) {
		int value;
		extract(st_lvalue_uncertain, value);
		lvalue ? Poliz[num] = Lex(POLIZ_ADDRESS, value) : Poliz[num] = Lex(LEX_ID, value);
	}

	if (!pp_id) {
		to_poliz();
	}
}

void Parser::A() {
	B();
	while (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR) {
		type_of_lex add_type = c_type;
		lvalue = 0;
		st_lex.push(c_type);
		gl();
		B();
		check_oper();
		Poliz.push_back(Lex(add_type));
	}
}

void Parser::B() {
	C();
	while (c_type == LEX_MULT || c_type == LEX_SLASH || c_type == LEX_AND) {
		type_of_lex mul_type = c_type;
		lvalue = 0;
		st_lex.push(c_type);
		gl();
		C();
		check_oper();
		Poliz.push_back(Lex(mul_type));
	}
}

void Parser::C() {
	switch(c_type) {
		case LEX_ID:
			if (lvalue) {
				st_lvalue_uncertain.push(c_val);
				num = Poliz.size();
				Poliz.push_back(Lex());
			} else {
				Poliz.push_back(Lex(LEX_ID, c_val));
			}

			int id_val;
			id_val = c_val;
			gl();
			if (c_type == LEX_COLON) {
				if (!pp_id) {
					if (lvalue) {
						st_lvalue_uncertain.pop();
					}
					Poliz.pop_back();

					if (TID[id_val].get_label()) {
						int pos = TID[id_val].get_address();
						if (TID[id_val].get_value() != -1) {
							syn_error("LABEL IS DECLARED TWICE: " + TID[id_val].get_name());
						}
						TID[id_val].put_value(Poliz.size());
						TID[id_val].put_assign();
						Poliz[pos] = Lex(POLIZ_LABEL, Poliz.size());
					} else if (!TID[id_val].get_declare()) {
						TID[id_val].put_label();
						TID[id_val].put_value(Poliz.size());
						TID[id_val].put_assign();
					} else {
						syn_error("LABEL IS ALREADY DECLARED AS AN IDENT: " + TID[id_val].get_name());
					}
				} else {
					syn_error("WRONG USAGE OF LABEL: " + TID[id_val].get_name());
				}
			}

			check_id(id_val);
			break;

		case LEX_NUM:
			st_lex.push(LEX_INT);
			Poliz.push_back(curr_lex);
			gl();
			break;

		case LEX_PLUS:
			lvalue = 0;
			gl();
			C();
			check_un_oper();
			break;

		case LEX_MINUS:
			lvalue = 0;
			gl();
			C();
			check_un_oper();
			Poliz.push_back(Lex(LEX_UN_MINUS));
			break;

		case LEX_QUOTE:
			gl();
			st_lex.push(LEX_STRING);
			Poliz.push_back(curr_lex);
			if (c_type != LEX_STR_CONST) {
				syn_error("NO STR_CONST");
			}
			gl();
			gl();
			break;

		case LEX_TRUE: case LEX_FALSE:
			st_lex.push(LEX_BOOL);
			c_type == LEX_TRUE ? Poliz.push_back(Lex(LEX_TRUE, 1)) : Poliz.push_back(Lex(LEX_FALSE, 0));
			gl();
			break;

		case LEX_NOT:
			lvalue = 0;
			gl();
			C();
			check_not();
			Poliz.push_back(Lex(LEX_NOT));
			break;

		case LEX_LPAREN:
			gl();
			EXPR(0);
			if (c_type != LEX_RPAREN) {
				syn_error("NO RPAREN IN C_LEX_LPAREN");
			}
			gl();
			break;

		default:
			syn_error("NO OPERAND");
			break;
	}
}

/* Semantic */
void Parser::dec() {
	if (TID[c_val].get_declare()) {
		sem_error("VARIABLE IS DECLARED TWICE: " + TID[c_val].get_name());
	} else {
		TID[c_val].put_type(st_lex.top());
		TID[c_val].put_declare();
	}
}

void Parser::check_id(int value) {
	if (TID[value].get_declare()) {
		st_lex.push(TID[value].get_type());
	} else {
		sem_error("VARIABLE NOT DECLARED: " + TID[value].get_name());
	}
}

void Parser::check_id_read() {
	if (!TID[c_val].get_declare()) {
		sem_error("IN READ - VARIABLE NOT DECLARED: " + TID[c_val].get_name());
	}
}

void Parser::check_oper() {
	type_of_lex op, op1, op2;
	type_of_lex op_type, res_type;
	extract(st_lex, op2);
	extract(st_lex, op);
	extract(st_lex, op1);

	if (op1 == LEX_STRING && op1 == op2) {
		op_type = LEX_STRING;
		if (op == LEX_PLUS) {
			res_type = LEX_STRING;
		} else if (op == LEX_EQ || op == LEX_NEQ || op == LEX_LSS || op == LEX_GRT) {
			res_type = LEX_BOOL;
		} else {
			sem_error("UNALLOWED OPERATOR FOR VARIABLES OF TYPES");
		}
	} else {
		if (op >= LEX_EQ && op <= LEX_NEQ) {
			op_type = LEX_INT;
			res_type = LEX_BOOL;
		} else if (op >= LEX_PLUS && op <= LEX_SLASH) {
			op_type = LEX_INT;
			res_type = LEX_INT;
		} else if (op == LEX_OR || op == LEX_AND) {
			op_type = LEX_BOOL;
			res_type = LEX_BOOL;
		}
	}

	if ((op1 == op2 && op1 == op_type) || (op_type == LEX_BOOL && op1 != LEX_STRING && op2 != LEX_STRING)) {
		st_lex.push(res_type);
	} else {
		sem_error("TYPES DON'T MATCH");
	}
}

void Parser::check_un_oper() {
	if (st_lex.top() != LEX_INT) {
		sem_error("WRONG TYPE FOR UNARY OPERATION");
	}
}

void Parser::check_not() {
	type_of_lex type = st_lex.top();
	if (type != LEX_BOOL) {
		sem_error("WRONG TYPE FOR NOT");
	}
}

void Parser::eq_type() {
	type_of_lex oper;
	extract(st_lex, oper);
	if (st_lex.top() != oper && (st_lex.top() != LEX_BOOL || oper != LEX_INT)) {
		sem_error("TYPES DON'T MATCH IN EQ_TYPE");
	}
}

void Parser::eq_bool() {
	if (st_lex.top() == LEX_BOOL) {
		st_lex.pop();
	} else {
		sem_error("NOT BOOLEAN");
	}
}

/* CONVERTATION TO POLIZ */
void Parser::to_poliz() {
	int value;
	while (!st_plus.empty()) {
		extract(st_plus, value);
		Poliz.push_back(Lex(POLIZ_ADDRESS, value));
		Poliz.push_back(Lex(LEX_ID, value));
		Poliz.push_back(Lex(LEX_NUM, 1));
		Poliz.push_back(Lex(LEX_PLUS));
		Poliz.push_back(Lex(LEX_ASSIGN));
	}
	while (!st_minus.empty()) {
		extract(st_minus, value);
		Poliz.push_back(Lex(POLIZ_ADDRESS, value));
		Poliz.push_back(Lex(LEX_ID, value));
		Poliz.push_back(Lex(LEX_NUM, 1));
		Poliz.push_back(Lex(LEX_MINUS));
		Poliz.push_back(Lex(LEX_ASSIGN));
	}
}


/* Execution */
class Executer {
	Lex curr;
	stack<int> args;
	stack<string> str_consts;
	stack<type_of_lex> types;

	void exec_error(string error) {
		cerr << "EXECUTION FAILED: " << error << endl;
		exit(1);
	}

public:
	void execute(vector<Lex> &poliz);
	void write();
};

void Executer::write() {
	if (!types.empty()) {
		type_of_lex tmp;
		int v;
		string s;
		extract(types, tmp);
		switch(tmp) {
			case LEX_STRING:
				extract(str_consts, s);
				break;

			case LEX_INT:
				extract(args, v);
				break;

			case LEX_BOOL:
				extract(args, v);
				break;

			default:
				break;
		}

		write();
		switch(tmp) {
			case LEX_STRING:
				cout << s;
				break;
			case LEX_BOOL:
				v ? cout << "true" : cout << "false";
  				break;

			case LEX_INT:
				cout << v;
				break;

			default:
				break;
		}
	}
}

void Executer::execute(vector<Lex> &poliz) {
	int i, j;
	string a, b;
	int index = 0;
	int size = poliz.size();

	cout << "BEGINNING EXECUTION... " << endl;

	while (index < size) {
		curr = poliz[index];
		switch(curr.get_type()) {
			case POLIZ_LABEL:
				args.push(curr.get_value());
				break;

			case POLIZ_ADDRESS:
				args.push(curr.get_value());
				types.push(TID[curr.get_value()].get_type());
				break;

			case LEX_NUM:
				args.push(curr.get_value());
				types.push(LEX_INT);
				break;

			case LEX_TRUE: case LEX_FALSE:
				args.push(curr.get_value());
				types.push(LEX_BOOL);
				break;

			case LEX_STR_CONST:
				str_consts.push(TSTRC[curr.get_value()]);
				types.push(LEX_STRING);
				break;

			case LEX_ID:
				i = curr.get_value();
				if (TID[i].get_assign()) {
					types.push(TID[i].get_type());
					if (TID[i].get_type() == LEX_STRING) {
						str_consts.push(TID[i].get_str_value());
					} else {
						args.push(TID[i].get_value());
					}
				} else {
					exec_error("IDENT DOESNT HAVE A VALUE: " + TID[i].get_name());
				}
				break;

			case LEX_NOT:
				extract(args, i);
				args.push(!i);
				break;

			case LEX_OR:
				extract(args, i);
				extract(args, j);
				args.push(j || i);
				types.pop();
				break;

			case LEX_AND:
				extract(args, i);
				extract(args, j);
				args.push(j && i);
				types.pop();
				break;

			case LEX_PLUS:
				if (types.top() == LEX_STRING) {
					extract(str_consts, a);
					extract(str_consts, b);
					str_consts.push(b + a);
				} else {
					extract(args, i);
					extract(args, j);
					args.push(j + i);
				}
				types.pop();
				break;

			case LEX_MINUS:
				extract(args, i);
				extract(args, j);
				args.push(j - i);
				types.pop();
				break;

			case LEX_MULT:
				extract(args, i);
				extract(args, j);
				args.push(j * i);
				types.pop();
				break;

			case LEX_SLASH:
				extract(args, i);
				extract(args, j);
				types.pop();
				if (i) {
					args.push(j / i);
				} else {
					exec_error("DIVIDE BY ZERO");
				}
				break;

			case LEX_UN_MINUS:
				extract(args, i);
				args.push(-1 * i);
				break;

			case LEX_EQ:
				if (types.top() == LEX_STRING) {
					extract(str_consts, a);
					extract(str_consts, b);
					args.push(b == a);
				} else {
					extract(args, i);
					extract(args, j);
					args.push(j == i);
				}
				types.pop();
				types.pop();
				types.push(LEX_BOOL);
				break;

			case LEX_NEQ:
				if (types.top() == LEX_STRING) {
					extract(str_consts, a);
					extract(str_consts, b);
					args.push(b != a);
				} else {
					extract(args, i);
					extract(args, j);
					args.push(j != i);
				}
				types.pop();
				types.pop();
				types.push(LEX_BOOL);
				break;

			case LEX_LSS:
				if (types.top() == LEX_STRING) {
					extract(str_consts, a);
					extract(str_consts, b);
					args.push(b < a);
				} else {
					extract(args, i);
					extract(args, j);
					args.push(j < i);
				}
				types.pop();
				types.pop();
				types.push(LEX_BOOL);
				break;

			case LEX_GRT:
				if (types.top() == LEX_STRING) {
					extract(str_consts, a);
					extract(str_consts, b);
					args.push(b > a);
				} else {
					extract(args, i);
					extract(args, j);
					args.push(j > i);
				}
				types.pop();
				types.pop();
				types.push(LEX_BOOL);
				break;

			case LEX_LEQ:
				extract(args, i);
				extract(args, j);
				args.push(j <= i);
				types.pop();
				types.pop();
				types.push(LEX_BOOL);
				break;

			case LEX_GEQ:
				extract(args, i);
				extract(args, j);
				args.push(j >= i);
				types.pop();
				types.pop();
				types.push(LEX_BOOL);
				break;

			case LEX_ASSIGN:
				types.pop();
				switch(types.top()) {
					case LEX_STRING:
						extract(str_consts, a);
						extract(args, j);
						TID[j].put_value(a);
						break;

					case LEX_INT:
						extract(args, i);
						extract(args, j);
						TID[j].put_value(i);
						break;

					case LEX_BOOL:
						extract(args, i);
						extract(args, j);
						if (i) {
							i = 1;
						}
						TID[j].put_value(i);
						break;

					default:
						break;
				}
				types.pop();
				TID[j].put_assign();
				break;

			case LEX_READ:
				extract(args, i);
				switch(types.top()) {
					case LEX_STRING:
						cout <<"\nENTER STRING VALUE FOR " << TID[i].get_name() << ": ";
						cin >> a;
						TID[i].put_value(a);
						break;

					case LEX_INT:
						cout << "\nENTER INT VALUE FOR " << TID[i].get_name() << ": ";
						cin >> j;
						TID[i].put_value(j);
						break;

					case LEX_BOOL:
						j = 0;
						cout << "ENTER BOOL VALUE FOR " << TID[i].get_name() << ": ";
						cin >> b;
						if (b == "true" || (isdigit(b[0]) && b[0] - '0') ||
							((b[0] == '+' || b[0] == '-') && isdigit(b[1]) && b[1] - '0')) {
							j = 1;
						}
						TID[i].put_value(j);
						break;

					default:
						break;
				}
				types.pop();
				TID[i].put_assign();
				break;

			case LEX_WRITE:
				write();
				break;

			case POLIZ_GO:
				extract(args, i);
				index = i - 1;
				break;

			case POLIZ_FGO:
				extract(args, i);
				extract(args, j);
				types.pop();
				if (!j) {
					index = i - 1;
				}
				break;

			default:
				break;
		}

		index++;
	}
	cout << "\nEXECUTION COMPLETE\n";
}

/* INTERPRETER */
class Interpreter {
	Parser pars;
	Executer exec;
public:
	Interpreter(const string _prog_name): pars(_prog_name) {}

	void interpret();
};

void Interpreter::interpret() {
	pars.analyse();
	exec.execute(pars.Poliz);
}


/* MAIN MENU */
int main() {
	string program;
	cout << "ENTER program name: ";
	cin >> program;

	Interpreter Interpret(program);
	Interpret.interpret();

	return 0;
}
