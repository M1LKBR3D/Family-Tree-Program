//MILKBRED
//FAMILY TREE PROGRAM

#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Person {
	string name;
	string gender = "unknown"; // store as string now
	int age = 0;
	
	Person* father = nullptr;
	Person* mother = nullptr;
	Person* spouse = nullptr;
	
	Person* children[10]; // max 10 children per person
	int childCount = 0;
};

//global variable
Person* allPersons[100]; // max 100 persons
int personCount = 0;

//utilities
void clearScreen() {
	system("cls");
}

void waitForKey(bool clear = true) {
	cout << "\nPress Enter to continue...";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	if (clear) system("cls");
}

string readLine(const string &prompt = "") {
	if (!prompt.empty()) cout << prompt;
	string line;
	getline(cin, line);
	return line;
}


//accept only "male" or "female" (case-insensitive). 
string readGenderFromUser(const string &prompt = "Enter gender (male/female): ") {
	while (true) {
		string gs = readLine(prompt);
		if (gs == "male" || gs == "female") return gs;
		cout << "Invalid gender. Please type exactly 'male' or 'female'.\n";
	}
}

//person management
Person* findPerson(const string &name) {
	for (int i = 0; i < personCount; i++)
		if (allPersons[i]->name == name) return allPersons[i];
	return nullptr;
}

bool existsPerson(const string &name) {
	return findPerson(name) != nullptr;
}

void addPerson() {
	string name = readLine("Enter name: ");
	if (name.empty() || existsPerson(name)) { cout << "Invalid or duplicate name.\n"; return; }
	
	string g = readGenderFromUser();
	
	int age = 0;
	string ageStr = readLine("Enter age: ");
	try { age = stoi(ageStr); } catch(...) { cout << "Invalid age.\n"; return; }
	
	Person* p = new Person;
	p->name = name;
	p->gender = g;
	p->age = age;
	allPersons[personCount++] = p;
	cout << "Person added successfully.\n";
}

void addChild() {
	string fatherName = readLine("Enter father's name: ");
	string motherName = readLine("Enter mother's name: ");
	if (!existsPerson(fatherName) || !existsPerson(motherName)) { cout << "Father or mother does not exist.\n"; return; }
	Person* father = findPerson(fatherName);
	Person* mother = findPerson(motherName);
	
	string childName = readLine("Enter child's name: ");
	if (existsPerson(childName)) { cout << "Child already exists.\n"; return; }
	
	string g = readGenderFromUser("Enter child's gender (male/female): ");
	
	int age = 0;
	string ageStr = readLine("Enter child's age: ");
	try { age = stoi(ageStr); } catch(...) { cout << "Invalid age.\n"; return; }
	
	Person* child = new Person;
	child->name = childName; child->gender = g; child->age = age;
	child->father = father; child->mother = mother;
	
	if (father->childCount < 10) father->children[father->childCount++] = child;
	if (mother->childCount < 10) mother->children[mother->childCount++] = child;
	allPersons[personCount++] = child;
	
	cout << "Child added. Siblings updated.\n";
}

void addSibling() {
	string newSibling = readLine("Enter new sibling's name: ");
	string existingSibling = readLine("Enter existing sibling's name: ");
	if (!existsPerson(existingSibling)) { cout << "Existing sibling does not exist.\n"; return; }
	if (existsPerson(newSibling)) { cout << "New sibling already exists.\n"; return; }
	Person* existing = findPerson(existingSibling);
	if (!existing->father && !existing->mother) { cout << "Existing sibling has unknown parents.\n"; return; }
	
	string g = readGenderFromUser("Enter sibling's gender (male/female): ");
	
	int age = 0;
	string ageStr = readLine("Enter sibling's age: ");
	try { age = stoi(ageStr); } catch(...) { cout << "Invalid age.\n"; return; }
	
	Person* sib = new Person;
	sib->name = newSibling; sib->gender = g; sib->age = age;
	sib->father = existing->father; sib->mother = existing->mother;
	if (sib->father && sib->father->childCount < 10) sib->father->children[sib->father->childCount++] = sib;
	if (sib->mother && sib->mother->childCount < 10) sib->mother->children[sib->mother->childCount++] = sib;
	allPersons[personCount++] = sib;
	cout << "Sibling added.\n";
}

void addSpouse() {
	string n1 = readLine("Enter first person: ");
	string n2 = readLine("Enter second person: ");
	if (!existsPerson(n1) || !existsPerson(n2)) { cout << "Person not found.\n"; return; }
	Person* p1 = findPerson(n1);
	Person* p2 = findPerson(n2);
	if (p1->spouse || p2->spouse) { cout << "One already has a spouse.\n"; return; }
	p1->spouse = p2; p2->spouse = p1;
	cout << "Spouse relation added.\n";
}

//displaying the tree
void printTree(Person* root, string prefix = "", bool last = true) {
	if (!root) return;
	cout << prefix;
	if (!prefix.empty()) cout << (last ? "+-- " : "|-- ");
	cout << root->name << " (" << root->gender << ", " << root->age << ")";
	if (root->spouse) cout << " (Spouse: " << root->spouse->name << ")";
	cout << "\n";
	for (int i = 0; i < root->childCount; i++)
		printTree(root->children[i], prefix + (last ? "    " : "|   "), i + 1 == root->childCount);
}

Person* autoDetectRoot() {
	if (personCount == 0) return nullptr;
	for (int i = 0; i < personCount; i++) {
		Person* p = allPersons[i];
		if (!p->father && !p->mother) return p;
	}
	return allPersons[0];
}

void displayTree() {
	if (personCount == 0) { cout << "Family tree is empty.\n"; return; }
	Person* root = autoDetectRoot();
	cout << "\nFamily Tree:\n\n";
	printTree(root);
}

//info
bool isSibling(Person* a, Person* b) {
	if (!a || !b) return false;
	return (a->father && b->father && a->father == b->father) ||
	(a->mother && b->mother && a->mother == b->mother);
}

bool isDescendant(Person* node, Person* target) {
	if (!node || !target) return false;
	for (int i = 0; i < node->childCount; i++) {
		if (node->children[i] == target) return true;
		if (isDescendant(node->children[i], target)) return true;
	}
	return false;
}

bool isAuntOrUncle(Person* auntUncle, Person* nieceNephew) {
	if (!auntUncle || !nieceNephew) return false;
	Person* parents[2] = {nieceNephew->father, nieceNephew->mother};
	for (int i = 0; i < 2; i++) {
		Person* p = parents[i];
		if (p && isSibling(auntUncle, p)) return true;
	}
	return false;
}

void displayPersonInfo() {
	string name = readLine("Enter person's name: ");
	Person* p = findPerson(name);
	if (!p) { cout << "Person not found.\n"; return; }
	cout << "Name: " << p->name << "\nGender: " << p->gender
	<< "\nAge: " << p->age << "\n";
	cout << "Father: " << (p->father ? p->father->name : "None") << "\n";
	cout << "Mother: " << (p->mother ? p->mother->name : "None") << "\n";
	cout << "Spouse: " << (p->spouse ? p->spouse->name : "None") << "\n";
	
	cout << "Children: ";
	if (p->childCount == 0) cout << "None\n"; else { cout << "\n"; for (int i = 0; i < p->childCount; i++) cout << " - " << p->children[i]->name << "\n"; }
	
	cout << "Siblings: ";
	bool hasSib = false;
	for (int i = 0; i < personCount; i++) {
		Person* q = allPersons[i];
		if (q != p && isSibling(p, q)) { cout << "\n - " << q->name; hasSib = true; }
	}
	if (!hasSib) cout << "None\n"; else cout << "\n";
	
	cout << "Grandchildren: ";
	bool hasGrand = false;
	for (int i = 0; i < p->childCount; i++)
		for (int j = 0; j < p->children[i]->childCount; j++) {
			cout << "\n - " << p->children[i]->children[j]->name;
			hasGrand = true;
		}
	if (!hasGrand) cout << "None\n"; else cout << "\n";
}

void detectIllegalRelations() {
	cout << "\n---- Illegal Relations ----\n";
	bool found = false;
	
	string reportedPairKeys[1000]; 
	int repPairCount = 0;
	
	Person* reportedInbreeding[100];
	int repInbCount = 0;
	Person* reportedMinorParent[100];
	int repMinorCount = 0;
	
	auto makePairKey = [](const string &a, const string &b, const string &reason) -> string {
		if (a < b) return a + "|" + b + "|" + reason;
		return b + "|" + a + "|" + reason;
	};
	
	for (int i = 0; i < personCount; i++) {
		Person* p = allPersons[i];
		if (!p || !p->spouse) continue;
		Person* s = p->spouse;
		if (!s) continue;
		
		if (p->father == s || p->mother == s || s->father == p || s->mother == p) {
			string k = makePairKey(p->name, s->name, "parent-child");
			bool already = false;
			for (int x = 0; x < repPairCount; x++) if (reportedPairKeys[x] == k) { already = true; break; }
			if (!already) { cout << "ILLEGAL: Parent-Child marriage: " << p->name << " & " << s->name << "\n"; reportedPairKeys[repPairCount++] = k; found = true; }
		}
		
		if (isSibling(p, s)) {
			string k = makePairKey(p->name, s->name, "sibling");
			bool already = false;
			for (int x = 0; x < repPairCount; x++) if (reportedPairKeys[x] == k) { already = true; break; }
			if (!already) { cout << "ILLEGAL: Sibling marriage: " << p->name << " & " << s->name << "\n"; reportedPairKeys[repPairCount++] = k; found = true; }
		}
		
		if (isAuntOrUncle(p, s) || isAuntOrUncle(s, p)) {
			string k = makePairKey(p->name, s->name, "aunt-uncle");
			bool already = false;
			for (int x = 0; x < repPairCount; x++) if (reportedPairKeys[x] == k) { already = true; break; }
			if (!already) { cout << "ILLEGAL: Aunt/Uncle marriage: " << p->name << " & " << s->name << "\n"; reportedPairKeys[repPairCount++] = k; found = true; }
		}
		
		if (isDescendant(p, s) || isDescendant(s, p)) {
			string k = makePairKey(p->name, s->name, "descendant");
			bool already = false;
			for (int x = 0; x < repPairCount; x++) if (reportedPairKeys[x] == k) { already = true; break; }
			if (!already) { cout << "ILLEGAL: Descendant marriage: " << p->name << " & " << s->name << "\n"; reportedPairKeys[repPairCount++] = k; found = true; }
		}
		
		if ((p->age < 18 && s->age >= 18) || (s->age < 18 && p->age >= 18)) {
			string k = makePairKey(p->name, s->name, "minor-adult-marriage");
			bool already = false;
			for (int x = 0; x < repPairCount; x++) if (reportedPairKeys[x] == k) { already = true; break; }
			if (!already) { cout << "ILLEGAL: Minor-Adult marriage: " << p->name << " & " << s->name << "\n"; reportedPairKeys[repPairCount++] = k; found = true; }
		}
	}
	
	for (int i = 0; i < personCount; i++) {
		Person* child = allPersons[i];
		if (!child) continue;
		
		Person* f = child->father;
		Person* m = child->mother;
		
		if (f && m && isSibling(f, m)) {
			bool already = false;
			for (int r = 0; r < repInbCount; r++) if (reportedInbreeding[r] == child) { already = true; break; }
			if (!already) {
				cout << "ILLEGAL: Child born from siblings (inbreeding): " << child->name
				<< " (Parents: " << f->name << " & " << m->name << ")\n";
				reportedInbreeding[repInbCount++] = child;
				found = true;
			}
		}
		
		bool fatherMinor = (f && f->age < 18);
		bool motherMinor = (m && m->age < 18);
		bool fatherAdult = (f && f->age >= 18);
		bool motherAdult = (m && m->age >= 18);
		
		bool minorParentIllegal = false;
		if ((fatherMinor && motherAdult) || (motherMinor && fatherAdult)) minorParentIllegal = true;
		if (fatherMinor && motherMinor && f && m) minorParentIllegal = true;
		
		if (minorParentIllegal) {
			bool already = false;
			for (int r = 0; r < repMinorCount; r++) if (reportedMinorParent[r] == child) { already = true; break; }
			if (!already) {
				cout << "ILLEGAL: Minor-Adult parentage (minor parent involved): " << child->name;
				if (f || m) {
					cout << " (Parents: ";
					if (f) cout << f->name; else cout << "?";
					cout << " & ";
					if (m) cout << m->name; else cout << "?";
					cout << ")";
				}
				cout << "\n";
				reportedMinorParent[repMinorCount++] = child;
				found = true;
			}
		}
	}
	
	if (!found) cout << "No illegal relations detected.\n";
}

void editPerson() {
	string name = readLine("Enter the name of the person to edit: ");
	Person* p = findPerson(name);
	if (!p) {
		cout << "Person not found.\n";
		return;
	}
	
	cout << "Editing " << p->name << ":\n";
	
	string newName = readLine("Enter new name (leave empty to keep current): ");
	if (!newName.empty()) {
		if (existsPerson(newName)) {
			cout << "Name already exists. Keeping original name.\n";
		} else {
			p->name = newName;
			cout << "Name updated.\n";
		}
	}
	
	string gs = readLine("Enter new gender (male/female, leave empty to keep current): ");
	if (!gs.empty()) {
		if (gs == "male" || gs == "female") {
			p->gender = gs;
			cout << "Gender updated.\n";
		} else cout << "Invalid gender. Keeping original.\n";
	}
	
	string ageStr = readLine("Enter new age (leave empty to keep current): ");
	if (!ageStr.empty()) {
		try {
			int age = stoi(ageStr);
			p->age = age;
			cout << "Age updated.\n";
		} catch (...) { cout << "Invalid age. Keeping original.\n"; }
	}
	
	string fatherName = readLine("Enter new father's name (leave empty to keep current): ");
	if (!fatherName.empty()) {
		if (!existsPerson(fatherName)) cout << "Father not found. Keeping original.\n";
		else p->father = findPerson(fatherName);
	}
	
	string motherName = readLine("Enter new mother's name (leave empty to keep current): ");
	if (!motherName.empty()) {
		if (!existsPerson(motherName)) cout << "Mother not found. Keeping original.\n";
		else p->mother = findPerson(motherName);
	}
	
	string spouseName = readLine("Enter new spouse's name (leave empty to keep current): ");
	if (!spouseName.empty()) {
		if (!existsPerson(spouseName)) cout << "Spouse not found. Keeping original.\n";
		else {
			Person* sp = findPerson(spouseName);
			if (sp->spouse && sp->spouse != p) cout << "That person already has a spouse. Skipping.\n";
			else {
				if (p->spouse) p->spouse->spouse = nullptr;
				p->spouse = sp;
				sp->spouse = p;
				cout << "Spouse updated.\n";
			}
		}
	}
	
	cout << "Editing completed.\n";
}

//main menu
void showMenu() {
	clearScreen();
	cout << "\n====== FAMILY TREE MENU ======\n";
	cout << "1) Add Person\n2) Add Child\n3) Add Sibling\n4) Add Spouse\n5) Display Tree\n6) Display Person Info\n7) Detect Illegal Relations\n8) Edit Person\n9) Exit\nChoice: ";
}

void art(){
	cout << R"(
            ('-.     _   .-')                                          .-') _   _  .-')     ('-.     ('-.   
           ( OO ).-.( '.( OO )_                                       (  OO) ) ( \( -O )  _(  OO)  _(  OO)  
   ,------./ . --. / ,--.   ,--.) ,-.-')  ,--.       ,--.   ,--.      /     '._ ,------. (,------.(,------. 
('-| _.---'| \-.  \  |   `.'   |  |  |OO) |  |.-')    \  `.'  /       |'--...__)|   /`. ' |  .---' |  .---'
(OO|(_\  .-'-'  |  | |         |  |  |  \ |  | OO ) .-')     /        '--.  .--'|  /  | | |  |     |  |    
/  |  '--.\| |_.'  | |  |'.'|  |  |  |(_/ |  |`-' |(OO  \   /            |  |   |  |_.' |(|  '--. (|  '--. 
\_)|  .--' |  .-.  | |  |   |  | ,|  |_.'(|  '---.' |   /  /\_           |  |   |  .  '.' |  .--'  |  .--'  
  \|  |_)  |  | |  | |  |   |  |(_|  |    |      |  `-./  /.__)          |  |   |  |\  \  |  `---. |  `---. 
   `--'    `--' `--' `--'   `--'  `--'    `------'    `--'               `--'   `--' '--' `------' `------' 

					WELCOME TO THE FAMILY TREE PROGRAM
  
)"; 
}


void bye(){
	cout << R"ART(
.-. .-')                 ('-.        .-. .-')                 ('-.        ,---.,---.      ,-.      
\  ( OO )              _(  OO)       \  ( OO )              _(  OO)       |   ||   |      |  |     
 ;-----.\  ,--.   ,--.(,------.       ;-----.\  ,--.   ,--.(,------.      |   ||   |      '.  '.-. 
 | .-.  |   \  `.'  /  |  .---'       | .-.  |   \  `.'  /  |  .---'      |   ||   |       |  |`-' 
 | '-' /_).-')     /   |  |           | '-' /_).-')     /   |  |          |   ||   |       |  |    
 | .-. `.(OO  \   /   (|  '--.        | .-. `.(OO  \   /   (|  '--.       |  .'|  .'       |  |.-. 
 | |  \  ||   /  /\_   |  .--'        | |  \  ||   /  /\_   |  .--'       `--' `--'       .'  '`-' 
 | '--'  /`-./  /.__)  |  `---.       | '--'  /`-./  /.__)  |  `---.      .--. .--.       |  /     
 `------'   `--'       `------'       `------'   `--'       `------'      '--' '--'       `-'      

 )ART";

}


int main() {
	art();
	waitForKey(false);   // show art, wait, but DON'T clear screen
	
	while (true) {
		showMenu();
		string choice = readLine();
		if (choice.empty()) continue;
		
		int opt = 0;
		try { opt = stoi(choice); } 
		catch(...) { 
			cout << "Invalid choice.\n"; 
			waitForKey(); 
			continue; 
		}
		
		switch (opt) {
			case 1: addPerson(); waitForKey(); break;
			case 2: addChild(); waitForKey(); break;
			case 3: addSibling(); waitForKey(); break;
			case 4: addSpouse(); waitForKey(); break;
			case 5: displayTree(); waitForKey(); break;
			case 6: displayPersonInfo(); waitForKey(); break;
			case 7: detectIllegalRelations(); waitForKey(); break;
			case 8: editPerson(); waitForKey(); break;
			
		case 9:
			bye();            // show ASCII
			waitForKey(false); // DON'T clear after bye()
			for (int i = 0; i < personCount; i++)
				delete allPersons[i];
			return 0;
			
		default:
			cout << "Invalid choice.\n";
			waitForKey();
		}
	}
}

