#include <vector>
#include <utility>
#include <iostream>
#include <string>
#include <list>
#include <new>

#define CAT_IMPL(s1, s2) s1##s2
#define CAT(s1, s2) CAT_IMPL(s1, s2)
#define ANONYMOUS(x) CAT(x, __COUNTER__)

#define NUM_CAPTURES_ON_STACK 5

struct ICapture { virtual std::string toString() const = 0; };

void addToCaptures(const ICapture* ptr);
void popFromCaptures();
void printCaptures();

struct InfoBuilder {
    template<typename T>
    struct Capture : ICapture {
        const T* capture;

        Capture(const T* in) : capture(in) {}
        std::string toString() const override { return std::to_string(*capture); }
    };

    struct Chunk { char buf[sizeof(Capture<char>)]; };

    Chunk stackChunks[NUM_CAPTURES_ON_STACK];
    int numCaptures = 0;
    std::list<Chunk> heapChunks;

    template<typename T>
    InfoBuilder& operator<<(const T& in) {
        if(numCaptures < NUM_CAPTURES_ON_STACK) {
            addToCaptures(new (stackChunks[numCaptures].buf) Capture<T>(&in));
        } else {
            heapChunks.push_back(Chunk());
            addToCaptures(new (heapChunks.back().buf) Capture<T>(&in));
        }
        ++numCaptures;
        return *this;
    }

    ~InfoBuilder() {
        for(int i = 0; i < numCaptures; ++i)
            popFromCaptures();
    }

    template<typename T>
    InfoBuilder& operator<<(const T&&) = delete; // prevent rvalues from being captured
};

#define INFO_IMPL(name, x) InfoBuilder name; name << x
#define INFO(x) INFO_IMPL(ANONYMOUS(_CAPTURE_), x)

// impl

std::vector<const ICapture*> captures;

void addToCaptures(const ICapture* ptr) { captures.push_back(ptr); }

void popFromCaptures() { captures.pop_back(); }

void printCaptures() {
    for(size_t i = 0; i < captures.size(); ++i)
        std::cout << captures[i]->toString() << std::endl;
}

// usage

int main() {
    int var1 = 42;
    int var2 = 43;
    int var3 = 45;
    int var4 = 46;
    int var5 = 47;
    float var6 = 48.f;
    bool var7 = true;

    INFO(var1 << var2 << var3 << var4 << var5 << var6 << var7);

    printCaptures();
}
