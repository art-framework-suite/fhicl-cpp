.. cetlib documentation master file, created by
   sphinx-quickstart on Sun Jul  8 22:29:32 2018.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

*fhicl-cpp* library
===================

.. toctree::
   :maxdepth: 2

* `Release notes <releaseNotes.html>`_

* `Depends on <depends.html>`_


Library Overview
----------------

The fhiclcpp library provides types and functions that constitute a binding of the FHiCL specification to C++. The current version of fhicl-cpp corresponds to FHiCL 3 (see FHiCL 3 Quick Start Guide).

The following types are provided, each declared in the fhicl namespace:

- ParameterSet, corresponding to a user-specified configuration (a "collection of named values accessible to a user’s program while it is running");
- Any of the fhiclcpp parameter types as described here(`LINK`).
- ParameterSetID, uniquely identifying a specific value of a ParameterSet instance;
- ParameterSetRegistry, automatically registering each ParameterSet instance (or sub-instance) with its corresponding ParameterSetID;
- intermediate_table, serving as an internal ("raw" or "working") representation of a FHiCL document;
- extended_value, representing a FHiCL value within an intermediate_table;
- value_tag, classifying an extended_value instance; and
- exception, describing a circumstance from which the library can't recover.

In addition, the library provides public member functions and (also in the fhicl namespace) free functions to construct, observe, and transform values of these types.

The next section details the interface of the ParameterSet type, likely of greatest interest to most users of this library. 
A separate page describes those remaining types and functions that are typically used only while initially producing ParameterSet instances by combining information obtained from a fhicl document with additional information from such sources as a command line and application-specific defaults.


The ParameterSet Interface
--------------------------

Names, keys, and values
~~~~~~~~~~~~~~~~~~~~~~~

- In the descriptions below, parameters and arguments denoted name are always of type of type std::string; each corresponds to FHiCL's notion of a name.
- In the descriptions below, parameters and arguments denoted key are always of type of type std::string; each also corresponds to FHiCL's notion of a name, but when one ParameterSet is nested within another ParameterSet, a key may employ the FHiCL member notation.
- Each ParameterSet object consists of some number of keys, each associated with a value of some C++ type. Such an association is termed a key-value pair. When the key is a simple name, the association is equivalently termed a name-value pair.
- A value can be retrieved from a ParameterSet by presenting its corresponding key. Similarly, a value can be inserted into a ParameterSet by presenting both the value and its desired corresponding key.
- A ParameterSet contains values of C++ types closely corresponding to FHiCL values as described in the below table. We define a FHiCL type as any C++ type for which a conversion from the corresponding FHiCL value to that type is supported.


+---------------------+-------------+-----------------+ 
| FHiCL type (C type) | FHiCL value | FHiCL category  | 
+=====================+=============+=================+ 
| bool                | boolean     |                 | 
+---------------------+-------------+                 + 
| (un)signed int &    | numeric     |                 | 
| floating type       |             |                 | 
+---------------------+-------------+ atom            + 
| std::string         | string      |                 | 
+---------------------+-------------+                 + 
| std::complex        | complex     |                 | 
+---------------------+-------------+-----------------+ 
| std::array          | sequence    | sequence        | 
+---------------------+             +                 + 
| std::pair           |             |                 | 
+---------------------+             +                 + 
| std::tuple          |             |                 | 
+---------------------+-------------+-----------------+ 
| fhicl:ParameterSet  | table       | table           | 
+---------------------+-------------+-----------------+ 




- Note that a processed FHiCL document also yields a ParameterSet.
- A nil value is an atom by definition.
- As an extension to the functionality required of a FHiCL binding, a ParameterSet can contain values of arbitrary user-defined C++ types. The library processes such values via user-supplied functions that convert to the user-defined type. (In practice, this has been a rarely-used fhiclcpp library feature.)



Relationship to ParameterSetID
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- A ParameterSetID is automatically generated from (and thus corresponds to) the value of a ParameterSet object.
- If the value of a ParameterSet object ps is modified in any way (whether by inserting, deleting, or updating any key-value pair), a new ParameterSetID value is generated to correspond to the updated value of ps.



Immutability
~~~~~~~~~~~~

In practice, most ParameterSet objects, once constructed, are treated as immutable. While such practice is not required, it is preferable because of the relationship of a ParameterSet to a ParameterSetID, described above.


Compiler-generated functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- A default-constructed ParameterSet object is empty; that is, it consists of no key-value pairs.
- A copy-constructed ParameterSet object has the same key-value pairs as did the ParameterSet object from which it was copied.
- A newly assigned-to ParameterSet object has the same key-value pairs as did the ParameterSet object from which it was copied.



Observers
~~~~~~~~~

- A call of the form **ps.is_empty()** returns true if ps is empty, and returns false otherwise.
- A call of the form **ps.id()** returns the ParameterSetID value corresponding to the current value of ps.
- A call of the form **ps.to_string()** returns a std::string representation of the current value of ps.
- A call of the form **ps.to_compact_string()** returns a compact std::string representation of the current value of ps, including the collapsing of non-trivial nested parameter sets to @id::<hash>.
- String-rendered representations of the parameter set can be obtain by any of the calls:
    1. **ps.to_indented_string()** returns an expanded and easier-to-read std::string representation of the current value of ps.
    2. **ps.to_indented_string(initial_indent_level)** -- same as 1, but with an optional indent level (unsigned int) to increase the left-side margin.
    3. **ps.to_indented_string(initial_indent_level, annotate)** -- same as 2 but additionally annotated (when annotate == true) with the filename and line number where the key was defined or overridden.
    4. **ps.to_indented_string(initial_indent_level, print_mode)** -- same as 3, but allows additional printing formats (as defined in source:fhiclcpp/detail/print_mode.h).
- A call of the form **ps.get_names()** returns, as a std::vector<std::string>, a list of all names in ps.
- A call of the form **ps.get_pset_names()** returns, as a std::vector<std::string>, a list of all names in ps whose corresponding values are of ParameterSet type.
- A call of the form **ps.get_src_info(key)** returns a std::string with the filename and line number (delimited by ':') corresponding to where the key was defined or overridden.
- **ps.has_key(key)** returns true if ps contains a pair with a matching name as key (nested keys allowed), false otherwise.
- **ps.is_key_to_atom(key)** returns true if ps contains a pair with a matching name as key (no dot notation allowed), and a value which is an atom (@nil is an atom by definition).
- **ps.is_key_to_sequence(key)** returns true if ps contains a pair with a matching name as key (no dot notation allowed), and a value which is a sequence.
- **ps.is_key_to_table(key)** returns true if ps contains a pair with a matching name as key (no dot notation allowed), and a value which is a table (nested ParameterSet).




Retrievers
~~~~~~~~~~

Keys specified as arguments to retrievers may be nested (dot-delimited).

- A call of the form **ps.get<T>(key)** (or of the variant form ps.get<T>(key, convert)) will return the value of type T associated with the key.
  - Either call will throw an exception if:
    - ps contains no pair with a matching key, or
    - ps does contain a pair with a matching key, but the corresponding value can't be returned as a value of type T.
  - The first form is used when the type T is corresponds to a FHiCL value.
  - The variant form is used when T is an arbitrary user-defined type. The convert argument is a user-provided function that converts a given FHiCL value to a value of type T.
- A call of the form **ps.get(key,default_value)** (or of the variant form ps.get<T>(key, default_value, convert)) will return the value of type T associated with the key.
  - The first form is used when the type T is corresponds to a FHiCL value.
  - The variant form is used when T is an arbitrary user-defined type. The convert argument is a user-provided function that converts a given FHiCL value to a value of type T.
  - Either call will return the supplied default_value (which must be of type T) if:
    - ps contains no pair with a matching key, or
    - ps does contain a pair with a matching key, but the corresponding value can't be returned as a value of type T.
- A call of the form get_if_present(key, result) (or of the variant form get_if_present(key, result, convert)) has the following behavior:
  - If the supplied key is an empty string, throw an exception.
  - If ps contains no pair with a matching key, return false.
  - If ps does contain a pair with a matching key, but the corresponding value isn't of type T, throw an exception.
  - Otherwise, set the supplied result (which must be an lvalue expression) to the corresponding value and return true.
  - The first form is used when the type T is corresponds to a FHiCL value.
  - The variant form is used when T is an arbitrary user-defined type. The convert argument is a user-provided function that converts a given FHiCL value to a value of type T.


Inserters
~~~~~~~~~

Note that all key arguments to inserters must be names: it is an error to specify a nested (dot-delimited) key.

- A call of the form **ps.put(name, value)** will insert into ps a name-value pair composed of the given name and the given value.
    - If ps already contains a pair with the given name, a cant_insert exception is thrown.
    - Otherwise, a new pair is constructed and inserted into ps.
    - The type of the supplied value must either be a FHiCL type, or the user must define a function encode(X const & x) in the same namespace as X which returns the result of calling fhicl::encode() on one of the aforementioned types. As a fallback, a boost::lexical_cast() will be attempted to decode x.
- A call of the form **ps.put(name)** will insert into ps a name-value pair composed of the given name and the library's equivalent of a FHiCL nil value.
    - If ps already contains a pair with the given name, a cant_insert exception is thrown.
    - Otherwise, a new pair is constructed and inserted into ps.
- A call of the form **ps.put_or_replace(name, value)** will insert into ps a name-value pair composed of the given name and the given value.
    - If ps already contains a pair with the given name, that pair's value component is replaced.
    - Otherwise, a new pair is constructed and inserted into ps.
- A call of the form **ps.put(name)** will insert into ps a name-value pair composed of the given name and the library's equivalent of a FHiCL nil value.
    - If ps already contains a pair with the given name, that pair's value component is replaced.
    - Otherwise, a new pair is constructed and inserted into ps.
- A call of the form **ps.put_or_replace_compatible(name, value)** will insert into ps a name-value pair composed of the given name and the given value.
    - If ps already contains a pair with the given name, that pair's value component is replaced if and only if it is of a compatible FHiCL category. A nil value is compatible with all classifications. If the replacement value is not compatible with the existing value, a cant_insert exception is thrown.
    - Otherwise, a new pair is constructed and inserted into ps.


Deleters
~~~~~~~~

A call of the form **ps.erase(name)** will attempt to remove from ps the name-value pair with matching name and will return true if successful and false otherwise (i.e., if ps contains no pair with the specified name).

Comparators
~~~~~~~~~~~
Two ParameterSet objects may be compared for equality or inequality via the corresponding conventional operator notations. The objects are considered equal if and only if their respective ParameterSetID values are equal.


Interface technical summary
~~~~~~~~~~~~~~~~~~~~~~~~~~~
See :rsource:`fhiclcpp/ParameterSet.h`









