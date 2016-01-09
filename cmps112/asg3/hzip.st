#!/afs/cats.ucsc.edu/courses/cmps112-wm/usr/smalltalk/bin/gst -f
 " Jim Brower         jrbrower@ucsc.edu "
 " Greer-Berezovsky   pbgreerb@ucsc.edu "

 " NAME "
 " hzip "

 " SYNOPSIS "
 "    hzip -option[utcd] [infile] [outfile] "

 " DESCRIPTION "
 "    A simple file compression and decompression  "
 "    tool.  "


nl := Character nl.
debug := 0.
process := $z.
file := 0.
fileContents := nil.
fileWrites := stdout.

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

Object subclass: BitStream [
   |bitIndex byte myStream|
   BitStream class >> new: fileStream [
      |result|
      result := super new.
      result init: fileStream.
      ^result
   ]
   init: fileStream [
      myStream := fileStream.
      bitIndex := 1.
   ]
   nextBit [
      bitIndex = 1 ifTrue: [
         byte := myStream next.
         bitIndex := 9.
      ].
      bitIndex := bitIndex - 1.
      ^byte value bitAt: bitIndex
   ]
   readNextBit [
      (bitIndex = 1)
      ifTrue: [
         byte := myStream next.
         bitIndex := 9.
         ^byte value bitAt: (bitIndex - 1).
      ]
      ifFalse: [
         ^byte value bitAt: (bitIndex - 1).
      ].
   ]
   atEnd [
      ^bitIndex = 1 and: [myStream atEnd]
   ]
]

Object subclass: Leaf [
   |char count debug|
   char [ ^ char ]
   count [ ^ count ]

   Leaf class >> new [
      self shouldNotImplement
   ]

   Leaf class >> new: aChar count: aCount debugg: aDebug[
      |result|
      result := super new.
      result setChar: aChar andCount: aCount andDebug: aDebug.
      ^result
   ]

   setChar: aChar andCount: aCount andDebug: aDebug[
      char := aChar.
      count := aCount.
      debug := aDebug.
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

   postorder: bitStream [
      |bitNumber intofchar|
      intofchar := char asInteger.
      bitStream writeBit: 0.

      bitNumber := 8.
      [bitNumber > 0] whileTrue: [
         bitStream writeBit: (intofchar bitAt: bitNumber).     
         bitNumber := bitNumber - 1.
      ].

      (debug == 1)
      ifTrue: [
      "DEBUG: says what character was found and its ascii code"
      stdout << char << ' - ' << intofchar << Character nl.
      ].

      (intofchar = 256) ifTrue: [
      bitStream writeBit: 1.].
      (intofchar = 0) ifTrue: [
      bitStream writeBit: 0.].
   ]

   isLeaf [
      |return|
      return := 1.
      ^ return.
   ]
]

Leaf subclass: Tree [
   |left right|
   left [ ^ left ]
   right [ ^ right ]

   Tree class >> new: aChar count: aCount debugg: aDebug [
      self shouldNotImplement
   ]

   Tree class >> new: aChar count: aCount debugg: 
   aDebug left: aLeft right: aRight [
      |result|
      result := super new: aChar count: aCount debugg: aDebug.
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

   postorder: bitStream [
      left postorder: bitStream.
      right postorder: bitStream.
      bitStream writeBit: 1.
   ]

   isLeaf [
      |return|
      return := 0.
      ^ return.
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
      myStream nextPutByte: currentByte.
      self clearByte.
   ]
   writeBit: bit [
      currentByte := currentByte bitAt: bitIndex put: bit.
      bitIndex := bitIndex - 1.
      bitIndex = 0 ifTrue: [self flushByte].
   ]
   finish [
      [(bitIndex <= 0) not] whileTrue: [
      stdout << bitIndex << Character nl.
         currentByte := currentByte bitAt: bitIndex put: 0.
         bitIndex := bitIndex - 1.
      ].


      self flushByte.
   ]
]

Object subclass: stackObj [
   |next leaf|

   stackObj class >> new [
      self ShouldNotImplement.
   ]

   stackObj class >> new: aNext leaf: aLeaf [
      |result|
      result := super new.
      result set: aNext leaf: aLeaf.
      ^ result.
   ]

   set: aNext leaf: aLeaf [
      next := aNext.
      leaf := aLeaf.
   ]

   getLeaf [
      ^ leaf.
   ]

   getNext [
      ^ next.
   ]
]

Object subclass: myStack [
   |curr|

   myStack class >> new [
      |result|
      result := super new.
      result setNil.
      ^ result.
   ]

   setNil [
      curr := nil.
   ]

   push: aLeaf [
      |temp|
      temp := stackObj new: curr leaf: aLeaf.
      curr := temp.
   ]

   pop [
      |temp|
      (curr == nil)
         ifTrue: [
            ^ nil.
            ]
         ifFalse: [
            temp := curr getLeaf.
            curr := curr getNext.
            ^ temp.
            ].
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
writeT := [:freqt :encodet :outStream |
   |i|
   i := 0.
   [i < 257] whileTrue: [
      |count string|
      count := freqt at: i.
      string := encodet at: i.
      (count notNil and: [count > 0]) ifTrue: [
         outStream << (padString value: (i asCharacter visible) 
                      value: 3)
                << ' ' << (padString value: count value: 5) << ' ' 
                << string << nl.
      ].
   i := i + 1.
   ].
].

"Printout the arguments if the in DEBUG mode"
(debug == 1)
ifTrue: [
   stdout << 'arguments: ' << Smalltalk arguments << nl.
].

"Parse the arguments for the program"
Smalltalk arguments: '-t -c -u -d'
do: [:opt :arg |
   (arg == nil)
      ifTrue: [
         (opt = $t)
            ifTrue: [
               (process ~= $z)
                  ifTrue: [
                     stderr << 'Invalid Operation Combination' << nl.
                     ObjectMemory quit: 1.
                     ].
               process := $t.
            ].
         (opt = $c)
            ifTrue: [
               (process ~= $z)
                  ifTrue: [
                     stderr << 'Invalid Operation Combination' << nl.
                     ObjectMemory quit: 1.
                     ].
               process := $c.
            ].
         (opt = $u)
            ifTrue: [
               (process ~= $z)
                  ifTrue: [
                     stderr << 'Invalid Operation Combination' << nl.
                     ObjectMemory quit: 1.
                     ].
               process := $u.
            ].
         (opt = $d)
            ifTrue: [
               debug := 1.
            ].
      ]
      ifFalse: [
         (file == 2)
            ifTrue: [
               stderr << 'Too many arguments given' << nl.
               ObjectMemory quit: 1.
            ].
         (file == 1)
            ifTrue: [
               fileWrites := FileStream open: arg 
                                        mode: FileStream write.
               file := 2.
            ].
         (file == 0)
            ifTrue: [
               fileContents := FileStream open: arg 
                                          mode: FileStream read.
               file := 1.
            ].
      ].
]
ifError: [
   stderr << 'Error: invalid option' << nl.
   ObjectMemory quit: 1.
].

"Error out if there are no files given"
(file == 0)
   ifTrue: [
      stderr << 'Error: no input file' << nl.
      ObjectMemory quit: 1.
   ].



"Perform a compression or table print operation"

((process == $c) | (process == $t))
ifTrue: [

freqtable := ZeroArray new: 257.
freqtable at: 256 put: 1.

"This is temporary since im not reading in a file right now"

[fileContents atEnd not] whileTrue: [
   |ordChar|
   ordChar := fileContents next asInteger.
   freqtable incr: ordChar.
].

(debug == 1)
ifTrue: [
"DEBUG: prints whats in freqtable"
freqtable keysAndValuesDo: [:key :value |
   (value notNil and: [value > 0]) ifTrue: [
      stdout << 'freq[' << key asCharacter visible << ']=' << value
             << nl.
   ]
].
stdout << nl.
].

"Now create leafs out of non nil elements of freqtable and 
put them into a sorted collection"
sortcol := SortedCollection new.
freqtable keysAndValuesDo: [:key :value |
   (value notNil and: [value > 0]) ifTrue: [
      |char leaf|
      char := key asCharacter.
      leaf := Leaf new: char count: value debugg: debug.
      sortcol add: leaf.
   ]
].

(debug == 1)
ifTrue: [
"DEBUG: prints whats in collection after its filled"
sortcol inspect.
stdout << nl.
].

"Build the tree. If they only give EOF then it will be a leaf."
[sortcol size > 1] whileTrue: [
   |l r p|
   l := sortcol removeFirst.
   r := sortcol removeFirst.
   p := Tree new: (l char) count: (l count + r count) 
             debugg: debug left: l right: r.
   sortcol add: p.
].
root := sortcol removeFirst.

(debug == 1)
ifTrue: [
"DEBUG: prints the constructed tree."
root inspect.
stdout << nl.
].

"Make array to contain bitstrings to represent each character."
bitstringtable := ZeroArray new: 257.
visit := [:char :string | bitstringtable at: (char asInteger) 
          put: string.].
root inorder: visit prefix: ''.

(debug == 1)
ifTrue: [
"DEBUG: prints whats in bitstringtable"
bitstringtable keysAndValuesDo: [:key :value |
   (value notNil and: [value > 0]) ifTrue: [
      stdout << 'char[' << key asCharacter visible << ']=' << value
             << nl.
   ]
].
stdout << nl.
].

"Everything before this point applies to both -c and -t."


"This should only be done with -t option."
(process == $t)
   ifTrue: [
      writeT value: freqtable value: bitstringtable value: fileWrites.
      ObjectMemory quit: 0.
      ].

"Everything after this should only be done with -c option."

"print the tree"
outBytes := OutBits new: fileWrites. "?"
root postorder: outBytes.
outBytes writeBit: 1.

"Convert the input character to their encoded versions."

fileContents position: 0.
[fileContents atEnd not] whileTrue: [
   |ordChar array|
   ordChar := fileContents next asInteger.
   array := bitstringtable at:ordChar.
   array do: [:bit |
      outBytes writeBit: (bit asInteger). 
   ].
].

"Add EOF"
array := (bitstringtable at:256) asByteArray.
array do: [:bit |
   outBytes writeBit: (bit asInteger).
].

outBytes finish.
].



"Perform a decompression operation"

(process == $u)
ifTrue: [
treeConstructed := 0.
leafPile := myStack new.
infileBits := BitStream new: fileContents. 

"Construct Tree"

[treeConstructed == 1] whileFalse: [
   (0 == (infileBits nextBit))
      ifTrue: [
         byte := 0.
         tempChar := nil.
         7 to: 0 by: -1 do: [ :bit |
            byte := byte + ((2 raisedTo: bit) * (infileBits nextBit))
            ].
         tempChar := Character value: byte.
         (debug == 1)
         ifTrue: [
            stdout << byte << ' ' << tempChar << nl.
            ].
         (byte == 0)
            ifTrue: [
               (1 == (infileBits readNextBit))
                  ifTrue: [
                     infileBits nextBit.
                     tempChar := eof.
                     ].
               ].
         temp := Leaf new: tempChar count: 0 debugg: debug.
         leafPile push: temp.
         ]
      ifFalse: [
         tempR1 := leafPile pop.
         tempL0 := leafPile pop.
         (tempL0 == nil)
            ifTrue: [

               "Rebuild File"

               [infileBits atEnd] whileFalse: [
                  currtree := tempR1.
                     [currtree isLeaf == 1] whileFalse: [
                        (infileBits nextBit == 0)
                           ifTrue: [
                              currtree := currtree left.
                              ]
                           ifFalse: [
                              currtree := currtree right.
                              ].
                        ].
                  (currtree char == eof)
                      ifTrue: [
                         ^ 0.
                      ].
                  fileWrites << currtree char.
                  ].
               ]
            ifFalse: [
               tempT := Tree new: $x count: 0 debugg: debug 
                             left: tempL0 right: tempR1.
               leafPile push: tempT.
               ].
         ].
   ]. 
].

ObjectMemory quit: 0.
