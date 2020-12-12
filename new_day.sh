#!/usr/bin/env bash

set -e

year=$1
day=$2

cp -a day_base/day_template/ $year/$day
sed -i '' -e "s/year = 'day_base'/year = '$year'/" $year/$day/BUILD
sed -i '' -e "s/day = 'day_template'/day = '$day'/" $year/$day/BUILD

find $year/$day -type f
