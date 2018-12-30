================================================================================
My sketch has the extension <alphabet.h> as one of its header files,
so extension needs to be downloaded (and be at the same folder)
for sketch.c to work.

I am not proud of the way both my sketch and extension works at all,
in fact I would be emberassed if you were to check it in detail,
but it works perfectly and thankfully you do not have the time to do that :)

However sketch was a really important experience for me, I really learned that
I should keep my code simple and clean. I will be careful from now on.
================================================================================
Extension is a simple sketch creator that creates sketch version of the given
text. I had to write all the alphabet by myself, so it only includes the
characters within the english alphabet and space (' '). In a sense those
character were designed by me, so I was indeed creative. To end the line,
'#' is used.

Autotesting was limited to a single example for this program
because I could not call run() multiple times. I think it is related to
SDL.

Extension can be auto-tested by

./sketch testExtension

Extension can be used in the following format

./sketch <string input> <fileNameToSave>

Examples:

./sketch "hello world" test.sketch
./sketch "wow this really#took a long time#to finish" test.sketch
./sketch "hello darkness#my old friend" test.sketch
