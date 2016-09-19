## Contributing

Support the development of the project with donations! There is a list of planned features which are all important and big - see the [**roadmap**](roadmap.md). I took a break from working in the industry to make open source software so every cent is a big deal.

If you work for a company using doctest or have the means to do so, please consider financial support.

[![Pledgie](https://pledgie.com/campaigns/31280.png)](https://pledgie.com/campaigns/31280)
[![Patreon](https://cloud.githubusercontent.com/assets/8225057/5990484/70413560-a9ab-11e4-8942-1a63607c0b00.png)](http://www.patreon.com/onqtam)
[![PayPal](https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=3K423Q6TK48BN)

## pull requests

Consider opening an issue for a discussion before making a pull request to make sure the contribution goes smoothly.

All pull requests should be made against the ```dev``` branch because the ```master``` is the stable one with the latest release.

If you're going to change something in the library itself - make sure you don't modify ```doctest/doctest.h``` because it's generated from ```doctest/parts/doctest_fwd.h``` and ```doctest/parts/doctest_impl.h``` - they get concatenated by CMake - so make sure you do a CMake build after you modify them so the ```assemble_single_header``` target gets built.

This framework has some design goals which must be kept. Make sure you have read the [**features and design goals**](doc/markdown/features.md) page.

If your changes also change the output of the library - you should also update the reference output for the tests or otherwise the CI builds (```travis``` and ```appveyor```) will fail when they compare the latest output to the outdated reference output (which is committed in the repository). To do this run CMake with the ```TEST_MODE``` variable set to ```COLLECT``` (making the new reference output) and then run ```ctest``` and commit the changed ```.txt``` files too. The default ```TEST_MODE``` is ```COMPARE```. 

Example: ```cmake -DTEST_MODE=COLLECT path/to/sources && ctest```

Code should be formatted with a recent-enough ```clang-format``` using the config file in the root of the repo (or I will do it...)

Testing with compilers different from GCC/Clang/MSVC (and more platforms) is something the project would benefit from.



