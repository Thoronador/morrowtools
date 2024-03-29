# Contribution guidelines

First off, thanks for taking the time to contribute! :tada::+1:

The following is a set of guidelines for contributing to this project.
These are not set in stone, but following these guidelines closely is usually
considered good practice. Use your best judgment, and feel free to propose
changes to this document in a pull request.

## Using GitHub to implement features and fix bugs

If you want to help out with one of our bugs or implement a new feature, here
are some suggestions for success. The basic process looks like this:

1. Fork the repository.
2. Clone your fork to your development machine.
3. Create your issue branch.
4. Implement your feature or fix the bug and push the results to your fork.
5. Send us a pull request.

You can find more info about [creating a fork](https://help.github.com/articles/fork-a-repo/)
and [pull request](https://help.github.com/articles/creating-a-pull-request-from-a-fork/)
in the GitHub help articles. Our maintainers will review your pull request as
soon as we can.

## What makes a good pull request?

To summarize:

* Address only one issue with a single pull request.
* For new functionality, have a conversation with the maintainers before putting
  in too much work. Opening an issue is a good way to start the conversation.
* Finally, small pull requests are preferred. That way we can find time to
  review them between work, and family, and watching videos of cute cats jumping
  into cardboard boxes on the internet.

## Styleguides

### Git Commit Messages

* Use the present tense ("Add feature" not "Added feature").
* Use the imperative mood ("Move cursor to..." not "Moves cursor to...").
* Limit all lines of the commit message to 65 characters or less.
  * Exception: When a line contains a URLs that is too long to fit into the
    character limit, do not add a line break after 65 characters.
* URLs (e. g. for linking to information about a vulnerability in a dependency)
  should be enclose in angle brackets, e. g. `<https://example.com/>`.
* For multi-line commit messages:
  * Write the summary on the first line.
  * Leave the second line empty.
  * Then follow up with a more detailed explanation of the changes.
* When only changing documentation, include `[ci skip]` on a separate line in
  the commit message.

### Code Styleguide

_Note: This project was started in January 2011, a time when the C++11 standard
had not been finalized yet. Furthermore, most compilers did not have complete
support for the upcoming C++ standard at that time. Therefore, most of the code
still uses the old C++98 style. Only a small number of files has been updated
for C++17._

* Try to limit lines to 80 characters.
* Use spaces (not tabs) for indentation, two spaces per indentation level.
* Try to follow best practices and guidelines for modern C++ (i. e. C++11 and
  later). If in doubt, consult the C++ Core Guidelines at
  <https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines>.
