#!/usr/bin/env bash

cd ../docs

# build the docs
make clean
make html

# switch branches and pull the data we want
git checkout gh-pages

# goto GIT root
cd $(git rev-parse --show-toplevel)
# remove tracked files
git ls-files -z | xargs -0 rm -f
mv ./docs/_build/html/{.,}* ./

gitignore="pg_dsp/
examples/
tests/
*.egg-info/
*_bdpfigure/
"
printf "$gitignore" > .gitignore

git add -A
git commit -m "publishing updated docs..."
git push origin gh-pages

# switch back
git checkout develop
