#SemaNet [0.1]
#This is Work in Progress

A C++11 library built on top of WordNet 3.0.
It offers:

* Trouble free WordNet querying without worrying about memory management
* Parsing of WordNet senses in a tree-graph class (see classes `graph` and `layer`)
* Path finding between words, and quantifying their *semantic distance*
* Other utilities such as intersections and unions of WordNet tree graphs.

##Dependencies

SemaNet library depends on:

* Boost library (1.56 or higher)
* WordNet Library (3.0)
* g++ (1.47 or higher)
* CMake (2.8 or higher)

Installing `g++`, `cmake` and `boost` is fairly common on Linux/OSX these days.
Take special care when installing WordNet: OSX uses different paths, so you may have
to edit the `CMakeLists.txt` file.

##Building

In order to build, please ensure you have satisfied all dependencies first.

```
mkddir build
cd build
cmake ..
make
```

You should now have in your `build` directory:

* `libsemanet-0.1.so` (unless you changed `CMakeLists.txt` to build a static library)
* `hypernym` an example on how to get hypernyms and print them on stdout
* `hyponym` an example on how to get hyponyms and print them on stdout
* `synonym` an example on how to get synonyms and prin them on stdout
* `pathfind` an example on how to obtain semantic difference between two words

##Guide about SemaNet

TODO

##Using SemaNet

TODO

##Notes about WordNet

It is important to know how exactly WordNet works, in order to efficiently use SemaNet.

###Hypernyms
Hypernyms are essentially graphs of super-classes (or super-ordinates).
WordNet uses tree-graphs where your query will be at the bottom layer,
and all super-classes will be returned as layers above the root layer,
within which your query resides.

Depending on the type of query and the lexical category (Noun, Verb, Adjective, Adverb)
most words share some kind of abstract archetype.

###Hyponyms
Hyponyms are *specializations* or *instantiations* linked as sub-classes
or sub-ordinate words which add specificity to your query.
Those are always returned as a single layer below your current layer
within which your query is to be found.

This however does not mean that there exist no more sub-classes,
its WordNet which choses to only return a sub-class layer.
The reason is most likely that returning all known sub-classes,
would return a huge graph.
Thus, if you wish to search all known sub-classes, you have to do
multiple queries, compared to hypernyms which requires a single query.

###Synonyms
Synonyms simply denote words which are *almost* the same (or exactly the same).
Thus when you query synonyms, you will get a layer which contains your query word,
as well as other words (the synonyms).

###Antonyms
As of version (0.1) I have not implemented antonyms.

###Contributor

- Alexander Giokas, **[a.gkiokas@warwick.ac.uk]**
