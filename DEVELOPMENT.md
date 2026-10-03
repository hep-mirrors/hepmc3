# HepMC developer instructions

## PLEASE ADHERE TO THESE RULES WHEN ADDING NEW CODE

This will help keep the code uniform and will greatly improve readability
of the code for new developers or old ones going back into the code after
several years.

## GENERAL RULES

- We follow these naming conventions:
   - `snake_case` style of function and variable naming;
   - `CamelCase` style of class naming;
   - class/struct variables start with `m_`;
   - enum values are `ALL_CAPS`;
   - getters do *not* use a `get_` prefix;
   - setters *do* use a `set_` prefix.

- Mark `const` functions and arguments accordingly.

- HepMC3 follows no-throw policy, as in: no part of the code
  should throw an exception or use `exit`, etc. to end the program.
  All critical cases should be checked and when needed an error should
  be printed.

- Declare use of each std class separately.
  `using std::vector` instead of `using namespace std`.

- Use pre-defined macros for any text output outside of print() functions.
  Precede any text with class and function name:
  ```c++
  HEPMC3_ERROR(    "GenEvent::test: No particles in the event!" )
  HEPMC3_WARNING(  "GenEvent::test: Only one particle present!" )
  HEPMC3_DEBUG( 3, "GenEvent::test: Has end vertex: " << (bool)end_vertex() )
  HEPMC3_DEBUG_CODE_BLOCK(
      int x = 10*10;
      HEPMC3_DEBUG( 3, "GenEvent::test: This should be 100: " << x )
      event->print();
  )
  ```

- Whenever you need an output for debugging, always use an
  appropriately commented `HEPMC3_DEBUG( 10, "info" )` block. This
  will make things easier if you later decide it's worth to leave the
  debug info in the code.

- `HEPMC3_DEBUG` and `HEPMC3_DEBUG_CODE_BLOCK` statements are not
   compiled in release mode so use them at will. To avoid information
   flooding, use appropriate debug levels for specific types of debug
   information:
   - level 1: critical, short info;
   - level 10: less important, longer info.


## CODE FORMATTING

To perform an automatic formatting the `astyle` utility is used.
Run `make beauty` to format the C++ code with `astyle` and `make beautypython`
to format the Python code with `black`.


## COMMENTS STYLE

- When writing a comment that goes into documentation, use
  Doxygen-style comments, with Doxygen keywords starting with `@`, e.g.:
  ```c++
  /**
   *  @file GenEvent.h
   *  @brief Definition of @b class GenEvent
   *
   *  @class HepMC3::GenEvent
   *  @brief Stores event-related information
   *
   *  Manages GenParticle and GenVertex objects
   *
   */
  ```
  or for one-line comments,
  ```c++
  /// Storage of blah blah
  int m_variable1;

  int variable2; //!< @todo This variable is probably useless
  ```

  Parts that do not go into documentation (e.g. steps inside a function)
  can be commented using usual `//` or `/* */` comments.

- Keep all of your code documented. building with `-D HEPMC3_BUILD_DOCS=ON`
  should give no warnings about missing documentation.

- Use `@bug` and `@todo` keywords to mark problems found in the
  code. They go into separate lists in the documentation so we can
  keep track of them at any time. Remove these keywords after fixing
  the issue.


## COMMITS AND REPOSITORY

Direct pushes into master are forbidden; all contributions should be
via merge requests.  Check success of CI jobs, and squash commits as
standard (this is set in the GitLab config).

To run a larger set of tests, add "FedoraCI" to the commit message
to run all Fedora-based jobs and "CentOSCI" to run all CentOS-based
jobs.

A change-log entry is recommended for most merge requests, including
technical changes. It helps users and developers understand what changed.
If an entry would not be useful, add the `skip-changelog` label.


## BEFORE RELEASE

- Check and update the `ChangeLog`.
- Check if there are no open issues that can be closed.
- Update the header-only libraries `pybind11` and `bxzstr` if possible.
- Rerun `binder` to re-generate Python bindings. Don't forget to check those are compileable on all platforms.
- Run `cppcheck`/`clang-tidy`, e.g. `make staticcheck` for `cppcheck`.
- Run code formatting, e.g. `make beauty`.
- Check the CI, run all jobs. See above how to do that.
- Run builds on Windows and Mac using e.g. `GitHub`. The configuration is stored in `.github`.
- Update the version number in the `CMakeLists`, etc.
- Request a tag.
