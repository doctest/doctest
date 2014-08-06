#include "doctest.h"

doctest{ cout << "anon 1\n"; }
doctest{ cout << "anon 2\n"; }
doctest{ cout << "anon 3\n"; }

#include "crap.h"

doctest{ cout << "anon 4\n"; }

