#!/afs/cats.ucsc.edu/courses/cmps112-wm/usr/smalltalk/bin/gst -f
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
   atEnd [
      ^bitIndex = 1 and: [myStream atEnd]
   ]
]

nl := Character nl.
stdinBits := BitStream new: stdin.
count := 0.
[stdinBits atEnd] whileFalse: [
   stdout << stdinBits nextBit.
   count := count + 1.
   count = 4 ifTrue: [stdout << ' '].
   count = 8 ifTrue: [stdout << nl. count := 0].
]
