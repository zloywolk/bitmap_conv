#!/bin/bash

echo "Create h3.c"
cat h2.c | sed -e 's/H2/H3/' | sed -e 's/RED/GREEN/' | sed 's/NUM_OF_CPUS > 0/NUM_OF_CPUS > 1/' > h3.c

echo "Create h4.c"
cat h2.c | sed -e 's/H2/H4/' | sed -e 's/RED/BLUE/' | sed 's/NUM_OF_CPUS > 0/NUM_OF_CPUS > 2/' > h4.c

exit 0
