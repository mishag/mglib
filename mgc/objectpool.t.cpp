#include <mgc_objectpool.h>

#include <string>
#include <iostream>

class Person {
public:
    Person() :
        d_age(0)
    {
    }

    Person(const std::string & name, int age) :
        d_age(age),
        d_name(name)
    {
    }

    std::string & name() { return d_name; }
    const std::string & name() const { return d_name; }

    int & age() { return d_age; }
    const int & age() const { return d_age; }

    friend std::ostream & operator<<(std::ostream & o,
                                     const Person & p);

private:
    int         d_age;
    std::string d_name;
};

class PersonCreator {

public:
    std::unique_ptr<Person> alloc()
    {
        return std::unique_ptr<Person>(new Person());
    }

};

class PersonResetter {
public:
    void reset(std::unique_ptr<Person> & personPtr)
    {
        personPtr->name() = "";
        personPtr->age() = 0;
    }
};

std::ostream & operator<<(std::ostream & o,
                          const Person & p)
{
    o <<  p.name() << ", Age " << p.age();
    return o;
}



int main(int argc, char * argv[])
{
    MG::mgc_ObjectPool<Person> pool;

    std::unique_ptr<Person> p1 = pool.getObject();
    p1->name() = "Misha";
    p1->age() = 34;

    std::cout << "Num Objects = " << pool.numObjects() << "\n";
    std::cout << "Num Available Objects = "
              << pool.numAvailableObjects() << "\n";
    std::cout << "Using object: " << *p1 << "\n";

    std::unique_ptr<Person> p2 = pool.getObject();
    std::cout << "Num Objects = " << pool.numObjects() << "\n";
    std::cout << "Num Available Objects = "
              << pool.numAvailableObjects() << "\n";
    std::cout << "Using 2nd object: " << *p2 << "\n";

    pool.returnObject(p1);
    std::cout << "Num Objects = " << pool.numObjects() << "\n";
    std::cout << "Num Available Objects = "
              << pool.numAvailableObjects() << "\n";

    pool.returnObject(p2);
    std::cout << "Num Objects = " << pool.numObjects() << "\n";
    std::cout << "Num Available Objects = "
              << pool.numAvailableObjects() << "\n";

    p1 = pool.getObject();
    p2 = pool.getObject();

    std::cout << *p1 << "\n";
    std::cout << *p2 << "\n";

}
