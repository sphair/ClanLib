#include <ClanLib/core.h>
#include <ClanLib/application.h>

#include <iostream>

using namespace std;

struct UCThing {
	int x;
	UCThing() : x(0) {cout << "--- UCThing default constructed\n";}
	UCThing(const UCThing& other) : x(other.x) {cout << "--- UCThing copy\n";}
	UCThing(int in) : x(in) {cout << "--- UCThing constructed\n";}
	UCThing& operator=(const UCThing& other) {x = other.x; cout << "--- UCThing assignment\n"; return *this;}
	~UCThing() {cout << "--- UCThing destroyed\n";}
};

struct Thing : public CL_Clonable {
	CL_Clonable* clone() const {return new Thing(*this);}
	
	int x;
	Thing() : x(0) {cout << "--- Thing default constructed\n";}
	Thing(const Thing& other) : x(other.x) {cout << "--- Thing copy\n";}
	Thing(int in) : x(in) {cout << "--- Thing constructed\n";}
	Thing& operator=(const Thing& other) {x = other.x; cout << "--- Thing assignment\n"; return *this;}
	~Thing() {cout << "--- Thing destroyed\n";}
};

void shptr_test()
{
	cout << "--- CL_SharedPtr Test ---\n";
	cout << "Creating CL_SharedPtr<UCThing> a, given x value of 1\n";
	CL_SharedPtr<UCThing> a(new UCThing(1));

	cout << "Creating CL_SharedPtr<UCThing> b, given x value of 2\n";
	CL_SharedPtr<UCThing> b(new UCThing(2));
	
	cout << "a->x is: " << a->x << "\n";
	cout << "b->x is: " << b->x << "\n";
	
	cout << "Assigning 3 to a->x\n";
	a->x = 3;
	
	cout << "a->x is: " << a->x << "\n";
	cout << "b->x is: " << b->x << "\n";
	
	cout << "Assigning b to a\n";
	a = b;
	
	cout << "a->x is: " << a->x << "\n";
	cout << "b->x is: " << b->x << "\n";
	
	cout << "Assigning 18 to b->x\n";
	b->x = 18;
	
	cout << "a->x is: " << a->x << "\n";
	cout << "b->x is: " << b->x << "\n";
	
	cout << "Assigning 44 to a->x\n";
	a->x = 44;
	
	cout << "a->x is: " << a->x << "\n";
	cout << "b->x is: " << b->x << "\n";
	
	cout << "--- Done with CL_SharedPtr test ---\n";
}

void poptr_test()
{
	cout << "\n--- CL_OwningPtr Test ---\n";
	
	cout << "Creating CL_OwningPtr<Thing> a, given x value of 1\n";
	CL_OwningPtr<Thing> a(new Thing(1));
	
	cout << "Creating CL_OwningPtr<Thing> b, given x value of 2\n";
	CL_OwningPtr<Thing> b(new Thing(2));
	
	cout << "a->x is: " << a->x << "\n";
	cout << "b->x is: " << b->x << "\n";
	
	cout << "Assigning 3 to a->x\n";
	a->x = 3;
	
	cout << "a->x is: " << a->x << "\n";
	cout << "b->x is: " << b->x << "\n";
	
	cout << "Assigning b to a\n";
	a = b;
	
	cout << "a->x is: " << a->x << "\n";
	cout << "b->x is: " << b->x << "\n";
	
	cout << "Assigning 18 to b->x\n";
	b->x = 18;
	
	cout << "a->x is: " << a->x << "\n";
	cout << "b->x is: " << b->x << "\n";
	
	cout << "Assigning 44 to a->x\n";
	a->x = 44;

	cout << "a->x is: " << a->x << "\n";
	cout << "b->x is: " << b->x << "\n";
	
	cout << "--- Done with CL_OwningPtr test ---\n";
}

void cowptr_test()
{
	cout << "\n--- CL_LazyCopyPtr Test ---\n";
	
	cout << "Creating CL_LazyCopyPtr<Thing> a, given x value of 1\n";
	CL_LazyCopyPtr<Thing> a(new Thing(1));
	
	cout << "Creating CL_LazyCopyPtr<Thing> b, given x value of 2\n";
	CL_LazyCopyPtr<Thing> b(new Thing(2));
	
	cout << "a.cd().x is: " << a.cd().x << "\n";
	cout << "b.cd().x is: " << b.cd().x << "\n";
	
	cout << "Assigning 3 to a->x\n";
	a->x = 3;
	
	cout << "a.cd().x is: " << a.cd().x << "\n";
	cout << "b.cd().x is: " << b.cd().x << "\n";
	
	cout << "Assigning b to a\n";
	a = b;
	
	cout << "a.cd().x is: " << a.cd().x << "\n";
	cout << "b.cd().x is: " << b.cd().x << "\n";
	
	cout << "Assigning 18 to b->x\n";
	b->x = 18;
	
	cout << "a.cd().x is: " << a.cd().x << "\n";
	cout << "b.cd().x is: " << b.cd().x << "\n";
	
	cout << "Assigning 44 to a->x\n";
	a->x = 44;
	
	cout << "a.cd().x is: " << a.cd().x << "\n";
	cout << "b.cd().x is: " << b.cd().x << "\n";
	
	cout << "--- Done with CL_LazyCopyPtr test ---\n";
}

struct App : public CL_ClanApplication {
	int main(int, char**) {
		shptr_test();
		poptr_test();
		cowptr_test();
		
		return 0;
	}
} app;
