#!/usr/bin/env bash

set -e

year=$1
day=$2

cp -a day_base/day_template/ $year/$day
sed -i.bak \
	-e "s/year = 'day_base'/year = '$year'/" \
	-e "s/day = 'day_template'/day = '$day'/" \
	$year/$day/BUILD
rm -f $year/$day/BUILD.bak

find $year/$day -type f
