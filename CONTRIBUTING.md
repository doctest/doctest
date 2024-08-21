## Contributing

This library is free, and will stay free but needs your support to sustain its development. There are lots of [**new features**](https://github.com/doctest/doctest/issues/600) and maintenance to do. If you work for a company using **doctest** or have the means to do so, please consider financial support.

[<img src="https://c5.patreon.com/external/logo/become_a_patron_button.png" align="top">](https://www.patreon.com/onqtam)
[<img src="https://user-images.githubusercontent.com/29021710/150090263-50ce0fa7-7813-4648-8273-fec3bbbd171c.jpg" width=175>](https://www.paypal.me/onqtam/10)

## Bug reports
Found a bug? Let's make it disappear forever.
However, there have been many bug reports over the years, please read through existing issues to see if the bug has been found before.
If your submit a bug that has been reported before it will be marked as a `verdict/duplicate`.
If the bug is a false positive, it will be marked as such (`verdict/false-positive`).
To file a proper bug report one should supply at least the following information

- Provide a minimal reproducible example (MRE) to let others verify the bug. This include **all** source code **and** command line arguments to reproduce the bug
  (or a step by step guide if you use an IDE, but please use the command line if you can).
- The output from the executable for runtime errors, or from the compiler if you get compiler errors / warnings
- Your system information, this includes
  - Operating system
  - Doctest version, including commit hash if based on `dev` branch
  - Compiler + version

For issues related to static code analysis, please provide exact version of your static analysis tool.

Any issue that does not follow this will be marked as `incomplete/needs-MRE`.
After you have filed the bug report, someone will have a look at it. This might take some time, so please be patient.
If the issue is still unclear, someone will ask for clarifications and the issue will be marked as `incomplete/needs-clarification`.
Once the bug report is proper, it will be marked as `type/bug` and considered for fixing.
If you decide to fix it, make a note in the issue and proceed to make a [pull requests](#pull-request).

To further categorize the type of feature to streamline triaging, it might be marked as

- `category/sanitization`: Related to issues with ASAN, UBSAN and Valgrind.
- `category/static-analysis`: Related to issues with static analysis tools such as the Clang Static Analyzer (used by clang-tidy).

## Improving the documentation
Improving the docs is always appreciated.
Performing an **addition** to the docs should quickly be accepted (given that it is correct) and can usually be done directly with a PR.
Performing a **change** might trigger a discussion, so please open an issue first to discuss it.
Issues related to documentation will be marked as `type/docs`.
When submitting the PR, please run the text through a spell checker first (such as `aspell` on Linux).

## Adding tests
Doctest strives to be an extremely stable testing library. Adding tests is always welcome and highly appreciated.
Usually you can proceed to PR directly where it can be reviewed in detail.

## Improving the CI
Doctest can not ensure that it works as intended on platform not continuously tested.
If you work with a platform not included there, please consider improving it by adding your platform to the CI.

## Feature proposals
Do you have an idea on how to make an improvement to the project?
Awesome! There are many ways to improve it, such as writing documentation, adding tests, or proposing new library features.
However, keep the following things in mind

- Please read through the [design goals](doc/markdown/features.md) before proposing a feature. While the feature might seem awesome, it might not be worth the trouble. Your idea might be rejected, not because it is bad, but because it is not worth it (think "lines of code + risk of breaking others divided by actual benefit").
- Checkout the `dev` branch to ensure the feature has not been implemented already and that the new feature works as intended.
- There have been many feature requests over the years. Please read through existing issues to see if your idea has been discussed before, and if you find it, join that thread.
  If your proposal has been discussed before it will be marked as a `verdict/duplicate`.
- Doctest is a mature library. Any **change** is likely to break it for someone, so this should be considered very carefully, and will most likely be rejected or postponed for a very distant 3.xx version.
- Any **addition** to the library needs to be clearly motivated. Submitting the proposal with a motivation of "it would be nice to have" is not motivating enough.
  Please provide a clear example with code showing real usage of the feature (not just a dummy example), and explain how it is useful in clear text.
  An issue which is considered unmotivated will be marked as `incomplete/needs-motivation`.

Once the feature proposal is proper, it will be marked as `type/feature-proposal`.
As part of the triaging procedure the discussions might lead to workarounds and marked accordingly.
This is a good thing, both because it drives the discussion itself as well as providing actual good workarounds.
Not every feature has to be implemented in the library, it is totally fine that users create their own functions and macros on top of it.
If many users repeatedly write the same boilerplate, that makes the feature more interesting to include in the library itself.
To further categorize the type of feature to streamline discussions, it might be marked as

- `category/API-usage`: Proposals of additions to the core API of doctest, such as new assertion macros or matchers
- `category/API-driver`: Proposals of additions to the C++ API for driving doctest, such as adding new methods or classes
- `category/BDD`: [Behaviour Driver Development](doc/markdown/testcases.md#bdd-style-test-cases)
- `category/build-system`: Proposals of new build system features. There can be furthed tagged with `build-system/*` labels.
- `category/CLI`: Proposals of additions to the doctest command line interface, such as adding new flags
- `category/config`: Proposals related to the configuration system
- `category/floats`: Testing of code dealing with floating point numbers.
- `category/strings`: Proposals of additions or changes to the stringification feature of types and variables
- `category/reporting`: Proposals related to the [reporter system](doc/markdown/reporters.md)
- `category/platforms`: Proposals for supporting additional platforms

If the feature proposal is accepted, please proceed to make a [pull requests](#pull-request).

## Pull requests

Consider opening an issue for a discussion before making a pull request to make sure the contribution goes smoothly.

All pull requests should be made against the ```dev``` branch because the ```master``` is the stable one with the latest release.

If you're going to change something in the library itself - make sure you don't modify ```doctest/doctest.h``` because it's generated from ```doctest/parts/doctest_fwd.h``` and ```doctest/parts/doctest.cpp``` - they get concatenated by CMake - so make sure you do a CMake build after you modify them so the ```assemble_single_header``` target gets built. Also take into consideration how the change affects the code coverage - based on the project in ```examples/all_features```. Also update any relevant examples in the ```examples``` folder.

This framework has some design goals which must be kept. Make sure you have read the [**features and design goals**](doc/markdown/features.md) page.

If your changes also change the output of the library - you should also update the reference output for the tests or otherwise the CI builds will fail when they compare the latest output to the outdated reference output (which is committed in the repository). To do this run CMake with the ```DOCTEST_TEST_MODE``` variable set to ```COLLECT``` (making the new reference output) and then run ```ctest``` and commit the changed (or newly created) ```.txt``` files in the ```test_output``` folders too. The default ```DOCTEST_TEST_MODE``` is ```COMPARE```. 

Example: ```cmake -DDOCTEST_TEST_MODE=COLLECT path/to/sources && cmake --build . && ctest```

Code should be formatted with a recent-enough ```clang-format``` using the config file in the root of the repo (or I will do it...)

Testing with compilers different from GCC/Clang/MSVC (and more platforms) is something the project would benefit from.

---------------

[Home](readme.md#reference)

<p align="center"><img src="scripts/data/logo/icon_2.svg"></p>
