/*
THE OUTPUT OF THIS PROGRAM IS THIS:

creating empty vector
+ 2
== size: 2
+ 2
== size: 4

creating empty vector
+ 2
== size: 2
+ 1
== size: 3

creating empty vector
+ 1
== size: 1

*/

#include <iostream>
#include <vector>
#include <set>

using namespace std;

void test();

set<pair<const char*, int> > passed_subtests;
set<int> entered_levels;
int current_level;
bool has_skipped;

struct Subtest {
    Subtest(const char* file, int line)
        : m_entered(false)
        , m_file(file)
        , m_line(line)
    {
        m_entered = false;
        
        // if we have already completed it
        if(passed_subtests.count(pair<const char*, int>(file, line)) != 0)
            return;
        
        // if a Subtest on the same level has already been entered
        if(entered_levels.count(current_level) != 0) {
            has_skipped = true;
            return;
        }
        
        entered_levels.insert(current_level++);
        
        m_entered = true;
    }
    
    ~Subtest() {
        if(m_entered) {
            current_level--;
            // only mark the subtest as passed if no subtests have been skipped
            if(has_skipped == false)
                passed_subtests.insert(pair<const char*, int>(m_file, m_line));
        }
    }
    
    operator bool() const { return m_entered; }
    
    bool m_entered;
    const char* m_file;
    int m_line;
};

int main() {
    passed_subtests.clear();
    do {
        has_skipped = false;
        current_level = 0;
        entered_levels.clear();
        test();
    } while(has_skipped == true);
    
    return 0;
}

#define STR_CONCAT_IMPL(s1, s2) s1##s2
#define STR_CONCAT(s1, s2) STR_CONCAT_IMPL(s1, s2)
#define ANON_VAR STR_CONCAT(anon, __LINE__)

#define subtest(title) if(Subtest(__FILE__, __LINE__))

void test() {
    cout << endl << "creating empty vector" << endl;
    vector<int> data;
    
    subtest("size should grow to 2") {
        cout << "+ 2" << endl;
        data.push_back(42);
        data.push_back(10);
        cout << "== size: " << data.size() << endl;
        
        subtest("size should grow to 4") {
            cout << "+ 2" << endl;
            data.push_back(666);
            data.push_back(100);
            cout << "== size: " << data.size() << endl;
        }
        subtest("size should grow to 3") {
            cout << "+ 1" << endl;
            data.push_back(666);
            cout << "== size: " << data.size() << endl;
        }
    }
    subtest("size should grow to 1") {
        cout << "+ 1" << endl;
        data.push_back(42);
        cout << "== size: " << data.size() << endl;
    }
}

/*
THE OUTPUT OF THIS PROGRAM IS THIS:

creating empty vector
+ 2
== size: 2
+ 2
== size: 4

creating empty vector
+ 2
== size: 2
+ 1
== size: 3

creating empty vector
+ 1
== size: 1

*/
