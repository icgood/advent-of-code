#!/usr/bin/env bash

set -e

year=$1
day=$2

if [[ "$OSTYPE" == "darwin"* ]]; then
	sed_i="-i ''"
else
	sed_i="-i"
fi

cp -a day_base/day_template/ $year/$day
sed $sed_i -e "s/year = 'day_base'/year = '$year'/" $year/$day/BUILD
sed $sed_i -e "s/day = 'day_template'/day = '$day'/" $year/$day/BUILD

find $year/$day -type f
