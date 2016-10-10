- update version in version.txt
- run update_stuff.py from the scripts folder
- regenerate test output by running cmake with -DTEST_MODE=COLLECT and then running ctest
- run update_wandbox_link.py
- commit in dev
- merge dev in master and coverity_scan (and rebase them?)
- push all branches (git push --all)
- run update_changelog.py in master
- commit in master and push
- create github release with the same semver tag as the changelog
- merge master in dev

on big releases spam (and also interlink) in:
- hackernews
- reddit (cpp/gamedev/Cplusplus/programming)
- isocpp.org

