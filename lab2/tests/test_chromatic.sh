#!/bin/bash

if [ $# -ne 1 ] ; then
    echo "Skripta ocekuje 1 ulazni argument!"
    exit 1
fi

graphs=graph_*.txt

for graph in $graphs
do
	echo "##################################################"
	echo "Testing graph: $graph"
	$1 $graph
done

echo "##################################################"