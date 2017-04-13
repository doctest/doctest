#include <vector>
#include <iostream>
#include <stdexcept>
using namespace std;

#define CAT_IMPL(s1, s2) s1##s2
#define CAT(s1, s2) CAT_IMPL(s1, s2)
#define ANONYMOUS(x) CAT(x, __COUNTER__)

struct ITranslator {
    virtual bool translate(string&) = 0;
};

template<typename T>
struct Translator : ITranslator {
    Translator(string(*func)(T)) : m_func(func) {}

    bool translate(string& res) {
        try {
            throw;
        } catch(T ex) {
            res = m_func(ex);
            return true;
        } catch(...) {
            return false;
        }
    }

    string(*m_func)(T);
};

void regTranslatorImpl(ITranslator* t); // fwd decl

template<typename T>
int regTranslator(string(*func)(T)) {
    static Translator<T> t(func);
    regTranslatorImpl(&t);
    return 0;
}

#define REG_TRANSLATOR_2(name, type)                     \
    static string name(type);                            \
    static int ANONYMOUS(TRANS_) = regTranslator(&name); \
    static string name(type)

#define REG_TRANSLATOR(type) REG_TRANSLATOR_2(ANONYMOUS(TRANS_), type)

// impl

vector<ITranslator*> translators;

void regTranslatorImpl(ITranslator* t) {   
    translators.push_back(t);
}

string translate() {
    // try translators
    string res;
    for(size_t i = 0; i < translators.size(); ++i)
        if(translators[i]->translate(res))
            return res;
    // proceed with default translation
    try {
        throw;
    } catch(exception& ex) {
        return ex.what();
    } catch(string& msg) {
        return msg;
    } catch(const char* msg) {
        return msg;
    } catch(...) {
        return "Unknown exception!";
    }
}

// usage

REG_TRANSLATOR(double& e) {
    return string("double: ") + to_string(e);
}

int main() {
    try {
        //throw 42;
        throw 5.0;
        //throw runtime_error("whops!");
    } catch(...) {
        cout << translate() << endl;
    }
}
