INITIALDIR=$PWD
#Create objects dirs
if [ ! -d "obj" ]; then
  mkdir "$INITIALDIR"/obj
else
  cd "$INITIALDIR"/obj
  rm -rf *.*
fi

cd "$INITIALDIR"
if [ ! -d "obj/compiler" ]; then
  mkdir "$INITIALDIR"/obj/compiler
else
  cd "$INITIALDIR"/obj/compiler
  rm -rf *.*
fi

cd "$INITIALDIR"
if [ ! -d "obj/vm" ]; then
  mkdir "$INITIALDIR"/obj/vm
else
  cd "$INITIALDIR"/obj/vm
  rm -rf *.*
fi

#create bins dirs
cd "$INITIALDIR"
if [ ! -d "bin" ]; then
  mkdir "$INITIALDIR"/bin
else
  cd "$INITIALDIR"/bin
  rm -rf *.*
fi

cd "$INITIALDIR"
if [ ! -d "bin/compiler" ]; then
  mkdir "$INITIALDIR"/bin/compiler
else
  cd "$INITIALDIR"/bin/compiler
  rm -rf *.*
fi

cd "$INITIALDIR"
if [ ! -d "bin/vm" ]; then
  mkdir "$INITIALDIR"/bin/vm
else
  cd "$INITIALDIR"/bin/vm
  rm -rf *.*
fi

#compile interpreter
cd "$INITIALDIR"/obj
gcc ../Instructions.c ../DynamicArray.c ../BfInterpreter.c -c
cd ../bin
gcc ../obj/Instructions.o ../obj/DynamicArray.o ../obj/BfInterpreter.o -o BfInterpreter

#compile compiler
cd "$INITIALDIR"/obj/compiler
gcc ../../Instructions.c ../../DynamicArray.c ../../compiler/BfCompiler.c -c
cd "$INITIALDIR"/bin/compiler
gcc ../../obj/Instructions.o ../../obj/DynamicArray.o ../../obj/compiler/BfCompiler.o -o BfCompiler

#compile vm
cd "$INITIALDIR"/obj/vm
gcc ../../Instructions.c ../../DynamicArray.c ../../bfvm/BfVM.c -c
cd "$INITIALDIR"/bin/vm
gcc ../../obj/Instructions.o ../../obj/DynamicArray.o ../../obj/vm/BfVM.o -o BfVM
