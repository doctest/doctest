#include <vector>
#include <utility>
#include <iostream>
#include <stdexcept>
using namespace std;

#define CAT_IMPL(s1, s2) s1##s2
#define CAT(s1, s2) CAT_IMPL(s1, s2)
#define ANONYMOUS(x) CAT(x, __COUNTER__)

struct ITranslator {
    virtual pair<bool, string> translate() const = 0;
};

template<typename T>
struct Translator : ITranslator {
    Translator(string(*func)(T)) : m_func(func) {}

    pair<bool, string> translate() const {
        try {
            throw;
        } catch(T ex) {
            return pair<bool, string>(true, m_func(ex));
        } catch(...) {
            return pair<bool, string>(false, "");
        }
    }

    string(*m_func)(T);
};

void regTranslatorImpl(const ITranslator* translator); // fwd decl

template<typename T>
int regTranslator(string(*func)(T)) {
    static Translator<T> Translator(func);
    regTranslatorImpl(&Translator);
    return 0;
}

#define REG_TRANSLATOR_2(name, type)                     \
    static string name(type);                            \
    static int ANONYMOUS(TRANS_) = regTranslator(&name); \
    static string name(type)

#define REG_TRANSLATOR(type) REG_TRANSLATOR_2(ANONYMOUS(TRANS_), type)

// impl

vector<const ITranslator*> translators;

void regTranslatorImpl(const ITranslator* translator) {   
    translators.push_back(translator);
}

string translate() {
    pair<bool, string> res(false, "");
    // try translators
    for(size_t i = 0; i < translators.size() && res.first == false; ++i)
        res = translators[i]->translate();
    // proceed with default translation
    if(res.first == false) {
        try {
            throw;
        } catch(exception& ex) {
            res.second = ex.what();
        } catch(string& msg) {
            res.second = msg;
        } catch(const char* msg) {
            res.second = msg;
        } catch(...) {
            res.second = "Unknown exception!";
        }
    }
    return res.second;
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
