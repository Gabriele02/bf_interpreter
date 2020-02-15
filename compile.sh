INITIALDIR=$PWD
if [ ! -d "obj" ]; then
  mkdir "$INITIALDIR"/obj
else
  cd "$INITIALDIR"/obj
  rm -rf *.*
fi

cd "$INITIALDIR"
if [ ! -d "bin" ]; then
  mkdir "$INITIALDIR"/bin
else
  cd "$INITIALDIR"/bin
  rm -rf *.*
fi

cd "$INITIALDIR"/obj
gcc ../DynamicArray.c ../BfInterpreter.c -c
cd ../bin
gcc ../obj/DynamicArray.o ../obj/BfInterpreter.o -o BfInterpreter
