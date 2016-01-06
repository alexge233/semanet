@author Alex Giokas <a.gkiokas@warwick.ac.uk>
@version 1

#WORK IN PROGRESS#

A C++11 wrapper around WordNet 3.0 with some extra functionality.
The wrapper iterates hypernyms, hyponyms and synonyms, and generates a tree graph.
There are similarity metrics defined and implemented in DeltaPathFinder and DeltaPath.

This library depends on:

Boost library (1.49)
WordNet Library (3.0)
Cereal Library serialisation (http://uscilab.github.io/cereal)
CGPP Library (https://github.com/alexge233/cgpp)

TODO: build and install instructions
