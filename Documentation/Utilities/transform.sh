#!/bin/sh
# This script prepares CS documentation for online publication by adding
# user-annotation capability.

DIRS=$@
EFED="perl Utilities/efed.pl"

for d in $DIRS; do
  if test -d $d; then
    echo "Preparing $d for annotation capability."
    $EFED -d \
      -a "/^(.*)\.html/" \
      -r "html=php" \
      -e "s/href *= *\"(?!http\:\/\/)([^\"]*)\.html([^\"]*)\"/href=\"\$1.php\$2\"/gi" \
      -e "s:<title>(.*)</title>:<title>\$1</title><?php \\\$theme=\"\$1\"; ?>:i" \
      -e "s:</body>:<?php require(\"annotate.php\"); ?></body>:i" \
      $d
    cp Utilities/*.php $d
  fi
done
