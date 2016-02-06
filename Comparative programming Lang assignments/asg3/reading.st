#!/afs/cats.ucsc.edu/courses/cmps112-wm/usr/smalltalk/bin/gst -f


nl := Character nl.

Character extend [
   isPrint [
      ^ (Character space <= self) & (self <= $~)
   ]
   visible [
      self isPrint ifTrue: [^ '', self asString]
                   ifFalse: [^ self asInteger "printStringRadix: 10"]
   ]
]

Object subclass: ZeroArray [
   |theArray|

   ZeroArray class >> new [
      self shouldNotImplement.
   ]

   ZeroArray class >> new: size [
      |result|
      result := super new.
      result init: size.
      ^ result
   ]

   init: size [
      theArray := Array new: size.
   ]

   size [
      ^ theArray size.
   ]

   at: index [
      ^ theArray at: index + 1.
   ]

   at: index put: value [
      ^ theArray at: index + 1 put: value.
   ]

   incr: index [
      (self at: index) isNil
         ifTrue: [ self at: index put: 0].
      self at: index put: (self at: index) + 1.
   ]

   keysAndValuesDo: aBlock [
      (0 to: (self size) - 1) do: [:index |
         aBlock value: index value: (self at: index).
      ]
   ]

]

Object subclass: Leaf [
   |char count|
   char [ ^ char ]
   count [ ^ count ]

   Leaf class >> new [
      self shouldNotImplement
   ]

   Leaf class >> new: aChar count: aCount [
      |result|
      result := super new.
      result setChar: aChar andCount: aCount.
      ^result
   ]

   setChar: aChar andCount: aCount [
      char := aChar.
      count := aCount.
   ]

   <= other [
      ^ (count < other count)
      | ((count = other count) & (char <= other char))
   ]

   printBase: aStream [
      ^ aStream << self class << '(' << char << ',' << count
   ]

   printOn: aStream [
      (self printBase: aStream) << ')'.
   ]

   inorder: visitor prefix: string [
      visitor value: char value: string.
   ]

   postorder: string [
      |bitNumber intofchar newstring|
      intofchar := char asInteger.
      bitNumber := 8.
      newstring := string, '0'.
      [bitNumber > 0] whileTrue: [
         newstring := newstring, ((intofchar bitAt: bitNumber) asString).
	 bitNumber := bitNumber - 1.
      ].

      "DEBUG: says what character was found and its ascii code"
      stdout << char << ' - ' << intofchar << Character nl.

      (intofchar = 256) ifTrue: [newstring := newstring, '1'].
      (intofchar = 0) ifTrue: [newstring := newstring, '0'].
      ^newstring.
   ]

]


Leaf subclass: Tree [
   |left right|

   Tree class >> new: aChar count: aCount [
      self shouldNotImplement
   ]

   Tree class >> new: aChar count: aCount left: aLeft right: aRight [
      |result|
      result := super new: aChar count: aCount.
      result setLeft: aLeft andRight: aRight.
      ^ result
   ]

   setLeft: aLeft andRight: aRight [
      left := aLeft.
      right := aRight.
   ]

   printOn: aStream [
      (self printBase: aStream) << ',' << left << ',' << right << ')'.
   ]

   inorder: visitor prefix: string [
      left inorder: visitor prefix: string, '0'.
      right inorder: visitor prefix: string, '1'.
   ]


   postorder: string [
      |newstring|
      newstring := string.
      newstring := left postorder: newstring.
      newstring := right postorder: newstring.
      newstring :=  newstring, '1'.
      ^newstring.
   ]

]


Object subclass: OutBits [
   |bitIndex currentByte myStream|
   OutBits class >> new [
      self shouldNotImplement.
   ]
   OutBits class >> new: fileStream [
      |result|
      result := super new.
      result init: fileStream.
      ^result
   ]
   clearByte [
      bitIndex := 8.
      currentByte := 0.
   ]
   init: fileStream [
      myStream := fileStream.
      self clearByte.
   ]
   flushByte [
      currentByte = 8 ifFalse: [
         myStream nextPutByte: currentByte.
         self clearByte.
      ]
   ]
   writeBit: bit [
      currentByte := currentByte bitAt: bitIndex put: bit.
      bitIndex := bitIndex - 1.
      bitIndex = 0 ifTrue: [self flushByte].
   ]
]

writeBitArray := [:array :outfile|
   |outBytes|
   outBytes := OutBits new: outfile.
   array do: [:bit| outBytes writeBit: (bit asInteger)].
   outBytes flushByte.
].

padString := [:val :pad |
   |string i|
   string := val asString.
   i := pad - (string size).
   [i > 0] whileTrue: [string := ' ', string. i := i - 1.].
   string.
].


"Given the frequency table and encode table, prints a table.
 Used with the -t option."
writeT := [:freqt :encodet |
   |i|
   i := 0.
   [i < 257] whileTrue: [
      |count string|
      count := freqt at: i.
      string := encodet at: i.
      (count notNil and: [count > 0]) ifTrue: [
         stdout << (padString value: (i asCharacter visible) value: 3)
                << ' ' << (padString value: count value: 5) << ' ' 
                << string << nl.
      ].
   i := i + 1.
   ].
].


freqtable := ZeroArray new: 257.
freqtable at: 256 put: 1.

"This is temporary since im not reading in a file right now"
input := ''.
[stdin atEnd not] whileTrue: [
   |ordChar|
   ordChar := stdin next asInteger.
   input := input, ((ordChar asCharacter) asString).
   freqtable incr: ordChar.
].


"DEBUG: prints whats in freqtable"
freqtable keysAndValuesDo: [:key :value |
   (value notNil and: [value > 0]) ifTrue: [
      stdout << 'freq[' << key asCharacter visible << ']=' << value
             << nl.
   ]
].
stdout << nl.


"Now create leafs out of non nil elements of freqtable and put them into 
a sorted collection"
sortcol := SortedCollection new.
freqtable keysAndValuesDo: [:key :value |
   (value notNil and: [value > 0]) ifTrue: [
      |char leaf|
      char := key asCharacter.
      leaf := Leaf new: char count: value.
      sortcol add: leaf.
   ]
].


"DEBUG: prints whats in collection after its filled"
sortcol inspect.
stdout << nl.


"Build the tree. If they only give EOF then it will be a leaf."
[sortcol size > 1] whileTrue: [
   |l r p|
   l := sortcol removeFirst.
   r := sortcol removeFirst.
   p := Tree new: (l char) count: (l count + r count) left: l right: r.
   sortcol add: p.
].
root := sortcol removeFirst.

"DEBUG: prints the constructed tree."
root inspect.
stdout << nl.


"Make array to contain bitstrings to represent each character."
bitstringtable := ZeroArray new: 257.
visit := [:char :string | bitstringtable at: (char asInteger) put: string.].
root inorder: visit prefix: ''.

"DEBUG: prints whats in bitstringtable"
bitstringtable keysAndValuesDo: [:key :value |
   (value notNil and: [value > 0]) ifTrue: [
      stdout << 'char[' << key asCharacter visible << ']=' << value
             << nl.
   ]
].
stdout << nl.

"Everything before this point applies to both -c and -t."


"This should only be done with -t option."
writeT value: freqtable value: bitstringtable.



"Everything after this should only be done with -c option."

"create bitstring to represent the tree"
treestring := (root postorder: '').


"DEBUG: prints the bitstring representation of the tree."
stdout << treestring << nl.


"Convert the input character to their encoded versions."
encodedtext := ''.


"only working with string right now, needs to work with filestream"
byteArray := input asByteArray.


"DEBUG:"
byteArray inspect.


byteArray do: [:elem | 
   encodedtext := encodedtext, (bitstringtable at: elem).
].
"Add EOF"
encodedtext := encodedtext, (bitstringtable at: 256).

"Concat treestring and encodedtext"
encodedtext := treestring, encodedtext.

"Pad if not multiple of 8 characters"
[(((encodedtext size) \\ 8) = 0) not] whileTrue: [
   encodedtext := encodedtext, '0'.
].


"DEBUG: Print the encoded text."
stdout << encodedtext << nl.


"print the string out as actual bits."
byteArray := encodedtext asByteArray.
writeBitArray value: byteArray value: stdout.
