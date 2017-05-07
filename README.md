#SemaNet [0.1]

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

On Ubuntu 16.04, the following should satisfy all the dependencies (assuming a g++/gcc already installed)

```bash
sudo apt-get install libboost-all-dev wordnet wordnet-dev cmake
```

For older Ubuntu (14.04) you need to manually build `libboost-serialization` to be at least 1.56 or higher.
Other Linuxes (Debian, Arch, etc) have to also manually build boost, and maybe (WordNet from source)[https://github.com/wordnet/wordnet].

##Building

In order to build, please ensure you have satisfied all dependencies first.

```
mkdir build
cd build
cmake ..
make
```

You should now have in your `build` directory:

* the library `libsemanet-0.1.so` (unless you changed `CMakeLists.txt` to build a static library)
* `hypernym` an example on how to get hypernyms and print them on stdout
* `hyponym` an example on how to get hyponyms and print them on stdout
* `synonym` an example on how to get synonyms and prin them on stdout
* `pathfind` an example on how to obtain semantic difference between two words
* `pathfind_loop` an example on how `path_finder` and `delta_path` squash semantic distance values
* `utilities` an example on how to acquire all senses, and how to find intersections between different graphs
* `delta_best` an example on how to acquire the best delta paths, using semantic graph intersections

##Installing

From inside your `build` directory, if you have `sudo` then type:

```
sudo make install
```

else, `su` and as root type:

```
make install
```

The library will be copied in your `usr/local/lib` directory.
The headers will be copied in your `usr/local/include` directory.
You can include any header from `usr/local/include/smnet/` directory, or
you can include the global directory:

```
#include <smnet/smnet>
```

Don't forget to link against `libsmnet-0.1.so`!

##Guide about SemaNet

SemaNet was built for those reasons:

* Avoid dealing with the dreaded C WordNet library, which is at the moment of writing this 20 years old.
* Avoid dealing with memory allocations and de-allocations. Up until know I have not seen anywhere an example
which explains how to use WordNet whilst properly managing memory.
* Do some rudimentary graph operations on the acquired graph senses.
* Establish *semantic similarity* or *distance* between two **words**. This is **not** a vector space model,
but a *single entity* semantic similarity quantification (for more info, see class `delta_path` and `path_finder`).

You can use SemaNet for most of the stuff you would use WordNet.
What's missing are:

* Antonyms
* Morphing
* Other non-semantics related functionality

You can also use it to acquire and manipulate the actual semantic tree graph, as encoded by wordnet,
but in a friedly to C++ manner.

You can search for paths and traverse semantic graphs,
or merge multiple graphs to create a Semantic map.

##Using SemaNet

All examples are under the `examples` folder.
Have a look at them, they provide a simple synopsis of how to use SemaNet.

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
